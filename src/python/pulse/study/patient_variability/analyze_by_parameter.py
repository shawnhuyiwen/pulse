# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.study.patient_variability.analysis_utils import PatientVariabilityResults, Field

lineSep = "-------------------------------------------------------------------"

class PatientVariabilityAnalysis(PatientVariabilityResults):
    __slots__ = ["_standardMale", "_standardFemale"]

    def __init__(self, dir: str):
        super().__init__(dir)
        self._standardMale = self.createFilter(name="StandardMale")
        self._standardFemale = self.createFilter(name="StandardFemale")

    def process_fields_individually(self):

        for field in Field:
            print("Analyzing Standard Male's with any " + str(field))
            print(lineSep)
            self.analyzeSlice(self.sliceResults(field, self._standardMale))
            print("\n\n")

            print("Analyzing Standard Female's with any " + str(field))
            print(lineSep)
            self.analyzeSlice(self.sliceResults(field, self._standardFemale))
            print("\n\n")
    def analyzeSlice(self, s):
        # Just going to print out the base filename for each entry in the slice
        for d in s:
            print(d.OutputBaseFilename)

if __name__ == '__main__':
    analysis = PatientVariabilityAnalysis("./test_results/patient_variability/test/validation/")
    analysis.process_fields_individually()
