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
  m_InspirationWaveform = eDriverWaveform::NullDriverWaveform;
  m_InspirationPatientTriggerFlow = nullptr;
  m_InspirationPatientTriggerPressure = nullptr;
  m_InspirationPatientTriggerRespiratoryModel = eSwitch::Off;
  m_ExpirationWaveform = eDriverWaveform::NullDriverWaveform;
  m_ExpirationCycleFlow = nullptr;
  m_ExpirationCyclePressure = nullptr;
  m_ExpirationCycleRespiratoryModel = eSwitch::Off;
}

SEMechanicalVentilatorContinuousPositiveAirwayPressure::~SEMechanicalVentilatorContinuousPositiveAirwayPressure()
{
  SAFE_DELETE(m_DeltaPressureSupport);
  SAFE_DELETE(m_FractionInspiredOxygen);
  SAFE_DELETE(m_PositiveEndExpiredPressure);
  SAFE_DELETE(m_Slope);
  m_InspirationWaveform = eDriverWaveform::NullDriverWaveform;
  SAFE_DELETE(m_InspirationPatientTriggerFlow);
  SAFE_DELETE(m_InspirationPatientTriggerPressure);
  m_InspirationPatientTriggerRespiratoryModel = eSwitch::Off;
  m_ExpirationWaveform = eDriverWaveform::NullDriverWaveform;
  SAFE_DELETE(m_ExpirationCycleFlow);
  SAFE_DELETE(m_ExpirationCyclePressure);
  m_ExpirationCycleRespiratoryModel = eSwitch::Off;
}

