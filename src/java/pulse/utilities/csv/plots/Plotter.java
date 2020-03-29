/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.utilities.csv.plots;

import pulse.cdm.substance.SESubstanceManager;
import pulse.utilities.LogListener;

public interface Plotter 
{
  void plot(LogListener listener, SESubstanceManager subMgr);
}
