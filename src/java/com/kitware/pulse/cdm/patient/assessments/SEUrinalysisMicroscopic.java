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
    this.redBloodCells = null;
    this.whiteBloodCells = null;
    this.epithelialCells = null;
    this.casts = null;
    this.crystals = null;
    this.bacteria = null;
    this.trichomonads = null;
    this.yeast = null;
  }
  
  public void reset()
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
      SEScalarAmount.load(src.getRedBloodCells(),dst.getRedBloodCellsResult());
    if(src.hasWhiteBloodCells())
      SEScalarAmount.load(src.getWhiteBloodCells(),dst.getWhiteBloodCellsResult());
    if(src.getEpithelialCells()!=eObservationAmount.UNRECOGNIZED)
      dst.setEpithelialCellsResult(src.getEpithelialCells());
    if(src.hasCasts())
      SEScalarAmount.load(src.getCasts(),dst.getCastsResult());
    if(src.getCrystals()!=eObservationAmount.UNRECOGNIZED)
      dst.setCrystalsResult(src.getCrystals());
    if(src.getBacteria()!=eObservationAmount.UNRECOGNIZED)
      dst.setBacteriaResult(src.getBacteria());
    if(src.getTrichomonads()!=eObservationAmount.UNRECOGNIZED)
      dst.setTrichomonadsResult(src.getTrichomonads());
    if(src.getYeast()!=eObservationAmount.UNRECOGNIZED)
      dst.setYeastResult(src.getYeast());
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
    if (src.hasRedBloodCellsResult())
      dst.setRedBloodCells(SEScalarAmount.unload(src.redBloodCells));
    if (src.hasWhiteBloodCellsResult())
        dst.setWhiteBloodCells(SEScalarAmount.unload(src.whiteBloodCells));
    if (src.hasEpithelialCellsResult())
      dst.setEpithelialCells(src.epithelialCells);
    if (src.hasCastsResult())
      dst.setCasts(SEScalarAmount.unload(src.casts));
    if (src.hasCrystalsResult())
      dst.setCrystals(src.crystals);
    if (src.hasBacteriaResult())
      dst.setBacteria(src.bacteria);
    if (src.hasTrichomonadsResult())
      dst.setTrichomonads(src.trichomonads);
    if (src.hasYeastResult())
      dst.setYeast(src.yeast);
  }
  
  public eObservationType getObservationType() { return this.observationType;}
  public void    setObservationType(eObservationType observationType){this.observationType=observationType;}
  public boolean hasObservationType(){return this.observationType==null?false:observationType!=eObservationType.NullObservationType;}
  
  public boolean hasRedBloodCellsResult()
  {
    return redBloodCells == null ? false : redBloodCells.isValid();
  }
  public SEScalarAmount getRedBloodCellsResult()
  {
    if (redBloodCells == null)
      redBloodCells = new SEScalarAmount();
    return redBloodCells;
  }

  public boolean hasWhiteBloodCellsResult()
  {
    return redBloodCells == null ? false : redBloodCells.isValid();
  }
  public SEScalarAmount getWhiteBloodCellsResult()
  {
    if (redBloodCells == null)
      redBloodCells = new SEScalarAmount();
    return redBloodCells;
  }
  
  public eObservationAmount  getEpithelialCellsResult() { return this.epithelialCells;}
  public void    setEpithelialCellsResult(eObservationAmount a){this.epithelialCells=a;}
  public boolean hasEpithelialCellsResult(){return this.epithelialCells==null?false:epithelialCells!=eObservationAmount.NullObservationAmount;}
  
  public boolean hasCastsResult()
  {
    return casts == null ? false : casts.isValid();
  }
  public SEScalarAmount getCastsResult()
  {
    if (casts == null)
      casts = new SEScalarAmount();
    return casts;
  }
  
  public eObservationAmount  getCrystalsResult() { return this.crystals;}
  public void    setCrystalsResult(eObservationAmount g){this.crystals=g;}
  public boolean hasCrystalsResult(){return this.crystals==null?false:crystals!=eObservationAmount.NullObservationAmount;}
  
  public eObservationAmount  getBacteriaResult() { return this.bacteria;}
  public void    setBacteriaResult(eObservationAmount k){this.bacteria=k;}
  public boolean hasBacteriaResult(){return this.bacteria==null?false:bacteria!=eObservationAmount.NullObservationAmount;}
  
  public eObservationAmount  getTrichomonadsResult() { return this.trichomonads;}
  public void    setTrichomonadsResult(eObservationAmount n){this.trichomonads=n;}
  public boolean hasTrichomonadsResult(){return this.trichomonads==null?false:trichomonads!=eObservationAmount.NullObservationAmount;}
  
  public eObservationAmount  getYeastResult() { return this.yeast;}
  public void    setYeastResult(eObservationAmount l){this.yeast=l;}
  public boolean hasYeastResult(){return this.yeast==null?false:yeast!=eObservationAmount.NullObservationAmount;}
}
