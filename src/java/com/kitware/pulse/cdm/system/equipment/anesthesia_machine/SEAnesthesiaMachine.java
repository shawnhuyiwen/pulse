/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.anesthesia_machine;

import com.kitware.pulse.cdm.bind.AnesthesiaMachine.AnesthesiaMachineData;
import com.kitware.pulse.cdm.bind.AnesthesiaMachine.AnesthesiaMachineData.eConnection;
import com.kitware.pulse.cdm.bind.AnesthesiaMachine.AnesthesiaMachineData.eOxygenSource;
import com.kitware.pulse.cdm.bind.AnesthesiaMachine.AnesthesiaMachineData.ePrimaryGas;
import com.kitware.pulse.cdm.properties.SEScalar;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarFrequency;
import com.kitware.pulse.cdm.properties.SEScalarPressure;
import com.kitware.pulse.cdm.properties.SEScalarVolumePerTime;
import com.kitware.pulse.cdm.system.equipment.SEEquipment;

public class SEAnesthesiaMachine extends SEEquipment
{
  protected eConnection                       connection;
  protected SEScalarVolumePerTime             inletFlow;
  protected SEScalar                          inspiratoryExpiratoryRatio;
  protected SEScalar0To1                      oxygenFraction;
  protected eOxygenSource                     oxygenSource;
  protected SEScalarPressure                  positiveEndExpiredPressure;
  protected ePrimaryGas                       primaryGas;
  protected SEScalarFrequency                 respiratoryRate;
  protected SEScalarPressure                  reliefValvePressure;

  protected SEScalarPressure                  peakInspiratoryPressure;

  protected SEAnesthesiaMachineChamber        leftChamber;
  protected SEAnesthesiaMachineChamber        rightChamber;

  protected SEAnesthesiaMachineOxygenBottle   oxygenBottleOne;
  protected SEAnesthesiaMachineOxygenBottle   oxygenBottleTwo;



  public SEAnesthesiaMachine()
  {
    connection = null;
    inletFlow = null;
    inspiratoryExpiratoryRatio = null;
    oxygenFraction = null;
    oxygenSource = null;
    positiveEndExpiredPressure = null;
    primaryGas = null;
    respiratoryRate = null;
    reliefValvePressure = null;

    peakInspiratoryPressure = null;

    leftChamber = null;
    rightChamber = null;
    
    oxygenBottleOne = null;
    oxygenBottleTwo = null;
  }

  @Override
  public void reset()
  {
    connection = null;
    if (inletFlow != null)
      inletFlow.invalidate();
    if (inspiratoryExpiratoryRatio != null)
      inspiratoryExpiratoryRatio.invalidate();
    if (oxygenFraction != null)
      oxygenFraction.invalidate();
    oxygenSource = null;
    if (positiveEndExpiredPressure != null)
      positiveEndExpiredPressure.invalidate();
    primaryGas = null;
    if (respiratoryRate != null)
      respiratoryRate.invalidate();
    if (reliefValvePressure != null)
      reliefValvePressure.invalidate();
    if (peakInspiratoryPressure != null)
      peakInspiratoryPressure.invalidate();

    if (hasLeftChamber())
      leftChamber.reset();
    if (hasRightChamber())
      rightChamber.reset();
    if (hasOxygenBottleOne())
      oxygenBottleOne.reset();
    if (hasOxygenBottleTwo())
      oxygenBottleTwo.reset();

  }

