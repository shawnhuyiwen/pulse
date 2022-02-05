/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"
#include "cdm/properties/SEArrayElectricPotential.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
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
}

void SEElectroCardioGramWaveform::Clear()
{
  m_Type = eElectroCardioGram_WaveformType::Sinus;
  m_LeadNumber = eElectroCardioGram_WaveformLead::NullLead;
  INVALIDATE_PROPERTY(m_OriginalData);
  INVALIDATE_PROPERTY(m_ActiveCycle);
  m_ActiveIndex = 0;
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

void SEElectroCardioGramWaveform::GenerateActiveCycle(const SEScalarFrequency& hr, double amplitudeModifier)
{
  double bps = (hr.IsZero()) ?
    1.0 : hr.GetValue(FrequencyUnit::Per_s);

  double cycleTime_s = 1 / bps;
  size_t numCyclePoints = std::ceil(cycleTime_s/0.02)+1;

  GetActiveCycle().Copy(GetOriginalData());

  // Interpolate original to our desired cycle points
  //GeneralMath::SplineInterpolater(GetActiveCycle().GetData(), numCyclePoints);
  //GeneralMath::LinearInterpolator(GetActiveCycle().GetData(), numCyclePoints);
  GeneralMath::LinearInterpolator1(GetActiveCycle().GetData(), numCyclePoints);

  m_ActiveIndex = 0;
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
        m_ActiveCycle->Clear();
      }
    }
  }
}
