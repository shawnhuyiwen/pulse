/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.utilities.csv.plots;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Paint;
import java.io.File;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartUtils;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.LogarithmicAxis;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.DatasetRenderingOrder;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.SeriesRenderingOrder;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import com.kitware.pulse.utilities.DoubleUtils;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.StringUtils;
import com.kitware.pulse.utilities.csv.CSVContents;

/**
 * @author abray
 * This is a generic results plot tool.
 * It just plots every heading based on a generic template.
 */

public class CSVPlotTool
{
  //font size for tick labels and legend
  public static Font smallFont = new Font("SansSerif", Font.PLAIN, 15);
  //font size for X,Y axis title and plot title
  public static Font largeFont = new Font("SansSerif", Font.PLAIN, 22);  
  //Expected line
  public static float expectedLineWidth = 4.5f;
  public static Paint expectedLineColor = Color.BLACK;
  //Computed Line
  public static Paint computedLineColor = Color.RED;
  public static float computedLineWidth = 2.0f;

  public static void main(String[] args)
  {
    if(args.length<1)
    {
      Log.fatal("Expected inputs : [csv filename]");
      return;
    }
    CSVPlotTool t = new CSVPlotTool();
    
    List<String> csvFiles = new ArrayList<>();
    
    for(String arg : args)
    {
      if(arg.endsWith(".csv"))
        csvFiles.add(args[0]);
      else
        csvFiles.addAll(FileUtils.findFiles(arg, "csv", true));
    }
    if(csvFiles.isEmpty())
    {
      Log.fatal("No csv files found, check provided inputs...");
      return;
    }
    for(String csvFile : csvFiles)
    {
      File f = new File(csvFile);
      if(!f.exists())
      {
        Log.fatal("Input file cannot be found : " + csvFile);
        continue;
      }
      Log.info("Creating plots for "+csvFile);
      String path = f.getAbsolutePath().replaceAll("\\\\", "/");
      path = path.substring(0,path.lastIndexOf("/"));
      String name = f.getName().substring(0,f.getName().lastIndexOf("."));
      String plotDir = path+"/"+name+"_plots";
      Log.info("Creating plots at "+plotDir);
      t.graphResults(csvFile,plotDir);
    }
  }

  public boolean graphResults(String csvFile, String graphLocation)
  {
    try
    {
      File f = new File(csvFile);
      if(!f.exists())
      {
        Log.fatal("Input file cannot be found");
        return false;
      }

      File dir = new File(graphLocation);
      if(dir.exists() && dir.isDirectory())
      {
        for(String g : dir.list())
          if(g.endsWith(".jpg"))
            FileUtils.delete(g);
      }      
      Thread.sleep(250);
      FileUtils.createDirectory(graphLocation);

      CSVContents result = new CSVContents(csvFile);
      Map<String, List<Double>> data = new HashMap<>();
      if(!result.readAll(data))
        return false;
      if(!data.containsKey("Time(s)"))
      {
        Log.error("CSV file, "+csvFile+", does not conteain a Time(s) header to plot against");
        return false;
      }
      
      for(String header : data.keySet())
      {
        if(header.equals("Time(s)"))
          continue;
        createGraph(graphLocation, Color.white, header + "_vs_Time","Time",header,createXYSeries(header,data.get("Time(s)"),data.get(header)));
      }
    }
    catch(Exception ex)
    {
      Log.error("Unable to graph results file "+csvFile);
      return false;
    }
    return true;
  }

