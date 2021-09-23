/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eHemorrhage_Type : int
  {
    External = 0,
    Internal
  }
  public class SEHemorrhage : SEPatientAction
  {
    // Convient 'enum' style class for often used compartments
    // Noted, External Hemorrhages are not limited to these compartments
    public class ExternalCompartment
    {
      public ExternalCompartment(string v) { value = v; }
      public readonly string value;
      public static ExternalCompartment RightLeg = new ExternalCompartment("RightLeg");
      public static ExternalCompartment LeftLeg = new ExternalCompartment("LeftLeg");
      public static ExternalCompartment RightArm = new ExternalCompartment("RightArm");
      public static ExternalCompartment LeftArm = new ExternalCompartment("LeftArm");
      public static ExternalCompartment Skin = new ExternalCompartment("Skin");
      public static ExternalCompartment Muscle = new ExternalCompartment("Muscle");
      public static ExternalCompartment Brain = new ExternalCompartment("Brain");
      public static ExternalCompartment LeftKidney = new ExternalCompartment("LeftKidney");
      public static ExternalCompartment RightKidney = new ExternalCompartment("RightKidney");
      public static ExternalCompartment Liver = new ExternalCompartment("Liver");
      public static ExternalCompartment Spleen = new ExternalCompartment("Spleen");
      public static ExternalCompartment Splanchnic = new ExternalCompartment("Splanchnic");
      public static ExternalCompartment SmallIntestine = new ExternalCompartment("SmallIntestine");
      public static ExternalCompartment LargeIntestine = new ExternalCompartment("LargeIntestine");
      public static ExternalCompartment Aorta = new ExternalCompartment("Aorta");
      public static ExternalCompartment VenaCava = new ExternalCompartment("VenaCava");
    }
    public class InternalCompartment
    {
      public InternalCompartment(string v) { value = v; }
      public readonly string value;
      public static InternalCompartment LeftKidney = new InternalCompartment("LeftKidney");
      public static InternalCompartment RightKidney = new InternalCompartment("RightKidney");
      public static InternalCompartment Liver = new InternalCompartment("Liver");
      public static InternalCompartment Spleen = new InternalCompartment("Spleen");
      public static InternalCompartment Splanchnic = new InternalCompartment("Splanchnic");
      public static InternalCompartment SmallIntestine = new InternalCompartment("SmallIntestine");
      public static InternalCompartment LargeIntestine = new InternalCompartment("LargeIntestine");
      public static InternalCompartment Aorta = new InternalCompartment("Aorta");
      public static InternalCompartment VenaCava = new InternalCompartment("VenaCava");
    }

    protected eHemorrhage_Type type;
    protected string compartment;
    protected SEScalarVolumePerTime flow_rate;
    protected SEScalar0To1 severity;

    public SEHemorrhage()
    {
      type = eHemorrhage_Type.External;
      compartment = null;
      flow_rate = null;
      severity = null;
    }

    public override void Clear()
    {
      base.Clear();
      type = eHemorrhage_Type.External;
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

    public new eHemorrhage_Type GetType()
    {
      return type;
    }
    public void SetType(eHemorrhage_Type t)
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

    public void SetExternal(ExternalCompartment c)
    {
      type = eHemorrhage_Type.External;
      compartment = c.value;
    }
    public void SetInternal(InternalCompartment c)
    {
      type = eHemorrhage_Type.External;
      compartment = c.value;
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
