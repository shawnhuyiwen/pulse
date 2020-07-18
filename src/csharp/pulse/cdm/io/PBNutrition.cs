/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBNutrition
  {
    public static void Load(pulse.cdm.bind.NutritionData src, SENutrition dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.NutritionData src, SENutrition dst)
    {
      dst.Clear();
      if (src.Carbohydrate != null)
        PBProperty.Load(src.Carbohydrate, dst.GetCarbohydrate());
      if (src.CarbohydrateDigestionRate != null)
        PBProperty.Load(src.CarbohydrateDigestionRate, dst.GetCarbohydrateDigestionRate());
      if (src.Fat != null)
        PBProperty.Load(src.Fat, dst.GetFat());
      if (src.FatDigestionRate != null)
        PBProperty.Load(src.FatDigestionRate, dst.GetFatDigestionRate());
      if (src.Protein != null)
        PBProperty.Load(src.Protein, dst.GetProtein());
      if (src.ProteinDigestionRate != null)
        PBProperty.Load(src.ProteinDigestionRate, dst.GetProteinDigestionRate());
      if (src.Calcium != null)
        PBProperty.Load(src.Calcium, dst.GetCalcium());
      if (src.Sodium != null)
        PBProperty.Load(src.Sodium, dst.GetSodium());
      if (src.Water != null)
        PBProperty.Load(src.Water, dst.GetWater());
    }
    public static bool SerializeFromString(string src, SENutrition dst)
    {
      try
      {
        pulse.cdm.bind.NutritionData data = JsonParser.Default.Parse<pulse.cdm.bind.NutritionData>(src);
        PBNutrition.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.NutritionData Unload(SENutrition src)
    {
      pulse.cdm.bind.NutritionData dst = new pulse.cdm.bind.NutritionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SENutrition src, pulse.cdm.bind.NutritionData dst)
    {
      if (src.HasCarbohydrate())
        dst.Carbohydrate = PBProperty.Unload(src.GetCarbohydrate());
      if (src.HasCarbohydrateDigestionRate())
        dst.CarbohydrateDigestionRate = PBProperty.Unload(src.GetCarbohydrateDigestionRate());
      if (src.HasFat())
        dst.Fat = PBProperty.Unload(src.GetFat());
      if (src.HasFatDigestionRate())
        dst.FatDigestionRate = PBProperty.Unload(src.GetFatDigestionRate());
      if (src.HasProtein())
        dst.Protein = PBProperty.Unload(src.GetProtein());
      if (src.HasProteinDigestionRate())
        dst.ProteinDigestionRate = PBProperty.Unload(src.GetProteinDigestionRate());
      if (src.HasCalcium())
        dst.Calcium = PBProperty.Unload(src.GetCalcium());
      if (src.HasSodium())
        dst.Sodium = PBProperty.Unload(src.GetSodium());
      if (src.HasWater())
        dst.Water = PBProperty.Unload(src.GetWater());
    }
    public static string SerializeToString(SENutrition src)
    {
      var pb = PBNutrition.Unload(src);
      return pb.ToString();
    }
  }
}
