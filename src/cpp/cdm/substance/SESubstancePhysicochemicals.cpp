/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstancePhysicochemicals.h"
#include "properties/SEScalar0To1.h"

SESubstancePhysicochemicals::SESubstancePhysicochemicals(Logger* logger) : Loggable(logger)
{
  m_AcidDissociationConstant = nullptr;
  m_BindingProtein = eSubstance_BindingProtein::NullBindingProtein;
  m_BloodPlasmaRatio = nullptr;
  m_FractionUnboundInPlasma = nullptr;
  m_IonicState = eSubstance_IonicState::NullIonicState;
  m_LogP = nullptr;
  m_OralAbsorptionRateConstant = nullptr;
}

SESubstancePhysicochemicals::~SESubstancePhysicochemicals()
{
  SAFE_DELETE(m_AcidDissociationConstant);
  m_BindingProtein = eSubstance_BindingProtein::NullBindingProtein;
  SAFE_DELETE(m_BloodPlasmaRatio);
  SAFE_DELETE(m_FractionUnboundInPlasma);
  m_IonicState = eSubstance_IonicState::NullIonicState;
  SAFE_DELETE(m_LogP);
  SAFE_DELETE(m_OralAbsorptionRateConstant);
}

void SESubstancePhysicochemicals::Clear()
{
  INVALIDATE_PROPERTY(m_AcidDissociationConstant);
  m_BindingProtein = eSubstance_BindingProtein::NullBindingProtein;
  INVALIDATE_PROPERTY(m_BloodPlasmaRatio);
  INVALIDATE_PROPERTY(m_FractionUnboundInPlasma);
  m_IonicState = eSubstance_IonicState::NullIonicState;
  INVALIDATE_PROPERTY(m_LogP);
  INVALIDATE_PROPERTY(m_OralAbsorptionRateConstant);
}

bool SESubstancePhysicochemicals::IsValid() const
{
  if (!HasAcidDissociationConstant())
    return false;
  if (!HasBindingProtein())
    return false;
  if (!HasBloodPlasmaRatio())
    return false;
  if (!HasFractionUnboundInPlasma())
    return false;
  if (!HasIonicState())
    return false;
  if (!HasLogP())
    return false; 
  return true;
}

const SEScalar* SESubstancePhysicochemicals::GetScalar(const std::string& name)
{
  if (name.compare("AcidDissociationConstant") == 0)
    return &GetAcidDissociationConstant();
  if (name.compare("BloodPlasmaRatio") == 0)
    return &GetBloodPlasmaRatio();
  if (name.compare("FractionUnboundInPlasma") == 0)
    return &GetFractionUnboundInPlasma();
  if (name.compare("LogP") == 0)
    return &GetLogP();
  if (name.compare("OralAbsorptionRateConstant") == 0)
    return &GetOralAbsorptionRateConstant();
  
  return nullptr;
}

bool SESubstancePhysicochemicals::HasAcidDissociationConstant() const
{
  return (m_AcidDissociationConstant == nullptr) ? false : m_AcidDissociationConstant->IsValid();
}
SEScalar& SESubstancePhysicochemicals::GetAcidDissociationConstant()
{
  if (m_AcidDissociationConstant == nullptr)
    m_AcidDissociationConstant = new SEScalar();
  return *m_AcidDissociationConstant;
}
double SESubstancePhysicochemicals::GetAcidDissociationConstant() const
{
  if (m_AcidDissociationConstant == nullptr)
    return SEScalar::dNaN();
  return m_AcidDissociationConstant->GetValue();
}

eSubstance_BindingProtein SESubstancePhysicochemicals::GetBindingProtein() const
{
  return m_BindingProtein;
}
void SESubstancePhysicochemicals::SetBindingProtein(eSubstance_BindingProtein protein)
{
  m_BindingProtein = protein;
}
bool SESubstancePhysicochemicals::HasBindingProtein() const
{
  return m_BindingProtein == eSubstance_BindingProtein::NullBindingProtein ? false : true;
}
void SESubstancePhysicochemicals::InvalidateBindingProtein()
{
  m_BindingProtein = eSubstance_BindingProtein::NullBindingProtein;
}

bool SESubstancePhysicochemicals::HasBloodPlasmaRatio() const
{
  return (m_BloodPlasmaRatio == nullptr) ? false : m_BloodPlasmaRatio->IsValid();
}
SEScalar& SESubstancePhysicochemicals::GetBloodPlasmaRatio()
{
  if (m_BloodPlasmaRatio == nullptr)
    m_BloodPlasmaRatio = new SEScalar();
  return *m_BloodPlasmaRatio;
}
double SESubstancePhysicochemicals::GetBloodPlasmaRatio() const
{
  if (m_BloodPlasmaRatio == nullptr)
    return SEScalar::dNaN();
  return m_BloodPlasmaRatio->GetValue();
}

bool SESubstancePhysicochemicals::HasFractionUnboundInPlasma() const
{
  return (m_FractionUnboundInPlasma == nullptr) ? false : m_FractionUnboundInPlasma->IsValid();
}
SEScalar0To1& SESubstancePhysicochemicals::GetFractionUnboundInPlasma()
{
  if (m_FractionUnboundInPlasma == nullptr)
    m_FractionUnboundInPlasma = new SEScalar0To1();
  return *m_FractionUnboundInPlasma;
}
double SESubstancePhysicochemicals::GetFractionUnboundInPlasma() const
{
  if (m_FractionUnboundInPlasma == nullptr)
    return SEScalar::dNaN();
  return m_FractionUnboundInPlasma->GetValue();
}


eSubstance_IonicState SESubstancePhysicochemicals::GetIonicState() const
{
  return m_IonicState;
}
void SESubstancePhysicochemicals::SetIonicState(eSubstance_IonicState state)
{
  m_IonicState = state;
}
bool SESubstancePhysicochemicals::HasIonicState() const
{
  return m_IonicState == eSubstance_IonicState::NullIonicState ? false : true;
}
void SESubstancePhysicochemicals::InvalidateIonicState()
{
  m_IonicState = eSubstance_IonicState::NullIonicState;
}

bool SESubstancePhysicochemicals::HasLogP() const
{
  return (m_LogP == nullptr) ? false : m_LogP->IsValid();
}
SEScalar& SESubstancePhysicochemicals::GetLogP()
{
  if (m_LogP == nullptr)
    m_LogP = new SEScalar();
  return *m_LogP;
}
double SESubstancePhysicochemicals::GetLogP() const
{
  if (m_LogP == nullptr)
    return SEScalar::dNaN();
  return m_LogP->GetValue();
}

bool SESubstancePhysicochemicals::HasOralAbsorptionRateConstant() const
{
  return (m_OralAbsorptionRateConstant == nullptr) ? false : m_OralAbsorptionRateConstant->IsValid();
}
SEScalar& SESubstancePhysicochemicals::GetOralAbsorptionRateConstant()
{
  if (m_OralAbsorptionRateConstant == nullptr)
    m_OralAbsorptionRateConstant = new SEScalar();
  return *m_OralAbsorptionRateConstant;
}
double SESubstancePhysicochemicals::GetOralAbsorptionRateConstant() const
{
  if (m_OralAbsorptionRateConstant == nullptr)
    return SEScalar::dNaN();
  return m_OralAbsorptionRateConstant->GetValue();
}
