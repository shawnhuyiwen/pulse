/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "physiology/Nervous.h"
#include "PulseConfiguration.h"
// Actions
#include "engine/SEActionManager.h"
#include "engine/SEPatientActionCollection.h"
#include "patient/actions/SEBrainInjury.h"
// Dependent Systems
#include "patient/SEPatient.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SEPupillaryResponse.h"
#include "system/physiology/SEDrugSystem.h"
// CDM
#include "engine/SEEventManager.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarNegative1To1.h"
#include "properties/SEScalar0To1.h"
#include "utils/GeneralMath.h"

#pragma warning(disable:4786)
#pragma warning(disable:4275)

// #define VERBOSE

Nervous::Nervous(PulseController& data) : SENervousSystem(data.GetLogger()), m_data(data)
{
  Clear();
}

Nervous::~Nervous()
{
  Clear();
}

void Nervous::Clear()
{
  SENervousSystem::Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Nervous::Initialize()
{
  PulseSystem::Initialize();
  m_FeedbackActive = false;
  GetBaroreceptorHeartRateScale().SetValue(1.0);
  GetBaroreceptorHeartElastanceScale().SetValue(1.0);
  GetBaroreceptorResistanceScale().SetValue(1.0);
  GetBaroreceptorComplianceScale().SetValue(1.0);
  GetLeftEyePupillaryResponse().GetSizeModifier().SetValue(0);
  GetLeftEyePupillaryResponse().GetShapeModifier().SetValue(0);
  GetLeftEyePupillaryResponse().GetReactivityModifier().SetValue(0);
  GetRightEyePupillaryResponse().GetSizeModifier().SetValue(0);
  GetRightEyePupillaryResponse().GetShapeModifier().SetValue(0);
  GetRightEyePupillaryResponse().GetReactivityModifier().SetValue(0);

  // Set when feedback is turned on
  m_ArterialOxygenBaseline_mmHg = 0;
  m_ArterialCarbonDioxideBaseline_mmHg = 0;
  m_BaroreceptorActiveTime_s = 0.0;
  m_BaroreceptorFeedbackStatus = false;
  m_BaroreceptorMeanArterialPressureBaseline_mmHg = 0;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes the nervous specific quantities
///
/// \details
/// Initializes the nervous system.
//--------------------------------------------------------------------------------------------------
void Nervous::SetUp()
{
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_NormalizedGammaHeartRate  = m_data.GetConfiguration().GetNormalizedHeartRateIntercept();
  m_NormalizedGammaElastance  = m_data.GetConfiguration().GetNormalizedHeartElastanceIntercept();
  m_NormalizedGammaCompliance = m_data.GetConfiguration().GetNormalizedComplianceIntercept();
  m_NormalizedGammaResistance = m_data.GetConfiguration().GetNormalizedResistanceIntercept();
  m_NormalizedAlphaHeartRate  = m_data.GetConfiguration().GetNormalizedHeartRateSympatheticSlope();
  m_NormalizedAlphaElastance  = m_data.GetConfiguration().GetNormalizedHeartElastanceSympatheticSlope();
  m_NormalizedAlphaCompliance = m_data.GetConfiguration().GetNormalizedComplianceParasympatheticSlope();
  m_NormalizedAlphaResistance = m_data.GetConfiguration().GetNormalizedResistanceSympatheticSlope();
  m_NormalizedBetaHeartRate   = m_data.GetConfiguration().GetNormalizedHeartRateParasympatheticSlope();
}

void Nervous::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState)
    m_FeedbackActive = true;

  // The set-points (Baselines) get reset at the end of each stabilization period.
  m_ArterialOxygenBaseline_mmHg = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  m_ArterialCarbonDioxideBaseline_mmHg = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg);
  m_BaroreceptorMeanArterialPressureBaseline_mmHg = m_data.GetCurrentPatient().GetMeanArterialPressureBaseline(PressureUnit::mmHg);
  // The baroreceptor scales need to be reset any time the baselines are reset.
  GetBaroreceptorHeartRateScale().SetValue(1.0);
  GetBaroreceptorHeartElastanceScale().SetValue(1.0);
  GetBaroreceptorResistanceScale().SetValue(1.0);
  GetBaroreceptorComplianceScale().SetValue(1.0);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Preprocess methods for the nervous system
