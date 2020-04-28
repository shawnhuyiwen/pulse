/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.system.equipment.mechanical_ventilator;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import pulse.cdm.bind.MechanicalVentilator.MechanicalVentilatorData;
import pulse.cdm.bind.MechanicalVentilator.MechanicalVentilatorData.*;
import pulse.cdm.bind.Substance.SubstanceConcentrationData;
import pulse.cdm.bind.Substance.SubstanceFractionData;
import pulse.cdm.bind.Substance.SubstanceData.eState;
import pulse.cdm.properties.SEScalar;
import pulse.cdm.properties.SEScalar0To1;
import pulse.cdm.properties.SEScalarFrequency;
import pulse.cdm.properties.SEScalarMassPerVolume;
import pulse.cdm.properties.SEScalarPressure;
import pulse.cdm.properties.SEScalarTime;
import pulse.cdm.substance.SESubstance;
import pulse.cdm.substance.SESubstanceConcentration;
import pulse.cdm.substance.SESubstanceFraction;
import pulse.cdm.substance.SESubstanceManager;
import pulse.cdm.system.equipment.SEEquipment;
import pulse.utilities.Log;

public class SEMechanicalVentilator extends SEEquipment
{
  protected SEScalarTime                      breathPeriod;
  protected eConnection                       connection;
  protected eControl                          control;
  protected eDriverWaveform                   driverWaveform;
  protected SEScalar                          inspiratoryExpiratoryRatio;
  protected SEScalarTime                      inspiratoryPeriod;
  protected SEScalarTime                      expiratoryPeriod;
  protected SEScalarPressure                  peakInspiratoryPressure;
  protected SEScalarPressure                  positiveEndExpiredPressure;
  protected SEScalarFrequency                 respiratoryRate;
  
  protected List<SESubstanceFraction>         fractionInspiredGases;
  protected List<SESubstanceConcentration>    concentrationInspiredAerosol;


  public SEMechanicalVentilator()
  {
    breathPeriod = null;
    connection = null;
    control = null;
    driverWaveform = null;
    expiratoryPeriod = null;
    inspiratoryExpiratoryRatio = null;
    inspiratoryPeriod = null;
    peakInspiratoryPressure = null;
    positiveEndExpiredPressure = null;
    respiratoryRate = null;

    this.fractionInspiredGases=new ArrayList<SESubstanceFraction>();
    this.concentrationInspiredAerosol=new ArrayList<SESubstanceConcentration>();
  }

  public void reset()
  {
    if (breathPeriod != null)
      breathPeriod.invalidate();
    connection = null;
    control = null;
    driverWaveform = null;
    if (expiratoryPeriod != null)
      expiratoryPeriod.invalidate();
    if (inspiratoryExpiratoryRatio != null)
      inspiratoryExpiratoryRatio.invalidate();
    if (inspiratoryPeriod != null)
      inspiratoryPeriod.invalidate();
    if (peakInspiratoryPressure != null)
      peakInspiratoryPressure.invalidate();
    if (positiveEndExpiredPressure != null)
      positiveEndExpiredPressure.invalidate();
    if (respiratoryRate != null)
      respiratoryRate.invalidate();
    
    this.fractionInspiredGases.clear();
    this.concentrationInspiredAerosol.clear();
  }

  public void copy(SEMechanicalVentilator from)
  {
    reset();
    if(from.hasBreathPeriod())
      this.getBreathPeriod().set(from.getBreathPeriod());
    if(from.connection!=null && from.connection != eConnection.NullConnection)
    	this.connection=from.connection;
    if(from.control!=null && from.control != eControl.NullControl)
      this.control=from.control;
    if(from.driverWaveform!=null && from.driverWaveform != eDriverWaveform.NullDriverWaveform)
      this.driverWaveform=from.driverWaveform;
    if(from.hasExpiratoryPeriod())
      this.getExpiratoryPeriod().set(from.getExpiratoryPeriod());
    if(from.hasInspiratoryExpiratoryRatio())
      this.getInspiratoryExpiratoryRatio().set(from.getInspiratoryExpiratoryRatio());
    if(from.hasInspiratoryPeriod())
      this.getInspiratoryPeriod().set(from.getInspiratoryPeriod());
    if(from.hasPeakInspiratoryPressure())
      this.getPeakInspiratoryPressure().set(from.getPeakInspiratoryPressure());
    if(from.hasPositiveEndExpiredPressure())
      this.getPositiveEndExpiredPressure().set(from.getPositiveEndExpiredPressure());
    if(from.hasRespiratoryRate())
      this.getRespiratoryRate().set(from.getRespiratoryRate());
    
    if(from.fractionInspiredGases!=null)
    {
      SESubstanceFraction mine;
      for(SESubstanceFraction sf : from.fractionInspiredGases)
      {
        mine=this.createFractionInspiredGas(sf.getSubstance());
        if(sf.hasAmount())
          mine.getAmount().set(sf.getAmount());
      }
    }
    if(from.concentrationInspiredAerosol!=null)
    {
      SESubstanceConcentration mine;
      for(SESubstanceConcentration sc : from.concentrationInspiredAerosol)
      {
        mine=this.createConcentrationInspiredAerosol(sc.getSubstance());
        if(sc.hasConcentration())
          mine.getConcentration().set(sc.getConcentration());
      }
    }
  }

