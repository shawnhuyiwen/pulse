package pulse.utilities.csv.plots;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import pulse.cdm.substance.SESubstanceManager;
import pulse.utilities.FileUtils;
import pulse.utilities.Log;
import pulse.utilities.csv.CSVContents;
import pulse.utilities.csv.plots.PlotDriver.PlotJob;

// Plotting the same columns from multiple csv files onto one plot

public class MultiSourcePlotter implements Plotter
{
  public static void main(String[] args)
  {
    PlotDriver.main(args);
  }
  
  @Override
  public void plot(PlotJob job, SESubstanceManager subMgr)
  {
    CSVPlotTool plotTool = new CSVPlotTool();  //to leverage existing functions
    XYSeriesCollection xySeries = new XYSeriesCollection();
    
    if (job.experimentalData != null)
      Log.error("MultiSourcePlotter does not support experimentalData");// Can add later if needed
    if(job.Y1headers.size()>1)
      Log.error("MultiSourcePlotter does not support multiple columns from a csv source");// Can add later if needed
    if(!job.Y2headers.isEmpty())
      Log.error("MultiSourcePlotter does not support X2/Y2 directives");// Can add later if needed
    
    //fill PlotJob with needed data if it doesn't exist
    if(job.dataPath == null || job.dataPath.isEmpty())
    {job.dataPath = job.verificationDirectory+"/";}
    if(job.dataFile == null || job.dataFile.isEmpty())
    {job.dataFile = job.name + "Results.csv";}
    
    int numSources=0;
    try
    {
      String[] dataFiles = job.dataFile.split(",");
      numSources=dataFiles.length;
      for(int s=0; s<numSources; s++)
      {
        String dataFile = dataFiles[s];
        CSVContents csv = new CSVContents(job.dataPath + dataFile);
        csv.abreviateContents = job.resultsSkipNum;
        
        // Get X Data
        Log.info("Reading "+job.X1header+" from "+dataFiles[s]);
        List<Double> xData = new ArrayList<Double>();
        csv.readHeader(csv.unitUnderscoreToSpace(job.X1header), xData);
        // Get Y Data
        Log.info("Reading "+job.Y1headers.get(0)+" from "+dataFiles[s]);
        List<Double> yData = new ArrayList<Double>();
        csv.readHeader(csv.unitUnderscoreToSpace(job.Y1headers.get(0)), yData);
        // Optionally, Scale the data
        if(job.percentOfBaseline)
        {
          double xBaseline = xData.get(0);
          double yBaseline = yData.get(0);
          for(int i=0; i < xData.size(); i++)
          {
            //xData.set(i, (1-(xData.get(i)/xBaseline))*100); // If not time
            yData.set(i, (yData.get(i)/yBaseline)*100);
          }
        }
        // Create our plot series with the data
        XYSeries dataSeries = plotTool.createXYSeries("Group "+(s+1),xData,yData);
        xySeries.addSeries(dataSeries);
      }
      
    } catch (IOException e)
    {
      Log.error("Could not analyze file " + job.dataPath + job.dataFile);
    }
    
    String title = job.name;
    if(job.titleOverride != null && !job.titleOverride.isEmpty() && !job.titleOverride.equalsIgnoreCase("None"))
      title = job.titleOverride;
    
    //Default labels
    String XAxisLabel = job.X1header;
    String YAxisLabel = job.Y1headers.get(0);
    
    JFreeChart chart = ChartFactory.createXYLineChart(
        job.titleOverride!=null&&job.titleOverride.equalsIgnoreCase("None")?"":title, // chart title
        XAxisLabel,                // x axis label
        YAxisLabel,                // y axis label
        xySeries,                  // data
        PlotOrientation.VERTICAL,  // orientation
        true,                      // include legend
        true,                      // tooltips
        false                      // urls
        );
    
    Log.info("Creating Graph "+title);
    XYPlot plot = (XYPlot) chart.getPlot();
    
    if (!job.logAxis)
    {
    
    }
    else
    {
      Log.error("MultiSourcePlotter does not support log axis");// Can add later if needed
    }
    
    //Override X bounds if desired
    try
    {
      if(job.X1LowerBound != null)
        plot.getDomainAxis(0).setLowerBound(job.X1LowerBound);
      if(job.X1UpperBound != null)
        plot.getDomainAxis(0).setUpperBound(job.X1UpperBound);
    }
    catch(Exception e)
    {
      Log.error("Couldn't set X bounds. You probably tried to set a bound on an axis that doesn't exist.");
    }
    
    //Override labels if desired
    if(job.X1Label != null && !plot.getDomainAxis(0).getLabel().contains("NaN"))
      plot.getDomainAxis(0).setLabel(job.X1Label.equalsIgnoreCase("None")?"":job.X1Label);
    if(job.Y1Label != null)
      plot.getRangeAxis(0).setLabel(job.Y1Label.equalsIgnoreCase("None")?"":job.Y1Label);
    
    //Format lines and colors
    plot.setDomainGridlinesVisible(job.showGridLines);
    plot.setRangeGridlinesVisible(job.showGridLines);
    formatPlotCollections(job, chart);
    
    //Handle legends
    if(job.removeAllLegends)
      chart.removeLegend();
    
    //Make the file
    try
    {
      FileUtils.createDirectory(job.outputDir);
      String filename = job.outputFilename==null ? job.outputDir+"/"+plotTool.MakeFileName(title)+".jpg" : job.outputDir+"/"+job.outputFilename;
      if(!filename.endsWith(".jpg"))
        filename = filename + ".jpg";
      File JPGFile = new File(filename);
      if(job.imageHeight != null && job.imageWidth != null)
        ChartUtilities.saveChartAsJPEG(JPGFile, chart, job.imageWidth, job.imageHeight);
      else
        ChartUtilities.saveChartAsJPEG(JPGFile, chart, 1600, 800);
    }
    catch (IOException e)
    {
      Log.error(e.getMessage());
    }
  }
  
