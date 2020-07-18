/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SENutrition
  {
    protected SEScalarMass carbohydrate;
    protected SEScalarMassPerTime carbohydrate_digestion_rate;
    protected SEScalarMass fat;
    protected SEScalarMassPerTime fat_digestion_rate;
    protected SEScalarMass protein;
    protected SEScalarMassPerTime protein_digestion_rate;
    protected SEScalarMass calcium;
    protected SEScalarMass sodium;
    protected SEScalarVolume water;

    public SENutrition()
    {
      carbohydrate = null;
      carbohydrate_digestion_rate = null;
      fat = null;
      fat_digestion_rate = null;
      protein = null;
      protein_digestion_rate = null;
      calcium = null;
      sodium = null;
      water = null;
    }

    public void Clear()
    {
      if (carbohydrate != null) { carbohydrate.Invalidate(); }
      if (carbohydrate_digestion_rate != null) { carbohydrate_digestion_rate.Invalidate(); }
      if (fat != null) { fat.Invalidate(); }
      if (fat_digestion_rate != null) { fat_digestion_rate.Invalidate(); }
      if (protein != null) { protein.Invalidate(); }
      if (protein_digestion_rate != null) { protein_digestion_rate.Invalidate(); }
      if (calcium != null) { calcium.Invalidate(); }
      if (sodium != null) { sodium.Invalidate(); }
      if (water != null) { water.Invalidate(); }
    }

    public void Copy(SENutrition src)
    {
      Clear();
      if (src.HasCarbohydrate()) { GetCarbohydrate().Set(src.carbohydrate); }
      if (src.HasCarbohydrateDigestionRate()) { GetCarbohydrateDigestionRate().Set(src.carbohydrate_digestion_rate); }
      if (src.HasFat()) { GetFat().Set(src.fat); }
      if (src.HasFatDigestionRate()) { GetFatDigestionRate().Set(src.fat_digestion_rate); }
      if (src.HasProtein()) { GetProtein().Set(src.protein); }
      if (src.HasProteinDigestionRate()) { GetProteinDigestionRate().Set(src.protein_digestion_rate); }
      if (src.HasCalcium()) { GetCalcium().Set(src.calcium); }
      if (src.HasSodium()) { GetSodium().Set(src.sodium); }
      if (src.HasWater()) { GetWater().Set(src.water); }
    }

    public bool SerializeFromFile(string filename)
    {
      try
      {
        string json = System.IO.File.ReadAllText(filename);
        return SerializeFromString(json, SerializationFormat.JSON);
      }
      catch (System.Exception ex)
      {
        System.Console.WriteLine("Error Reading Nutrition File! " + ex.ToString());
        return false;
      }
    }
    public bool SerializeFromString(string src, SerializationFormat fmt)
    {
      return PBNutrition.SerializeFromString(src, this);
    }

    public bool SerializeToFile(string filename)
    {
      string json = SerializeToString(SerializationFormat.JSON);
      if (json == null || json.Length == 0)
        return false;
      try
      {
        System.IO.File.WriteAllText(filename, json);
      }
      catch (System.Exception ex)
      {
        System.Console.WriteLine("Error Writing Patient File! " + ex.ToString());
        return false;
      }
      return true;
    }
    public string SerializeToString(SerializationFormat format)
    {
      return PBNutrition.SerializeToString(this);
    }

    public bool HasCarbohydrate()
    {
      return carbohydrate == null ? false : carbohydrate.IsValid();
    }
    public SEScalarMass GetCarbohydrate()
    {
      if (carbohydrate == null)
        carbohydrate = new SEScalarMass();
      return carbohydrate;
    }

    public bool HasCarbohydrateDigestionRate()
    {
      return carbohydrate_digestion_rate == null ? false : carbohydrate_digestion_rate.IsValid();
    }
    public SEScalarMassPerTime GetCarbohydrateDigestionRate()
    {
      if (carbohydrate_digestion_rate == null)
        carbohydrate_digestion_rate = new SEScalarMassPerTime();
      return carbohydrate_digestion_rate;
    }

    public bool HasFat()
    {
      return fat == null ? false : fat.IsValid();
    }
    public SEScalarMass GetFat()
    {
      if (fat == null)
        fat = new SEScalarMass();
      return fat;
    }

    public bool HasFatDigestionRate()
    {
      return fat_digestion_rate == null ? false : fat_digestion_rate.IsValid();
    }
    public SEScalarMassPerTime GetFatDigestionRate()
    {
      if (fat_digestion_rate == null)
        fat_digestion_rate = new SEScalarMassPerTime();
      return fat_digestion_rate;
    }

    public bool HasProtein()
    {
      return protein == null ? false : protein.IsValid();
    }
    public SEScalarMass GetProtein()
    {
      if (protein == null)
        protein = new SEScalarMass();
      return protein;
    }

    public bool HasProteinDigestionRate()
    {
      return protein_digestion_rate == null ? false : protein_digestion_rate.IsValid();
    }
    public SEScalarMassPerTime GetProteinDigestionRate()
    {
      if (protein_digestion_rate == null)
        protein_digestion_rate = new SEScalarMassPerTime();
      return protein_digestion_rate;
    }

    public bool HasCalcium()
    {
      return calcium == null ? false : calcium.IsValid();
    }
    public SEScalarMass GetCalcium()
    {
      if (calcium == null)
        calcium = new SEScalarMass();
      return calcium;
    }

    public bool HasSodium()
    {
      return sodium == null ? false : sodium.IsValid();
    }
    public SEScalarMass GetSodium()
    {
      if (sodium == null)
        sodium = new SEScalarMass();
      return sodium;
    }

    public bool HasWater()
    {
      return water == null ? false : water.IsValid();
    }
    public SEScalarVolume GetWater()
    {
      if (water == null)
        water = new SEScalarVolume();
      return water;
    }
  }
}
