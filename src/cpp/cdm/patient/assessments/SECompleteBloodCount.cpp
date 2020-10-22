/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/assessments/SECompleteBloodCount.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarMassPerVolume.h"
#include "io/protobuf/PBPatientAssessments.h"

SECompleteBloodCount::SECompleteBloodCount(Logger* logger) : SEPatientAssessment(logger)
{
  m_Hematocrit = nullptr;
  m_Hemoglobin = nullptr;
  m_PlateletCount = nullptr;
  m_MeanCorpuscularHemoglobin = nullptr;
  m_MeanCorpuscularHemoglobinConcentration = nullptr;
  m_MeanCorpuscularVolume = nullptr;
  m_RedBloodCellCount = nullptr;
  m_WhiteBloodCellCount = nullptr;
}

SECompleteBloodCount::~SECompleteBloodCount()
{
  SAFE_DELETE(m_Hematocrit);
  SAFE_DELETE(m_Hemoglobin);
  SAFE_DELETE(m_PlateletCount);
  SAFE_DELETE(m_MeanCorpuscularHemoglobin);
  SAFE_DELETE(m_MeanCorpuscularHemoglobinConcentration);
  SAFE_DELETE(m_MeanCorpuscularVolume);
  SAFE_DELETE(m_RedBloodCellCount);
  SAFE_DELETE(m_WhiteBloodCellCount);
}

void SECompleteBloodCount::Clear()
{
  SEPatientAssessment::Clear();
  INVALIDATE_PROPERTY(m_Hematocrit);
  INVALIDATE_PROPERTY(m_Hemoglobin);
  INVALIDATE_PROPERTY(m_PlateletCount);
  INVALIDATE_PROPERTY(m_MeanCorpuscularHemoglobin);
  INVALIDATE_PROPERTY(m_MeanCorpuscularHemoglobinConcentration);
  INVALIDATE_PROPERTY(m_MeanCorpuscularVolume);
  INVALIDATE_PROPERTY(m_RedBloodCellCount);
  INVALIDATE_PROPERTY(m_WhiteBloodCellCount);
}

bool SECompleteBloodCount::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBPatientAssessment::SerializeToString(*this, output, m);
}
bool SECompleteBloodCount::SerializeToFile(const std::string& filename) const
{
  return PBPatientAssessment::SerializeToFile(*this, filename);
}

bool SECompleteBloodCount::HasHematocrit() const
{
  return m_Hematocrit == nullptr ? false : m_Hematocrit->IsValid();
}
SEScalar0To1& SECompleteBloodCount::GetHematocrit()
{
  if (m_Hematocrit == nullptr)
    m_Hematocrit = new SEScalar0To1();
  return *m_Hematocrit;
}
double SECompleteBloodCount::GetHematocrit() const
{
  if (!HasHematocrit())
    return SEScalar::dNaN();
  return m_Hematocrit->GetValue();
}

bool SECompleteBloodCount::HasHemoglobin() const
{
  return m_Hemoglobin == nullptr ? false : m_Hemoglobin->IsValid();
}
SEScalarMassPerVolume& SECompleteBloodCount::GetHemoglobin()
{
  if (m_Hemoglobin == nullptr)
    m_Hemoglobin = new SEScalarMassPerVolume();
  return *m_Hemoglobin;
}
double SECompleteBloodCount::GetHemoglobin(const MassPerVolumeUnit& unit) const
{
  if (!HasHemoglobin())
    return SEScalar::dNaN();
  return m_Hemoglobin->GetValue(unit);
}

bool SECompleteBloodCount::HasPlateletCount() const
{
  return m_PlateletCount == nullptr ? false : m_PlateletCount->IsValid();
}
SEScalarAmountPerVolume& SECompleteBloodCount::GetPlateletCount()
{
  if (m_PlateletCount == nullptr)
    m_PlateletCount = new SEScalarAmountPerVolume();
  return *m_PlateletCount;
}
double SECompleteBloodCount::GetPlateletCount(const AmountPerVolumeUnit& unit) const
{
  if (!HasPlateletCount())
    return SEScalar::dNaN();
  return m_PlateletCount->GetValue(unit);
}

