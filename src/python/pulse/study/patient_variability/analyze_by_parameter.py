# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.Patient_pb2 import PatientData
from pulse.study.patient_variability.analysis_utils import PatientVariabilityResults, Conditional, Field, PropertyError

from enum import Enum

import plotly.graph_objects as go
import plotly.offline as pyo
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


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


class ConditionalType(Enum):
    AND = 1
    OR = 2

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
        #analysis.createBarChart()
        #analysis.createBoxPlot()

    def createBarChart(self):
        # some example data
        threshold = 43.0
        values = np.array([30., 87.3, 99.9, 3.33, 50.0])
        x = range(len(values))

        # split it up
        above_threshold = np.maximum(values - threshold, 0)
        below_threshold = np.minimum(values, threshold)

        # and plot it
        fig, ax = plt.subplots()
        ax.bar(x, below_threshold, 0.35, color="g")
        ax.bar(x, above_threshold, 0.35, color="r",
               bottom=below_threshold)

        # horizontal line indicating the threshold
        ax.plot([0., 4.5], [threshold, threshold], "k--")

        fig.savefig("look-ma_a-threshold-plot.png")

    def createBoxPlot(self):
        N = 50
        df = pd.DataFrame({"Prior": np.random.geometric(0.5, N) * 2 + 10,
                           "DSI": np.random.geometric(0.1, N) * 3 + 20,
                           "DSI.ESMDA": np.random.geometric(0.2, N) + 40,
                           "DSI.ESMDA.LOC": np.random.geometric(0.2, N) * 4 + 30,
                           "ES-MDA": np.random.geometric(0.7, N) * 2.5 + 60})
        df.boxplot(grid=False, rot=18, fontsize=15)
        reference = [20, 50, 75]
        left, right = plt.xlim()
        plt.hlines(reference, xmin=left, xmax=right, color='r', linestyles='--')
        plt.subplots_adjust(bottom=0.20)
        plt.show()

    def createRadarCharts(self):
        # Everything
        results = analysis.everythingQuery()
        numPatients = analysis.numPatients(results)

        # Standard male
        results = analysis.standardMaleQuery()

        # Calculate pass rate for each system
        radarCategories = analysis.combineCategories(results)

        passError = 10.0 # Errors are in %, not fraction!!!
        radarStandardMaleValues = analysis.calculateSystemPassRate(results, passError, radarCategories)

        #Standard female
        results = analysis.standardFemaleQuery()
        # Calculate pass rate for each system
        radarStandardFemaleValues = analysis.calculateSystemPassRate(results, passError, radarCategories)

        # Nonstandard male query
        results = analysis.nonStandardMaleQuery()
        # Calculate pass rate for each system
        radarNonstandardMaleValues = analysis.calculateSystemPassRate(results, passError, radarCategories)

        # Nonstandard female query
        results = analysis.nonStandardFemaleQuery()
        # Calculate pass rate for each system
        radarNonstandardFemaleValues = analysis.calculateSystemPassRate(results, passError, radarCategories)

        # Generate radar chart
        radarCategories = [*radarCategories, radarCategories[0]]

        radarStandardMaleValues = [*radarStandardMaleValues, radarStandardMaleValues[0]]
        radarStandardFemaleValues = [*radarStandardFemaleValues, radarStandardFemaleValues[0]]
        radarNonstandardMaleValues = [*radarNonstandardMaleValues, radarNonstandardMaleValues[0]]
        radarNonstandardFemaleValues = [*radarNonstandardFemaleValues, radarNonstandardFemaleValues[0]]

        fig = go.Figure(
            data=[
                go.Scatterpolar(r=radarStandardMaleValues, theta=radarCategories, name='Standard Male'),
                go.Scatterpolar(r=radarStandardFemaleValues, theta=radarCategories, name='Standard Female'),
                go.Scatterpolar(r=radarNonstandardMaleValues, theta=radarCategories, name='Cumulative Nonstandard Male'),
                go.Scatterpolar(r=radarNonstandardFemaleValues, theta=radarCategories, name='Cumulative Nonstandard Female')
            ],
            layout=go.Layout(
                title=go.layout.Title(text='Physiology System Pass Rate'),
                polar={'radialaxis': {'visible': True}},
                showlegend=True
            )
        )

        pyo.plot(fig)

    def numPatients(self, results):
        # TODO: Find a more elegant way to do this
        return len(results["Patient"]["HeartRateBaseline(1/min)-Patient2SystemMean"].patient_ids)

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
        query.addCondition(Field.MeanArterialPressureBaseline_mmHg, '==', stdMAP_mmHg)
        query.addCondition(Field.PulsePressureBaseline_mmHg, '==', stdPulsePressure_mmHg)
        query.addCondition(Field.RespirationRateBaseline_bpm, '==', stdRR_bpm)
        results = self.conditionalFilter([query])
        return results

    def nonStandardMaleQuery(self):
        print("Nonstandard male query")
        query = Conditional()
        query.sex(PatientData.eSex.Male)
        queryOr = Conditional(ConditionalType.OR)
        queryOr.addCondition(Field.Age_yr, '!=', stdAge_yr)
        queryOr.addCondition(Field.Height_cm, '!=', stdMaleHeight_cm)
        queryOr.addCondition(Field.BodyMassIndex, '!=', stdMaleBMI)
        queryOr.addCondition(Field.BodyFatFraction, '!=', stdMaleBFF)
        queryOr.addCondition(Field.HeartRateBaseline_bpm, '!=', stdHR_bpm)
        queryOr.addCondition(Field.MeanArterialPressureBaseline_mmHg, '!=', stdMAP_mmHg)
        queryOr.addCondition(Field.PulsePressureBaseline_mmHg, '!=', stdPulsePressure_mmHg)
        queryOr.addCondition(Field.RespirationRateBaseline_bpm, '!=', stdRR_bpm)
        query.addConditional(queryOr)
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
        query.addCondition(Field.MeanArterialPressureBaseline_mmHg, '==', stdMAP_mmHg)
        query.addCondition(Field.PulsePressureBaseline_mmHg, '==', stdPulsePressure_mmHg)
        query.addCondition(Field.RespirationRateBaseline_bpm, '==', stdRR_bpm)
        results = self.conditionalFilter([query])
        return results

    def nonStandardFemaleQuery(self):
        print("Nonstandard female query")
        query = Conditional()
        query.sex(PatientData.eSex.Female)
        queryOr = Conditional(ConditionalType.OR)
        queryOr.addCondition(Field.Age_yr, '!=', stdAge_yr)
        queryOr.addCondition(Field.Height_cm, '!=', stdFemaleHeight_cm)
        queryOr.addCondition(Field.BodyMassIndex, '!=', stdFemaleBMI)
        queryOr.addCondition(Field.BodyFatFraction, '!=', stdFemaleBFF)
        queryOr.addCondition(Field.HeartRateBaseline_bpm, '!=', stdHR_bpm)
        queryOr.addCondition(Field.MeanArterialPressureBaseline_mmHg, '!=', stdMAP_mmHg)
        queryOr.addCondition(Field.PulsePressureBaseline_mmHg, '!=', stdPulsePressure_mmHg)
        queryOr.addCondition(Field.RespirationRateBaseline_bpm, '!=', stdRR_bpm)
        query.addConditional(queryOr)
        results = self.conditionalFilter([query])
        return results

    def everythingQuery(self):
        print("Everything query")
        query = Conditional()
        results = self.conditionalFilter([query])
        return results

    def calculateSystemPassRate(self, results, passError, categories):
        passRates = [0] * len(categories)
        totalNumPass = [0] * len(categories)
        totalNumFail = [0] * len(categories)
        for system,properties in results.items():
            print("Examining system "+system)
            numPass = 0
            numFail = 0
            for property,property_error in properties.items():
                for error in property_error.errors:
                    if abs(error) > passError:
                        numFail = numFail + 1
                    else:
                        numPass = numPass + 1
            for idx, category in enumerate(categories):
                if category in system:
                    totalNumPass[idx] = totalNumPass[idx] + numPass
                    totalNumFail[idx] = totalNumFail[idx] + numFail

        for idx, category in enumerate(categories):
            passRate = 0
            numPass = totalNumPass[idx]
            numFail = totalNumFail[idx]
            if numPass + numFail != 0:
                passRate = numPass / (numPass + numFail)
            passRates[idx] = passRate
        return passRates

    def combineCategories(self, results):
        startCategories = []
        endCategories = []
        for system, properties in results.items():
            startCategories.append(system)
        for startCategory in startCategories:
            newCategory = True
            for idx, endCategory in enumerate(endCategories):
                if startCategory in endCategory:
                    # Replace endCategory with startCategory
                    endCategories[idx] = startCategory
                    newCategory = False
                if endCategory in startCategory:
                    # Don't do anything
                    newCategory = False
                    break
            if newCategory:
                # New, so add
                endCategories.append(startCategory)
        return endCategories


if __name__ == '__main__':
    analysis = PatientVariabilityAnalysis("./test_results/patient_variability/validation/full/")
    #analysis.exampleProcess()
    analysis.process()
