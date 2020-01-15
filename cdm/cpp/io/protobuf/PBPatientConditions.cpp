/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBPatientConditions.h"
#include "io/protobuf/PBPatientNutrition.h"
#include "io/protobuf/PBConditions.h"
#include "io/protobuf/PBProperties.h"
#include "bind/cpp/cdm/PatientConditions.pb.h"

#include "substance/SESubstanceManager.h"
#include "patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
#include "patient/conditions/SEChronicAnemia.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SEChronicPericardialEffusion.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "patient/conditions/SEConsumeMeal.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "patient/conditions/SEPulmonaryFibrosis.h"
#include "patient/conditions/SESepsis.h"

void PBPatientCondition::Serialize(const cdm::PatientConditionData& src, SEPatientCondition& dst)
{
  PBCondition::Serialize(src.condition(), dst);
}
void PBPatientCondition::Serialize(const SEPatientCondition& src, cdm::PatientConditionData& dst)
{
  PBCondition::Serialize(src, *dst.mutable_condition());
}

void PBPatientCondition::Load(const cdm::AcuteRespiratoryDistressSyndromeData& src, SEAcuteRespiratoryDistressSyndrome& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::AcuteRespiratoryDistressSyndromeData& src, SEAcuteRespiratoryDistressSyndrome& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
  if (src.has_leftlungaffected())
    PBProperty::Load(src.leftlungaffected(), dst.GetLeftLungAffected());
  if (src.has_rightlungaffected())
    PBProperty::Load(src.rightlungaffected(), dst.GetRightLungAffected());
}
cdm::AcuteRespiratoryDistressSyndromeData* PBPatientCondition::Unload(const SEAcuteRespiratoryDistressSyndrome& src)
{
  cdm::AcuteRespiratoryDistressSyndromeData* dst = new cdm::AcuteRespiratoryDistressSyndromeData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEAcuteRespiratoryDistressSyndrome& src, cdm::AcuteRespiratoryDistressSyndromeData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
  if (src.HasRightLungAffected())
    dst.set_allocated_rightlungaffected(PBProperty::Unload(*src.m_RightLungAffected));
  if (src.HasLeftLungAffected())
    dst.set_allocated_leftlungaffected(PBProperty::Unload(*src.m_LeftLungAffected));
}
void PBPatientCondition::Copy(const SEAcuteRespiratoryDistressSyndrome& src, SEAcuteRespiratoryDistressSyndrome& dst)
{
  cdm::AcuteRespiratoryDistressSyndromeData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::ChronicAnemiaData& src, SEChronicAnemia& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::ChronicAnemiaData& src, SEChronicAnemia& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_reductionfactor())
    PBProperty::Load(src.reductionfactor(), dst.GetReductionFactor());
}
cdm::ChronicAnemiaData* PBPatientCondition::Unload(const SEChronicAnemia& src)
{
  cdm::ChronicAnemiaData* dst = new cdm::ChronicAnemiaData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicAnemia& src, cdm::ChronicAnemiaData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasReductionFactor())
    dst.set_allocated_reductionfactor(PBProperty::Unload(*src.m_ReductionFactor));
}
void PBPatientCondition::Copy(const SEChronicAnemia& src, SEChronicAnemia& dst)
{
  cdm::ChronicAnemiaData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::ChronicObstructivePulmonaryDiseaseData& src, SEChronicObstructivePulmonaryDisease& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::ChronicObstructivePulmonaryDiseaseData& src, SEChronicObstructivePulmonaryDisease& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_bronchitisseverity())
    PBProperty::Load(src.bronchitisseverity(), dst.GetBronchitisSeverity());
  if (src.has_emphysemaseverity())
    PBProperty::Load(src.emphysemaseverity(), dst.GetEmphysemaSeverity());
}
cdm::ChronicObstructivePulmonaryDiseaseData* PBPatientCondition::Unload(const SEChronicObstructivePulmonaryDisease& src)
{
  cdm::ChronicObstructivePulmonaryDiseaseData* dst = new cdm::ChronicObstructivePulmonaryDiseaseData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicObstructivePulmonaryDisease& src, cdm::ChronicObstructivePulmonaryDiseaseData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasBronchitisSeverity())
    dst.set_allocated_bronchitisseverity(PBProperty::Unload(*src.m_BronchitisSeverity));
  if (src.HasEmphysemaSeverity())
    dst.set_allocated_emphysemaseverity(PBProperty::Unload(*src.m_EmphysemaSeverity));
}
void PBPatientCondition::Copy(const SEChronicObstructivePulmonaryDisease& src, SEChronicObstructivePulmonaryDisease& dst)
{
  cdm::ChronicObstructivePulmonaryDiseaseData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::ChronicPericardialEffusionData& src, SEChronicPericardialEffusion& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::ChronicPericardialEffusionData& src, SEChronicPericardialEffusion& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_accumulatedvolume())
    PBProperty::Load(src.accumulatedvolume(), dst.GetAccumulatedVolume());
}
cdm::ChronicPericardialEffusionData* PBPatientCondition::Unload(const SEChronicPericardialEffusion& src)
{
  cdm::ChronicPericardialEffusionData* dst = new cdm::ChronicPericardialEffusionData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicPericardialEffusion& src, cdm::ChronicPericardialEffusionData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasAccumulatedVolume())
    dst.set_allocated_accumulatedvolume(PBProperty::Unload(*src.m_AccumulatedVolume));
}
void PBPatientCondition::Copy(const SEChronicPericardialEffusion& src, SEChronicPericardialEffusion& dst)
{
  cdm::ChronicPericardialEffusionData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::ChronicRenalStenosisData& src, SEChronicRenalStenosis& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::ChronicRenalStenosisData& src, SEChronicRenalStenosis& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_leftkidneyseverity())
    PBProperty::Load(src.leftkidneyseverity(), dst.GetLeftKidneySeverity());
  if (src.has_rightkidneyseverity())
    PBProperty::Load(src.rightkidneyseverity(), dst.GetRightKidneySeverity());
}
cdm::ChronicRenalStenosisData* PBPatientCondition::Unload(const SEChronicRenalStenosis& src)
{
  cdm::ChronicRenalStenosisData* dst = new cdm::ChronicRenalStenosisData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicRenalStenosis& src, cdm::ChronicRenalStenosisData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasLeftKidneySeverity())
    dst.set_allocated_leftkidneyseverity(PBProperty::Unload(*src.m_LeftKidneySeverity));
  if (src.HasRightKidneySeverity())
    dst.set_allocated_rightkidneyseverity(PBProperty::Unload(*src.m_RightKidneySeverity));
}
void PBPatientCondition::Copy(const SEChronicRenalStenosis& src, SEChronicRenalStenosis& dst)
{
  cdm::ChronicRenalStenosisData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::ChronicVentricularSystolicDysfunctionData& src, SEChronicVentricularSystolicDysfunction& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::ChronicVentricularSystolicDysfunctionData& src, SEChronicVentricularSystolicDysfunction& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
}
cdm::ChronicVentricularSystolicDysfunctionData* PBPatientCondition::Unload(const SEChronicVentricularSystolicDysfunction& src)
{
  cdm::ChronicVentricularSystolicDysfunctionData* dst = new cdm::ChronicVentricularSystolicDysfunctionData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicVentricularSystolicDysfunction& src, cdm::ChronicVentricularSystolicDysfunctionData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
}
void PBPatientCondition::Copy(const SEChronicVentricularSystolicDysfunction& src, SEChronicVentricularSystolicDysfunction& dst)
{
  cdm::ChronicVentricularSystolicDysfunctionData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::ConsumeMealData& src, SEConsumeMeal& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::ConsumeMealData& src, SEConsumeMeal& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_meal())
    PBPatientNutrition::Load(src.meal(), dst.GetMeal());
  else
    dst.SetMealFile(src.mealfile());
}
cdm::ConsumeMealData* PBPatientCondition::Unload(const SEConsumeMeal& src)
{
  cdm::ConsumeMealData* dst = new cdm::ConsumeMealData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEConsumeMeal& src, cdm::ConsumeMealData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasMeal())
    dst.set_allocated_meal(PBPatientNutrition::Unload(*src.m_Meal));
  else if (src.HasMealFile())
    dst.set_mealfile(src.m_MealFile);
}
void PBPatientCondition::Copy(const SEConsumeMeal& src, SEConsumeMeal& dst)
{
  cdm::ConsumeMealData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::ImpairedAlveolarExchangeData& src, SEImpairedAlveolarExchange& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::ImpairedAlveolarExchangeData& src, SEImpairedAlveolarExchange& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_impairedfraction())
    PBProperty::Load(src.impairedfraction(), dst.GetImpairedFraction());
  if (src.has_impairedsurfacearea())
    PBProperty::Load(src.impairedsurfacearea(), dst.GetImpairedSurfaceArea());
}
cdm::ImpairedAlveolarExchangeData* PBPatientCondition::Unload(const SEImpairedAlveolarExchange& src)
{
  cdm::ImpairedAlveolarExchangeData* dst = new cdm::ImpairedAlveolarExchangeData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEImpairedAlveolarExchange& src, cdm::ImpairedAlveolarExchangeData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasImpairedFraction())
    dst.set_allocated_impairedfraction(PBProperty::Unload(*src.m_ImpairedFraction));
  if (src.HasImpairedSurfaceArea())
    dst.set_allocated_impairedsurfacearea(PBProperty::Unload(*src.m_ImpairedSurfaceArea));
}
void PBPatientCondition::Copy(const SEImpairedAlveolarExchange& src, SEImpairedAlveolarExchange& dst)
{
  cdm::ImpairedAlveolarExchangeData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::LobarPneumoniaData& src, SELobarPneumonia& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::LobarPneumoniaData& src, SELobarPneumonia& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
  if (src.has_leftlungaffected())
    PBProperty::Load(src.leftlungaffected(), dst.GetLeftLungAffected());
  if (src.has_rightlungaffected())
    PBProperty::Load(src.rightlungaffected(), dst.GetRightLungAffected());
}
cdm::LobarPneumoniaData* PBPatientCondition::Unload(const SELobarPneumonia& src)
{
  cdm::LobarPneumoniaData* dst = new cdm::LobarPneumoniaData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SELobarPneumonia& src, cdm::LobarPneumoniaData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
  if (src.HasRightLungAffected())
    dst.set_allocated_rightlungaffected(PBProperty::Unload(*src.m_RightLungAffected));
  if (src.HasLeftLungAffected())
    dst.set_allocated_leftlungaffected(PBProperty::Unload(*src.m_LeftLungAffected));
}
void PBPatientCondition::Copy(const SELobarPneumonia& src, SELobarPneumonia& dst)
{
  cdm::LobarPneumoniaData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::PulmonaryFibrosisData& src, SEPulmonaryFibrosis& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::PulmonaryFibrosisData& src, SEPulmonaryFibrosis& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::PulmonaryFibrosisData* PBPatientCondition::Unload(const SEPulmonaryFibrosis& src)
{
  cdm::PulmonaryFibrosisData* dst = new cdm::PulmonaryFibrosisData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEPulmonaryFibrosis& src, cdm::PulmonaryFibrosisData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientCondition::Copy(const SEPulmonaryFibrosis& src, SEPulmonaryFibrosis& dst)
{
  cdm::PulmonaryFibrosisData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const cdm::SepsisData& src, SESepsis& dst)
{
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const cdm::SepsisData& src, SESepsis& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::SepsisData* PBPatientCondition::Unload(const SESepsis& src)
{
  cdm::SepsisData* dst = new cdm::SepsisData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SESepsis& src, cdm::SepsisData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientCondition::Copy(const SESepsis& src, SESepsis& dst)
{
  cdm::SepsisData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

SEPatientCondition* PBPatientCondition::Load(const cdm::AnyPatientConditionData& any, SESubstanceManager& subMgr)
{
  switch (any.Condition_case())
  {
  case cdm::AnyPatientConditionData::ConditionCase::kAcuteRespiratoryDistressSyndrome:
  {
    SEAcuteRespiratoryDistressSyndrome* a = new SEAcuteRespiratoryDistressSyndrome();
    PBPatientCondition::Load(any.acuterespiratorydistresssyndrome(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kChronicAnemia:
  {
    SEChronicAnemia* a = new SEChronicAnemia();
    PBPatientCondition::Load(any.chronicanemia(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kChronicObstructivePulmonaryDisease:
  {
    SEChronicObstructivePulmonaryDisease* a = new SEChronicObstructivePulmonaryDisease();
    PBPatientCondition::Load(any.chronicobstructivepulmonarydisease(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kChronicPericardialEffusion:
  {
    SEChronicPericardialEffusion* a = new SEChronicPericardialEffusion();
    PBPatientCondition::Load(any.chronicpericardialeffusion(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kChronicRenalStenosis:
  {
    SEChronicRenalStenosis* a = new SEChronicRenalStenosis();
    PBPatientCondition::Load(any.chronicrenalstenosis(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kChronicVentricularSystolicDysfunction:
  {
    SEChronicVentricularSystolicDysfunction* a = new SEChronicVentricularSystolicDysfunction();
    PBPatientCondition::Load(any.chronicventricularsystolicdysfunction(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kConsumeMeal:
  {
    SEConsumeMeal* a = new SEConsumeMeal();
    PBPatientCondition::Load(any.consumemeal(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kImpairedAlveolarExchange:
  {
    SEImpairedAlveolarExchange* a = new SEImpairedAlveolarExchange();
    PBPatientCondition::Load(any.impairedalveolarexchange(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kLobarPneumonia:
  {
    SELobarPneumonia* a = new SELobarPneumonia();
    PBPatientCondition::Load(any.lobarpneumonia(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kPulmonaryFibrosis:
  {
    SEPulmonaryFibrosis* a = new SEPulmonaryFibrosis();
    PBPatientCondition::Load(any.pulmonaryfibrosis(), *a);
    return a;
  }
  case cdm::AnyPatientConditionData::ConditionCase::kSepsis:
  {
    SESepsis* a = new SESepsis();
    PBPatientCondition::Load(any.sepsis(), *a);
    return a;
  }
  }
  subMgr.Error("Unknown condition type : " + any.Condition_case());
  return nullptr;
}
cdm::AnyPatientConditionData* PBPatientCondition::Unload(const SEPatientCondition& condition)
{
  cdm::AnyPatientConditionData* any = new cdm::AnyPatientConditionData();
  const SEAcuteRespiratoryDistressSyndrome* ards = dynamic_cast<const SEAcuteRespiratoryDistressSyndrome*>(&condition);
  if (ards != nullptr)
  {
    any->set_allocated_acuterespiratorydistresssyndrome(PBPatientCondition::Unload(*ards));
    return any;
  }
  const SEChronicAnemia* ca = dynamic_cast<const SEChronicAnemia*>(&condition);
  if (ca != nullptr)
  {
    any->set_allocated_chronicanemia(PBPatientCondition::Unload(*ca));
    return any;
  }
  const SEChronicObstructivePulmonaryDisease* copd = dynamic_cast<const SEChronicObstructivePulmonaryDisease*>(&condition);
  if (copd != nullptr)
  {
    any->set_allocated_chronicobstructivepulmonarydisease(PBPatientCondition::Unload(*copd));
    return any;
  }
  const SEChronicPericardialEffusion* cpe = dynamic_cast<const SEChronicPericardialEffusion*>(&condition);
  if (cpe != nullptr)
  {
    any->set_allocated_chronicpericardialeffusion(PBPatientCondition::Unload(*cpe));
    return any;
  }
  const SEChronicRenalStenosis* crs = dynamic_cast<const SEChronicRenalStenosis*>(&condition);
  if (crs != nullptr)
  {
    any->set_allocated_chronicrenalstenosis(PBPatientCondition::Unload(*crs));
    return any;
  }
  const SEChronicVentricularSystolicDysfunction* cvsd = dynamic_cast<const SEChronicVentricularSystolicDysfunction*>(&condition);
  if (cvsd != nullptr)
  {
    any->set_allocated_chronicventricularsystolicdysfunction(PBPatientCondition::Unload(*cvsd));
    return any;
  }
  const SEConsumeMeal* cm = dynamic_cast<const SEConsumeMeal*>(&condition);
  if (cm != nullptr)
  {
    any->set_allocated_consumemeal(PBPatientCondition::Unload(*cm));
    return any;
  }
  const SEImpairedAlveolarExchange* iae = dynamic_cast<const SEImpairedAlveolarExchange*>(&condition);
  if (iae != nullptr)
  {
    any->set_allocated_impairedalveolarexchange(PBPatientCondition::Unload(*iae));
    return any;
  }
  const SELobarPneumonia* lp = dynamic_cast<const SELobarPneumonia*>(&condition);
  if (lp != nullptr)
  {
    any->set_allocated_lobarpneumonia(PBPatientCondition::Unload(*lp));
    return any;
  }
  const SEPulmonaryFibrosis* pf = dynamic_cast<const SEPulmonaryFibrosis*>(&condition);
  if (pf != nullptr)
  {
    any->set_allocated_pulmonaryfibrosis(PBPatientCondition::Unload(*pf));
    return any;
  }
  const SESepsis* s = dynamic_cast<const SESepsis*>(&condition);
  if (s != nullptr)
  {
    any->set_allocated_sepsis(PBPatientCondition::Unload(*s));
    return any;
  }
  delete any;
  return nullptr;
}
