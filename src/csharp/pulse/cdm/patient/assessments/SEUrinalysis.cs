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

    public eUrinalysis_UrineColor GetColor() { return this.color; }
    public void SetColor(eUrinalysis_UrineColor color) { this.color = color; }
    public bool HasColor() { return this.color == eUrinalysis_UrineColor.NullColor ? false : color != eUrinalysis_UrineColor.NullColor; }

    public eUrinalysis_ClarityIndicator GetAppearance() { return this.appearance; }
    public void SetAppearance(eUrinalysis_ClarityIndicator a) { this.appearance = a; }
    public bool HasAppearance() { return this.appearance == eUrinalysis_ClarityIndicator.NullClarity ? false : appearance != eUrinalysis_ClarityIndicator.NullClarity; }

    public eUrinalysis_PresenceIndicator GetGlucose() { return this.glucose; }
    public void SetGlucose(eUrinalysis_PresenceIndicator g) { this.glucose = g; }
    public bool HasGlucose() { return this.glucose == eUrinalysis_PresenceIndicator.NullPresence ? false : glucose != eUrinalysis_PresenceIndicator.NullPresence; }

    public eUrinalysis_PresenceIndicator GetKetone() { return this.ketone; }
    public void SetKetone(eUrinalysis_PresenceIndicator k) { this.ketone = k; }
    public bool HasKetone() { return this.ketone == eUrinalysis_PresenceIndicator.NullPresence ? false : ketone != eUrinalysis_PresenceIndicator.NullPresence; }

    public bool HasBilirubin()
    {
      return bilirubin == null ? false : bilirubin.IsValid();
    }
    public SEScalar GetBilirubin()
    {
      if (bilirubin == null)
        bilirubin = new SEScalar();
      return bilirubin;
    }

    public bool HasSpecificGravity()
    {
      return specificGravity == null ? false : specificGravity.IsValid();
    }
    public SEScalar GetSpecificGravity()
    {
      if (specificGravity == null)
        specificGravity = new SEScalar();
      return specificGravity;
    }

    public eUrinalysis_PresenceIndicator GetBlood() { return this.blood; }
    public void SetBlood(eUrinalysis_PresenceIndicator b) { this.blood = b; }
    public bool HasBlood() { return this.blood == eUrinalysis_PresenceIndicator.NullPresence ? false : blood != eUrinalysis_PresenceIndicator.NullPresence; }

    public bool HasPH()
    {
      return pH == null ? false : pH.IsValid();
    }
    public SEScalar GetPH()
    {
      if (pH == null)
        pH = new SEScalar();
      return pH;
    }

    public eUrinalysis_PresenceIndicator GetProtein() { return this.protein; }
    public void SetProtein(eUrinalysis_PresenceIndicator p) { this.protein = p; }
    public bool HasProtein() { return this.protein == eUrinalysis_PresenceIndicator.NullPresence ? false : protein != eUrinalysis_PresenceIndicator.NullPresence; }

    public bool HasUrobilinogen()
    {
      return urobilinogen == null ? false : urobilinogen.IsValid();
    }
    public SEScalarMassPerVolume GetUrobilinogen()
    {
      if (urobilinogen == null)
        urobilinogen = new SEScalarMassPerVolume();
      return urobilinogen;
    }

    public eUrinalysis_PresenceIndicator GetNitrite() { return this.nitrite; }
    public void SetNitrite(eUrinalysis_PresenceIndicator n) { this.nitrite = n; }
    public bool HasNitrite() { return this.nitrite == eUrinalysis_PresenceIndicator.NullPresence ? false : nitrite != eUrinalysis_PresenceIndicator.NullPresence; }

    public eUrinalysis_PresenceIndicator GetLeukocyteEsterase() { return this.leukocyteEsterase; }
    public void SetLeukocyteEsterase(eUrinalysis_PresenceIndicator l) { this.leukocyteEsterase = l; }
    public bool HasLeukocyteEsterase() { return this.leukocyteEsterase == eUrinalysis_PresenceIndicator.NullPresence ? false : leukocyteEsterase != eUrinalysis_PresenceIndicator.NullPresence; }

    public bool HasMicroscopic()
    {
      return microscopic == null ? false : true;
    }
    public SEUrinalysisMicroscopic GetMicroscopic()
    {
      if (microscopic == null)
        microscopic = new SEUrinalysisMicroscopic();
      return microscopic;
    }
  }
}
