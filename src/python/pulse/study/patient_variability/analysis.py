# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.Patient_pb2 import PatientData
from pulse.study.bind.PatientVariability_pb2 import PatientStateListData, PatientStateData
from pulse.study.patient_variability.analyze_by_parameter import PatientVariabilityAnalysis

from google.protobuf import json_format
from enum import Enum
from typing import NamedTuple
from os.path import exists

class PatientVariabilityTextAnalysis(PatientVariabilityAnalysis):
    # Possible result types
    class ResultType(Enum):
        # Note this order matters for comparison
        Pass = 0
        Marginal = 1
        Fail = 2


    # Results for Patient Validation, not compared to standard patients
    class PatientValidationAnalysis(NamedTuple):
        # sex -> id -> ResultType
        byPatient: dict

        # sex -> property name -> ResultType
        byProperty: dict

        # failure type -> sex -> list of id's of patients that experienced failure
        failures: dict

        # id -> str description of patient
        descriptors: dict


    # Results for System Validation, compared to standard baselines
    class SystemsAnalysis(NamedTuple):
        # sex -> id -> ResultType
        byPatient: dict

        # sex -> system name -> property name -> ResultType
        bySystem: dict

        # sex -> system name -> ResultType
        systemTotals: dict


    # Determine pass/fail/in-between from error
    def determineResultType(self, error):
        if abs(error) < self.passErrors[0]:
            return self.ResultType.Pass
        elif abs(error) < self.passErrors[1]:
            return self.ResultType.Marginal
        else:
            return self.ResultType.Fail

    # Zero-initialized dictionary of ResultTypes
    def initResultsDict(self):
        return {self.ResultType.Pass: 0, self.ResultType.Fail: 0, self.ResultType.Marginal: 0}

    # Dictonary of empty dictionaries for each sex
    def initSexDict(self):
        return {PatientData.eSex.Male: {}, PatientData.eSex.Female: {}}

    # Generate patient descriptor string
    def getPatientDescriptor(self, p):
        out = ("male" if p.Sex == PatientData.eSex.Male else "female")
        out += "/{}({})".format(p.Age.ScalarTime.Value, p.Age.ScalarTime.Unit)
        out += "/{:.2f}({})".format(p.Height.ScalarLength.Value, p.Height.ScalarLength.Unit)
        out += "/{:.2f}".format(p.BodyMassIndex.Value)
        out += "/{:.2f}".format(p.BodyFatFraction.Scalar0To1.Value)
        out += "/{:.2f}({})".format(p.HeartRateBaseline.ScalarFrequency.Value, p.HeartRateBaseline.ScalarFrequency.Unit)
        out += "/{:.2f}({})".format(p.MeanArterialPressureBaseline.ScalarPressure.Value, p.MeanArterialPressureBaseline.ScalarPressure.Unit)
        out += "/{:.2f}({})".format(p.PulsePressureBaseline.ScalarPressure.Value, p.PulsePressureBaseline.ScalarPressure.Unit)
        out += "/{:.2f}({})".format(p.RespirationRateBaseline.ScalarFrequency.Value, p.RespirationRateBaseline.ScalarFrequency.Unit)
        return out

    # Determine result of standard paitent for given criteria
    def getStandardResult(self, standardPatient, system, propertyName):
        filename = system + "-" + standardPatient.Patient.Name + "-ValidationResults.json"
        propertyList = standardPatient.Validation.ValidationMap[filename]
        for p in propertyList.Property:
            if p.Name == propertyName:
                return self.determineResultType(p.Error)

    # Process system validation data
    def systemValidation(self):
        # Load standard patient results for comparison
        standardResults = PatientStateListData()
        standardFile = self._results_dir + "standard_results.json"
        standard = {}
        if exists(standardFile):
            with open(standardFile) as f:
                json = f.read()
            json_format.Parse(json, standardResults)
            for patientState in standardResults.PatientState:
                if patientState.OutputBaseFilename == "StandardMale":
                    standard[PatientData.eSex.Male] = patientState
                elif patientState.OutputBaseFilename == "StandardFemale":
                    standard[PatientData.eSex.Female] = patientState
        else: # Attempt to get standard results from full results
            stdMaleQuery = self.standardQuery(PatientData.eSex.Male)
            if not stdMaleQuery['ids']:
                print("ERROR: Could not locate standard male results")
            else:
                standard[PatientData.eSex.Male] = self.getPatientState(stdMaleQuery['ids'][0])

            stdFemaleQuery = self.standardQuery(PatientData.eSex.Female)
            if not stdFemaleQuery['ids']:
                print("ERROR: Could not locate standard female results")
            else:
                standard[PatientData.eSex.Female] = self.getPatientState(stdFemaleQuery['ids'][0])

        byPatient = self.initSexDict()
        bySystem = self.initSexDict()
        systemTotals = self.initSexDict()
        for patientState in self._results.PatientState:
            # Only look at patients without failure
            if patientState.Failure:
                continue

            sex = patientState.Patient.Sex
            ID = patientState.ID

            # Loop through all system results
            byPatient[sex][ID] = self.initResultsDict()
            for file in patientState.Validation.ValidationMap:
                # Patient validation taken care of separately
                if file.startswith("Patient"):
                    continue
                
                # Extract system from file name
                system = file[:file.find("-")]
                if system not in bySystem[sex]:
                    bySystem[sex][system] = {}
                    systemTotals[sex][system] = self.initResultsDict()

                # Iterate over every property in each system
                propertyList = patientState.Validation.ValidationMap[file]
                for p in propertyList.Property:
                    propertyName = p.Name
                    error = p.Error

                    if propertyName not in bySystem[sex][system]:
                        bySystem[sex][system][propertyName] = self.initResultsDict()

                    # Did this result get worse compared to the standard patient?
                    standardResult = self.getStandardResult(standard[sex], system, propertyName)
                    result = self.determineResultType(error)
                    if standardResult is not None and result.value > standardResult.value:
                        byPatient[sex][ID][result] += 1
                        bySystem[sex][system][propertyName][result] += 1
                        systemTotals[sex][system][result] += 1
                        byPatient[sex][ID][standardResult] -= 1
                        bySystem[sex][system][propertyName][standardResult] -= 1
                        systemTotals[sex][system][standardResult] -= 1

        return self.SystemsAnalysis(byPatient, bySystem, systemTotals)

    # Process patient validation data
    def patientValidation(self):
        failures= {
                    PatientStateData.eFailure.FailedSetup: {PatientData.eSex.Male: [], PatientData.eSex.Female: []},
                    PatientStateData.eFailure.FailedStabilization: {PatientData.eSex.Male: [], PatientData.eSex.Female: []},
                    PatientStateData.eFailure.RuntimeError: {PatientData.eSex.Male: [], PatientData.eSex.Female: []},
                    PatientStateData.eFailure.FatalRuntimeError: {PatientData.eSex.Male: [], PatientData.eSex.Female: []}
                    }
        byPatient = self.initSexDict()
        byProperty = self.initSexDict()
        descriptors = self.initSexDict()
        for patientState in self._results.PatientState:
            sex = patientState.Patient.Sex
            ID = patientState.ID

            # Did the patient fail to stabilize
            if patientState.Failure:
                failures[patientState.Failure][sex].append(ID)

            descriptors[sex][ID] = self.getPatientDescriptor(patientState.Patient)

            # Get patient validation results for this patient
            patientFile = "Patient-Patient" + str(ID) + "-ValidationResults.json"
            if "Patient--ValidationResults.json" in patientState.Validation.ValidationMap.keys(): # Solo runs
                patientFile = "Patient--ValidationResults.json"
            propertyList = patientState.Validation.ValidationMap[patientFile]

            # Determine pass/fail for each property
            byPatient[sex][ID] = self.initResultsDict()
            for p in propertyList.Property:
                propertyName = p.Name
                error = p.Error

                if propertyName not in byProperty[sex]:
                    byProperty[sex][propertyName] = self.initResultsDict()

                result = self.determineResultType(error)
                byPatient[sex][ID][result] += 1
                byProperty[sex][propertyName][result] += 1
        
        return self.PatientValidationAnalysis(byPatient, byProperty, failures, descriptors)

    # Analysis driver
    def process(self):
        patientAnalysis = self.patientValidation()
        systemsAnalysis = self.systemValidation()
        self.writeAnalysis(patientAnalysis, systemsAnalysis)

    def writeAnalysis(self, patientAnalysis, systemsAnalysis):
        # Separate everything by sex
        sexLabels = {PatientData.eSex.Male: "Male", PatientData.eSex.Female: "Female"}
        for sex in sexLabels.keys():
            descriptors = patientAnalysis.descriptors[sex]
            PAbyPatient = patientAnalysis.byPatient[sex]
            PAbyProperty = patientAnalysis.byProperty[sex]
            SAbyPatient = systemsAnalysis.byPatient[sex]
            SAbySystem = systemsAnalysis.bySystem[sex]
            SAtotals = systemsAnalysis.systemTotals[sex]

            # Determine stabilized and unstabilized counts
            totalPatientsRun = len(PAbyPatient)
            numStabilizationFailures = len(patientAnalysis.failures[PatientStateData.eFailure.FailedStabilization][sex])
            numSetupFailures = len(patientAnalysis.failures[PatientStateData.eFailure.FailedSetup][sex])
            numStabilizedPatients = totalPatientsRun - numStabilizationFailures - numSetupFailures

            # Patient validation by patient
            spacer = "-" * 148 + "\n"
            with open(self._results_dir + "PatientValidationByPatient-" + sexLabels[sex] + ".txt", "w") as f:
                f.write("Stabilized {} of {} set up patients (failed to stabilize {} patients [*], failed to set up {} patients [^]).\n".format(numStabilizedPatients, totalPatientsRun - numSetupFailures, numStabilizationFailures, numSetupFailures))
                f.write("\nPatient Validation Results (by patient) -- " + sexLabels[sex] + " [" + str(len(PAbyProperty)) + " expected]\n" + spacer)
                format_row = "{:>12}" * 4 + "{:>100}" + "\n"
                f.write(format_row.format("ID", "Green", "Yellow", "Red", "Descriptor"))
                for p in sorted(PAbyPatient.keys(), key=lambda x: (PAbyPatient[x][self.ResultType.Pass], -PAbyPatient[x][self.ResultType.Fail], -PAbyPatient[x][self.ResultType.Marginal])):
                    patient = PAbyPatient[p]
                    stabilizedStr = ""
                    if p in patientAnalysis.failures[PatientStateData.eFailure.FailedSetup][sex]:
                        stabilizedStr = "^"
                    elif p in patientAnalysis.failures[PatientStateData.eFailure.FailedStabilization][sex]:
                        stabilizedStr = "*"
                    f.write(format_row.format(stabilizedStr+str(p), patient[self.ResultType.Pass], patient[self.ResultType.Marginal], patient[self.ResultType.Fail], descriptors[p]))

            # Patient validation summary by property
            spacer = "-" * 96 + "\n"
            total = self.initResultsDict()
            with open(self._results_dir + "PatientValidationSummary-" + sexLabels[sex] + ".txt", "w") as f:
                f.write("Stabilized {} of {} set up patients (failed to stabilize {} patients, failed to set up {} patients).\n".format(numStabilizedPatients, totalPatientsRun - numSetupFailures, numStabilizationFailures, numSetupFailures))
                f.write("\nPatient Validation Results (by property) -- " + sexLabels[sex] + "\n" + spacer)
                format_row = "{:>60}" + ("{:>12}" * 3) + "\n"
                f.write(format_row.format("Property", "Green", "Yellow", "Red"))
                for p in sorted(PAbyProperty.keys(), key=lambda x: (PAbyProperty[x][self.ResultType.Pass], -PAbyProperty[x][self.ResultType.Fail], -PAbyProperty[x][self.ResultType.Marginal])):
                    prop = PAbyProperty[p]
                    total[self.ResultType.Pass] += prop[self.ResultType.Pass]
                    total[self.ResultType.Marginal] += prop[self.ResultType.Marginal]
                    total[self.ResultType.Fail] += prop[self.ResultType.Fail]
                    f.write(format_row.format(p, prop[self.ResultType.Pass], prop[self.ResultType.Marginal], prop[self.ResultType.Fail]))
                f.write("\n" + format_row.format("Total", total[self.ResultType.Pass], total[self.ResultType.Marginal], total[self.ResultType.Fail]) + "\n")

            # System validation by patient
            spacer = "-" * 148 + "\n"
            with open(self._results_dir + "SystemValidationByPatient-" + sexLabels[sex] + ".txt", "w") as f:
                f.write("****Only stabilized patients are included in results below.****\n".format(numStabilizedPatients))
                f.write("Values indicate degradation from standard patient baseline.\n")
                f.write("\nSystem Validation Results (by patient) -- " + sexLabels[sex] + "\n" + spacer)
                format_row = "{:>12}" * 4 + "{:>100}" + "\n"
                f.write(format_row.format("ID", "Green", "Yellow", "Red", "Descriptor"))
                for p in sorted(SAbyPatient.keys(), key=lambda x: (SAbyPatient[x][self.ResultType.Pass], -SAbyPatient[x][self.ResultType.Fail], -SAbyPatient[x][self.ResultType.Marginal])):
                    patient = SAbyPatient[p]
                    f.write(format_row.format(p, patient[self.ResultType.Pass], patient[self.ResultType.Marginal], patient[self.ResultType.Fail], descriptors[p]))

            # System validation summary by systems/properties
            total = self.initResultsDict()
            with open(self._results_dir + "SystemValidationSummary-" + sexLabels[sex] + ".txt", "w") as f:
                f.write("****Following results based on the {} stabilized patients****\n".format(numStabilizedPatients))
                f.write("Values indicate degradation from standard patient baseline.\n")
                
                # System validation summaries
                spacer = "-" * 85 + "\n"
                f.write("\nSystem Validation Results (by System) -- " + sexLabels[sex] + "\n" + spacer)
                format_row = "{:>40}" + ("{:>15}" * 3) + "\n"
                f.write(format_row.format("System", "Green", "Yellow", "Red"))
                for s in sorted(SAtotals.keys(), key=lambda x: (SAtotals[x][self.ResultType.Pass], -SAtotals[x][self.ResultType.Fail], -SAtotals[x][self.ResultType.Marginal])):
                    total[self.ResultType.Pass] += SAtotals[s][self.ResultType.Pass]
                    total[self.ResultType.Marginal] += SAtotals[s][self.ResultType.Marginal]
                    total[self.ResultType.Fail] += SAtotals[s][self.ResultType.Fail]
                    f.write(format_row.format(s + " (overall)", SAtotals[s][self.ResultType.Pass], SAtotals[s][self.ResultType.Marginal], SAtotals[s][self.ResultType.Fail]))
                f.write("\n" + format_row.format("Total", total[self.ResultType.Pass], total[self.ResultType.Marginal], total[self.ResultType.Fail]) + "\n")

                # System-specific validation by properties
                spacer = "-" * 115 + "\n"
                format_row = "{:>70}" + ("{:>15}" * 3) + "\n"
                for s in sorted(SAbySystem.keys()):
                    sys = SAbySystem[s]
                    f.write("\n" + s + " -- " + sexLabels[sex] + "\n" + spacer)
                    f.write(format_row.format("Property", "Green", "Yellow", "Red"))
                    for prop in sorted(sys.keys(), key=lambda x: (sys[x][self.ResultType.Pass], -sys[x][self.ResultType.Fail], -sys[x][self.ResultType.Marginal])):
                        f.write(format_row.format(prop, sys[prop][self.ResultType.Pass], sys[prop][self.ResultType.Marginal], sys[prop][self.ResultType.Fail]))
                    f.write("\n" + format_row.format("Total", SAtotals[s][self.ResultType.Pass], SAtotals[s][self.ResultType.Marginal], SAtotals[s][self.ResultType.Fail]) + "\n")

if __name__ == '__main__':
    analysis = PatientVariabilityTextAnalysis("./test_results/patient_variability/validation/full/")
    analysis.process()