  public static void load(MechanicalVentilatorData src, SEMechanicalVentilator dst, SESubstanceManager subMgr)
  {
    dst.reset();
    if(src.hasBreathProfile())
    {
      if (src.getBreathProfile().hasPeriod())
        SEScalarTime.load(src.getBreathProfile().getPeriod(), dst.getBreathPeriod());
      else if (src.getBreathProfile().hasRate())
        SEScalarFrequency.load(src.getBreathProfile().getRate(), dst.getRespiratoryRate());
      
      if(src.getBreathProfile().hasInspiratoryExpiratoryRatio())
        SEScalar.load(src.getBreathProfile().getInspiratoryExpiratoryRatio(), dst.getInspiratoryExpiratoryRatio());
    }
    else if(src.hasInspiratoryExpiratoryPeriod())
    {
      if(src.getInspiratoryExpiratoryPeriod().hasExpiratoryPeriod())
        SEScalarTime.load(src.getInspiratoryExpiratoryPeriod().getExpiratoryPeriod(), dst.getExpiratoryPeriod());
      if(src.getInspiratoryExpiratoryPeriod().hasInspiratoryPeriod())
        SEScalarTime.load(src.getInspiratoryExpiratoryPeriod().getInspiratoryPeriod(), dst.getInspiratoryPeriod());
    }
    
    if (src.getConnection()!=eConnection.UNRECOGNIZED)
      dst.setConnection(src.getConnection());
    if (src.getControl()!=eControl.UNRECOGNIZED)
      dst.setControl(src.getControl());
    if (src.getDriverWaveform()!=eDriverWaveform.UNRECOGNIZED)
      dst.setDriverWaveform(src.getDriverWaveform());
    if (src.hasPeakInspiratoryPressure())
      SEScalarPressure.load(src.getPeakInspiratoryPressure(), dst.getPeakInspiratoryPressure());
    if (src.hasPositiveEndExpiredPressure())
      SEScalarPressure.load(src.getPositiveEndExpiredPressure(), dst.getPositiveEndExpiredPressure());
    
    SESubstance sub;
    if(src.getFractionInspiredGasList()!=null)
    {
      for(SubstanceFractionData subData : src.getFractionInspiredGasList())
      {
        sub = subMgr.getSubstance(subData.getName());
        if(sub == null)
        {
          Log.error("Substance does not exist for ambient gas : "+subData.getName());
          continue;
        }
        if(sub.getState() != eState.Gas)
        {
          Log.error("Environment Ambient Gas must be a gas, "+subData.getName()+" is not a gas...");
          continue;
        }
        SEScalar0To1.load(subData.getAmount(),dst.createFractionInspiredGas(sub).getAmount());
      }
    }
    
    if(src.getConcentrationInspiredAerosolList()!=null)
    {
      for(SubstanceConcentrationData subData : src.getConcentrationInspiredAerosolList())
      {
        sub = subMgr.getSubstance(subData.getName());
        if(sub == null)
        {
          Log.error("Substance does not exist for ambient aerosol : "+subData.getName());
          continue;
        }
        if(sub.getState() != eState.Solid && sub.getState() != eState.Liquid)
        {
          Log.error("Environment Ambient Aerosol must be a liquid or a gas, "+subData.getName()+" is neither...");
          continue;
        }
        SEScalarMassPerVolume.load(subData.getConcentration(),dst.createConcentrationInspiredAerosol(sub).getConcentration());
      }
    }
  }
  public static MechanicalVentilatorData unload(SEMechanicalVentilator src)
  {
    MechanicalVentilatorData.Builder dst = MechanicalVentilatorData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEMechanicalVentilator src, MechanicalVentilatorData.Builder dst)
  {
    if (src.hasConnection())
      dst.setConnection(src.connection);
    if (src.hasControl())
      dst.setControl(src.control);
    if (src.hasDriverWaveform())
      dst.setDriverWaveform(src.driverWaveform);
    
    if( (src.hasBreathPeriod() || src.hasRespiratoryRate()) && src.hasInspiratoryExpiratoryRatio())
    {
      BreathProfileData.Builder builder = dst.getBreathProfileBuilder();
      builder.setInspiratoryExpiratoryRatio(SEScalar.unload(src.inspiratoryExpiratoryRatio));
      
      if (src.hasRespiratoryRate())
        builder.setRate(SEScalarFrequency.unload(src.respiratoryRate));
      else if(src.hasBreathPeriod())
        builder.setPeriod(SEScalarTime.unload(src.breathPeriod));
    }
    else if(src.hasExpiratoryPeriod() && src.hasInspiratoryPeriod())
    {
      InspiratoryExpiratoryPeriodData.Builder builder = dst.getInspiratoryExpiratoryPeriodBuilder();
      
      builder.setInspiratoryPeriod(SEScalarTime.unload(src.inspiratoryPeriod));
      builder.setExpiratoryPeriod(SEScalarTime.unload(src.expiratoryPeriod));
    }

    if (src.hasPeakInspiratoryPressure())
      dst.setPeakInspiratoryPressure(SEScalarPressure.unload(src.peakInspiratoryPressure));
    if (src.hasPositiveEndExpiredPressure())
      dst.setPositiveEndExpiredPressure(SEScalarPressure.unload(src.positiveEndExpiredPressure));
    
    for(SESubstanceFraction ambSub : src.fractionInspiredGases)
      dst.addFractionInspiredGas(SESubstanceFraction.unload(ambSub));
    
    for(SESubstanceConcentration ambSub : src.concentrationInspiredAerosol)
      dst.addConcentrationInspiredAerosol(SESubstanceConcentration.unload(ambSub));
  }
  
