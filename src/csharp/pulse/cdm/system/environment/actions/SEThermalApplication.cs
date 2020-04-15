/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEThermalApplication : SEEnvironmentAction
  {
    protected SEActiveConditioning active_heating;
    protected SEActiveConditioning active_cooling;
    protected SEAppliedTemperature applied_temperature;
    protected bool                 clearContents;

    public SEThermalApplication()
    {
      clearContents = true;
      active_heating = null;
      active_cooling = null;
      applied_temperature = null;
    }

    public override void Clear()
    {
      base.Clear();
      clearContents = true;
      if (active_heating != null)
        active_heating.Clear();
      if (active_cooling != null)
        active_cooling.Clear();
      if (applied_temperature != null)
        applied_temperature.Clear();
    }

    public void Copy(SEThermalApplication other)
    {
      if (this == other)
        return;

      Clear();
      clearContents = other.clearContents;
      if (other.active_heating != null)
        GetActiveHeating().Copy(other.GetActiveHeating());
      else if (active_heating != null)
        active_heating.Clear();
      if (other.active_cooling != null)
        GetActiveCooling().Copy(other.GetActiveCooling());
      else if (active_cooling != null)
        active_cooling.Clear();
      if (other.applied_temperature != null)
        GetAppliedTemperature().Copy(other.GetAppliedTemperature());
      else if (applied_temperature != null)
        applied_temperature.Clear();
    }

    public override bool IsValid()
    {
      return HasActiveCooling() || HasActiveHeating() || HasAppliedTemperature();
    }

    public bool SetState(bool t)
    {
      clearContents = t;
      return clearContents;
    }
    public bool GetClearContents() { return clearContents; }

    public bool HasActiveHeating()
    {
      return active_heating != null;
    }
    public SEActiveConditioning GetActiveHeating()
    {
      if (active_heating == null)
        active_heating = new SEActiveConditioning();
      return active_heating;
    }

    public bool HasActiveCooling()
    {
      return active_cooling != null;
    }
    public SEActiveConditioning GetActiveCooling()
    {
      if (active_cooling == null)
        active_cooling = new SEActiveConditioning();
      return active_cooling;
    }

    public bool HasAppliedTemperature()
    {
      return applied_temperature != null;
    }
    public SEAppliedTemperature GetAppliedTemperature()
    {
      if (applied_temperature == null)
        applied_temperature = new SEAppliedTemperature();
      return applied_temperature;
    }

    public new string ToString()
    {
      return "Thermal Application:"
          + (HasActiveHeating() ? GetActiveHeating().ToString() : "No Heating")
          + (HasActiveCooling() ? GetActiveCooling().ToString() : "No Cooling")
          + (HasAppliedTemperature() ? GetAppliedTemperature().ToString() : "No Applied Temperature");
    }
  }
}


