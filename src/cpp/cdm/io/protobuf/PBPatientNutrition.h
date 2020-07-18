/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Nutrition)
CDM_BIND_DECL2(Meal)

class CDM_DECL PBPatientNutrition
{
public:

  static void Load(const CDM_BIND::NutritionData& src, SENutrition& dst);
  static CDM_BIND::NutritionData* Unload(const SENutrition& src);
  static void Serialize(const CDM_BIND::NutritionData& src, SENutrition& dst);
  static void Serialize(const SENutrition& src, CDM_BIND::NutritionData& dst);
  static void Copy(const SENutrition& src, SENutrition& dst);

  static bool SerializeToString(const SENutrition& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SENutrition& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SENutrition& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SENutrition& dst);

  static void Load(const CDM_BIND::MealData& src, SEMeal& dst);
  static CDM_BIND::MealData* Unload(const SEMeal& src);
  static void Serialize(const CDM_BIND::MealData& src, SEMeal& dst);
  static void Serialize(const SEMeal& src, CDM_BIND::MealData& dst);
  static void Copy(const SEMeal& src, SEMeal& dst);

  static bool SerializeToString(const SEMeal& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEMeal& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEMeal& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEMeal& dst);
};