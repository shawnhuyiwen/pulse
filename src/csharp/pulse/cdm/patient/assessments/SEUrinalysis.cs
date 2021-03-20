/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEUrinalysis : SEPatientAssessment
  {
    public enum eUrineColor : int
    {
      NullColor = 0,
      PaleYellow,
      Yellow,
      DarkYellow
    }

    public enum eClarityIndicator : int
    {
      NullClarity = 0,
      Translucent,
      SlightlyCloudy,
      Cloudy,
      Turbid
    }

    public enum ePresenceIndicator : int
    {
      NullPresence = 0,
      Positive,
      Negative
    }

    protected eUrineColor color;
    protected eClarityIndicator appearance;
    protected ePresenceIndicator glucose;
    protected ePresenceIndicator ketone;
    protected SEScalar bilirubin;
    protected SEScalar specificGravity;
    protected ePresenceIndicator blood;
    protected SEScalar pH;
    protected ePresenceIndicator protein;
    protected SEScalarMassPerVolume urobilinogen;
    protected ePresenceIndicator nitrite;
    protected ePresenceIndicator leukocyteEsterase;

    protected SEUrinalysisMicroscopic microscopic;

    public override void Clear()
    {
      base.Clear();
      this.color = eUrineColor.NullColor;
      this.appearance = eClarityIndicator.NullClarity;
      this.urobilinogen = null;
      this.ketone = ePresenceIndicator.NullPresence;
      if (this.bilirubin != null)
        this.bilirubin.Invalidate();
      if (this.specificGravity != null)
        this.specificGravity.Invalidate();
      this.blood = ePresenceIndicator.NullPresence;
      if (this.pH != null)
        this.pH.Invalidate();
      this.protein = ePresenceIndicator.NullPresence;
      if (this.urobilinogen != null)
        this.urobilinogen.Invalidate();
      this.nitrite = ePresenceIndicator.NullPresence;
      this.leukocyteEsterase = ePresenceIndicator.NullPresence;

      if (this.microscopic != null)
        this.microscopic.Clear();
    }

    public eUrineColor GetColorResult() { return this.color; }
    public void SetColorResult(eUrineColor color) { this.color = color; }
    public bool HasColorResult() { return this.color == eUrineColor.NullColor ? false : color != eUrineColor.NullColor; }

    public eClarityIndicator GetAppearanceResult() { return this.appearance; }
    public void SetAppearanceResult(eClarityIndicator a) { this.appearance = a; }
    public bool HasAppearanceResult() { return this.appearance == eClarityIndicator.NullClarity ? false : appearance != eClarityIndicator.NullClarity; }

    public ePresenceIndicator GetGlucoseResult() { return this.glucose; }
    public void SetGlucoseResult(ePresenceIndicator g) { this.glucose = g; }
    public bool HasGlucoseResult() { return this.glucose == ePresenceIndicator.NullPresence ? false : glucose != ePresenceIndicator.NullPresence; }

    public ePresenceIndicator GetKetoneResult() { return this.ketone; }
    public void SetKetoneResult(ePresenceIndicator k) { this.ketone = k; }
    public bool HasKetoneResult() { return this.ketone == ePresenceIndicator.NullPresence ? false : ketone != ePresenceIndicator.NullPresence; }

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

    public ePresenceIndicator GetBloodResult() { return this.blood; }
    public void SetBloodResult(ePresenceIndicator b) { this.blood = b; }
    public bool HasBloodResult() { return this.blood == ePresenceIndicator.NullPresence ? false : blood != ePresenceIndicator.NullPresence; }

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

    public ePresenceIndicator GetProteinResult() { return this.protein; }
    public void SetProteinResult(ePresenceIndicator p) { this.protein = p; }
    public bool HasProteinResult() { return this.protein == ePresenceIndicator.NullPresence ? false : protein != ePresenceIndicator.NullPresence; }

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

    public ePresenceIndicator GetNitriteResult() { return this.nitrite; }
    public void SetNitriteResult(ePresenceIndicator n) { this.nitrite = n; }
    public bool HasNitriteResult() { return this.nitrite == ePresenceIndicator.NullPresence ? false : nitrite != ePresenceIndicator.NullPresence; }

    public ePresenceIndicator GetLeukocyteEsteraseResult() { return this.leukocyteEsterase; }
    public void SetLeukocyteEsteraseResult(ePresenceIndicator l) { this.leukocyteEsterase = l; }
    public bool HasLeukocyteEsteraseResult() { return this.leukocyteEsterase == ePresenceIndicator.NullPresence ? false : leukocyteEsterase != ePresenceIndicator.NullPresence; }

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
