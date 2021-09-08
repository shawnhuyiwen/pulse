/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions;

import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorContinuousPositiveAirwayPressureData;
import com.kitware.pulse.cdm.properties.*;

public class SEMechanicalVentilatorContinuousPositiveAirwayPressure extends SEMechanicalVentilatorMode
{
  protected SEScalarPressure deltaPressureSupport;
  protected SEScalar0To1     fractionInspiredOxygen;
  protected SEScalarPressure positiveEndExpiredPressure;
  protected SEScalarTime     slope;

  public SEMechanicalVentilatorContinuousPositiveAirwayPressure()
  {
    deltaPressureSupport = null;
    fractionInspiredOxygen = null;
    positiveEndExpiredPressure = null;
    slope = null;
  }

  public SEMechanicalVentilatorContinuousPositiveAirwayPressure(
      SEMechanicalVentilatorContinuousPositiveAirwayPressure other)
  {
    copy(other);
  }

  public void copy(SEMechanicalVentilatorContinuousPositiveAirwayPressure other)
  {
    super.copy(other);
    if (other.hasDeltaPressureSupport())
      getDeltaPressureSupport().set(other.getDeltaPressureSupport());
    if (other.hasFractionInspiredOxygen())
      getFractionInspiredOxygen().set(other.getFractionInspiredOxygen());
    if (other.hasPositiveEndExpiredPressure())
      getPositiveEndExpiredPressure().set(other.getPositiveEndExpiredPressure());
    if (other.hasSlope())
      getSlope().set(other.getSlope());
  }

  public void clear()
  {
    super.clear();
    if (deltaPressureSupport != null)
      deltaPressureSupport.invalidate();
    if (fractionInspiredOxygen != null)
      fractionInspiredOxygen.invalidate();
    if (positiveEndExpiredPressure != null)
      positiveEndExpiredPressure.invalidate();
    if (slope != null)
      slope.invalidate();
  }
  
  public static void load(MechanicalVentilatorContinuousPositiveAirwayPressureData src, SEMechanicalVentilatorContinuousPositiveAirwayPressure dst)
  {
    dst.reset();
    SEMechanicalVentilatorMode.load(src.getMechanicalVentilatorMode(),dst);
    if (src.hasDeltaPressureSupport())
      SEScalarPressure.load(src.getDeltaPressureSupport(), dst.getDeltaPressureSupport());
    if (src.hasFractionInspiredOxygen())
      SEScalar0To1.load(src.getFractionInspiredOxygen(), dst.getFractionInspiredOxygen());
    if (src.hasPositiveEndExpiredPressure())
      SEScalarPressure.load(src.getPositiveEndExpiredPressure(), dst.getPositiveEndExpiredPressure());
    if (src.hasSlope())
      SEScalarTime.load(src.getSlope(), dst.getSlope());
  }
  public static MechanicalVentilatorContinuousPositiveAirwayPressureData unload(SEMechanicalVentilatorContinuousPositiveAirwayPressure src)
  {
    MechanicalVentilatorContinuousPositiveAirwayPressureData.Builder dst = MechanicalVentilatorContinuousPositiveAirwayPressureData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEMechanicalVentilatorContinuousPositiveAirwayPressure src, MechanicalVentilatorContinuousPositiveAirwayPressureData.Builder dst)
  {
    SEMechanicalVentilatorMode.unload(src, dst.getMechanicalVentilatorModeBuilder());
    if(src.hasDeltaPressureSupport())
      dst.setDeltaPressureSupport(SEScalarPressure.unload(src.getDeltaPressureSupport()));
    if (src.hasFractionInspiredOxygen())
      dst.setFractionInspiredOxygen(SEScalar0To1.unload(src.getFractionInspiredOxygen()));
    if (src.hasPositiveEndExpiredPressure())
      dst.setPositiveEndExpiredPressure(SEScalarPressure.unload(src.getPositiveEndExpiredPressure()));
    if (src.hasSlope())
      dst.setSlope(SEScalarTime.unload(src.getSlope()));
  }

  public boolean isValid()
  {
    return hasDeltaPressureSupport() && hasFractionInspiredOxygen() && hasPositiveEndExpiredPressure() && hasSlope();
  }

  public boolean hasDeltaPressureSupport()
  {
    return deltaPressureSupport == null ? false : deltaPressureSupport.isValid();
  }

  public SEScalarPressure getDeltaPressureSupport()
  {
    if (deltaPressureSupport == null)
      deltaPressureSupport = new SEScalarPressure();
    return deltaPressureSupport;
  }

  public boolean hasFractionInspiredOxygen()
  {
    return fractionInspiredOxygen == null ? false : fractionInspiredOxygen.isValid();
  }

  public SEScalar0To1 getFractionInspiredOxygen()
  {
    if (fractionInspiredOxygen == null)
      fractionInspiredOxygen = new SEScalar0To1();
    return fractionInspiredOxygen;
  }

  public boolean hasPositiveEndExpiredPressure()
  {
    return positiveEndExpiredPressure == null ? false : positiveEndExpiredPressure.isValid();
  }

  public SEScalarPressure getPositiveEndExpiredPressure()
  {
    if (positiveEndExpiredPressure == null)
      positiveEndExpiredPressure = new SEScalarPressure();
    return positiveEndExpiredPressure;
  }

  public boolean hasSlope()
  {
    return slope == null ? false : slope.isValid();
  }

  public SEScalarTime getSlope()
  {
    if (slope == null)
      slope = new SEScalarTime();
    return slope;
  }

  public String toString()
  {
    String str = "Mechanical Ventilator Volume Control";
    str += "\n\tDeltaPressureSupport: "
        + (hasDeltaPressureSupport() ? getDeltaPressureSupport().toString() : "Not Provided");
    str += "\n\tFractionInspiredOxygen: "
        + (hasFractionInspiredOxygen() ? getFractionInspiredOxygen().toString() : "Not Provided");
    str += "\n\tPositiveEndExpiredPressure: "
        + (hasPositiveEndExpiredPressure() ? getPositiveEndExpiredPressure().toString() : "Not Provided");
    str += "\n\tSlope: " + (hasSlope() ? getSlope().toString() : "Not Provided");

    return str;
  }
}
