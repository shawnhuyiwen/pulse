/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Nutrition)
CDM_BIND_DECL2(Meal)

class CDM_DECL PBPatientNutrition
{
public:

  static void Load(const cdm::NutritionData& src, SENutrition& dst);
  static cdm::NutritionData* Unload(const SENutrition& src);
  static void Serialize(const cdm::NutritionData& src, SENutrition& dst);
  static void Serialize(const SENutrition& src, cdm::NutritionData& dst);
  static void Copy(const SENutrition& src, SENutrition& dst);

  static bool SerializeToString(const SENutrition& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SENutrition& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SENutrition& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SENutrition& dst, SerializationMode m);

  static void Load(const cdm::MealData& src, SEMeal& dst);
  static cdm::MealData* Unload(const SEMeal& src);
  static void Serialize(const cdm::MealData& src, SEMeal& dst);
  static void Serialize(const SEMeal& src, cdm::MealData& dst);
  static void Copy(const SEMeal& src, SEMeal& dst);

  static bool SerializeToString(const SEMeal& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEMeal& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEMeal& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEMeal& dst, SerializationMode m);
};