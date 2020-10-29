/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/assessments/SEPulmonaryFunctionTest.h"
#include "patient/SEPatient.h"
#include "system/physiology/SERespiratorySystem.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "io/protobuf/PBPatientAssessments.h"

SEPulmonaryFunctionTest::SEPulmonaryFunctionTest(Logger* logger) : SEPatientAssessment(logger)
{
  m_NumberOfPlotPoints = 100;
  m_ExpiratoryReserveVolume=nullptr;
  m_ForcedVitalCapacity=nullptr;
  m_ForcedExpiratoryVolume=nullptr;
  m_ForcedExpiratoryFlow=nullptr;
  m_InspiratoryCapacity=nullptr;
  m_InspiratoryReserveVolume=nullptr;
  m_PeakExpiratoryFlow=nullptr;
  m_MaximumVoluntaryVentilation=nullptr;
  m_SlowVitalCapacity=nullptr;
  m_TotalLungCapacity=nullptr;
  m_FunctionalResidualCapacity=nullptr;
  m_ResidualVolume=nullptr;
  m_VitalCapacity=nullptr;

  m_LungVolumePlot=nullptr;
}

SEPulmonaryFunctionTest::~SEPulmonaryFunctionTest()
{
  SAFE_DELETE(m_ExpiratoryReserveVolume);
  SAFE_DELETE(m_ForcedVitalCapacity);
  SAFE_DELETE(m_ForcedExpiratoryVolume);
  SAFE_DELETE(m_ForcedExpiratoryFlow);
  SAFE_DELETE(m_InspiratoryCapacity);
  SAFE_DELETE(m_InspiratoryReserveVolume);
  SAFE_DELETE(m_PeakExpiratoryFlow);
  SAFE_DELETE(m_MaximumVoluntaryVentilation);
  SAFE_DELETE(m_SlowVitalCapacity);
  SAFE_DELETE(m_TotalLungCapacity);
  SAFE_DELETE(m_FunctionalResidualCapacity);
  SAFE_DELETE(m_ResidualVolume);
  SAFE_DELETE(m_VitalCapacity);

  SAFE_DELETE(m_LungVolumePlot);
}

void SEPulmonaryFunctionTest::Clear()
{
  SEPatientAssessment::Clear();
  INVALIDATE_PROPERTY(m_ExpiratoryReserveVolume);
  INVALIDATE_PROPERTY(m_ForcedVitalCapacity);
  INVALIDATE_PROPERTY(m_ForcedExpiratoryVolume);
  INVALIDATE_PROPERTY(m_ForcedExpiratoryFlow);
  INVALIDATE_PROPERTY(m_InspiratoryCapacity);
  INVALIDATE_PROPERTY(m_InspiratoryReserveVolume);
  INVALIDATE_PROPERTY(m_PeakExpiratoryFlow);
  INVALIDATE_PROPERTY(m_MaximumVoluntaryVentilation);
  INVALIDATE_PROPERTY(m_SlowVitalCapacity);
  INVALIDATE_PROPERTY(m_TotalLungCapacity);
  INVALIDATE_PROPERTY(m_FunctionalResidualCapacity);
  INVALIDATE_PROPERTY(m_ResidualVolume);
  INVALIDATE_PROPERTY(m_VitalCapacity);

  INVALIDATE_PROPERTY(m_LungVolumePlot);
}

bool SEPulmonaryFunctionTest::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBPatientAssessment::SerializeToString(*this, output, m);
}
bool SEPulmonaryFunctionTest::SerializeToFile(const std::string& filename) const
{
  return PBPatientAssessment::SerializeToFile(*this, filename);
}

