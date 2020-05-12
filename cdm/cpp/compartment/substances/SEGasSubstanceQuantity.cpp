/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"

#include "compartment/substances/SEGasSubstanceQuantity.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "substance/SESubstanceTransport.h"
#include "substance/SESubstance.h"

#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalar0To1.h"
#include "utils/GeneralMath.h"

SEGasSubstanceQuantity::SEGasSubstanceQuantity(SESubstance& sub, SEGasCompartment& compartment) : SESubstanceQuantity(sub), m_Compartment(compartment)
{
  m_PartialPressure = nullptr;
  m_Volume = nullptr;
  m_VolumeFraction = nullptr;

  if (m_Substance.GetState() != eSubstance_State::Gas)
    Fatal("The substance for a Gas Substance quantity must be a gas");
}

SEGasSubstanceQuantity::~SEGasSubstanceQuantity()
{
  Clear();
}
void SEGasSubstanceQuantity::Invalidate()
{
  if(m_PartialPressure!=nullptr)
    m_PartialPressure->Invalidate();
  if (m_Volume!=nullptr)
    m_Volume->Invalidate();
  if (m_VolumeFraction!=nullptr)
    m_VolumeFraction->Invalidate();
}

void SEGasSubstanceQuantity::Clear()
{
  SAFE_DELETE(m_PartialPressure);
  SAFE_DELETE(m_Volume);
  SAFE_DELETE(m_VolumeFraction);
  m_Children.clear();
}

std::string SEGasSubstanceQuantity::GetCompartmentName()
{
  return m_Compartment.GetName();
}

void SEGasSubstanceQuantity::SetToZero()
{
  auto& pp = GetPartialPressure();
  if (pp.HasUnit())
    pp.SetValue(0, *pp.GetUnit());
  else
    pp.SetValue(0, PressureUnit::mmHg);

  auto& vol = GetVolume();
  if (vol.HasUnit())
    vol.SetValue(0, *vol.GetUnit());
  else
    vol.SetValue(0, VolumeUnit::mL);

  GetVolumeFraction().SetValue(0);
}

const SEScalar* SEGasSubstanceQuantity::GetScalar(const std::string& name)
{
  if (name.compare("PartialPressure") == 0)
    return &GetPartialPressure();
  if (name.compare("Volume") == 0)
    return &GetVolume();
  if (name.compare("VolumeFraction") == 0)
    return &GetVolumeFraction();
  return nullptr;
}

bool SEGasSubstanceQuantity::HasPartialPressure() const
{
  if (!m_Children.empty())
  {
    for (SEGasSubstanceQuantity* child : m_Children)
      if (child->HasPartialPressure())
        return true;
    return false;
  }
  return (m_PartialPressure == nullptr) ? false : m_PartialPressure->IsValid();
}
SEScalarPressure& SEGasSubstanceQuantity::GetPartialPressure()
{
  if (m_PartialPressure == nullptr)
    m_PartialPressure = new SEScalarPressure();
  if (!m_Children.empty())
  {
    m_PartialPressure->SetReadOnly(false);
    if (HasVolumeFraction() && m_Compartment.HasPressure())
    {
      if (!GeneralMath::CalculatePartialPressureInGas(GetVolumeFraction(), m_Compartment.GetPressure(), *m_PartialPressure, m_Logger))
        Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
    }
    else
      m_PartialPressure->Invalidate();
    m_PartialPressure->SetReadOnly(true);
  }
  return *m_PartialPressure;
}
double SEGasSubstanceQuantity::GetPartialPressure(const PressureUnit& unit) const
{
  if (!m_Children.empty())
  {
    if (!HasVolumeFraction() || !m_Compartment.HasPressure())
      return SEScalar::dNaN();
    SEScalar0To1 volFrac;
    SEScalarPressure partialPressure;
    volFrac.SetValue(const_cast<const SEGasSubstanceQuantity*>(this)->GetVolumeFraction());
    if(!GeneralMath::CalculatePartialPressureInGas(volFrac, m_Compartment.GetPressure(), partialPressure, m_Logger))
      Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
    return partialPressure.GetValue(unit);
  }
  if (m_PartialPressure == nullptr)
    return SEScalar::dNaN();  
  return m_PartialPressure->GetValue(unit);
}

bool SEGasSubstanceQuantity::HasVolume() const
{
  if (!m_Children.empty())
  {
    for (SEGasSubstanceQuantity* child : m_Children)
      if (child->HasVolume())
        return true;
    return false;
  }
  return (m_Volume == nullptr) ? false : m_Volume->IsValid();
}
SEScalarVolume& SEGasSubstanceQuantity::GetVolume()
{
  if (m_Volume == nullptr)
    m_Volume = new SEScalarVolume();
  if (!m_Children.empty())
  {
    m_Volume->SetReadOnly(false);
    m_Volume->Invalidate();
    for (SEGasSubstanceQuantity* child : m_Children)
      if (child->HasVolume())
        m_Volume->Increment(child->GetVolume());
    m_Volume->SetReadOnly(true);
  }
  return *m_Volume;
}
double SEGasSubstanceQuantity::GetVolume(const VolumeUnit& unit) const
{
  if (!m_Children.empty())
  {
    double volume = 0;
    for (SEGasSubstanceQuantity* child : m_Children)
      if (child->HasVolume())
        volume += child->GetVolume(unit);
    return volume;
  }
  if (m_Volume == nullptr)
    return SEScalar::dNaN();
  return m_Volume->GetValue(unit);
}

bool SEGasSubstanceQuantity::HasVolumeFraction() const
{
  if (!m_Children.empty())
  {
    for (SEGasSubstanceQuantity* child : m_Children)
      if (child->HasVolumeFraction())
        return true;
    return false;
  }
  return (m_VolumeFraction == nullptr) ? false : m_VolumeFraction->IsValid();
}
SEScalar0To1& SEGasSubstanceQuantity::GetVolumeFraction()
{
  if (m_VolumeFraction == nullptr)
    m_VolumeFraction = new SEScalar0To1();
  if (!m_Children.empty())
  {
    m_VolumeFraction->SetReadOnly(false);
    if (HasVolume() && m_Compartment.HasVolume())
      m_VolumeFraction->SetValue(GetVolume(VolumeUnit::mL) / m_Compartment.GetVolume(VolumeUnit::mL));
    else
      m_VolumeFraction->Invalidate();
    m_VolumeFraction->SetReadOnly(true);
  }
  return *m_VolumeFraction;
}
double SEGasSubstanceQuantity::GetVolumeFraction() const
{
  if (!m_Children.empty())
  {
    if (!HasVolume() || !m_Compartment.HasVolume())
      return SEScalar::dNaN();
    return GetVolume(VolumeUnit::mL) / m_Compartment.GetVolume(VolumeUnit::mL);
  }
  if (m_VolumeFraction == nullptr)
    return SEScalar::dNaN();
  return m_VolumeFraction->GetValue();
}

void SEGasSubstanceQuantity::AddChild(SEGasSubstanceQuantity& subQ)
{
  if (!Contains(m_Children, subQ))
    m_Children.push_back(&subQ);
}