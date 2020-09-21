/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/assessments/SEComprehensiveMetabolicPanel.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarMass.h"
#include "io/protobuf/PBPatientAssessments.h"


SEComprehensiveMetabolicPanel::SEComprehensiveMetabolicPanel(Logger* logger) : SEPatientAssessment(logger)
{
  m_Albumin = nullptr;
  m_ALP = nullptr;
  m_ALT = nullptr;
  m_AST = nullptr;
  m_BUN = nullptr;
  m_Calcium = nullptr;
  m_Chloride = nullptr;
  m_CO2 = nullptr;
  m_Creatinine = nullptr;
  m_Glucose = nullptr;
  m_Potassium = nullptr;
  m_Sodium = nullptr;
  m_TotalBilirubin = nullptr;
  m_TotalProtein = nullptr;
}

SEComprehensiveMetabolicPanel::~SEComprehensiveMetabolicPanel()
{
  SAFE_DELETE(m_Albumin);
  SAFE_DELETE(m_ALP);
  SAFE_DELETE(m_ALT);
  SAFE_DELETE(m_AST);
  SAFE_DELETE(m_BUN);
  SAFE_DELETE(m_Calcium);
  SAFE_DELETE(m_Chloride);
  SAFE_DELETE(m_CO2);
  SAFE_DELETE(m_Creatinine);
  SAFE_DELETE(m_Glucose);
  SAFE_DELETE(m_Potassium);
  SAFE_DELETE(m_Sodium);
  SAFE_DELETE(m_TotalBilirubin);
  SAFE_DELETE(m_TotalProtein);
}

void SEComprehensiveMetabolicPanel::Clear()
{
  SEPatientAssessment::Clear();
  INVALIDATE_PROPERTY(m_Albumin);
  INVALIDATE_PROPERTY(m_ALP);
  INVALIDATE_PROPERTY(m_ALT);
  INVALIDATE_PROPERTY(m_AST);
  INVALIDATE_PROPERTY(m_BUN);
  INVALIDATE_PROPERTY(m_Calcium);
  INVALIDATE_PROPERTY(m_Chloride);
  INVALIDATE_PROPERTY(m_CO2);
  INVALIDATE_PROPERTY(m_Creatinine);
  INVALIDATE_PROPERTY(m_Glucose);
  INVALIDATE_PROPERTY(m_Potassium);
  INVALIDATE_PROPERTY(m_Sodium);
  INVALIDATE_PROPERTY(m_TotalBilirubin);
  INVALIDATE_PROPERTY(m_TotalProtein);
}

bool SEComprehensiveMetabolicPanel::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBPatientAssessment::SerializeToString(*this, output, m);
}
bool SEComprehensiveMetabolicPanel::SerializeToFile(const std::string& filename) const
{
  return PBPatientAssessment::SerializeToFile(*this, filename);
}

