# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.Patient_pb2 import PatientData
from pulse.study.bind.PatientVariability_pb2 import PatientStateListData

from google.protobuf import json_format
from os.path import exists
from enum import Enum
import math
import operator
from typing import List, Dict, Union

class Field(Enum):
    Age_yr = 1
    Height_cm = 2
    BMI = 3
    HeartRate_bpm = 4
    MeanArterialPressure_mmHg = 5
    PulsePressure_mmHg = 6

class ConditionalType(Enum):
    AND = 1
    OR = 2

class Condition():
    _slots = ["field", "_operator", "_value"]

    def __init__(self, field:Field, operator, value):
        self._field = field
        self._operator = operator
        self._value = value

    def equal(self, frm, to):
        if not frm or not to:
            return False
        if not math.isclose(frm, to, abs_tol=0.0001):
            return False
        return True

    def compare(self, left, right, op):
        ops = {'>': operator.gt,
               '<': operator.lt,
               '>=': operator.ge,
               '<=': operator.le,
               '==': self.equal}

        return ops[op](left, right)

    def eval(self, patient):
        if self._field is Field.Age_yr:
            return self.compare(patient.Age_yr, self._value, self._operator)
        elif self._field is Field.Height_cm:
            return self.compare(patient.Height_cm, self._value, self._operator)
        elif self._field is Field.BMI:
            return self.compare(patient.BMI, self._value, self._operator)
        elif self._field is Field.HeartRate_bpm:
            return self.compare(patient.HeartRate_bpm, self._value, self._operator)
        elif self._field is Field.MeanArterialPressure_mmHg:
            return self.compare(patient.MeanArterialPressure_mmHg, self._value, self._operator)
        elif self._field is Field.PulsePressure_mmHg:
            return self.compare(patient.PulsePressure_mmHg, self._value, self._operator)

        return False

class Conditional():
    _slots = ["_conditions", "_conditionalType", "_sex"]

    def __init__(self, cType=ConditionalType.AND, sex:PatientData.eSex=PatientData.eSex.Male):
        self._conditions = []
        self._conditionalType = cType
        self._sex = PatientData.eSex.Male

    def sex(self, sex:PatientData.eSex):
        self._sex = sex

    def addCondition(self, field:Field, operator, value):
        self._conditions.append(Condition(field, operator, value))

    def addConditional(self, conditional):
        self._conditions.append(conditional)

    def eval(self, patient):
        if patient.Sex is not self._sex:
            return False
        if self._conditionalType is ConditionalType.AND:
            for condition in self._conditions:
                if not condition.eval(patient):
                    return False
            return True
        elif self._conditionalType is ConditionalType.OR:
            for condition in self._conditions:
                if condition.eval(patient):
                    return True
            return False