  public void createGraph(String toDir, Paint color, String title, String XAxisLabel, String YAxisLabel, XYSeries...xyData)
  {
    new File(toDir).mkdir();
    
    Log.info("Creating Graph "+toDir+"/"+title);
    double resMin0    = 1.e6;
    double resMax0   = -1.e6;
    double resMin1    = 1.e6;
    double resMax1   = -1.e6;

    XYSeriesCollection dataSet = new XYSeriesCollection();
    for(XYSeries data : xyData)
    {
      if(data != null && !data.isEmpty())
        dataSet.addSeries(data);
    }

    JFreeChart chart = ChartFactory.createXYLineChart(
        title ,           // chart title
        XAxisLabel,         // x axis label
        YAxisLabel,         // y axis label
        dataSet,                   // data
        PlotOrientation.VERTICAL,  // orientation
        true,                      // include legend
        true,                      // tooltips
        false                      // urls
        );

    XYPlot plot = (XYPlot) chart.getPlot();
    plot.setDatasetRenderingOrder(DatasetRenderingOrder.FORWARD);
    plot.setSeriesRenderingOrder(SeriesRenderingOrder.FORWARD);
/* I have residual and error plots turned off, there are some plots that contain these names in their title, and I don't want this code running, need a better way to see if we are doing a special plot rather than title contents
    if(title.contains("Residual"))
    {
      // Make plot symmetric about x axis
      resMax0 = xyData[0].getMaxY();
      resMin0 = xyData[0].getMinY();
      if (Math.abs(xyData[0].getMinY()) > Math.abs(xyData[0].getMaxY()))  resMax0 = Math.abs(resMin0);
      if (Math.abs(xyData[0].getMaxY()) > Math.abs(xyData[0].getMinY()))  resMin0 = -1.0*Math.abs(resMax0);
      if((resMin0==0.0) && (resMax0==0.0))
      {
        resMin0 = -0.00001;
        resMax0 =  0.00001;
      }      
      ValueAxis yAxis = plot.getRangeAxis();
      yAxis.setRange(resMin0 + 0.05*resMin0, resMax0 + 0.05*resMax0);//5% buffer so we can see top and bottom clearly
    }
    else if(title.contains("Error"))
    {
      // Make plot symmetric about x axis
      resMax0 = xyData[0].getMaxY();
      resMin0 = xyData[0].getMinY();
      if((resMin0==0.0) && (resMax0==0.0))
      {
        resMin0 = -0.00001;
        resMax0 =  0.00001;
      }
      if(resMin0>=0.0) resMin0 = -0.01;
      ValueAxis yAxis = plot.getRangeAxis();
      yAxis.setRange(resMin0 + 0.05*resMin0, resMax0 + 0.05*resMax0);//5% buffer so we can see top and bottom clearly

      /*
      yAxis.setTickLabelPaint(new Color(1,0,0));
      yAxis.setTickMarkPaint(new Color(1,0,0));
      yAxis.setAxisLinePaint(new Color(1,0,0));
      yAxis.setLabelPaint(new Color(1,0,0));

      ValueAxis xAxis = plot.getDomainAxis();
      xAxis.setTickLabelPaint(new Color(1,0,0));
      xAxis.setTickMarkPaint(new Color(1,0,0));
      yAxis.setAxisLinePaint(new Color(1,0,0));
      yAxis.setLabelPaint(new Color(1,0,0));
       *
    }
    else
*/
    {
      if(title.indexOf("Hemoglobin-GlomerularFilterability")>-1)
        System.out.println("stop");
      if(xyData.length > 1)
      {        
        // Make plot symmetric about x axis
        resMax0 = xyData[0].getMaxY();
        resMin0 = xyData[0].getMinY();
        resMax1 = xyData[1].getMaxY();
        resMin1 = xyData[1].getMinY();
        if(resMin1<resMin0) resMin0=resMin1;
        if(resMax1>resMax0) resMax0=resMax1;
        if(DoubleUtils.isZero(resMin0))
          resMin0 = -0.001;
        if(DoubleUtils.isZero(resMax0))
          resMax0 =  0.001;
        if(resMin0>=0.0) resMin0 = -0.01;
        if(YAxisLabel.indexOf("PlasmaConcentration")>-1)
          plot.setRangeAxis(new LogarithmicAxis("Log("+YAxisLabel+")")); 
        else
        {
          ValueAxis yAxis = plot.getRangeAxis();
          yAxis.setRange(resMin0 + 0.05*resMin0, resMax0 + 0.15*Math.abs(resMax0));//5% buffer so we can see top and bottom clearly
        }
        String NaNCheck = "";
        if(Double.isNaN(resMax0)||Double.isNaN(resMin0))
          NaNCheck += "Expected is NaN ";
        if(Double.isNaN(resMax1)||Double.isNaN(resMin1))
          NaNCheck += "Computed is NaN ";
        if(!NaNCheck.isEmpty())
          plot.getDomainAxis().setLabel(NaNCheck);
      }
      else
      {
        // Make plot symmetric about x axis
        resMax0 = xyData[0].getMaxY();
        resMin0 = xyData[0].getMinY();
        if(Double.isNaN(resMax0)||Double.isNaN(resMin0))
          plot.getDomainAxis().setLabel("Computed is NaN");
        if(DoubleUtils.isZero(resMin0))
          resMin0 = -0.001;
        if(DoubleUtils.isZero(resMax0))
          resMax0 =  0.001;
        if(resMin0>=0.0) resMin0 = -0.01;
        if(YAxisLabel.indexOf("PlasmaConcentration")>-1)
          plot.setRangeAxis(new LogarithmicAxis("Log("+YAxisLabel+")"));
        else
        {
          ValueAxis yAxis = plot.getRangeAxis();
          yAxis.setRange(resMin0 + 0.05*resMin0, resMax0 + 0.15*Math.abs(resMax0));//5% buffer so we can see top and bottom clearly
        }
      }
    }

    formatXYPlot(chart,color);
    //Changing line widths and colors
    XYItemRenderer r = plot.getRenderer();
    r.setSeriesStroke(0, new BasicStroke( expectedLineWidth ));
    r.setSeriesStroke(1, new BasicStroke( computedLineWidth ));
    XYLineAndShapeRenderer renderer = (XYLineAndShapeRenderer) plot.getRenderer();
    if(xyData.length>1)
    {
      //renderer.setSeriesStroke(//makes a dashed line
      //    0, //argument below float[]{I,K} -> alternates between solid and opaque (solid for I, opaque for K)
      //    new BasicStroke(expectedlineWidth, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 1.0f, new float[]{15.0f, 30.0f}, 0.0f)
      //    );
      //renderer.setDrawSeriesLineAsPath(true);
      //renderer.setUseFillPaint(true);
    }
    renderer.setDefaultShapesVisible(false);//setBaseShapesVisible(false);
    renderer.setSeriesFillPaint(0, expectedLineColor );
    renderer.setSeriesPaint(0, expectedLineColor);
    renderer.setSeriesFillPaint(1, computedLineColor);
    renderer.setSeriesPaint(1, computedLineColor);

    try
    {
      if(toDir==null||toDir.isEmpty())
        toDir=".";
      File JPGFile = new File(toDir+"/"+MakeFileName(title)+".jpg");
      ChartUtils.saveChartAsJPEG(JPGFile, chart, 1600, 800);
    }
    catch (IOException e)
    {
      Log.error(e.getMessage());
    }
  }
  
