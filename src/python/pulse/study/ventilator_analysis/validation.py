# using python 3.8
import json
import os
import pandas as pd
import matplotlib.pyplot as plt


def writeHtmlTableDocument(saveFilename: str, results: pd.DataFrame,
                           jIndex: list) -> str:  # name of report
    '''
    create an HTML document from a dict
    '''
    # render an html table string
    # class utilizes our custom css
    table = results.to_html(classes='pulseVal', index=False)

    # apply the red or green formatting
    table = table.replace('<td>Passed</td>', '<td class="passed">Passed</td>')
    table = table.replace('<td>Failed</td>', '<td class="failed">Failed</td>')

    html = returnHTMLstring('Pulse Validation', returnTableCSS(), table)
    saveHtmlPage(saveFilename, html)


def writeHtmlPhotoDoc(saveFilename: str, pics: dict, patientSettings: dict,
                      ventSettings: dict, compTable: dict,
                      priority: list, sep: str) -> None:
    '''
    creates an html doc pointing to the photos
    '''
    # turn the dictionaries into html
    html = ''
    for sel in pics.keys():
        imgs = ['', '', '']  # setup for the 3 main plots
        matched = []
        title = sel.split(sep)
        html += '<div class="title"><h2>' + title[0] \
            + ' ' + title[1] + '</h2></div>'
        # get the settings
        newPS = {}
        newPS['Patient Setting'] = []
        newPS['Value'] = []
        for k in patientSettings[title[0]].keys():
            if 'Units' in k:
                # add the units
                newPS['Patient Setting'][-1] += ' (' + \
                    patientSettings[title[0]][k] + ')'
            else:
                # append the name
                newPS['Patient Setting'].append(k)
                newPS['Value'].append(patientSettings[title[0]][k])
        ps = pd.DataFrame(newPS)

        newVS = {}
        newVS['Ventilator Setting'] = []
        newVS['Value'] = []
        for k in ventSettings[sel].keys():
            if 'Units' in k:
                # add the units
                newVS['Ventilator Setting'][-1] += ' (' + \
                    ventSettings[sel][k] + ')'
            else:
                # append the name
                newVS['Ventilator Setting'].append(k)
                newVS['Value'].append(ventSettings[sel][k])
        vs = pd.DataFrame(newVS)

        # add the settings
        html += '<div class="scene2" style="flex grow: 0;"><div>' + \
            ps.to_html(classes='pulseVal', index=False) + \
            '</div><div class="spacer"></div> <div>'
        html += vs.to_html(classes='pulseVal', index=False) + \
            '</div></div>'

        # add the gt pic once for all of the others
        html += '<div class="scene">\n' + \
            '<img class="regularImg" src="' + \
            pics[sel]['Ground Truth'] +  \
            '" /> <div class="imageSet">'

        for name in pics[sel].keys():
            if name == 'Ground Truth':
                continue
            if 'Pulse' not in pics[sel][name].keys():
                continue
            # save the img
            temp = '<img class="setImg" src="' \
                + pics[sel][name]['Pulse'] + '" />'

            # plot based on priority - order them to match the instrument
            for ind in range(0, len(priority)):
                # compare the name
                sec = sel + sep + priority[ind]
                if sec == name:
                    imgs[ind] = temp
                    matched.append(pics[sel][name]['Pulse'])

            # grab pics that aren't in the priority list for last
            if pics[sel][name]['Pulse'] not in matched:
                temp = temp.replace('setImg', 'squareImg')
                imgs.append(temp)

        # write out the imageset
        for img in imgs:
            html += img
        # close out this set
        html += '</div></div>'
        # get the comparison
        comp = pd.DataFrame(compTable[sel])
        html += '<div>' + \
            comp.to_html(classes='pulseVal', index=False) + '</div>'

    html = returnHTMLstring('Pulse Validation', returnPicCSS(), html)

    saveHtmlPage(saveFilename, html)


def saveHtmlPage(saveFilename: str, code: str) -> None:
    # save the file
    outputFile = saveFilename + '.html'
    print('writing summary: ' + outputFile)
    with open(outputFile, 'w') as f:
        # f.write(style1.render())
        f.write(code)


def returnHTMLstring(name: str, css: str, htmlTable: str) -> str:
    '''
    create a basic html formatted page
    '''
    html_string = '''
    <html>
    <head><title>{title}</title></head>
    <style type="text/css">{cssStyle}</style>
    <body>
        {table}
    </body>
    </html>
    '''
    return html_string.format(title=name, cssStyle=css, table=htmlTable)


def returnTableCSS() -> str:
    '''
    Gives some formatting for the html
    '''
    return '''
    .pulseVal {
        font-size: 12pt;
        font-family: sans-serif;
        border-collapse: collapse;
        border: 1px solid silver;
        padding: 0px;
        margin: 0px;
        margin-bottom: 10pt;
    }

    .pulseVal tbody {
        background: #fff;
    }

    .pulseVal th {
        border: 0;
        text-align: center;
        padding: 0.5ex 1.5ex;
        margin: 0px;
    }

    .pulseVal td {
        white-space: nowrap;
        border: 1;
        text-align: center;
        padding: 0.5ex 1.5ex;
        margin 0px;
        border-width: 1px 1px;
    }

    .passed {
        background-color: green;
    }

    .failed {
        background-color: red;
    }

    .pulseVal tbody tr:nth-child(odd) {
        background: #fff;
    }

    .pulseVal thead>tr:first-child>th {
        border: 1;
        border-left: 1px;
        border-right: 1px;
        border-top: 2pt solid black;
    }

    .pulseVal tr:nth-child(even) {
        background: #CCC;
    }

    .pulseVal tr:nth-child(odd) {
        background: #FFF;
    }

    .pulseVal thead>tr:last-child>th {
        border-bottom: 1pt solid black;
    }

    .pulseVal tbody>tr:last-child>td {
        border-bottom: 2pt solid black;
    }
    '''


