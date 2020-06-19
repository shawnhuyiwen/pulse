/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveformInterpolator.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEFunctionElectricPotentialVsTime.h"
#include "io/protobuf/PBElectroCardioGram.h"
#include "utils/GeneralMath.h"

SEElectroCardioGramWaveformInterpolator::SEElectroCardioGramWaveformInterpolator(Logger* logger) : Loggable(logger)
{

}

SEElectroCardioGramWaveformInterpolator::~SEElectroCardioGramWaveformInterpolator()
{
  Clear();
}

void SEElectroCardioGramWaveformInterpolator::Clear()
{
  for (auto i : m_Waveforms)
    for (auto j : i.second)
      delete j.second;
  m_Waveforms.clear();
  m_Leads.clear();
}

void SEElectroCardioGramWaveformInterpolator::Copy(const SEElectroCardioGramWaveformInterpolator& src)
{
  PBElectroCardioGram::Copy(src, *this);
}

bool SEElectroCardioGramWaveformInterpolator::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBElectroCardioGram::SerializeToString(*this, output, m);
}
bool SEElectroCardioGramWaveformInterpolator::SerializeToFile(const std::string& filename) const
{
  return PBElectroCardioGram::SerializeToFile(*this, filename);
}
bool SEElectroCardioGramWaveformInterpolator::SerializeFromString(const std::string& src, SerializationFormat m, const SEScalarTime* timeStep)
{
  if (!PBElectroCardioGram::SerializeFromString(src, *this, m))
    return false;
  if (timeStep != nullptr)
    Interpolate(*timeStep);
  return true;
}
bool SEElectroCardioGramWaveformInterpolator::SerializeFromFile(const std::string& filename, const SEScalarTime* timeStep)
{
  if (!PBElectroCardioGram::SerializeFromFile(filename, *this))
    return false;
  if (timeStep != nullptr)
    Interpolate(*timeStep);
  return true;
}