class PatientVariabilityResults():
    __slots__ = ["_results", "_results_dir"]

    def __init__(self, dir:str):
        self._results_dir = ""
        self._results = PatientStateListData()

        # Load up result set
        self._results_dir = dir
        resultsFile = self._results_dir + "patient_results.json"
        if not exists(resultsFile):
            resultsFile = self._results_dir + "patient_results.pbb"
            with open(resultsFile, "rb") as f:
                binary = f.read()
            self._results.ParseFromString(binary)
        else:
            with open(resultsFile) as f:
                json = f.read()
            json_format.Parse(json, self._results)

    def createFilterList(self):
        return PatientStateListData().Patient

    def sliceResults(self, field:Field, base):
        filters = self.createFilterList()
        filter = filters.add()
        filter.CopyFrom(base)
        if field is Field.Age_yr:
            filter.Age_yr = 0
        elif field is Field.Height_cm:
            filter.Height_cm = 0
        elif field is Field.BMI:
            filter.BMI = 0
        elif field is Field.HeartRate_bpm:
            filter.HeartRate_bpm = 0
        elif field is Field.MeanArterialPressure_mmHg:
            filter.MeanArterialPressure_mmHg = 0
        elif field is Field.PulsePressure_mmHg:
            filter.PulsePressure_mmHg = 0
        return self.filter(filters)

    # Identify parameter bounds of the given parameter in the given list of patients
    def getFieldValues(self, field:Field, sex: PatientData.eSex):
        v = []
        for r in self._results.Patient:
            if r.Sex is not sex:
                continue
            if field is Field.Age_yr:
                param = r.Age_yr
            elif field is Field.Height_cm:
                param = r.Height_cm
            elif field is Field.BMI:
                param = r.BMI
            elif field is Field.HeartRate_bpm:
                param = r.HeartRate_bpm
            elif field is Field.MeanArterialPressure_mmHg:
                param = r.MeanArterialPressure_mmHg
            elif field is Field.PulsePressure_mmHg:
                param = r.PulsePressure_mmHg
            if param not in v:
                v.append(param)
        v.sort()
        return v

    def equal(self, frm, to):
        if not frm or not to:
            return False
        if not math.isclose(frm, to, abs_tol=0.0001):
            return False
        return True

    # Generates list of patients that match any of the provided filters
    def filter(self, filters):
        filteredPatients = PatientStateListData()

        for patient in self._results.Patient:
            for filter in filters:
                if filter.Sex != patient.Sex: # Note: default value of sex is Male
                    continue
                if filter.Age_yr and not self.equal(filter.Age_yr, patient.Age_yr):
                    continue
                if filter.Height_cm and not self.equal(filter.Height_cm, patient.Height_cm):
                    continue
                if filter.BMI and not self.equal(filter.BMI, patient.BMI):
                    continue
                if filter.HeartRate_bpm and not self.equal(filter.HeartRate_bpm, patient.HeartRate_bpm):
                    continue
                if filter.MeanArterialPressure_mmHg and not self.equal(filter.MeanArterialPressure_mmHg, patient.MeanArterialPressure_mmHg):
                    continue
                if filter.PulsePressure_mmHg and not self.equal(filter.PulsePressure_mmHg, patient.PulsePressure_mmHg):
                    continue

                # Patient matched all set values of this filter.
                # Add it to the list and move on to the next patient.
                p = filteredPatients.Patient.add()
                p.CopyFrom(patient)
                break

        return filteredPatients.Patient

    # Generates list of patients that match any of the conditionals/conditions
    def conditionalFilter(self, conditionals:List[Union[Conditional, Condition]]):
        filteredPatients = PatientStateListData()

        for patient in self._results.Patient:
            for conditional in conditionals:
                if conditional.eval(patient):
                    # Patient passes this conditional.
                    # Add it to the list and move on to the next patient.
                    p = filteredPatients.Patient.add()
                    p.CopyFrom(patient)
                    break

        return filteredPatients.Patient

    # Create a filter from patient.
    # Provide patient name (e.g "StandardMale") for standard results
    # OR locate patient in given patients with ID
    def createFilter(self, name = None, id = None):
        # If name given, looking for a patient in standard results
        if name:
            standardResults = PatientStateListData()
            with open(self._results_dir + "standard_results.json") as f:
                json = f.read()
            json_format.Parse(json, standardResults)
            for patient in standardResults.Patient:
                if patient.OutputBaseFilename == name:
                    return patient
        # Otherwise looking for an ID in given patients data
        elif self._results and id is not None:
            for r in self._results:
                if r.ID == id:
                    return r

        # Could not locate patient with given arguments
        return None

