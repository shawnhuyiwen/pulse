/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.utilities.csv.plots;

import com.kitware.physiology.datamodel.substance.SESubstanceManager;
import com.kitware.physiology.utilities.LogListener;

public interface Plotter 
{
  void plot(LogListener listener, SESubstanceManager subMgr);
}
