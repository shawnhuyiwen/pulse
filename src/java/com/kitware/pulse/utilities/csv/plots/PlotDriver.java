/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.utilities.csv.plots;

import java.awt.Color;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.RunConfiguration;
import com.kitware.pulse.utilities.JNIBridge;
import com.kitware.pulse.utilities.csv.CSVContents;

public class PlotDriver 
{

  public static void main(String[] args) 
  {    
    JNIBridge.initialize();
    PlotDriver me = new PlotDriver();
    RunConfiguration cfg = new RunConfiguration();
    //invalid input
    if(args.length > 0)
    {
      if(args.length == 1)
      {// Plotting from config file
        if(args[0].substring(args[0].lastIndexOf(".")+1).equalsIgnoreCase("config"))
        {
          File configFile = new File(cfg.getTestConfigDirectory()+"/"+args[0]);
          if(configFile.exists())
          {
            me.processConfigFile(configFile,cfg);
          }
          else
          {
            Log.error("ConfigFile "+configFile.getAbsolutePath()+" not found");
          } 
        }
        else
        {
          Log.error("Value "+args[0]+" doesn't seem to be a valid config file");
        }
      }
      else
      {// Plotting from two results files (compare-type plotting)
        me.expectedFilePath = args[0];
        me.computedFilePath = args[1];
        // Default 
        me.preload = true;
        me.onlyPlotFailures = false;
        me.abbreviateContents = 0;
        if (args.length > 2)
          me.preload = args[2].equalsIgnoreCase("true") ? true : false;
        if (args.length > 3)
          me.onlyPlotFailures = args[3].equalsIgnoreCase("true") ? true : false;
        if (args.length > 4)
          me.abbreviateContents = Integer.parseInt(args[4]);

        if(me.preload)
        {
          if(me.preloadData(me.expectedFilePath, me.computedFilePath))
            me.generateCompareJobs(me.expectedFilePath, me.computedFilePath, null);
        }
        else
          me.generateCompareJobs(me.expectedFilePath, me.computedFilePath, null);
      }
      me.execute();
    }
    else
    {
      Log.error("No files specified");
    }
  }
  
  public void execute()
  {
    //now execute for each job
    for(PlotJob job : this.jobs)
    {
      if(!job.ignore)
      {
        try
        {
          job.plotter.plot(job);
        }
        catch(Exception e)
        {
          Log.error("Plotter couldn't plot job "+job.name+". Check your config file line.", e);
          continue;
        }
        job.Reset();
        System.gc();
      }
    }
  }
  
  protected String name;
  protected Map<String,Class<? extends Plotter>> plotters = new HashMap<>();
  protected Map<String,String> macros = new HashMap<>();
  protected List<PlotJob> jobs = new ArrayList<>();
  protected Map<String,Map<String,List<Double>>> compareData = new HashMap<>();
  public boolean preload = false;
  public boolean onlyPlotFailures = false;
  protected String computedFilePath = "";
  protected String expectedFilePath = "";
  public int abbreviateContents = 0;
  public boolean isScenario = false;
  
  public class PlotJob
  {
    public PlotJob(){ }
    public String         name;
    public List<String>   headers = new ArrayList<>();
    public Plotter        plotter = null;
    public String         titleOverride;
    public boolean        ignore = false;
    public boolean        showGridLines = true;
    public boolean        logAxis = false;
    public boolean        forceZeroYAxisBound = false;
    public boolean        removeAllLegends = false;
    public int            resultsSkipNum = 0;
    public Color          bgColor = Color.white;
    public String         dataFile = null;
    public String         dataPath;
    public String         logFile = null;
    public String         logPath;
    public String         scenarioPath;
    public String         scenarioFile = null;
    public String         verificationDirectory;
    public String         experimentalData = null;
    public String         outputDir = "../verification/Plots/";
    public Integer        imageWidth = null;
    public Integer        imageHeight = null;
    public Integer        fontSize = 22;
    public Integer        legendFontSize = 15;
    public String         outputFilename = null;
    public boolean        xPercentOfBaseline = false;
    public boolean        yPercentOfBaseline = false;