  public String MakeFileName(String title)
  {
    String name = StringUtils.removeParens(title);
    name=name.replaceAll(" ", "_");
    name=name.replaceAll("[/\\\\]", "_Per_");
    return name;
  }

  public XYSeries createXYSeries(String name, List<Double> x, List<Double>y)
  {
    if(y==null || x==null)
    {
      Log.error("null data provided");
      return null;
    }
    if(y.size()!=x.size())
    {
      Log.error("axis data is not the same size");
    }    
    XYSeries series = new XYSeries(name);
    for(int i=0; i<x.size();i++) 
    {
      if(i>=y.size())
        series.add(x.get(i),(Double)Double.NaN);
      else
        series.add(x.get(i),y.get(i));
    }
    return series;
  }

  public void formatXYPlot(JFreeChart chart, Paint bgColor)
  {
    XYPlot plot = (XYPlot) chart.getPlot();
    
    //For Scientific notation
    NumberFormat formatter = new DecimalFormat("0.###E0");
    
    for(int i = 0; i < plot.getDomainAxisCount(); i++)
    {
      plot.getDomainAxis(i).setLabelFont(largeFont);
      plot.getDomainAxis(i).setTickLabelFont(smallFont);
      plot.getDomainAxis(i).setLabelPaint(bgColor==Color.red?Color.white:Color.black);
      plot.getDomainAxis(i).setTickLabelPaint(bgColor==Color.red?Color.white:Color.black);
    }
    for(int i = 0; i < plot.getRangeAxisCount(); i++)
    {
      plot.getRangeAxis(i).setLabelFont(largeFont);
      plot.getRangeAxis(i).setTickLabelFont(smallFont);
      plot.getRangeAxis(i).setLabelPaint(bgColor==Color.red?Color.white:Color.black);
      plot.getRangeAxis(i).setTickLabelPaint(bgColor==Color.red?Color.white:Color.black);
      NumberAxis rangeAxis = (NumberAxis)plot.getRangeAxis(i);
      rangeAxis.setNumberFormatOverride(formatter);
    }

    //White background outside of plottable area
    chart.setBackgroundPaint(bgColor);

    plot.setBackgroundPaint(Color.white);
    plot.setDomainGridlinePaint(Color.black);
    plot.setRangeGridlinePaint(Color.black);

    plot.setDomainCrosshairVisible(true);
    plot.setRangeCrosshairVisible(true);

    chart.getLegend().setItemFont(smallFont);
    chart.getTitle().setFont(largeFont);
    chart.getTitle().setPaint(bgColor==Color.red?Color.white:Color.black);
  }

}
