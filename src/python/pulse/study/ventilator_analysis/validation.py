# using python 3.8
import json
import os
import pandas as pd
import matplotlib.pyplot as plt


def writeHtmlTableDocument(saveFilename: str, results: pd.DataFrame, jIndex: list) -> str:  # name of report
    '''
    create an HTML document from a dict
    '''
    # render an html table string
    table = results.to_html(classes='pulseVal', index=False)  # class utilizes our custom css

    # apply the red or green formatting
    table = table.replace('<td>Passed</td>', '<td class="passed">Passed</td>')
    table = table.replace('<td>Failed</td>', '<td class="failed">Failed</td>')

    html = returnHTMLstring('Pulse Validation', returnTableCSS(), table)
    saveHtmlPage(saveFilename, html)


def writeHtmlPhotoDoc(saveFilename: str, pics: dict, patientSettings: dict,
                      ventSettings: dict, compTable: dict) -> None:
    '''
    creates an html doc pointing to the photos
    '''
    # turn the dictionaries into html
    html = ''
    imgs = ['', '', '']
    cnt = 0
    for p in range(0, len(pics['Parameter'])):
        if cnt == 0:
            title = pics['Parameter'][p].split('_')
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

            sel = title[0] + '_' + title[1]
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

            # add the gt pic once for all 4 of the others
            html += '<div class="scene">\n' + \
                '<img class="regularImg" src="' + \
                pics['Ground Truth'][p] +  \
                '" /> <div class="imageSet">'
        # only plot the 3:
        if ('Flow' in pics['Parameter'][p] or \
            'Pressure' in pics['Parameter'][p] \
            or 'Volume' in pics['Parameter'][p]) \
                and 'Intrapulmonary' not in pics['Parameter'][p]:
            # save the img
            temp = '<img class="setImg" src="' \
                + pics['Pulse'][p] + '" />'
            # order them to match the instrument
            if 'Pressure' in pics['Parameter'][p]:
                imgs[0] = temp
            elif 'Flow' in pics['Parameter'][p]:
                imgs[1] = temp
            else:
                imgs[2] = temp
        cnt += 1
        if cnt > 3:
            # write out the imageset
            for img in imgs:
                html += img
            # close out this set
            html += '</div></div>'
            # get the comparison
            comp = pd.DataFrame(compTable[sel])
            html += '<div>' + \
                comp.to_html(classes='pulseVal', index=False) + '</div>'
            cnt = 0

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
    valPlots['Parameter'] = []
    valPlots['Ground Truth'] = []
    valPlots['Pulse'] = []
    for sel in gtData['validationData']:
        valPerTable[sel] = {}
        valPerTable[sel]['Parameter'] = []
        valPerTable[sel]['Ground Truth'] = []
        valPerTable[sel]['Pulse'] = []
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
        x = 'Time(s)'
        for plot in param['plots']:
            if plot in csvs[sel].columns:
                xlist = csvs[sel][x]
                ylist = csvs[sel][plot]
                plt.clf()
                plt.plot(xlist, ylist)
                plt.tight_layout()
                plt.fill_between(xlist, ylist, facecolor="blue")
                # plt.show()
                name = plot.split('(')[0]
                plt.title(sel + sep + name)
                savedPulse = './out/' + sel + sep + name + '.png'
                fig = plt.gcf()
                fig.set_size_inches(6, 2, forward=True)
                fig.savefig(savedPulse, bbox_inches='tight', dpi=100)
                valPlots['Parameter'].append(sel + sep + name)
                valPlots['Pulse'].append(savedPulse)
                valPlots['Ground Truth'].append(pics[sel])

    #outDF = pd.DataFrame(valTable)
    #writeHtmlTableDocument('PulseValidation', outDF, valTable.keys())
    writeHtmlPhotoDoc('PulseValidation', valPlots,
                      gtData['patientSettings'],
                      gtData['ventilatorSettings'],
                      valPerTable)


if __name__ == "__main__":

    main()  # run the main def
