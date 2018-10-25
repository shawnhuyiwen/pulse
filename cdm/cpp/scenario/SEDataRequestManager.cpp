/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "SEDataRequestManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBScenario.h"
#include "io/protobuf/PBScenario.h"

SEDataRequestManager::SEDataRequestManager(Logger* logger) : Loggable(logger)
{
  m_DefaultDecimalFormatting = nullptr;
  m_OverrideDecimalFormatting = nullptr;
  Clear();
}

SEDataRequestManager::~SEDataRequestManager()
{
  Clear();
}

void SEDataRequestManager::Clear()
{
  m_SamplesPerSecond = 0; // Sample every time step
  DELETE_VECTOR(m_Requests);
  SAFE_DELETE(m_DefaultDecimalFormatting);
  SAFE_DELETE(m_OverrideDecimalFormatting);
}

void SEDataRequestManager::Copy(const SEDataRequestManager& src, const SESubstanceManager& subMgr)
{
  PBScenario::Copy(src, *this, subMgr);
}

bool SEDataRequestManager::SerializeToString(std::string& output, SerializationMode m) const
{
  return PBScenario::SerializeToString(*this, output, m);
}
bool SEDataRequestManager::SerializeToFile(const std::string& filename, SerializationMode m) const
{
  return PBScenario::SerializeToFile(*this, filename, m);
}
bool SEDataRequestManager::SerializeFromString(const std::string& src, SerializationMode m, const SESubstanceManager& subMgr)
{
  return PBScenario::SerializeFromString(src, *this, m, subMgr);
}
bool SEDataRequestManager::SerializeFromFile(const std::string& filename, SerializationMode m, const SESubstanceManager& subMgr)
{
  return PBScenario::SerializeFromFile(filename, *this, m, subMgr);
}

bool SEDataRequestManager::HasDefaultDecimalFormatting() const
{
  return m_DefaultDecimalFormatting != nullptr;
}
SEDecimalFormat& SEDataRequestManager::GetDefaultDecimalFormatting()
{
  if (m_DefaultDecimalFormatting == nullptr)
    m_DefaultDecimalFormatting = new SEDecimalFormat();
  return *m_DefaultDecimalFormatting;
}
void SEDataRequestManager::RemoveDefaultDecimalFormatting()
{
  SAFE_DELETE(m_DefaultDecimalFormatting);
}

bool SEDataRequestManager::HasOverrideDecimalFormatting() const
{
  return m_OverrideDecimalFormatting != nullptr;
}
SEDecimalFormat& SEDataRequestManager::GetOverrideDecimalFormatting()
{
  if (m_OverrideDecimalFormatting == nullptr)
    m_OverrideDecimalFormatting = new SEDecimalFormat();
  return *m_OverrideDecimalFormatting;
}
void SEDataRequestManager::RemoveOverrideDecimalFormatting()
{
  SAFE_DELETE(m_OverrideDecimalFormatting);
}

SEDataRequest& SEDataRequestManager::CopyDataRequest(const SEDataRequest& dr)
{
  SEDataRequest* my_dr = new SEDataRequest(dr);
  m_Requests.push_back(my_dr);
  return *my_dr;
}

SEDataRequest& SEDataRequestManager::CreateDataRequest(eDataRequest_Category category, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(category, dfault);
  m_Requests.push_back(dr);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreatePatientDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Patient, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreatePatientDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Patient, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreatePhysiologyDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Physiology, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreatePhysiologyDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Physiology, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateEnvironmentDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Environment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateEnvironmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Environment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::GasCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::GasCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::GasCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::GasCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::LiquidCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::LiquidCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::LiquidCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::LiquidCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateThermalCompartmentDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::ThermalCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateThermalCompartmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::ThermalCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateTissueCompartmentDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::TissueCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateTissueCompartmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::TissueCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateSubstanceDataRequest(const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Substance, dfault);
  m_Requests.push_back(dr);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateSubstanceDataRequest(const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Substance, dfault);
  m_Requests.push_back(dr);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateAnesthesiaMachineDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::AnesthesiaMachine, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateAnesthesiaMachineDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::AnesthesiaMachine, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateECGDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::ECG, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateECGDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::ECG, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreateInhalerDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Inhaler, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateInhalerDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(eDataRequest_Category::Inhaler, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
