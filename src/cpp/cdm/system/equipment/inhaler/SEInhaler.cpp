/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/equipment/inhaler/SEInhaler.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
// State Actions
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarVolume.h"

#include "io/protobuf/PBInhaler.h"

SEInhaler::SEInhaler(Logger* logger) : SEEquipment(logger)
{
  m_State = eSwitch::Off;
  m_MeteredDose = nullptr;
  m_NozzleLoss = nullptr;
  m_SpacerVolume = nullptr;
  m_Substance = nullptr;
}

SEInhaler::~SEInhaler()
{
  m_State = eSwitch::Off;
  SAFE_DELETE(m_MeteredDose);
  SAFE_DELETE(m_NozzleLoss);
  SAFE_DELETE(m_SpacerVolume);
  m_Substance = nullptr;
}

void SEInhaler::Clear()
{
  SEEquipment::Clear();

  m_State = eSwitch::Off;
  INVALIDATE_PROPERTY(m_MeteredDose);
  INVALIDATE_PROPERTY(m_NozzleLoss);
  INVALIDATE_PROPERTY(m_SpacerVolume);
  m_Substance = nullptr;
}

bool SEInhaler::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBInhaler::SerializeToString(*this, output, m);
}
bool SEInhaler::SerializeToFile(const std::string& filename) const
{
  return PBInhaler::SerializeToFile(*this, filename);
}
bool SEInhaler::SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBInhaler::SerializeFromString(src, *this, m, subMgr);
}
bool SEInhaler::SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr)
{
  return PBInhaler::SerializeFromFile(filename, *this, subMgr);
}

const SEScalar* SEInhaler::GetScalar(const std::string& name)
{
  if (name.compare("MeteredDose") == 0)
    return &GetMeteredDose();
  if (name.compare("NozzleLoss") == 0)
    return &GetNozzleLoss();
  if (name.compare("SpacerVolume") == 0)
    return &GetSpacerVolume();
  return nullptr;
}

void SEInhaler::Merge(const SEInhaler& from, SESubstanceManager& subMgr)
{
  SetState(from.m_State);
  COPY_PROPERTY(MeteredDose);
  COPY_PROPERTY(NozzleLoss);
  COPY_PROPERTY(SpacerVolume);
  m_Substance = subMgr.GetSubstance(from.m_Substance->GetName());
    if (m_Substance == nullptr)
      Error("Do not have substance : " + from.m_Substance->GetName());
}

void SEInhaler::ProcessConfiguration(SEInhalerConfiguration& config, SESubstanceManager& subMgr)
{
  if (config.HasConfiguration())
    Merge(config.GetConfiguration(), subMgr);
  else if (config.HasConfigurationFile())
  {
    // Update the action with the file contents
    std::string cfg_file = config.GetConfigurationFile();
    if (!config.GetConfiguration().SerializeFromFile(cfg_file, subMgr))
      Error("Unable to load configuration file", "SEInhaler::ProcessConfiguration");
    Merge(config.GetConfiguration(), subMgr);// Preserve our pointers!
  }
  StateChange();
}

eSwitch SEInhaler::GetState() const
{
  return m_State;
}
void SEInhaler::SetState(eSwitch state)
{
  m_State = (state == eSwitch::NullSwitch) ? eSwitch::Off : state;
}

bool SEInhaler::HasMeteredDose() const
{
  return m_MeteredDose == nullptr ? false : m_MeteredDose->IsValid();
}
SEScalarMass& SEInhaler::GetMeteredDose()
{
  if (m_MeteredDose == nullptr)
    m_MeteredDose = new SEScalarMass();
  return *m_MeteredDose;
}
double SEInhaler::GetMeteredDose(const MassUnit& unit) const
{
  if (m_MeteredDose == nullptr)
    return SEScalar::dNaN();
  return m_MeteredDose->GetValue(unit);
}

bool SEInhaler::HasNozzleLoss() const
{
  return m_NozzleLoss == nullptr ? false : m_NozzleLoss->IsValid();
}
SEScalar0To1& SEInhaler::GetNozzleLoss()
{
  if (m_NozzleLoss == nullptr)
    m_NozzleLoss = new SEScalar0To1();
  return *m_NozzleLoss;
}
double SEInhaler::GetNozzleLoss() const
{
  if (m_NozzleLoss == nullptr)
    return SEScalar::dNaN();
  return m_NozzleLoss->GetValue();
}

bool SEInhaler::HasSpacerVolume() const
{
  return m_SpacerVolume == nullptr ? false : m_SpacerVolume->IsValid();
}
SEScalarVolume& SEInhaler::GetSpacerVolume()
{
  if (m_SpacerVolume == nullptr)
    m_SpacerVolume = new SEScalarVolume();
  return *m_SpacerVolume;
}
double SEInhaler::GetSpacerVolume(const VolumeUnit& unit) const
{
  if (m_SpacerVolume == nullptr)
    return SEScalar::dNaN();
  return m_SpacerVolume->GetValue(unit);
}

bool SEInhaler::HasSubstance() const
{
  return m_Substance != nullptr;
}
void SEInhaler::SetSubstance(const SESubstance* sub)
{
  if (!sub->HasAerosolization())
    throw CommonDataModelException("Inhaler substance must have aerosolization data");
  m_Substance = sub;
}
SESubstance* SEInhaler::GetSubstance() const
{
  return const_cast<SESubstance*>(m_Substance);
}