bool SECompleteBloodCount::HasMeanCorpuscularHemoglobin() const
{
  return m_MeanCorpuscularHemoglobin == nullptr ? false : m_MeanCorpuscularHemoglobin->IsValid();
}
SEScalarMassPerAmount& SECompleteBloodCount::GetMeanCorpuscularHemoglobin()
{
  if (m_MeanCorpuscularHemoglobin == nullptr)
    m_MeanCorpuscularHemoglobin = new SEScalarMassPerAmount();
  return *m_MeanCorpuscularHemoglobin;
}
double SECompleteBloodCount::GetMeanCorpuscularHemoglobin(const MassPerAmountUnit& unit) const
{
  if (!HasMeanCorpuscularHemoglobin())
    return SEScalar::dNaN();
  return m_MeanCorpuscularHemoglobin->GetValue(unit);
}

bool SECompleteBloodCount::HasMeanCorpuscularHemoglobinConcentration() const
{
  return m_MeanCorpuscularHemoglobinConcentration == nullptr ? false : m_MeanCorpuscularHemoglobinConcentration->IsValid();
}
SEScalarMassPerVolume& SECompleteBloodCount::GetMeanCorpuscularHemoglobinConcentration()
{
  if (m_MeanCorpuscularHemoglobinConcentration == nullptr)
    m_MeanCorpuscularHemoglobinConcentration = new SEScalarMassPerVolume();
  return *m_MeanCorpuscularHemoglobinConcentration;
}
double SECompleteBloodCount::GetMeanCorpuscularHemoglobinConcentration(const MassPerVolumeUnit& unit) const
{
  if (!HasMeanCorpuscularHemoglobinConcentration())
    return SEScalar::dNaN();
  return m_MeanCorpuscularHemoglobinConcentration->GetValue(unit);
}

bool SECompleteBloodCount::HasMeanCorpuscularVolume() const
{
  return m_MeanCorpuscularVolume == nullptr ? false : m_MeanCorpuscularVolume->IsValid();
}
SEScalarVolume& SECompleteBloodCount::GetMeanCorpuscularVolume()
{
  if (m_MeanCorpuscularVolume == nullptr)
    m_MeanCorpuscularVolume = new SEScalarVolume();
  return *m_MeanCorpuscularVolume;
}
double SECompleteBloodCount::GetMeanCorpuscularVolume(const VolumeUnit& unit) const
{
  if (!HasMeanCorpuscularVolume())
    return SEScalar::dNaN();
  return m_MeanCorpuscularVolume->GetValue(unit);
}

bool SECompleteBloodCount::HasRedBloodCellCount() const
{
  return m_RedBloodCellCount == nullptr ? false : m_RedBloodCellCount->IsValid();
}
SEScalarAmountPerVolume& SECompleteBloodCount::GetRedBloodCellCount()
{
  if (m_RedBloodCellCount == nullptr)
    m_RedBloodCellCount = new SEScalarAmountPerVolume();
  return *m_RedBloodCellCount;
}
double SECompleteBloodCount::GetRedBloodCellCount(const AmountPerVolumeUnit& unit) const
{
  if (!HasRedBloodCellCount())
    return SEScalar::dNaN();
  return m_RedBloodCellCount->GetValue(unit);
}

bool SECompleteBloodCount::HasWhiteBloodCellCount() const
{
  return m_WhiteBloodCellCount == nullptr ? false : m_WhiteBloodCellCount->IsValid();
}
SEScalarAmountPerVolume& SECompleteBloodCount::GetWhiteBloodCellCount()
{
  if (m_WhiteBloodCellCount == nullptr)
    m_WhiteBloodCellCount = new SEScalarAmountPerVolume();
  return *m_WhiteBloodCellCount;
}
double SECompleteBloodCount::GetWhiteBloodCellCount(const AmountPerVolumeUnit& unit) const
{
  if (!HasWhiteBloodCellCount())
    return SEScalar::dNaN();
  return m_WhiteBloodCellCount->GetValue(unit);
}