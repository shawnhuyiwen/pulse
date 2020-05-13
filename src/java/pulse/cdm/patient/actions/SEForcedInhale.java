/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.ForcedInhaleData;

import pulse.cdm.properties.SEScalar0To1;
import pulse.cdm.properties.SEScalarTime;

public class SEForcedInhale extends SEConsciousRespirationCommand
{
  protected SEScalar0To1 inspiratoryCapacityFraction;
  protected SEScalarTime inhalePeriod;
  protected SEScalarTime holdPeriod;
  protected SEScalarTime releasePeriod;
  
  public SEForcedInhale()
  {
    inspiratoryCapacityFraction = null;
    inhalePeriod = null;
    holdPeriod = null;
    releasePeriod = null;
  }
  
  public void copy(SEForcedInhale other)
  {
    if(this==other)
      return;
    
    if(other.hasInspiratoryCapacityFraction())
      getInspiratoryCapacityFraction().set(other.inspiratoryCapacityFraction);
    if(other.hasInspiratoryCapacityFraction())
      getInspiratoryCapacityFraction().set(other.inspiratoryCapacityFraction);
    if(other.hasInspiratoryCapacityFraction())
      getInspiratoryCapacityFraction().set(other.inspiratoryCapacityFraction);
    if(other.hasInspiratoryCapacityFraction())
      getInspiratoryCapacityFraction().set(other.inspiratoryCapacityFraction);
  }
  
  public SEConsciousRespirationCommand getCopy()
  {
    SEConsciousRespirationCommand command = new SEForcedInhale();
    command.copy(this);
    
    return command;
  }
  
  public void reset()
  {
    super.reset();
    if (inspiratoryCapacityFraction != null)
      inspiratoryCapacityFraction.invalidate();
    if (inhalePeriod != null)
      inhalePeriod.invalidate();
    if (holdPeriod != null)
      holdPeriod.invalidate();
    if (releasePeriod != null)
      releasePeriod.invalidate();
  }
  
  public boolean isValid()
  {
    return hasInspiratoryCapacityFraction() && hasInhalePeriod();
  }
  
  public static void load(ForcedInhaleData src, SEForcedInhale dst)
  {
    //SEConsciousRespirationCommand.load(src.getConsciousRespirationCommand(), dst);
    if(src.hasInspiratoryCapacityFraction())
      SEScalar0To1.load(src.getInspiratoryCapacityFraction(),dst.getInspiratoryCapacityFraction());
    if(src.hasInhalePeriod())
      SEScalarTime.load(src.getInhalePeriod(),dst.getInhalePeriod());
    if(src.hasHoldPeriod())
      SEScalarTime.load(src.getHoldPeriod(),dst.getHoldPeriod());
    if(src.hasReleasePeriod())
      SEScalarTime.load(src.getReleasePeriod(),dst.getReleasePeriod());
  }
  
  public static ForcedInhaleData unload(SEForcedInhale src)
  {
    ForcedInhaleData.Builder dst = ForcedInhaleData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEForcedInhale src, ForcedInhaleData.Builder dst)
  {
    //SEConsciousRespirationCommand.unload(src,dst.getConsciousRespirationCommand());    
    if (src.hasInspiratoryCapacityFraction())
      dst.setInspiratoryCapacityFraction(SEScalar0To1.unload(src.inspiratoryCapacityFraction));
    if (src.hasInhalePeriod())
      dst.setInhalePeriod(SEScalarTime.unload(src.inhalePeriod));
    if (src.hasHoldPeriod())
      dst.setHoldPeriod(SEScalarTime.unload(src.holdPeriod));
    if (src.hasReleasePeriod())
      dst.setReleasePeriod(SEScalarTime.unload(src.releasePeriod));
  }
  
  public boolean hasInspiratoryCapacityFraction()
  {
    return inspiratoryCapacityFraction != null;
  }
  public SEScalar0To1 getInspiratoryCapacityFraction()
  {
    if (inspiratoryCapacityFraction == null)
      inspiratoryCapacityFraction = new SEScalar0To1();
    return inspiratoryCapacityFraction;
  }
  
  public boolean hasInhalePeriod()
  {
    return inhalePeriod != null;
  }
  public SEScalarTime getInhalePeriod()
  {
    if (inhalePeriod == null)
      inhalePeriod = new SEScalarTime();
    return inhalePeriod;
  }
  
  public boolean hasHoldPeriod()
  {
    return holdPeriod != null;
  }
  public SEScalarTime getHoldPeriod()
  {
    if (holdPeriod == null)
      holdPeriod = new SEScalarTime();
    return holdPeriod;
  }
  
  public boolean hasReleasePeriod()
  {
    return releasePeriod != null;
  }
  public SEScalarTime getReleasePeriod()
  {
    if (releasePeriod == null)
      releasePeriod = new SEScalarTime();
    return releasePeriod;
  }
  
  public String toString()
  {
      return "Forced Inhale" 
          + "\n\tInspiratoryCapacityFraction: " + (hasInspiratoryCapacityFraction() ? getInspiratoryCapacityFraction() : "Not Specified")
          + "\n\tInhalePeriod: " + (hasInhalePeriod() ? getInhalePeriod() : "Not Specified")
          + "\n\tHoldPeriod: " + (hasHoldPeriod() ? getHoldPeriod() : "Not Specified")
          + "\n\tReleasePeriod: " + (hasReleasePeriod() ? getReleasePeriod() : "Not Specified");
  }
}
