# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from typing import NamedTuple
from os.path import exists

basePath = "./test_results/patient_variability/validation/"

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

    # sex -> list of id's of patients that failed to stabilize
    stabilizationFailures: dict

    # id -> output base filename
    outputBaseFilenames: dict


# Results for System Validation, compared to standard baselines
class SystemsAnalysis(NamedTuple):
    # sex -> id -> ResultType
    byPatient: dict

    # sex -> system name -> property name -> ResultType
    bySystem: dict

    # sex -> system name -> ResultType
    systemTotals: dict


# Determine pass/fail/in-between from error
def determineResultType(error, passThreshold = 10, failThreshold = 30):
    if abs(error) < passThreshold:
        return ResultType.Pass
    elif abs(error) < failThreshold:
        return ResultType.Marginal
    else:
        return ResultType.Fail


# Zero-initialized dictionary of ResultTypes
def initResultsDict():
    return {ResultType.Pass: 0, ResultType.Fail: 0, ResultType.Marginal: 0}


# Dictonary of empty dictionaries for each sex
def initSexDict():
    return {PatientData.eSex.Male: {}, PatientData.eSex.Female: {}}


# Determine result of standard paitent for given criteria
def getStandardResult(standardPatient, system, propertyName, passThreshold = 10, failThreshold = 30):
    filename = system + "-" + standardPatient.OutputBaseFilename + "ValidationResults.json"
    propertyList = standardPatient.Validation.ValidationMap[filename]
    for p in propertyList.Property:
        if p.Name == propertyName:
            return determineResultType(p.Error, passThreshold, failThreshold)


# Process system validation data
def systemValidation(patients, passThreshold = 10, failThreshold = 30 ):

    # Load standard patient results for comparison
    standardResults = PatientStateListData()
    with open(basePath + "standard_results.json") as f:
        json = f.read()
    json_format.Parse(json, standardResults)
    standard = {}
    for patient in standardResults.Patient:
        if patient.OutputBaseFilename == "StandardMale":
            standard[PatientData.eSex.Male] = patient
        elif patient.OutputBaseFilename == "StandardFemale":
            standard[PatientData.eSex.Female] = patient

    byPatient = initSexDict()
    bySystem = initSexDict()
    systemTotals = initSexDict()
    for patient in patients:
        # Only look at stabilized patients
        if not patient.AchievedStabilization:
            continue

        # Loop through all system results
        byPatient[patient.Sex][patient.ID] = initResultsDict()
        for file in patient.Validation.ValidationMap:
            # Patient validation taken care of separately
            if file == "Patient" + str(patient.ID) + "ValidationResults.json":
                continue
            
            # Extract system from file name
            system = file[:(file.find("-Patient" + str(patient.ID) + "ValidationResults.json"))]
            if system not in bySystem[patient.Sex]:
                bySystem[patient.Sex][system] = {}
                systemTotals[patient.Sex][system] = initResultsDict()

            # Iterate over every property in each system
            propertyList = patient.Validation.ValidationMap[file]
            for p in propertyList.Property:
                propertyName = p.Name
                error = p.Error

                if propertyName not in bySystem[patient.Sex][system]:
                    bySystem[patient.Sex][system][propertyName] = initResultsDict()

                # Did this result get worse compared to the standard patient?
                standardResult = getStandardResult(standard[patient.Sex], system, propertyName, passThreshold, failThreshold)
                result = determineResultType(error, passThreshold, failThreshold)
                if result.value > standardResult.value:
                    byPatient[patient.Sex][patient.ID][result] += 1
                    bySystem[patient.Sex][system][propertyName][result] += 1
                    systemTotals[patient.Sex][system][result] += 1
                    byPatient[patient.Sex][patient.ID][standardResult] -= 1
                    bySystem[patient.Sex][system][propertyName][standardResult] -= 1
                    systemTotals[patient.Sex][system][standardResult] -= 1

    return SystemsAnalysis(byPatient, bySystem, systemTotals)

