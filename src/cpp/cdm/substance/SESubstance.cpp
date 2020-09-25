/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceAerosolization.h"
#include "substance/SESubstanceClearance.h"
#include "substance/SESubstancePhysicochemicals.h"
#include "substance/SESubstancePharmacokinetics.h"
#include "substance/SESubstancePharmacodynamics.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarInversePressure.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerAreaTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolumePerTimePressure.h"
#include "io/protobuf/PBSubstance.h"

SESubstance::SESubstance(const std::string& name, Logger* logger) : Loggable(logger)
{
  m_Name = name;
  m_State = eSubstance_State::NullState;
  m_Density = nullptr;
  m_MolarMass = nullptr;

  m_MaximumDiffusionFlux = nullptr;
  m_MichaelisCoefficient = nullptr;

  m_Aerosolization = nullptr;
  m_BloodConcentration = nullptr;
  m_MassInBody = nullptr;
  m_MassInBlood = nullptr;
  m_MassInTissue = nullptr;
  m_PlasmaConcentration = nullptr;
  m_SystemicMassCleared = nullptr;
  m_TissueConcentration = nullptr;

  m_AlveolarTransfer = nullptr;
  m_DiffusingCapacity = nullptr;
  m_EndTidalFraction = nullptr;
  m_EndTidalPressure = nullptr;
  m_SolubilityCoefficient = nullptr;
  m_RelativeDiffusionCoefficient = nullptr;

  m_Clearance = nullptr;
  m_PK = nullptr;
  m_PD = nullptr;
}

SESubstance::~SESubstance()
{
  m_State = eSubstance_State::NullState;
  SAFE_DELETE(m_Density);
  SAFE_DELETE(m_MolarMass);

  SAFE_DELETE(m_MaximumDiffusionFlux);
  SAFE_DELETE(m_MichaelisCoefficient);

  SAFE_DELETE(m_BloodConcentration);
  SAFE_DELETE(m_MassInBody);
  SAFE_DELETE(m_MassInBlood);
  SAFE_DELETE(m_MassInTissue);
  SAFE_DELETE(m_PlasmaConcentration);
  SAFE_DELETE(m_SystemicMassCleared);
  SAFE_DELETE(m_TissueConcentration);

  SAFE_DELETE(m_AlveolarTransfer);
  SAFE_DELETE(m_DiffusingCapacity);
  SAFE_DELETE(m_EndTidalFraction);
  SAFE_DELETE(m_EndTidalPressure);
  SAFE_DELETE(m_SolubilityCoefficient);
  SAFE_DELETE(m_RelativeDiffusionCoefficient);

  SAFE_DELETE(m_Aerosolization);
  SAFE_DELETE(m_Clearance);
  SAFE_DELETE(m_PK);
  SAFE_DELETE(m_PD);
}

void SESubstance::Clear()
{
  m_State = eSubstance_State::NullState;
  INVALIDATE_PROPERTY(m_Density);
  INVALIDATE_PROPERTY(m_MolarMass);
  
  INVALIDATE_PROPERTY(m_MaximumDiffusionFlux);
  INVALIDATE_PROPERTY(m_MichaelisCoefficient);

  INVALIDATE_PROPERTY(m_BloodConcentration);
  INVALIDATE_PROPERTY(m_MassInBody);
  INVALIDATE_PROPERTY(m_MassInBlood);
  INVALIDATE_PROPERTY(m_MassInTissue);
  INVALIDATE_PROPERTY(m_PlasmaConcentration);
  INVALIDATE_PROPERTY(m_SystemicMassCleared);
  INVALIDATE_PROPERTY(m_TissueConcentration);

  INVALIDATE_PROPERTY(m_AlveolarTransfer);
  INVALIDATE_PROPERTY(m_DiffusingCapacity);
  INVALIDATE_PROPERTY(m_EndTidalFraction);
  INVALIDATE_PROPERTY(m_EndTidalPressure);
  INVALIDATE_PROPERTY(m_SolubilityCoefficient);
  INVALIDATE_PROPERTY(m_RelativeDiffusionCoefficient);

  if (m_Aerosolization != nullptr)
    m_Aerosolization->Clear();
  if (m_Clearance != nullptr)
    m_Clearance->Clear();
  if (m_PK != nullptr)
    m_PK->Clear();
  if (m_PD != nullptr)
    m_PD->Clear();
}

