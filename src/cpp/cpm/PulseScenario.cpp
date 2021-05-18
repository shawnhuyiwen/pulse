/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulseScenario.h"
#include "PulseConfiguration.h"
#include "io/protobuf/PBPulse.h"

PulseScenario::PulseScenario(Logger* logger, std::string const& dataDir) : SEScenario(logger,dataDir)
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

void PulseScenario::Copy(const PulseScenario& src)
{
  PBPulse::Copy(src, *this);
}

bool PulseScenario::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBPulse::SerializeToString(*this, output, m);
}
bool PulseScenario::SerializeToFile(const std::string& filename) const
{
  return PBPulse::SerializeToFile(*this, filename);
}
bool PulseScenario::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBPulse::SerializeFromString(src, *this, m);
}
bool PulseScenario::SerializeFromFile(const std::string& filename)
{
  return PBPulse::SerializeFromFile(filename, *this);
}


bool PulseScenario::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
PulseConfiguration& PulseScenario::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new PulseConfiguration(GetLogger());
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
