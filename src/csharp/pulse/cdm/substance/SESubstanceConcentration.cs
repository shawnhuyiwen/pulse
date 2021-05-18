/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESubstanceConcentration
  {
    protected SEScalarMassPerVolume concentration;
    protected string substance;

    public SESubstanceConcentration(string s)
    {
      substance = s ?? throw new System.ArgumentException("Must provide a valid substance");
    }

    public void Reset()
    {
      if (concentration != null)
        concentration.Invalidate();
    }

    //public static void load(SubstanceConcentrationData src, SESubstanceConcentration dst)
    //{
    //  dst.reset();
    //  if(src.hasConcentration())
    //    SEScalarMassPerVolume.load(src.getConcentration(), dst.getConcentration());
    //}
    //public static SubstanceConcentrationData unload(SESubstanceConcentration src)
    //{
    //  SubstanceConcentrationData.Builder dst = SubstanceConcentrationData.newBuilder();
    //  SESubstanceConcentration.unload(src,dst);
    //  return dst.build();
    //}
    //protected static void unload(SESubstanceConcentration src, SubstanceConcentrationData.Builder dst)
    //{
    //  dst.setName(src.substance.getName());
    //  if(src.hasConcentration())
    //    dst.setConcentration(SEScalarMassPerVolume.unload(src.concentration)); 
    //}

    public SEScalarMassPerVolume GetConcentration()
    {
      if (concentration == null)
        concentration = new SEScalarMassPerVolume();
      return concentration;
    }
    public bool HasConcentration() { return concentration == null ? false : concentration.IsValid(); }

    public bool HasSubstance() { return substance != null; }
    public string GetSubstance()
    {
      return substance;
    }

    public new string ToString()
    {
      if (!HasSubstance())
        return "";
      string str = "Concentration of " + substance + ": " + (HasConcentration() ? GetConcentration().ToString() : "None");
      return str;
    }
  }
}
