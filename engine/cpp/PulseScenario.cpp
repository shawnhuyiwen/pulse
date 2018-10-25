/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulseScenario.h"
#include "PulseConfiguration.h"
#include "io/protobuf/PBPulse.h"

PulseScenario::PulseScenario(SESubstanceManager& subMgr) : SEScenario(subMgr)
{
  m_Configuration = nullptr;
}

PulseScenario::~PulseScenario()
{
  Clear();
}

void PulseScenario::Clear()
{
  SEScenario::Clear();
  SAFE_DELETE(m_Configuration);
}

bool PulseScenario::SerializeToString(std::string& output, SerializationMode m) const
{
  return PBPulse::SerializeToString(*this, output, m);
}
bool PulseScenario::SerializeToFile(const std::string& filename, SerializationMode m) const
{
  return PBPulse::SerializeToFile(*this, filename, m);
}
bool PulseScenario::SerializeFromString(const std::string& src, SerializationMode m)
{
  return PBPulse::SerializeFromString(src, *this, m);
}
bool PulseScenario::SerializeFromFile(const std::string& filename, SerializationMode m)
{
  return PBPulse::SerializeFromFile(filename, *this, m);
}


bool PulseScenario::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
PulseConfiguration& PulseScenario::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new PulseConfiguration(m_SubMgr);
  return *m_Configuration;
}
const PulseConfiguration* PulseScenario::GetConfiguration() const
{
  return m_Configuration;
}
void PulseScenario::InvalidateConfiguration()
{
  SAFE_DELETE(m_Configuration);
}
