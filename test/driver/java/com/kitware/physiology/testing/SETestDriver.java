/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.testing;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.*;

import com.kitware.physiology.utilities.FileUtils;
import com.kitware.physiology.utilities.jniBridge;
import com.kitware.physiology.utilities.Log;
import com.kitware.physiology.utilities.RunConfiguration;
import com.kitware.physiology.testing.csv.CSVComparison;
import com.kitware.physiology.utilities.csv.plots.CSVComparePlotter;
import com.kitware.physiology.utilities.csv.plots.CSVComparePlotter.PlotType;

public class SETestDriver
{
	protected SETestConfiguration cfg = new SETestConfiguration();

  public interface Executor
  {
    boolean ExecuteTest(SETestJob job);
  }

  public static void main(String[] args)
  {
    jniBridge.initialize();
    SETestDriver me = new SETestDriver();
    if(args.length > 0)
    {
      me.config = args[0];
      if(args.length == 4)
      {
        me.commitHash = args[1];
        me.environment = args[2];
        me.architecture = args[3];
      }
      me.test();
    }
    else
      Log.error("No configFile specified");
    jniBridge.deinitialize();
  }
  
  public void test()
  {
    RunConfiguration rcfg = new RunConfiguration();
    
    File configFile = new File(rcfg.getTestConfigDirectory()+"/"+this.config);
    if(!configFile.exists())
    {
      System.err.println("Config file "+configFile.getName()+" does not exist");
      return;
    }
    this.cfg.processConfigFile(configFile, rcfg);
    this.cfg.deleteExistingResults();
    
    int     availableThreads = Runtime.getRuntime().availableProcessors();
    boolean isPlotting;
    int     activeThreads=0;
    int     engineThreads=1;
    if(this.cfg.numThreads <= 0)
      engineThreads = availableThreads + this.cfg.numThreads;
    else
      engineThreads = this.cfg.numThreads;
    if(engineThreads<=0)
      engineThreads = 1;
    if(engineThreads > availableThreads)
    {
      engineThreads = availableThreads;
      Log.info("You dont have " + this.cfg.numThreads + " available on your computer.");
    }

    Log.info("I can run " + availableThreads + " threads, I am going to use " + engineThreads);

    do
    {
      activeThreads = 0;
      isPlotting = false;
      for(SETestJob job : this.cfg.jobs)
      {
        if(job.state == SETestJob.State.Executing)
          activeThreads++;
        if(job.state == SETestJob.State.Comparing)
        {
          activeThreads++;
          isPlotting = true;
        }
      }
      if(!isPlotting && activeThreads != engineThreads)
      {// Look for a job to compare first    
        for(SETestJob job : this.cfg.jobs)
        {                      
          if(job.state == SETestJob.State.Executed)
          {
            if(job.PlottableResults && !job.skipPlot)
            {
              CompareThread cThread = this.new CompareThread();
              job.state = SETestJob.State.Comparing;
              cThread.job = job;
              activeThreads++;
              cThread.start();              
              break;
            }
            else
            {
              job.state = SETestJob.State.Complete;
            }            
          }
        }
      }
      if(activeThreads != engineThreads)
      {
        for(SETestJob job : this.cfg.jobs)
        {
          if(job.state==SETestJob.State.Provisioned)
          {
            ExecuteThread eThread = this.new ExecuteThread();
            eThread.job = job;
            job.state = SETestJob.State.Executing;
            eThread.start();
            activeThreads++;
          }
          if(activeThreads == engineThreads)
            break;            
        }
      }      
      if(activeThreads==0)
        break;

      try
      {Thread.sleep(100);}
      catch (InterruptedException ex)
      { Log.error("I have insomnia...",ex); }
    }while(true);

    this.createReport();
  }

  public String config = "";
  public String environment = "";
  public String architecture = "";
  public String commitHash = "";

  public class ExecuteThread extends Thread
  {
    public SETestJob job;
    public void run()
    {          
      for(String toCompute : job.computedFiles)
      {
        if(job.PlottableResults)
        {
          if(job.name.endsWith(".pba"))//This should be a scenario file, different naming convention
          {
            String[] dirs = toCompute.substring(0,toCompute.indexOf(".csv")).split("[/\\\\]");
            String report = toCompute.substring(0,toCompute.indexOf(".csv"))+"/"+dirs[dirs.length-1]+"Report.pba";
            job.reportFiles.add(report);
          }
          else
          {
            String path = toCompute.substring(0, toCompute.lastIndexOf("."));
            path = path + path.substring(path.lastIndexOf("/")) + "Report.pba";
            job.reportFiles.add(path);
          }
        }
        else
          job.reportFiles.add(toCompute.substring(0, toCompute.lastIndexOf("."))+"Report.pba");
      }

      if(!job.skipExecution)
      {
        if(job.executor!=null)
        {
          Log.info("Executing Job : "+job.name);
          try
          {
            if(!job.executor.ExecuteTest(job))
              job.skipPlot = true;
          }
          catch(Exception ex)
          {
            Log.error("Could not exeucute job : "+job.name, ex);
            job.skipPlot = true;
          }
          catch(Throwable t)
          {
            Log.error("Could not exeucute job : "+job.name, t);
            job.skipPlot = true;
          }
        }
        else      
          Log.error("No executor for job "+job.name);
      }
      System.gc();
      job.state = SETestJob.State.Executed;
    }
  }

