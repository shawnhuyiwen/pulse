/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEUrinalysisMicroscopic
  {
    public enum eObservationType : int
    {
      NullObservationType = 0,
      LowPowerField,
      HighPowerField
    }

    public enum eObservationAmount : int
    {
      NullObservationAmount = 0,
      Few,
      Moderate,
      Many
    }

    protected eObservationType observationType;
    protected SEScalarAmount redBloodCells;
    protected SEScalarAmount whiteBloodCells;
    protected eObservationAmount epithelialCells;
    protected SEScalarAmount casts;
    protected eObservationAmount crystals;
    protected eObservationAmount bacteria;
    protected eObservationAmount trichomonads;
    protected eObservationAmount yeast;

    public void Clear()
    {
      this.observationType = eObservationType.NullObservationType;
      if (this.redBloodCells != null)
        this.redBloodCells.Invalidate();
      if (this.whiteBloodCells != null)
        this.whiteBloodCells.Invalidate();
      this.epithelialCells = eObservationAmount.NullObservationAmount;
      if (this.casts != null)
        this.casts.Invalidate();
      this.crystals = eObservationAmount.NullObservationAmount;
      this.bacteria = eObservationAmount.NullObservationAmount;
      this.trichomonads = eObservationAmount.NullObservationAmount;
      this.yeast = eObservationAmount.NullObservationAmount;
    }

    public eObservationType GetObservationType() { return this.observationType; }
    public void SetObservationType(eObservationType observationType) { this.observationType = observationType; }
    public bool HasObservationType() { return this.observationType == eObservationType.NullObservationType ? false : observationType != eObservationType.NullObservationType; }

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

    public eObservationAmount GetEpithelialCellsResult() { return this.epithelialCells; }
    public void SetEpithelialCellsResult(eObservationAmount a) { this.epithelialCells = a; }
    public bool HasEpithelialCellsResult() { return this.epithelialCells == eObservationAmount.NullObservationAmount ? false : epithelialCells != eObservationAmount.NullObservationAmount; }

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

    public eObservationAmount GetCrystalsResult() { return this.crystals; }
    public void SetCrystalsResult(eObservationAmount g) { this.crystals = g; }
    public bool HasCrystalsResult() { return this.crystals == eObservationAmount.NullObservationAmount ? false : crystals != eObservationAmount.NullObservationAmount; }

    public eObservationAmount GetBacteriaResult() { return this.bacteria; }
    public void SetBacteriaResult(eObservationAmount k) { this.bacteria = k; }
    public bool HasBacteriaResult() { return this.bacteria == eObservationAmount.NullObservationAmount ? false : bacteria != eObservationAmount.NullObservationAmount; }

    public eObservationAmount GetTrichomonadsResult() { return this.trichomonads; }
    public void SetTrichomonadsResult(eObservationAmount n) { this.trichomonads = n; }
    public bool HasTrichomonadsResult() { return this.trichomonads == eObservationAmount.NullObservationAmount ? false : trichomonads != eObservationAmount.NullObservationAmount; }

    public eObservationAmount GetYeastResult() { return this.yeast; }
    public void SetYeastResult(eObservationAmount l) { this.yeast = l; }
    public bool HasYeastResult() { return this.yeast == eObservationAmount.NullObservationAmount ? false : yeast != eObservationAmount.NullObservationAmount; }
  }
}