  public SEScalarTime getBreathPeriod()
  {
    if (breathPeriod == null)
      breathPeriod = new SEScalarTime();
    return breathPeriod;
  }
  public boolean hasBreathPeriod()
  {
    return breathPeriod == null ? false : breathPeriod.isValid();
  }
  
  public eConnection getConnection()
  {
    return connection;
  }
  public void setConnection(eConnection c)
  {
    connection = (c == eConnection.UNRECOGNIZED) ? null : c;
  }
  public boolean hasConnection()
  {
    return connection != null;
  }
  
  public eControl getControl()
  {
    return control;
  }
  public void setControl(eControl c)
  {
    control = (c == eControl.UNRECOGNIZED) ? null : c;
  }
  public boolean hasControl()
  {
    return control != null;
  }
  
  public eDriverWaveform getDriverWaveform()
  {
    return driverWaveform;
  }
  public void setDriverWaveform(eDriverWaveform w)
  {
    driverWaveform = (w == eDriverWaveform.UNRECOGNIZED) ? null : w;
  }
  public boolean hasDriverWaveform()
  {
    return driverWaveform != null;
  }
  
  public SEScalarTime getExpiratoryPeriod()
  {
    if (expiratoryPeriod == null)
      expiratoryPeriod = new SEScalarTime();
    return expiratoryPeriod;
  }
  public boolean hasExpiratoryPeriod()
  {
    return expiratoryPeriod == null ? false : expiratoryPeriod.isValid();
  }

  public SEScalar getInspiratoryExpiratoryRatio()
  {
    if (inspiratoryExpiratoryRatio == null)
      inspiratoryExpiratoryRatio = new SEScalar();
    return inspiratoryExpiratoryRatio;
  }
  public boolean hasInspiratoryExpiratoryRatio()
  {
    return inspiratoryExpiratoryRatio == null ? false : inspiratoryExpiratoryRatio.isValid();
  }
  
  public SEScalarTime getInspiratoryPeriod()
  {
    if (inspiratoryPeriod == null)
      inspiratoryPeriod = new SEScalarTime();
    return inspiratoryPeriod;
  }
  public boolean hasInspiratoryPeriod()
  {
    return inspiratoryPeriod == null ? false : inspiratoryPeriod.isValid();
  }

  public SEScalarPressure getPeakInspiratoryPressure()
  {
    if (peakInspiratoryPressure == null)
      peakInspiratoryPressure = new SEScalarPressure();
    return peakInspiratoryPressure;
  }
  public boolean hasPeakInspiratoryPressure()
  {
    return peakInspiratoryPressure == null ? false : peakInspiratoryPressure.isValid();
  }

  public SEScalarPressure getPositiveEndExpiredPressure()
  {
    if (positiveEndExpiredPressure == null)
      positiveEndExpiredPressure = new SEScalarPressure();
    return positiveEndExpiredPressure;
  }
  public boolean hasPositiveEndExpiredPressure()
  {
    return positiveEndExpiredPressure == null ? false : positiveEndExpiredPressure.isValid();
  }