bool SESubstance::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBSubstance::SerializeToString(*this, output, m);
}
bool SESubstance::SerializeToFile(const std::string& filename) const
{
  return PBSubstance::SerializeToFile(*this, filename);
}
bool SESubstance::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBSubstance::SerializeFromString(src, *this, m);
}
bool SESubstance::SerializeFromFile(const std::string& filename)
{
  return PBSubstance::SerializeFromFile(filename, *this);
}

const SEScalar* SESubstance::GetScalar(const std::string& name)
{
  if (name.compare("Density") == 0)
    return &GetDensity();
  if (name.compare("MolarMass") == 0)
    return &GetMolarMass();

  if (name.compare("MaximumDiffusionFlux") == 0)
    return &GetMaximumDiffusionFlux();
  if (name.compare("MichaelisCoefficient") == 0)
    return &GetMichaelisCoefficient();

  if (name.compare("BloodConcentration") == 0)
    return &GetBloodConcentration();
  if (name.compare("MassInBody") == 0)
    return &GetMassInBody();
  if (name.compare("MassInBlood") == 0)
    return &GetMassInBlood();
  if (name.compare("MassInTissue") == 0)
    return &GetMassInTissue();
  if (name.compare("PlasmaConcentration") == 0)
    return &GetPlasmaConcentration();
  if (name.compare("SystemicMassCleared") == 0)
    return &GetSystemicMassCleared();
  if (name.compare("TissueConcentration") == 0)
    return &GetTissueConcentration();

  if (name.compare("AlveolarTransfer") == 0)
    return &GetAlveolarTransfer();
  if (name.compare("DiffusingCapacity") == 0)
    return &GetDiffusingCapacity();
  if (name.compare("EndTidalFraction") == 0)
    return &GetEndTidalFraction();
  if (name.compare("EndTidalPressure") == 0)
    return &GetEndTidalPressure();
  if (name.compare("RelativeDiffusionCoefficient") == 0)
    return &GetRelativeDiffusionCoefficient();
  if (name.compare("SolubilityCoefficient") == 0)
    return &GetSolubilityCoefficient();

  size_t split = name.find('-');
  if (split != name.npos)
  {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == "Aerosolization")
      return GetAerosolization().GetScalar(prop);
    if (child == "Clearance")
        return GetClearance().GetScalar(prop);
    if (child == "PK")
      return GetPK().GetScalar(prop);
    if (child == "PD")
      return GetPD().GetScalar(prop);
  }

  return nullptr;
}


std::string SESubstance::GetName() const
{
  return m_Name;
}

eSubstance_State SESubstance::GetState() const
{
  return m_State;
}
void SESubstance::SetState(eSubstance_State state)
{
  m_State = state;
}
bool SESubstance::HasState() const
{
  return m_State == eSubstance_State::NullState ? false : true;
}
void SESubstance::InvalidateState()
{
  m_State = eSubstance_State::NullState;
}

bool SESubstance::HasDensity() const
{
  return (m_Density == nullptr) ? false : m_Density->IsValid();
}
SEScalarMassPerVolume& SESubstance::GetDensity()
{
  if (m_Density == nullptr)
    m_Density = new SEScalarMassPerVolume();
  return *m_Density;
}
double SESubstance::GetDensity(const MassPerVolumeUnit& unit) const
{
  if (m_Density == nullptr)
    return SEScalar::dNaN();
  return m_Density->GetValue(unit);
}