void SEElectroCardioGramWaveformInterpolator::Interpolate(const SEScalarTime& timeStep)
{
  for (auto& l : m_Waveforms)
    for (auto& w : l.second)
      if (w.second != nullptr)
        Interpolate(*w.second, timeStep);
}
void SEElectroCardioGramWaveformInterpolator::Interpolate(SEElectroCardioGramWaveform& w, const SEScalarTime& timeStep)
{
  //If the wavefore has a time-step, use it
  //The default waveform that is deployed does not have a time-step specified
  SEFunctionElectricPotentialVsTime& data = w.GetData();
  SEScalarTime* waveformTimeStep = nullptr;
  if (w.HasTimeStep())
    waveformTimeStep = &w.GetTimeStep();

  bool interpolate = true; // now we need to make the data correspond to our time step.      
  if (waveformTimeStep != nullptr)
  {
    if (waveformTimeStep->Equals(timeStep)) // if the data is already sampled at the engine's time step, no interpolation is needed
      interpolate = false;
  }
  if (interpolate)
  {
    // NOTE: This assumes that the data is a SINGLE waveform
    std::vector<double>  iTime;
    std::vector<double>& wTime = data.GetTime();
    double currentTime_s = 0;
    double timeStep_s = timeStep.GetValue(TimeUnit::s);
    double endTime_s = wTime[wTime.size() - 1];
    while (currentTime_s <= endTime_s) // figure out how many data points are needed and populate the new time vector
    {
      iTime.push_back(currentTime_s);
      currentTime_s += timeStep_s;
    }
    if (!InterpolateToTime(data, iTime, TimeUnit::s)) // sets new wavefor based on the old waveform data
      throw new CommonDataModelException("Could not Interpolate to provided time");
    w.GetTimeStep().Set(timeStep);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Interpolates the original data to match the provided time series
///
/// \param  newTime  vector of the new time values
/// \param  unit    the unit to use when creating the output
///
/// \return the new waveform data
///
/// \details
/// This function creates the new waveform data for the ECG output by interpolating the data from the
/// original file. It inherits m_Independent and m_Dependent from the original data and calls
/// GeneralMath::LinearInterpolator to create the new vector of voltage points that correspond to the 
/// time points in newTime. It is then assigned the unit of the original data and output as the new
/// waveform.
//--------------------------------------------------------------------------------------------------
bool SEElectroCardioGramWaveformInterpolator::InterpolateToTime(SEFunctionElectricPotentialVsTime& waveform, std::vector<double>& newTime, const TimeUnit& unit)
{
  if (!waveform.IsValid())
    return false;
  std::vector<double> newElectricPotential;

  //m_Independent;// Original X (Time)
  //m_Dependent;// Original Y (ElectricPotential)
  double x1, x2, y1, y2, xPrime, yPrime;
  unsigned int x1Index = 0;

  for (unsigned int newTimeIterator = 0; newTimeIterator < newTime.size(); newTimeIterator++)
  {
    xPrime = newTime[newTimeIterator]; // new time point

    //find x1
    while (x1Index < waveform.GetTime().size() - 2 &&
      waveform.GetTimeValue(x1Index + 1, unit) <= xPrime)
    {
      x1Index++;
    }

    const ElectricPotentialUnit& epUnit = *waveform.GetElectricPotentialUnit();
    // get the points needed for interpolation.
    x1 = waveform.GetTimeValue(x1Index, unit);
    x2 = waveform.GetTimeValue(x1Index + 1, unit);
    y1 = waveform.GetElectricPotentialValue(x1Index, epUnit);
    y2 = waveform.GetElectricPotentialValue(x1Index + 1, epUnit);

    //Shouldn't need this, but just to be sure we don't go beyond the data
    if (xPrime > x2)
    {
      xPrime = x2;
    }

    // call general math function LinearInterpolator to find yPrime at xPrime, xPrime must be between x1 and x2
    yPrime = GeneralMath::LinearInterpolator(x1, x2, y1, y2, xPrime);
    // populate the voltage vector
    newElectricPotential.push_back(yPrime);
  }
  waveform.GetTime() = newTime;
  waveform.GetElectricPotential() = newElectricPotential;
  if (!waveform.IsValid())
    return false;
  return true;
}

bool SEElectroCardioGramWaveformInterpolator::CanInterpolateLeadPotential(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm) const
{
  if (!HasWaveform(lead, rhythm))
    return false;
  auto l = m_Leads.find(lead);
  if (l == m_Leads.end())
    return false;
  return l->second != nullptr;
}
void SEElectroCardioGramWaveformInterpolator::SetLeadElectricPotential(eElectroCardioGram_WaveformLead lead, SEScalarElectricPotential& ep)
{
  m_Leads[lead] = &ep;
}

bool SEElectroCardioGramWaveformInterpolator::StartNewCycle(eHeartRhythm rhythm)
{
  for (auto l2rw : m_Waveforms)
  {
    if (m_Leads.find(l2rw.first) == m_Leads.end() && !HasWaveform(l2rw.first, rhythm))
    {
      std::stringstream ss;
      ss << "Waveform not provided for Lead " << eElectroCardioGram_WaveformLead_Name(l2rw.first) << " rhythm " << eHeartRhythm_Name(rhythm);
      Error(ss);
      return false;
    }
    l2rw.second[rhythm]->GetActiveIndicies().push_back(0);
  }
  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Prepares the ECG output data
///
/// \details
/// CalculateWaveform takes data from all active iterators and sums them together. It also assigns
/// the unit of millivolts to the output. If there are no active iterators, the output defaults to 0.
//--------------------------------------------------------------------------------------------------
void SEElectroCardioGramWaveformInterpolator::CalculateWaveformsElectricPotential()
{
  // Pull Data from our iterators
  int idx;
  double val;
  for (auto &l2s : m_Leads)//Lead# to Scalar
  {
    SEScalarElectricPotential* ep = l2s.second;
    if (ep == nullptr)
      continue;
    ep->SetValue(0, ElectricPotentialUnit::mV);

    for (auto & r2w : m_Waveforms[l2s.first])//rhythm to indecies
    {
      SEElectroCardioGramWaveform* waveform = r2w.second;
      if (waveform == nullptr)
        continue;
      SEFunctionElectricPotentialVsTime& data = waveform->GetData();
      for (size_t i = 0; i < waveform->GetActiveIndicies().size(); i++)
      {
        idx = waveform->GetActiveIndicies()[i];
        val = data.GetElectricPotentialValue(idx++, *data.GetElectricPotentialUnit());
        ep->IncrementValue(val, *data.GetElectricPotentialUnit());
        if (idx == data.Length())
          waveform->GetActiveIndicies().erase(waveform->GetActiveIndicies().begin() + i--);// Remove the iterator if we are at the end
        else
          waveform->GetActiveIndicies()[i] = idx;// Increment iterator  
      }
    }
  }

}

bool SEElectroCardioGramWaveformInterpolator::HasWaveform(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm) const
{
  auto l = m_Waveforms.find(lead);
  if (l == m_Waveforms.end())
    return false;
  auto w = l->second.find(rhythm);
  if (w == l->second.end())
    return false;
  return w->second != nullptr;
}
SEElectroCardioGramWaveform& SEElectroCardioGramWaveformInterpolator::GetWaveform(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm)
{
  SEElectroCardioGramWaveform* w = m_Waveforms[lead][rhythm];
  if (w == nullptr)
  {
    w = new SEElectroCardioGramWaveform(GetLogger());
    m_Waveforms[lead][rhythm] = w;
  }
  return *w;
}
const SEElectroCardioGramWaveform* SEElectroCardioGramWaveformInterpolator::GetWaveform(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm) const
{
  auto l = m_Waveforms.find(lead);
  if (l == m_Waveforms.end())
    return nullptr;
  auto w = l->second.find(rhythm);
  if (w == l->second.end())
    return nullptr;
  return w->second;
}
void SEElectroCardioGramWaveformInterpolator::RemoveWaveform(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm)
{
  auto l = m_Waveforms.find(lead);
  if (l == m_Waveforms.end())
    return;
  auto w = l->second.find(rhythm);
  if (w == l->second.end())
    return;
  delete w->second;
  w->second = nullptr;
}