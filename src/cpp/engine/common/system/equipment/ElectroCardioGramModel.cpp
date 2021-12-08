/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/PulseConfiguration.h"
#include "engine/common/system/equipment/ElectroCardioGramModel.h"

#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveformInterpolator.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEFunctionElectricPotentialVsTime.h"
#include "cdm/properties/SEScalarTime.h"

namespace pulse
{
  /*
  ========================
  Constructors
  ========================
  */

  ElectroCardioGramModel::ElectroCardioGramModel(Data& data) : ElectroCardioGramSystem(data.GetLogger()), Model(data)
  {
    Clear();
    m_Interpolator = new SEElectroCardioGramWaveformInterpolator(data.GetLogger());
  }

  ElectroCardioGramModel::~ElectroCardioGramModel()
  {
    Clear();
    delete m_Interpolator;
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief Cleans up any memory this class is responsible for
  //--------------------------------------------------------------------------------------------------
  void ElectroCardioGramModel::Clear()
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
  void ElectroCardioGramModel::Initialize()
  {
    Model::Initialize();

    m_HeartRhythmTime_s = 0;
    m_HeartRhythmPeriod_s = 0;
    m_AmplitudeModifier = 1.0;
    m_LastRhythm = eHeartRhythm::NormalSinus;
    m_Interpolator->Copy(*m_data.GetConfiguration().GetECGInterpolator());
    // You can uncomment this code to compare the original waveform to the interpolated waveform and make sure you are capturing the data properly
  /* Code to write out the ECG data in a format easy to view in plotting tools
    std::vector<double> original_s = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetTime();
    std::vector<double> original_mV = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetElectricPotential();
    DataTrack Original;
    for (size_t i = 0; i < original_s.size(); i++)
      Original.Track("Original_ECG",original_s[i], original_mV[i]);
    Original.WriteTrackToFile("OriginalECG.csv");
  */
    m_Interpolator->Interpolate(m_data.GetTimeStep());
    /* Code to write out the Interpolated ECG data in a format easy to view in plotting tools
      std::vector<double> interpolated_s = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetTime();
      std::vector<double> interpolated_mV = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetElectricPotential();
      DataTrack Interpolated;
      for (size_t i = 0; i < interpolated_s.size(); i++)
        Interpolated.Track("Interpolated_ECG", interpolated_s[i], interpolated_mV[i]);
      Interpolated.WriteTrackToFile("InterpolatedECG.csv");
    */
    m_Interpolator->SetLeadElectricPotential(eElectroCardioGram_WaveformLead::Lead3, GetLead3ElectricPotential());
  }

  void ElectroCardioGramModel::SetUp()
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
  void ElectroCardioGramModel::PreProcess()
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
  void ElectroCardioGramModel::Process(bool /*solve_and_transport*/)
  {
    m_HeartRhythmTime_s += m_data.GetTimeStep_s();
    if (m_HeartRhythmTime_s >= m_HeartRhythmPeriod_s ||
        m_data.GetCardiovascular().GetHeartRhythm() != m_LastRhythm)// ||
        //m_data.GetEvents().IsEventActive(eEvent::StartOfCardiacCycle))
    {
      double hr;
      m_HeartRhythmTime_s = 0;
      m_HeartRhythmPeriod_s = 0;
      m_LastRhythm = m_data.GetCardiovascular().GetHeartRhythm();

      switch (m_LastRhythm)
      {
      case eHeartRhythm::NormalSinus:
      case eHeartRhythm::SinusBradycardia:
      case eHeartRhythm::SinusTachycardia:
      case eHeartRhythm::PulselessElectricalActivity:
      {
        // If zero, set our hr to our minimum
        // Sync back up with the CV after this
        hr = (m_data.GetCardiovascular().GetHeartRate().IsZero()) ?
          1.0 : m_data.GetCardiovascular().GetHeartRate(FrequencyUnit::Per_s);

        m_AmplitudeModifier = 1.0;
        if (m_LastRhythm == eHeartRhythm::PulselessElectricalActivity)
          m_AmplitudeModifier = 0.75;
        m_Interpolator->StartNewCycle(eElectroCardioGram_WaveformType::Sinus);
        m_HeartRhythmPeriod_s = 1 / hr;
        break;
      }
      case eHeartRhythm::Asystole:
      {
        m_Interpolator->ClearCycles();
        break;
      }
      case eHeartRhythm::CourseVentricularFibrillation:
      case eHeartRhythm::FineVentricularFibrillation:
      {
        m_AmplitudeModifier = 1.0;
        if (m_LastRhythm == eHeartRhythm::FineVentricularFibrillation)
          m_AmplitudeModifier = 0.5;
        m_Interpolator->StartNewCycle(eElectroCardioGram_WaveformType::VentricularFibrillation);
        m_HeartRhythmPeriod_s = m_Interpolator->GetCycleLength(eElectroCardioGram_WaveformType::VentricularFibrillation,TimeUnit::s);
        break;
      }
      case eHeartRhythm::PulselessVentricularTachycardia:
      case eHeartRhythm::StableVentricularTachycardia:
      case eHeartRhythm::UnstableVentricularTachycardia:
      {
        m_AmplitudeModifier = 1.0;
        if (m_LastRhythm == eHeartRhythm::PulselessVentricularTachycardia)
        {
          m_AmplitudeModifier = 0.5;
          hr = (m_data.GetCardiovascular().GetHeartRate().IsZero()) ?
            1.8 : m_data.GetCardiovascular().GetHeartRate(FrequencyUnit::Per_s);
        }
        else
        {
          hr = (m_data.GetCardiovascular().GetHeartRate().IsZero()) ?
            1.0 : m_data.GetCardiovascular().GetHeartRate(FrequencyUnit::Per_s);
        }
        m_Interpolator->StartNewCycle(eElectroCardioGram_WaveformType::VentricularTachycardia);
        m_HeartRhythmPeriod_s = 1 / hr;
        break;
      }
      default:
      {
        Error(eHeartRhythm_Name(m_LastRhythm) + " is not a supported Heart Rhythm for ECG");
      }
      }
    }

    m_Interpolator->CalculateWaveformsElectricPotential(m_AmplitudeModifier);
    ComputeExposedModelParameters();
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
  void ElectroCardioGramModel::PostProcess(bool /*solve_and_transport*/)
  {

  }

  void ElectroCardioGramModel::ComputeExposedModelParameters()
  {

  }
END_NAMESPACE
