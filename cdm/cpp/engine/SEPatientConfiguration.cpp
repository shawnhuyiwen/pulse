/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SEConditionManager.h"
#include "engine/SEEngineConfiguration.h"
#include "patient/SEPatient.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBEngine.h"

SEPatientConfiguration::SEPatientConfiguration(SESubstanceManager& subMgr) : Loggable(subMgr.GetLogger()), m_SubMgr(subMgr)
{
  m_Patient = nullptr;
  m_Conditions = nullptr;
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
bool SEPatientConfiguration::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBEngine::SerializeFromString(src, *this, m, m_SubMgr);
}
bool SEPatientConfiguration::SerializeFromFile(const std::string& filename, SerializationFormat m)
{
  return PBEngine::SerializeFromFile(filename, *this, m, m_SubMgr);
}

void SEPatientConfiguration::Clear()
{
  m_PatientFile = "";
  SAFE_DELETE(m_Patient);
  SAFE_DELETE(m_Conditions);
}

bool SEPatientConfiguration::IsValid() const
{
  if(!HasPatientFile() && !HasPatient())
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

SEConditionManager& SEPatientConfiguration::GetConditions()
{
  if (m_Conditions == nullptr)
    m_Conditions = new SEConditionManager(m_SubMgr);
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
void SEPatientConfiguration::InvalidateConditions()
{
  SAFE_DELETE(m_Conditions);
}
