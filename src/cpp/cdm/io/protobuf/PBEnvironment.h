/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
CDM_BIND_DECL2(ActiveConditioning)
CDM_BIND_DECL2(AppliedTemperature)
CDM_BIND_DECL2(Environment)
CDM_BIND_DECL2(EnvironmentalConditions)

class CDM_DECL PBEnvironment
{
public:

  static void Load(const CDM_BIND::ActiveConditioningData& src, SEActiveConditioning& dst);
  static CDM_BIND::ActiveConditioningData* Unload(const SEActiveConditioning& src);
  static void Serialize(const CDM_BIND::ActiveConditioningData& src, SEActiveConditioning& dst);
  static void Serialize(const SEActiveConditioning& src, CDM_BIND::ActiveConditioningData& dst);
  static void Copy(const SEActiveConditioning& src, SEActiveConditioning& dst);

  static void Load(const CDM_BIND::AppliedTemperatureData& src, SEAppliedTemperature& dst);
  static CDM_BIND::AppliedTemperatureData* Unload(const SEAppliedTemperature& src);
  static void Serialize(const CDM_BIND::AppliedTemperatureData& src, SEAppliedTemperature& dst);
  static void Serialize(const SEAppliedTemperature& src, CDM_BIND::AppliedTemperatureData& dst);
  static void Copy(const SEAppliedTemperature& src, SEAppliedTemperature& dst);

  static void Load(const CDM_BIND::EnvironmentData& src, SEEnvironment& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::EnvironmentData* Unload(const SEEnvironment& src);
  static void Serialize(const CDM_BIND::EnvironmentData& src, SEEnvironment& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEEnvironment& src, CDM_BIND::EnvironmentData& dst);
  static void Copy(const SEEnvironment& src, SEEnvironment& dst, const SESubstanceManager& subMgr);

  static void Load(const CDM_BIND::EnvironmentalConditionsData& src, SEEnvironmentalConditions& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::EnvironmentalConditionsData* Unload(const SEEnvironmentalConditions& src);
  static void Serialize(const CDM_BIND::EnvironmentalConditionsData& src, SEEnvironmentalConditions& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEEnvironmentalConditions& src, CDM_BIND::EnvironmentalConditionsData& dst);
  static void Copy(const SEEnvironmentalConditions& src, SEEnvironmentalConditions& dst, const SESubstanceManager& subMgr);

  static bool SerializeToString(const SEEnvironmentalConditions& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEEnvironmentalConditions& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEEnvironmentalConditions& dst, SerializationFormat m, const SESubstanceManager& subMgr);
  static bool SerializeFromFile(const std::string& filename, SEEnvironmentalConditions& dst, const SESubstanceManager& subMgr);
};