  public void copy(SEAnesthesiaMachine from)
  {
    reset();
    if(from.connection!=null && from.connection != eConnection.NullConnection)
    	this.connection=from.connection;      
    if(from.hasInletFlow())
      this.getInletFlow().set(from.getInletFlow());
    if(from.hasInspiratoryExpiratoryRatio())
      this.getInspiratoryExpiratoryRatio().set(from.getInspiratoryExpiratoryRatio());
    if(from.hasOxygenFraction())
      this.getOxygenFraction().set(from.getOxygenFraction());
    if(from.oxygenSource!=null && from.oxygenSource != eOxygenSource.NullSource)
    	this.setOxygenSource(from.getOxygenSource());
    if(from.hasPositiveEndExpiredPressure())
      this.getPositiveEndExpiredPressure().set(from.getPositiveEndExpiredPressure());
    if(from.primaryGas!=null && from.primaryGas != ePrimaryGas.NullGas)
    	this.setPrimaryGas(from.getPrimaryGas());
    if(from.hasReliefValvePressure())
      this.getReliefValvePressure().set(from.getReliefValvePressure());
    if(from.hasRespiratoryRate())
      this.getRespiratoryRate().set(from.getRespiratoryRate());
    if(from.hasPeakInspiratoryPressure())
      this.getPeakInspiratoryPressure().set(from.getPeakInspiratoryPressure());

    if(from.hasLeftChamber())    
      getLeftChamber().copy(from.getLeftChamber());
    if(from.hasRightChamber())
      getRightChamber().copy(from.getRightChamber());
    if(from.hasOxygenBottleOne())
      getOxygenBottleOne().copy(from.getOxygenBottleOne());
    if(from.hasOxygenBottleTwo())
      getOxygenBottleTwo().copy(from.getOxygenBottleTwo());
  }

