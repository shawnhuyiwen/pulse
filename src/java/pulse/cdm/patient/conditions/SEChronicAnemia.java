/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.conditions;

import pulse.cdm.bind.PatientConditions.ChronicAnemiaData;
import pulse.cdm.properties.SEScalar0To1;

public class SEChronicAnemia extends SEPatientCondition
{

  private static final long serialVersionUID = -3748129373804790787L;
  protected SEScalar0To1 reductionFactor;
  
  public SEChronicAnemia()
  {
    reductionFactor = null;
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if (reductionFactor != null)
      reductionFactor.invalidate();
  }
  
  public void copy(SEChronicAnemia other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.reductionFactor != null)
      getReductionFactor().set(other.getReductionFactor());
    else if (reductionFactor != null)
      reductionFactor.invalidate();
  }
  
  public static void load(ChronicAnemiaData src, SEChronicAnemia dst) 
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);
    if(src.hasReductionFactor())
      SEScalar0To1.load(src.getReductionFactor(),dst.getReductionFactor());
  }
  
  public static ChronicAnemiaData unload(SEChronicAnemia src) 
  {
    ChronicAnemiaData.Builder dst = ChronicAnemiaData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChronicAnemia src, ChronicAnemiaData.Builder dst)
  {
    SEPatientCondition.unload(src, dst.getPatientConditionBuilder());
    if (src.hasReductionFactor())
      dst.setReductionFactor(SEScalar0To1.unload(src.reductionFactor));
  }
  
  public boolean hasReductionFactor()
  {
    return reductionFactor == null ? false : reductionFactor.isValid();
  }
  public SEScalar0To1 getReductionFactor()
  {
    if (reductionFactor == null)
      reductionFactor = new SEScalar0To1();
    return reductionFactor;
  }
  
  @Override
  public String toString()
  {
    return "Chronic Anemia" 
        + "\n\tReduction Factor: " + getReductionFactor();
  }

  
}
