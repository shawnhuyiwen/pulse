/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/environment/actions/SEThermalApplication.h"
#include "system/environment/SEActiveConditioning.h"
#include "system/environment/SEAppliedTemperature.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarTemperature.h"
#include "io/protobuf/PBEnvironmentActions.h"

SEThermalApplication::SEThermalApplication(Logger* logger) : SEEnvironmentAction(logger)
{
  m_ClearContents = true;
  m_ActiveHeating = nullptr;
  m_ActiveCooling = nullptr;
  m_AppliedTemperature = nullptr;
}

SEThermalApplication::~SEThermalApplication()
{
  SAFE_DELETE(m_ActiveHeating);
  SAFE_DELETE(m_ActiveCooling);
  SAFE_DELETE(m_AppliedTemperature);
}

void SEThermalApplication::Clear()
{
  SEEnvironmentAction::Clear();
  if (m_ClearContents)
  {
    if (m_ActiveHeating)
      m_ActiveHeating->Clear();
    if (m_ActiveCooling)
      m_ActiveCooling->Clear();
    if (m_AppliedTemperature)
      m_AppliedTemperature->Clear();
  }
}

void SEThermalApplication::Copy(const SEThermalApplication& src, bool preserveState)
{// Using Bindings to make a copy
  PBEnvironmentAction::Copy(src, *this);
}

bool SEThermalApplication::IsValid() const
{
  return SEEnvironmentAction::IsValid();
}

bool SEThermalApplication::IsActive() const
{
  if (!SEEnvironmentAction::IsActive())
    return false;
  if (HasActiveHeating() && m_ActiveHeating->GetPower().IsPositive())
    return true;
  if (HasActiveCooling() && m_ActiveCooling->GetPower().IsPositive())
    return true;
  if (HasAppliedTemperature() && m_AppliedTemperature->GetState() == eSwitch::On)
    return true;
  return false;
}

void SEThermalApplication::Deactivate()
{
  SEEnvironmentAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEThermalApplication::GetScalar(const std::string& name)
{
  size_t split = name.find('-');
  if (split != name.npos)
  {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == "ActiveHeating")
      return GetActiveHeating().GetScalar(prop);
    if (child == "ActiveCooling")
      return GetActiveCooling().GetScalar(prop);
    if (child == "AppliedTemperature")
      return GetAppliedTemperature().GetScalar(prop);
  }
  return nullptr;
}

bool SEThermalApplication::HasActiveHeating() const
{
  if (m_ActiveHeating == nullptr)
    return false;
  if (!m_ActiveHeating->HasPower() || m_ActiveHeating->GetPower().IsZero())
    return false;
  return true;
}
SEActiveConditioning& SEThermalApplication::GetActiveHeating()
{
  if (m_ActiveHeating == nullptr)
    m_ActiveHeating = new SEActiveConditioning(GetLogger());
  return *m_ActiveHeating;
}
const SEActiveConditioning* SEThermalApplication::GetActiveHeating() const
{
  return m_ActiveHeating;
}
void SEThermalApplication::RemoveActiveHeating()
{
  SAFE_DELETE(m_ActiveHeating);
}

bool SEThermalApplication::HasActiveCooling() const
{
  if (m_ActiveCooling == nullptr)
    return false;
  if (!m_ActiveCooling->HasPower() || m_ActiveCooling->GetPower().IsZero())
    return false;
  return true;
}
SEActiveConditioning& SEThermalApplication::GetActiveCooling()
{
  if (m_ActiveCooling == nullptr)
    m_ActiveCooling = new SEActiveConditioning(GetLogger());
  return *m_ActiveCooling;
}
const SEActiveConditioning* SEThermalApplication::GetActiveCooling() const
{
  return m_ActiveCooling;
}
void SEThermalApplication::RemoveActiveCooling()
{
  SAFE_DELETE(m_ActiveCooling);
}

bool SEThermalApplication::HasAppliedTemperature() const
{
  if (m_AppliedTemperature == nullptr)
    return false;
  if (m_AppliedTemperature->GetState() == eSwitch::Off)
    return false;
  if (!m_AppliedTemperature->HasTemperature())
    return false;
  return true;
}
SEAppliedTemperature& SEThermalApplication::GetAppliedTemperature()
{
  if (m_AppliedTemperature == nullptr)
    m_AppliedTemperature = new SEAppliedTemperature(GetLogger());
  return *m_AppliedTemperature;
}
const SEAppliedTemperature* SEThermalApplication::GetAppliedTemperature() const
{
  return m_AppliedTemperature;
}
void SEThermalApplication::RemoveAppliedTemperature()
{
  SAFE_DELETE(m_AppliedTemperature);
}

void SEThermalApplication::ToString(std::ostream &str) const
{
  str << "Environment Action : Thermal Application";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasActiveHeating())
  {
    str << "\n\t Active Heating : ";
    m_ActiveHeating->ToString(str);
  }
  else
    str << "\n\tNo Active Heating";

  if (HasActiveCooling())
  {
    str << "\n\t Active Cooling : ";
    m_ActiveCooling->ToString(str);
  }
  else
    str << "\n\tNo Active Cooling";

  if (HasAppliedTemperature())
  {
    str << "\n\t";
    m_AppliedTemperature->ToString(str);
  }
  else
    str << "\n\tNo Applied Temperature";
  
  str << std::flush;
}