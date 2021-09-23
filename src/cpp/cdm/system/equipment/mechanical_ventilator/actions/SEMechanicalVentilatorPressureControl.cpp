/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorPressureControl.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorPressureControl::SEMechanicalVentilatorPressureControl(Logger* logger) : SEMechanicalVentilatorMode(logger)
{
  m_Mode = eMechanicalVentilator_PressureControlMode::AssistedControl;
  m_FractionInspiredOxygen = nullptr;
  m_InspiratoryPeriod = nullptr;
  m_InspiratoryPressure = nullptr;
  m_PositiveEndExpiredPressure = nullptr;
  m_RespirationRate = nullptr;
  m_Slope = nullptr;
}

SEMechanicalVentilatorPressureControl::~SEMechanicalVentilatorPressureControl()
{
  m_Mode = eMechanicalVentilator_PressureControlMode::AssistedControl;
  SAFE_DELETE(m_FractionInspiredOxygen);
  SAFE_DELETE(m_InspiratoryPeriod);
  SAFE_DELETE(m_InspiratoryPressure);
  SAFE_DELETE(m_PositiveEndExpiredPressure);
  SAFE_DELETE(m_RespirationRate);
  SAFE_DELETE(m_Slope);
}

void SEMechanicalVentilatorPressureControl::Clear()
{
  SEMechanicalVentilatorMode::Clear();
  m_Mode = eMechanicalVentilator_PressureControlMode::AssistedControl;
  INVALIDATE_PROPERTY(m_FractionInspiredOxygen);
  INVALIDATE_PROPERTY(m_InspiratoryPeriod);
  INVALIDATE_PROPERTY(m_InspiratoryPressure);
  INVALIDATE_PROPERTY(m_PositiveEndExpiredPressure);
  INVALIDATE_PROPERTY(m_RespirationRate);
  INVALIDATE_PROPERTY(m_Slope);
}

