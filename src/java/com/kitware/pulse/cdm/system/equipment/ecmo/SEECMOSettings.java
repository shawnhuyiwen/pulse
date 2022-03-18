/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.ecmo;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.ECMO.ECMOSettingsData;
import com.kitware.pulse.cdm.bind.ECMO.eECMOCannulationLocation;
import com.kitware.pulse.cdm.bind.Substance.SubstanceConcentrationData;
import com.kitware.pulse.cdm.bind.Substance.SubstanceFractionData;
import com.kitware.pulse.cdm.properties.*;
import com.kitware.pulse.cdm.substance.SESubstanceConcentration;
import com.kitware.pulse.cdm.substance.SESubstanceFraction;

public class SEECMOSettings
{
  protected eECMOCannulationLocation          inflowLocation;
  protected eECMOCannulationLocation          outflowLocation;
  protected SEScalarVolume                    oxygenatorVolume;
  protected SEScalarVolumePerTime             transfusionFlow;
  protected String                            substanceCompound;
  
  protected List<SESubstanceConcentration>    substanceConcentrations;


  public SEECMOSettings()
  {
    inflowLocation = null;
    outflowLocation = null;
    oxygenatorVolume = null;
    transfusionFlow = null;
    substanceCompound = null;

    this.substanceConcentrations=new ArrayList<SESubstanceConcentration>();
  }

  public void clear()
  {
    inflowLocation = null;
    outflowLocation = null;
    if (oxygenatorVolume != null)
      oxygenatorVolume.invalidate();
    if (transfusionFlow != null)
      transfusionFlow.invalidate();
    substanceCompound = null;
    
    this.substanceConcentrations.clear();
  }

  public void copy(SEECMOSettings from)
  {
    clear();
    if(from.inflowLocation!=null && from.inflowLocation != eECMOCannulationLocation.UNRECOGNIZED)
    	this.inflowLocation=from.inflowLocation;
    if(from.outflowLocation!=null && from.outflowLocation != eECMOCannulationLocation.UNRECOGNIZED)
      this.outflowLocation=from.outflowLocation;
    if(from.hasOxygenatorVolume())
      this.getOxygenatorVolume().set(from.getOxygenatorVolume());
    if(from.hasTransfusionFlow())
      this.getTransfusionFlow().set(from.getTransfusionFlow());
    substanceCompound = from.substanceCompound;
    
    if(from.substanceConcentrations!=null)
    {
      SESubstanceConcentration mine;
      for(SESubstanceConcentration sc : from.substanceConcentrations)
      {
        mine=this.createSubstanceConcentration(sc.getSubstance());
        if(sc.hasConcentration())
          mine.getConcentration().set(sc.getConcentration());
      }
    }
  }

  public static void load(ECMOSettingsData src, SEECMOSettings dst)
  {
    dst.clear();
    if (src.getInflowLocation()!=eECMOCannulationLocation.UNRECOGNIZED)
      dst.setInflowLocation(src.getInflowLocation());
    if (src.getOutflowLocation()!=eECMOCannulationLocation.UNRECOGNIZED)
      dst.setOutflowLocation(src.getOutflowLocation());
    if (src.hasOxygenatorVolume())
      SEScalarVolume.load(src.getOxygenatorVolume(), dst.getOxygenatorVolume());
    if (src.hasTransfusionFlow())
      SEScalarVolumePerTime.load(src.getTransfusionFlow(), dst.getTransfusionFlow());
    dst.substanceCompound = src.getSubstanceCompound();
    
    if(src.getSubstanceConcentrationsList()!=null)
    {
      for(SubstanceConcentrationData subData : src.getSubstanceConcentrationsList())
      {
        SEScalarMassPerVolume.load(subData.getConcentration(),dst.createSubstanceConcentration(subData.getName()).getConcentration());
      }
    }
  }
  public static ECMOSettingsData unload(SEECMOSettings src)
  {
    ECMOSettingsData.Builder dst = ECMOSettingsData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEECMOSettings src, ECMOSettingsData.Builder dst)
  {
    if (src.hasInflowLocation())
      dst.setInflowLocation(src.getInflowLocation());
    if (src.hasOutflowLocation())
      dst.setOutflowLocation(src.getOutflowLocation());
    if(src.hasOxygenatorVolume())
      dst.setOxygenatorVolume(SEScalarVolume.unload(src.getOxygenatorVolume()));
    if(src.hasTransfusionFlow())
      dst.setTransfusionFlow(SEScalarVolumePerTime.unload(src.getTransfusionFlow()));
    dst.setSubstanceCompound(src.substanceCompound);
    
    for(SESubstanceConcentration sc : src.substanceConcentrations)
      dst.addSubstanceConcentrations(SESubstanceConcentration.unload(sc));
  }
  
