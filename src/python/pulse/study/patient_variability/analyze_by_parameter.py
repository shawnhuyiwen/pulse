# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.Patient_pb2 import PatientData
from pulse.study.patient_variability.analysis_utils import PatientVariabilityResults, Conditional, Field, PropertyError

import plotly.graph_objects as go
import plotly.offline as pyo


minAge_yr = 18
stdAge_yr = 44
maxAge_yr = 65
minMaleHeight_cm = 163
stdMaleHeight_cm = 180.34
maxMaleHeight_cm = 190
minFemaleHeight_cm = 151
stdFemaleHeight_cm = 162.56
maxFemaleHeight_cm = 175.5
minBMI = 16
stdMaleBMI = 23.71
stdFemaleBMI = 22.31
maxBMI = 29.9
minMaleBFF = 0.02
stdMaleBFF = 0.21
maxMaleBFF = 0.25
minFemaleBFF = 0.1
stdFemaleBFF = 0.28
maxFemaleBFF = 0.32
minHR_bpm = 60
stdHR_bpm = 72
maxHR_bpm = 100
minMAP_mmHg = 70
stdMAP_mmHg = 87
maxMAP_mmHg = 100
minPulsePressure_mmHg = 30
stdPulsePressure_mmHg = 40.5
maxPulsePressure_mmHg = 60
minRR_bpm = 8
stdRR_bpm = 12
maxRR_bpm = 20

mapAllowableErrorFraction = 0.1
stdMapMin_mmHg = stdMAP_mmHg - stdMAP_mmHg * mapAllowableErrorFraction
stdMapMax_mmHg = stdMAP_mmHg + stdMAP_mmHg * mapAllowableErrorFraction
pulsePressureAllowableErrorFraction = 0.1
stdPulsePressureMin_mmHg = stdPulsePressure_mmHg - stdPulsePressure_mmHg * pulsePressureAllowableErrorFraction
stdPulsePressureMax_mmHg = stdPulsePressure_mmHg + stdPulsePressure_mmHg * pulsePressureAllowableErrorFraction

lineSep = "-------------------------------------------------------------------"


class PatientVariabilityAnalysis(PatientVariabilityResults):

    def __init__(self, dir: str):
        super().__init__(dir)

    def exampleProcess(self):
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

    def process(self):
        analysis.createRadarCharts()

    def createRadarCharts(self):
        # Standard male
        results = analysis.standardMaleQuery()

        # Calculate pass rate for each system
        radarCategories = []
        for system,properties in results.items():
            radarCategories.append(system)

        passError = 0.1
        radarStandardMaleValues = analysis.calculateSystemPassRate(results, passError, radarCategories)

        #Standard female
        results = analysis.standardFemaleQuery()

        # Calculate pass rate for each system
        radarStandardFemaleValues = analysis.calculateSystemPassRate(results, passError, radarCategories)

        # Nonstandard male query
        # To-Do

        # Nonstandard female query
        # To-Do

        # Generate radar chart
        radarCategories = [*radarCategories, radarCategories[0]]

        radarStandardMaleValues = [*radarStandardMaleValues, radarStandardMaleValues[0]]
        radarStandardFemaleValues = [*radarStandardFemaleValues, radarStandardFemaleValues[0]]

        fig = go.Figure(
            data=[
                go.Scatterpolar(r=radarStandardMaleValues, theta=radarCategories, name='Standard Male'),
                go.Scatterpolar(r=radarStandardFemaleValues, theta=radarCategories, name='Standard Female')
            ],
            layout=go.Layout(
                title=go.layout.Title(text='Physiology System Pass Rate'),
                polar={'radialaxis': {'visible': True}},
                showlegend=True
            )
        )

        pyo.plot(fig)

    def analyzePropertyError(self, property_error: PropertyError):
        # We can dynamically add members to our property_error object
        property_error.average_error = 0
        for error in property_error.errors:
            property_error.average_error = property_error.average_error + error
        property_error.average_error = property_error.average_error / len(property_error.errors)

    def standardMaleQuery(self):
        print("Standard male query")
        query = Conditional()
        query.sex(PatientData.eSex.Male)
        query.addCondition(Field.Age_yr, '==', stdAge_yr)
        query.addCondition(Field.Height_cm, '==', stdMaleHeight_cm)
        query.addCondition(Field.BodyMassIndex, '==', stdMaleBMI)
        query.addCondition(Field.BodyFatFraction, '==', stdMaleBFF)
        query.addCondition(Field.HeartRateBaseline_bpm, '==', stdHR_bpm)
        query.addCondition(Field.MeanArterialPressureBaseline_mmHg, '>=', stdMapMin_mmHg)
        query.addCondition(Field.MeanArterialPressureBaseline_mmHg, '<=', stdMapMax_mmHg)
        query.addCondition(Field.PulsePressureBaseline_mmHg, '>=', stdPulsePressureMin_mmHg)
        query.addCondition(Field.PulsePressureBaseline_mmHg, '<=', stdPulsePressureMax_mmHg)
        query.addCondition(Field.RespirationRateBaseline_bpm, '==', stdRR_bpm)
        results = self.conditionalFilter([query])
        return results

    def standardFemaleQuery(self):
        print("Standard female query")
        query = Conditional()
        query.sex(PatientData.eSex.Female)
        query.addCondition(Field.Age_yr, '==', stdAge_yr)
        query.addCondition(Field.Height_cm, '==', stdFemaleHeight_cm)
        query.addCondition(Field.BodyMassIndex, '==', stdFemaleBMI)
        query.addCondition(Field.BodyFatFraction, '==', stdFemaleBFF)
        query.addCondition(Field.HeartRateBaseline_bpm, '==', stdHR_bpm)
        query.addCondition(Field.MeanArterialPressureBaseline_mmHg, '>=', stdMapMin_mmHg)
        query.addCondition(Field.MeanArterialPressureBaseline_mmHg, '<=', stdMapMax_mmHg)
        query.addCondition(Field.PulsePressureBaseline_mmHg, '>=', stdPulsePressureMin_mmHg)
        query.addCondition(Field.PulsePressureBaseline_mmHg, '<=', stdPulsePressureMax_mmHg)
        query.addCondition(Field.RespirationRateBaseline_bpm, '==', stdRR_bpm)
        results = self.conditionalFilter([query])
        return results

    def calculateSystemPassRate(self, results, passError, categories):
        passRates = [0] * len(categories)
        for system,properties in results.items():
            print("Examining system "+system)
            numPass = 0
            numFailed = 0
            for property,property_error in properties.items():
                for error in property_error.errors:
                    if abs(error) > passError:
                        numFailed = numFailed + 1
                    else:
                        numPass = numPass + 1
            passRate = 0
            if numPass + numFailed != 0:
                passRate = numPass / (numPass + numFailed)
            passRates[categories.index(system)] = passRate
        return passRates


if __name__ == '__main__':
    analysis = PatientVariabilityAnalysis("./test_results/patient_variability/validation/full/")
    #analysis.exampleProcess()
    analysis.process()
