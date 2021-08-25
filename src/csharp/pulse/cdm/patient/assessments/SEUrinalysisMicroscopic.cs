/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eUrinalysis_MicroscopicObservationType : int
  {
    NullObservationType = 0,
    LowPowerField,
    HighPowerField
  }

  public enum eUrinalysis_MicroscopicObservationAmount : int
  {
    NullObservationAmount = 0,
    Few,
    Moderate,
    Many
  }

  public class SEUrinalysisMicroscopic
  {
    protected eUrinalysis_MicroscopicObservationType   observationType;
    protected SEScalarAmount                           redBloodCells;
    protected SEScalarAmount                           whiteBloodCells;
    protected eUrinalysis_MicroscopicObservationAmount epithelialCells;
    protected SEScalarAmount                           casts;
    protected eUrinalysis_MicroscopicObservationAmount crystals;
    protected eUrinalysis_MicroscopicObservationAmount bacteria;
    protected eUrinalysis_MicroscopicObservationAmount trichomonads;
    protected eUrinalysis_MicroscopicObservationAmount yeast;

    public void Clear()
    {
      this.observationType = eUrinalysis_MicroscopicObservationType.NullObservationType;
      if (this.redBloodCells != null)
        this.redBloodCells.Invalidate();
      if (this.whiteBloodCells != null)
        this.whiteBloodCells.Invalidate();
      this.epithelialCells = eUrinalysis_MicroscopicObservationAmount.NullObservationAmount;
      if (this.casts != null)
        this.casts.Invalidate();
      this.crystals = eUrinalysis_MicroscopicObservationAmount.NullObservationAmount;
      this.bacteria = eUrinalysis_MicroscopicObservationAmount.NullObservationAmount;
      this.trichomonads = eUrinalysis_MicroscopicObservationAmount.NullObservationAmount;
      this.yeast = eUrinalysis_MicroscopicObservationAmount.NullObservationAmount;
    }

    public eUrinalysis_MicroscopicObservationType GetObservationType() { return this.observationType; }
    public void SetObservationType(eUrinalysis_MicroscopicObservationType observationType) { this.observationType = observationType; }
    public bool HasObservationType() { return this.observationType == eUrinalysis_MicroscopicObservationType.NullObservationType ? false : observationType != eUrinalysis_MicroscopicObservationType.NullObservationType; }

    public bool HasRedBloodCellsResult()
    {
      return redBloodCells == null ? false : redBloodCells.IsValid();
    }
    public SEScalarAmount GetRedBloodCellsResult()
    {
      if (redBloodCells == null)
        redBloodCells = new SEScalarAmount();
      return redBloodCells;
    }

    public bool HasWhiteBloodCellsResult()
    {
      return redBloodCells == null ? false : redBloodCells.IsValid();
    }
    public SEScalarAmount GetWhiteBloodCellsResult()
    {
      if (redBloodCells == null)
        redBloodCells = new SEScalarAmount();
      return redBloodCells;
    }

    public eUrinalysis_MicroscopicObservationAmount GetEpithelialCellsResult() { return this.epithelialCells; }
    public void SetEpithelialCellsResult(eUrinalysis_MicroscopicObservationAmount a) { this.epithelialCells = a; }
    public bool HasEpithelialCellsResult() { return this.epithelialCells == eUrinalysis_MicroscopicObservationAmount.NullObservationAmount ? false : epithelialCells != eUrinalysis_MicroscopicObservationAmount.NullObservationAmount; }

    public bool HasCastsResult()
    {
      return casts == null ? false : casts.IsValid();
    }
    public SEScalarAmount GetCastsResult()
    {
      if (casts == null)
        casts = new SEScalarAmount();
      return casts;
    }

    public eUrinalysis_MicroscopicObservationAmount GetCrystalsResult() { return this.crystals; }
    public void SetCrystalsResult(eUrinalysis_MicroscopicObservationAmount g) { this.crystals = g; }
    public bool HasCrystalsResult() { return this.crystals == eUrinalysis_MicroscopicObservationAmount.NullObservationAmount ? false : crystals != eUrinalysis_MicroscopicObservationAmount.NullObservationAmount; }

    public eUrinalysis_MicroscopicObservationAmount GetBacteriaResult() { return this.bacteria; }
    public void SetBacteriaResult(eUrinalysis_MicroscopicObservationAmount k) { this.bacteria = k; }
    public bool HasBacteriaResult() { return this.bacteria == eUrinalysis_MicroscopicObservationAmount.NullObservationAmount ? false : bacteria != eUrinalysis_MicroscopicObservationAmount.NullObservationAmount; }

    public eUrinalysis_MicroscopicObservationAmount GetTrichomonadsResult() { return this.trichomonads; }
    public void SetTrichomonadsResult(eUrinalysis_MicroscopicObservationAmount n) { this.trichomonads = n; }
    public bool HasTrichomonadsResult() { return this.trichomonads == eUrinalysis_MicroscopicObservationAmount.NullObservationAmount ? false : trichomonads != eUrinalysis_MicroscopicObservationAmount.NullObservationAmount; }

    public eUrinalysis_MicroscopicObservationAmount GetYeastResult() { return this.yeast; }
    public void SetYeastResult(eUrinalysis_MicroscopicObservationAmount l) { this.yeast = l; }
    public bool HasYeastResult() { return this.yeast == eUrinalysis_MicroscopicObservationAmount.NullObservationAmount ? false : yeast != eUrinalysis_MicroscopicObservationAmount.NullObservationAmount; }
  }
}
