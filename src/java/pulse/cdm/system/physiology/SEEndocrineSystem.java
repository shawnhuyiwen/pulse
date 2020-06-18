/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.system.physiology;

import pulse.cdm.bind.Physiology.EndocrineSystemData;
import pulse.cdm.properties.SEScalarAmountPerTime;
import pulse.cdm.system.SESystem;

public class SEEndocrineSystem extends SEPhysiologySystem implements SESystem
{
  protected SEScalarAmountPerTime insulinSynthesisRate;

  public SEEndocrineSystem()
  {
    insulinSynthesisRate = null;
  }

  @Override
  public void reset()
  {    
    if (insulinSynthesisRate != null)
      insulinSynthesisRate.invalidate();
  }

  public static void load(EndocrineSystemData src, SEEndocrineSystem dst)
  {    
    if (src.hasInsulinSynthesisRate())
      SEScalarAmountPerTime.load(src.getInsulinSynthesisRate(),dst.getInsulinSynthesisRate());
  }

  public static EndocrineSystemData unload(SEEndocrineSystem src)
  {
    EndocrineSystemData.Builder dst = EndocrineSystemData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(SEEndocrineSystem src, EndocrineSystemData.Builder dst)
  {    
    if (src.hasInsulinSynthesisRate())
      dst.setInsulinSynthesisRate(SEScalarAmountPerTime.unload(src.getInsulinSynthesisRate()));
  }

  public boolean hasInsulinSynthesisRate()
  {
    return insulinSynthesisRate == null ? false : insulinSynthesisRate.isValid();
  }
  public SEScalarAmountPerTime getInsulinSynthesisRate()
  {
    if (insulinSynthesisRate == null)
      insulinSynthesisRate = new SEScalarAmountPerTime();
    return insulinSynthesisRate;
  }
}