void SEMechanicalVentilatorPressureControl::Copy(const SEMechanicalVentilatorPressureControl& src, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEMechanicalVentilatorPressureControl::ToSettings(SEMechanicalVentilatorSettings& s, const SESubstanceManager& subMgr)
{
  if (!SEMechanicalVentilatorMode::ToSettings(s, subMgr))
    return false;
  if (SEMechanicalVentilatorMode::IsActive())
  {
    // Translate ventilator settings
    double totalPeriod_s = 60.0 / GetRespirationRate(FrequencyUnit::Per_min);
    double expiratoryPeriod_s = totalPeriod_s - GetInspiratoryPeriod(TimeUnit::s);

    s.SetInspirationWaveform(eMechanicalVentilator_DriverWaveform::Ramp);
    s.SetExpirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
    s.GetInspirationWaveformPeriod().Set(GetSlope());
    s.GetInspirationMachineTriggerTime().SetValue(expiratoryPeriod_s, TimeUnit::s);
    s.GetPeakInspiratoryPressure().Set(GetInspiratoryPressure());
    s.GetPositiveEndExpiredPressure().Set(GetPositiveEndExpiredPressure());
    s.GetExpirationCycleTime().Set(GetInspiratoryPeriod());
    s.GetFractionInspiredGas(*subMgr.GetSubstance("Oxygen")).GetFractionAmount().Set(GetFractionInspiredOxygen());
    if(GetMode() == eMechanicalVentilator_PressureControlMode::AssistedControl)
      s.GetInspirationPatientTriggerFlow().SetValue(0.1, VolumePerTimeUnit::L_Per_s);
  }
  return true;
}

bool SEMechanicalVentilatorPressureControl::IsValid() const
{
  return SEMechanicalVentilatorMode::IsValid() &&
    HasFractionInspiredOxygen() &&
    HasInspiratoryPeriod() &&
    HasInspiratoryPressure() &&
    HasPositiveEndExpiredPressure() &&
    HasRespirationRate() &&
    HasSlope();
}

bool SEMechanicalVentilatorPressureControl::IsActive() const
{
  return SEMechanicalVentilatorMode::IsActive();
}
void SEMechanicalVentilatorPressureControl::Deactivate()
{
  SEMechanicalVentilatorMode::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEMechanicalVentilatorPressureControl::GetScalar(const std::string& name)
{
  if (name.compare("FractionInspiredOxygen") == 0)
    return &GetFractionInspiredOxygen();
  if (name.compare("InspiratoryPeriod") == 0)
    return &GetInspiratoryPeriod();
  if (name.compare("InspiratoryPressure") == 0)
    return &GetInspiratoryPressure();
  if (name.compare("PositiveEndExpiredPressure") == 0)
    return &GetPositiveEndExpiredPressure();
  if (name.compare("RespirationRate") == 0)
    return &GetRespirationRate();
  if (name.compare("Slope") == 0)
    return &GetSlope();
  return nullptr;
}

void SEMechanicalVentilatorPressureControl::SetMode(eMechanicalVentilator_PressureControlMode m)
{
  m_Mode = m;
}
eMechanicalVentilator_PressureControlMode SEMechanicalVentilatorPressureControl::GetMode() const
{
  return m_Mode;
}

bool SEMechanicalVentilatorPressureControl::HasFractionInspiredOxygen() const
{
  return m_FractionInspiredOxygen != nullptr;
}
SEScalar0To1& SEMechanicalVentilatorPressureControl::GetFractionInspiredOxygen()
{
  if (m_FractionInspiredOxygen == nullptr)
    m_FractionInspiredOxygen = new SEScalar0To1();
  return *m_FractionInspiredOxygen;
}
double SEMechanicalVentilatorPressureControl::GetFractionInspiredOxygen() const
{
  if (m_FractionInspiredOxygen == nullptr)
    return SEScalar::dNaN();
  return m_FractionInspiredOxygen->GetValue();
}

bool SEMechanicalVentilatorPressureControl::HasInspiratoryPeriod() const
{
  return m_InspiratoryPeriod != nullptr;
}
SEScalarTime& SEMechanicalVentilatorPressureControl::GetInspiratoryPeriod()
{
  if (m_InspiratoryPeriod == nullptr)
    m_InspiratoryPeriod = new SEScalarTime();
  return *m_InspiratoryPeriod;
}
double SEMechanicalVentilatorPressureControl::GetInspiratoryPeriod(const TimeUnit& unit) const
{
  if (m_InspiratoryPeriod == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryPeriod->GetValue(unit);
}

bool SEMechanicalVentilatorPressureControl::HasInspiratoryPressure() const
{
  return m_InspiratoryPressure != nullptr;
}
SEScalarPressure& SEMechanicalVentilatorPressureControl::GetInspiratoryPressure()
{
  if (m_InspiratoryPressure == nullptr)
    m_InspiratoryPressure = new SEScalarPressure();
  return *m_InspiratoryPressure;
}
double SEMechanicalVentilatorPressureControl::GetInspiratoryPressure(const PressureUnit& unit) const
{
  if (m_InspiratoryPressure == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryPressure->GetValue(unit);
}

bool SEMechanicalVentilatorPressureControl::HasPositiveEndExpiredPressure() const
{
  return m_PositiveEndExpiredPressure != nullptr;
}
SEScalarPressure& SEMechanicalVentilatorPressureControl::GetPositiveEndExpiredPressure()
{
  if (m_PositiveEndExpiredPressure == nullptr)
    m_PositiveEndExpiredPressure = new SEScalarPressure();
  return *m_PositiveEndExpiredPressure;
}
double SEMechanicalVentilatorPressureControl::GetPositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_PositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_PositiveEndExpiredPressure->GetValue(unit);
}

bool SEMechanicalVentilatorPressureControl::HasRespirationRate() const
{
  return m_RespirationRate != nullptr;
}
SEScalarFrequency& SEMechanicalVentilatorPressureControl::GetRespirationRate()
{
  if (m_RespirationRate == nullptr)
    m_RespirationRate = new SEScalarFrequency();
  return *m_RespirationRate;
}
double SEMechanicalVentilatorPressureControl::GetRespirationRate(const FrequencyUnit& unit) const
{
  if (m_RespirationRate == nullptr)
    return SEScalar::dNaN();
  return m_RespirationRate->GetValue(unit);
}

bool SEMechanicalVentilatorPressureControl::HasSlope() const
{
  return m_Slope != nullptr;
}
SEScalarTime& SEMechanicalVentilatorPressureControl::GetSlope()
{
  if (m_Slope == nullptr)
    m_Slope = new SEScalarTime();
  return *m_Slope;
}
double SEMechanicalVentilatorPressureControl::GetSlope(const TimeUnit& unit) const
{
  if (m_Slope == nullptr)
    return SEScalar::dNaN();
  return m_Slope->GetValue(unit);
}

void SEMechanicalVentilatorPressureControl::ToString(std::ostream& str) const
{
  str << "Mechanical Ventilator Action : Pressure Control";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tConnection: " << eSwitch_Name(m_Connection);
  str << "\n\tMode: " << eMechanicalVentilator_PressureControlMode_Name(m_Mode);
  str << "\n\tFractionInspiredOxygen :"; HasFractionInspiredOxygen() ? str << *m_FractionInspiredOxygen : str << "NaN";
  str << "\n\tInspiratoryPeriod :"; HasInspiratoryPeriod() ? str << *m_InspiratoryPeriod : str << "NaN";
  str << "\n\tInspiratoryPressure :"; HasInspiratoryPressure() ? str << *m_InspiratoryPressure : str << "NaN";
  str << "\n\tPositiveEndExpiredPressure :"; HasPositiveEndExpiredPressure() ? str << *m_PositiveEndExpiredPressure : str << "NaN";
  str << "\n\tRespirationRate :"; HasRespirationRate() ? str << *m_RespirationRate : str << "NaN";
  str << "\n\tSlope :"; HasSlope() ? str << *m_Slope : str << "NaN";
  str << std::flush;
}