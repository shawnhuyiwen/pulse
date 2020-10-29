/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEFunctionElectricPotentialVsTime.h"

SEElectroCardioGramWaveform::SEElectroCardioGramWaveform(Logger* logger) : Loggable(logger)
{
  m_TimeStep = nullptr;
  m_Data = nullptr;
  m_Rhythm = eHeartRhythm::NormalSinus;
  m_LeadNumber = eElectroCardioGram_WaveformLead::NullLead;
}

SEElectroCardioGramWaveform::~SEElectroCardioGramWaveform()
{
  m_Rhythm = eHeartRhythm::NormalSinus;
  m_LeadNumber = eElectroCardioGram_WaveformLead::NullLead;
  SAFE_DELETE(m_TimeStep);
  SAFE_DELETE(m_Data);
  m_ActiveIndicies.clear();
}

void SEElectroCardioGramWaveform::Clear()
{
  m_Rhythm = eHeartRhythm::NormalSinus;
  m_LeadNumber = eElectroCardioGram_WaveformLead::NullLead;
  INVALIDATE_PROPERTY(m_TimeStep);
  INVALIDATE_PROPERTY(m_Data);
  m_ActiveIndicies.clear();
}

eElectroCardioGram_WaveformLead SEElectroCardioGramWaveform::GetLeadNumber() const
{
  return m_LeadNumber;
}
void SEElectroCardioGramWaveform::SetLeadNumber(eElectroCardioGram_WaveformLead n)
{
  m_LeadNumber = n;
}
bool SEElectroCardioGramWaveform::HasLeadNumber() const
{
  return m_LeadNumber == eElectroCardioGram_WaveformLead::NullLead ? false : true;
}
void SEElectroCardioGramWaveform::InvalidateLeadNumber()
{
  m_LeadNumber = eElectroCardioGram_WaveformLead::NullLead;
}

eHeartRhythm SEElectroCardioGramWaveform::GetRhythm() const
{
  return m_Rhythm;
}
void SEElectroCardioGramWaveform::SetRhythm(eHeartRhythm rhythm)
{
  m_Rhythm = rhythm;
}

bool SEElectroCardioGramWaveform::HasData() const
{
  return m_Data == nullptr ? false : m_Data->IsValid();
}
SEFunctionElectricPotentialVsTime& SEElectroCardioGramWaveform::GetData()
{
  if (m_Data == nullptr)
    m_Data = new SEFunctionElectricPotentialVsTime();
  return *m_Data;
}
const SEFunctionElectricPotentialVsTime* SEElectroCardioGramWaveform::GetData() const
{
  return m_Data;
}

bool SEElectroCardioGramWaveform::HasTimeStep() const
{
  return m_TimeStep == nullptr ? false : m_TimeStep->IsValid();
}
SEScalarTime& SEElectroCardioGramWaveform::GetTimeStep()
{
  if (m_TimeStep == nullptr)
    m_TimeStep = new SEScalarTime();
  return *m_TimeStep;
}
double SEElectroCardioGramWaveform::GetTimeStep(const TimeUnit& unit) const
{
  if (m_TimeStep == nullptr)
    return SEScalar::dNaN();
  return m_TimeStep->GetValue(unit);
}