if __name__ == '__main__':
    lineSep = "-------------------------------------------------------------------"
    results = PatientVariabilityResults("./test_results/patient_variability/test/validation/")

    # List all the ranges of our result set for males
    ages_yr = results.getFieldValues(Field.Age_yr, PatientData.eSex.Male)
    print("Male Age_yr: Min : {} Max : {}".format(ages_yr[0], ages_yr[-1]))
    heights_cm = results.getFieldValues(Field.Height_cm, PatientData.eSex.Male)
    print("Male Height_cm: Min : {} Max : {}".format(heights_cm[0], heights_cm[-1]))
    bmis = results.getFieldValues(Field.BMI, PatientData.eSex.Male)
    print("Male BMI: Min : {} Max : {}".format(bmis[0], bmis[-1]))
    heart_rates_bpm = results.getFieldValues(Field.HeartRate_bpm, PatientData.eSex.Male)
    print("Male HeartRate_bpm: Min : {} Max : {}".format(heart_rates_bpm[0], heart_rates_bpm[-1]))
    mean_arterial_pressures_mmHg = results.getFieldValues(Field.MeanArterialPressure_mmHg, PatientData.eSex.Male)
    print("Male MeanArterialPressure_mmHg: Min : {} Max : {}".format(mean_arterial_pressures_mmHg[0], mean_arterial_pressures_mmHg[-1]))
    pulse_pressures_mmHg = results.getFieldValues(Field.PulsePressure_mmHg, PatientData.eSex.Male)
    print("Male PulsePressure_mmHg: Min : {} Max : {}".format(pulse_pressures_mmHg[0], pulse_pressures_mmHg[-1]))

    # Note that if sex isn't specified, female patients will be filtered out.
    # Protobuf can't tell the difference between an unset value and a default value (male, 0, "", false)
    # You will need to add an additional filter with Female to get both sexes

    # Example: Get all patients that have a baseline HR of 72bpm
    filters = results.createFilterList()
    filterM = filters.add()
    filterM.Sex = PatientData.eSex.Male
    filterM.HeartRate_bpm = heart_rates_bpm[0]
    filterF = filters.add()
    filterF.CopyFrom(filterM)
    filterF.Sex = PatientData.eSex.Female
    filteredPatients_HR_bpm72 = results.filter(filters)
    print("\n\n\n72bpm patients:\n" + lineSep)
    for p in filteredPatients_HR_bpm72:
        print(p.OutputBaseFilename)

    # Example: Get all patients that match multiple fields
    filters = results.createFilterList()
    filter = filters.add()
    filter.BMI = bmis[0]
    filter.Height_cm = heights_cm[0]
    filteredPatients_age = results.filter(filters)
    print("\n\n\nBMI:{}, Height(cm):{}, filter:\n".format(bmis[0],heights_cm[0]) + lineSep)
    for p in filteredPatients_age:
        print(p.OutputBaseFilename)

    # Example: Get the runs where age is the only thing that could be different from the StandardMale
    filters = results.createFilterList()
    filter = filters.add()
    standardMaleFilter = results.createFilter(name="StandardMale")
    filter.CopyFrom(standardMaleFilter)
    filter.Age_yr = 0 # Reset any fields you want to vary
    filteredPatients_age2 = results.filter(filters)
    print("\n\n\nVary age from 'Standard Male' patient:\n" + lineSep)
    for p in filteredPatients_age2:
        print(p.OutputBaseFilename)

    # Example: Use conditional filtering (AND only)
    # male AND age < 45 yr AND height >= 165 cm AND height <= 185 cm AND pulse pressure = 40.5 mmHg
    conditional = Conditional()
    conditional.sex(PatientData.eSex.Male) # Assumes male if not provided
    conditional.addCondition(Field.Age_yr, '<', 45)
    conditional.addCondition(Field.Height_cm, '>=', 165)
    conditional.addCondition(Field.Height_cm, '<=', 185)
    conditional.addCondition(Field.PulsePressure_mmHg, '==', 40.5)
    filtered_conditionalA = results.conditionalFilter([conditional])
    print("\n\n\nMale AND age < 45 yr AND height >= 165 cm AND height <= 185 cm AND pulse pressure = 40.5 mmHg:\n" + lineSep)
    for p in filtered_conditionalA:
        print(p.OutputBaseFilename)

    # Example: Use conditional filtering (includes OR)
    # (implicit: male AND) BMI == 16 AND (HR < 65 bpm OR HR > 85 bpm)
    conditional = Conditional()
    conditional.addCondition(Field.BMI, '==', 16)
    conditional1 = Conditional(ConditionalType.OR)
    conditional1.addCondition(Field.HeartRate_bpm, '<', 65)
    conditional1.addCondition(Field.HeartRate_bpm, '>', 85)
    conditional.addConditional(conditional1)
    filteredPatients_conditionalB = results.conditionalFilter([conditional])
    print("\n\n\nMale AND BMI == 16 AND (HR < 65 bpm OR HR > 85 bpm):\n" + lineSep)
    for p in filteredPatients_conditionalB:
        print(p.OutputBaseFilename)
