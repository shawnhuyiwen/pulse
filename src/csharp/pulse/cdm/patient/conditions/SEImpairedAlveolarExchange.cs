/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEImpairedAlveolarExchange : SEPatientCondition
  {
    protected SEScalarArea impaired_surface_area;
    protected SEScalar0To1 impaired_fraction;
    protected SEScalar0To1 severity;

    public SEImpairedAlveolarExchange()
    {
      impaired_surface_area = null;
      impaired_fraction = null;
      severity = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (impaired_surface_area != null)
        impaired_surface_area.Invalidate();
      if (impaired_fraction != null)
        impaired_fraction.Invalidate();
      if (severity != null)
        severity.Invalidate();
    }

    public override bool IsValid()
    {
      return HasImpairedSurfaceArea() || HasImpairedFraction() || HasSeverity();
    }

    public bool HasImpairedSurfaceArea()
    {
      return impaired_surface_area == null ? false : impaired_surface_area.IsValid();
    }
    public SEScalarArea GetImpairedSurfaceArea()
    {
      if (impaired_surface_area == null)
        impaired_surface_area = new SEScalarArea();
      return impaired_surface_area;
    }

    public bool HasImpairedFraction()
    {
      return impaired_fraction == null ? false : impaired_fraction.IsValid();
    }
    public SEScalar0To1 GetImpairedFraction()
    {
      if (impaired_fraction == null)
        impaired_fraction = new SEScalar0To1();
      return impaired_fraction;
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
