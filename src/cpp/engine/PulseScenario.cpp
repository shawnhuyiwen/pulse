/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseScenario.h"
#include "engine/io/protobuf/PBScenario.h"

PulseScenario::PulseScenario(std::string const& dataDir) : PulseScenario(nullptr, dataDir)
{
  m_Configuration = nullptr;
}
PulseScenario::PulseScenario(Logger* logger, std::string const& dataDir) : SEScenario(logger, dataDir)
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
  pulse::PBScenario::Copy(src, *this);
}

bool PulseScenario::SerializeToString(std::string& output, eSerializationFormat m) const
{
  return pulse::PBScenario::SerializeToString(*this, output, m);
}
bool PulseScenario::SerializeToFile(const std::string& filename) const
{
  return pulse::PBScenario::SerializeToFile(*this, filename);
}
bool PulseScenario::SerializeFromString(const std::string& src, eSerializationFormat m)
{
  return pulse::PBScenario::SerializeFromString(src, *this, m);
}
bool PulseScenario::SerializeFromFile(const std::string& filename)
{
  return pulse::PBScenario::SerializeFromFile(filename, *this);
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
