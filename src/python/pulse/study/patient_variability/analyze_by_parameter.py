# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.Patient_pb2 import PatientData
from pulse.study.patient_variability.analysis_utils import PatientVariabilityResults, Conditional, Field

lineSep = "-------------------------------------------------------------------"

class PatientVariabilityAnalysis(PatientVariabilityResults):

    def __init__(self, dir: str):
        super().__init__(dir)

    def process(self):
        # Create queries and analyze them
        query = Conditional()
        query.sex(PatientData.eSex.Male)
        query.addCondition(Field.Age_yr, '<=', 45)
        results = self.conditionalFilter([query])
        for r in results:
            print(r.OutputBaseFilename + " Age_yr: " + str(r.SetupPatient.Age.ScalarTime.Value))
        print("Average RespirationRate(1/min) Error: " + str(self.averageError("RespirationRate(1/min)", results)))

    def averageError(self, name:str, results):
        properties = []
        for r in results:
            rr = self.getProperty(name, r.Validation)
            if rr is None:
                print("Could not find your property, check spelling/unit")
                return None
            else:
                properties.append(rr)
        if len(properties) > 0:
            rrError = 0
            for rr in properties:
                rrError = rrError + rr.Error
            rrError = rrError / len(properties)
            return rrError
        return None

if __name__ == '__main__':
    analysis = PatientVariabilityAnalysis("./test_results/patient_variability/validation/full/")
    analysis.process()
