/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

#include <memory>

// Include the various types you will be using in your code
#include "patient/SEPatient.h"
#include "patient/assessments/SEPulmonaryFunctionTest.h"
#include "compartment/SECompartmentManager.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstance.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEPatientConfiguration.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarLength.h"

#include "PulsePhysiologyEnginePool.h"
#include "utils/TimingProfile.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating a patient
///
/// \details
/// Creating a customized patient in Pulse
//--------------------------------------------------------------------------------------------------
void HowToPulseEnginePool()
{
  size_t numEngine = 16 * 6;
  TimingProfile profiler;
  double sim_time_s = 20;

  PulsePhysiologyEnginePool pool(numEngine);

  for (auto& engine : pool.getEngines())
  {
      engine.first->GetLogger()->SetLogFile("./test_results/HowTo_CreateAPatient.log");
      engine.first->GetLogger()->Info("HowTo_CreateAPatient");
  }

//   std::vector<SEPatientConfiguration*> configurations;
//   for (size_t i = 0; i < numEngine; ++i)
//   {
//       auto pc = new SEPatientConfiguration();
//       SEPatient& patient = pc->GetPatient();
//       patient.SetName("HowToCreateAPatient");
//       patient.SetSex((i%2 == 0) ? ePatient_Sex::Male : ePatient_Sex::Female);
//       patient.GetAge().SetValue(44, TimeUnit::yr);
//       patient.GetWeight().SetValue(170, MassUnit::lb);
//       patient.GetHeight().SetValue(71, LengthUnit::in);
//       patient.GetBodyFatFraction().SetValue(0.21);
//       patient.GetDiastolicArterialPressureBaseline().SetValue(74, PressureUnit::mmHg);
//       patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
//       patient.GetRespirationRateBaseline().SetValue(16, FrequencyUnit::Per_min);
//       patient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);
//       configurations.push_back(pc);
//   }
  std::vector<std::string> filenames;
  for (size_t i = 0; i < numEngine; ++i)
  {
      filenames.push_back("./states/StandardMale@0s.json");
  }

  pool.initAll(filenames);
  for (auto& engine : pool.getEngines())
  {
      if (!engine.second) 
      {
          engine.first->GetLogger()->Error("Could not load state, check the error");
      }
  }

  // Maybe this call should just return true if all succeeded and false if one or more failed 

  profiler.Start("s");
  pool.advanceAll(sim_time_s, TimeUnit::s);
  for (auto& engine : pool.getEngines())
  {
      if (!engine.second)
      {
          engine.first->GetLogger()->Error("Advance, check the error");
      }
  }

  double elapsed_time_s = profiler.GetElapsedTime_s("s");

  // Are we still running real time?
  std::cout << numEngine << " (" << pool.workerCount() << " threads ), needed " << elapsed_time_s << "s to simulate " << sim_time_s << "s\n";



//   for (auto c : configurations)
//   {
//       delete c;
//   }
}