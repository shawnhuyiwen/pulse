/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(PatientCondition)
CDM_BIND_DECL2(ChronicAnemia)
CDM_BIND_DECL2(ChronicObstructivePulmonaryDisease)
CDM_BIND_DECL2(ChronicPericardialEffusion)
CDM_BIND_DECL2(ChronicRenalStenosis)
CDM_BIND_DECL2(ChronicVentricularSystolicDysfunction)
CDM_BIND_DECL2(ConsumeMeal)
CDM_BIND_DECL2(ImpairedAlveolarExchange)
CDM_BIND_DECL2(LobarPneumonia)
CDM_BIND_DECL(AnyPatientConditionData)
class SESubstanceManager;


class CDM_DECL PBPatientCondition
{
public:

  /** Create a new condition based on the binding object, load that data into the new condition, and return said condition */
  static SEPatientCondition* Load(const cdm::AnyPatientConditionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyPatientConditionData* Unload(const SEPatientCondition& condition);
  static void Serialize(const cdm::PatientConditionData& src, SEPatientCondition& dst);
  static void Serialize(const SEPatientCondition& src, cdm::PatientConditionData& dst);

  static void Load(const cdm::ChronicAnemiaData& src, SEChronicAnemia& dst);
  static cdm::ChronicAnemiaData* Unload(const SEChronicAnemia& src);
  static void Serialize(const cdm::ChronicAnemiaData& src, SEChronicAnemia& dst);
  static void Serialize(const SEChronicAnemia& src, cdm::ChronicAnemiaData& dst);
  static void Copy(const SEChronicAnemia& src, SEChronicAnemia& dst);

  static void Load(const cdm::ChronicObstructivePulmonaryDiseaseData& src, SEChronicObstructivePulmonaryDisease& dst);
  static cdm::ChronicObstructivePulmonaryDiseaseData* Unload(const SEChronicObstructivePulmonaryDisease& src);
  static void Serialize(const cdm::ChronicObstructivePulmonaryDiseaseData& src, SEChronicObstructivePulmonaryDisease& dst);
  static void Serialize(const SEChronicObstructivePulmonaryDisease& src, cdm::ChronicObstructivePulmonaryDiseaseData& dst);
  static void Copy(const SEChronicObstructivePulmonaryDisease& src, SEChronicObstructivePulmonaryDisease& dst);

  static void Load(const cdm::ChronicPericardialEffusionData& src, SEChronicPericardialEffusion& dst);
  static cdm::ChronicPericardialEffusionData* Unload(const SEChronicPericardialEffusion& src);
  static void Serialize(const cdm::ChronicPericardialEffusionData& src, SEChronicPericardialEffusion& dst);
  static void Serialize(const SEChronicPericardialEffusion& src, cdm::ChronicPericardialEffusionData& dst);
  static void Copy(const SEChronicPericardialEffusion& src, SEChronicPericardialEffusion& dst);

  static void Load(const cdm::ChronicRenalStenosisData& src, SEChronicRenalStenosis& dst);
  static cdm::ChronicRenalStenosisData* Unload(const SEChronicRenalStenosis& src);
  static void Serialize(const cdm::ChronicRenalStenosisData& src, SEChronicRenalStenosis& dst);
  static void Serialize(const SEChronicRenalStenosis& src, cdm::ChronicRenalStenosisData& dst);
  static void Copy(const SEChronicRenalStenosis& src, SEChronicRenalStenosis& dst);

  static void Load(const cdm::ChronicVentricularSystolicDysfunctionData& src, SEChronicVentricularSystolicDysfunction& dst);
  static cdm::ChronicVentricularSystolicDysfunctionData* Unload(const SEChronicVentricularSystolicDysfunction& src);
  static void Serialize(const cdm::ChronicVentricularSystolicDysfunctionData& src, SEChronicVentricularSystolicDysfunction& dst);
  static void Serialize(const SEChronicVentricularSystolicDysfunction& src, cdm::ChronicVentricularSystolicDysfunctionData& dst);
  static void Copy(const SEChronicVentricularSystolicDysfunction& src, SEChronicVentricularSystolicDysfunction& dst);

  static void Load(const cdm::ConsumeMealData& src, SEConsumeMeal& dst);
  static cdm::ConsumeMealData* Unload(const SEConsumeMeal& src);
  static void Serialize(const cdm::ConsumeMealData& src, SEConsumeMeal& dst);
  static void Serialize(const SEConsumeMeal& src, cdm::ConsumeMealData& dst);
  static void Copy(const SEConsumeMeal& src, SEConsumeMeal& dst);

  static void Load(const cdm::ImpairedAlveolarExchangeData& src, SEImpairedAlveolarExchange& dst);
  static cdm::ImpairedAlveolarExchangeData* Unload(const SEImpairedAlveolarExchange& src);
  static void Serialize(const cdm::ImpairedAlveolarExchangeData& src, SEImpairedAlveolarExchange& dst);
  static void Serialize(const SEImpairedAlveolarExchange& src, cdm::ImpairedAlveolarExchangeData& dst);
  static void Copy(const SEImpairedAlveolarExchange& src, SEImpairedAlveolarExchange& dst);

  static void Load(const cdm::LobarPneumoniaData& src, SELobarPneumonia& dst);
  static cdm::LobarPneumoniaData* Unload(const SELobarPneumonia& src);
  static void Serialize(const cdm::LobarPneumoniaData& src, SELobarPneumonia& dst);
  static void Serialize(const SELobarPneumonia& src, cdm::LobarPneumoniaData& dst);
  static void Copy(const SELobarPneumonia& src, SELobarPneumonia& dst);

};