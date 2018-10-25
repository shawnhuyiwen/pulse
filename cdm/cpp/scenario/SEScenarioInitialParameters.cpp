/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEScenarioInitialParameters.h"
#include "engine/SEEngineConfiguration.h"
#include "patient/SEPatient.h"
#include "scenario/SECondition.h"
#include "substance/SESubstanceManager.h"

SEScenarioInitialParameters::SEScenarioInitialParameters(SESubstanceManager& subMgr) : Loggable(subMgr.GetLogger()), m_SubMgr(subMgr)
{
  m_Patient = nullptr;
  Clear();
}

SEScenarioInitialParameters::~SEScenarioInitialParameters()
{
  Clear();
}

void SEScenarioInitialParameters::Clear()
{
  m_PatientFile = "";
  SAFE_DELETE(m_Patient);
  DELETE_VECTOR(m_Conditions);
}

bool SEScenarioInitialParameters::IsValid() const
{
  if(!HasPatientFile() && HasPatient())
    return false;
  return true;
}


std::string SEScenarioInitialParameters::GetPatientFile() const
{
  return m_PatientFile;
}
void SEScenarioInitialParameters::SetPatientFile(const std::string& patientFile)
{
  InvalidatePatient();
  m_PatientFile = patientFile;
}
bool SEScenarioInitialParameters::HasPatientFile() const
{
  return m_PatientFile.empty()?false:true;
}
void SEScenarioInitialParameters::InvalidatePatientFile()
{
  m_PatientFile = "";
}

SEPatient& SEScenarioInitialParameters::GetPatient()
{
  InvalidatePatientFile(); 
  if (m_Patient == nullptr)
    m_Patient = new SEPatient(GetLogger());
  return *m_Patient;
}
const SEPatient* SEScenarioInitialParameters::GetPatient() const 
{
  return m_Patient;
}
bool SEScenarioInitialParameters::HasPatient() const
{
  return m_Patient != nullptr;
}
void SEScenarioInitialParameters::InvalidatePatient()
{
    SAFE_DELETE(m_Patient);
}

const std::vector<SECondition*>& SEScenarioInitialParameters::GetConditions() const
{
  return m_Conditions;
}
