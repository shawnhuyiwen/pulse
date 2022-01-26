/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/patient/assessments/SEPulmonaryFunctionTest.h"
#include "cdm/system/physiology/SEBloodChemistrySystem.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/system/physiology/SERespiratorySystem.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEFunctionVolumeVsTime.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/compartment/SECompartmentManager.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for performing a Pulmonary Function Test (PFT) on the patient
///
/// \details
/// Refer to the SEPulmonaryFunctionTest class
/// Refer to the SEPatient class
/// Refer to the SERespiratory class
//--------------------------------------------------------------------------------------------------
void HowToPulmonaryFunctionTest()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo_PulmonaryFunctionTest.log");
  pe->GetLogger()->Info("HowTo_PulmonaryFunctionTest");
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Let's analyze the respiratory system more specifically by performing a Pulmonary Function Test (PFT)
  // The PFT Calculates a wave form representing total lung volume during a normal breathing cycle, 
  // forced inhalation and exhalation from current tidal volume and engine parameters

  // Create data requests for each value that should be written to the output log as the engine is executing
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("HowToPulmonaryFunctionTest.csv");

  AdvanceAndTrackTime_s(5, *pe);

  SEPulmonaryFunctionTest pft(pe->GetLogger());
  pe->GetPatientAssessment(pft);

  pe->GetLogger()->Info("Performing PFT at time 0s");
  pe->GetLogger()->Info("Currently these are the PFT properties computed by the Pulse engine");
  pe->GetLogger()->Info(std::stringstream() <<"Expiratory Reserve Volume"    << pft.GetExpiratoryReserveVolume()  ); 
  pe->GetLogger()->Info(std::stringstream() <<"Functional Residual Capacity" << pft.GetFunctionalResidualCapacity());
  pe->GetLogger()->Info(std::stringstream() <<"Inspiratory Capacity"         << pft.GetInspiratoryCapacity()      );
  pe->GetLogger()->Info(std::stringstream() <<"Inspiratory Reserve Volume"   << pft.GetInspiratoryReserveVolume() );
  pe->GetLogger()->Info(std::stringstream() <<"Residual Volume"              << pft.GetResidualVolume()           );
  pe->GetLogger()->Info(std::stringstream() <<"Total Lung Capacity"          << pft.GetTotalLungCapacity()        );
  pe->GetLogger()->Info(std::stringstream() <<"Vital Capacity"               << pft.GetVitalCapacity()            );
  pe->GetLogger()->Info("Currently, Pulse does not support calculation of the following values:");

  // Values will be NaN
  pe->GetLogger()->Info(std::stringstream() <<"Forced Vital Capacity"        << pft.GetForcedVitalCapacity()       );
  pe->GetLogger()->Info(std::stringstream() <<"Forced Expiratory Volume"     << pft.GetForcedExpiratoryVolume()    );
  pe->GetLogger()->Info(std::stringstream() <<"Forced Expiratory Flow"       << pft.GetForcedExpiratoryFlow()      );
  pe->GetLogger()->Info(std::stringstream() <<"Maximum Voluntary Ventilation"<< pft.GetMaximumVoluntaryVentilation());
  pe->GetLogger()->Info(std::stringstream() <<"Peak Expiratory Flow"         << pft.GetPeakExpiratoryFlow()        );
  pe->GetLogger()->Info(std::stringstream() <<"Slow Vital Capacity"          << pft.GetSlowVitalCapacity()         );
  
  // Pulse does compute the LungVolumePlot Data
  //The resulting plot is obtained which displays lung volume as a function of time 
  SEFunctionVolumeVsTime& lungVolumePlot = pft.GetLungVolumePlot();

  //The results may be displayed from from the abscissa (time) and ordinate (lung volume) of the lung volume plot
  lungVolumePlot.GetTime(); //This is the time component of the pulmonary function test
  lungVolumePlot.GetVolume(); //This is the lung volume component of the pulmonary function test
  // This is intended to be a a data form that can easily be plotted.
  pe->GetLogger()->Info("Finished");
}