# Process patient validation data
def patientValidation(patients, passThreshold = 10, failThreshold = 30):
    stabilizationFailures = {PatientData.eSex.Male: [], PatientData.eSex.Female: []}
    outputBaseFilenames = initSexDict()
    byPatient = initSexDict()
    byProperty = initSexDict()
    for patient in patients:
        # Did the patient fail to stabilize
        if not patient.AchievedStabilization:
            stabilizationFailures[patient.Sex].append(patient.ID)

        # Add output base filename to describe patient
        outputBaseFilenames[patient.Sex][patient.ID] = patient.OutputBaseFilename

        # Get patient validation results for this patient
        propertyList = patient.Validation.ValidationMap["Patient" + str(patient.ID) + "ValidationResults.json"]

        # Determine pass/fail for each property
        byPatient[patient.Sex][patient.ID] = initResultsDict()
        for p in propertyList.Property:
            propertyName = p.Name
            error = p.Error

            if propertyName not in byProperty[patient.Sex]:
                byProperty[patient.Sex][propertyName] = initResultsDict()

            result = determineResultType(error, passThreshold, failThreshold)
            byPatient[patient.Sex][patient.ID][result] += 1
            byProperty[patient.Sex][propertyName][result] += 1
    
    return PatientValidationAnalysis(byPatient, byProperty, stabilizationFailures, outputBaseFilenames)

# Analysis driver
def analyze(patients, passThreshold = 10, failThreshold = 30):
    patientAnalysis = patientValidation(patients, passThreshold, failThreshold)
    systemsAnalysis = systemValidation(patients, passThreshold, failThreshold)
    writeAnalysis(patientAnalysis, systemsAnalysis)