    public boolean        skipAllActions = false;
    public boolean        skipAllEvents = false;
    public boolean        hideAELegend = false;
    public boolean        legendOnly = false;
    public List<String>   eventOmissions = new ArrayList<>();
    public List<String>   actionOmissions = new ArrayList<>();

    public boolean        isComparePlot = false;
    public String         computedDataPath = null;  //only used when comparing AND not preloading
    public String         computedDataFile = null;
    
    public List<String>   Y1headers = new ArrayList<>();
    public List<String>   Y2headers = new ArrayList<>();
    public String         X1header = null;
    public String         X2header = null;
    public Double         X1LowerBound;
    public Double         X2LowerBound;
    public Double         X1UpperBound;
    public Double         X2UpperBound;
    public Double         Y1LowerBound;
    public Double         Y2LowerBound;
    public Double         Y1UpperBound;
    public Double         Y2UpperBound;
    public String         X1Label = null;
    public String         Y1Label = null;
    public String         X2Label = null;
    public String         Y2Label = null;
    
    //Null all allocated data so it can be cleaned up
    public void Reset()
    {
      headers = null;
      eventOmissions = null;
      actionOmissions = null;
      compareData = null;
      Y1headers = null;
      Y2headers = null;
      plotter = null;
    }
  }
  
  public void setupComparison()
  {
    
  }

