/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.testing.csv;

import java.io.File;
import java.io.IOException;
import java.util.*;

import com.kitware.pulse.cdm.testing.SETestReport;
import com.kitware.pulse.cdm.testing.SETestSuite;
import com.kitware.pulse.utilities.DoubleUtils;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.SEEqualOptions;
import com.kitware.pulse.utilities.csv.CSVContents;
import com.kitware.pulse.utilities.csv.plots.PlotDriver;

/**
 * Compare 2 CSV files and create a report on the differences
 * Will create plot images if a time column is provided
 * @author abray
 *
 */
public class CSVComparison extends SETestReport
{
  public double limit=2.0;
  public boolean reportDifferences=false;
  
  protected class Error
  {
    public Error(double t) { firstTime = t; }
    public int total=0;
    public double firstTime;
    public double lastTime;
    public double maxTime;
    public double maxExpected;
    public double maxComputed;
    public double maxErr=0;
    public double minTime;
    public double minExpected;
    public double minComputed;
    public double minErr=10000;
  }
  
  public static void main(String[] args) throws IOException, InterruptedException
  {
    if(args.length<2)
    {
      if(args.length==1&&args[0].equals("default"))
      {
        args = new String[10];
        args[0] = "./Scenario1Results_1.csv";// Expected
        args[1] = "./Scenario1Results_1.csv";// Computed
        args[2] = "2.0";// Percent Difference Allowed
        args[3] = "false";// Report Differences in the error report, turn off if you are expecting a LOT of error
      }
      else
      {
        Log.fatal("Expected inputs : [expected results file path] [computed results file path] [percentDifference] [time to start comparing at] [track differences] [output folder] [zip results]");
        return;
      }
    }

    CSVComparison t = new CSVComparison();
    
    // Tolerance Limit
    if(args.length>2)
      t.limit=Double.parseDouble(args[2]);
    
    // Report the differences
    t.reportDifferences = false;
    if(args.length>3&&args[3]!=null&&args[3].equalsIgnoreCase("true"))
      t.reportDifferences = true;

    Log.info("Comparing "+args[0]+" to "+args[1]);
    Set<String> failures = t.compare(args[0],args[1]);
    t.write();
    //CSVComparePlotter plotter = new CSVComparePlotter();
    //plotter.plot(args[0], args[1], failures);
    PlotDriver driver = new PlotDriver();
    driver.preload = true;
    driver.preloadData(args[0], args[1]);
    driver.generateCompareJobs(args[0], args[1], failures);
    driver.execute();
  }