  public SEScalarFrequency getRespiratoryRate()
  {
    if (respiratoryRate == null)
      respiratoryRate = new SEScalarFrequency();
    return respiratoryRate;
  }
  public boolean hasRespiratoryRate()
  {
    return respiratoryRate == null ? false : respiratoryRate.isValid();
  }
  
  public SESubstanceFraction createFractionInspiredGas(SESubstance substance)
  {
    return getFractionInspiredGas(substance);
  }
  public SESubstanceFraction getFractionInspiredGas(SESubstance substance)
  {
    for(SESubstanceFraction sf : this.fractionInspiredGases)
    {
      if(sf.getSubstance().getName().equals(substance.getName()))
      {        
        return sf;
      }
    }    
    SESubstanceFraction sf = new SESubstanceFraction(substance);
    this.fractionInspiredGases.add(sf);
    return sf;
  }
  public boolean hasFractionInspiredGas()
  {
    return !this.fractionInspiredGases.isEmpty();
  }
  public boolean hasFractionInspiredGas(SESubstance substance)
  {
    for(SESubstanceFraction sf : this.fractionInspiredGases)
    {
      if(sf.getSubstance()==substance)
      {        
        return true;
      }
    }
    return false;
  }
  public List<SESubstanceFraction> getFractionInspiredGas()
  {
    return this.fractionInspiredGases;
  }
  public void removeFractionInspiredGas(SESubstance substance)
  {
    for(SESubstanceFraction sf : this.fractionInspiredGases)
    {
      if(sf.getSubstance()==substance)
      {
        this.fractionInspiredGases.remove(sf);
        return;
      }
    }  
  }
  

  public SESubstanceConcentration createConcentrationInspiredAerosol(SESubstance substance)
  {
    return getConcentrationInspiredAerosol(substance);
  }
  public SESubstanceConcentration getConcentrationInspiredAerosol(SESubstance substance)
  {
    for(SESubstanceConcentration sc : this.concentrationInspiredAerosol)
    {
      if(sc.getSubstance().getName().equals(substance.getName()))
      {        
        return sc;
      }
    }    
    SESubstanceConcentration sc = new SESubstanceConcentration(substance);
    this.concentrationInspiredAerosol.add(sc);
    return sc;
  }
  public boolean hasConcentrationInspiredAerosol()
  {
    return !this.concentrationInspiredAerosol.isEmpty();
  }
  public boolean hasConcentrationInspiredAerosol(SESubstance substance)
  {
    for(SESubstanceConcentration sc : this.concentrationInspiredAerosol)
    {
      if(sc.getSubstance()==substance)
      {        
        return true;
      }
    }
    return false;
  }
  public List<SESubstanceConcentration> getConcentrationInspiredAerosol()
  {
    return Collections.unmodifiableList(this.concentrationInspiredAerosol);
  }
  public void removeConcentrationInspiredAerosol(SESubstance substance)
  {
    for(SESubstanceConcentration sc : this.concentrationInspiredAerosol)
    {
      if(sc.getSubstance()==substance)
      {
        this.concentrationInspiredAerosol.remove(sc);
        return;
      }
    }  
  }
  
  public void trim()
  {
    SESubstanceFraction sf;
    Iterator<SESubstanceFraction> ifrac = this.fractionInspiredGases.iterator();
    while(ifrac.hasNext())
    {
      sf=ifrac.next();
      if(sf.getAmount().getValue()==0)
        ifrac.remove();
    }    
  }

  public String toString()
  {
    String str = "Mechanical Ventilator"
        + "\n\tBreathPeriod: " + getBreathPeriod()
        + "\n\tConnection: " + (hasConnection()?getConnection():"NotProvided")
        + "\n\tControl: " + (hasConnection()?getConnection():"NotProvided")
        + "\n\tDriverWaveform: " + (hasConnection()?getConnection():"NotProvided")
        + "\n\tExpiratoryPeriod: " + getExpiratoryPeriod()
        + "\n\tInspiratoryExpiratoryRatio: " + getInspiratoryExpiratoryRatio()
        + "\n\tInspiratoryPeriod: " + getInspiratoryPeriod()
        + "\n\tPeakInspiratoryPressure: " + getPeakInspiratoryPressure()
        + "\n\tPositiveEndExpiredPressure: " + getPositiveEndExpiredPressure()
        + "\n\tRespiratoryRate: " + getRespiratoryRate();
    for(SESubstanceFraction sf : this.fractionInspiredGases)
      str += "\n\t"+sf.toString();
    for(SESubstanceConcentration sc : this.concentrationInspiredAerosol)
    str += "\n\t"+sc.toString();
    return str;
  }
}