bool SEPulmonaryFunctionTest::HasExpiratoryReserveVolume() const
{
  return m_ExpiratoryReserveVolume==nullptr?false:m_ExpiratoryReserveVolume->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetExpiratoryReserveVolume()
{
  if(m_ExpiratoryReserveVolume==nullptr)
    m_ExpiratoryReserveVolume = new SEScalarVolume();
  return *m_ExpiratoryReserveVolume;
}
double SEPulmonaryFunctionTest::GetExpiratoryReserveVolume(const VolumeUnit& unit) const
{
  if (!HasExpiratoryReserveVolume())
    return SEScalar::dNaN();
  return m_ExpiratoryReserveVolume->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasForcedVitalCapacity() const
{
  return m_ForcedVitalCapacity==nullptr?false:m_ForcedVitalCapacity->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetForcedVitalCapacity()
{
  if(m_ForcedVitalCapacity==nullptr)
    m_ForcedVitalCapacity = new SEScalarVolume();
  return *m_ForcedVitalCapacity;
}
double SEPulmonaryFunctionTest::GetForcedVitalCapacity(const VolumeUnit& unit) const
{
  if (!HasForcedVitalCapacity())
    return SEScalar::dNaN();
  return m_ForcedVitalCapacity->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasForcedExpiratoryVolume() const
{
  return m_ForcedExpiratoryVolume==nullptr?false:m_ForcedExpiratoryVolume->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetForcedExpiratoryVolume()
{
  if(m_ForcedExpiratoryVolume==nullptr)
    m_ForcedExpiratoryVolume = new SEScalarVolume();
  return *m_ForcedExpiratoryVolume;
}
double SEPulmonaryFunctionTest::GetForcedExpiratoryVolume(const VolumeUnit& unit) const
{
  if (!HasForcedExpiratoryVolume())
    return SEScalar::dNaN();
  return m_ForcedExpiratoryVolume->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasForcedExpiratoryFlow() const
{
  return m_ForcedExpiratoryFlow==nullptr?false:m_ForcedExpiratoryFlow->IsValid();
}
SEScalarVolumePerTime& SEPulmonaryFunctionTest::GetForcedExpiratoryFlow()
{
  if(m_ForcedExpiratoryFlow==nullptr)
    m_ForcedExpiratoryFlow = new SEScalarVolumePerTime();
  return *m_ForcedExpiratoryFlow;
}
double SEPulmonaryFunctionTest::GetForcedExpiratoryFlow(const VolumePerTimeUnit& unit) const
{
  if (!HasForcedExpiratoryFlow())
    return SEScalar::dNaN();
  return m_ForcedExpiratoryFlow->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasInspiratoryCapacity() const
{
  return m_InspiratoryCapacity==nullptr?false:m_InspiratoryCapacity->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetInspiratoryCapacity()
{
  if(m_InspiratoryCapacity==nullptr)
    m_InspiratoryCapacity = new SEScalarVolume();
  return *m_InspiratoryCapacity;
}
double SEPulmonaryFunctionTest::GetInspiratoryCapacity(const VolumeUnit& unit) const
{
  if (!HasInspiratoryCapacity())
    return SEScalar::dNaN();
  return m_InspiratoryCapacity->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasInspiratoryReserveVolume() const
{
  return m_InspiratoryReserveVolume==nullptr?false:m_InspiratoryReserveVolume->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetInspiratoryReserveVolume()
{
  if(m_InspiratoryReserveVolume==nullptr)
    m_InspiratoryReserveVolume = new SEScalarVolume();
  return *m_InspiratoryReserveVolume;
}
double SEPulmonaryFunctionTest::GetInspiratoryReserveVolume(const VolumeUnit& unit) const
{
  if (!HasInspiratoryReserveVolume())
    return SEScalar::dNaN();
  return m_InspiratoryReserveVolume->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasPeakExpiratoryFlow() const
{
  return m_PeakExpiratoryFlow==nullptr?false:m_PeakExpiratoryFlow->IsValid();
}
SEScalarVolumePerTime& SEPulmonaryFunctionTest::GetPeakExpiratoryFlow()
{
  if(m_PeakExpiratoryFlow==nullptr)
    m_PeakExpiratoryFlow = new SEScalarVolumePerTime();
  return *m_PeakExpiratoryFlow;
}
double SEPulmonaryFunctionTest::GetPeakExpiratoryFlow(const VolumePerTimeUnit& unit) const
{
  if (!HasPeakExpiratoryFlow())
    return SEScalar::dNaN();
  return m_PeakExpiratoryFlow->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasMaximumVoluntaryVentilation() const
{
  return m_MaximumVoluntaryVentilation==nullptr?false:m_MaximumVoluntaryVentilation->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetMaximumVoluntaryVentilation()
{
  if(m_MaximumVoluntaryVentilation==nullptr)
    m_MaximumVoluntaryVentilation = new SEScalarVolume();
  return *m_MaximumVoluntaryVentilation;
}
double SEPulmonaryFunctionTest::GetMaximumVoluntaryVentilation(const VolumeUnit& unit) const
{
  if (!HasMaximumVoluntaryVentilation())
    return SEScalar::dNaN();
  return m_MaximumVoluntaryVentilation->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasSlowVitalCapacity() const
{
  return m_SlowVitalCapacity==nullptr?false:m_SlowVitalCapacity->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetSlowVitalCapacity()
{
  if(m_SlowVitalCapacity==nullptr)
    m_SlowVitalCapacity = new SEScalarVolume();
  return *m_SlowVitalCapacity;
}
double SEPulmonaryFunctionTest::GetSlowVitalCapacity(const VolumeUnit& unit) const
{
  if (!HasSlowVitalCapacity())
    return SEScalar::dNaN();
  return m_SlowVitalCapacity->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasTotalLungCapacity() const
{
  return m_TotalLungCapacity==nullptr?false:m_TotalLungCapacity->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetTotalLungCapacity()
{
  if(m_TotalLungCapacity==nullptr)
    m_TotalLungCapacity = new SEScalarVolume();
  return *m_TotalLungCapacity;
}
double SEPulmonaryFunctionTest::GetTotalLungCapacity(const VolumeUnit& unit) const
{
  if (!HasTotalLungCapacity())
    return SEScalar::dNaN();
  return m_TotalLungCapacity->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasFunctionalResidualCapacity() const
{
  return m_FunctionalResidualCapacity==nullptr?false:m_FunctionalResidualCapacity->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetFunctionalResidualCapacity()
{
  if(m_FunctionalResidualCapacity==nullptr)
    m_FunctionalResidualCapacity = new SEScalarVolume();
  return *m_FunctionalResidualCapacity;
}
double SEPulmonaryFunctionTest::GetFunctionalResidualCapacity(const VolumeUnit& unit) const
{
  if (!HasFunctionalResidualCapacity())
    return SEScalar::dNaN();
  return m_FunctionalResidualCapacity->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasResidualVolume() const
{
  return m_ResidualVolume==nullptr?false:m_ResidualVolume->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetResidualVolume()
{
  if(m_ResidualVolume==nullptr)
    m_ResidualVolume = new SEScalarVolume();
  return *m_ResidualVolume;
}
double SEPulmonaryFunctionTest::GetResidualVolume(const VolumeUnit& unit) const
{
  if (!HasResidualVolume())
    return SEScalar::dNaN();
  return m_ResidualVolume->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasVitalCapacity() const
{
  return m_VitalCapacity==nullptr?false:m_VitalCapacity->IsValid();
}
SEScalarVolume& SEPulmonaryFunctionTest::GetVitalCapacity()
{
  if(m_VitalCapacity==nullptr)
    m_VitalCapacity = new SEScalarVolume();
  return *m_VitalCapacity;
}
double SEPulmonaryFunctionTest::GetVitalCapacity(const VolumeUnit& unit) const
{
  if (!HasVitalCapacity())
    return SEScalar::dNaN();
  return m_VitalCapacity->GetValue(unit);
}

bool SEPulmonaryFunctionTest::HasLungVolumePlot() const
{
  return m_LungVolumePlot==nullptr?false:m_LungVolumePlot->IsValid();
}
SEFunctionVolumeVsTime& SEPulmonaryFunctionTest::GetLungVolumePlot()
{
  if(m_LungVolumePlot==nullptr)
    m_LungVolumePlot = new SEFunctionVolumeVsTime();
  return *m_LungVolumePlot;
}
const SEFunctionVolumeVsTime* SEPulmonaryFunctionTest::GetLungVolumePlot() const
{
  if (!HasLungVolumePlot())
    return nullptr;
  return m_LungVolumePlot;
}



