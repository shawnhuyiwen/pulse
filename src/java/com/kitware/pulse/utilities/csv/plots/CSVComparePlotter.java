/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.utilities.csv.plots;

import java.awt.Color;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.csv.CSVContents;

/**
 * Compare 2 CSV files and create a report on the differences
 * Will create plot images if a time column is provided
 * @author abray
 *
 */
public class CSVComparePlotter
{
  public enum PlotType { NoPlot, FullPlot, FullPlotErrors, FastPlot, FastPlotErrors, MemoryFastPlot}
  public PlotType plotType=PlotType.FastPlot;

  public static void main(String[] args) throws IOException, InterruptedException
  {
    if(args.length<2)
    {
      if(args.length==1&&args[0].equals("default"))
      {
        args = new String[10];
        args[0] = "./BaselineResults.csv";// Expected
        args[1] = "./ComputedResults.csv";// Computed
        args[2] = "false"; // Full plot
      }
      else
      {
        Log.fatal("Expected inputs : [expected results file path] [computed results file path] [percentDifference] [time to start comparing at] [track differences] [output folder] [results file]");
        return;
      }
    }

    CSVComparePlotter t = new CSVComparePlotter();

    File expected = new File(args[0]);
    if(!expected.exists())
    {
      // Check to see if there is a result file available
      String rFile = args[0].replaceAll(".csv", ".csv");
      expected = new File(rFile);
      if(!expected.exists())
      {
        Log.error("Expected file does not exist "+args[0]);
        Log.info("I am going to try to plot the computed");
      }
      args[0] = rFile;
    }
    File computed = new File(args[1]);
    if(!computed.exists())
    {
      Log.error("Computed file does not exist "+args[1]);
      return;
    }
         
    // Should we plot every point
    if(args.length>2)
      t.plotType = args[2].equalsIgnoreCase("true")?PlotType.FullPlot:PlotType.FastPlot;

    Log.info("Plotting "+args[0]+" and "+args[1]);
    t.plot(args[0],args[1],null);
  }
  
