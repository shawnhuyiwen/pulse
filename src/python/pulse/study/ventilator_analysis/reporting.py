# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import pandas as pd

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
