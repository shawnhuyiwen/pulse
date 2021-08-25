/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorVolumeControl.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorVolumeControl::SEMechanicalVentilatorVolumeControl(Logger* logger) : SEMechanicalVentilatorMode(logger)
{
  m_Mode = eMechanicalVentilator_VolumeControlMode::AssistedControl;
  m_Flow = nullptr;
  m_FractionInspiredOxygen = nullptr;
  m_InspiratoryPeriod = nullptr;
  m_PositiveEndExpiredPressure = nullptr;
  m_RespirationRate = nullptr;
  m_TidalVolume = nullptr;
}

SEMechanicalVentilatorVolumeControl::~SEMechanicalVentilatorVolumeControl()
{
  m_Mode = eMechanicalVentilator_VolumeControlMode::AssistedControl;
  SAFE_DELETE(m_Flow);
  SAFE_DELETE(m_FractionInspiredOxygen);
  SAFE_DELETE(m_InspiratoryPeriod);
  SAFE_DELETE(m_PositiveEndExpiredPressure);
  SAFE_DELETE(m_RespirationRate);
  SAFE_DELETE(m_TidalVolume);
}

void SEMechanicalVentilatorVolumeControl::Clear()
{
  SEMechanicalVentilatorMode::Clear();
  m_Mode = eMechanicalVentilator_VolumeControlMode::AssistedControl;
  INVALIDATE_PROPERTY(m_Flow);
  INVALIDATE_PROPERTY(m_FractionInspiredOxygen);
  INVALIDATE_PROPERTY(m_InspiratoryPeriod);
  INVALIDATE_PROPERTY(m_PositiveEndExpiredPressure);
  INVALIDATE_PROPERTY(m_RespirationRate);
  INVALIDATE_PROPERTY(m_TidalVolume);
}

void SEMechanicalVentilatorVolumeControl::Copy(const SEMechanicalVentilatorVolumeControl& src, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEMechanicalVentilatorVolumeControl::ToSettings(SEMechanicalVentilatorSettings& s, const SESubstanceManager& subMgr)
{
  if (!SEMechanicalVentilatorMode::ToSettings(s, subMgr))
    return false;
  if (SEMechanicalVentilatorMode::IsActive())
  {
    // Translate ventilator settings
    double totalPeriod_s = 60.0 / GetRespirationRate(FrequencyUnit::Per_min);
    double expiratoryPeriod_s = totalPeriod_s - GetInspiratoryPeriod(TimeUnit::s);

    s.SetInspirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
    s.SetExpirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
    s.GetInspirationTargetFlow().Set(GetFlow());
    s.GetPositiveEndExpiredPressure().Set(GetPositiveEndExpiredPressure());
    s.GetInspirationMachineTriggerTime().SetValue(expiratoryPeriod_s, TimeUnit::s);
    s.GetExpirationCycleVolume().Set(GetTidalVolume());
    s.GetFractionInspiredGas(*subMgr.GetSubstance("Oxygen")).GetFractionAmount().Set(GetFractionInspiredOxygen());
    if (GetMode() == eMechanicalVentilator_VolumeControlMode::AssistedControl)
      s.GetInspirationPatientTriggerFlow().SetValue(0.1, VolumePerTimeUnit::L_Per_s);
  }
  return true;
}

bool SEMechanicalVentilatorVolumeControl::IsValid() const
{
  return SEMechanicalVentilatorMode::IsValid() &&
    HasFlow() &&
    HasFractionInspiredOxygen() &&
    HasInspiratoryPeriod() &&
    HasPositiveEndExpiredPressure() &&
    HasRespirationRate() &&
    HasTidalVolume();
}

bool SEMechanicalVentilatorVolumeControl::IsActive() const
{
  return SEMechanicalVentilatorMode::IsActive();
}
void SEMechanicalVentilatorVolumeControl::Deactivate()
{
  SEMechanicalVentilatorMode::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEMechanicalVentilatorVolumeControl::GetScalar(const std::string& name)
{
  if (name.compare("Flow") == 0)
    return &GetFlow();
  if (name.compare("FractionInspiredOxygen") == 0)
    return &GetFractionInspiredOxygen();
  if (name.compare("InspiratoryPeriod") == 0)
    return &GetInspiratoryPeriod();
  if (name.compare("PositiveEndExpiredPressure") == 0)
    return &GetPositiveEndExpiredPressure();
  if (name.compare("RespirationRate") == 0)
    return &GetRespirationRate();
  if (name.compare("TidalVolume") == 0)
    return &GetTidalVolume();
  return nullptr;
}

void SEMechanicalVentilatorVolumeControl::SetMode(eMechanicalVentilator_VolumeControlMode m)
{
  m_Mode = m;
}
eMechanicalVentilator_VolumeControlMode SEMechanicalVentilatorVolumeControl::GetMode() const
{
  return m_Mode;
}

bool SEMechanicalVentilatorVolumeControl::HasFlow() const
{
  return m_Flow != nullptr;
}
SEScalarVolumePerTime& SEMechanicalVentilatorVolumeControl::GetFlow()
{
  if (m_Flow == nullptr)
    m_Flow = new SEScalarVolumePerTime();
  return *m_Flow;
}
double SEMechanicalVentilatorVolumeControl::GetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Flow == nullptr)
    return SEScalar::dNaN();
  return m_Flow->GetValue(unit);
}

