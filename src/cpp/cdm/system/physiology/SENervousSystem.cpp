/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/physiology/SENervousSystem.h"
#include "system/physiology/SEPupillaryResponse.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarLength.h"

SENervousSystem::SENervousSystem(Logger* logger) : SESystem(logger)
{
  m_ChemoreceptorFeedback = eSwitch::NullSwitch;
  m_ChemoreceptorHeartRateScale = nullptr;
  m_ChemoreceptorHeartElastanceScale = nullptr;
  m_BaroreceptorFeedback = eSwitch::NullSwitch;
  m_BaroreceptorHeartRateScale = nullptr;
  m_BaroreceptorHeartElastanceScale = nullptr;
  m_BaroreceptorResistanceScale = nullptr;
  m_BaroreceptorComplianceScale = nullptr;
  m_LeftEyePupillaryResponse = nullptr;
  m_RightEyePupillaryResponse = nullptr;
}

SENervousSystem::~SENervousSystem()
{
  m_BaroreceptorFeedback = eSwitch::NullSwitch;
  SAFE_DELETE(m_BaroreceptorHeartRateScale);
  SAFE_DELETE(m_BaroreceptorHeartElastanceScale);
  SAFE_DELETE(m_BaroreceptorResistanceScale);
  SAFE_DELETE(m_BaroreceptorComplianceScale);
  m_ChemoreceptorFeedback = eSwitch::NullSwitch;
  SAFE_DELETE(m_ChemoreceptorHeartRateScale);
  SAFE_DELETE(m_ChemoreceptorHeartElastanceScale);
  SAFE_DELETE(m_LeftEyePupillaryResponse);
  SAFE_DELETE(m_RightEyePupillaryResponse);
}

void SENervousSystem::Clear()
{
  SESystem::Clear();
  m_BaroreceptorFeedback = eSwitch::NullSwitch;
  INVALIDATE_PROPERTY(m_BaroreceptorHeartRateScale);
  INVALIDATE_PROPERTY(m_BaroreceptorHeartElastanceScale);
  INVALIDATE_PROPERTY(m_BaroreceptorResistanceScale);
  INVALIDATE_PROPERTY(m_BaroreceptorComplianceScale);
  m_ChemoreceptorFeedback = eSwitch::NullSwitch;
  INVALIDATE_PROPERTY(m_ChemoreceptorHeartRateScale);
  INVALIDATE_PROPERTY(m_ChemoreceptorHeartElastanceScale);
  if (m_LeftEyePupillaryResponse)
    m_LeftEyePupillaryResponse->Clear();
  if (m_RightEyePupillaryResponse)
    m_RightEyePupillaryResponse->Clear();
}

const SEScalar* SENervousSystem::GetScalar(const std::string& name)
{
  if (name.compare("BaroreceptorHeartRateScale") == 0)
    return &GetBaroreceptorHeartRateScale();
  if (name.compare("BaroreceptorHeartElastanceScale") == 0)
    return &GetBaroreceptorHeartElastanceScale();
  if (name.compare("BaroreceptorResistanceScale") == 0)
    return &GetBaroreceptorResistanceScale();
  if (name.compare("BaroreceptorComplianceScale") == 0)
    return &GetBaroreceptorComplianceScale();
  if (name.compare("ChemoreceptorHeartRateScale") == 0)
    return &GetChemoreceptorHeartRateScale();
  if (name.compare("ChemoreceptorHeartElastanceScale") == 0)
    return &GetChemoreceptorHeartElastanceScale();

  size_t split = name.find('-');
  if (split != name.npos)
  {
    std::string child = name.substr(0, split);
    std::string prop  = name.substr(split+1, name.npos);
    if (child == "LeftEyePupillaryResponse")
      return GetLeftEyePupillaryResponse().GetScalar(prop);
    if (child == "RightEyePupillaryResponse")
      return GetRightEyePupillaryResponse().GetScalar(prop);
  }
  return nullptr;
}

eSwitch SENervousSystem::GetBaroreceptorFeedback() const
{
  return m_BaroreceptorFeedback;
}
void SENervousSystem::SetBaroreceptorFeedback(eSwitch s)
{
  m_BaroreceptorFeedback = (s == eSwitch::NullSwitch) ? eSwitch::On : s;
}

bool SENervousSystem::HasBaroreceptorHeartRateScale() const
{
  return m_BaroreceptorHeartRateScale == nullptr ? false : m_BaroreceptorHeartRateScale->IsValid();
}
SEScalar& SENervousSystem::GetBaroreceptorHeartRateScale()
{
  if (m_BaroreceptorHeartRateScale == nullptr)
    m_BaroreceptorHeartRateScale = new SEScalar();
  return *m_BaroreceptorHeartRateScale;
}
double SENervousSystem::GetBaroreceptorHeartRateScale() const
{
  if (m_BaroreceptorHeartRateScale == nullptr)
    return SEScalar::dNaN();
  return m_BaroreceptorHeartRateScale->GetValue();
}