  protected void formatPlotCollections(PlotJob job, JFreeChart chart)
  {
    Float[] strokes = {2.f,2.f,3.f,4.f,5.f,8.f};
    Color[] variedColors = {Color.red, Color.blue, Color.green, Color.orange, Color.magenta, Color.cyan, Color.gray, new Color(255,165,0), new Color(42,183,136), new Color(87,158,186)};
    XYPlot plot = (XYPlot) chart.getPlot();
    XYLineAndShapeRenderer renderer = (XYLineAndShapeRenderer) plot.getRenderer();
    
    //For Scientific notation
    //NumberFormat formatter = new DecimalFormat("0.######E0");
    
    for(int i = 0; i < plot.getDomainAxisCount(); i++)
    {
      plot.getDomainAxis(i).setLabelFont(new Font("SansSerif", Font.PLAIN, job.fontSize));
      plot.getDomainAxis(i).setTickLabelFont(new Font("SansSerif", Font.PLAIN, 15));
      plot.getDomainAxis(i).setLabelPaint(job.bgColor==Color.red?Color.white:Color.black);
      plot.getDomainAxis(i).setTickLabelPaint(job.bgColor==Color.red?Color.white:Color.black);
    }
    for(int i = 0; i < plot.getRangeAxisCount(); i++)
    {
      plot.getRangeAxis(i).setLabelFont(new Font("SansSerif", Font.PLAIN, job.fontSize));
      plot.getRangeAxis(i).setTickLabelFont(new Font("SansSerif", Font.PLAIN, 15));
      plot.getRangeAxis(i).setLabelPaint(job.bgColor==Color.red?Color.white:Color.black);
      plot.getRangeAxis(i).setTickLabelPaint(job.bgColor==Color.red?Color.white:Color.black);
      NumberAxis rangeAxis = (NumberAxis)plot.getRangeAxis(i);
      //rangeAxis.setNumberFormatOverride(formatter);
    }
    
    //White background outside of plottable area
    chart.setBackgroundPaint(job.bgColor);
    
    plot.setBackgroundPaint(Color.white);
    plot.setDomainGridlinePaint(Color.black);
    plot.setRangeGridlinePaint(Color.black);
    
    plot.setDomainCrosshairVisible(true);
    plot.setRangeCrosshairVisible(true);
    
    renderer.setBaseLegendTextFont(new Font("SansSerif", Font.PLAIN, job.legendFontSize));
    chart.getTitle().setFont(new Font("SansSerif", Font.PLAIN, job.fontSize));
    chart.getTitle().setPaint(job.bgColor==Color.red?Color.white:Color.black);
    
    for(int i = 0, cIndex = 0; i < plot.getSeriesCount(); i++, cIndex++)
    {
      BasicStroke wideLine = new BasicStroke( strokes[i] );
      renderer.setSeriesStroke(i, wideLine); 
      renderer.setBaseShapesVisible(false);
      if(cIndex > 9) cIndex = 0;
      renderer.setSeriesFillPaint(i, variedColors[cIndex]); 
      renderer.setSeriesPaint(i, variedColors[cIndex]);
    }
    
  }

}
