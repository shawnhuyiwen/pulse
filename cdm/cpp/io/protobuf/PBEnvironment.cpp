/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBEnvironment.h"
#include "io/protobuf/PBSubstance.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cdm/Environment.pb.h"
#include "system/environment/SEEnvironment.h"
#include "system/environment/SEActiveConditioning.h"
#include "system/environment/SEAppliedTemperature.h"
#include "system/environment/SEEnvironmentalConditions.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarPower.h"
#include "utils/FileUtils.h"

void PBEnvironment::Load(const cdm::EnvironmentData& src, SEEnvironment& dst)
{
  PBEnvironment::Serialize(src, dst);
  dst.StateChange();
}
void PBEnvironment::Serialize(const cdm::EnvironmentData& src, SEEnvironment& dst)
{
  if (src.has_convectiveheatloss())
    PBProperty::Load(src.convectiveheatloss(), dst.GetConvectiveHeatLoss());
  if (src.has_convectiveheattranfercoefficient())
    PBProperty::Load(src.convectiveheattranfercoefficient(), dst.GetConvectiveHeatTranferCoefficient());
  if (src.has_evaporativeheatloss())
    PBProperty::Load(src.evaporativeheatloss(), dst.GetEvaporativeHeatLoss());
  if (src.has_evaporativeheattranfercoefficient())
    PBProperty::Load(src.evaporativeheattranfercoefficient(), dst.GetEvaporativeHeatTranferCoefficient());
  if (src.has_radiativeheatloss())
    PBProperty::Load(src.radiativeheatloss(), dst.GetRadiativeHeatLoss());
  if (src.has_radiativeheattranfercoefficient())
    PBProperty::Load(src.radiativeheattranfercoefficient(), dst.GetRadiativeHeatTranferCoefficient());
  if (src.has_respirationheatloss())
    PBProperty::Load(src.respirationheatloss(), dst.GetRespirationHeatLoss());
  if (src.has_skinheatloss())
    PBProperty::Load(src.skinheatloss(), dst.GetSkinHeatLoss());

  if (src.has_conditions())
    PBEnvironment::Load(src.conditions(), dst.GetConditions());
  if (src.has_activeheating())
    PBEnvironment::Load(src.activeheating(), dst.GetActiveHeating());
  if (src.has_activecooling())
    PBEnvironment::Load(src.activecooling(), dst.GetActiveCooling());
  if (src.has_appliedtemperature())
    PBEnvironment::Load(src.appliedtemperature(), dst.GetAppliedTemperature());
}
cdm::EnvironmentData* PBEnvironment::Unload(const SEEnvironment& src)
{
  cdm::EnvironmentData* dst = new cdm::EnvironmentData();
  PBEnvironment::Serialize(src, *dst);
  return dst;
}
void PBEnvironment::Serialize(const SEEnvironment& src, cdm::EnvironmentData& dst)
{
  if (src.HasConvectiveHeatLoss())
    dst.set_allocated_convectiveheatloss(PBProperty::Unload(*src.m_ConvectiveHeatLoss));
  if (src.HasConvectiveHeatTranferCoefficient())
    dst.set_allocated_convectiveheattranfercoefficient(PBProperty::Unload(*src.m_ConvectiveHeatTranferCoefficient));
  if (src.HasEvaporativeHeatLoss())
    dst.set_allocated_evaporativeheatloss(PBProperty::Unload(*src.m_EvaporativeHeatLoss));
  if (src.HasEvaporativeHeatTranferCoefficient())
    dst.set_allocated_evaporativeheattranfercoefficient(PBProperty::Unload(*src.m_EvaporativeHeatTranferCoefficient));
  if (src.HasRadiativeHeatLoss())
    dst.set_allocated_radiativeheatloss(PBProperty::Unload(*src.m_RadiativeHeatLoss));
  if (src.HasRadiativeHeatTranferCoefficient())
    dst.set_allocated_radiativeheattranfercoefficient(PBProperty::Unload(*src.m_RadiativeHeatTranferCoefficient));
  if (src.HasRespirationHeatLoss())
    dst.set_allocated_respirationheatloss(PBProperty::Unload(*src.m_RespirationHeatLoss));
  if (src.HasSkinHeatLoss())
    dst.set_allocated_skinheatloss(PBProperty::Unload(*src.m_SkinHeatLoss));

  if (src.HasActiveHeating() && src.m_ActiveHeating->GetPower().IsPositive())
    dst.set_allocated_activeheating(PBEnvironment::Unload(*src.m_ActiveHeating));
  if (src.HasActiveCooling() && src.m_ActiveCooling->GetPower().IsPositive())
    dst.set_allocated_activecooling(PBEnvironment::Unload(*src.m_ActiveCooling));
  if (src.HasAppliedTemperature())
    dst.set_allocated_appliedtemperature(PBEnvironment::Unload(*src.m_AppliedTemperature));
  if (src.HasConditions())
    dst.set_allocated_conditions(PBEnvironment::Unload(*src.m_Conditions));
}
void PBEnvironment::Copy(const SEEnvironment& src, SEEnvironment& dst)
{
  cdm::EnvironmentData data;
  PBEnvironment::Serialize(src, data);
  PBEnvironment::Serialize(data, dst);
}

