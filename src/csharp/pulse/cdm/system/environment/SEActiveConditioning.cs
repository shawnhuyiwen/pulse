/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;

namespace Pulse.CDM
{
  public class SEActiveConditioning
  {
    protected SEScalarPower power;
    protected SEScalarArea  surface_area;
    protected SEScalar0To1  surface_area_fraction;

    public SEActiveConditioning()
    {
      power = null;
      surface_area = null;
      surface_area_fraction = null;
    }

    public void Copy(SEActiveConditioning other)
    {
      if (this == other)
        return;
      Clear();
      if (other.power != null)
        GetPower().Set(other.GetPower());
      else if (power != null)
        power.Invalidate();
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
      if (power != null)
        power.Invalidate();
      if (surface_area != null)
        surface_area.Invalidate();
      if (surface_area_fraction != null)
        surface_area_fraction.Invalidate();
    }

    public Boolean HasPower()
    {
      return power == null ? false : power.IsValid();
    }
    public SEScalarPower GetPower()
    {
      if (power == null)
        power = new SEScalarPower();
      return power;
    }

    public Boolean HasSurfaceArea()
    {
      return surface_area == null ? false : surface_area.IsValid();
    }
    public SEScalarArea GetSurfaceArea()
    {
      if (surface_area == null)
        surface_area = new SEScalarArea();
      return surface_area;
    }

    public Boolean HasSurfaceAreaFraction()
    {
      return surface_area_fraction == null ? false : surface_area_fraction.IsValid();
    }
    public SEScalar0To1 GetSurfaceAreaFraction()
    {
      if (surface_area_fraction == null)
        surface_area_fraction = new SEScalar0To1();
      return surface_area_fraction;
    }

    public override string ToString()
    {
      return "Active Conditioning:"
          + "\n\tPower: " + (this.HasPower() ? this.GetPower().ToString() : "None")
          + "\n\tSurfaceArea: " + (this.HasSurfaceArea() ? this.GetSurfaceArea().ToString() : "None")
          + "\n\tSurfaceAreaFraction: " + (this.HasSurfaceAreaFraction() ? this.GetSurfaceAreaFraction().ToString() : "None");
    }
  }
}
