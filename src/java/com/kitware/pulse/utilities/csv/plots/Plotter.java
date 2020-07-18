/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.utilities.csv.plots;

import com.kitware.pulse.utilities.csv.plots.PlotDriver.PlotJob;

public interface Plotter 
{
  void plot(PlotJob job);
}
