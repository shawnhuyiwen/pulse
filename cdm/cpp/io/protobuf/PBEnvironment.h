/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(ActiveConditioning)
CDM_BIND_DECL2(AppliedTemperature)
CDM_BIND_DECL2(Environment)
CDM_BIND_DECL2(EnvironmentalConditions)

class CDM_DECL PBEnvironment
{
public:

  static void Load(const cdm::ActiveConditioningData& src, SEActiveConditioning& dst);
  static cdm::ActiveConditioningData* Unload(const SEActiveConditioning& src);
  static void Serialize(const cdm::ActiveConditioningData& src, SEActiveConditioning& dst);
  static void Serialize(const SEActiveConditioning& src, cdm::ActiveConditioningData& dst);
  static void Copy(const SEActiveConditioning& src, SEActiveConditioning& dst);

  static void Load(const cdm::AppliedTemperatureData& src, SEAppliedTemperature& dst);
  static cdm::AppliedTemperatureData* Unload(const SEAppliedTemperature& src);
  static void Serialize(const cdm::AppliedTemperatureData& src, SEAppliedTemperature& dst);
  static void Serialize(const SEAppliedTemperature& src, cdm::AppliedTemperatureData& dst);
  static void Copy(const SEAppliedTemperature& src, SEAppliedTemperature& dst);

  static void Load(const cdm::EnvironmentData& src, SEEnvironment& dst);
  static cdm::EnvironmentData* Unload(const SEEnvironment& src);
  static void Serialize(const cdm::EnvironmentData& src, SEEnvironment& dst);
  static void Serialize(const SEEnvironment& src, cdm::EnvironmentData& dst);
  static void Copy(const SEEnvironment& src, SEEnvironment& dst);

  static void Load(const cdm::EnvironmentalConditionsData& src, SEEnvironmentalConditions& dst);
  static cdm::EnvironmentalConditionsData* Unload(const SEEnvironmentalConditions& src);
  static void Serialize(const cdm::EnvironmentalConditionsData& src, SEEnvironmentalConditions& dst);
  static void Serialize(const SEEnvironmentalConditions& src, cdm::EnvironmentalConditionsData& dst);
  static void Copy(const SEEnvironmentalConditions& src, SEEnvironmentalConditions& dst);

  static bool SerializeToString(const SEEnvironmentalConditions& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEEnvironmentalConditions& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEEnvironmentalConditions& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEEnvironmentalConditions& dst, SerializationFormat m);
};