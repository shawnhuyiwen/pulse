/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar0To1.h"
#include "substance/SESubstance.h"

SESubstanceFraction::SESubstanceFraction(SESubstance& substance) : m_Substance(substance)
{
  m_FractionAmount = nullptr;
}

SESubstanceFraction::~SESubstanceFraction()
{
  Clear();
}

void SESubstanceFraction::Clear()
{
  SAFE_DELETE(m_FractionAmount);
}

bool SESubstanceFraction::HasFractionAmount() const
{
  return m_FractionAmount==nullptr?false:m_FractionAmount->IsValid();
}
SEScalar0To1& SESubstanceFraction::GetFractionAmount()
{
  if(m_FractionAmount==nullptr)
    m_FractionAmount = new SEScalar0To1();
  return *m_FractionAmount;
}
double SESubstanceFraction::GetFractionAmount() const
{
  if (m_FractionAmount == nullptr)
    SEScalar::dNaN();
  return m_FractionAmount->GetValue();
}

SESubstance& SESubstanceFraction::GetSubstance() const
{
  return m_Substance;
}