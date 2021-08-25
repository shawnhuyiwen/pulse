/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eUrinalysis_UrineColor : int
  {
    NullColor = 0,
    PaleYellow,
    Yellow,
    DarkYellow
  }

  public enum eUrinalysis_ClarityIndicator : int
  {
    NullClarity = 0,
    Translucent,
    SlightlyCloudy,
    Cloudy,
    Turbid
  }

  public enum eUrinalysis_PresenceIndicator : int
  {
    NullPresence = 0,
    Positive,
    Negative
  }

  public class SEUrinalysis : SEPatientAssessment
  {
    protected eUrinalysis_UrineColor color;
    protected eUrinalysis_ClarityIndicator appearance;
    protected eUrinalysis_PresenceIndicator glucose;
    protected eUrinalysis_PresenceIndicator ketone;
    protected SEScalar bilirubin;
    protected SEScalar specificGravity;
    protected eUrinalysis_PresenceIndicator blood;
    protected SEScalar pH;
    protected eUrinalysis_PresenceIndicator protein;
    protected SEScalarMassPerVolume urobilinogen;
    protected eUrinalysis_PresenceIndicator nitrite;
    protected eUrinalysis_PresenceIndicator leukocyteEsterase;

    protected SEUrinalysisMicroscopic microscopic;

    public override void Clear()
    {
      base.Clear();
      this.color = eUrinalysis_UrineColor.NullColor;
      this.appearance = eUrinalysis_ClarityIndicator.NullClarity;
      this.urobilinogen = null;
      this.ketone = eUrinalysis_PresenceIndicator.NullPresence;
      if (this.bilirubin != null)
        this.bilirubin.Invalidate();
      if (this.specificGravity != null)
        this.specificGravity.Invalidate();
      this.blood = eUrinalysis_PresenceIndicator.NullPresence;
      if (this.pH != null)
        this.pH.Invalidate();
      this.protein = eUrinalysis_PresenceIndicator.NullPresence;
      if (this.urobilinogen != null)
        this.urobilinogen.Invalidate();
      this.nitrite = eUrinalysis_PresenceIndicator.NullPresence;
      this.leukocyteEsterase = eUrinalysis_PresenceIndicator.NullPresence;

      if (this.microscopic != null)
        this.microscopic.Clear();
    }

    public eUrinalysis_UrineColor GetColorResult() { return this.color; }
    public void SetColorResult(eUrinalysis_UrineColor color) { this.color = color; }
    public bool HasColorResult() { return this.color == eUrinalysis_UrineColor.NullColor ? false : color != eUrinalysis_UrineColor.NullColor; }

    public eUrinalysis_ClarityIndicator GetAppearanceResult() { return this.appearance; }
    public void SetAppearanceResult(eUrinalysis_ClarityIndicator a) { this.appearance = a; }
    public bool HasAppearanceResult() { return this.appearance == eUrinalysis_ClarityIndicator.NullClarity ? false : appearance != eUrinalysis_ClarityIndicator.NullClarity; }

    public eUrinalysis_PresenceIndicator GetGlucoseResult() { return this.glucose; }
    public void SetGlucoseResult(eUrinalysis_PresenceIndicator g) { this.glucose = g; }
    public bool HasGlucoseResult() { return this.glucose == eUrinalysis_PresenceIndicator.NullPresence ? false : glucose != eUrinalysis_PresenceIndicator.NullPresence; }

    public eUrinalysis_PresenceIndicator GetKetoneResult() { return this.ketone; }
    public void SetKetoneResult(eUrinalysis_PresenceIndicator k) { this.ketone = k; }
    public bool HasKetoneResult() { return this.ketone == eUrinalysis_PresenceIndicator.NullPresence ? false : ketone != eUrinalysis_PresenceIndicator.NullPresence; }

    public bool HasBilirubinResult()
    {
      return bilirubin == null ? false : bilirubin.IsValid();
    }
    public SEScalar GetBilirubinResult()
    {
      if (bilirubin == null)
        bilirubin = new SEScalar();
      return bilirubin;
    }

    public bool HasSpecificGravityResult()
    {
      return specificGravity == null ? false : specificGravity.IsValid();
    }
    public SEScalar GetSpecificGravityResult()
    {
      if (specificGravity == null)
        specificGravity = new SEScalar();
      return specificGravity;
    }

    public eUrinalysis_PresenceIndicator GetBloodResult() { return this.blood; }
    public void SetBloodResult(eUrinalysis_PresenceIndicator b) { this.blood = b; }
    public bool HasBloodResult() { return this.blood == eUrinalysis_PresenceIndicator.NullPresence ? false : blood != eUrinalysis_PresenceIndicator.NullPresence; }

    public bool HasPHResult()
    {
      return pH == null ? false : pH.IsValid();
    }
    public SEScalar GetPHResult()
    {
      if (pH == null)
        pH = new SEScalar();
      return pH;
    }

    public eUrinalysis_PresenceIndicator GetProteinResult() { return this.protein; }
    public void SetProteinResult(eUrinalysis_PresenceIndicator p) { this.protein = p; }
    public bool HasProteinResult() { return this.protein == eUrinalysis_PresenceIndicator.NullPresence ? false : protein != eUrinalysis_PresenceIndicator.NullPresence; }

    public bool HasUrobilinogenResult()
    {
      return urobilinogen == null ? false : urobilinogen.IsValid();
    }
    public SEScalarMassPerVolume GetUrobilinogenResult()
    {
      if (urobilinogen == null)
        urobilinogen = new SEScalarMassPerVolume();
      return urobilinogen;
    }

    public eUrinalysis_PresenceIndicator GetNitriteResult() { return this.nitrite; }
    public void SetNitriteResult(eUrinalysis_PresenceIndicator n) { this.nitrite = n; }
    public bool HasNitriteResult() { return this.nitrite == eUrinalysis_PresenceIndicator.NullPresence ? false : nitrite != eUrinalysis_PresenceIndicator.NullPresence; }

    public eUrinalysis_PresenceIndicator GetLeukocyteEsteraseResult() { return this.leukocyteEsterase; }
    public void SetLeukocyteEsteraseResult(eUrinalysis_PresenceIndicator l) { this.leukocyteEsterase = l; }
    public bool HasLeukocyteEsteraseResult() { return this.leukocyteEsterase == eUrinalysis_PresenceIndicator.NullPresence ? false : leukocyteEsterase != eUrinalysis_PresenceIndicator.NullPresence; }

    public bool HasMicroscopicResult()
    {
      return microscopic == null ? false : true;
    }
    public SEUrinalysisMicroscopic GetMicroscopicResult()
    {
      if (microscopic == null)
        microscopic = new SEUrinalysisMicroscopic();
      return microscopic;
    }
  }
}