void PBEnvironment::Load(const cdm::EnvironmentalConditionsData& src, SEEnvironmentalConditions& dst)
{
  PBEnvironment::Serialize(src, dst);
}
void PBEnvironment::Serialize(const cdm::EnvironmentalConditionsData& src, SEEnvironmentalConditions& dst)
{
  dst.Clear();
  dst.SetSurroundingType((eSurroundingType)src.surroundingtype());
  if (src.has_airdensity())
    PBProperty::Load(src.airdensity(), dst.GetAirDensity());
  if (src.has_airvelocity())
    PBProperty::Load(src.airvelocity(), dst.GetAirVelocity());
  if (src.has_ambienttemperature())
    PBProperty::Load(src.ambienttemperature(), dst.GetAmbientTemperature());
  if (src.has_atmosphericpressure())
    PBProperty::Load(src.atmosphericpressure(), dst.GetAtmosphericPressure());
  if (src.has_clothingresistance())
    PBProperty::Load(src.clothingresistance(), dst.GetClothingResistance());
  if (src.has_emissivity())
    PBProperty::Load(src.emissivity(), dst.GetEmissivity());
  if (src.has_meanradianttemperature())
    PBProperty::Load(src.meanradianttemperature(), dst.GetMeanRadiantTemperature());
  if (src.has_relativehumidity())
    PBProperty::Load(src.relativehumidity(), dst.GetRelativeHumidity());
  if (src.has_respirationambienttemperature())
    PBProperty::Load(src.respirationambienttemperature(), dst.GetRespirationAmbientTemperature());

  SESubstance* sub;
  for (int i = 0; i < src.ambientgas_size(); i++)
  {
    const cdm::SubstanceFractionData& sfData = src.ambientgas()[i];
    sub = dst.m_Substances.GetSubstance(sfData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring an environmental conditions ambient gas that was not found : " + sfData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Gas)
    {
      dst.Error("Ignoring an environmental conditions ambient gas that is not a gas : " + sfData.name());
      continue;
    }
    PBSubstance::Load(sfData, dst.GetAmbientGas(*sub));
  }

  for (int i = 0; i < src.ambientaerosol_size(); i++)
  {
    const cdm::SubstanceConcentrationData& scData = src.ambientaerosol()[i];
    sub = dst.m_Substances.GetSubstance(scData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring an environmental conditions ambient aerosol that was not found : " + scData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Liquid && sub->GetState() != eSubstance_State::Solid)
    {
      dst.Error("Ignoring an environmental conditions ambient aerosol that is not a gas : " + scData.name());
      continue;
    }
    PBSubstance::Load(scData, dst.GetAmbientAerosol(*sub));
  }
}
cdm::EnvironmentalConditionsData* PBEnvironment::Unload(const SEEnvironmentalConditions& src)
{
  cdm::EnvironmentalConditionsData* dst = new cdm::EnvironmentalConditionsData();
  PBEnvironment::Serialize(src, *dst);
  return dst;
}
void PBEnvironment::Serialize(const SEEnvironmentalConditions& src, cdm::EnvironmentalConditionsData& dst)
{
  dst.set_surroundingtype((cdm::eEnvironment_SurroundingType)src.m_SurroundingType);
  if (src.HasAirDensity())
    dst.set_allocated_airdensity(PBProperty::Unload(*src.m_AirDensity));
  if (src.HasAirVelocity())
    dst.set_allocated_airvelocity(PBProperty::Unload(*src.m_AirVelocity));
  if (src.HasAmbientTemperature())
    dst.set_allocated_ambienttemperature(PBProperty::Unload(*src.m_AmbientTemperature));
  if (src.HasAtmosphericPressure())
    dst.set_allocated_atmosphericpressure(PBProperty::Unload(*src.m_AtmosphericPressure));
  if (src.HasClothingResistance())
    dst.set_allocated_clothingresistance(PBProperty::Unload(*src.m_ClothingResistance));
  if (src.HasEmissivity())
    dst.set_allocated_emissivity(PBProperty::Unload(*src.m_Emissivity));
  if (src.HasMeanRadiantTemperature())
    dst.set_allocated_meanradianttemperature(PBProperty::Unload(*src.m_MeanRadiantTemperature));
  if (src.HasRelativeHumidity())
    dst.set_allocated_relativehumidity(PBProperty::Unload(*src.m_RelativeHumidity));
  if (src.HasRespirationAmbientTemperature())
    dst.set_allocated_respirationambienttemperature(PBProperty::Unload(*src.m_RespirationAmbientTemperature));

  for (SESubstanceFraction *sf : src.m_AmbientGases)
    dst.mutable_ambientgas()->AddAllocated(PBSubstance::Unload(*sf));

  for (SESubstanceConcentration *sc : src.m_AmbientAerosols)
    dst.mutable_ambientaerosol()->AddAllocated(PBSubstance::Unload(*sc));
}
void PBEnvironment::Copy(const SEEnvironmentalConditions& src, SEEnvironmentalConditions& dst)
{
  cdm::EnvironmentalConditionsData data;
  PBEnvironment::Serialize(src, data);
  PBEnvironment::Serialize(data, dst);
}

void PBEnvironment::Load(const cdm::AppliedTemperatureData& src, SEAppliedTemperature& dst)
{
  PBEnvironment::Serialize(src, dst);
}
void PBEnvironment::Serialize(const cdm::AppliedTemperatureData& src, SEAppliedTemperature& dst)
{
  dst.Clear();
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
  if (src.has_temperature())
    PBProperty::Load(src.temperature(), dst.GetTemperature());
  if (src.has_surfacearea())
    PBProperty::Load(src.surfacearea(), dst.GetSurfaceArea());
  if (src.has_surfaceareafraction())
    PBProperty::Load(src.surfaceareafraction(), dst.GetSurfaceAreaFraction());
}
cdm::AppliedTemperatureData* PBEnvironment::Unload(const SEAppliedTemperature& src)
{
  cdm::AppliedTemperatureData* dst = new cdm::AppliedTemperatureData();
  PBEnvironment::Serialize(src, *dst);
  return dst;
}
void PBEnvironment::Serialize(const SEAppliedTemperature& src, cdm::AppliedTemperatureData& dst)
{
  dst.set_state((cdm::eSwitch)src.m_State);
  if (src.HasTemperature())
    dst.set_allocated_temperature(PBProperty::Unload(*src.m_Temperature));
  if (src.HasSurfaceArea())
    dst.set_allocated_surfacearea(PBProperty::Unload(*src.m_SurfaceArea));
  if (src.HasSurfaceAreaFraction())
    dst.set_allocated_surfaceareafraction(PBProperty::Unload(*src.m_SurfaceAreaFraction));
}
void PBEnvironment::Copy(const SEAppliedTemperature& src, SEAppliedTemperature& dst)
{
  cdm::AppliedTemperatureData data;
  PBEnvironment::Serialize(src, data);
  PBEnvironment::Serialize(data, dst);
}

void PBEnvironment::Load(const cdm::ActiveConditioningData& src, SEActiveConditioning& dst)
{
  PBEnvironment::Serialize(src, dst);
}
void PBEnvironment::Serialize(const cdm::ActiveConditioningData& src, SEActiveConditioning& dst)
{
  dst.Clear();
  if (src.has_power())
    PBProperty::Load(src.power(), dst.GetPower());
  if (src.has_surfacearea())
    PBProperty::Load(src.surfacearea(), dst.GetSurfaceArea());
  if (src.has_surfaceareafraction())
    PBProperty::Load(src.surfaceareafraction(), dst.GetSurfaceAreaFraction());
}
cdm::ActiveConditioningData* PBEnvironment::Unload(const SEActiveConditioning& src)
{
  cdm::ActiveConditioningData* dst = new cdm::ActiveConditioningData();
  PBEnvironment::Serialize(src, *dst);
  return dst;
}
void PBEnvironment::Serialize(const SEActiveConditioning& src, cdm::ActiveConditioningData& dst)
{
  if (src.HasPower())
    dst.set_allocated_power(PBProperty::Unload(*src.m_Power));
  if (src.HasSurfaceArea())
    dst.set_allocated_surfacearea(PBProperty::Unload(*src.m_SurfaceArea));
  if (src.HasSurfaceAreaFraction())
    dst.set_allocated_surfaceareafraction(PBProperty::Unload(*src.m_SurfaceAreaFraction));
}
void PBEnvironment::Copy(const SEActiveConditioning& src, SEActiveConditioning& dst)
{
  cdm::ActiveConditioningData data;
  PBEnvironment::Serialize(src, data);
  PBEnvironment::Serialize(data, dst);
}

bool PBEnvironment::SerializeToString(const SEEnvironmentalConditions& src, std::string& output, SerializationMode m)
{
  cdm::EnvironmentalConditionsData data;
  PBEnvironment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBEnvironment::SerializeToFile(const SEEnvironmentalConditions& src, const std::string& filename, SerializationMode m)
{
  cdm::EnvironmentalConditionsData data;
  PBEnvironment::Serialize(src, data);
  std::string content;
  PBEnvironment::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}

bool PBEnvironment::SerializeFromString(const std::string& src, SEEnvironmentalConditions& dst, SerializationMode m)
{
  cdm::EnvironmentalConditionsData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBEnvironment::Load(data, dst);
  return true;
}
bool PBEnvironment::SerializeFromFile(const std::string& filename, SEEnvironmentalConditions& dst, SerializationMode m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEnvironment::SerializeFromString(content, dst, m);
}