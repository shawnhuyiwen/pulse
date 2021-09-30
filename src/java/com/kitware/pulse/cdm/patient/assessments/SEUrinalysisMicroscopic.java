/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.patient.assessments;

import com.kitware.pulse.cdm.bind.PatientAssessments.*;
import com.kitware.pulse.cdm.bind.PatientAssessments.UrinalysisMicroscopicData.*;
import com.kitware.pulse.cdm.properties.SEScalarAmount;

public class SEUrinalysisMicroscopic
{
  protected eObservationType       observationType;
  protected SEScalarAmount         redBloodCells;
  protected SEScalarAmount         whiteBloodCells;
  protected eObservationAmount     epithelialCells;
  protected SEScalarAmount         casts;
  protected eObservationAmount     crystals;
  protected eObservationAmount     bacteria;
  protected eObservationAmount     trichomonads;  
  protected eObservationAmount     yeast;
  
  public void clear()
  {
    this.observationType = null;
    if(this.redBloodCells!=null)
      this.redBloodCells.invalidate();
    if(this.whiteBloodCells!=null)
      this.whiteBloodCells.invalidate();
    this.epithelialCells = null;
    if(this.casts!=null)
      this.casts.invalidate();
    this.crystals = null;
    this.bacteria = null;
    this.trichomonads = null;
    this.yeast = null;
  }
  
  public static void load(UrinalysisMicroscopicData src, SEUrinalysisMicroscopic dst)
  {
    if(src.getObservationType()!=eObservationType.UNRECOGNIZED)
      dst.setObservationType(src.getObservationType());
    if(src.hasRedBloodCells())
      SEScalarAmount.load(src.getRedBloodCells(),dst.getRedBloodCells());
    if(src.hasWhiteBloodCells())
      SEScalarAmount.load(src.getWhiteBloodCells(),dst.getWhiteBloodCells());
    if(src.getEpithelialCells()!=eObservationAmount.UNRECOGNIZED)
      dst.setEpithelialCells(src.getEpithelialCells());
    if(src.hasCasts())
      SEScalarAmount.load(src.getCasts(),dst.getCasts());
    if(src.getCrystals()!=eObservationAmount.UNRECOGNIZED)
      dst.setCrystals(src.getCrystals());
    if(src.getBacteria()!=eObservationAmount.UNRECOGNIZED)
      dst.setBacteria(src.getBacteria());
    if(src.getTrichomonads()!=eObservationAmount.UNRECOGNIZED)
      dst.setTrichomonads(src.getTrichomonads());
    if(src.getYeast()!=eObservationAmount.UNRECOGNIZED)
      dst.setYeast(src.getYeast());
  }
  
  public static UrinalysisMicroscopicData unload(SEUrinalysisMicroscopic src)
  {
    UrinalysisMicroscopicData.Builder dst = UrinalysisMicroscopicData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEUrinalysisMicroscopic src, UrinalysisMicroscopicData.Builder dst)
  {
    if (src.hasObservationType())
      dst.setObservationType(src.observationType);
    if (src.hasRedBloodCells())
      dst.setRedBloodCells(SEScalarAmount.unload(src.redBloodCells));
    if (src.hasWhiteBloodCells())
        dst.setWhiteBloodCells(SEScalarAmount.unload(src.whiteBloodCells));
    if (src.hasEpithelialCells())
      dst.setEpithelialCells(src.epithelialCells);
    if (src.hasCasts())
      dst.setCasts(SEScalarAmount.unload(src.casts));
    if (src.hasCrystals())
      dst.setCrystals(src.crystals);
    if (src.hasBacteria())
      dst.setBacteria(src.bacteria);
    if (src.hasTrichomonads())
      dst.setTrichomonads(src.trichomonads);
    if (src.hasYeast())
      dst.setYeast(src.yeast);
  }
  
  public eObservationType getObservationType() { return this.observationType;}
  public void    setObservationType(eObservationType observationType){this.observationType=observationType;}
  public boolean hasObservationType(){return this.observationType==null?false:observationType!=eObservationType.NullObservationType;}
  
  public boolean hasRedBloodCells()
  {
    return redBloodCells == null ? false : redBloodCells.isValid();
  }
  public SEScalarAmount getRedBloodCells()
  {
    if (redBloodCells == null)
      redBloodCells = new SEScalarAmount();
    return redBloodCells;
  }

  public boolean hasWhiteBloodCells()
  {
    return redBloodCells == null ? false : redBloodCells.isValid();
  }
  public SEScalarAmount getWhiteBloodCells()
  {
    if (redBloodCells == null)
      redBloodCells = new SEScalarAmount();
    return redBloodCells;
  }
  
  public eObservationAmount  getEpithelialCells() { return this.epithelialCells;}
  public void    setEpithelialCells(eObservationAmount a){this.epithelialCells=a;}
  public boolean hasEpithelialCells(){return this.epithelialCells==null?false:epithelialCells!=eObservationAmount.NullObservationAmount;}
  
  public boolean hasCasts()
  {
    return casts == null ? false : casts.isValid();
  }
  public SEScalarAmount getCasts()
  {
    if (casts == null)
      casts = new SEScalarAmount();
    return casts;
  }
  
  public eObservationAmount  getCrystals() { return this.crystals;}
  public void    setCrystals(eObservationAmount g){this.crystals=g;}
  public boolean hasCrystals(){return this.crystals==null?false:crystals!=eObservationAmount.NullObservationAmount;}
  
  public eObservationAmount  getBacteria() { return this.bacteria;}
  public void    setBacteria(eObservationAmount k){this.bacteria=k;}
  public boolean hasBacteria(){return this.bacteria==null?false:bacteria!=eObservationAmount.NullObservationAmount;}
  
  public eObservationAmount  getTrichomonads() { return this.trichomonads;}
  public void    setTrichomonads(eObservationAmount n){this.trichomonads=n;}
  public boolean hasTrichomonads(){return this.trichomonads==null?false:trichomonads!=eObservationAmount.NullObservationAmount;}
  
  public eObservationAmount  getYeast() { return this.yeast;}
  public void    setYeast(eObservationAmount l){this.yeast=l;}
  public boolean hasYeast(){return this.yeast==null?false:yeast!=eObservationAmount.NullObservationAmount;}
}
