/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEAppliedTemperature
  {
    protected SEScalarTemperature temperature;
    protected SEScalarArea        surface_area;
    protected SEScalar0To1        surface_area_fraction;
    protected eSwitch             state;

    public SEAppliedTemperature()
    {
      temperature = null;
      surface_area = null;
      surface_area_fraction = null;
      state = eSwitch.Off;
    }

    public void Copy(SEAppliedTemperature other)
    {
      if (this == other)
        return;
      Clear();
      state = other.state;
      if (other.temperature != null)
        GetTemperature().Set(other.GetTemperature());
      else if (temperature != null)
        temperature.Invalidate();
      if (other.surface_area != null)
        GetSurfaceArea().Set(other.GetSurfaceArea());
      else if (surface_area != null)
        surface_area.Invalidate();
      if (other.surface_area_fraction != null)
        GetSurfaceAreaFraction().Set(other.GetSurfaceAreaFraction());
      else if (surface_area_fraction != null)
        surface_area_fraction.Invalidate();
    }

    public void Clear()
    {
      state = eSwitch.Off;
      if (temperature != null)
        temperature.Invalidate();
      if (surface_area != null)
        surface_area.Invalidate();
      if (surface_area_fraction != null)
        surface_area_fraction.Invalidate();
    }

    public eSwitch GetState()
    {
      return state;
    }
    public void SetState(eSwitch s)
    {
      this.state = (s == eSwitch.NullSwitch) ? eSwitch.Off : s;
    }

    public bool HasTemperature()
    {
      return temperature == null ? false : temperature.IsValid();
    }
    public SEScalarTemperature GetTemperature()
    {
      if (temperature == null)
        temperature = new SEScalarTemperature();
      return temperature;
    }

    public bool HasSurfaceArea()
    {
      return surface_area == null ? false : surface_area.IsValid();
    }
    public SEScalarArea GetSurfaceArea()
    {
      if (surface_area == null)
        surface_area = new SEScalarArea();
      return surface_area;
    }

    public bool HasSurfaceAreaFraction()
    {
      return surface_area_fraction == null ? false : surface_area_fraction.IsValid();
    }
    public SEScalar0To1 GetSurfaceAreaFraction()
    {
      if (surface_area_fraction == null)
        surface_area_fraction = new SEScalar0To1();
      return surface_area_fraction;
    }

    public new string ToString()
    {
      return "Thermal Application:"
          + "\n\tState: " + GetState()
          + "\n\tAppliedTemperature: " + (this.HasTemperature() ? this.GetTemperature().ToString() : "None")
          + "\n\tAppliedSurfaceArea: " + (this.HasSurfaceArea() ? this.GetSurfaceArea().ToString() : "None")
          + "\n\tAppliedSurfaceAreaFraction: " + (this.HasSurfaceAreaFraction() ? this.GetSurfaceAreaFraction().ToString() : "None");
    }

  }
}
