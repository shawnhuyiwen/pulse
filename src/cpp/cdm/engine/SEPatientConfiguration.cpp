/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SEConditionManager.h"
#include "engine/SEEngineConfiguration.h"
#include "patient/SEPatient.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar.h"
#include "io/protobuf/PBEngine.h"

SEPatientConfiguration::SEPatientConfiguration(Logger* logger) : Loggable(logger)
{
  m_Patient = nullptr;
  m_Conditions = nullptr;
  Clear();
}

SEPatientConfiguration::~SEPatientConfiguration()
{
  m_DataRoot = "./";
  m_PatientFile = "";
  SAFE_DELETE(m_Patient);
  SAFE_DELETE(m_Conditions);
}

bool SEPatientConfiguration::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBEngine::SerializeToString(*this, output, m);
}
bool SEPatientConfiguration::SerializeToFile(const std::string& filename) const
{
  return PBEngine::SerializeToFile(*this, filename);
}
bool SEPatientConfiguration::SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromString(src, *this, m, subMgr);
}
bool SEPatientConfiguration::SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromFile(filename, *this, subMgr);
}

void SEPatientConfiguration::Clear()
{
  m_DataRoot = "./";
  m_PatientFile = "";
  if (m_Patient)
    m_Patient->Clear();
  if (m_Conditions)
    m_Conditions->Clear();
}

bool SEPatientConfiguration::IsValid() const
{
  if(!HasPatientFile() && !HasPatient())
    return false;
  return true;
}

std::string SEPatientConfiguration::GetDataRoot() const
{
  return m_DataRoot;
}
void SEPatientConfiguration::SetDataRoot(const std::string& rootDir)
{
  m_DataRoot = rootDir;
}

std::string SEPatientConfiguration::GetPatientFile() const
{
  return m_PatientFile;
}
void SEPatientConfiguration::SetPatientFile(const std::string& patientFile)
{
  m_PatientFile = patientFile;
}
bool SEPatientConfiguration::HasPatientFile() const
{
  return !m_PatientFile.empty();
}

SEPatient& SEPatientConfiguration::GetPatient()
{
  if (m_Patient == nullptr)
    m_Patient = new SEPatient(GetLogger());
  return *m_Patient;
}
const SEPatient* SEPatientConfiguration::GetPatient() const 
{
  return m_Patient;
}
bool SEPatientConfiguration::HasPatient() const
{
  return m_Patient != nullptr;
}

SEConditionManager& SEPatientConfiguration::GetConditions()
{
  if (m_Conditions == nullptr)
    m_Conditions = new SEConditionManager(GetLogger());
  return *m_Conditions;
}
const SEConditionManager* SEPatientConfiguration::GetConditions() const
{
  return m_Conditions;
}
bool SEPatientConfiguration::HasConditions() const
{
  return m_Conditions != nullptr;
}

bool SEPatientConfiguration::HasOverride() const
{
  return m_ScalarOverrides.size() > 1;
}
void SEPatientConfiguration::AddScalarOverride(const std::string& name, double value)
{
  m_ScalarOverrides.push_back(SEScalarProperty(name, value, ""));
}
void SEPatientConfiguration::AddScalarOverride(const std::string& name, double value, std::string unit)
{
  m_ScalarOverrides.push_back(SEScalarProperty(name, value, unit));
}
void SEPatientConfiguration::AddScalarOverride(const std::string& name, double value, const CCompoundUnit& unit)
{
  m_ScalarOverrides.push_back(SEScalarProperty(name, value, unit.GetString()));
}
std::vector<SEScalarProperty>& SEPatientConfiguration::GetScalarOverrides()
{
  return m_ScalarOverrides;
}
const std::vector<SEScalarProperty>& SEPatientConfiguration::GetScalarOverrides() const
{
  return m_ScalarOverrides;
}
void SEPatientConfiguration::RemoveOverrides()
{
  m_ScalarOverrides.clear();
}
