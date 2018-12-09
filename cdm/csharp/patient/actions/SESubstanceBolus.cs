/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SESubstanceBolus : SEPatientAction
{
  public enum eAdministration : int
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

    protected eAdministration adminRoute;
    protected SEScalarMassPerVolume concentration;
    protected SEScalarVolume dose;
    protected string substance;

    public SESubstanceBolus()
    {
        this.adminRoute = eAdministration.Intravenous;
        this.dose = null;
        this.concentration = null;
        this.substance = null;
    }

    public override void Clear()
    {
        base.Clear();
        substance = null;
        adminRoute = eAdministration.Intravenous;
        if (dose != null)
            dose.Invalidate();
        if (concentration != null)
            concentration.Invalidate();
    }

    public override bool IsValid()
    {
        return HasDose() && HasConcentration() && HasSubstance();
    }

    public eAdministration GetAdminRoute()
    {
        return adminRoute;
    }
    public void SetAdminRoute(eAdministration adminRoute)
    {
        this.adminRoute = adminRoute;
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