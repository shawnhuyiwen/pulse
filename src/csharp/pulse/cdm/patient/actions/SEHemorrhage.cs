/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eHemorrhage_Compartment : int
  {
    None = 0,
    Aorta = 1,
    Brain = 2,
    Muscle = 3,
    LargeIntestine = 4,
    LeftArm = 5,
    LeftKidney = 6,
    LeftLeg = 7,
    Liver = 8,
    RightArm = 9,
    RightKidney = 10,
    RightLeg = 11,
    Skin = 12,
    SmallIntestine = 13,
    Splanchnic = 14,
    Spleen = 15,
    VenaCava = 16
  }
  public enum eHemorrhage_Type : int
  {
    External = 0,
    Internal
  }
  public class SEHemorrhage : SEPatientAction
  {
    protected eHemorrhage_Type type;
    protected eHemorrhage_Compartment compartment;
    protected SEScalarVolumePerTime flow_rate;
    protected SEScalar0To1 severity;

    public SEHemorrhage()
    {
      type = eHemorrhage_Type.External;
      compartment = eHemorrhage_Compartment.None;
      flow_rate = null;
      severity = null;
    }

    public override void Clear()
    {
      base.Clear();
      type = eHemorrhage_Type.External;
      compartment = eHemorrhage_Compartment.None;
      if (flow_rate != null)
        flow_rate.Invalidate();
      if (severity != null)
        severity.Invalidate();
    }

    public override bool IsValid()
    {
      return HasCompartment() && (HasFlowRate() || HasSeverity());
    }

    public new eHemorrhage_Type GetType()
    {
      return type;
    }
    public void SetType(eHemorrhage_Type t)
    {
      type = t;
    }

    public eHemorrhage_Compartment GetCompartment()
    {
      return compartment;
    }
    public void SetCompartment(eHemorrhage_Compartment cmpt)
    {
      compartment = cmpt;
    }
    public bool HasCompartment()
    {
      return compartment != eHemorrhage_Compartment.None;
    }

    public bool HasFlowRate()
    {
      return flow_rate == null ? false : flow_rate.IsValid();
    }
    public SEScalarVolumePerTime GetFlowRate()
    {
      if (flow_rate == null)
        flow_rate = new SEScalarVolumePerTime();
      return flow_rate;
    }

    public bool HasSeverity()
    {
      return severity == null ? false : severity.IsValid();
    }
    public SEScalar0To1 GetSeverity()
    {
      if (severity == null)
        severity = new SEScalar0To1();
      return severity;
    }
  }
}
