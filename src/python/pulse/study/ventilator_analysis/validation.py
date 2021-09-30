# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import os
import shutil
import reporting
import baseline

import pandas as pd
import matplotlib.pyplot as plt

def main(param):
    sep = '_'
    imgs = {}
    csvs = {}
    imgDir = getImgDir()
    expected = baseline.data()
    patientTypes = ["Normal", "ARDS", "COPD"]
    patientTypes = ["Healthy", "Obstructive", "Restrictive"]

    ventilatorModes = ["VC_AC", "PC_AC", "CPAP"]

    csvDir = "./verification/scenarios/miscellaneous/ventilation_mechanics/"

    for p in patientTypes:
        for v in ventilatorModes:
            # assemble per the naming convention
            name = p+'_'+v
            csvFile = csvDir+name+"Results.csv"
            if not os.path.exists(csvFile):
                raise ValueError("Unable to load csv file: "+csvFile)
            csvs[name] = pd.read_csv(csvFile).loc[500:1250]
            imgFilename = imgDir+name+".jpg"
            if not os.path.exists(imgFilename):
                raise ValueError("Unable to find image file: "+imgFilename)
            imgs[name] = name+".jpg"

    # Setup results directory
    outputDir = "./test_results/ventilator_analysis/"
    if os.path.exists(outputDir):
        shutil.rmtree(outputDir)
    os.mkdir(outputDir)
    # Copy the images to this output
    # This helps to pack up and send the results
    src_files = os.listdir(imgDir)
    for file_name in src_files:
        full_file_name = os.path.join(imgDir, file_name)
        if os.path.isfile(full_file_name):
            shutil.copy(full_file_name, outputDir)


    # now we have all the data - analyze is
    valTable = {}
    valTable['Parameter'] = []
    valTable['PatientType'] = []
    valTable['VentilatorMode'] = []
    valTable['Ground Truth'] = []
    valTable['Pulse'] = []
    valPerTable = {}

    params = []
    valPlots = {}
    for sel in expected['validationData']:
        valPerTable[sel] = {}
        valPerTable[sel]['Parameter'] = []
        valPerTable[sel]['Ground Truth'] = []
        valPerTable[sel]['Pulse'] = []
        valPlots[sel] = {}
        headers = expected['validationData'][sel].keys()
        # go through these headers
        for head in headers:
            # see if the key is in this dataframe
            if head in csvs[sel].columns:
                # print(sel + " : " + head)
                cat = sel.split(sep)
                valTable['PatientType'].append(cat[0])
                valTable['VentilatorMode'].append(cat[1])
                valTable['Parameter'].append(head)
                valPerTable[sel]['Parameter'].append(head)
                if head not in params:
                    params.append(head)

                last = "{:.3g}".format(float(csvs[sel][head].iloc[-1]))
                val = "{:.3g}".format(float(expected['validationData'][sel][head]))
                # print('gt: ' + val + ', pulse: ' + last)
                valTable['Ground Truth'].append(val)
                valTable['Pulse'].append(last)
                valPerTable[sel]['Ground Truth'].append(val)
                valPerTable[sel]['Pulse'].append(last)

        # setup the plots
        valPlots[sel]['Ground Truth'] = imgs[sel]
        for plot in param['plots']:
            # plots use the time column by default - unless
            # they have a $vs$ in them
            xLabel = 'Time(s)'
            yLabel = '(' + plot.split('(')[-1]
            name = sel + sep + plot.split('(')[0]
            plotX = param['plotSizes']['default']['x']
            plotY = param['plotSizes']['default']['y']
            # Setting for filling in the plot
            fillPlot = True
            if plot in param['plotNoFill']:
                fillPlot = False
            # setting for plots that need to have reversed axes
            reverseAxes = False
            if plot in param['plotReverseAxes']:
                reverseAxes = True
            # special plots with vs in them
            if '$vs$' in plot:
                splstr = plot.split('$vs$')
                xLabel = splstr[0]
                yLabel = plot = splstr[1]
                name = sel + sep + xLabel.split('(')[0] + \
                    sep + 'vs' + sep + plot.split('(')[0]
                plotX = param['plotSizes']['vsPlot']['x']
                plotY = param['plotSizes']['vsPlot']['y']

            valPlots[sel][name] = {}
            if plot in csvs[sel].columns:
                if reverseAxes:
                    ylist = csvs[sel][xLabel]
                    xlist = csvs[sel][plot]
                    temp = xLabel
                    xLabel = yLabel
                    yLabel = temp
                else:
                    xlist = csvs[sel][xLabel]
                    ylist = csvs[sel][plot]
                plt.clf()
                plt.plot(xlist, ylist)
                plt.xlabel(xLabel)
                plt.ylabel(yLabel)
                plt.tight_layout()
                # check for no fill setting
                if fillPlot:
                    plt.fill_between(xlist, ylist, facecolor="blue")
                # plt.show()
                plt.title(name)
                imgName = name + ".png"
                fig = plt.gcf()
                fig.set_size_inches(plotX, plotY, forward=True)
                fig.savefig("./test_results/ventilator_analysis/"+imgName, bbox_inches='tight', dpi=100)
                valPlots[sel][name]['Pulse'] = imgName

    # outDF = pd.DataFrame(valTable)
    # writeHtmlTableDocument('PulseValidation', outDF, valTable.keys())
    priority = param['plotPriorityOrder']
    reporting.writeHtmlPhotoDoc(outputDir+"report", valPlots,
                      expected['patientSettings'],
                      expected['ventilatorSettings'],
                      valPerTable,
                      priority, sep)


def getImgDir():
    with open("run.config") as file:
        for line in file:
            if line.startswith("root_dir"):
                line = line.replace('\n','')
                root = line.split("=")
                return root[1]+"src/python/pulse/study/ventilator_analysis/images/"
    raise ValueError('Could not find image directory.')

if __name__ == "__main__":
    params = {}

    plots = {}
    plots["MechanicalVentilator-InspiratoryFlow(L/min)"] = "Flow(L/min)"
    plots["MechanicalVentilator-TotalLungVolume(mL)"] = "Volume(mL)"
    plots["MechanicalVentilator-AirwayPressure(cmH2O)"] = "P1"
    plots["MechanicalVentilator-TotalLungVolume(mL)$vs$MechanicalVentilator-AirwayPressure(cmH2O)"] = "LungVolume_vs_AirwayPressure"
    plots["MechanicalVentilator-InspiratoryFlow(L/min)$vs$MechanicalVentilator-TotalLungVolume(mL)"] = "InspiratoryFlow_vs_LungVolume"
    params["plots"] = plots

    params["plotNoFill"] = [
        "MechanicalVentilator-TotalLungVolume(mL)$vs$MechanicalVentilator-AirwayPressure(cmH2O)",
        "MechanicalVentilator-InspiratoryFlow(L/min)$vs$MechanicalVentilator-TotalLungVolume(mL)"]

    params["plotReverseAxes"] = [
        "MechanicalVentilator-TotalLungVolume(mL)$vs$MechanicalVentilator-AirwayPressure(cmH2O)",
        "MechanicalVentilator-InspiratoryFlow(L/min)$vs$MechanicalVentilator-TotalLungVolume(mL)"]

    # This is used to make these plots show in this order - to match the photo
    params["plotPriorityOrder"] = [
        "MechanicalVentilator-AirwayPressure",
        "MechanicalVentilator-InspiratoryFlow",
        "MechanicalVentilator-TotalLungVolume"]

    params["plotSizes"] = {"default": {"x": 6, "y": 2}, "vsPlot": {"x": 6, "y": 6}}

    main(params)
