/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEHemorrhage : SEPatientAction
  {
    public enum eType : int
    {
      External = 0,
      Internal
    }

    protected eType type;
    protected string compartment;
    protected SEScalarVolumePerTime flow_rate;
    protected SEScalar0To1 severity;

    public SEHemorrhage()
    {
      type = eType.External;
      compartment = null;
      flow_rate = null;
      severity = null;
    }

    public override void Clear()
    {
      base.Clear();
      type = eType.External;
      compartment = null;
      if (flow_rate != null)
        flow_rate.Invalidate();
      if (severity != null)
        severity.Invalidate();
    }

    public override bool IsValid()
    {
      return HasCompartment() && (HasFlowRate() || HasSeverity());
    }

    public new eType GetType()
    {
      return type;
    }
    public void SetType(eType t)
    {
      type = t;
    }

    public string GetCompartment()
    {
      return compartment;
    }
    public void SetCompartment(string name)
    {
      compartment = name;
    }
    public bool HasCompartment()
    {
      return !string.IsNullOrEmpty(compartment);
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
