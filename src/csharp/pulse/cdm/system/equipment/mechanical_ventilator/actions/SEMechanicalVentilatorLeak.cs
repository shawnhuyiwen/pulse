/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEMechanicalVentilatorLeak : SEMechanicalVentilatorAction
  {
    protected SEScalar0To1 severity;

    public SEMechanicalVentilatorLeak()
    {
      severity = null;
    }

    public SEMechanicalVentilatorLeak(SEMechanicalVentilatorLeak other)
    {
      Copy(other);
    }

    public void Copy(SEMechanicalVentilatorLeak other)
    {
      base.Copy(other);
      if (other.HasSeverity())
        GetSeverity().Set(other.GetSeverity());
    }

    public override void Clear()
    {
      base.Clear();
      if (severity != null)
        severity.Invalidate();
    }

    public override bool IsValid()
    {
      return HasSeverity();
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

    public override string ToString()
    {
      string str = "Mechanical Ventilator Leak";
      str +="\n\tSeverity: " + (HasSeverity() ? GetSeverity().ToString() : "Not Provided");


      return str;
    }
  }
}