  public class CompareThread extends Thread
  {
    public SETestJob job;
    public void run()
    {
      if(job.PlottableResults && !job.skipPlot)
      {          
        for(int i=0; i<job.baselineFiles.size(); i++)
        {
          CSVComparison compare = new CSVComparison();
          compare.limit = job.percentDifference;
          compare.reportDifferences = false;          
          compare.setFullReportPath(job.reportFiles.get(i)); 
          if(new File(job.computedFiles.get(i)).exists())
          {
            Set<String> failures = null;
            if(new File(job.baselineFiles.get(i)).exists())
            {
            	try {
            		failures = compare.compare(job.baselineFiles.get(i), job.computedFiles.get(i));
            	}catch(Exception ex){ failures=null; }
              if(failures==null)// Something bad happened in running this test...
                compare.createErrorSuite(job.name,"Could not compare these files for some reason: "+job.baselineFiles.get(i)+" and " + job.computedFiles.get(i));
            }
            else
            {
              compare.createErrorSuite(job.name,"Basline file not found : "+job.baselineFiles.get(i)+" and " + job.computedFiles.get(i));
            }
            
            compare.write();
           
            if((job.plotType == PlotType.FastPlotErrors || job.plotType == PlotType.FullPlotErrors) && (failures==null || failures.isEmpty()))
            {
              Log.info("No plots for "+job.computedFiles.get(i));
            }
            else
            {
              CSVComparePlotter plotter = new CSVComparePlotter();
              plotter.plotType = job.plotType;
              plotter.plot(job.baselineFiles.get(i), job.computedFiles.get(i), failures);
              /*
              PlotDriver driver = new PlotDriver();
              if(job.plotType == PlotType.FastPlot)
              {
                driver.preload = true;
                driver.onlyPlotFailures = false;
                driver.abbreviateContents = 5;
              }
              else if(job.plotType == PlotType.FullPlot)
              {
                driver.preload = true;
                driver.onlyPlotFailures = false;
                driver.abbreviateContents = 0;
              }
              else if(job.plotType == PlotType.FastPlotErrors)
              {
                driver.preload = true;
                driver.onlyPlotFailures = true;
                driver.abbreviateContents = 5;
              }
              else if(job.plotType == PlotType.FullPlotErrors)
              {
                driver.preload = true;
                driver.onlyPlotFailures = true;
                driver.abbreviateContents = 0;
              }
              else if(job.plotType == PlotType.MemoryFastPlot)
              {
                driver.preload = false;
                driver.onlyPlotFailures = false;
                driver.abbreviateContents = 0;
              }
              if(driver.preload)
              {
                if(!driver.preloadData(job.baselineFiles.get(i), job.computedFiles.get(i)))
                  continue;
              }
              if(job.executor.getClass().getName().indexOf("Scenario")!=-1)
                driver.isScenario = true;

              driver.generateCompareJobs(job.baselineFiles.get(i), job.computedFiles.get(i), failures);
              driver.execute();
               */
            }
          }
          else
          {
            Log.error("Couldn't read file "+job.computedFiles.get(i));
          }
        }
      }
      System.gc();
      job.state = SETestJob.State.Complete;
    }
  }

  public void createReport()
  {
    List<String> currentGroup;
    SETestReport report = new SETestReport();    
    report.setFullReportPath("./test_results/"+cfg.testName+"Report.pba");    
    for(SETestJob job : cfg.jobs)
    {
      String group = cfg.job2groups.get(job);
      if(!cfg.groups.containsKey(group))
      	cfg.groups.put(group,new ArrayList<String>());
      currentGroup = cfg.groups.get(group);

      if (job.knownFailure)
      {
        report.addKnownSuiteFailure(job.name);
        continue;
      }    
      if (job.isAssessment)
      {
        SETestSuite ts = report.createTestSuite();
        ts.setName(job.name);
        ts.setPerformed(true);
        SETestCase  tc = ts.createTestCase();
        tc.setName(job.name);
        if(new File(job.computedFiles.get(0)).exists())
        {
          ts.setPerformed(true);
          currentGroup.add(ts.getName());
        }
        else
        {
          ts.setPerformed(false);
          tc.AddFailure("Could not find assessment file");
        }
        continue;
      }
      if(job.reportFiles.isEmpty())
      {
        report.createErrorSuite(job.name,"No reports found for "+job.name+" to summarize");
        continue;
      }
      for(String reportFile : job.reportFiles)
      {
        if(!FileUtils.fileExists(reportFile))
        {
          report.createErrorSuite(job.name,"Unable to find file "+reportFile+" to summarize");
          continue;
        }
        SETestReport tRpt = new SETestReport();
        try
        {
        	tRpt.readFile(reportFile);
        	for(SETestSuite ts : tRpt.testSuites)
          {
          	report.addTestSuite(ts);
          	currentGroup.add(ts.getName());
          }
        }
        catch(Exception ex)
        {
        	report.createErrorSuite(job.name,reportFile);
        	Log.error("Need file with TestReportData object");
        }
      }
    }    
    report.write();
    // Write the HTML to a file
    try
    {
      BufferedWriter out = new BufferedWriter(new FileWriter("./test_results/"+cfg.testName+".html"));
      out.write(report.toHTML(cfg.reportName,cfg.groups));
      out.close();
    }
    catch(Exception ex)
    {
      Log.error("Unable to write HTML report for "+cfg.testName,ex);
    }
  }

  
}


