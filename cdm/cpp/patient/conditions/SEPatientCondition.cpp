/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "SEPatientCondition.h"
PROTO_PUSH
#include "bind/cdm/PatientConditions.pb.h"
PROTO_POP

SEPatientCondition::SEPatientCondition() : SECondition()
{
  
}

SEPatientCondition::~SEPatientCondition()
{
  Clear();
}

void SEPatientCondition::Clear()
{
  SECondition::Clear();
}

bool SEPatientCondition::IsValid() const
{
  return SECondition::IsValid();
}

void SEPatientCondition::Serialize(const cdm::PatientConditionData& src, SEPatientCondition& dst)
{
  SECondition::Serialize(src.condition(), dst);
}

void SEPatientCondition::Serialize(const SEPatientCondition& src, cdm::PatientConditionData& dst)
{
  SECondition::Serialize(src, *dst.mutable_condition());
}

#include "substance/SESubstanceManager.h"
#include "patient/conditions/SEChronicAnemia.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SEChronicPericardialEffusion.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "patient/conditions/SEConsumeMeal.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
#include "patient/conditions/SELobarPneumonia.h"
SEPatientCondition* SEPatientCondition::Load(const cdm::AnyPatientConditionData& any, SESubstanceManager& subMgr)
{
  switch (any.Condition_case())
  {
    case cdm::AnyPatientConditionData::ConditionCase::kChronicAnemia:
    {
      SEChronicAnemia* a = new SEChronicAnemia();
      SEChronicAnemia::Load(any.chronicanemia(), *a);
      return a;
    }
    case cdm::AnyPatientConditionData::ConditionCase::kChronicObstructivePulmonaryDisease:
    {
      SEChronicObstructivePulmonaryDisease* a = new SEChronicObstructivePulmonaryDisease();
      SEChronicObstructivePulmonaryDisease::Load(any.chronicobstructivepulmonarydisease(), *a);
      return a;
    }
    case cdm::AnyPatientConditionData::ConditionCase::kChronicPericardialEffusion:
    {
      SEChronicPericardialEffusion* a = new SEChronicPericardialEffusion();
      SEChronicPericardialEffusion::Load(any.chronicpericardialeffusion(), *a);
      return a;
    }
    case cdm::AnyPatientConditionData::ConditionCase::kChronicRenalStenosis:
    {
      SEChronicRenalStenosis* a = new SEChronicRenalStenosis();
      SEChronicRenalStenosis::Load(any.chronicrenalstenosis(), *a);
      return a;
    }
    case cdm::AnyPatientConditionData::ConditionCase::kChronicVentricularSystolicDysfunction:
    {
      SEChronicVentricularSystolicDysfunction* a = new SEChronicVentricularSystolicDysfunction();
      SEChronicVentricularSystolicDysfunction::Load(any.chronicventricularsystolicdysfunction(), *a);
      return a;
    }
    case cdm::AnyPatientConditionData::ConditionCase::kConsumeMeal:
    {
      SEConsumeMeal* a = new SEConsumeMeal();
      SEConsumeMeal::Load(any.consumemeal(), *a);
      return a;
    }
    case cdm::AnyPatientConditionData::ConditionCase::kImpairedAlveolarExchange:
    {
      SEImpairedAlveolarExchange* a = new SEImpairedAlveolarExchange();
      SEImpairedAlveolarExchange::Load(any.impairedalveolarexchange(), *a);
      return a;
    }
    case cdm::AnyPatientConditionData::ConditionCase::kLobarPneumonia:
    {
      SELobarPneumonia* a = new SELobarPneumonia();
      SELobarPneumonia::Load(any.lobarpneumonia(), *a);
      return a;
    }
  }
  subMgr.Error("Unknown condition type : " + any.Condition_case());
  return nullptr;
}
cdm::AnyPatientConditionData* SEPatientCondition::Unload(const SEPatientCondition& condition)
{
  cdm::AnyPatientConditionData* any = new cdm::AnyPatientConditionData();
  const SEChronicAnemia* ca = dynamic_cast<const SEChronicAnemia*>(&condition);
  if (ca != nullptr)
  {
    any->set_allocated_chronicanemia(SEChronicAnemia::Unload(*ca));
    return any;
  }
  const SEChronicObstructivePulmonaryDisease* copd = dynamic_cast<const SEChronicObstructivePulmonaryDisease*>(&condition);
  if (copd != nullptr)
  {
    any->set_allocated_chronicobstructivepulmonarydisease(SEChronicObstructivePulmonaryDisease::Unload(*copd));
    return any;
  }
  const SEChronicPericardialEffusion* cpe = dynamic_cast<const SEChronicPericardialEffusion*>(&condition);
  if (cpe != nullptr)
  {
    any->set_allocated_chronicpericardialeffusion(SEChronicPericardialEffusion::Unload(*cpe));
    return any;
  }
  const SEChronicRenalStenosis* crs = dynamic_cast<const SEChronicRenalStenosis*>(&condition);
  if (crs != nullptr)
  {
    any->set_allocated_chronicrenalstenosis(SEChronicRenalStenosis::Unload(*crs));
    return any;
  }
  const SEChronicVentricularSystolicDysfunction* cvsd = dynamic_cast<const SEChronicVentricularSystolicDysfunction*>(&condition);
  if (cvsd != nullptr)
  {
    any->set_allocated_chronicventricularsystolicdysfunction(SEChronicVentricularSystolicDysfunction::Unload(*cvsd));
    return any;
  }
  const SEConsumeMeal* cm = dynamic_cast<const SEConsumeMeal*>(&condition);
  if (cm != nullptr)
  {
    any->set_allocated_consumemeal(SEConsumeMeal::Unload(*cm));
    return any;
  }
  const SEImpairedAlveolarExchange* iae = dynamic_cast<const SEImpairedAlveolarExchange*>(&condition);
  if (iae != nullptr)
  {
    any->set_allocated_impairedalveolarexchange(SEImpairedAlveolarExchange::Unload(*iae));
    return any;
  }
  const SELobarPneumonia* lp = dynamic_cast<const SELobarPneumonia*>(&condition);
  if (lp != nullptr)
  {
    any->set_allocated_lobarpneumonia(SELobarPneumonia::Unload(*lp));
    return any;
  }
  delete any;
  return nullptr;
}