void SEMechanicalVentilatorContinuousPositiveAirwayPressure::Clear()
{
  SEMechanicalVentilatorMode::Clear();
  INVALIDATE_PROPERTY(m_DeltaPressureSupport);
  INVALIDATE_PROPERTY(m_FractionInspiredOxygen);
  INVALIDATE_PROPERTY(m_PositiveEndExpiredPressure);
  INVALIDATE_PROPERTY(m_Slope);
  m_InspirationWaveform = eDriverWaveform::NullDriverWaveform;
  INVALIDATE_PROPERTY(m_InspirationPatientTriggerFlow);
  INVALIDATE_PROPERTY(m_InspirationPatientTriggerPressure);
  m_InspirationPatientTriggerRespiratoryModel = eSwitch::Off;
  m_ExpirationWaveform = eDriverWaveform::NullDriverWaveform;
  INVALIDATE_PROPERTY(m_ExpirationCycleFlow);
  INVALIDATE_PROPERTY(m_ExpirationCyclePressure);
  m_ExpirationCycleRespiratoryModel = eSwitch::Off;
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
    double inspirationWaveformPeriod_s = 0.0;
    if (HasSlope())
    {
      inspirationWaveformPeriod_s = GetSlope(TimeUnit::s);
    }

    double positiveEndExpiredPressure_cmH2O = GetPositiveEndExpiredPressure(PressureUnit::cmH2O);
    double peakInspiratoryPressure_cmH2O =
        positiveEndExpiredPressure_cmH2O + GetDeltaPressureSupport(PressureUnit::cmH2O);
    if (positiveEndExpiredPressure_cmH2O > peakInspiratoryPressure_cmH2O)
    {
        Fatal("Positive End Expired Pressure cannot be higher than the Peak Inspiratory Pressure.");
    }

    s.SetInspirationWaveform(GetInspirationWaveform());
    s.SetExpirationWaveform(eDriverWaveform::Square);
    s.GetInspirationWaveformPeriod().SetValue(inspirationWaveformPeriod_s, TimeUnit::s);
    s.GetPeakInspiratoryPressure().SetValue(peakInspiratoryPressure_cmH2O, PressureUnit::cmH2O);
    s.GetPositiveEndExpiredPressure().SetValue(positiveEndExpiredPressure_cmH2O, PressureUnit::cmH2O);
    s.GetFractionInspiredGas(*subMgr.GetSubstance("Oxygen")).GetFractionAmount().Set(GetFractionInspiredOxygen());
    s.SetInspirationPatientTriggerRespiratoryModel(GetInspirationPatientTriggerRespiratoryModel());
    if (HasInspirationPatientTriggerPressure())
    {
      s.GetInspirationPatientTriggerPressure().Set(GetInspirationPatientTriggerPressure());
    }
    if (HasInspirationPatientTriggerFlow())
    {
      s.GetInspirationPatientTriggerFlow().Set(GetInspirationPatientTriggerFlow());
    }
    s.SetExpirationCycleRespiratoryModel(GetExpirationCycleRespiratoryModel());
    if (HasExpirationCyclePressure())
    {
      s.GetExpirationCyclePressure().Set(GetExpirationCyclePressure());
    }
    if (HasExpirationCycleFlow())
    {
      s.GetExpirationCycleFlow().Set(GetExpirationCycleFlow());
    }
  }
  return true;
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::IsValid() const
{
  return SEMechanicalVentilatorMode::IsValid() &&
    HasDeltaPressureSupport() &&
    HasFractionInspiredOxygen() &&
    HasPositiveEndExpiredPressure() &&
    HasExpirationWaveform() &&
    HasInspirationWaveform() &&
    ( HasInspirationPatientTriggerPressure() ||
      HasInspirationPatientTriggerFlow() ||
      GetInspirationPatientTriggerRespiratoryModel() == eSwitch::On) &&
    ( HasExpirationCyclePressure() ||
      HasExpirationCycleFlow() ||
      GetExpirationCycleRespiratoryModel() == eSwitch::On);
    //Slope is optional
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
  //if (name.compare("ExpirationWaveform") == 0)
  //  return &GetExpirationWaveform();
  if (name.compare("ExpirationCycleFlow") == 0)
    return &GetExpirationCycleFlow();
  if (name.compare("ExpirationCyclePressure") == 0)
    return &GetExpirationCyclePressure();
  //if (name.compare("ExpirationCycleRespiratoryModel") == 0)
  //  return &GetExpirationCycleRespiratoryModel();
  if (name.compare("FractionInspiredOxygen") == 0)
    return &GetFractionInspiredOxygen();
  //if (name.compare("InspirationWaveform") == 0)
  //  return &GetInspirationWaveform();
  if (name.compare("InspirationPatientTriggerFlow") == 0)
    return &GetInspirationPatientTriggerFlow();
  if (name.compare("InspirationPatientTriggerPressure") == 0)
    return &GetInspirationPatientTriggerPressure();
  //if (name.compare("InspirationPatientTriggerRespiratoryModel") == 0)
  //  return &GetInspirationPatientTriggerRespiratoryModel();
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

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasInspirationWaveform() const
{
  return m_InspirationWaveform != eDriverWaveform::NullDriverWaveform;
}
eDriverWaveform SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetInspirationWaveform() const
{
  return m_InspirationWaveform;
}
void SEMechanicalVentilatorContinuousPositiveAirwayPressure::SetInspirationWaveform(eDriverWaveform w)
{
  m_InspirationWaveform = w;
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasInspirationPatientTriggerFlow() const
{
  return m_InspirationPatientTriggerFlow == nullptr ? false : m_InspirationPatientTriggerFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetInspirationPatientTriggerFlow()
{
  if (m_InspirationPatientTriggerFlow == nullptr)
    m_InspirationPatientTriggerFlow = new SEScalarVolumePerTime();
  return *m_InspirationPatientTriggerFlow;
}
double SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetInspirationPatientTriggerFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspirationPatientTriggerFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspirationPatientTriggerFlow->GetValue(unit);
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasInspirationPatientTriggerPressure() const
{
  return m_InspirationPatientTriggerPressure == nullptr ? false : m_InspirationPatientTriggerPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetInspirationPatientTriggerPressure()
{
  if (m_InspirationPatientTriggerPressure == nullptr)
    m_InspirationPatientTriggerPressure = new SEScalarPressure();
  return *m_InspirationPatientTriggerPressure;
}
double SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetInspirationPatientTriggerPressure(const PressureUnit& unit) const
{
  if (m_InspirationPatientTriggerPressure == nullptr)
    return SEScalar::dNaN();
  return m_InspirationPatientTriggerPressure->GetValue(unit);
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasInspirationPatientTriggerRespiratoryModel() const
{
  return m_InspirationPatientTriggerRespiratoryModel != eSwitch::NullSwitch;
}
eSwitch SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetInspirationPatientTriggerRespiratoryModel() const
{
  return m_InspirationPatientTriggerRespiratoryModel;
}
void SEMechanicalVentilatorContinuousPositiveAirwayPressure::SetInspirationPatientTriggerRespiratoryModel(eSwitch c)
{
  m_InspirationPatientTriggerRespiratoryModel = c;
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasExpirationWaveform() const
{
  return m_ExpirationWaveform != eDriverWaveform::NullDriverWaveform;
}
eDriverWaveform SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetExpirationWaveform() const
{
  return m_ExpirationWaveform;
}
void SEMechanicalVentilatorContinuousPositiveAirwayPressure::SetExpirationWaveform(eDriverWaveform w)
{
  m_ExpirationWaveform = w;
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasExpirationCycleFlow() const
{
  return m_ExpirationCycleFlow == nullptr ? false : m_ExpirationCycleFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetExpirationCycleFlow()
{
  if (m_ExpirationCycleFlow == nullptr)
    m_ExpirationCycleFlow = new SEScalarVolumePerTime();
  return *m_ExpirationCycleFlow;
}
double SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetExpirationCycleFlow(const VolumePerTimeUnit& unit) const
{
  if (m_ExpirationCycleFlow == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCycleFlow->GetValue(unit);
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasExpirationCyclePressure() const
{
  return m_ExpirationCyclePressure == nullptr ? false : m_ExpirationCyclePressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetExpirationCyclePressure()
{
  if (m_ExpirationCyclePressure == nullptr)
    m_ExpirationCyclePressure = new SEScalarPressure();
  return *m_ExpirationCyclePressure;
}
double SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetExpirationCyclePressure(const PressureUnit& unit) const
{
  if (m_ExpirationCyclePressure == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCyclePressure->GetValue(unit);
}

bool SEMechanicalVentilatorContinuousPositiveAirwayPressure::HasExpirationCycleRespiratoryModel() const
{
  return m_ExpirationCycleRespiratoryModel != eSwitch::NullSwitch;
}
void SEMechanicalVentilatorContinuousPositiveAirwayPressure::SetExpirationCycleRespiratoryModel(eSwitch c)
{
  m_ExpirationCycleRespiratoryModel = c;
}
eSwitch SEMechanicalVentilatorContinuousPositiveAirwayPressure::GetExpirationCycleRespiratoryModel() const
{
  return m_ExpirationCycleRespiratoryModel;
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
  str << "\n\tInspirationWaveform: " << eDriverWaveform_Name(GetInspirationWaveform());
  str << "\n\tInspirationPatientTriggerFlow: "; HasInspirationPatientTriggerFlow() ? str << m_InspirationPatientTriggerFlow : str << "NaN";
  str << "\n\tInspirationPatientTriggerPressure: "; HasInspirationPatientTriggerPressure() ? str << m_InspirationPatientTriggerPressure : str << "NaN";
  str << "\n\tInspirationPatientTriggerRespiratoryModel: " << eSwitch_Name(m_InspirationPatientTriggerRespiratoryModel);
  str << "\n\tExpirationWaveform: " << eDriverWaveform_Name(GetExpirationWaveform());
  str << "\n\tExpirationCycleFlow: "; HasExpirationCycleFlow() ? str << m_ExpirationCycleFlow : str << "NaN";
  str << "\n\tExpirationCyclePressure: "; HasExpirationCyclePressure() ? str << m_ExpirationCyclePressure : str << "NaN";
  str << "\n\tExpirationCycleRespiratoryModel: " << eSwitch_Name(m_ExpirationCycleRespiratoryModel);
  str << std::flush;
}