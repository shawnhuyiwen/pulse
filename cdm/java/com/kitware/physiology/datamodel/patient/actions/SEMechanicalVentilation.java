/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.patient.actions;

import java.util.ArrayList;
import java.util.List;

import com.kitware.physiology.cdm.PatientActions.MechanicalVentilationData;
import com.kitware.physiology.cdm.Enums.eSwitch;
import com.kitware.physiology.cdm.Substance.SubstanceData;
import com.kitware.physiology.cdm.Substance.SubstanceData.eState;
import com.kitware.physiology.cdm.Substance.SubstanceFractionData;

import com.kitware.physiology.utilities.Log;
import com.kitware.physiology.datamodel.properties.*;
import com.kitware.physiology.datamodel.substance.SESubstance;
import com.kitware.physiology.datamodel.substance.SESubstanceConcentration;
import com.kitware.physiology.datamodel.substance.SESubstanceFraction;
import com.kitware.physiology.datamodel.substance.SESubstanceManager;
import com.kitware.physiology.utilities.DoubleUtils;

public class SEMechanicalVentilation extends SEPatientAction
{
  protected SEScalarVolumePerTime flow;
  protected SEScalarPressure      pressure;
  protected eSwitch               state;

  protected List<SESubstanceFraction>  gasFractions;
  protected List<SESubstanceConcentration>   aerosols;

  public SEMechanicalVentilation()
  {
    clear();
  }
  
  public void clear()
  {
    flow = null;
    pressure = null;
    state = eSwitch.Off;
    this.gasFractions=new ArrayList<SESubstanceFraction>();
    this.aerosols=new ArrayList<SESubstanceConcentration>();
  }

  public void reset()
  {
    state = eSwitch.Off;
    if (flow != null)
      flow.invalidate();
    if (pressure != null)
      pressure.invalidate();
    this.gasFractions.clear();
    this.aerosols.clear();
  }

  public void copy(SEMechanicalVentilation from)
  {
    reset();

    setState(from.getState());
    if(from.hasFlow())
      getFlow().set(from.getFlow());
    if(from.hasPressure())
      getPressure().set(from.getPressure());    
    
    if(from.gasFractions!=null)
    {
      SESubstanceFraction mine;
      for(SESubstanceFraction sf : from.gasFractions)
      {
        mine=this.createGasFraction(sf.getSubstance());
        if(sf.hasAmount())
          mine.getAmount().set(sf.getAmount());
      }
    }   
    
    if(from.aerosols!=null)
    {
      SESubstanceConcentration mine;
      for(SESubstanceConcentration sc : from.aerosols)
      {
        mine=this.createAerosol(sc.getSubstance());
        if(sc.hasConcentration())
          mine.getConcentration().set(sc.getConcentration());
      }
    }    
  }
  
  public boolean isValid()
  {
    if (getState() == eSwitch.Off)
      return true;
    if (!hasGasFraction())
    {
      Log.error("Mechanical Ventilation must have gas fractions.");
      return false;
    }
    else
    {
      double total = 0;
      for (SESubstanceFraction sf : gasFractions)
      {
        total += sf.getAmount().getValue();
      }
      if (!DoubleUtils.equals(1.0, total))
      {
        Log.error("Mechanical Ventilation Gas fractions do not sum to 1");
        return false;
      }
    }
    if (!hasPressure() && !hasFlow())
    {
      Log.error("Mechanical Ventilation must have pressure and/or flow");
      return false;
    }
    return true;
  }

  public static void load(MechanicalVentilationData src, SEMechanicalVentilation dst, SESubstanceManager substances)
  {
    SEPatientAction.load(src.getPatientAction(), dst);   
    if(src.getState()!=eSwitch.UNRECOGNIZED && src.getState()!=eSwitch.NullSwitch)
    	dst.setState(src.getState());
    if (src.hasFlow())
      SEScalarVolumePerTime.load(src.getFlow(),dst.getFlow());
    if (src.hasPressure())
      SEScalarPressure.load(src.getPressure(),dst.getPressure());
    
    SESubstance sub;
    for(SubstanceFractionData subData : src.getGasFractionList())
    {
      sub = substances.getSubstance(subData.getName());
      if(sub == null)
      {
        Log.error("Substance does not exist : "+subData.getName());
      }
      if(sub.getState() != eState.Gas)
      {
        Log.error("Gas Fraction substance must be a gas, "+subData.getName()+" is not a gas...");
      }
      SEScalar0To1.load(subData.getAmount(),dst.createGasFraction(sub).getAmount());
    }
  }

