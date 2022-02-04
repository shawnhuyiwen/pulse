/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/PulseConfiguration.h"
#include "engine/common/system/equipment/ElectroCardioGramModel.h"

#include "cdm/utils/GeneralMath.h"
#include "cdm/properties/SEArrayElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
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
  }

  ElectroCardioGramModel::~ElectroCardioGramModel()
  {
    Clear();
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
    m_LastRhythm = eHeartRhythm::NormalSinus;
    if(m_data.GetConfiguration().HasECG())
      Copy(*m_data.GetConfiguration().GetECG());
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
    if (m_data.GetCardiovascular().GetHeartRhythm() != m_LastRhythm ||
        m_data.GetEvents().IsEventActive(eEvent::StartOfCardiacCycle))
    {
      double amplitudeModifier = 1.0;
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

        if (m_LastRhythm == eHeartRhythm::PulselessElectricalActivity)
          amplitudeModifier = 0.75;
        StartNewCycle(eElectroCardioGram_WaveformType::Sinus,
          m_data.GetCardiovascular().GetHeartRate(),
          amplitudeModifier);
        break;
      }
      case eHeartRhythm::Asystole:
      {
        ClearCycles();
        break;
      }
      case eHeartRhythm::CoarseVentricularFibrillation:
      case eHeartRhythm::FineVentricularFibrillation:
      {
        if (m_LastRhythm == eHeartRhythm::FineVentricularFibrillation)
          amplitudeModifier = 0.5;
        StartNewCycle(eElectroCardioGram_WaveformType::VentricularFibrillation,
          m_data.GetCardiovascular().GetHeartRate(),
          amplitudeModifier);
        break;
      }
      case eHeartRhythm::PulselessVentricularTachycardia:
      case eHeartRhythm::StableVentricularTachycardia:
      case eHeartRhythm::UnstableVentricularTachycardia:
      {
        if (m_LastRhythm == eHeartRhythm::PulselessVentricularTachycardia)
          amplitudeModifier = 0.5;
        StartNewCycle(eElectroCardioGram_WaveformType::VentricularTachycardia,
          m_data.GetCardiovascular().GetHeartRate(),
          amplitudeModifier);
        break;
      }
      default:
      {
        Error(eHeartRhythm_Name(m_LastRhythm) + " is not a supported Heart Rhythm for ECG");
      }
      }
    }
    PullCycleValues();
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