bool SEComprehensiveMetabolicPanel::HasAlbumin() const
{
  return m_Albumin == nullptr ? false : m_Albumin->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetAlbumin()
{
  if (m_Albumin == nullptr)
    m_Albumin = new SEScalarMassPerVolume();
  return *m_Albumin;
}
double SEComprehensiveMetabolicPanel::GetAlbumin(const MassPerVolumeUnit& unit) const
{
  if (!HasAlbumin())
    return SEScalar::dNaN();
  return m_Albumin->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasALP() const
{
  return m_ALP == nullptr ? false : m_ALP->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetALP()
{
  if (m_ALP == nullptr)
    m_ALP = new SEScalarMassPerVolume();
  return *m_ALP;
}
double SEComprehensiveMetabolicPanel::GetALP(const MassPerVolumeUnit& unit) const
{
  if (!HasALP())
    return SEScalar::dNaN();
  return m_ALP->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasALT() const
{
  return m_ALT == nullptr ? false : m_ALT->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetALT()
{
  if (m_ALT == nullptr)
    m_ALT = new SEScalarMassPerVolume();
  return *m_ALT;
}
double SEComprehensiveMetabolicPanel::GetALT(const MassPerVolumeUnit& unit) const
{
  if (!HasALT())
    return SEScalar::dNaN();
  return m_ALT->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasAST() const
{
  return m_AST == nullptr ? false : m_AST->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetAST()
{
  if (m_AST == nullptr)
    m_AST = new SEScalarMassPerVolume();
  return *m_AST;
}
double SEComprehensiveMetabolicPanel::GetAST(const MassPerVolumeUnit& unit) const
{
  if (!HasAST())
    return SEScalar::dNaN();
  return m_AST->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasBUN() const
{
  return m_BUN == nullptr ? false : m_BUN->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetBUN()
{
  if (m_BUN == nullptr)
    m_BUN = new SEScalarMassPerVolume();
  return *m_BUN;
}
double SEComprehensiveMetabolicPanel::GetBUN(const MassPerVolumeUnit& unit) const
{
  if (!HasBUN())
    return SEScalar::dNaN();
  return m_BUN->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasCalcium() const
{
  return m_Calcium == nullptr ? false : m_Calcium->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetCalcium()
{
  if (m_Calcium == nullptr)
    m_Calcium = new SEScalarMassPerVolume();
  return *m_Calcium;
}
double SEComprehensiveMetabolicPanel::GetCalcium(const MassPerVolumeUnit& unit) const
{
  if (!HasCalcium())
    return SEScalar::dNaN();
  return m_Calcium->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasChloride() const
{
  return m_Chloride == nullptr ? false : m_Chloride->IsValid();
}
SEScalarAmountPerVolume& SEComprehensiveMetabolicPanel::GetChloride()
{
  if (m_Chloride == nullptr)
    m_Chloride = new SEScalarAmountPerVolume();
  return *m_Chloride;
}
double SEComprehensiveMetabolicPanel::GetChloride(const AmountPerVolumeUnit& unit) const
{
  if (!HasChloride())
    return SEScalar::dNaN();
  return m_Chloride->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasCO2() const
{
  return m_CO2 == nullptr ? false : m_CO2->IsValid();
}
SEScalarAmountPerVolume& SEComprehensiveMetabolicPanel::GetCO2()
{
  if (m_CO2 == nullptr)
    m_CO2 = new SEScalarAmountPerVolume();
  return *m_CO2;
}
double SEComprehensiveMetabolicPanel::GetCO2(const AmountPerVolumeUnit& unit) const
{
  if (!HasCO2())
    return SEScalar::dNaN();
  return m_CO2->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasCreatinine() const
{
  return m_Creatinine == nullptr ? false : m_Creatinine->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetCreatinine()
{
  if (m_Creatinine == nullptr)
    m_Creatinine = new SEScalarMassPerVolume();
  return *m_Creatinine;
}
double SEComprehensiveMetabolicPanel::GetCreatinine(const MassPerVolumeUnit& unit) const
{
  if (!HasCreatinine())
    return SEScalar::dNaN();
  return m_Creatinine->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasGlucose() const
{
  return m_Glucose == nullptr ? false : m_Glucose->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetGlucose()
{
  if (m_Glucose == nullptr)
    m_Glucose = new SEScalarMassPerVolume();
  return *m_Glucose;
}
double SEComprehensiveMetabolicPanel::GetGlucose(const MassPerVolumeUnit& unit) const
{
  if (!HasGlucose())
    return SEScalar::dNaN();
  return m_Glucose->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasPotassium() const
{
  return m_Potassium == nullptr ? false : m_Potassium->IsValid();
}
SEScalarAmountPerVolume& SEComprehensiveMetabolicPanel::GetPotassium()
{
  if (m_Potassium == nullptr)
    m_Potassium = new SEScalarAmountPerVolume();
  return *m_Potassium;
}
double SEComprehensiveMetabolicPanel::GetPotassium(const AmountPerVolumeUnit& unit) const
{
  if (!HasPotassium())
    return SEScalar::dNaN();
  return m_Potassium->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasSodium() const
{
  return m_Sodium == nullptr ? false : m_Sodium->IsValid();
}
SEScalarAmountPerVolume& SEComprehensiveMetabolicPanel::GetSodium()
{
  if (m_Sodium == nullptr)
    m_Sodium = new SEScalarAmountPerVolume();
  return *m_Sodium;
}
double SEComprehensiveMetabolicPanel::GetSodium(const AmountPerVolumeUnit& unit) const
{
  if (!HasSodium())
    return SEScalar::dNaN();
  return m_Sodium->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasTotalBilirubin() const
{
  return m_TotalBilirubin == nullptr ? false : m_TotalBilirubin->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetTotalBilirubin()
{
  if (m_TotalBilirubin == nullptr)
    m_TotalBilirubin = new SEScalarMassPerVolume();
  return *m_TotalBilirubin;
}
double SEComprehensiveMetabolicPanel::GetTotalBilirubin(const MassPerVolumeUnit& unit) const
{
  if (!HasTotalBilirubin())
    return SEScalar::dNaN();
  return m_TotalBilirubin->GetValue(unit);
}

bool SEComprehensiveMetabolicPanel::HasTotalProtein() const
{
  return m_TotalProtein == nullptr ? false : m_TotalProtein->IsValid();
}
SEScalarMassPerVolume& SEComprehensiveMetabolicPanel::GetTotalProtein()
{
  if (m_TotalProtein == nullptr)
    m_TotalProtein = new SEScalarMassPerVolume();
  return *m_TotalProtein;
}
double SEComprehensiveMetabolicPanel::GetTotalProtein(const MassPerVolumeUnit& unit) const
{
  if (!HasTotalProtein())
    return SEScalar::dNaN();
  return m_TotalProtein->GetValue(unit);
}