/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/PatientConditions.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBPatientConditions.h"
#include "io/protobuf/PBPatientNutrition.h"
#include "io/protobuf/PBConditions.h"
#include "io/protobuf/PBProperties.h"

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
#include "patient/conditions/SEPulmonaryShunt.h"
#include "patient/conditions/SESepsis.h"

void PBPatientCondition::Serialize(const CDM_BIND::PatientConditionData& src, SEPatientCondition& dst)
{
  PBCondition::Serialize(src.condition(), dst);
}
void PBPatientCondition::Serialize(const SEPatientCondition& src, CDM_BIND::PatientConditionData& dst)
{
  PBCondition::Serialize(src, *dst.mutable_condition());
}

void PBPatientCondition::Load(const CDM_BIND::AcuteRespiratoryDistressSyndromeData& src, SEAcuteRespiratoryDistressSyndrome& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::AcuteRespiratoryDistressSyndromeData& src, SEAcuteRespiratoryDistressSyndrome& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
  if (src.has_leftlungaffected())
    PBProperty::Load(src.leftlungaffected(), dst.GetLeftLungAffected());
  if (src.has_rightlungaffected())
    PBProperty::Load(src.rightlungaffected(), dst.GetRightLungAffected());
}
CDM_BIND::AcuteRespiratoryDistressSyndromeData* PBPatientCondition::Unload(const SEAcuteRespiratoryDistressSyndrome& src)
{
  CDM_BIND::AcuteRespiratoryDistressSyndromeData* dst = new CDM_BIND::AcuteRespiratoryDistressSyndromeData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEAcuteRespiratoryDistressSyndrome& src, CDM_BIND::AcuteRespiratoryDistressSyndromeData& dst)
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
  dst.Clear();
  CDM_BIND::AcuteRespiratoryDistressSyndromeData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::ChronicAnemiaData& src, SEChronicAnemia& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::ChronicAnemiaData& src, SEChronicAnemia& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_reductionfactor())
    PBProperty::Load(src.reductionfactor(), dst.GetReductionFactor());
}
CDM_BIND::ChronicAnemiaData* PBPatientCondition::Unload(const SEChronicAnemia& src)
{
  CDM_BIND::ChronicAnemiaData* dst = new CDM_BIND::ChronicAnemiaData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicAnemia& src, CDM_BIND::ChronicAnemiaData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasReductionFactor())
    dst.set_allocated_reductionfactor(PBProperty::Unload(*src.m_ReductionFactor));
}
void PBPatientCondition::Copy(const SEChronicAnemia& src, SEChronicAnemia& dst)
{
  dst.Clear();
  CDM_BIND::ChronicAnemiaData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::ChronicObstructivePulmonaryDiseaseData& src, SEChronicObstructivePulmonaryDisease& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::ChronicObstructivePulmonaryDiseaseData& src, SEChronicObstructivePulmonaryDisease& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_bronchitisseverity())
    PBProperty::Load(src.bronchitisseverity(), dst.GetBronchitisSeverity());
  if (src.has_emphysemaseverity())
    PBProperty::Load(src.emphysemaseverity(), dst.GetEmphysemaSeverity());
}
CDM_BIND::ChronicObstructivePulmonaryDiseaseData* PBPatientCondition::Unload(const SEChronicObstructivePulmonaryDisease& src)
{
  CDM_BIND::ChronicObstructivePulmonaryDiseaseData* dst = new CDM_BIND::ChronicObstructivePulmonaryDiseaseData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicObstructivePulmonaryDisease& src, CDM_BIND::ChronicObstructivePulmonaryDiseaseData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasBronchitisSeverity())
    dst.set_allocated_bronchitisseverity(PBProperty::Unload(*src.m_BronchitisSeverity));
  if (src.HasEmphysemaSeverity())
    dst.set_allocated_emphysemaseverity(PBProperty::Unload(*src.m_EmphysemaSeverity));
}
void PBPatientCondition::Copy(const SEChronicObstructivePulmonaryDisease& src, SEChronicObstructivePulmonaryDisease& dst)
{
  dst.Clear();
  CDM_BIND::ChronicObstructivePulmonaryDiseaseData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::ChronicPericardialEffusionData& src, SEChronicPericardialEffusion& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::ChronicPericardialEffusionData& src, SEChronicPericardialEffusion& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_accumulatedvolume())
    PBProperty::Load(src.accumulatedvolume(), dst.GetAccumulatedVolume());
}
CDM_BIND::ChronicPericardialEffusionData* PBPatientCondition::Unload(const SEChronicPericardialEffusion& src)
{
  CDM_BIND::ChronicPericardialEffusionData* dst = new CDM_BIND::ChronicPericardialEffusionData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicPericardialEffusion& src, CDM_BIND::ChronicPericardialEffusionData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasAccumulatedVolume())
    dst.set_allocated_accumulatedvolume(PBProperty::Unload(*src.m_AccumulatedVolume));
}
void PBPatientCondition::Copy(const SEChronicPericardialEffusion& src, SEChronicPericardialEffusion& dst)
{
  dst.Clear();
  CDM_BIND::ChronicPericardialEffusionData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::ChronicRenalStenosisData& src, SEChronicRenalStenosis& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::ChronicRenalStenosisData& src, SEChronicRenalStenosis& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_leftkidneyseverity())
    PBProperty::Load(src.leftkidneyseverity(), dst.GetLeftKidneySeverity());
  if (src.has_rightkidneyseverity())
    PBProperty::Load(src.rightkidneyseverity(), dst.GetRightKidneySeverity());
}
CDM_BIND::ChronicRenalStenosisData* PBPatientCondition::Unload(const SEChronicRenalStenosis& src)
{
  CDM_BIND::ChronicRenalStenosisData* dst = new CDM_BIND::ChronicRenalStenosisData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicRenalStenosis& src, CDM_BIND::ChronicRenalStenosisData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasLeftKidneySeverity())
    dst.set_allocated_leftkidneyseverity(PBProperty::Unload(*src.m_LeftKidneySeverity));
  if (src.HasRightKidneySeverity())
    dst.set_allocated_rightkidneyseverity(PBProperty::Unload(*src.m_RightKidneySeverity));
}
void PBPatientCondition::Copy(const SEChronicRenalStenosis& src, SEChronicRenalStenosis& dst)
{
  dst.Clear();
  CDM_BIND::ChronicRenalStenosisData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::ChronicVentricularSystolicDysfunctionData& src, SEChronicVentricularSystolicDysfunction& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::ChronicVentricularSystolicDysfunctionData& src, SEChronicVentricularSystolicDysfunction& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
}
CDM_BIND::ChronicVentricularSystolicDysfunctionData* PBPatientCondition::Unload(const SEChronicVentricularSystolicDysfunction& src)
{
  CDM_BIND::ChronicVentricularSystolicDysfunctionData* dst = new CDM_BIND::ChronicVentricularSystolicDysfunctionData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEChronicVentricularSystolicDysfunction& src, CDM_BIND::ChronicVentricularSystolicDysfunctionData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
}
void PBPatientCondition::Copy(const SEChronicVentricularSystolicDysfunction& src, SEChronicVentricularSystolicDysfunction& dst)
{
  dst.Clear();
  CDM_BIND::ChronicVentricularSystolicDysfunctionData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::ConsumeMealData& src, SEConsumeMeal& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::ConsumeMealData& src, SEConsumeMeal& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if(!src.mealfile().empty())
    dst.SetMealFile(src.mealfile());
  else if (src.has_meal())
    PBPatientNutrition::Load(src.meal(), dst.GetMeal());
}
CDM_BIND::ConsumeMealData* PBPatientCondition::Unload(const SEConsumeMeal& src)
{
  CDM_BIND::ConsumeMealData* dst = new CDM_BIND::ConsumeMealData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEConsumeMeal& src, CDM_BIND::ConsumeMealData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasMealFile())
    dst.set_mealfile(src.m_MealFile); 
  else if (src.HasMeal())
    dst.set_allocated_meal(PBPatientNutrition::Unload(*src.m_Meal));
}
void PBPatientCondition::Copy(const SEConsumeMeal& src, SEConsumeMeal& dst)
{
  dst.Clear();
  CDM_BIND::ConsumeMealData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::ImpairedAlveolarExchangeData& src, SEImpairedAlveolarExchange& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::ImpairedAlveolarExchangeData& src, SEImpairedAlveolarExchange& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
  else if (src.has_impairedfraction())
    PBProperty::Load(src.impairedfraction(), dst.GetImpairedFraction());
  else if (src.has_impairedsurfacearea())
    PBProperty::Load(src.impairedsurfacearea(), dst.GetImpairedSurfaceArea());
}
CDM_BIND::ImpairedAlveolarExchangeData* PBPatientCondition::Unload(const SEImpairedAlveolarExchange& src)
{
  CDM_BIND::ImpairedAlveolarExchangeData* dst = new CDM_BIND::ImpairedAlveolarExchangeData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEImpairedAlveolarExchange& src, CDM_BIND::ImpairedAlveolarExchangeData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
  else if (src.HasImpairedFraction())
    dst.set_allocated_impairedfraction(PBProperty::Unload(*src.m_ImpairedFraction));
  else if (src.HasImpairedSurfaceArea())
    dst.set_allocated_impairedsurfacearea(PBProperty::Unload(*src.m_ImpairedSurfaceArea));
}
void PBPatientCondition::Copy(const SEImpairedAlveolarExchange& src, SEImpairedAlveolarExchange& dst)
{
  dst.Clear();
  CDM_BIND::ImpairedAlveolarExchangeData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::LobarPneumoniaData& src, SELobarPneumonia& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::LobarPneumoniaData& src, SELobarPneumonia& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
  if (src.has_leftlungaffected())
    PBProperty::Load(src.leftlungaffected(), dst.GetLeftLungAffected());
  if (src.has_rightlungaffected())
    PBProperty::Load(src.rightlungaffected(), dst.GetRightLungAffected());
}
CDM_BIND::LobarPneumoniaData* PBPatientCondition::Unload(const SELobarPneumonia& src)
{
  CDM_BIND::LobarPneumoniaData* dst = new CDM_BIND::LobarPneumoniaData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SELobarPneumonia& src, CDM_BIND::LobarPneumoniaData& dst)
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
  dst.Clear();
  CDM_BIND::LobarPneumoniaData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::PulmonaryFibrosisData& src, SEPulmonaryFibrosis& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::PulmonaryFibrosisData& src, SEPulmonaryFibrosis& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::PulmonaryFibrosisData* PBPatientCondition::Unload(const SEPulmonaryFibrosis& src)
{
  CDM_BIND::PulmonaryFibrosisData* dst = new CDM_BIND::PulmonaryFibrosisData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEPulmonaryFibrosis& src, CDM_BIND::PulmonaryFibrosisData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientCondition::Copy(const SEPulmonaryFibrosis& src, SEPulmonaryFibrosis& dst)
{
  dst.Clear();
  CDM_BIND::PulmonaryFibrosisData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::PulmonaryShuntData& src, SEPulmonaryShunt& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::PulmonaryShuntData& src, SEPulmonaryShunt& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::PulmonaryShuntData* PBPatientCondition::Unload(const SEPulmonaryShunt& src)
{
  CDM_BIND::PulmonaryShuntData* dst = new CDM_BIND::PulmonaryShuntData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SEPulmonaryShunt& src, CDM_BIND::PulmonaryShuntData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientCondition::Copy(const SEPulmonaryShunt& src, SEPulmonaryShunt& dst)
{
  dst.Clear();
  CDM_BIND::PulmonaryShuntData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

void PBPatientCondition::Load(const CDM_BIND::SepsisData& src, SESepsis& dst)
{
  dst.Clear();
  PBPatientCondition::Serialize(src, dst);
}
void PBPatientCondition::Serialize(const CDM_BIND::SepsisData& src, SESepsis& dst)
{
  PBPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::SepsisData* PBPatientCondition::Unload(const SESepsis& src)
{
  CDM_BIND::SepsisData* dst = new CDM_BIND::SepsisData();
  PBPatientCondition::Serialize(src, *dst);
  return dst;
}
void PBPatientCondition::Serialize(const SESepsis& src, CDM_BIND::SepsisData& dst)
{
  PBPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientCondition::Copy(const SESepsis& src, SESepsis& dst)
{
  dst.Clear();
  CDM_BIND::SepsisData data;
  PBPatientCondition::Serialize(src, data);
  PBPatientCondition::Serialize(data, dst);
}

SEPatientCondition* PBPatientCondition::Load(const CDM_BIND::AnyPatientConditionData& any, const SESubstanceManager& subMgr)
{
  switch (any.Condition_case())
  {
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kAcuteRespiratoryDistressSyndrome:
  {
    SEAcuteRespiratoryDistressSyndrome* a = new SEAcuteRespiratoryDistressSyndrome();
    PBPatientCondition::Load(any.acuterespiratorydistresssyndrome(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kChronicAnemia:
  {
    SEChronicAnemia* a = new SEChronicAnemia();
    PBPatientCondition::Load(any.chronicanemia(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kChronicObstructivePulmonaryDisease:
  {
    SEChronicObstructivePulmonaryDisease* a = new SEChronicObstructivePulmonaryDisease();
    PBPatientCondition::Load(any.chronicobstructivepulmonarydisease(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kChronicPericardialEffusion:
  {
    SEChronicPericardialEffusion* a = new SEChronicPericardialEffusion();
    PBPatientCondition::Load(any.chronicpericardialeffusion(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kChronicRenalStenosis:
  {
    SEChronicRenalStenosis* a = new SEChronicRenalStenosis();
    PBPatientCondition::Load(any.chronicrenalstenosis(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kChronicVentricularSystolicDysfunction:
  {
    SEChronicVentricularSystolicDysfunction* a = new SEChronicVentricularSystolicDysfunction();
    PBPatientCondition::Load(any.chronicventricularsystolicdysfunction(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kConsumeMeal:
  {
    SEConsumeMeal* a = new SEConsumeMeal();
    PBPatientCondition::Load(any.consumemeal(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kImpairedAlveolarExchange:
  {
    SEImpairedAlveolarExchange* a = new SEImpairedAlveolarExchange();
    PBPatientCondition::Load(any.impairedalveolarexchange(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kLobarPneumonia:
  {
    SELobarPneumonia* a = new SELobarPneumonia();
    PBPatientCondition::Load(any.lobarpneumonia(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kPulmonaryFibrosis:
  {
    SEPulmonaryFibrosis* a = new SEPulmonaryFibrosis();
    PBPatientCondition::Load(any.pulmonaryfibrosis(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kPulmonaryShunt:
  {
    SEPulmonaryShunt* a = new SEPulmonaryShunt();
    PBPatientCondition::Load(any.pulmonaryshunt(), *a);
    return a;
  }
  case CDM_BIND::AnyPatientConditionData::ConditionCase::kSepsis:
  {
    SESepsis* a = new SESepsis();
    PBPatientCondition::Load(any.sepsis(), *a);
    return a;
  }
  }
  subMgr.Error("Unknown condition type : " + any.Condition_case());
  return nullptr;
}
CDM_BIND::AnyPatientConditionData* PBPatientCondition::Unload(const SEPatientCondition& condition)
{
  CDM_BIND::AnyPatientConditionData* any = new CDM_BIND::AnyPatientConditionData();
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
  const SEPulmonaryShunt* ps = dynamic_cast<const SEPulmonaryShunt*>(&condition);
  if (ps != nullptr)
  {
    any->set_allocated_pulmonaryshunt(PBPatientCondition::Unload(*ps));
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
