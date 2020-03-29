/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.ForcedExhaleData;

import pulse.cdm.properties.SEScalar0To1;
import pulse.cdm.properties.SEScalarTime;

public class SEForcedExhale extends SEConsciousRespirationCommand
{
  protected SEScalar0To1 expiratoryReserveVolumeFraction;
  protected SEScalarTime exhalePeriod;
  protected SEScalarTime holdPeriod;
  protected SEScalarTime releasePeriod;
  
  public SEForcedExhale()
  {
    expiratoryReserveVolumeFraction = null;
    exhalePeriod = null;
    holdPeriod = null;
    releasePeriod = null;
  }
  
  public void copy(SEForcedExhale other)
  {
    if(this==other)
      return;
    
    if(other.hasExpiratoryReserveVolumeFraction())
      getExpiratoryReserveVolumeFraction().set(other.expiratoryReserveVolumeFraction);
    if(other.hasExpiratoryReserveVolumeFraction())
      getExpiratoryReserveVolumeFraction().set(other.expiratoryReserveVolumeFraction);
    if(other.hasExpiratoryReserveVolumeFraction())
      getExpiratoryReserveVolumeFraction().set(other.expiratoryReserveVolumeFraction);
    if(other.hasExpiratoryReserveVolumeFraction())
      getExpiratoryReserveVolumeFraction().set(other.expiratoryReserveVolumeFraction);
  }
  
  public SEConsciousRespirationCommand getCopy()
  {
    SEConsciousRespirationCommand command = new SEForcedExhale();
    command.copy(this);
    
    return command;
  }
  
  public void reset()
  {
    super.reset();
    if (expiratoryReserveVolumeFraction != null)
      expiratoryReserveVolumeFraction.invalidate();
    if (exhalePeriod != null)
      exhalePeriod.invalidate();
    if (holdPeriod != null)
      holdPeriod.invalidate();
    if (releasePeriod != null)
      releasePeriod.invalidate();
  }
  
  public boolean isValid()
  {
    return hasExpiratoryReserveVolumeFraction() && hasExhalePeriod();
  }
  
  public static void load(ForcedExhaleData src, SEForcedExhale dst)
  {
    //SEConsciousRespirationCommand.load(src.getConsciousRespirationCommand(), dst);
    if(src.hasExpiratoryReserveVolumeFraction())
      SEScalar0To1.load(src.getExpiratoryReserveVolumeFraction(),dst.getExpiratoryReserveVolumeFraction());
    if(src.hasExhalePeriod())
      SEScalarTime.load(src.getExhalePeriod(),dst.getExhalePeriod());
    if(src.hasHoldPeriod())
      SEScalarTime.load(src.getHoldPeriod(),dst.getHoldPeriod());
    if(src.hasReleasePeriod())
      SEScalarTime.load(src.getReleasePeriod(),dst.getReleasePeriod());
  }
  
  public static ForcedExhaleData unload(SEForcedExhale src)
  {
    ForcedExhaleData.Builder dst = ForcedExhaleData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEForcedExhale src, ForcedExhaleData.Builder dst)
  {
    //SEConsciousRespirationCommand.unload(src,dst.getConsciousRespirationCommand());    
    if (src.hasExpiratoryReserveVolumeFraction())
      dst.setExpiratoryReserveVolumeFraction(SEScalar0To1.unload(src.expiratoryReserveVolumeFraction));
    if (src.hasExhalePeriod())
      dst.setExhalePeriod(SEScalarTime.unload(src.exhalePeriod));
    if (src.hasHoldPeriod())
      dst.setHoldPeriod(SEScalarTime.unload(src.holdPeriod));
    if (src.hasReleasePeriod())
      dst.setReleasePeriod(SEScalarTime.unload(src.releasePeriod));
  }
  
  public boolean hasExpiratoryReserveVolumeFraction()
  {
    return expiratoryReserveVolumeFraction != null;
  }
  public SEScalar0To1 getExpiratoryReserveVolumeFraction()
  {
    if (expiratoryReserveVolumeFraction == null)
      expiratoryReserveVolumeFraction = new SEScalar0To1();
    return expiratoryReserveVolumeFraction;
  }
  
  public boolean hasExhalePeriod()
  {
    return exhalePeriod != null;
  }
  public SEScalarTime getExhalePeriod()
  {
    if (exhalePeriod == null)
      exhalePeriod = new SEScalarTime();
    return exhalePeriod;
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
      return "Forced Exhale" 
          + "\n\tExpiratoryReserveVolumeFraction: " + (hasExpiratoryReserveVolumeFraction() ? getExpiratoryReserveVolumeFraction() : "Not Specified")
          + "\n\tExhalePeriod: " + (hasExhalePeriod() ? getExhalePeriod() : "Not Specified")
          + "\n\tHoldPeriod: " + (hasHoldPeriod() ? getHoldPeriod() : "Not Specified")
          + "\n\tReleasePeriod: " + (hasReleasePeriod() ? getReleasePeriod() : "Not Specified");
  }
}
