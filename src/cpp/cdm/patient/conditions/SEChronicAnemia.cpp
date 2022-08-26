/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SEChronicAnemia.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SEChronicAnemia::SEChronicAnemia(Logger* logger) : SEPatientCondition(logger)
{
  m_ReductionFactor=nullptr;
}

SEChronicAnemia::~SEChronicAnemia()
{
  SAFE_DELETE(m_ReductionFactor);
}

void SEChronicAnemia::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_ReductionFactor);
}

void SEChronicAnemia::Copy(const SEChronicAnemia& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEChronicAnemia::IsValid() const
{
  return HasReductionFactor();
}
bool SEChronicAnemia::IsActive() const
{
  if (!IsValid())
    return false;
  return GetReductionFactor() > 0;
}

bool SEChronicAnemia::HasReductionFactor() const
{
  return m_ReductionFactor==nullptr?false:m_ReductionFactor->IsValid();
}

SEScalar0To1& SEChronicAnemia::GetReductionFactor()
{
  if(m_ReductionFactor==nullptr)
    m_ReductionFactor=new SEScalar0To1();
  return *m_ReductionFactor;
}
double SEChronicAnemia::GetReductionFactor() const
{
  if (m_ReductionFactor == nullptr)
    return SEScalar::dNaN();
  return m_ReductionFactor->GetValue();
}
