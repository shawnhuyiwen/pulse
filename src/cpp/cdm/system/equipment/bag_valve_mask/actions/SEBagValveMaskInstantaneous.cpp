/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskInstantaneous.h"
#include "system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolumePerTime.h"
#include "io/protobuf/PBEquipmentActions.h"

SEBagValveMaskInstantaneous::SEBagValveMaskInstantaneous(Logger* logger) : SEBagValveMaskAction(logger)
{
  m_Flow = nullptr;
  m_Pressure = nullptr;
}

SEBagValveMaskInstantaneous::~SEBagValveMaskInstantaneous()
{
  SAFE_DELETE(m_Flow);
  SAFE_DELETE(m_Pressure);
}

void SEBagValveMaskInstantaneous::Clear()
{
  SEBagValveMaskAction::Clear();
  INVALIDATE_PROPERTY(m_Flow);
  INVALIDATE_PROPERTY(m_Pressure);
}

void SEBagValveMaskInstantaneous::Copy(const SEBagValveMaskInstantaneous& src, const SESubstanceManager& subMgr, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEBagValveMaskInstantaneous::IsValid() const
{
  return SEBagValveMaskAction::IsValid() && (HasPressure() || HasFlow());
}

bool SEBagValveMaskInstantaneous::IsActive() const
{
  return SEBagValveMaskAction::IsActive();
}
void SEBagValveMaskInstantaneous::Deactivate()
{
  SEBagValveMaskAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEBagValveMaskInstantaneous::GetScalar(const std::string& name)
{
  if (name == "Flow")
    return &GetFlow();
  if (name == "Pressure")
    return &GetPressure();
  return nullptr;
}

bool SEBagValveMaskInstantaneous::HasFlow() const
{
  return m_Flow == nullptr ? false : m_Flow->IsValid();
}
SEScalarVolumePerTime& SEBagValveMaskInstantaneous::GetFlow()
{
  if (m_Flow == nullptr)
    m_Flow = new SEScalarVolumePerTime();
  return *m_Flow;
}
double SEBagValveMaskInstantaneous::GetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Flow == nullptr)
    return SEScalar::dNaN();
  return m_Flow->GetValue(unit);
}

bool SEBagValveMaskInstantaneous::HasPressure() const
{
  return m_Pressure == nullptr ? false : m_Pressure->IsValid();
}
SEScalarPressure& SEBagValveMaskInstantaneous::GetPressure()
{
  if (m_Pressure == nullptr)
    m_Pressure = new SEScalarPressure();
  return *m_Pressure;
}
double SEBagValveMaskInstantaneous::GetPressure(const PressureUnit& unit) const
{
  if (m_Pressure == nullptr)
    return SEScalar::dNaN();
  return m_Pressure->GetValue(unit);
}

void SEBagValveMaskInstantaneous::ToString(std::ostream &str) const
{
  str << "Bag Valve Mask Instantaneous";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tFlow: "; HasFlow() ? str << *m_Flow : str << "NaN";
  str << "\n\tPressure: "; HasPressure() ? str << *m_Pressure : str << "NaN";
  str << std::flush;
}
