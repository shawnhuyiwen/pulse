/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eSubstanceAdministration_Route : int
  {
    Intravenous = 0,
    Epidural,
    Intraosseous,
    Intraarterial,
    Intracardiac,
    Intracerebral,
    Intracerebroventricular,
    Intradermal,
    Intramuscular,
    Subcutaneous
  }

  public class SESubstanceBolus : SEPatientAction
  {
    protected eSubstanceAdministration_Route admin_route;
    protected SEScalarTime admin_duration;
    protected SEScalarMassPerVolume concentration;
    protected SEScalarVolume dose;
    protected string substance;

    public SESubstanceBolus()
    {
      this.admin_route = eSubstanceAdministration_Route.Intravenous;
      this.admin_duration = null;
      this.dose = null;
      this.concentration = null;
      this.substance = null;
    }

    public override void Clear()
    {
      base.Clear();
      substance = null;
      admin_route = eSubstanceAdministration_Route.Intravenous;
      if (admin_duration != null)
        admin_duration.Invalidate();
      if (dose != null)
        dose.Invalidate();
      if (concentration != null)
        concentration.Invalidate();
    }

    public override bool IsValid()
    {
      return HasDose() && HasConcentration() && HasSubstance();
    }

    public eSubstanceAdministration_Route GetAdminRoute()
    {
      return admin_route;
    }
    public void SetAdminRoute(eSubstanceAdministration_Route adminRoute)
    {
      this.admin_route = adminRoute;
    }

    public bool HasAdminDuration()
    {
      return admin_duration == null ? false : admin_duration.IsValid();
    }
    public SEScalarTime GetAdminDuration()
    {
      if (admin_duration == null)
        admin_duration = new SEScalarTime();
      return admin_duration;
    }

    public bool HasConcentration()
    {
      return concentration == null ? false : concentration.IsValid();
    }
    public SEScalarMassPerVolume GetConcentration()
    {
      if (concentration == null)
        concentration = new SEScalarMassPerVolume();
      return concentration;
    }

    public bool HasDose()
    {
      return dose == null ? false : dose.IsValid();
    }

    public SEScalarVolume GetDose()
    {
      if (dose == null)
        dose = new SEScalarVolume();
      return dose;
    }

    public string GetSubstance()
    {
      return substance;
    }
    public bool HasSubstance() { return !string.IsNullOrEmpty(substance); }
    public void SetSubstance(string s)
    {
      substance = s;
    }
  }
}