bool SENervousSystem::HasBaroreceptorHeartElastanceScale() const
{
  return m_BaroreceptorHeartElastanceScale == nullptr ? false : m_BaroreceptorHeartElastanceScale->IsValid();
}
SEScalar& SENervousSystem::GetBaroreceptorHeartElastanceScale()
{
  if (m_BaroreceptorHeartElastanceScale == nullptr)
    m_BaroreceptorHeartElastanceScale = new SEScalar();
  return *m_BaroreceptorHeartElastanceScale;
}
double SENervousSystem::GetBaroreceptorHeartElastanceScale() const
{
  if (m_BaroreceptorHeartElastanceScale == nullptr)
    return SEScalar::dNaN();
  return m_BaroreceptorHeartElastanceScale->GetValue();
}

bool SENervousSystem::HasBaroreceptorResistanceScale() const
{
  return m_BaroreceptorResistanceScale == nullptr ? false : m_BaroreceptorResistanceScale->IsValid();
}
SEScalar& SENervousSystem::GetBaroreceptorResistanceScale()
{
  if (m_BaroreceptorResistanceScale == nullptr)
    m_BaroreceptorResistanceScale = new SEScalar();
  return *m_BaroreceptorResistanceScale;
}
double SENervousSystem::GetBaroreceptorResistanceScale() const
{
  if (m_BaroreceptorResistanceScale == nullptr)
    return SEScalar::dNaN();
  return m_BaroreceptorResistanceScale->GetValue();
}

bool SENervousSystem::HasBaroreceptorComplianceScale() const
{
  return m_BaroreceptorComplianceScale == nullptr ? false : m_BaroreceptorComplianceScale->IsValid();
}
SEScalar& SENervousSystem::GetBaroreceptorComplianceScale()
{
  if (m_BaroreceptorComplianceScale == nullptr)
    m_BaroreceptorComplianceScale = new SEScalar();
  return *m_BaroreceptorComplianceScale;
}
double SENervousSystem::GetBaroreceptorComplianceScale() const
{
  if (m_BaroreceptorComplianceScale == nullptr)
    return SEScalar::dNaN();
  return m_BaroreceptorComplianceScale->GetValue();
}

eSwitch SENervousSystem::GetChemoreceptorFeedback() const
{
  return m_ChemoreceptorFeedback;
}
void SENervousSystem::SetChemoreceptorFeedback(eSwitch s)
{
  m_ChemoreceptorFeedback = (s == eSwitch::NullSwitch) ? eSwitch::On : s;
}

bool SENervousSystem::HasChemoreceptorHeartRateScale() const
{
  return m_ChemoreceptorHeartRateScale == nullptr ? false : m_ChemoreceptorHeartRateScale->IsValid();
}
SEScalar& SENervousSystem::GetChemoreceptorHeartRateScale()
{
  if (m_ChemoreceptorHeartRateScale == nullptr)
    m_ChemoreceptorHeartRateScale = new SEScalar();
  return *m_ChemoreceptorHeartRateScale;
}
double SENervousSystem::GetChemoreceptorHeartRateScale() const
{
  if (m_ChemoreceptorHeartRateScale == nullptr)
    return SEScalar::dNaN();
  return m_ChemoreceptorHeartRateScale->GetValue();
}

bool SENervousSystem::HasChemoreceptorHeartElastanceScale() const
{
  return m_ChemoreceptorHeartElastanceScale == nullptr ? false : m_ChemoreceptorHeartElastanceScale->IsValid();
}
SEScalar& SENervousSystem::GetChemoreceptorHeartElastanceScale()
{
  if (m_ChemoreceptorHeartElastanceScale == nullptr)
    m_ChemoreceptorHeartElastanceScale = new SEScalar();
  return *m_ChemoreceptorHeartElastanceScale;
}
double SENervousSystem::GetChemoreceptorHeartElastanceScale() const
{
  if (m_ChemoreceptorHeartElastanceScale == nullptr)
    return SEScalar::dNaN();
  return m_ChemoreceptorHeartElastanceScale->GetValue();
}
bool SENervousSystem::HasLeftEyePupillaryResponse() const
{
  return (m_LeftEyePupillaryResponse != nullptr);
}
SEPupillaryResponse& SENervousSystem::GetLeftEyePupillaryResponse()
{
  if (m_LeftEyePupillaryResponse == nullptr)
    m_LeftEyePupillaryResponse = new SEPupillaryResponse(GetLogger());
  return *m_LeftEyePupillaryResponse;
}
const SEPupillaryResponse* SENervousSystem::GetLeftEyePupillaryResponse() const
{
  return m_LeftEyePupillaryResponse;
}
void SENervousSystem::RemoveLeftEyePupillaryResponse()
{
  SAFE_DELETE(m_LeftEyePupillaryResponse);
}

bool SENervousSystem::HasRightEyePupillaryResponse() const
{
  return (m_RightEyePupillaryResponse != nullptr);
}
SEPupillaryResponse& SENervousSystem::GetRightEyePupillaryResponse()
{
  if (m_RightEyePupillaryResponse == nullptr)
    m_RightEyePupillaryResponse = new SEPupillaryResponse(GetLogger());
  return *m_RightEyePupillaryResponse;
}
const SEPupillaryResponse* SENervousSystem::GetRightEyePupillaryResponse() const
{
  return m_RightEyePupillaryResponse;
}
void SENervousSystem::RemoveRightEyePupillaryResponse()
{
  SAFE_DELETE(m_RightEyePupillaryResponse);
}