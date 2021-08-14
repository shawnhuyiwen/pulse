/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDataModel.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAutomated.h"
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEBagValveMaskAutomated::SEBagValveMaskAutomated(Logger* logger) : SEBagValveMaskAction(logger)
{
  m_BreathFrequency = nullptr;
  m_InspiratoryExpiratoryRatio = nullptr;
  m_SqueezePressure = nullptr;
  m_SqueezeVolume = nullptr;
}

SEBagValveMaskAutomated::~SEBagValveMaskAutomated()
{
  SAFE_DELETE(m_BreathFrequency);
  SAFE_DELETE(m_InspiratoryExpiratoryRatio);
  SAFE_DELETE(m_SqueezePressure);
  SAFE_DELETE(m_SqueezeVolume);
}

void SEBagValveMaskAutomated::Clear()
{
  SEBagValveMaskAction::Clear();

  INVALIDATE_PROPERTY(m_BreathFrequency);
  INVALIDATE_PROPERTY(m_InspiratoryExpiratoryRatio);
  INVALIDATE_PROPERTY(m_SqueezePressure);
  INVALIDATE_PROPERTY(m_SqueezeVolume);
}

void SEBagValveMaskAutomated::Copy(const SEBagValveMaskAutomated& src)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEBagValveMaskAutomated::IsValid() const
{
  return SEBagValveMaskAction::IsValid();
}

bool SEBagValveMaskAutomated::IsActive() const
{
  return SEBagValveMaskAction::IsActive();
}
void SEBagValveMaskAutomated::Deactivate()
{
  SEBagValveMaskAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEBagValveMaskAutomated::GetScalar(const std::string& name)
{
  if (name == "BreathFrequency")
    return &GetBreathFrequency();
  if (name == "InspiratoryExpiratoryRatio")
    return &GetInspiratoryExpiratoryRatio();
  if (name == "SqueezePressure")
    return &GetSqueezePressure();
  if (name == "SqueezeVolume")
    return &GetSqueezeVolume();
  return nullptr;
}

bool SEBagValveMaskAutomated::HasBreathFrequency() const
{
  return m_BreathFrequency == nullptr ? false : m_BreathFrequency->IsValid();
}
SEScalarFrequency& SEBagValveMaskAutomated::GetBreathFrequency()
{
  if (m_BreathFrequency == nullptr)
    m_BreathFrequency = new SEScalarFrequency();
  return *m_BreathFrequency;
}
double SEBagValveMaskAutomated::GetBreathFrequency(const FrequencyUnit& unit) const
{
  if (m_BreathFrequency == nullptr)
    return SEScalar::dNaN();
  return m_BreathFrequency->GetValue(unit);
}

bool SEBagValveMaskAutomated::HasInspiratoryExpiratoryRatio() const
{
  return m_InspiratoryExpiratoryRatio == nullptr ? false : m_InspiratoryExpiratoryRatio->IsValid();
}
SEScalar& SEBagValveMaskAutomated::GetInspiratoryExpiratoryRatio()
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    m_InspiratoryExpiratoryRatio = new SEScalar();
  return *m_InspiratoryExpiratoryRatio;
}
double SEBagValveMaskAutomated::GetInspiratoryExpiratoryRatio() const
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryExpiratoryRatio->GetValue();
}

bool SEBagValveMaskAutomated::HasSqueezePressure() const
{
  return m_SqueezePressure == nullptr ? false : m_SqueezePressure->IsValid();
}
SEScalarPressure& SEBagValveMaskAutomated::GetSqueezePressure()
{
  if (m_SqueezePressure == nullptr)
    m_SqueezePressure = new SEScalarPressure();
  return *m_SqueezePressure;
}
double SEBagValveMaskAutomated::GetSqueezePressure(const PressureUnit& unit) const
{
  if (m_SqueezePressure == nullptr)
    return SEScalar::dNaN();
  return m_SqueezePressure->GetValue(unit);
}

bool SEBagValveMaskAutomated::HasSqueezeVolume() const
{
  return m_SqueezeVolume == nullptr ? false : m_SqueezeVolume->IsValid();
}
SEScalarVolume& SEBagValveMaskAutomated::GetSqueezeVolume()
{
  if (m_SqueezeVolume == nullptr)
    m_SqueezeVolume = new SEScalarVolume();
  return *m_SqueezeVolume;
}
double SEBagValveMaskAutomated::GetSqueezeVolume(const VolumeUnit& unit) const
{
  if (m_SqueezeVolume == nullptr)
    return SEScalar::dNaN();
  return m_SqueezeVolume->GetValue(unit);
}

void SEBagValveMaskAutomated::ToString(std::ostream &str) const
{
  str << "Bag Valve Mask Automated";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tBreathFrequency: "; HasBreathFrequency() ? str << *m_BreathFrequency : str << "NaN";
  str << "\n\tInspiratoryExpiratoryRatio: "; HasInspiratoryExpiratoryRatio() ? str << *m_InspiratoryExpiratoryRatio : str << "NaN";
  str << "\n\tSqueezePressure: ";HasSqueezePressure() ? str << *m_SqueezePressure : str << "NaN";
  str << "\n\tSqueezeVolume: "; HasSqueezeVolume() ? str << *m_SqueezeVolume : str << "NaN";
  str << std::flush;
}
