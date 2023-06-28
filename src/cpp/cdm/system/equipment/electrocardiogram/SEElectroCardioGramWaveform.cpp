/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"
#include "cdm/properties/SEArrayElectricPotential.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/GeneralMath.h"

SEElectroCardioGramWaveform::SEElectroCardioGramWaveform(Logger* logger) : Loggable(logger)
{
  m_OriginalData = nullptr;
  m_ActiveCycle = nullptr;
  Clear();
}

SEElectroCardioGramWaveform::~SEElectroCardioGramWaveform()
{
  m_Type = eElectroCardioGram_WaveformType::Sinus;
  m_LeadNumber = eElectroCardioGram_WaveformLead::NullLead;
  SAFE_DELETE(m_OriginalData);
  SAFE_DELETE(m_ActiveCycle);
  m_ActiveIndex = 0;
  m_Recycling = false;
}

void SEElectroCardioGramWaveform::Clear()
{
  m_Type = eElectroCardioGram_WaveformType::Sinus;
  m_LeadNumber = eElectroCardioGram_WaveformLead::NullLead;
  INVALIDATE_PROPERTY(m_OriginalData);
  INVALIDATE_PROPERTY(m_ActiveCycle);
  m_ActiveIndex = 0;
  m_Recycling = false;
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

eElectroCardioGram_WaveformType SEElectroCardioGramWaveform::GetType() const
{
  return m_Type;
}
void SEElectroCardioGramWaveform::SetType(eElectroCardioGram_WaveformType t)
{
  m_Type = t;
}

bool SEElectroCardioGramWaveform::HasOriginalData() const
{
  return m_OriginalData == nullptr ? false : m_OriginalData->IsValid();
}
SEArrayElectricPotential& SEElectroCardioGramWaveform::GetOriginalData()
{
  if (m_OriginalData == nullptr)
    m_OriginalData = new SEArrayElectricPotential();
  return *m_OriginalData;
}
const SEArrayElectricPotential* SEElectroCardioGramWaveform::GetOriginalData() const
{
  return m_OriginalData;
}

bool SEElectroCardioGramWaveform::HasActiveCycle() const
{
  return m_ActiveCycle == nullptr ? false : m_ActiveCycle->IsValid();
}
SEArrayElectricPotential& SEElectroCardioGramWaveform::GetActiveCycle()
{
  if (m_ActiveCycle == nullptr)
    m_ActiveCycle = new SEArrayElectricPotential();
  return *m_ActiveCycle;
}
const SEArrayElectricPotential* SEElectroCardioGramWaveform::GetActiveCycle() const
{
  return m_ActiveCycle;
}

void SEElectroCardioGramWaveform::GenerateActiveCycle(const SEScalarFrequency& hr, const SEScalarTime& step, double amplitudeModifier)
{
  // Get a copy of the original data
  GetActiveCycle().Copy(GetOriginalData());
  if (!hr.IsZero())
  {
    double bps = hr.GetValue(FrequencyUnit::Per_s);
    double cycleTime_s = 1 / bps;
    size_t numCyclePoints = (size_t)(std::ceil(cycleTime_s / step.GetValue(TimeUnit::s))+1);
    // Interpolate original to our desired cycle points
    GeneralMath::LinearInterpolator1(GetActiveCycle().GetData(), numCyclePoints);
    if (m_ActiveIndex >= numCyclePoints)
      m_ActiveIndex = 0;
  }
  // Apply the amplitudeModifier
  for(size_t i=0; i< GetActiveCycle().GetData().size(); i++)
    GetActiveCycle().GetData()[i] *= amplitudeModifier;

  if (!m_Recycling)// We are starting a new cycle before our cycle ended...
    m_ActiveIndex = 0;// Cut the last cycle short, and start the current cycle
  m_Recycling = false;
}

void SEElectroCardioGramWaveform::GetCycleValue(SEScalarElectricPotential& v, bool advance)
{
  if (!HasActiveCycle())
    v.SetValue(0, ElectricPotentialUnit::mV);
  else
  {
    v.SetValue(m_ActiveCycle->GetData()[m_ActiveIndex], *m_ActiveCycle->GetUnit());
    if (advance)
    {
      m_ActiveIndex++;
      if (m_ActiveIndex >= m_ActiveCycle->GetData().size())
      {
        m_ActiveIndex = 0;
        // We have completed the cardiac cycle waveform and a new cardica cycle has not started yet...
        // So use the same waveform, and when the new cycle starts, just use that new waveform at the active index
        m_Recycling = true;
      }
    }
  }
}