  public static void load(AnesthesiaMachineData src, SEAnesthesiaMachine dst)
  {
    dst.reset();
    if (src.getConnection()!=eConnection.UNRECOGNIZED)
      dst.setConnection(src.getConnection());
    if (src.hasInletFlow())
      SEScalarVolumePerTime.load(src.getInletFlow(), dst.getInletFlow());
    if (src.hasInspiratoryExpiratoryRatio())
      SEScalar.load(src.getInspiratoryExpiratoryRatio(), dst.getInspiratoryExpiratoryRatio());
    if (src.hasOxygenFraction())
      SEScalar0To1.load(src.getOxygenFraction(), dst.getOxygenFraction());
    if (src.getOxygenSource()!=eOxygenSource.UNRECOGNIZED)
      dst.setOxygenSource(src.getOxygenSource());
    if (src.hasPositiveEndExpiredPressure())
      SEScalarPressure.load(src.getPositiveEndExpiredPressure(), dst.getPositiveEndExpiredPressure());
    if (src.getPrimaryGas()!=ePrimaryGas.UNRECOGNIZED)
      dst.setPrimaryGas(src.getPrimaryGas());
    if (src.hasRespiratoryRate())
      SEScalarFrequency.load(src.getRespiratoryRate(), dst.getRespiratoryRate());
    if (src.hasReliefValvePressure())
      SEScalarPressure.load(src.getReliefValvePressure(), dst.getReliefValvePressure());
    if (src.hasPeakInspiratoryPressure())
      SEScalarPressure.load(src.getPeakInspiratoryPressure(), dst.getPeakInspiratoryPressure());
    if (src.hasLeftChamber())
      SEAnesthesiaMachineChamber.load(src.getLeftChamber(), dst.getLeftChamber());
    if (src.hasRightChamber())
      SEAnesthesiaMachineChamber.load(src.getRightChamber(), dst.getRightChamber());
    if (src.hasOxygenBottleOne())
      SEAnesthesiaMachineOxygenBottle.load(src.getOxygenBottleOne(), dst.getOxygenBottleOne());
    if (src.hasOxygenBottleTwo())
      SEAnesthesiaMachineOxygenBottle.load(src.getOxygenBottleTwo(), dst.getOxygenBottleTwo());
  }
  public static AnesthesiaMachineData unload(SEAnesthesiaMachine src)
  {
    AnesthesiaMachineData.Builder dst = AnesthesiaMachineData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachine src, AnesthesiaMachineData.Builder dst)
  {
    if (src.hasConnection())
      dst.setConnection(src.connection);
    if (src.hasInletFlow())
      dst.setInletFlow(SEScalarVolumePerTime.unload(src.inletFlow));
    if (src.hasInspiratoryExpiratoryRatio())
      dst.setInspiratoryExpiratoryRatio(SEScalar.unload(src.inspiratoryExpiratoryRatio));
    if (src.hasOxygenFraction())
      dst.setOxygenFraction(SEScalar0To1.unload(src.oxygenFraction));
    if (src.hasOxygenSource())
      dst.setOxygenSource(src.oxygenSource);
    if (src.hasPositiveEndExpiredPressure())
      dst.setPositiveEndExpiredPressure(SEScalarPressure.unload(src.positiveEndExpiredPressure));
    if (src.hasPrimaryGas())
      dst.setPrimaryGas(src.primaryGas);
    if (src.hasRespiratoryRate())
      dst.setRespiratoryRate(SEScalarFrequency.unload(src.respiratoryRate));
    if (src.hasReliefValvePressure())
      dst.setReliefValvePressure(SEScalarPressure.unload(src.reliefValvePressure));
    if (src.hasPeakInspiratoryPressure())
      dst.setPeakInspiratoryPressure(SEScalarPressure.unload(src.peakInspiratoryPressure));
    
    if (src.hasLeftChamber())
      dst.setLeftChamber(SEAnesthesiaMachineChamber.unload(src.leftChamber));
    if (src.hasRightChamber())
      dst.setRightChamber(SEAnesthesiaMachineChamber.unload(src.rightChamber));
    if (src.hasOxygenBottleOne())
      dst.setOxygenBottleOne(SEAnesthesiaMachineOxygenBottle.unload(src.oxygenBottleOne));
    if (src.hasOxygenBottleTwo())
      dst.setOxygenBottleTwo(SEAnesthesiaMachineOxygenBottle.unload(src.oxygenBottleTwo));
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

  /*
   * Inspiratory/Expiratory Ratio
   */
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

  /*
   * Inlet Flow
   */
  public SEScalarVolumePerTime getInletFlow()
  {
    if (inletFlow == null)
      inletFlow = new SEScalarVolumePerTime();
    return inletFlow;
  }
  public boolean hasInletFlow()
  {
    return inletFlow == null ? false : inletFlow.isValid();
  }

  /*
   * Oxygen Fraction
   */
  public SEScalar0To1 getOxygenFraction()
  {
    if (oxygenFraction == null)
      oxygenFraction = new SEScalar0To1();
    return oxygenFraction;
  }
  public boolean hasOxygenFraction()
  {
    return oxygenFraction == null ? false : oxygenFraction.isValid();
  }

  /*
   * Oxygen Source
   */
  public eOxygenSource getOxygenSource()
  {
    return oxygenSource;
  }
  public void setOxygenSource(eOxygenSource os)
  {
  	oxygenSource = (os == eOxygenSource.UNRECOGNIZED) ? null : os;
  }
  public boolean hasOxygenSource()
  {
    return oxygenSource != null;
  }

  /*
   * Positive End-Expired Pressure
   */
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

  /*
   * Anesthesia Machine Primary Gas
   */
  public ePrimaryGas getPrimaryGas()
  {
    return primaryGas;
  }
  public void setPrimaryGas(ePrimaryGas pg)
  {
  	primaryGas = (pg == ePrimaryGas.UNRECOGNIZED) ? null : pg;
  }
  public boolean hasPrimaryGas()
  {
    return primaryGas != null;
  }

  /*
   * Respiratory Rate
   */
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

  /*
   * Relief Valve Pressure
   */
  public SEScalarPressure getReliefValvePressure()
  {
    if (reliefValvePressure == null)
      reliefValvePressure = new SEScalarPressure();
    return reliefValvePressure;
  }
  public boolean hasReliefValvePressure()
  {
    return reliefValvePressure == null ? false : reliefValvePressure.isValid();
  }
  
  /*
   * Ventilator Pressure
   */
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

  /*
   * Left Chamber
   */
  public boolean hasLeftChamber()
  {
    return leftChamber == null ? false : true;
  }
  public SEAnesthesiaMachineChamber getLeftChamber()
  {
    if(leftChamber==null)
      this.leftChamber=new SEAnesthesiaMachineChamber();
    return leftChamber;
  }
  public void removeLeftChamber()
  {
    leftChamber = null;
  }

  /*
   * Right Chamber
   */
  public boolean hasRightChamber()
  {
    return rightChamber == null ? false : true;
  }
  public SEAnesthesiaMachineChamber getRightChamber()
  {
    if(rightChamber==null)
      this.rightChamber=new SEAnesthesiaMachineChamber();
    return rightChamber;
  }
  public void removeRightChamber()
  {
    rightChamber = null;
  }

  /*
   * Oxygen Bottle One
   */
  public SEAnesthesiaMachineOxygenBottle getOxygenBottleOne()
  {
    if (oxygenBottleOne == null)
      oxygenBottleOne = new SEAnesthesiaMachineOxygenBottle();

    return oxygenBottleOne;
  }

  public boolean hasOxygenBottleOne()
  {
    return oxygenBottleOne == null ? false : true;
  }

  /*
   * Oxygen Bottle Two
   */
  public SEAnesthesiaMachineOxygenBottle getOxygenBottleTwo()
  {
    if (oxygenBottleTwo == null)
      oxygenBottleTwo = new SEAnesthesiaMachineOxygenBottle();

    return oxygenBottleTwo;
  }

  public boolean hasOxygenBottleTwo()
  {
    return oxygenBottleTwo == null ? false : true;
  }
  
  @Override
  public String toString()
  {
    String leftChamber = "Left Chamber: NotProvided";
    if(hasLeftChamber())
    {
      leftChamber = getLeftChamber().toString();
      leftChamber = leftChamber.replaceAll("Chamber", "Left Chamber");
    }
    String rightChamber = "Right Chamber: NotProvided";
    if(hasRightChamber())
    {
      rightChamber = getRightChamber().toString();
      rightChamber = rightChamber.replaceAll("Chamber", "Right Chamber");
    }
    String o2BottleOne = "Oxygen Bottle One: NotProvided";
    if(hasOxygenBottleOne())
    {
      o2BottleOne = getOxygenBottleOne().toString();
      o2BottleOne = o2BottleOne.replaceAll("Bottle", "Bottle One");
    }
    String o2BottleTwo = "Oxygen Bottle Two: NotProvided";
    if(hasOxygenBottleTwo())
    {
      o2BottleTwo = getOxygenBottleTwo().toString();
      o2BottleTwo = o2BottleTwo.replaceAll("Bottle", "Bottle Two");
    }
    
      return "Anesthesia Machine"
      + "\n\tConnection: " + (hasConnection()?getConnection():"NotProvided")
      + "\n\tInletFlow: " + getInletFlow()
      + "\n\tInspiratoryExpiratoryRatio: " + getInspiratoryExpiratoryRatio()
      + "\n\tOxygenFraction: " + getOxygenFraction()
      + "\n\tOxygenSource: " + (hasOxygenSource()?getOxygenSource():"NotProvided")
      + "\n\tPositiveEndExpiredPressure: " + getPositiveEndExpiredPressure()
      + "\n\tPrimaryGas: " + (hasPrimaryGas()?getPrimaryGas():"NotProvided")
      + "\n\tRespiratoryRate: " + getRespiratoryRate()
      + "\n\tReliefValvePressure: " + getReliefValvePressure()
      + "\n\tPeakInspiratoryPressure: " + getPeakInspiratoryPressure()
      + "\n\t" + leftChamber
      + "\n\t" + rightChamber
      + "\n\t" + o2BottleOne
      + "\n\t" + o2BottleTwo;
  }
}