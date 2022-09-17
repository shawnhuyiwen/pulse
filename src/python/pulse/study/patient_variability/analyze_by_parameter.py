# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.Patient_pb2 import PatientData
from pulse.study.patient_variability.analysis_utils import PatientVariabilityResults, Conditional, Field, PropertyError

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
        # The results are a map{"SystemName" : map{"PropertyName" : PropertyError()} }
        # The PropertyError class has an array, errors, of all the errors of that property in the query results
        # This condition filter will ONLY create PropertyName entries if the property is PatientSpecific

        # Let's go through and calculate 'stuff' for each property
        for system,properties in results.items():
            print("Examining system "+system)
            for property,property_error in properties.items():
                print("Examining Property" + property)
                self.analyzePropertyError(property_error)
                print("  Average Error: " + str(property_error.average_error))

    def analyzePropertyError(self, property_error: PropertyError):
        # We can dynamically add members to our property_error object
        property_error.average_error = 0
        for error in property_error.errors:
            property_error.average_error = property_error.average_error + error
        property_error.average_error = property_error.average_error / len(property_error.errors)

if __name__ == '__main__':
    analysis = PatientVariabilityAnalysis("./test_results/patient_variability/validation/full/")
    analysis.process()
