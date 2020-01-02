/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class MassPerAmountUnit
{
  private MassPerAmountUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static MassPerAmountUnit g_Per_ct { get { return new MassPerAmountUnit("g/ct"); } }
  public static MassPerAmountUnit g_Per_mol { get { return new MassPerAmountUnit("g/mol"); } }
  public static MassPerAmountUnit g_Per_mmol { get { return new MassPerAmountUnit("g/mmol"); } }
  public static MassPerAmountUnit g_Per_umol { get { return new MassPerAmountUnit("g/umol"); } }
  public static MassPerAmountUnit mg_Per_mol { get { return new MassPerAmountUnit("mg/mol"); } }
  public static MassPerAmountUnit mg_Per_mmol { get { return new MassPerAmountUnit("mg/mmol"); } }
  public static MassPerAmountUnit kg_Per_mol { get { return new MassPerAmountUnit("kg/mol"); } }
  public static MassPerAmountUnit ug_Per_mmol { get { return new MassPerAmountUnit("ug/mmol"); } }
  public static MassPerAmountUnit ug_Per_mol { get { return new MassPerAmountUnit("ug/mol"); } }
  public static MassPerAmountUnit ug_Per_ct { get { return new MassPerAmountUnit("ug/ct"); } }
  public static MassPerAmountUnit pg_Per_ct { get { return new MassPerAmountUnit("pg/ct"); } }

  public static MassPerAmountUnit FromString(string u)
  {
    if (u == MassPerAmountUnit.g_Per_ct.Value)
      return MassPerAmountUnit.g_Per_ct;
    if (u == MassPerAmountUnit.g_Per_mol.Value)
      return MassPerAmountUnit.g_Per_mol;
    if (u == MassPerAmountUnit.g_Per_mmol.Value)
      return MassPerAmountUnit.g_Per_mmol;
    if (u == MassPerAmountUnit.g_Per_umol.Value)
      return MassPerAmountUnit.g_Per_umol;
    if (u == MassPerAmountUnit.mg_Per_mol.Value)
      return MassPerAmountUnit.mg_Per_mol;
    if (u == MassPerAmountUnit.mg_Per_mmol.Value)
      return MassPerAmountUnit.mg_Per_mmol;
    if (u == MassPerAmountUnit.kg_Per_mol.Value)
      return MassPerAmountUnit.kg_Per_mol;
    if (u == MassPerAmountUnit.ug_Per_mmol.Value)
      return MassPerAmountUnit.ug_Per_mmol;
    if (u == MassPerAmountUnit.ug_Per_mol.Value)
      return MassPerAmountUnit.ug_Per_mol;
    if (u == MassPerAmountUnit.ug_Per_ct.Value)
      return MassPerAmountUnit.ug_Per_ct;
    if (u == MassPerAmountUnit.pg_Per_ct.Value)
      return MassPerAmountUnit.pg_Per_ct;

    throw new System.NotSupportedException(u + " is an unsupported MassPerAmountUnit");
  }
}

public class SEScalarMassPerAmount : SEScalar
{
  protected MassPerAmountUnit unit;

  public SEScalarMassPerAmount() : base()
  {

  }

  public SEScalarMassPerAmount(SEScalarMassPerAmount from)
  {
    Set(from);
  }

  public SEScalarMassPerAmount(double value, MassPerAmountUnit unit)
  {
    SetValue(value, unit);
  }

  public new void SetValue(double value)
  {
    throw new System.NotSupportedException("You must provide a unit");
  }

  public void Set(SEScalarMassPerAmount from)
  {
    if (from == null)
    {
      Invalidate();
      return;
    }
    SetValue(from.value, from.unit);
  }

  public void SetValue(double value, MassPerAmountUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(MassPerAmountUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public MassPerAmountUnit GetUnit()
  {
    return unit;
  }
}
