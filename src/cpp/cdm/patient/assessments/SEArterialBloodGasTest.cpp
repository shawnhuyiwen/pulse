/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/assessments/SEArterialBloodGasTest.h"
#include "cdm/system/physiology/SEBloodChemistrySystem.h"
#include "cdm/properties/SEScalarEquivalentWeightPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/io/protobuf/PBPatientAssessments.h"

SEArterialBloodGasTest::SEArterialBloodGasTest(Logger* logger) : SEPatientAssessment(logger)
{
  m_BloodPH = nullptr;
  m_Bicarbonate = nullptr;
  m_PartialPressureOfOxygen = nullptr;
  m_PartialPressureOfCarbonDioxide = nullptr;
  m_OxygenSaturation = nullptr;
}

SEArterialBloodGasTest::~SEArterialBloodGasTest()
{
  SAFE_DELETE(m_BloodPH);
  SAFE_DELETE(m_Bicarbonate);
  SAFE_DELETE(m_PartialPressureOfOxygen);
  SAFE_DELETE(m_PartialPressureOfCarbonDioxide);
  SAFE_DELETE(m_OxygenSaturation);
}

void SEArterialBloodGasTest::Clear()
{
  SEPatientAssessment::Clear();
  INVALIDATE_PROPERTY(m_BloodPH);
  INVALIDATE_PROPERTY(m_Bicarbonate);
  INVALIDATE_PROPERTY(m_PartialPressureOfOxygen);
  INVALIDATE_PROPERTY(m_PartialPressureOfCarbonDioxide);
  INVALIDATE_PROPERTY(m_OxygenSaturation);
}

bool SEArterialBloodGasTest::SerializeToString(std::string& output, eSerializationFormat m) const
{
  return PBPatientAssessment::SerializeToString(*this, output, m);
}
bool SEArterialBloodGasTest::SerializeToFile(const std::string& filename) const
{
  return PBPatientAssessment::SerializeToFile(*this, filename);
}

bool SEArterialBloodGasTest::HasBloodPH() const
{
  return m_BloodPH == nullptr ? false : m_BloodPH->IsValid();
}
SEScalar& SEArterialBloodGasTest::GetBloodPH()
{
  if (m_BloodPH == nullptr)
    m_BloodPH = new SEScalar();
  return *m_BloodPH;
}
double SEArterialBloodGasTest::GetBloodPH() const
{
  if (!HasBloodPH())
    return SEScalar::dNaN();
  return m_BloodPH->GetValue();
}

bool SEArterialBloodGasTest::HasBicarbonate() const
{
  return m_Bicarbonate == nullptr ? false : m_Bicarbonate->IsValid();
}
SEScalarEquivalentWeightPerVolume& SEArterialBloodGasTest::GetBicarbonate()
{
  if (m_Bicarbonate == nullptr)
    m_Bicarbonate = new SEScalarEquivalentWeightPerVolume();
  return *m_Bicarbonate;
}
double SEArterialBloodGasTest::GetBicarbonate(const EquivalentWeightPerVolumeUnit& unit) const
{
  if (!HasBicarbonate())
    return SEScalar::dNaN();
  return m_Bicarbonate->GetValue(unit);
}

bool SEArterialBloodGasTest::HasPartialPressureOfOxygen() const
{
  return m_PartialPressureOfOxygen == nullptr ? false : m_PartialPressureOfOxygen->IsValid();
}
SEScalarPressure& SEArterialBloodGasTest::GetPartialPressureOfOxygen()
{
  if (m_PartialPressureOfOxygen == nullptr)
    m_PartialPressureOfOxygen = new SEScalarPressure();
  return *m_PartialPressureOfOxygen;
}
double SEArterialBloodGasTest::GetPartialPressureOfOxygen(const PressureUnit& unit) const
{
  if (!HasPartialPressureOfOxygen())
    return SEScalar::dNaN();
  return m_PartialPressureOfOxygen->GetValue(unit);
}

bool SEArterialBloodGasTest::HasPartialPressureOfCarbonDioxide() const
{
  return m_PartialPressureOfCarbonDioxide == nullptr ? false : m_PartialPressureOfCarbonDioxide->IsValid();
}
SEScalarPressure& SEArterialBloodGasTest::GetPartialPressureOfCarbonDioxide()
{
  if (m_PartialPressureOfCarbonDioxide == nullptr)
    m_PartialPressureOfCarbonDioxide = new SEScalarPressure();
  return *m_PartialPressureOfCarbonDioxide;
}
double SEArterialBloodGasTest::GetPartialPressureOfCarbonDioxide(const PressureUnit& unit) const
{
  if (!HasPartialPressureOfCarbonDioxide())
    return SEScalar::dNaN();
  return m_PartialPressureOfCarbonDioxide->GetValue(unit);
}

bool SEArterialBloodGasTest::HasOxygenSaturation() const
{
  return m_OxygenSaturation == nullptr ? false : m_OxygenSaturation->IsValid();
}
SEScalar& SEArterialBloodGasTest::GetOxygenSaturation()
{
  if (m_OxygenSaturation == nullptr)
    m_OxygenSaturation = new SEScalar();
  return *m_OxygenSaturation;
}
double SEArterialBloodGasTest::GetOxygenSaturation() const
{
  if (!HasOxygenSaturation())
    return SEScalar::dNaN();
  return m_OxygenSaturation->GetValue();
}
