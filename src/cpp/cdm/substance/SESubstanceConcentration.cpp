/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceConcentration.h"
#include "properties/SEScalarMassPerVolume.h"

SESubstanceConcentration::SESubstanceConcentration(const SESubstance& substance) : Loggable(substance.GetLogger()), m_Substance(substance)
{
  m_Concentration=nullptr;
}

SESubstanceConcentration::~SESubstanceConcentration()
{
  SAFE_DELETE(m_Concentration);
}

void SESubstanceConcentration::Clear()
{
  INVALIDATE_PROPERTY(m_Concentration);
}

bool SESubstanceConcentration::HasConcentration() const
{
  return (m_Concentration==nullptr)?false:m_Concentration->IsValid();
}
SEScalarMassPerVolume& SESubstanceConcentration::GetConcentration()
{
  if(m_Concentration==nullptr)
    m_Concentration=new SEScalarMassPerVolume();
  return *m_Concentration;
}
double SESubstanceConcentration::GetConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_Concentration == nullptr)
    return SEScalar::dNaN();
  return m_Concentration->GetValue(unit);
}

const SESubstance& SESubstanceConcentration::GetSubstance() const
{
  return m_Substance;
}
