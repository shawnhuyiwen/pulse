/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/PatientNutrition.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBPatientNutrition.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "patient/SEMeal.h"
#include "utils/FileUtils.h"


void PBPatientNutrition::Load(const CDM_BIND::NutritionData& src, SENutrition& dst)
{
  dst.Clear();
  PBPatientNutrition::Serialize(src, dst);
}
void PBPatientNutrition::Serialize(const CDM_BIND::NutritionData& src, SENutrition& dst)
{
  if (src.has_carbohydrate())
    PBProperty::Load(src.carbohydrate(), dst.GetCarbohydrate());
  if (src.has_carbohydratedigestionrate())
    PBProperty::Load(src.carbohydratedigestionrate(), dst.GetCarbohydrateDigestionRate());
  if (src.has_fat())
    PBProperty::Load(src.fat(), dst.GetFat());
  if (src.has_fatdigestionrate())
    PBProperty::Load(src.fatdigestionrate(), dst.GetFatDigestionRate());
  if (src.has_protein())
    PBProperty::Load(src.protein(), dst.GetProtein());
  if (src.has_proteindigestionrate())
    PBProperty::Load(src.proteindigestionrate(), dst.GetProteinDigestionRate());
  if (src.has_calcium())
    PBProperty::Load(src.calcium(), dst.GetCalcium());
  if (src.has_sodium())
    PBProperty::Load(src.sodium(), dst.GetSodium());
  if (src.has_water())
    PBProperty::Load(src.water(), dst.GetWater());
}

CDM_BIND::NutritionData* PBPatientNutrition::Unload(const SENutrition& src)
{
  CDM_BIND::NutritionData* dst = new CDM_BIND::NutritionData();
  PBPatientNutrition::Serialize(src, *dst);
  return dst;
}
void PBPatientNutrition::Serialize(const SENutrition& src, CDM_BIND::NutritionData& dst)
{
  if (src.HasCarbohydrate())
    dst.set_allocated_carbohydrate(PBProperty::Unload(*src.m_Carbohydrate));
  if (src.HasCarbohydrateDigestionRate())
    dst.set_allocated_carbohydratedigestionrate(PBProperty::Unload(*src.m_CarbohydrateDigestionRate));
  if (src.HasFat())
    dst.set_allocated_fat(PBProperty::Unload(*src.m_Fat));
  if (src.HasFatDigestionRate())
    dst.set_allocated_fatdigestionrate(PBProperty::Unload(*src.m_FatDigestionRate));
  if (src.HasProtein())
    dst.set_allocated_protein(PBProperty::Unload(*src.m_Protein));
  if (src.HasProteinDigestionRate())
    dst.set_allocated_proteindigestionrate(PBProperty::Unload(*src.m_ProteinDigestionRate));
  if (src.HasSodium())
    dst.set_allocated_sodium(PBProperty::Unload(*src.m_Sodium));
  if (src.HasCalcium())
    dst.set_allocated_calcium(PBProperty::Unload(*src.m_Calcium));
  if (src.HasWater())
    dst.set_allocated_water(PBProperty::Unload(*src.m_Water));
}
void PBPatientNutrition::Copy(const SENutrition& src, SENutrition& dst)
{
  dst.Clear();
  CDM_BIND::NutritionData data;
  PBPatientNutrition::Serialize(src, data);
  PBPatientNutrition::Serialize(data, dst);
}

bool PBPatientNutrition::SerializeToString(const SENutrition& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::NutritionData data;
  PBPatientNutrition::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientNutrition::SerializeToFile(const SENutrition& src, const std::string& filename)
{
  CDM_BIND::NutritionData data;
  PBPatientNutrition::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBPatientNutrition::SerializeFromString(const std::string& src, SENutrition& dst, SerializationFormat m)
{
  CDM_BIND::NutritionData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBPatientNutrition::Load(data, dst);
  return true;
}
bool PBPatientNutrition::SerializeFromFile(const std::string& filename, SENutrition& dst)
{
  CDM_BIND::NutritionData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBPatientNutrition::Load(data, dst);
  return true;
}

void PBPatientNutrition::Load(const CDM_BIND::MealData& src, SEMeal& dst)
{
  dst.Clear();
  PBPatientNutrition::Serialize(src, dst);
}
void PBPatientNutrition::Serialize(const CDM_BIND::MealData& src, SEMeal& dst)
{
  PBPatientNutrition::Serialize(src.nutrition(), dst);
  if (src.has_elapsedtime())
    PBProperty::Load(src.elapsedtime(), dst.GetElapsedTime());
}
CDM_BIND::MealData* PBPatientNutrition::Unload(const SEMeal& src)
{
  CDM_BIND::MealData* dst = new CDM_BIND::MealData();
  PBPatientNutrition::Serialize(src, *dst);
  return dst;
}
void PBPatientNutrition::Serialize(const SEMeal& src, CDM_BIND::MealData& dst)
{
  PBPatientNutrition::Serialize(src, *dst.mutable_nutrition());
  if (src.HasElapsedTime())
    dst.set_allocated_elapsedtime(PBProperty::Unload(*src.m_ElapsedTime));
}
void PBPatientNutrition::Copy(const SEMeal& src, SEMeal& dst)
{
  dst.Clear();
  CDM_BIND::MealData data;
  PBPatientNutrition::Serialize(src, data);
  PBPatientNutrition::Serialize(data, dst);
}

bool PBPatientNutrition::SerializeToString(const SEMeal& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::MealData data;
  PBPatientNutrition::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientNutrition::SerializeToFile(const SEMeal& src, const std::string& filename)
{
  CDM_BIND::MealData data;
  PBPatientNutrition::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBPatientNutrition::SerializeFromString(const std::string& src, SEMeal& dst, SerializationFormat m)
{
  CDM_BIND::MealData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBPatientNutrition::Load(data, dst);
  return true;
}
bool PBPatientNutrition::SerializeFromFile(const std::string& filename, SEMeal& dst)
{
  CDM_BIND::MealData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBPatientNutrition::Load(data, dst);
  return true;
}
