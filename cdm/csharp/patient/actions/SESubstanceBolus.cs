/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class eSubstanceAdministration
{
    private eSubstanceAdministration(string v) { Value = v; }
    protected string Value { get; }

    public static eSubstanceAdministration Intravenous { get { return new eSubstanceAdministration("Intravenous"); } }
    public static eSubstanceAdministration Epidural { get { return new eSubstanceAdministration("Epidural"); } }
    public static eSubstanceAdministration Intraosseous { get { return new eSubstanceAdministration("Intraosseous"); } }
    public static eSubstanceAdministration Intraarterial { get { return new eSubstanceAdministration("Intraarterial"); } }
    public static eSubstanceAdministration Intracardiac { get { return new eSubstanceAdministration("Intracardiac"); } }
    public static eSubstanceAdministration Intracerebral { get { return new eSubstanceAdministration("Intracerebral"); } }
    public static eSubstanceAdministration Intracerebroventricular { get { return new eSubstanceAdministration("Intracerebroventricular"); } }
    public static eSubstanceAdministration Intradermal { get { return new eSubstanceAdministration("Intradermal"); } }
    public static eSubstanceAdministration Intramuscular { get { return new eSubstanceAdministration("Intramuscular"); } }
    public static eSubstanceAdministration Subcutaneous { get { return new eSubstanceAdministration("Subcutaneous"); } }
}

public class SESubstanceBolus : SEPatientAction
{
  protected eSubstanceAdministration adminRoute;
  protected SEScalarMassPerVolume concentration;
  protected SEScalarVolume dose;
  protected string substance;
  
  public SESubstanceBolus(string substance)
  {
    this.adminRoute = null;
    this.dose = null;
    this.concentration = null;
    this.substance = substance;
  }
  
  public override void Reset()
  {
    base.Reset();
    adminRoute = null;
    if (dose != null)
      dose.Invalidate();
    if (concentration != null)
      concentration.Invalidate();
  }
  
  public override bool IsValid()
  {
    return HasAdminRoute() && HasDose() && HasConcentration() && HasSubstance();
  }

  public eSubstanceAdministration GetAdminRoute()
  {
    return adminRoute;
  }
  public void SetAdminRoute(eSubstanceAdministration adminRoute)
  {
    this.adminRoute = adminRoute;
  }
  public bool HasAdminRoute()
  {
    return adminRoute == null ? false : true;
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
  public bool HasSubstance() { return substance != null; }
}
