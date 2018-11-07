/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SECondition.h"
#include "engine/SEEngineConfiguration.h"
#include "patient/SEPatient.h"
#include "substance/SESubstanceManager.h"

SEPatientConfiguration::SEPatientConfiguration(SESubstanceManager& subMgr) : Loggable(subMgr.GetLogger()), m_SubMgr(subMgr)
{
  m_Patient = nullptr;
  Clear();
}

SEPatientConfiguration::~SEPatientConfiguration()
{
  Clear();
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

const std::vector<SECondition*>& SEPatientConfiguration::GetConditions() const
{
  return m_Conditions;
}