  /**
   * @param args
   */
  public void plot(String expectedFilePath, String computedFilePath, Set<String> failures)
  {    
    String plotDir = computedFilePath.substring(0,computedFilePath.length()-4); 
  
    try
    {
      File toDir = FileUtils.createDirectory(plotDir);
      File[] files = toDir.listFiles(new FilenameFilter() {
        @Override
        public boolean accept(File dir, String name) {
            return name.toLowerCase().endsWith(".jpg");
        }
      });
      for(File file : files)
        file.delete();      
    }
    catch(Exception ex)
    {
      Log.warn("Having a hard time deleting the old report directory");
      Log.warn(ex.getMessage());
    }

    
    
    boolean readAll=true;
    if(plotType == PlotType.FastPlotErrors || plotType == PlotType.FullPlotErrors)
      readAll=false;//Let's assume these are all going to pass, so we don't really need to read anything in!
    if(plotType == PlotType.MemoryFastPlot)
    {// Take our time and try to conserve memory
      readAll=false;
      plotType = PlotType.FastPlot;
    }
    
    CSVContents expectedResults = null;
    CSVContents computedResults = null;
    List<String> expectedHeaders = null;
    List<String> computedHeaders = null;
    List<Double> expectedData=new ArrayList<>();
    List<Double> computedData=new ArrayList<>();
    Map<String,List<Double>> allExpected=new HashMap<>();
    Map<String,List<Double>> allComputed=new HashMap<>();
    
    try
    {
      expectedResults = new CSVContents(expectedFilePath);
      if(this.plotType == PlotType.FastPlot || this.plotType == PlotType.FastPlotErrors)
      {
        expectedResults.abreviateContents = 5;
      }
      expectedHeaders = expectedResults.getHeaders();
      if(readAll)
        expectedResults.readAll(allExpected);
    }
    catch(Exception ex)
    {
      Log.warn("Unable to read expected results file");
      Log.warn(ex.getMessage());
    }
    
    try
    {
      computedResults = new CSVContents(computedFilePath);
      if(this.plotType == PlotType.FastPlot || this.plotType == PlotType.FastPlotErrors)
      {
        computedResults.abreviateContents = 5;
      }
      computedHeaders = computedResults.getHeaders();
      if(computedHeaders==null)
      {
        Log.error("No headers for "+computedFilePath);
        return;
      }
      if(readAll)
        computedResults.readAll(allComputed);
    }
    catch(Exception ex)
    {
      Log.error("Unable to read computed results file");
      Log.error(ex.getMessage());
      return;
    }

    CSVPlotTool plot = new CSVPlotTool();

    
    if(expectedHeaders!=null)
    {
      if(expectedHeaders.size()!=computedHeaders.size())
      {
        Log.warn("Number of results is different, expected "+expectedHeaders.size()+" but computed is "+computedHeaders.size());
      }    
      for(String header : expectedHeaders)
      {
        if(!computedHeaders.contains(header))
        {
          Log.error("Computed results did not provide expected result '"+header);
        }       
      }
    }

    List<Double> expectedTime = null;
    List<Double> computedTime = null;
    if(readAll)    
    {
      expectedTime = allExpected.get("Time(s)");
      computedTime = allComputed.get("Time(s)");
      if(expectedTime==null)
        Log.error("There is no data in the expected file? I will plot what you computed...");
      allExpected.remove("Time(s)");
      allComputed.remove("Time(s)");
    }

    // Plot both data point on one graph
    if(expectedHeaders != null)
    {
      for(String header : expectedHeaders)
      {
        if("Time(s)".equals(header))
          continue;      
        if(!computedHeaders.contains(header))
          continue;

        if(plotType == PlotType.FastPlotErrors || plotType == PlotType.FullPlotErrors)
        {
          if(failures!=null&&!failures.contains(header))
            continue;// only plot errors
        }

        // Clear out the last runs data;
        
        System.gc();

        if(!readAll)
        {
          if(expectedTime == null)
          {
            expectedTime = new ArrayList<>();
            computedTime = new ArrayList<>();
            expectedResults.readHeader("Time(s)",expectedTime);
            computedResults.readHeader("Time(s)",computedTime);
          }
          expectedData.clear();
          computedData.clear();    
          expectedResults.readHeader(header,expectedData);
          computedResults.readHeader(header,computedData);
        }
        else
        {
          expectedData = allExpected.get(header);
          if(expectedData == null)
          {
            Log.warn(header + "No data was found in expected results for header "+header);
          }
          computedData = allComputed.get(header);
          if(computedData == null)
          {
            Log.error(header + "No data was found in computed results for header "+header);
            continue;
          }
          allExpected.remove(header);// Take it out of the map
          allComputed.remove(header);// Take it out of the map
        }
        
        

        try
        {
          plot.createGraph
          (
              plotDir,
              failures==null?Color.white:failures.contains(header)?Color.red:Color.green,
                  header+"vsTime",// Title
                  "Time(s)",// X Axis
                  header,// Y Axis
                  plot.createXYSeries("Expected", expectedTime, expectedData),
                  plot.createXYSeries("Computed", computedTime, computedData)
              );
        }
        catch(Exception ex)
        {
          Log.error("Unable to create graph for " + header);
          Log.error(ex.getMessage());
        }
      }
    }

    // Graph anything that was not in the expected data as well
    for(String header : computedHeaders)
    {
      if( (expectedHeaders!=null && expectedHeaders.contains(header)) || header.equals("Time(s)"))
        continue;

      try
      {
        if(!readAll)
          computedResults.readHeader(header,computedData);
        else
          computedData = allComputed.get(header);

        allComputed.remove(header);// Take it out of the map
        plot.createGraph
        (
            plotDir,
            Color.white,
            header+"vsTime",// Title
            "Time(s)",// X Axis
            header,// Y Axis
            plot.createXYSeries("Computed", computedTime, computedData)
            );
      }
      catch(Exception ex)
      {
        Log.error("Unable to create graph for " + header);
        Log.error(ex.getMessage());
      }
    }
  }

}
