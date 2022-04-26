from pulse.study.bind.PatientVariability_pb2 import *
from pulse.cdm.bind.TestReport_pb2 import PropertyValidationListData, PropertyValidationData
from google.protobuf import json_format

from enum import Enum
from typing import NamedTuple
from os.path import exists

class ResultType(Enum):
    PASS = 0
    FAIL = 1
    MARGINAL = 2

class PatientValidationAnalysis(NamedTuple):
    byPatient: dict
    byProperty: dict
    stabilizationFailures: list

class SystemsAnalysis(NamedTuple):
    pass


def systemsValidation(patients, passThreshold = 10, failThreshold = 30 ):
    byPatient = {}
    byProperty = {}
    for patient in patients:
        # Only look at stabilized patients
        if not patient.AchievedStabilization:
            continue

        # Loop through all systems results
        for file in patient.ValidationMap:
            if file == "Patient" + str(patient.ID) + "ValidationResults.json":
                continue


def patientValidation(patients, passThreshold = 10, failThreshold = 30):
    stabilizationFailures = []
    byPatient = {}
    byProperty = {}
    for patient in patients:
        # Did the patient fail to stabilize
        if not patient.AchievedStabilization:
            stabilizationFailure.append(patient.ID)

        # Get patient validation results for this patient
        propertyList = patient.ValidationMap["Patient" + str(patient.ID) + "ValidationResults.json"]

        # Determine pass/fail for each property
        byPatient[patient.ID] = {ResultType.PASS: 0, ResultType.FAIL: 0, ResultType.MARGINAL: 0}
        for p in propertyList.Property:
            propertyName = p.Name
            error = p.Error
            if propertyName not in byProperty:
                byProperty[propertyName] = {ResultType.PASS: 0, ResultType.FAIL: 0, ResultType.MARGINAL: 0}

            # TODO: Only report change from standard patients instead of totals
            if abs(error) < passThreshold:
                byPatient[patient.ID][ResultType.PASS] += 1
                byProperty[propertyName][ResultType.PASS] += 1
            elif abs(error) < failThreshold:
                byPatient[patient.ID][ResultType.MARGINAL] += 1
                byProperty[propertyName][ResultType.MARGINAL] += 1
            else:
                byPatient[patient.ID][ResultType.FAIL] += 1
                byProperty[propertyName][ResultType.FAIL] += 1
    
    return PatientValidationAnalysis(byPatient, byProperty, stabilizationFailures)

def parseResults(patients):
    patientValidation(patients, 10, 30)
            

if __name__ == '__main__':
    # Load up result set
    results = PatientStateListData()
    results_file = "./test_results/patient_variability/patient_results.json"
    if not exists(results_file):
        results_file = "./test_results/patient_variability/patient_results.pbb"
        with open(results_file, "rb") as f:
            binary = f.read()
        results.ParseFromString(binary)
    else:
        with open(results_file) as f:
            json = f.read()
        json_format.Parse(json, results)

    patientAnalysis = patientValidation(results.Patient)
    print(patientAnalysis.byPatient)
    print(patientAnalysis.byProperty)