  public eECMOCannulationLocation getInflowLocation()
  {
    return inflowLocation;
  }
  public void setInflowLocation(eECMOCannulationLocation c)
  {
    inflowLocation = (c == eECMOCannulationLocation.UNRECOGNIZED) ? null : c;
  }
  public boolean hasInflowLocation()
  {
    return inflowLocation != null && inflowLocation != eECMOCannulationLocation.UNRECOGNIZED;
  }
  
  public eECMOCannulationLocation getOutflowLocation()
  {
    return outflowLocation;
  }
  public void setOutflowLocation(eECMOCannulationLocation c)
  {
    outflowLocation = (c == eECMOCannulationLocation.UNRECOGNIZED) ? null : c;
  }
  public boolean hasOutflowLocation()
  {
    return outflowLocation != null && outflowLocation != eECMOCannulationLocation.UNRECOGNIZED;
  }
  
  public SEScalarVolume getOxygenatorVolume()
  {
    if (oxygenatorVolume == null)
      oxygenatorVolume = new SEScalarVolume();
    return oxygenatorVolume;
  }
  public boolean hasOxygenatorVolume()
  {
    return oxygenatorVolume == null ? false : oxygenatorVolume.isValid();
  }
  
  public SEScalarVolumePerTime getTransfusionFlow()
  {
    if (transfusionFlow == null)
      transfusionFlow = new SEScalarVolumePerTime();
    return transfusionFlow;
  }
  public boolean hasTransfusionFlow()
  {
    return transfusionFlow == null ? false : transfusionFlow.isValid();
  }
  
  public String getSubstanceCompound()
  {
    return substanceCompound;
  }
  public void setSubstanceCompound(String c)
  {
    substanceCompound = c;
  }
  public boolean hasSubstanceCompound()
  {
    return substanceCompound != null;
  }

  public SESubstanceConcentration createSubstanceConcentration(String substance)
  {
    return getSubstanceConcentration(substance);
  }
  public SESubstanceConcentration getSubstanceConcentration(String substance)
  {
    for(SESubstanceConcentration sc : this.substanceConcentrations)
    {
      if(sc.getSubstance().equals(substance))
      {
        return sc;
      }
    }    
    SESubstanceConcentration sc = new SESubstanceConcentration(substance);
    this.substanceConcentrations.add(sc);
    return sc;
  }
  public boolean hasSubstanceConcentration()
  {
    return !this.substanceConcentrations.isEmpty();
  }
  public boolean hasSubstanceConcentration(String substance)
  {
    for(SESubstanceConcentration sc : this.substanceConcentrations)
    {
      if(sc.getSubstance().equals(substance))
      {
        return true;
      }
    }
    return false;
  }
  public List<SESubstanceConcentration> getSubstanceConcentration()
  {
    return Collections.unmodifiableList(this.substanceConcentrations);
  }
  public void removeSubstanceConcentration(String substance)
  {
    for(SESubstanceConcentration sc : this.substanceConcentrations)
    {
      if(sc.getSubstance().equals(substance))
      {
        this.substanceConcentrations.remove(sc);
        return;
      }
    }  
  }

  @Override
  public String toString()
  {
    String str = "ECMO Settings"
        + "\n\tInflowLocation: " + (hasInflowLocation() ? getInflowLocation() : "NotProvided")
        + "\n\tOutflowLocation: " + (hasOutflowLocation() ? getOutflowLocation() : "NotProvided")
        + "\n\tOxygenatorVolume: " + (hasOxygenatorVolume() ? getOxygenatorVolume().toString() : "Not Provided")
        + "\n\tTransfusionFlow: " + (hasTransfusionFlow() ? getTransfusionFlow().toString() : "Not Provided")
        + "\n\tSubstanceCompound: " + (hasSubstanceCompound() ? getSubstanceCompound().toString() : "Not Provided");

    for(SESubstanceConcentration sc : this.substanceConcentrations)
    str += "\n\t"+sc.toString();
    return str;
  }
}