bool SEMechanicalVentilatorVolumeControl::HasFractionInspiredOxygen() const
{
  return m_FractionInspiredOxygen != nullptr;
}
SEScalar0To1& SEMechanicalVentilatorVolumeControl::GetFractionInspiredOxygen()
{
  if (m_FractionInspiredOxygen == nullptr)
    m_FractionInspiredOxygen = new SEScalar0To1();
  return *m_FractionInspiredOxygen;
}
double SEMechanicalVentilatorVolumeControl::GetFractionInspiredOxygen() const
{
  if (m_FractionInspiredOxygen == nullptr)
    return SEScalar::dNaN();
  return m_FractionInspiredOxygen->GetValue();
}

bool SEMechanicalVentilatorVolumeControl::HasInspiratoryPeriod() const
{
  return m_InspiratoryPeriod != nullptr;
}
SEScalarTime& SEMechanicalVentilatorVolumeControl::GetInspiratoryPeriod()
{
  if (m_InspiratoryPeriod == nullptr)
    m_InspiratoryPeriod = new SEScalarTime();
  return *m_InspiratoryPeriod;
}
double SEMechanicalVentilatorVolumeControl::GetInspiratoryPeriod(const TimeUnit& unit) const
{
  if (m_InspiratoryPeriod == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryPeriod->GetValue(unit);
}

bool SEMechanicalVentilatorVolumeControl::HasPositiveEndExpiredPressure() const
{
  return m_PositiveEndExpiredPressure != nullptr;
}
SEScalarPressure& SEMechanicalVentilatorVolumeControl::GetPositiveEndExpiredPressure()
{
  if (m_PositiveEndExpiredPressure == nullptr)
    m_PositiveEndExpiredPressure = new SEScalarPressure();
  return *m_PositiveEndExpiredPressure;
}
double SEMechanicalVentilatorVolumeControl::GetPositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_PositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_PositiveEndExpiredPressure->GetValue(unit);
}

bool SEMechanicalVentilatorVolumeControl::HasRespirationRate() const
{
  return m_RespirationRate != nullptr;
}
SEScalarFrequency& SEMechanicalVentilatorVolumeControl::GetRespirationRate()
{
  if (m_RespirationRate == nullptr)
    m_RespirationRate = new SEScalarFrequency();
  return *m_RespirationRate;
}
double SEMechanicalVentilatorVolumeControl::GetRespirationRate(const FrequencyUnit& unit) const
{
  if (m_RespirationRate == nullptr)
    return SEScalar::dNaN();
  return m_RespirationRate->GetValue(unit);
}

bool SEMechanicalVentilatorVolumeControl::HasTidalVolume() const
{
  return m_TidalVolume != nullptr;
}
SEScalarVolume& SEMechanicalVentilatorVolumeControl::GetTidalVolume()
{
  if (m_TidalVolume == nullptr)
    m_TidalVolume = new SEScalarVolume();
  return *m_TidalVolume;
}
double SEMechanicalVentilatorVolumeControl::GetTidalVolume(const VolumeUnit& unit) const
{
  if (m_TidalVolume == nullptr)
    return SEScalar::dNaN();
  return m_TidalVolume->GetValue(unit);
}

void SEMechanicalVentilatorVolumeControl::ToString(std::ostream& str) const
{
  str << "Mechanical Ventilator Action : Volume Control";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tConnection: " << eMechanicalVentilator_Connection_Name(m_Connection);
  str << "\n\tMode: " << eMechanicalVentilator_VolumeControlMode_Name(m_Mode);
  str << "\n\tFlow :"; HasFlow() ? str << *m_Flow : str << "NaN";
  str << "\n\tFractionInspiredOxygen :"; HasFractionInspiredOxygen() ? str << *m_FractionInspiredOxygen : str << "NaN";
  str << "\n\tInspiratoryPeriod :"; HasInspiratoryPeriod() ? str << *m_InspiratoryPeriod : str << "NaN";
  str << "\n\tPositiveEndExpiredPressure :"; HasPositiveEndExpiredPressure() ? str << *m_PositiveEndExpiredPressure : str << "NaN";
  str << "\n\tRespirationRate :"; HasRespirationRate() ? str << *m_RespirationRate : str << "NaN";
  str << "\n\tTidalVolume :"; HasTidalVolume() ? str << *m_TidalVolume : str << "NaN";
  str << std::flush;
}