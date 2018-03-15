package mil.tatrc.physiology.testing;

import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.engine.SEAutoSerialization;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.LogListener;
import mil.tatrc.physiology.utilities.csv.plots.CSVComparePlotter.PlotType;

public class SETestJob extends LogListener
{
  public enum State {Provisioned, Executing, Executed, Comparing, Complete}
  
  public SETestJob(){ listen(false);  }
  public boolean      useState=false;
  public SEAutoSerialization autoSerialization=null;
  public String       patientFile = null;
  public State        state=State.Provisioned;
  public String       name;
  public boolean      skipExecution = false;
  public boolean      skipPlot = false;
  public boolean      knownFailure = false;
  public SETestDriver.Executor executor = null;
  public boolean      PlottableResults = false;
  public PlotType     plotType = PlotType.FastPlot;//Only plot every nth data point
  public double       percentDifference;
  public List<String> baselineFiles = new ArrayList<String>();
  public String       baselineDirectory = null;
  public List<String> computedFiles = new ArrayList<String>();
  public String       computedDirectory = null;
  public List<String> reportFiles = new ArrayList<String>();
  public String       resultsFiles = null;

  public SETestJob clone()
  {
  	SETestJob copy = new SETestJob();
    copy.useState = this.useState;
    copy.patientFile = this.patientFile;
    copy.state = this.state;
    copy.name = this.name;
    copy.skipExecution = this.skipExecution;
    copy.skipPlot = this.skipPlot;
    copy.knownFailure = this.knownFailure;
    copy.executor = this.executor;
    copy.PlottableResults = this.PlottableResults;
    copy.plotType = this.plotType;
    copy.percentDifference = this.percentDifference;
    copy.baselineFiles.addAll(this.baselineFiles);
    copy.baselineDirectory = this.baselineDirectory;
    copy.computedFiles.addAll(this.computedFiles);
    copy.computedDirectory = this.computedDirectory;
    copy.reportFiles.addAll(this.reportFiles);
    copy.resultsFiles = this.resultsFiles;
    return copy;
  }

  public void handleDebug(String msg) { Log.debug("["+name+"] "+msg,""); }
  public void handleInfo(String msg)  { Log.info("["+name+"] "+msg,""); }
  public void handleWarn(String msg)  { Log.warn("["+name+"] "+msg,""); }
  public void handleError(String msg) { Log.error("["+name+"] "+msg,""); }
  public void handleFatal(String msg) { Log.fatal("["+name+"] "+msg,""); }
}