  /**
   * @param args
   */
  public Set<String> compare(String expectedFilePath, String computedFilePath)
  {    
    
    File expectedFile = new File(expectedFilePath);
    if(!expectedFile.exists())
    {
      // Check to see if there is a zip available
      String eZip = expectedFilePath.replaceAll(".csv", ".zip");
      expectedFile = new File(eZip);
      if(!expectedFile.exists())
      {
        Log.error("Expected file does not exist "+expectedFilePath);
        Log.info("I am going to try to plot the computed");
      }
      expectedFilePath = eZip;
    }
    File computedFile = new File(computedFilePath);
    if(!computedFile.exists())
    {
      Log.error("Computed file does not exist "+computedFilePath);
      return null;
    }
    
    String report = computedFilePath.substring(0,computedFilePath.length()-4)+"/"+computedFile.getName();
    report=report.substring(0, report.length()-4)+"Report.json";
    this.setFullReportPath(report);
        
    Set<String> failures = new HashSet<>();
    try
    {
      FileUtils.delete(this.reportDir);
      Thread.sleep(1000);
      FileUtils.createDirectory(this.reportDir);
    }
    catch(Exception ex)
    {
      Log.warn("Having a hard time deleting the old report directory");
      Log.warn(ex.getMessage());
    }
    
    CSVContents expectedResults;
    CSVContents computedResults;
    try
    {
      expectedResults = new CSVContents(expectedFilePath);
      computedResults = new CSVContents(computedFilePath);
    }
    catch(Exception ex)
    {
      Log.error("Unable to read input files");
      Log.error(ex.getMessage());
      return null;
    }
    
    int totalErrors=0;

    List<String> expectedHeaders = expectedResults.getHeaders();
    List<String> computedHeaders = computedResults.getHeaders();
    
    if(expectedHeaders==null)
    {
      Log.error("No headers for "+expectedFilePath);
      return null;
    }
    if(computedHeaders==null)
    {
      Log.error("No headers for "+computedFilePath);
      return null;
    }
    
    // Create the Test Case
    SETestSuite suite = createTestSuite();
    suite.setName(this.name);
    suite.startCase(this.name);
    // Set up the Compare Options
    SEEqualOptions opts = suite.getCaseEqualOptions();
    opts.setPercentDifference(this.limit);
    opts.trackError(true);
    opts.trackDifferences(false);
    //opts.truncateMantissasTo(5);

    if(expectedHeaders.size()!=computedHeaders.size())
    {
      Log.warn("Number of results is different, expected("+expectedFilePath+") "+expectedHeaders.size()+" but computed ("+computedFilePath+") is "+computedHeaders.size());
    }
    
    for(String header : expectedHeaders)
    {
      if(!computedHeaders.contains(header))
      {
        totalErrors++;
        Log.error("Computed results ("+computedFilePath+") did not provide expected result "+header);
      }
    }

    Map<String,Error> headerError = new HashMap<String,Error>();
    boolean firstColumnIsTime = false;
    if(expectedHeaders.get(0).substring(0, 4).equalsIgnoreCase("Time"))
      firstColumnIsTime = true;
    int totalLines = 0;
    
    while(true)
    {
      totalLines++;
      List<Double> expectedData = expectedResults.readNextLine();
      List<Double> computedData = computedResults.readNextLine();
      
      if( ( expectedData.isEmpty() && !computedData.isEmpty()) ||
          (!expectedData.isEmpty() &&  computedData.isEmpty()))
      {
        Log.error("Computed ("+computedFilePath+") is not the same length as expected");
        totalErrors++;
        break;
      }
      if(expectedData.isEmpty() || computedData.isEmpty())
        break;
      
      for(int i=0; i<expectedData.size(); i++)
      {
        
        String header = expectedHeaders.get(i);
        double expected=expectedData.get(i);
        int idx=computedHeaders.indexOf(header);
        if(idx==-1)
          continue;
        double computed=computedData.get(idx);
  
        if(!DoubleUtils.equals(expected, computed, opts))
        {
          totalErrors++;
          if(!headerError.containsKey(header))
            headerError.put(header, new Error(expectedData.get(0)));
          Error err = headerError.get(header);
          err.total++;
          double optsErr = opts.getLastError();
          if(optsErr > err.maxErr)
          {
            err.maxTime = expectedData.get(0);
            err.maxExpected = expected;
            err.maxComputed = computed;
            err.maxErr = optsErr;
          }
          if(optsErr < err.minErr)
          {
            err.minTime = expectedData.get(0);
            err.minExpected = expected;
            err.minComputed = computed;
            err.minErr = optsErr;
          }
          err.lastTime = expectedData.get(0);
          
          if(reportDifferences)
          {
            if(!firstColumnIsTime)
              Log.error(header+" does not match expected "+expected+" != computed "+computed+" ["+optsErr+"%]");
            else
              Log.error(header+" @Time "+expectedData.get(0)+": expected "+expected+" != computed "+computed+" ["+optsErr+"%]");
          }
          failures.add(header);
        }
      }
    }

    Log.info("Compared "+totalLines+" total times");
    if(totalErrors>0)
    {
      Log.error(totalErrors+" total errors found");
      for(String key : headerError.keySet())
      {
        Error err = headerError.get(key);
        Log.error(key+" has a total of "+err.total+" errors between times ["+err.firstTime+", "+err.lastTime+"] \n"+
                      "-  min error : @Time "+err.minTime+": expected "+err.minExpected+" != computed "+err.minComputed+" ["+err.minErr+"%] \n"+
                      "-  max error : @Time "+err.maxTime+": expected "+err.maxExpected+" != computed "+err.maxComputed+" ["+err.maxErr+"%]");
      }
    }
    if(suite.getActiveCase().hasFailures())
      Log.error(computedFilePath +" Comparison failed!!");
    else
      Log.info(computedFilePath + " Comparison SUCCESS!!");
        
    suite.endCase();
    return failures;
  }
  
}
