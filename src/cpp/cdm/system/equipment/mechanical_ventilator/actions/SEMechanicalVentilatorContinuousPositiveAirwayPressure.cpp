/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorContinuousPositiveAirwayPressure.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorContinuousPositiveAirwayPressure::SEMechanicalVentilatorContinuousPositiveAirwayPressure(Logger* logger) : SEMechanicalVentilatorMode(logger)
{
  m_DeltaPressureSupport = nullptr;
  m_FractionInspiredOxygen = nullptr;
  m_PositiveEndExpiredPressure = nullptr;
  m_Slope = nullptr;
}

SEMechanicalVentilatorContinuousPositiveAirwayPressure::~SEMechanicalVentilatorContinuousPositiveAirwayPressure()
{
  SAFE_DELETE(m_DeltaPressureSupport);
  SAFE_DELETE(m_FractionInspiredOxygen);
  SAFE_DELETE(m_PositiveEndExpiredPressure);
  SAFE_DELETE(m_Slope);
}

void SEMechanicalVentilatorContinuousPositiveAirwayPressure::Clear()
{
  SEMechanicalVentilatorMode::Clear();
  INVALIDATE_PROPERTY(m_DeltaPressureSupport);
  INVALIDATE_PROPERTY(m_FractionInspiredOxygen);
  INVALIDATE_PROPERTY(m_PositiveEndExpiredPressure);
  INVALIDATE_PROPERTY(m_Slope);
}

void SEMechanicalVentilatorContinuousPositiveAirwayPressure::Copy(const SEMechanicalVentilatorContinuousPositiveAirwayPressure& src, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::ToSettings(SEMechanicalVentilatorSettings& s, const SESubstanceManager& subMgr)
{
  if (!SEMechanicalVentilatorMode::ToSettings(s, subMgr))
    return false;
  if (SEMechanicalVentilatorMode::IsActive())
  {
    // Translate ventilator settings
    double peakInspiratoryPressure_cmH2O =
      GetPositiveEndExpiredPressure(PressureUnit::cmH2O) + GetDeltaPressureSupport(PressureUnit::cmH2O);

    s.SetInspirationWaveform(eMechanicalVentilator_DriverWaveform::Ramp);
    s.SetExpirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
    s.GetInspirationWaveformPeriod().Set(GetSlope());
    s.GetPeakInspiratoryPressure().SetValue(peakInspiratoryPressure_cmH2O, PressureUnit::cmH2O);
    s.GetPositiveEndExpiredPressure().Set(GetPositiveEndExpiredPressure());
    s.GetFractionInspiredGas(*subMgr.GetSubstance("Oxygen")).GetFractionAmount().Set(GetFractionInspiredOxygen());
    s.SetExpirationCycleRespiratoryModel(eSwitch::On);
    s.SetInspirationPatientTriggerRespiratoryModel(eSwitch::On);
  }
  return true;
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::IsValid() const
{
  return SEMechanicalVentilatorMode::IsValid() &&
    HasDeltaPressureSupport() &&
    HasFractionInspiredOxygen() &&
    HasPositiveEndExpiredPressure() &&
    HasSlope();
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::IsActive() const
{
  return SEMechanicalVentilatorMode::IsActive();
}
void SEMechanicalVentilatorContinuousPositiveAirwayPressure::Deactivate()
{
  SEMechanicalVentilatorMode::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetScalar(const std::string& name)
{
  if (name.compare("DeltaPressureSupport") == 0)
    return &GetDeltaPressureSupport();
  if (name.compare("FractionInspiredOxygen") == 0)
    return &GetFractionInspiredOxygen();
  if (name.compare("PositiveEndExpiredPressure") == 0)
    return &GetPositiveEndExpiredPressure();
  if (name.compare("Slope") == 0)
    return &GetSlope();
  return nullptr;
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasDeltaPressureSupport() const
{
  return m_DeltaPressureSupport != nullptr;
}
SEScalarPressure& SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetDeltaPressureSupport()
{
  if (m_DeltaPressureSupport == nullptr)
    m_DeltaPressureSupport = new SEScalarPressure();
  return *m_DeltaPressureSupport;
}
double SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetDeltaPressureSupport(const PressureUnit& unit) const
{
  if (m_DeltaPressureSupport == nullptr)
    return SEScalar::dNaN();
  return m_DeltaPressureSupport->GetValue(unit);
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasFractionInspiredOxygen() const
{
  return m_FractionInspiredOxygen != nullptr;
}
SEScalar0To1& SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetFractionInspiredOxygen()
{
  if (m_FractionInspiredOxygen == nullptr)
    m_FractionInspiredOxygen = new SEScalar0To1();
  return *m_FractionInspiredOxygen;
}
double SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetFractionInspiredOxygen() const
{
  if (m_FractionInspiredOxygen == nullptr)
    return SEScalar::dNaN();
  return m_FractionInspiredOxygen->GetValue();
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasPositiveEndExpiredPressure() const
{
  return m_PositiveEndExpiredPressure != nullptr;
}
SEScalarPressure& SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetPositiveEndExpiredPressure()
{
  if (m_PositiveEndExpiredPressure == nullptr)
    m_PositiveEndExpiredPressure = new SEScalarPressure();
  return *m_PositiveEndExpiredPressure;
}
double SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetPositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_PositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_PositiveEndExpiredPressure->GetValue(unit);
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasSlope() const
{
  return m_Slope != nullptr;
}
SEScalarTime& SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetSlope()
{
  if (m_Slope == nullptr)
    m_Slope = new SEScalarTime();
  return *m_Slope;
}
double SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetSlope(const TimeUnit& unit) const
{
  if (m_Slope == nullptr)
    return SEScalar::dNaN();
  return m_Slope->GetValue(unit);
}

void SEMechanicalVentilatorContinuousPositiveAirwayPressure::ToString(std::ostream& str) const
{
  str << "Mechanical Ventilator Action : Continuous Positive Airway Pressure";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tConnection: " << eSwitch_Name(m_Connection);
  str << "\n\tDeltaPressureSupport :"; HasDeltaPressureSupport() ? str << *m_DeltaPressureSupport : str << "NaN";
  str << "\n\tFractionInspiredOxygen :"; HasFractionInspiredOxygen() ? str << *m_FractionInspiredOxygen : str << "NaN";
  str << "\n\tPositiveEndExpiredPressure :"; HasPositiveEndExpiredPressure() ? str << *m_PositiveEndExpiredPressure : str << "NaN";
  str << "\n\tSlope :"; HasSlope() ? str << *m_Slope : str << "NaN";
  str << std::flush;
}