def writeAnalysis(patientAnalysis, systemsAnalysis):
    # Separate everything by sex
    sexLabels = {PatientData.eSex.Male: "Male", PatientData.eSex.Female: "Female"}
    for sex in sexLabels.keys():
        baseFilenames = patientAnalysis.outputBaseFilenames[sex]
        PAbyPatient = patientAnalysis.byPatient[sex]
        PAbyProperty = patientAnalysis.byProperty[sex]
        SAbyPatient = systemsAnalysis.byPatient[sex]
        SAbySystem = systemsAnalysis.bySystem[sex]
        SAtotals = systemsAnalysis.systemTotals[sex]

        # Determine stabilized and unstabilized counts
        totalPatientsRun = len(PAbyPatient)
        numStabilizationFailures = len(patientAnalysis.stabilizationFailures[sex])
        numStabilizedPatients = totalPatientsRun - numStabilizationFailures

        # Patient validation by patient
        spacer = "---------------------------------------------------------------------------------------------------------------------------\n"
        with open(basePath + "PatientValidationByPatient-" + sexLabels[sex] + ".txt", "w") as f:
            f.write("Stabilized {} of {} patients (failed to stabilize {} patients).\n".format(numStabilizedPatients, totalPatientsRun, numStabilizationFailures))
            f.write("\nPatient Validation Results (by patient) -- " + sexLabels[sex] + " [" + str(len(PAbyProperty)) + " expected]\n" + spacer)
            format_row = "{:>12}" * 4 + "{:>75}" + "\n"
            f.write(format_row.format("ID", "Green", "Yellow", "Red", "Base Filename"))
            for p in sorted(PAbyPatient.keys(), key=lambda x: (PAbyPatient[x][ResultType.Pass], -PAbyPatient[x][ResultType.Fail], -PAbyPatient[x][ResultType.Marginal])):
                patient = PAbyPatient[p]
                stabilizedStr = ""
                if p in patientAnalysis.stabilizationFailures[sex]:
                    stabilizedStr = "*"
                f.write(format_row.format(stabilizedStr+str(p), patient[ResultType.Pass], patient[ResultType.Marginal], patient[ResultType.Fail], baseFilenames[p]))

        # Patient validation summary by property
        spacer = "----------------------------------------------------------------------------\n"
        total = initResultsDict()
        with open(basePath + "PatientValidationSummary-" + sexLabels[sex] + ".txt", "w") as f:
            f.write("Stabilized {} of {} patients (failed to stabilize {} patients).\n".format(numStabilizedPatients, totalPatientsRun, numStabilizationFailures))
            f.write("\nPatient Validation Results (by property) -- " + sexLabels[sex] + "\n" + spacer)
            format_row = "{:>40}" + ("{:>12}" * 3) + "\n"
            f.write(format_row.format("Property", "Green", "Yellow", "Red"))
            for p in sorted(PAbyProperty.keys(), key=lambda x: (PAbyProperty[x][ResultType.Pass], -PAbyProperty[x][ResultType.Fail], -PAbyProperty[x][ResultType.Marginal])):
                prop = PAbyProperty[p]
                total[ResultType.Pass] += prop[ResultType.Pass]
                total[ResultType.Marginal] += prop[ResultType.Marginal]
                total[ResultType.Fail] += prop[ResultType.Fail]
                f.write(format_row.format(p, prop[ResultType.Pass], prop[ResultType.Marginal], prop[ResultType.Fail]))
            f.write("\n" + format_row.format("Total", total[ResultType.Pass], total[ResultType.Marginal], total[ResultType.Fail]) + "\n")

        # System validation by patient
        spacer = "---------------------------------------------------------------------------------------------------------------------------\n"
        with open(basePath + "SystemValidationByPatient-" + sexLabels[sex] + ".txt", "w") as f:
            f.write("****Only stabilized patients are included in results below.****\n".format(numStabilizedPatients))
            f.write("Values indicate degradation from standard patient baseline.\n")
            f.write("\nSystem Validation Results (by patient) -- " + sexLabels[sex] + "\n" + spacer)
            format_row = "{:>12}" * 4 + "{:>75}" + "\n"
            f.write(format_row.format("ID", "Green", "Yellow", "Red", "Base Filename"))
            for p in sorted(SAbyPatient.keys(), key=lambda x: (SAbyPatient[x][ResultType.Pass], -SAbyPatient[x][ResultType.Fail], -SAbyPatient[x][ResultType.Marginal])):
                patient = SAbyPatient[p]
                f.write(format_row.format(p, patient[ResultType.Pass], patient[ResultType.Marginal], patient[ResultType.Fail], baseFilenames[p]))

        # System validation summary by systems/properties
        spacer = "---------------------------------------------------------------------------------------------------------\n"
        total = initResultsDict()
        with open(basePath + "SystemValidationSummary-" + sexLabels[sex] + ".txt", "w") as f:
            f.write("****Following results based on the {} stabilized patients****\n".format(numStabilizedPatients))
            f.write("Values indicate degradation from standard patient baseline.\n")
            
            # System validation summaries
            f.write("\nSystem Validation Results (by System) -- " + sexLabels[sex] + "\n" + spacer)
            format_row = "{:>40}" + ("{:>15}" * 3) + "\n"
            f.write(format_row.format("System", "Green", "Yellow", "Red"))
            for s in sorted(SAtotals.keys(), key=lambda x: (SAtotals[x][ResultType.Pass], -SAtotals[x][ResultType.Fail], -SAtotals[x][ResultType.Marginal])):
                total[ResultType.Pass] += SAtotals[s][ResultType.Pass]
                total[ResultType.Marginal] += SAtotals[s][ResultType.Marginal]
                total[ResultType.Fail] += SAtotals[s][ResultType.Fail]
                f.write(format_row.format(s + " (overall)", SAtotals[s][ResultType.Pass], SAtotals[s][ResultType.Marginal], SAtotals[s][ResultType.Fail]))
            f.write("\n" + format_row.format("Total", total[ResultType.Pass], total[ResultType.Marginal], total[ResultType.Fail]) + "\n")

            # System-specific validation by properties
            format_row = "{:>60}" + ("{:>15}" * 3) + "\n"
            for s in sorted(SAbySystem.keys()):
                sys = SAbySystem[s]
                f.write("\n" + s + " -- " + sexLabels[sex] + "\n" + spacer)
                f.write(format_row.format("Property", "Green", "Yellow", "Red"))
                for prop in sorted(sys.keys(), key=lambda x: (sys[x][ResultType.Pass], -sys[x][ResultType.Fail], -sys[x][ResultType.Marginal])):
                    f.write(format_row.format(prop, sys[prop][ResultType.Pass], sys[prop][ResultType.Marginal], sys[prop][ResultType.Fail]))
                f.write("\n" + format_row.format("Total", SAtotals[s][ResultType.Pass], SAtotals[s][ResultType.Marginal], SAtotals[s][ResultType.Fail]) + "\n")


       
if __name__ == '__main__':
    # Load up result set
    results = PatientStateListData()
    resultsFile = basePath + "patient_results.json"
    if not exists(resultsFile):
        resultsFile = basePath + "patient_results.pbb"
        with open(resultsFile, "rb") as f:
            binary = f.read()
        results.ParseFromString(binary)
    else:
        with open(resultsFile) as f:
            json = f.read()
        json_format.Parse(json, results)

    # Process results
    analyze(results.Patient, 10, 30)
    
