# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import os
import shutil
import reporting
import baseline

from pulse.cdm.utils import plotter

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
            csvs[name] = plotter.read_csv_into_df(csvFile, replace_slashes=False).loc[500:1250]
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
            xLabel = xParam = 'Time(s)'
            yLabel = '(' + plot.split('(')[-1]
            yParam = plot.split("$vs$")[0]
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
                xParam = xLabel = splstr[0]
                yParam = yLabel = plot = splstr[1]
                name = sel + sep + xLabel.split('(')[0] + \
                    sep + 'vs' + sep + plot.split('(')[0]
                plotX = param['plotSizes']['vsPlot']['x']
                plotY = param['plotSizes']['vsPlot']['y']

            valPlots[sel][name] = {}
            if plot in csvs[sel].columns:
                plot_source = plotter.PlotSource()
                plot_source.df = csvs[sel]
                plot_source.color = "blue"
                plot_source.fill = fillPlot
                if reverseAxes:
                    xLabel, yLabel = yLabel, xLabel
                    xParam, yParam = yParam, xParam
                    tempParam = xParam

                plot_settings = plotter.PlotSettings()
                plot_settings.title = name
                plot_settings.xlabel = xLabel
                plot_settings.ylabel = yLabel
                plot_settings.legend = False
                plot_settings.grid = False
                plot_settings.right_ticks = False
                plot_settings.ticklabel_style = "plain"

                if plotter.create_plot(xParam, yParam, [plot_source], plot_settings):
                    imgName = name + ".png"
                    plotter.save_current_plot("./test_results/ventilator_analysis/"+imgName, plotX, plotY)
                    valPlots[sel][name]['Pulse'] = imgName
                    plotter.clear_current_plot()

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
