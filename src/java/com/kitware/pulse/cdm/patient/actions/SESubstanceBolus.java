/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.SubstanceBolusData;
import com.kitware.pulse.cdm.bind.PatientActions.SubstanceBolusData.eRoute;
import com.kitware.pulse.cdm.properties.*;

public class SESubstanceBolus extends SEPatientAction
{

  private static final long serialVersionUID = -2177347285157137770L;
  protected eRoute adminRoute;
  protected SEScalarTime adminDuration;
  protected SEScalarMassPerVolume concentration;
  protected SEScalarVolume dose;
  protected String substance;
  
  public SESubstanceBolus()
  {
    this.adminRoute = null;
    this.adminDuration = null;
    this.dose = null;
    this.concentration = null;
    this.substance = "";
  }
  
  public void copy(SESubstanceBolus other)
  {
    if(this==other)
      return;
    super.copy(other);
    substance = other.substance;
    adminRoute = other.adminRoute;
    
    if (other.adminDuration != null)
      getAdministrationDuration().set(other.getAdministrationDuration());
    else if (adminDuration != null)
      adminDuration.invalidate();
    
    if (other.dose != null)
      getDose().set(other.getDose());
    else if (dose != null)
      dose.invalidate();
    
    if (other.concentration != null)
      getConcentration().set(other.getConcentration());
    else if (concentration != null)
      concentration.invalidate();
  }
  
  @Override
  public void reset()
  {
    super.reset();
    adminRoute = null;
    if (adminDuration != null)
      adminDuration.invalidate();
    if (dose != null)
      dose.invalidate();
    if (concentration != null)
      concentration.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasAdministrationRoute() && hasDose() && hasConcentration() && hasSubstance();
  }
  
  public static void load(SubstanceBolusData src, SESubstanceBolus dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    dst.setSubstance(src.getSubstance());
    if(src.hasAdministrationDuration())
      SEScalarTime.load(src.getAdministrationDuration(),dst.getAdministrationDuration());
    if(src.hasDose())
      SEScalarVolume.load(src.getDose(),dst.getDose());
    if(src.hasConcentration())
      SEScalarMassPerVolume.load(src.getConcentration(),dst.getConcentration());
    if(src.getAdministrationRoute()!=eRoute.UNRECOGNIZED)
    	dst.adminRoute = src.getAdministrationRoute();
  }
  
  public static SubstanceBolusData unload(SESubstanceBolus src)
  {
    SubstanceBolusData.Builder dst = SubstanceBolusData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESubstanceBolus src, SubstanceBolusData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasAdministrationDuration())
      dst.setAdministrationDuration(SEScalarTime.unload(src.adminDuration));
    if (src.hasDose())
      dst.setDose(SEScalarVolume.unload(src.dose));
    if (src.hasConcentration())
      dst.setConcentration(SEScalarMassPerVolume.unload(src.concentration));
    if (src.hasAdministrationRoute())
      dst.setAdministrationRoute(src.adminRoute);
    if (src.hasSubstance())
      dst.setSubstance(src.substance);
  }

  public eRoute getAdministrationRoute()
  {
    return adminRoute;
  }
  public void setAdministrationRoute(eRoute adminRoute)
  {
    this.adminRoute = adminRoute;
  }
  public boolean hasAdministrationRoute()
  {
    return adminRoute == null ? false : true;
  }
  
  public boolean hasAdministrationDuration()
  {
    return adminDuration == null ? false : adminDuration.isValid();
  }
  public SEScalarTime getAdministrationDuration()
  {
    if (adminDuration == null)
      adminDuration = new SEScalarTime();
    return adminDuration;
  }
  
  public boolean hasConcentration()
  {
    return concentration == null ? false : concentration.isValid();
  }
  public SEScalarMassPerVolume getConcentration()
  {
    if (concentration == null)
      concentration = new SEScalarMassPerVolume();
    return concentration;
  }
  
  public boolean hasDose()
  {
    return dose == null ? false : dose.isValid();
  }
  
  public SEScalarVolume getDose()
  {
    if (dose == null)
      dose = new SEScalarVolume();
    return dose;
  }
  
  public boolean hasSubstance(){return !substance.isEmpty();}
  public void setSubstance(String name)
  {
    substance = name==null?"":name;
  }
  public String getSubstance()
  {
    return substance;
  }
  
  @Override
  public String toString()
  {
    if (dose != null || concentration != null)
      return "Substance Bolus"
          + "\n\tDose: " + getDose()
          + "\n\tConcentration: " + getConcentration()
          + "\n\tSubstance: " + getSubstance()
          + "\n\tAdministration Route: " + getAdministrationRoute()
          + "\n\tAdministration Duration: " + getAdministrationDuration();
    else
      return "Action not specified properly";
  }
}