  //Reading from config file for validation ("pretty graphs")
  public void processConfigFile(File configFile, RunConfiguration cfg)
  {  	
    this.name = configFile.getName();
    this.name = this.name.substring(0, this.name.lastIndexOf('.'));
    
    String key;
    String value;
    
    this.plotters.clear();
    this.jobs.clear();
    
    String currentGroup = this.name;
    
    // Parse the config file
    try
    {
      BufferedReader br = new BufferedReader(new FileReader(configFile));
      String line;
      while ((line = br.readLine()) != null)
      {
        if (line.length() == 0 || line.startsWith("#"))
          continue;
        if(line.startsWith("@group"))
        {
          currentGroup = line.substring(6).trim();
          if(currentGroup.isEmpty())
            currentGroup = this.name;
          continue;
        }
        if(line.indexOf('=') == -1)
          continue;
        
        line = line.trim();
        key = line.substring(0, line.indexOf('='));
        value = line.substring(line.indexOf("=") + 1);
        
        //add functionality for any config keywords here
        if(key.equalsIgnoreCase("Plotter"))
        {
          Class<? extends Plotter> clazz = null;
          try
          {
            clazz = Class.forName(value).asSubclass(Plotter.class);
            this.plotters.put(clazz.getSimpleName(), clazz);
          } 
          catch(Exception e){Log.error("Could not find Plotter "+value);}
          continue;
        }        

        if(key.startsWith("Macro"))
        {
          macros.put(key.substring(6), value);
          continue;
        }

        for(String macro : macros.keySet())
        {
          if(value.indexOf(macro)>-1)
            value = value.replaceAll(macro, macros.get(macro));
        }
        
        PlotJob job = new PlotJob();
        job.scenarioPath = cfg.getScenarioDirectory()+"/patient";
        job.verificationDirectory=cfg.getVerificationDirectory()+"/scenarios/patient";
        //job2groups.put(job, currentGroup);
        if (key.charAt(0) == '-')
        {
          job.ignore = true;
          key = key.substring(1);
        }
        this.jobs.add(job);
        job.name = key.trim();

        String[] directives = value.trim().split(" ");
        for(String directive : directives)
        {
          if(directive.indexOf("=")==-1)
          {
            if(plotters.containsKey(directive))
            {
              try{job.plotter = plotters.get(directive).getConstructor().newInstance();}
              catch (Exception e)
              {
                job.ignore = true;
                Log.error("Could not make a new "+plotters.get(directive).getName()+" plotter");
              }
              continue;
            }
            //One-word directives can go here
            if(directive.equalsIgnoreCase("NoGrid")) 
            { job.showGridLines = false; continue; }
            else if(directive.equalsIgnoreCase("NoActions")) 
            { job.skipAllActions = true; continue; }
            else if(directive.equalsIgnoreCase("NoEvents")) 
            { job.skipAllEvents = true; continue; }
            else if(directive.equalsIgnoreCase("LogAxis")) 
            { job.logAxis = true; continue; }
            else if(directive.equalsIgnoreCase("ZeroAxis")) 
            { job.forceZeroYAxisBound = true; continue; }
            else if(directive.equalsIgnoreCase("HideActionEventLegend")) 
            { job.hideAELegend = true; continue; }
            else if(directive.equalsIgnoreCase("RemoveLegends")) 
            { job.hideAELegend = true; job.removeAllLegends = true; continue; }
            else if(directive.equalsIgnoreCase("LegendOnly")) 
            { job.legendOnly = true; continue; }
            else if(directive.equalsIgnoreCase("PercentOfBaseline")) 
            { job.yPercentOfBaseline = true; job.xPercentOfBaseline = true; continue; }
            else if(directive.equalsIgnoreCase("XPercentOfBaseline")) 
            { job.yPercentOfBaseline = false; job.xPercentOfBaseline = true; continue; }
            else if(directive.equalsIgnoreCase("YPercentOfBaseline")) 
            { job.yPercentOfBaseline = true; job.xPercentOfBaseline = false; continue; }
          }
          else
          {
            key = directive.substring(0, directive.indexOf('='));
            value = directive.substring(directive.indexOf("=") + 1);
            if(key.equalsIgnoreCase("Header"))
            {
              //Multiple desired headers will be split by commas
              String[] configHeaders = value.split(",");
              for(String h : configHeaders)
              {
                job.headers.add(h);
              }
              continue;
            }
            else if(key.equalsIgnoreCase("Y1"))
            {
              //Multiple desired headers will be split by commas
              String[] configHeaders = value.split(",");
              for(String h : configHeaders)
              {
                job.headers.add(h);
                job.Y1headers.add(h);
              }
              continue;
            }
            else if(key.equalsIgnoreCase("Y2"))
            {
              //Multiple desired headers will be split by commas
              String[] configHeaders = value.split(",");
              for(String h : configHeaders)
              {
                job.headers.add(h);
                job.Y2headers.add(h);
              }
              continue;
            }
            else if(key.equalsIgnoreCase("OmitActionsWith"))
            {
              //Multiple desired omissions will be split by commas
              String[] allSkips = value.split(",");
              for(String h : allSkips)
              {
                job.actionOmissions.add(h);
              }
              continue;
            }
            else if(key.equalsIgnoreCase("OmitEventsWith"))
            {
              //Multiple desired omissions will be split by commas
              String[] allSkips = value.split(",");
              for(String h : allSkips)
              {
                job.eventOmissions.add(h);
              }
              continue;
            } 
            else if(key.equalsIgnoreCase("ImageDimensions"))
            {
              //Expect width,height
              String[] dims = value.split(",");
              job.imageWidth = Integer.parseInt(dims[0]);
              job.imageHeight = Integer.parseInt(dims[1]);
              continue;
            } 
            else if(key.equalsIgnoreCase("VerificationDir"))
            {
              job.scenarioPath = cfg.getScenarioDirectory()+"/"+value;
              job.verificationDirectory = cfg.getVerificationDirectory()+"/scenarios/"+value;
              continue;
            }
            else if(key.equalsIgnoreCase("Title"))
            {job.titleOverride = value; continue;}    
            else if(key.equalsIgnoreCase("OutputFilename"))
            {job.outputFilename = value; continue;}   
            else if(key.equalsIgnoreCase("ReadSkip"))
            {job.resultsSkipNum = Integer.parseInt(value); continue;}
            else if(key.equalsIgnoreCase("X1"))
            {job.X1header = value; job.headers.add(value); continue;}
            else if(key.equalsIgnoreCase("X2"))
            {job.X2header = value; job.headers.add(value); continue;}
            else if(key.equalsIgnoreCase("X1LowerBound"))
            {job.X1LowerBound = Double.parseDouble(value); continue;}
            else if(key.equalsIgnoreCase("X2LowerBound"))
            {job.X2LowerBound = Double.parseDouble(value); continue;}
            else if(key.equalsIgnoreCase("X1UpperBound"))
            {job.X1UpperBound = Double.parseDouble(value); continue;}
            else if(key.equalsIgnoreCase("X2UpperBound"))
            {job.X2UpperBound = Double.parseDouble(value); continue;}
            else if(key.equalsIgnoreCase("Y1LowerBound"))
            {job.Y1LowerBound = Double.parseDouble(value); continue;}
            else if(key.equalsIgnoreCase("Y2LowerBound"))
            {job.Y2LowerBound = Double.parseDouble(value); continue;}
            else if(key.equalsIgnoreCase("Y1UpperBound"))
            {job.Y1UpperBound = Double.parseDouble(value); continue;}
            else if(key.equalsIgnoreCase("Y2UpperBound"))
            {job.Y2UpperBound = Double.parseDouble(value); continue;}
            else if(key.equalsIgnoreCase("X1Label"))
            {job.X1Label = value.replace("_", " "); continue;}
            else if(key.equalsIgnoreCase("Y1Label"))
            {job.Y1Label = value.replace("_", " "); continue;}
            else if(key.equalsIgnoreCase("X2Label"))
            {job.X2Label = value.replace("_", " "); continue;}
            else if(key.equalsIgnoreCase("Y2Label"))
            {job.Y2Label = value.replace("_", " "); continue;}
            else if(key.equalsIgnoreCase("ValidationData"))
            {job.experimentalData = cfg.getValidationDirectory()+"/"+value; continue;}
            else if(key.equalsIgnoreCase("DataFileOverride"))
            {job.dataFile = value; continue;}
            else if(key.equalsIgnoreCase("DataPathValidationOverride"))
            {job.dataPath = cfg.getValidationDirectory()+"/"+value; continue;}
            else if(key.equalsIgnoreCase("DataPathVerificationOverride"))
            {job.dataPath = cfg.getVerificationDirectory()+"/"+value; continue;}
            else if(key.equalsIgnoreCase("OutputOverride"))
            {job.outputDir = value; continue;}
            else if(key.equalsIgnoreCase("FontSize"))
            {job.fontSize = Integer.parseInt(value); continue;}
            else if(key.equalsIgnoreCase("LegendFontSize"))
            {job.legendFontSize = Integer.parseInt(value); continue;}
            else
            {
              Log.warn("Unrecognized config directive: "+directive);
            }
          }
        }
      }
      br.close();
    }
    catch (IOException e)
    {
      Log.error("Ouch",e);
    }
  }

