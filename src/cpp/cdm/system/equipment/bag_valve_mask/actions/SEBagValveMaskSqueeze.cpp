/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskSqueeze.h"
#include "system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBEquipmentActions.h"

SEBagValveMaskSqueeze::SEBagValveMaskSqueeze(Logger* logger) : SEBagValveMaskAction(logger)
{
  m_PositiveInspiratoryPressure = nullptr;
  m_TidalVolume = nullptr;
  m_ExpiratoryPeriod = nullptr;
  m_InspiratoryPeriod = nullptr;
}

SEBagValveMaskSqueeze::~SEBagValveMaskSqueeze()
{
  SAFE_DELETE(m_PositiveInspiratoryPressure);
  SAFE_DELETE(m_TidalVolume);
  SAFE_DELETE(m_ExpiratoryPeriod);
  SAFE_DELETE(m_InspiratoryPeriod);
}

void SEBagValveMaskSqueeze::Clear()
{
  SEBagValveMaskAction::Clear();
  INVALIDATE_PROPERTY(m_PositiveInspiratoryPressure);
  INVALIDATE_PROPERTY(m_TidalVolume);
  INVALIDATE_PROPERTY(m_ExpiratoryPeriod);
  INVALIDATE_PROPERTY(m_InspiratoryPeriod);
}

void SEBagValveMaskSqueeze::Copy(const SEBagValveMaskSqueeze& src, const SESubstanceManager& subMgr, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEBagValveMaskSqueeze::IsValid() const
{
  return SEBagValveMaskAction::IsValid() &&
    (HasPositiveInspiratoryPressure() || HasTidalVolume()) &&
    HasExpiratoryPeriod() && HasInspiratoryPeriod();
}

bool SEBagValveMaskSqueeze::IsActive() const
{
  return SEBagValveMaskAction::IsActive();
}
void SEBagValveMaskSqueeze::Deactivate()
{
  SEBagValveMaskAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEBagValveMaskSqueeze::GetScalar(const std::string& name)
{
  if (name == "PositiveInspiratoryPressure")
    return &GetPositiveInspiratoryPressure();
  if (name == "TidalVolume")
    return &GetTidalVolume();
  if (name == "ExpiratoryPeriod")
    return &GetExpiratoryPeriod();
  if (name == "InspiratoryPeriod")
    return &GetInspiratoryPeriod();
  return nullptr;
}

void SEBagValveMaskSqueeze::ToString(std::ostream &str) const
{
  str << "Mechanical Ventilator Squeeze";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tPositiveInspiratoryPressure: ";HasPositiveInspiratoryPressure() ? str << *m_PositiveInspiratoryPressure : str << "NaN";
  str << "\n\tTidalVolume: "; HasTidalVolume() ? str << *m_TidalVolume : str << "NaN";
  str << "\n\tExpiratoryPeriod: "; HasExpiratoryPeriod() ? str << *m_ExpiratoryPeriod : str << "NaN";
  str << "\n\tInspiratoryPeriod: "; HasInspiratoryPeriod() ? str << *m_InspiratoryPeriod : str << "NaN";
  str << std::flush;
}