bool SESubstance::HasMolarMass() const
{
  return (m_MolarMass == nullptr) ? false : m_MolarMass->IsValid();
}
SEScalarMassPerAmount& SESubstance::GetMolarMass()
{
  if (m_MolarMass == nullptr)
    m_MolarMass = new SEScalarMassPerAmount();
  return *m_MolarMass;
}
double SESubstance::GetMolarMass(const MassPerAmountUnit& unit) const
{
  if (m_MolarMass == nullptr)
    return SEScalar::dNaN();
  return m_MolarMass->GetValue(unit);
}

bool SESubstance::HasMaximumDiffusionFlux() const
{
  return (m_MaximumDiffusionFlux == nullptr) ? false : m_MaximumDiffusionFlux->IsValid();
}
SEScalarMassPerAreaTime& SESubstance::GetMaximumDiffusionFlux()
{
  if (m_MaximumDiffusionFlux == nullptr)
    m_MaximumDiffusionFlux = new SEScalarMassPerAreaTime();
  return *m_MaximumDiffusionFlux;
}
double SESubstance::GetMaximumDiffusionFlux(const MassPerAreaTimeUnit& unit) const
{
  if (m_MaximumDiffusionFlux == nullptr)
    return SEScalar::dNaN();
  return m_MaximumDiffusionFlux->GetValue(unit);
}

bool SESubstance::HasMichaelisCoefficient() const
{
  return (m_MichaelisCoefficient == nullptr) ? false : m_MichaelisCoefficient->IsValid();
}
SEScalar& SESubstance::GetMichaelisCoefficient()
{
  if (m_MichaelisCoefficient == nullptr)
    m_MichaelisCoefficient = new SEScalar();
  return *m_MichaelisCoefficient;
}
double SESubstance::GetMichaelisCoefficient() const
{
  if (m_MichaelisCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_MichaelisCoefficient->GetValue();
}

bool SESubstance::HasAerosolization() const
{
  return (m_Aerosolization != nullptr && m_Aerosolization->IsValid());
}
SESubstanceAerosolization& SESubstance::GetAerosolization()
{
  if (m_Aerosolization == nullptr)
    m_Aerosolization = new SESubstanceAerosolization(GetLogger());
  return *m_Aerosolization;
}
const SESubstanceAerosolization* SESubstance::GetAerosolization() const
{
  return m_Aerosolization;
}
void SESubstance::RemoveAerosolization()
{
  SAFE_DELETE(m_Aerosolization);
}

bool SESubstance::HasBloodConcentration() const
{
  return (m_BloodConcentration == nullptr) ? false : m_BloodConcentration->IsValid();
}
SEScalarMassPerVolume& SESubstance::GetBloodConcentration()
{
  if (m_BloodConcentration == nullptr)
    m_BloodConcentration = new SEScalarMassPerVolume();
  return *m_BloodConcentration;
}
double SESubstance::GetBloodConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_BloodConcentration == nullptr)
    return SEScalar::dNaN();
  return m_BloodConcentration->GetValue(unit);
}

bool SESubstance::HasMassInBody() const
{
  return (m_MassInBody == nullptr) ? false : m_MassInBody->IsValid();
}
SEScalarMass& SESubstance::GetMassInBody()
{
  if (m_MassInBody == nullptr)
    m_MassInBody = new SEScalarMass();
  return *m_MassInBody;
}
double SESubstance::GetMassInBody(const MassUnit& unit) const
{
  if (m_MassInBody == nullptr)
    return SEScalar::dNaN();
  return m_MassInBody->GetValue(unit);
}

bool SESubstance::HasMassInBlood() const
{
  return (m_MassInBlood == nullptr) ? false : m_MassInBlood->IsValid();
}
SEScalarMass& SESubstance::GetMassInBlood()
{
  if (m_MassInBlood == nullptr)
    m_MassInBlood = new SEScalarMass();
  return *m_MassInBlood;
}
double SESubstance::GetMassInBlood(const MassUnit& unit) const
{
  if (m_MassInBlood == nullptr)
    return SEScalar::dNaN();
  return m_MassInBlood->GetValue(unit);
}