///
/// \details
/// Computes nervous system regulation of the body.
/// Baroreceptor and chemoreceptor feedback is computed and modifiers set in preparation for systems processing.
//--------------------------------------------------------------------------------------------------
void Nervous::PreProcess()
{
  if (m_FeedbackActive)
  {
    BaroreceptorFeedback();
    ChemoreceptorFeedback();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Nervous Process Step
///
/// \details
/// The only current Process-specific function checks the brain status to set events.
//--------------------------------------------------------------------------------------------------
void Nervous::Process()
{
  CheckBrainStatus();
  SetPupilEffects();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Nervous PostProcess Step
///
/// \details
/// Currently no nervous postprocess methods.
//--------------------------------------------------------------------------------------------------
void Nervous::PostProcess()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the baroreceptor feedback and sets the scaling parameters in the CDM
///
/// \details
/// The baroreceptor feedback function uses the current mean arterial pressure relative to the mean arterial
/// pressure set-point in order to calculate the sympathetic and parasympathetic response fractions.
/// These fractions are used to update the scaling parameters of heart rate, heart elastance, resistance and compliance
/// for each time step.
//--------------------------------------------------------------------------------------------------
void Nervous::BaroreceptorFeedback()
{
  //First calculate the sympathetic and parasympathetic firing rates:
  double nu = m_data.GetConfiguration().GetResponseSlope();
  double meanArterialPressure_mmHg = m_data.GetCardiovascular().GetMeanArterialPressure(PressureUnit::mmHg);

  //Adjusting the mean arterial pressure set-point to account for cardiovascular drug effects
  double meanArterialPressureCombinedBaseline_mmHg = m_BaroreceptorMeanArterialPressureBaseline_mmHg
    + m_data.GetDrugs().GetMeanBloodPressureChange(PressureUnit::mmHg)
    + m_data.GetEnergy().GetExerciseMeanArterialPressureDelta(PressureUnit::mmHg);
  
  //Determine if barorectors are active and for how long
  double pressureDeviation = meanArterialPressure_mmHg - meanArterialPressureCombinedBaseline_mmHg;
  double pressureDeviationFraction = std::abs(pressureDeviation) / meanArterialPressureCombinedBaseline_mmHg;
  if (pressureDeviationFraction > 0.05)
  {
    if (!m_BaroreceptorFeedbackStatus)
    {
      m_BaroreceptorActiveTime_s = 0.0;
      m_BaroreceptorFeedbackStatus = true;
      Info("Starting Baroreceptor timer ");
    }
    else
    {
      m_BaroreceptorActiveTime_s += m_dt_s;
      //Need to change the threshold every 7 minutes based on over activation
      if (m_BaroreceptorActiveTime_s > 420.0/*s*/)
      {
        //Reset the time
        m_BaroreceptorActiveTime_s = 0.0;
        //Adjust the threshold by up to 30%
        m_BaroreceptorMeanArterialPressureBaseline_mmHg += 0.35 * pressureDeviation;
        m_ss << "Baroreceptor MAP Baseline updated to " << m_BaroreceptorMeanArterialPressureBaseline_mmHg << " mmHg";
        Info(m_ss);
      }
    }
  }
  else
  {
    if (m_BaroreceptorFeedbackStatus)
      Info("Stopping Baroreceptor timer ");
    m_BaroreceptorFeedbackStatus = false;
  }
  
  // \todo Dampen Baroreceptor response due to sedation
  //double sedationDampeningEffect = m_data.GetDrugs().GetSedationLevel().GetValue() * 0.35; //maximum slope reduction of 35% - making that up right now
  //update nu - the slope response of the firing rate
  //nu *= (1 - sedationDampeningEffect);  TODO: Add this back when I have data for validation
  
  double sympatheticFraction = 1.0 / (1.0 + pow(meanArterialPressure_mmHg / meanArterialPressureCombinedBaseline_mmHg, nu));
  double parasympatheticFraction = 1.0 / (1.0 + pow(meanArterialPressure_mmHg / meanArterialPressureCombinedBaseline_mmHg, -nu));
  
  //Calculate the normalized change in heart rate
  double normalizedHeartRate = GetBaroreceptorHeartRateScale().GetValue();
  double tauHeartRate_s = m_data.GetConfiguration().GetHeartRateDistributedTimeDelay(TimeUnit::s);
  double deltaNormalizedHeartRate = (1.0 / tauHeartRate_s)*(-normalizedHeartRate + m_NormalizedAlphaHeartRate*sympatheticFraction - m_NormalizedBetaHeartRate*parasympatheticFraction + m_NormalizedGammaHeartRate)*m_dt_s;
  normalizedHeartRate += deltaNormalizedHeartRate;
  GetBaroreceptorHeartRateScale().SetValue(normalizedHeartRate);
  
  //Calculate the normalized change in heart elastance
  double normalizedHeartElastance = GetBaroreceptorHeartElastanceScale().GetValue();
  double tauElastance_s = m_data.GetConfiguration().GetHeartElastanceDistributedTimeDelay(TimeUnit::s);
  double deltaNormalizedHeartElastance = (1.0 / tauElastance_s)*(-normalizedHeartElastance + m_NormalizedAlphaElastance*sympatheticFraction + m_NormalizedGammaElastance)*m_dt_s;
  normalizedHeartElastance += deltaNormalizedHeartElastance;
  GetBaroreceptorHeartElastanceScale().SetValue(normalizedHeartElastance);
  
  //Calculate the normalized change in flow resistance for any cardiovascular resistor
  double normalizedResistance = GetBaroreceptorResistanceScale().GetValue();
  double tauResistance_s = m_data.GetConfiguration().GetSystemicResistanceDistributedTimeDelay(TimeUnit::s);
  double deltaNormalizedResistance = (1.0 / tauResistance_s)*(-normalizedResistance + m_NormalizedAlphaResistance*sympatheticFraction + m_NormalizedGammaResistance)*m_dt_s;
  normalizedResistance += deltaNormalizedResistance;
  GetBaroreceptorResistanceScale().SetValue(normalizedResistance);
  
  //Calculate the normalized change in flow compliance for any cardiovascular compliance
  double normalizedCompliance = GetBaroreceptorComplianceScale().GetValue();
  double tauCompliance_s = m_data.GetConfiguration().GetVenousComplianceDistributedTimeDelay(TimeUnit::s);
  double deltaNormalizedCompliance = (1.0 / tauCompliance_s)*(-normalizedCompliance + m_NormalizedAlphaCompliance*parasympatheticFraction + m_NormalizedGammaCompliance)*m_dt_s;
  normalizedCompliance += deltaNormalizedCompliance;
  GetBaroreceptorComplianceScale().SetValue(normalizedCompliance);
#ifdef VERBOSE
  m_data.GetDataTrack().Probe("normalizedHeartRate", normalizedHeartRate);
  m_data.GetDataTrack().Probe("normalizedHeartElastance", normalizedHeartElastance);
  m_data.GetDataTrack().Probe("normalizedResistance", normalizedResistance);
  m_data.GetDataTrack().Probe("normalizedCompliance", normalizedCompliance);
  m_data.GetDataTrack().Probe("meanArterialPressureBaseline_mmHg", meanArterialPressureBaseline_mmHg);
#endif
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Checks metrics in the brain to determine events to be thrown
///
/// \details
/// Intracranial pressure is checked to determine if the patient has Intracranial Hyper/hypotension
//--------------------------------------------------------------------------------------------------
void Nervous::CheckBrainStatus()
{
  double icp_mmHg = m_data.GetCardiovascular().GetIntracranialPressure().GetValue(PressureUnit::mmHg);

  //Intracranial Hypertension
  if (icp_mmHg > 25.0) // \cite steiner2006monitoring
  {
    /// \event Patient: Intracranial Hypertension. The intracranial pressure has risen above 25 mmHg.
    m_data.GetEvents().SetEvent(eEvent::IntracranialHypertension, true, m_data.GetSimulationTime());
  }
  else if (m_data.GetEvents().IsEventActive(eEvent::IntracranialHypertension) && icp_mmHg < 24.0)
  {
    /// \event Patient: End Intracranial Hypertension. The intracranial pressure has fallen below 24 mmHg.
    m_data.GetEvents().SetEvent(eEvent::IntracranialHypertension, false, m_data.GetSimulationTime());
  }

  //Intracranial Hypotension
  if (icp_mmHg < 7.0) // \cite steiner2006monitoring
  {
    /// \event Patient: Intracranial Hypotension. The intracranial pressure has fallen below 7 mmHg.
    m_data.GetEvents().SetEvent(eEvent::IntracranialHypotension, true, m_data.GetSimulationTime());
  }
  else if (m_data.GetEvents().IsEventActive(eEvent::IntracranialHypotension) && icp_mmHg > 7.5)
  {
    /// \event Patient: End Intracranial Hypotension. The intracranial pressure has risen above 7.5 mmHg.
    m_data.GetEvents().SetEvent(eEvent::IntracranialHypertension, false, m_data.GetSimulationTime());
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the chemoreceptor feedback and sets the scaling parameters in the CDM
///
/// \details
/// The chemoreceptor feedback function uses the current arterial partial pressure of oxygen and carbon dioxide
/// relative to the partial pressure set-points in order to calculate response signal.
/// The affected systems identify the signal and adjust accordingly. Note that chemoreception
/// is currently built into the respiratory driver; therefore, the chemoreceptor feedback only sets CV modifiers.
//--------------------------------------------------------------------------------------------------
void Nervous::ChemoreceptorFeedback()
{
  double normalized_pO2 = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg) / m_ArterialOxygenBaseline_mmHg;
  double normalized_pCO2 = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg) / m_ArterialCarbonDioxideBaseline_mmHg;

  // The chemoreceptor heart rate modification function shape parameters.
  // See NervousMethodology documentation for details.
  double amax = -0.1;
  double a50 = 0.5;
  double aeta = -12.;
  double bmax = 1.;
  double b50 = 1.7;
  double beta = 18;
  double cmax = 1.;
  double c50 = 0.65;
  double ceta = -20;
  double dmax = -0.1;
  double d50 = b50;
  double deta = -aeta;

  //Calculate the normalized change in heart rate
  //double HRBaseline_per_min = m_HeartRateNoFeedbackBaseline_per_min;
  // Maximum HR delta is 1.23 times baseline. The derivation of this maximum is described in the NervousMethodology documentation
  //double maxHeartRateDelta = 1.23 * m_HeartRateNoFeedbackBaseline_per_min;
  double maxHeartRateDelta = 1.23 * m_data.GetCurrentPatient().GetHeartRateBaseline(FrequencyUnit::Per_min);
  double modifier = GeneralMath::LogisticFunction(amax, a50, aeta, normalized_pCO2);
  modifier += GeneralMath::LogisticFunction(bmax, b50, beta, normalized_pCO2);
  modifier += GeneralMath::LogisticFunction(cmax, c50, ceta, normalized_pO2);
  modifier += GeneralMath::LogisticFunction(dmax, d50, deta, normalized_pO2);
  
  GetChemoreceptorHeartRateScale().SetValue(maxHeartRateDelta*modifier);

  // Calculate the normalized change in heart elastance
  double normalizedHeartElastance = 1.0;
  /// \todo Compute and apply chemoreceptor-mediated contractility changes
  GetChemoreceptorHeartElastanceScale().SetValue(normalizedHeartElastance);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets pupil size and reactivity modifiers based on drug and TBI effects
///
/// \details
/// Modifiers are on a scale between -1 (for reduction in size/reactivity) and 1 (for increase)
/// TBI effects are applied to the eye on the same side of the injury if localized or both if diffuse
/// Drug and TBI pupil effects are simply summed together
//--------------------------------------------------------------------------------------------------
void Nervous::SetPupilEffects()
{
  // Get modifiers from Drugs
  double leftPupilSizeResponseLevel = m_data.GetDrugs().GetPupillaryResponse().GetSizeModifier().GetValue();
  double leftPupilReactivityResponseLevel = m_data.GetDrugs().GetPupillaryResponse().GetReactivityModifier().GetValue();
  double rightPupilSizeResponseLevel = leftPupilSizeResponseLevel;
  double rightPupilReactivityResponseLevel = leftPupilReactivityResponseLevel;

  // Calculate the TBI response
  if (m_data.GetActions().GetPatientActions().HasBrainInjury())
  {
    SEBrainInjury* b = m_data.GetActions().GetPatientActions().GetBrainInjury();

    if (b->GetSeverity().GetValue() > 0)
    {
      double icp_mmHg = m_data.GetCardiovascular().GetIntracranialPressure().GetValue(PressureUnit::mmHg);

      if (b->GetType() == eBrainInjury_Type::Diffuse)
      {
        leftPupilSizeResponseLevel += (1 / (1 + exp(-2.3*(icp_mmHg - 22.5))));
        leftPupilReactivityResponseLevel += -.001*pow(10, .3*(icp_mmHg - 15));
        rightPupilSizeResponseLevel = leftPupilSizeResponseLevel;
        rightPupilReactivityResponseLevel = leftPupilReactivityResponseLevel;
      }
      else if (b->GetType() == eBrainInjury_Type::LeftFocal)
      {
        leftPupilSizeResponseLevel += (1 / (1 + exp(-2.3*(icp_mmHg - 22.5))));
        leftPupilReactivityResponseLevel += -.001*pow(10, .3*(icp_mmHg - 15));
      }
      else if(b->GetType() == eBrainInjury_Type::RightFocal)
      {
        rightPupilSizeResponseLevel += (1 / (1 + exp(-2.3*(icp_mmHg - 22.5))));
        rightPupilReactivityResponseLevel += -.001*pow(10, .3*(icp_mmHg - 15));
      }
    }
  }

  BLIM(leftPupilSizeResponseLevel, -1, 1);
  BLIM(leftPupilReactivityResponseLevel, -1, 1);
  BLIM(rightPupilSizeResponseLevel, -1, 1);
  BLIM(rightPupilReactivityResponseLevel, -1, 1);
  GetLeftEyePupillaryResponse().GetSizeModifier().SetValue(leftPupilSizeResponseLevel);
  GetLeftEyePupillaryResponse().GetReactivityModifier().SetValue(leftPupilReactivityResponseLevel);
  GetRightEyePupillaryResponse().GetSizeModifier().SetValue(rightPupilSizeResponseLevel);
  GetRightEyePupillaryResponse().GetReactivityModifier().SetValue(rightPupilReactivityResponseLevel);
}