def returnPicCSS() -> str:
    '''
    Gives some formatting for the html
    '''
    return '''
    .pulseVal {
        font-size: 12pt;
        font-family: sans-serif;
        border-collapse: collapse;
        border: 1px solid silver;
        padding: 0px;
        margin: 0px;
        margin-bottom: 10pt;
    }

    .pulseVal tbody {
        background: #fff;
    }

    .regularImg {
        height: 600px;
        max-width: auto;
    }

    .setImg {
        display: block;
        max-width:500px;
        max-height:200px;
        width: auto;
        height: auto;
    }

    .squareImg {
        display: block;
        max-width:500px;
        max-height:500px;
        width: auto;
        height: auto;
    }

    .title {
        display: flex;
        align-items: center;
        justify-content: center;
        flex-direction: column;
        flex-grow: 1;
        padding: 0.1em;
    }

    .scene {
        display: grid;
        grid-template-columns: 0.5fr 0.5fr;
        align-items: left;
        justify-content: center;
    }

    .scene2 {
        display: grid;
        grid-template-columns: 0.4fr 0.2fr 0.4fr;
        align-items: left;
        justify-content: center;
    }

    .imageSet {
        display: flex;
        flex-direction: column;
        align-items: stretch;
        flex-grow: 1;
    }
    '''


def main():
    '''
    Read in a parameters.json file and extracts naming convention for
    validation processes
    '''
    # parameters file to load
    wd = os.getcwd()
    parameters = './parameters.json'
    naming = 'picNamingConvention'  # used to extract the files and contents

    # load in the data
    param = {}
    fileP = os.path.abspath(os.path.join(wd, parameters))
    with open(fileP, 'r') as f:
        param = json.load(f)

    paths = param['paths']
    picFolder = paths['picsFolder']
    pulseResultsFolder = paths['pulseResultsFolder']
    groundTruthDataJSON = paths['groundTruthDataJSON']
    plotsFolder = paths['plotOutputFolder']

    # load in the gt json
    gtData = {}
    fileP = os.path.abspath(os.path.join(wd, groundTruthDataJSON))
    with open(fileP, 'r') as f:
        gtData = json.load(f)

    names = param[naming]
    sep = names["separator"]
    convention = names["comment"].split(sep)
    extensions = names["extensions"]
    pics = {}
    csvs = {}
    for p in param[convention[0]]:
        for v in param[convention[1]]:
            # assemble per the naming convention
            gtSelect = str(p + sep + v)

            csvFile = pulseResultsFolder + p + sep + v + '.csv'
            cFile = os.path.abspath(os.path.join(wd, csvFile))
            if os.path.exists(cFile):
                # print(csvFile)
                csvs[gtSelect] = pd.read_csv(cFile)

            for e in extensions:
                picfile = picFolder + gtSelect + '.' + e
                pFile = os.path.abspath(os.path.join(wd, picfile))
                if os.path.exists(pFile):
                    # store this for later loading
                    pics[gtSelect] = picfile  # pFile
    # create out folder if not there
    if not os.path.exists(plotsFolder):
        try:
            os.mkdir(plotsFolder)
        except Exception as ex:
            print('error making folder: ' + plotsFolder + ': ' + str(ex))
    # clear plots out of the results folder:
    pngs = os.listdir(plotsFolder)
    for png in pngs:
        if '.png' in png:
            try:
                os.remove(plotsFolder + png)
            except OSError as e:
                print("Error: %s : %s" % (plotsFolder + png, e.strerror))

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
    for sel in gtData['validationData']:
        valPerTable[sel] = {}
        valPerTable[sel]['Parameter'] = []
        valPerTable[sel]['Ground Truth'] = []
        valPerTable[sel]['Pulse'] = []
        valPlots[sel] = {}
        headers = gtData['validationData'][sel].keys()
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
                val = "{:.3g}".format(float(gtData['validationData'][sel][head]))
                # print('gt: ' + val + ', pulse: ' + last)
                valTable['Ground Truth'].append(val)
                valTable['Pulse'].append(last)
                valPerTable[sel]['Ground Truth'].append(val)
                valPerTable[sel]['Pulse'].append(last)

        # setup the plots
        valPlots[sel]['Ground Truth'] = pics[sel]
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
                savedPulse = plotsFolder + name + '.png'
                fig = plt.gcf()
                fig.set_size_inches(plotX, plotY, forward=True)
                fig.savefig(savedPulse, bbox_inches='tight', dpi=100)
                valPlots[sel][name]['Pulse'] = savedPulse

    # outDF = pd.DataFrame(valTable)
    # writeHtmlTableDocument('PulseValidation', outDF, valTable.keys())
    priority = param['plotPriorityOrder']
    writeHtmlPhotoDoc('PulseValidation', valPlots,
                      gtData['patientSettings'],
                      gtData['ventilatorSettings'],
                      valPerTable,
                      priority, sep)


if __name__ == "__main__":

    main()  # run the main def