bool SESubstance::HasMassInTissue() const
{
  return (m_MassInTissue == nullptr) ? false : m_MassInTissue->IsValid();
}
SEScalarMass& SESubstance::GetMassInTissue()
{
  if (m_MassInTissue == nullptr)
    m_MassInTissue = new SEScalarMass();
  return *m_MassInTissue;
}
double SESubstance::GetMassInTissue(const MassUnit& unit) const
{
  if (m_MassInTissue == nullptr)
    return SEScalar::dNaN();
  return m_MassInTissue->GetValue(unit);
}

bool SESubstance::HasPlasmaConcentration() const
{
  return (m_PlasmaConcentration == nullptr) ? false : m_PlasmaConcentration->IsValid();
}
SEScalarMassPerVolume& SESubstance::GetPlasmaConcentration()
{
  if (m_PlasmaConcentration == nullptr)
    m_PlasmaConcentration = new SEScalarMassPerVolume();
  return *m_PlasmaConcentration;
}
double SESubstance::GetPlasmaConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_PlasmaConcentration == nullptr)
    return SEScalar::dNaN();
  return m_PlasmaConcentration->GetValue(unit);
}

bool SESubstance::HasSystemicMassCleared() const
{
  return (m_SystemicMassCleared == nullptr) ? false : m_SystemicMassCleared->IsValid();
}
SEScalarMass& SESubstance::GetSystemicMassCleared()
{
  if (m_SystemicMassCleared == nullptr)
    m_SystemicMassCleared = new SEScalarMass();
  return *m_SystemicMassCleared;
}
double SESubstance::GetSystemicMassCleared(const MassUnit& unit) const
{
  if (m_SystemicMassCleared == nullptr)
    return SEScalar::dNaN();
  return m_SystemicMassCleared->GetValue(unit);
}

bool SESubstance::HasTissueConcentration() const
{
  return (m_TissueConcentration == nullptr) ? false : m_TissueConcentration->IsValid();
}
SEScalarMassPerVolume& SESubstance::GetTissueConcentration()
{
  if (m_TissueConcentration == nullptr)
    m_TissueConcentration = new SEScalarMassPerVolume();
  return *m_TissueConcentration;
}
double SESubstance::GetTissueConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_TissueConcentration == nullptr)
    return SEScalar::dNaN();
  return m_TissueConcentration->GetValue(unit);
}


bool SESubstance::HasAlveolarTransfer() const
{
  return m_AlveolarTransfer == nullptr ? false : m_AlveolarTransfer->IsValid();
}
SEScalarVolumePerTime& SESubstance::GetAlveolarTransfer()
{
  if (m_AlveolarTransfer == nullptr)
    m_AlveolarTransfer = new SEScalarVolumePerTime();
  return *m_AlveolarTransfer;
}
double SESubstance::GetAlveolarTransfer(const VolumePerTimeUnit& unit) const
{
  if (m_AlveolarTransfer == nullptr)
    return SEScalar::dNaN();
  return m_AlveolarTransfer->GetValue(unit);
}

bool SESubstance::HasDiffusingCapacity() const
{
  return m_DiffusingCapacity == nullptr ? false : m_DiffusingCapacity->IsValid();
}
SEScalarVolumePerTimePressure& SESubstance::GetDiffusingCapacity()
{
  if (m_DiffusingCapacity == nullptr)
    m_DiffusingCapacity = new SEScalarVolumePerTimePressure();
  return *m_DiffusingCapacity;
}
double SESubstance::GetDiffusingCapacity(const VolumePerTimePressureUnit& unit) const
{
  if (m_DiffusingCapacity == nullptr)
    return SEScalar::dNaN();
  return m_DiffusingCapacity->GetValue(unit);
}

