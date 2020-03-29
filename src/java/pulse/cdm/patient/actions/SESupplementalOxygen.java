/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.SupplementalOxygenData;
import pulse.cdm.bind.PatientActions.SupplementalOxygenData.eDevice;
import pulse.cdm.properties.SEScalarVolume;
import pulse.cdm.properties.SEScalarVolumePerTime;

public  class SESupplementalOxygen extends SEPatientAction
{
  protected eDevice               device;
  protected SEScalarVolumePerTime flow;
  protected SEScalarVolume        volume;
  
  public SESupplementalOxygen()
  {
    device = null;
    flow = null;
    volume = null;
  }
  
  public void copy(SESupplementalOxygen other)
  {
    if(this==other)
      return;
    super.copy(other);
    device = other.device;
    
    if (other.flow != null)
      getFlow().set(other.flow);
    else if (flow != null)
      flow.invalidate();
    
    if (other.volume != null)
      getVolume().set(other.volume);
    else if (volume != null)
      volume.invalidate();
  }
  
  public void reset()
  {
    super.reset();
    device = null;
    if (flow != null)
      flow.invalidate();
    if (volume != null)
      volume.invalidate();
  }
  
  public boolean isValid()
  {
    return hasDevice();
  }
  
  public static void load(SupplementalOxygenData src, SESupplementalOxygen dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getDevice()!=eDevice.UNRECOGNIZED)
      dst.device = src.getDevice();
    if(src.hasFlow())
        SEScalarVolumePerTime.load(src.getFlow(),dst.getFlow());
      if(src.hasVolume())
        SEScalarVolume.load(src.getVolume(),dst.getVolume());
  }
  
  public static SupplementalOxygenData unload(SESupplementalOxygen src)
  {
    SupplementalOxygenData.Builder dst = SupplementalOxygenData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESupplementalOxygen src, SupplementalOxygenData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasDevice())
      dst.setDevice(src.device);
    if (src.hasFlow())
      dst.setFlow(SEScalarVolumePerTime.unload(src.flow));
    if (src.hasVolume())
      dst.setVolume(SEScalarVolume.unload(src.volume));
  }
  
  public eDevice getDevice()
  {
    return device;
  }
  public void setDevice(eDevice d)
  {
    device = d;
  }
  public boolean hasDevice()
  {
    return device == null ? false : true;
  }
  
  public boolean hasFlow()
  {
    return flow == null ? false : flow.isValid();
  }
  public SEScalarVolumePerTime getFlow()
  {
    if (flow == null)
      flow = new SEScalarVolumePerTime();
    return flow;
  }
  
  public boolean hasVolume()
  {
    return volume ==  null ? false : volume.isValid();
  }
  public SEScalarVolume getVolume()
  {
    if (volume == null)
      volume = new SEScalarVolume();
    return volume;
  }
  
  public String toString() 
  {
    return "Supplemental Oxygen"
        + "\n\tDevice: " + getDevice()
        + "\n\tFlow: " + getFlow() 
        + "\n\tVolume: " + getVolume();
  }
}