  //try to go ahead and fill the map with all data to speed up compare plotting
  public boolean preloadData(String expectedFilePath, String computedFilePath)
  {    
    CSVContents expectedResults = null;
    CSVContents computedResults = null;
    Map<String,List<Double>> allExpected=new HashMap<>();
    Map<String,List<Double>> allComputed=new HashMap<>();
    
    //If expected results aren't present, still try to plot computed results
    try
    {
      expectedResults = new CSVContents(expectedFilePath);
      expectedResults.abreviateContents = this.abbreviateContents;
      expectedResults.readAll(allExpected);
      compareData.put(expectedFilePath, allExpected);
    }
    catch(Exception ex)
    {
      Log.warn("Unable to read expected results file");
    }
    
    //If there are no computed results, return false since this job shouldn't execute
    try
    {
      computedResults = new CSVContents(computedFilePath);
      computedResults.abreviateContents = this.abbreviateContents;
      computedResults.readAll(allComputed);
      compareData.put(computedFilePath, allComputed);
    }
    catch(Exception ex)
    {
      Log.error("Unable to read computed results file");
      return false;
    }

    return true;
  }
  
  public void generateCompareJobs(String expectedFilePath, String computedFilePath, Set<String> failures)
  {
    List<String> expectedHeaders = null;
    List<String> computedHeaders = null;
    
    //If we preloaded...
    if (compareData != null && !compareData.isEmpty())
    {
      try
      {
        expectedHeaders = new ArrayList<>(compareData.get(expectedFilePath).keySet());
      }
      catch(Exception e)
      {
        Log.warn("Unable to read headers from expected results file");
      }
      try
      {
        computedHeaders = new ArrayList<>(compareData.get(computedFilePath).keySet());
      }
      catch(Exception e)
      {
        Log.error("Unable to read headers from computed results file");
        return;
      }
    }
    //If we didn't preload...
    else
    {
      CSVContents expectedResults = null;
      CSVContents computedResults = null;
      
      //If expected results aren't present, still try to plot computed results
      try
      {
        expectedResults = new CSVContents(expectedFilePath);
        expectedResults.abreviateContents = this.abbreviateContents;
      }
      catch(Exception ex)
      {
        Log.warn("Unable to read expected results file");
      }
      
      //If there are no computed results, return false since this job shouldn't execute
      try
      {
        computedResults = new CSVContents(computedFilePath);
        computedResults.abreviateContents = this.abbreviateContents;
      }
      catch(Exception ex)
      {
        Log.error("Unable to read computed results file");
        return;
      }
    }
    
    if(expectedHeaders!=null)
    {
      if (expectedHeaders.size() != computedHeaders.size())
      {
        Log.warn("Number of results is different, expected "
            + expectedHeaders.size() + " but computed is "
            + computedHeaders.size());
      }

      //Loop through all expected results and generate jobs
      for(String header : expectedHeaders)
      {
        if(header.equalsIgnoreCase("Time(s)"))  //Time will be handled specially
          continue;
        //If we're plotting failures, only plot headers that failed
        if(onlyPlotFailures && !failures.isEmpty() && !failures.contains(header))
            continue;
        if(!computedHeaders.contains(header))
        {
          //This should have already been hit in CSVComparison
          //Log.error("Computed results did not contain expected header "+header);
        } 
        
        PlotJob job = new PlotJob();
        
        //Try to get a name for the job
        Path p = Paths.get(expectedFilePath);
        String expectedName = p.getFileName().toString();
        String expectedPath = p.toString().substring(0, p.toString().indexOf(expectedName));
        p = Paths.get(computedFilePath);
        String computedName = p.getFileName().toString();
        String computedPath = p.toString().substring(0, p.toString().indexOf(computedName));
        
        if(expectedName.isEmpty() || expectedName == null)
        {
          Log.warn("Could not generate a job name for header "+header);
          job.name = "";  //What should these jobs be named?
        }
        else
        {
          job.name = expectedName.substring(0, expectedName.lastIndexOf("."));
        }
        
        //We always want PlasmaConcentration on a log axis
        if(header.contains("PlasmaConcentration"))
          job.logAxis = true;
        
        //Set other job members
        job.plotter = new ActionEventPlotter();
        job.resultsSkipNum = this.abbreviateContents;
        job.skipAllActions = !isScenario;  //unit tests don't have scenario files with actions
        job.skipAllEvents = !isScenario;  //unit tests won't have events in their logs
        job.dataPath = expectedPath;
        job.dataFile = expectedName;
        if (compareData == null)  //only need these if we want AEPlotter to pull computed data
        {
          job.computedDataPath = computedPath;
          job.computedDataFile = computedName;
        }
        job.logPath = computedPath;  //only computed files have a log
        if(isScenario)
        {
          //If this is a scenario test, remove "Results" from name and don't add "Test"
          job.logFile = job.name.substring(0,job.name.indexOf("Results"))+".log";
          job.scenarioPath = expectedPath.substring(0,expectedPath.indexOf("Current Baseline"));
          job.scenarioFile = job.name.substring(0,job.name.indexOf("Results"))+".json";
          job.hideAELegend = true;
        }
        else
          job.logFile = job.name + "Test.log";  
        job.headers.add(header);  //We're plotting two datasets, but with the same header
        job.headers.add(header);
        job.isComparePlot = true;
        job.titleOverride = header+"vsTime";
        job.forceZeroYAxisBound = true;
        
        if(failures != null && !failures.isEmpty() && failures.contains(header))
          job.bgColor = Color.red;
        else if(failures != null && !failures.isEmpty() && !failures.contains(header))
          job.bgColor = Color.green;
        else if(failures != null && failures.isEmpty())
        {
          job.bgColor = Color.green;
        }
        
        ActionEventPlotter AEplotter = (ActionEventPlotter) job.plotter;
        job.outputDir = computedPath + job.name + "/";
        
        //Now load in preloaded data so ActionEventPlotter doesn't try to load it
        if (compareData != null && !compareData.isEmpty())
        {
          AEplotter.data.add(compareData.get(expectedFilePath).get(header));
          AEplotter.data.add(compareData.get(computedFilePath).get(header));
          AEplotter.timeData.add(compareData.get(expectedFilePath).get("Time(s)"));
          AEplotter.timeData.add(compareData.get(computedFilePath).get("Time(s)"));
        }
        this.jobs.add(job);
      }
    }
    
    //Now loop through all computed results and generate jobs for any extras
    for(String header : computedHeaders)
    {
      if(expectedHeaders!=null && expectedHeaders.contains(header))
        continue;
      if(header.equalsIgnoreCase("Time(s)"))  //Time will be handled specially
        continue;
      //If we're plotting failures, only plot headers that failed
      if(onlyPlotFailures && !failures.isEmpty() && !failures.contains(header))
          continue;
      
      PlotJob job = new PlotJob();
      
      Log.warn("Header "+header+" from computed results was not found in expected results... plotting it by itself...");
      
      //Try to get a name for the job
      Path p = Paths.get(expectedFilePath);
      String expectedName = p.getFileName().toString();
      String expectedPath = p.toString().substring(0, p.toString().indexOf(expectedName));
      p = Paths.get(computedFilePath);
      String computedName = p.getFileName().toString();
      String computedPath = p.toString().substring(0, p.toString().indexOf(computedName));
      
      if(expectedName.isEmpty() || expectedName == null)
      {
        Log.warn("Could not generate a job name for header "+header);
        job.name = "";  //What should these jobs be named?
      }
      else
      {
        job.name = expectedName.substring(0, expectedName.lastIndexOf("."));
      }
      
      //We always want PlasmaConcentration on a log axis
      if(header.contains("PlasmaConcentration"))
        job.logAxis = true;
      
      //Set other job members
      job.plotter = new ActionEventPlotter();
      job.resultsSkipNum = this.abbreviateContents;
      job.skipAllActions = !isScenario;  //unit tests don't have scenario files with actions
      job.skipAllEvents = !isScenario;  //unit tests won't have events in their logs
      job.dataPath = expectedPath;
      job.dataFile = expectedName;
      if (compareData == null)  //only need these if we want AEPlotter to pull computed data
      {
        job.computedDataPath = computedPath;
        job.computedDataFile = computedName;
      }
      job.logPath = computedPath;  //only computed files have a log
      if(isScenario)
      {
        //If this is a scenario test, remove "Results" from name and don't add "Test"
        job.logFile = job.name.substring(0,job.name.indexOf("Results"))+".log";
        job.scenarioPath = expectedPath.substring(0,expectedPath.indexOf("Current Baseline"));
        job.scenarioFile = job.name.substring(0,job.name.indexOf("Results"))+".json";
        job.hideAELegend = true;
      }
      else
        job.logFile = job.name + "Test.log";  
      job.headers.add(header);  //We're plotting two datasets, but with the same header
      job.headers.add(header);
      job.isComparePlot = true;
      job.titleOverride = header+"vsTime";
      job.forceZeroYAxisBound = true;
      
      if(failures != null && !failures.isEmpty() && failures.contains(header))
        job.bgColor = Color.red;
      else if(failures != null && !failures.isEmpty() && !failures.contains(header))
        job.bgColor = Color.green;
      
      ActionEventPlotter AEplotter = (ActionEventPlotter) job.plotter;
      job.outputDir = computedPath + job.name + "/";
      
      //Now load in preloaded data so ActionEventPlotter doesn't try to load it
      if (compareData != null && !compareData.isEmpty())
      {
        if(expectedHeaders != null)
        {
          AEplotter.data.add(compareData.get(expectedFilePath).get(header));
          AEplotter.timeData.add(compareData.get(expectedFilePath).get("Time(s)"));
        }
        
        AEplotter.data.add(compareData.get(computedFilePath).get(header));
        AEplotter.timeData.add(compareData.get(computedFilePath).get("Time(s)"));
      }
      
      this.jobs.add(job);
    }
  }
  
}

