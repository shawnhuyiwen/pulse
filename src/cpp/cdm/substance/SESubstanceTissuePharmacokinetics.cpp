/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstanceTissuePharmacokinetics.h"
#include "properties/SEScalar.h"

SESubstanceTissuePharmacokinetics::SESubstanceTissuePharmacokinetics(const std::string& name, Logger* logger) : Loggable(logger), m_Name(name)
{
  m_PartitionCoefficient = nullptr;
}

SESubstanceTissuePharmacokinetics::~SESubstanceTissuePharmacokinetics()
{

  SAFE_DELETE(m_PartitionCoefficient)
}

void SESubstanceTissuePharmacokinetics::Clear()
{;
  INVALIDATE_PROPERTY(m_PartitionCoefficient)
}

const SEScalar* SESubstanceTissuePharmacokinetics::GetScalar(const std::string& name)
{
  if (name.compare("PartitionCoefficient") == 0)
    return &GetPartitionCoefficient();

  return nullptr;
}

std::string SESubstanceTissuePharmacokinetics::GetName() const
{
  return m_Name;
}

bool SESubstanceTissuePharmacokinetics::HasPartitionCoefficient() const
{
  return (m_PartitionCoefficient == nullptr) ? false : m_PartitionCoefficient->IsValid();
}
SEScalar& SESubstanceTissuePharmacokinetics::GetPartitionCoefficient()
{
  if (m_PartitionCoefficient == nullptr)
    m_PartitionCoefficient = new SEScalar();
  return *m_PartitionCoefficient;
}
double SESubstanceTissuePharmacokinetics::GetPartitionCoefficient() const
{
  if (m_PartitionCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_PartitionCoefficient->GetValue();
}