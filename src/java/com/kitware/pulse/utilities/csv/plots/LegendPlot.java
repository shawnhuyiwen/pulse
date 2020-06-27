package com.kitware.pulse.utilities.csv.plots;

import java.awt.Graphics2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;

import org.jfree.chart.plot.Plot;
import org.jfree.chart.plot.PlotRenderingInfo;
import org.jfree.chart.plot.PlotState;

//This is a little hacky, but if we want only the legend, just extend Plot() and remove the draw functionality so it makes a blank plot
public class LegendPlot extends Plot
{
  private static final long serialVersionUID = -628117496047176276L;
  
  @Override
  public void draw(Graphics2D arg0, Rectangle2D arg1, Point2D arg2,
      PlotState arg3, PlotRenderingInfo arg4)
  {
    
  }
  @Override
  public String getPlotType()
  {
    return null;
  }
}
