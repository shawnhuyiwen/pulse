/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SECondition.h"
#include "engine/SEEngineConfiguration.h"
#include "patient/SEPatient.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBEngine.h"

SEPatientConfiguration::SEPatientConfiguration(Logger* logger) : Loggable(logger)
{
  m_Patient = nullptr;
  Clear();
}

SEPatientConfiguration::~SEPatientConfiguration()
{
  Clear();
}

bool SEPatientConfiguration::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBEngine::SerializeToString(*this, output, m);
}
bool SEPatientConfiguration::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBEngine::SerializeToFile(*this, filename, m);
}
bool SEPatientConfiguration::SerializeFromString(const std::string& src, SerializationFormat m, SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromString(src, *this, m, subMgr);
}
bool SEPatientConfiguration::SerializeFromFile(const std::string& filename, SerializationFormat m, SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromFile(filename, *this, m, subMgr);
}

void SEPatientConfiguration::Clear()
{
  m_PatientFile = "";
  SAFE_DELETE(m_Patient);
  DELETE_VECTOR(m_Conditions);
}

bool SEPatientConfiguration::IsValid() const
{
  if(!HasPatientFile() && HasPatient())
    return false;
  return true;
}


std::string SEPatientConfiguration::GetPatientFile() const
{
  return m_PatientFile;
}
void SEPatientConfiguration::SetPatientFile(const std::string& patientFile)
{
  InvalidatePatient();
  m_PatientFile = patientFile;
}
bool SEPatientConfiguration::HasPatientFile() const
{
  return m_PatientFile.empty()?false:true;
}
void SEPatientConfiguration::InvalidatePatientFile()
{
  m_PatientFile = "";
}

SEPatient& SEPatientConfiguration::GetPatient()
{
  InvalidatePatientFile(); 
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
void SEPatientConfiguration::InvalidatePatient()
{
    SAFE_DELETE(m_Patient);
}

std::vector<SECondition*>& SEPatientConfiguration::GetConditions()
{
  return m_Conditions;
}

const std::vector<const SECondition*>& SEPatientConfiguration::GetConditions() const
{
  m_cConditions.clear();
  for (SECondition* c : m_Conditions)
    m_cConditions.push_back(c);
  return m_cConditions;
}
