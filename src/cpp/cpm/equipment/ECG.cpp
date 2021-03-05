/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


#include "stdafx.h"
#include "equipment/ECG.h"
#include "PulseConfiguration.h"
#include "physiology/Cardiovascular.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveformInterpolator.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEFunctionElectricPotentialVsTime.h"
#include "properties/SEScalarTime.h"

/*
========================
Constructors
========================
*/

ECG::ECG(PulseData& data) : PulseElectroCardioGram(data)
{
  Clear();
  m_interpolator = new SEElectroCardioGramWaveformInterpolator(data.GetLogger());
}

ECG::~ECG()
{
  Clear();
  delete m_interpolator;
}

//--------------------------------------------------------------------------------------------------
/// \brief Cleans up any memory this class is responsible for
//--------------------------------------------------------------------------------------------------
void ECG::Clear()
{
  SEElectroCardioGram::Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes the ECG specific quantities
///
/// \details
/// Local member variables used in the ECG class are initialized here. These variables are used to
/// keep track of the heart cycle and to create the ECG machine's output. 
/// SEElectrocardioGramInterpolater also loads in the data needed to create the output and
/// interpolates it to match our time step. Placing the variables in this function allows for the 
/// system to be easy initialized at the beginning of a simulation or after a crash.
//--------------------------------------------------------------------------------------------------
void ECG::Initialize()
{
  PulseSystem::Initialize();

  m_heartRhythmTime_s = 0;
  m_heartRhythmPeriod_s = 0;
  m_interpolator->Copy(*m_data.GetConfiguration().GetECGInterpolator());
  // You can uncomment this code to compare the original waveform to the interpolated waveform and make sure you are capturing the data properly
/* Code to write out the ECG data in a format easy to view in plotting tools 
  std::vector<double> original_s = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetTime();
  std::vector<double> original_mV = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetElectricPotential();
  DataTrack Original;  
  for (size_t i = 0; i < original_s.size(); i++)
    Original.Track("Original_ECG",original_s[i], original_mV[i]);
  Original.WriteTrackToFile("OriginalECG.csv");
*/
  m_interpolator->Interpolate(m_data.GetTimeStep());
/* Code to write out the Interpolated ECG data in a format easy to view in plotting tools
  std::vector<double> interpolated_s = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetTime();
  std::vector<double> interpolated_mV = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetElectricPotential();
  DataTrack Interpolated;
  for (size_t i = 0; i < interpolated_s.size(); i++)
    Interpolated.Track("Interpolated_ECG", interpolated_s[i], interpolated_mV[i]);
  Interpolated.WriteTrackToFile("InterpolatedECG.csv");
*/
  m_interpolator->SetLeadElectricPotential(eElectroCardioGram_WaveformLead::Lead3, GetLead3ElectricPotential());
}

void ECG::SetUp()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Prepares the ECG machine for the circuit solver
///
/// \details
/// At this time, we do not have a circuit model for the ECG machine so this function is left empty.
/// The PreProcess function is included to preserve our system methodology and as a placeholder for
/// models developed in the future.
//--------------------------------------------------------------------------------------------------
void ECG::PreProcess()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Process creates the ECG output
///
/// \details
/// The process function queries the heart rhythm and heart rate from the cardiovascular system. The
/// time period of the heart beat is determined from the heart rate and a new iterator on the waveform
/// is initialized if the queried rhythm is Normal Sinus. The iterator holds the value of the waveform
/// data at a specific time point on the interpolated waveform data. Process increments the time point
/// and sums up the value of any active iterators. The iterators continue being incremented until they
/// reach the end of the interpolated waveform data. If there are no active iterators, the output
/// defaults to 0 mV.
//--------------------------------------------------------------------------------------------------
void ECG::Process(bool solve_and_transport)
{
  m_heartRhythmTime_s += m_data.GetTimeStep_s();
  if (m_heartRhythmTime_s >= m_heartRhythmPeriod_s)
  {
    m_heartRhythmTime_s = 0;
    m_heartRhythmPeriod_s = 0;
    // Currently we  have one data set for all currently supported Heart Rhythms
    // Eventually we will support multiple rhythmic data
    if (m_data.GetCardiovascular().GetHeartRhythm() == eHeartRhythm::NormalSinus)
    {
      m_interpolator->StartNewCycle(eHeartRhythm::NormalSinus);
      m_heartRhythmPeriod_s = 1 / m_data.GetCardiovascular().GetHeartRate(FrequencyUnit::Per_s);
    }
    else if (m_data.GetCardiovascular().GetHeartRhythm() == eHeartRhythm::Asystolic)
    {
      // Nothing to do here but be zero
    }
    else
    {
      m_ss << eHeartRhythm_Name(m_data.GetCardiovascular().GetHeartRhythm()) << " is not a supported Heart Rhythm for ECG";
      Error(m_ss);
    }
  }
  m_interpolator->CalculateWaveformsElectricPotential();
  ComputeExposedModelParameters();
}
void ECG::ComputeExposedModelParameters()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Post process updates the math after the circuit calculation are done
///
/// \details
/// At this time, we do not have a circuit model for the ECG machine so this function is left empty.
/// The PostProcess function is included to preserve our system methodology and as a placeholder for
/// models developed in the future.
//--------------------------------------------------------------------------------------------------
void ECG::PostProcess(bool solve_and_transport)
{

}