bool SESubstance::HasEndTidalFraction() const
{
  return (m_EndTidalFraction == nullptr) ? false : m_EndTidalFraction->IsValid();
}
SEScalar0To1& SESubstance::GetEndTidalFraction()
{
  if (m_EndTidalFraction == nullptr)
    m_EndTidalFraction = new SEScalar0To1();
  return *m_EndTidalFraction;
}
double SESubstance::GetEndTidalFraction() const
{
  if (m_EndTidalFraction == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalFraction->GetValue();
}

bool SESubstance::HasEndTidalPressure() const
{
  return (m_EndTidalPressure == nullptr) ? false : m_EndTidalPressure->IsValid();
}
SEScalarPressure& SESubstance::GetEndTidalPressure()
{
  if (m_EndTidalPressure == nullptr)
    m_EndTidalPressure = new SEScalarPressure();
  return *m_EndTidalPressure;
}
double SESubstance::GetEndTidalPressure(const PressureUnit& unit) const
{
  if (m_EndTidalPressure == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalPressure->GetValue(unit);
}

bool SESubstance::HasRelativeDiffusionCoefficient() const
{
  return (m_RelativeDiffusionCoefficient == nullptr) ? false : m_RelativeDiffusionCoefficient->IsValid();
}
SEScalar& SESubstance::GetRelativeDiffusionCoefficient()
{
  if (m_RelativeDiffusionCoefficient == nullptr)
    m_RelativeDiffusionCoefficient = new SEScalar();
  return *m_RelativeDiffusionCoefficient;
}
double SESubstance::GetRelativeDiffusionCoefficient() const
{
  if (m_RelativeDiffusionCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_RelativeDiffusionCoefficient->GetValue();
}

bool SESubstance::HasSolubilityCoefficient() const
{
  return (m_SolubilityCoefficient == nullptr) ? false : m_SolubilityCoefficient->IsValid();
}
SEScalarInversePressure& SESubstance::GetSolubilityCoefficient()
{
  if (m_SolubilityCoefficient == nullptr)
    m_SolubilityCoefficient = new SEScalarInversePressure();
  return *m_SolubilityCoefficient;
}
double SESubstance::GetSolubilityCoefficient(const InversePressureUnit& unit) const
{
  if (m_SolubilityCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_SolubilityCoefficient->GetValue(unit);
}

bool SESubstance::HasClearance() const
{
  return (m_Clearance != nullptr && m_Clearance->IsValid());
}
SESubstanceClearance& SESubstance::GetClearance()
{
  if (m_Clearance == nullptr)
    m_Clearance = new SESubstanceClearance(GetLogger());
  return *m_Clearance;
}
const SESubstanceClearance* SESubstance::GetClearance() const
{
  return m_Clearance;
}
void SESubstance::RemoveClearance()
{
  SAFE_DELETE(m_Clearance);
}

bool SESubstance::HasPK() const
{
  return (m_PK != nullptr && m_PK->IsValid());
}
SESubstancePharmacokinetics& SESubstance::GetPK()
{
  if (m_PK == nullptr)
    m_PK = new SESubstancePharmacokinetics(GetLogger());
  return *m_PK;
}
const SESubstancePharmacokinetics* SESubstance::GetPK() const
{
  return m_PK;
}
void SESubstance::RemovePK()
{
  SAFE_DELETE(m_PK);
}

bool SESubstance::HasPD() const
{
  return (m_PD != nullptr && m_PD->IsValid());
}
SESubstancePharmacodynamics& SESubstance::GetPD()
{
  if (m_PD == nullptr)
    m_PD = new SESubstancePharmacodynamics(GetLogger());
  return *m_PD;
}
const SESubstancePharmacodynamics* SESubstance::GetPD() const
{
  return m_PD;
}
void SESubstance::RemovePD()
{
  SAFE_DELETE(m_PD);
}

