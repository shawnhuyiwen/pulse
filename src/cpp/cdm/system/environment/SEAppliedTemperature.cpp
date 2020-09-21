/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/environment/SEAppliedTemperature.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarTemperature.h"

SEAppliedTemperature::SEAppliedTemperature(Logger* logger) : Loggable(logger)
{
  m_Temperature = nullptr;
  m_SurfaceArea = nullptr;
  m_SurfaceAreaFraction = nullptr;
  m_State = eSwitch::Off;
}

SEAppliedTemperature::~SEAppliedTemperature()
{
  m_State = eSwitch::Off;
  SAFE_DELETE(m_Temperature);
  SAFE_DELETE(m_SurfaceArea);
  SAFE_DELETE(m_SurfaceAreaFraction);
}

void SEAppliedTemperature::Clear()
{
  m_State = eSwitch::Off;
  INVALIDATE_PROPERTY(m_Temperature);
  INVALIDATE_PROPERTY(m_SurfaceArea);
  INVALIDATE_PROPERTY(m_SurfaceAreaFraction);
}

const SEScalar* SEAppliedTemperature::GetScalar(const std::string& name)
{
  if (name.compare("Temperature") == 0)
    return &GetTemperature();
  if (name.compare("SurfaceArea") == 0)
    return &GetSurfaceArea();
  if (name.compare("SurfaceAreaFraction") == 0)
    return &GetSurfaceAreaFraction();
  return nullptr;
}

bool SEAppliedTemperature::HasTemperature() const
{
  return m_Temperature == nullptr ? false : m_Temperature->IsValid();
}
SEScalarTemperature& SEAppliedTemperature::GetTemperature()
{
  if (m_Temperature == nullptr)
    m_Temperature = new SEScalarTemperature();
  return *m_Temperature;
}
double SEAppliedTemperature::GetTemperature(const TemperatureUnit& unit) const
{
  if (m_Temperature == nullptr)
    return SEScalar::dNaN();
  return m_Temperature->GetValue(unit);
}

bool SEAppliedTemperature::HasSurfaceArea() const
{
  return m_SurfaceArea == nullptr ? false : m_SurfaceArea->IsValid();
}
SEScalarArea& SEAppliedTemperature::GetSurfaceArea()
{
  if (m_SurfaceArea == nullptr)
    m_SurfaceArea = new SEScalarArea();
  return *m_SurfaceArea;
}
double SEAppliedTemperature::GetSurfaceArea(const AreaUnit& unit) const
{
  if (m_SurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_SurfaceArea->GetValue(unit);
}

bool SEAppliedTemperature::HasSurfaceAreaFraction() const
{
  return m_SurfaceAreaFraction == nullptr ? false : m_SurfaceAreaFraction->IsValid();
}
SEScalar0To1& SEAppliedTemperature::GetSurfaceAreaFraction()
{
  if (m_SurfaceAreaFraction == nullptr)
    m_SurfaceAreaFraction = new SEScalar0To1();
  return *m_SurfaceAreaFraction;
}
double SEAppliedTemperature::GetSurfaceAreaFraction() const
{
  if (m_SurfaceAreaFraction == nullptr)
    return SEScalar::dNaN();
  return m_SurfaceAreaFraction->GetValue();
}

eSwitch SEAppliedTemperature::GetState() const
{
  return m_State;
}
void SEAppliedTemperature::SetState(eSwitch state)
{
  m_State = (state == eSwitch::NullSwitch) ? eSwitch::Off : state;
}



void SEAppliedTemperature::ToString(std::ostream &str) const
{
  str << "Applied Temperature :";
  str << "\n\tTemperature :";         HasTemperature() ? str << *m_Temperature : str << "NaN";
  str << "\n\tSurfaceArea :";         HasSurfaceArea() ? str << *m_SurfaceArea : str << "NaN";
  str << "\n\tSurfaceAreaFraction :"; HasSurfaceAreaFraction() ? str << *m_SurfaceAreaFraction : str << "NaN";
  str << "\n\tState :" <<  eSwitch_Name(m_State);
  str << std::flush;
}