  public static MechanicalVentilationData unload(SEMechanicalVentilation src)
  {
    MechanicalVentilationData.Builder dst = MechanicalVentilationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(SEMechanicalVentilation src, MechanicalVentilationData.Builder dst)
  {
    dst.setState(src.state);
    if (src.hasFlow())
      dst.setFlow(SEScalarVolumePerTime.unload(src.flow));
    if (src.hasPressure())
      dst.setPressure(SEScalarPressure.unload(src.pressure));
    
    for(SESubstanceFraction gf : src.gasFractions)
      dst.addGasFraction(SESubstanceFraction.unload(gf));
  }

  public eSwitch getState()
  {
    return state;
  }
  public void setState(eSwitch s)
  {
  	this.state = (s==eSwitch.NullSwitch) ? eSwitch.Off : s;
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

  public boolean hasPressure()
  {
    return pressure == null ? false : pressure.isValid();
  }
  public SEScalarPressure getPressure()
  {
    if (pressure == null)
      pressure = new SEScalarPressure();
    return pressure;
  }
  
  public SESubstanceFraction createGasFraction(SESubstance substance)
  {
    return getGasFraction(substance);
  }
  public SESubstanceFraction getGasFraction(SESubstance substance)
  {
    for(SESubstanceFraction sf : this.gasFractions)
    {
      if(sf.getSubstance().getName().equals(substance.getName()))
      {        
        return sf;
      }
    }    
    SESubstanceFraction sf = new SESubstanceFraction(substance);    
    this.gasFractions.add(sf);
    return sf;
  }
  public boolean hasGasFraction()
  {
    return !this.gasFractions.isEmpty();
  }
  public boolean hasGasFraction(SESubstance substance)
  {
    for(SESubstanceFraction sf : this.gasFractions)
    {
      if(sf.getSubstance()==substance)
      {        
        return true;
      }
    }
    return false;
  }
  public List<SESubstanceFraction> getGasFraction()
  {
    return this.gasFractions;
  }
  public void removeGasFraction(SESubstance substance)
  {
    for(SESubstanceFraction sf : this.gasFractions)
    {
      if(sf.getSubstance()==substance)
      {
        this.gasFractions.remove(sf);
        return;
      }
    }  
  }

  public SESubstanceConcentration createAerosol(SESubstance substance)
  {
    return getAerosol(substance);
  }
  public SESubstanceConcentration getAerosol(SESubstance substance)
  {
    for(SESubstanceConcentration sc : this.aerosols)
    {
      if(sc.getSubstance().getName().equals(substance.getName()))
      {        
        return sc;
      }
    }    
    SESubstanceConcentration sc = new SESubstanceConcentration(substance);    
    this.aerosols.add(sc);
    return sc;
  }
  public boolean hasAerosol()
  {
    return !this.aerosols.isEmpty();
  }
  public boolean hasAerosol(SESubstance substance)
  {
    for(SESubstanceConcentration sc : this.aerosols)
    {
      if(sc.getSubstance()==substance)
      {        
        return true;
      }
    }
    return false;
  }
  public List<SESubstanceConcentration> getAerosol()
  {
    return this.aerosols;
  }
  public void removeAerosol(SESubstance substance)
  {
    for(SESubstanceConcentration sc : this.aerosols)
    {
      if(sc.getSubstance()==substance)
      {
        this.aerosols.remove(sc);
        return;
      }
    }  
  }
  
  public String toString()
  {
    String cnts = "Mechanical Ventilation"
        + "\n\tState: " + getState()
        + "\n\tFlow: " + (hasFlow() ? getFlow() : "Not Provided")
        + "\n\tPressure: " + (hasPressure() ? getPressure() : "Not Provided");
    for(SESubstanceFraction sf : this.gasFractions)
      cnts += "\n\tSubstanceFraction: " + sf.getSubstance().getName() + " : " + sf.getAmount();
    for(SESubstanceConcentration sc : this.aerosols)
        cnts += "\n\tSubstanceConcentration: " + sc.getSubstance().getName() + " : " + sc.getConcentration();
    
    return cnts;
  }
}