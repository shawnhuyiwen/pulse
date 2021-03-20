/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "EngineHowTo.h"

#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarTime.h"
#include "compartment/SECompartmentManager.h"
#include "substance/SESubstanceManager.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// A place to do experiment with Pulse
///
/// \details
/// This is a good place to code up something that demonstrates an issue for reproduction and debugging
//--------------------------------------------------------------------------------------------------
void HowToSandbox()
{
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/PulseSandbox.log");
  const SELiquidCompartment* g = pe->GetCompartments().GetLiquidCompartment("Ground");
  if (!pe->SerializeFromFile("./states/testing/equipment/AnesthesiaMachineVariedConfiguration/AnesthesiaMachineVariedConfigurationAfterActions@30.02.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  pe->AdvanceModelTime();

  pe->AdvanceModelTime();
  pe->AdvanceModelTime();
  pe->SerializeToFile("./states/testing/equipment/AnesthesiaMachineVariedConfiguration/AnesthesiaMachineVariedConfigurationAfterActions@30.04.debug.json");

  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");

  // Ok we are done with that patient, let's do another patient with the same engine
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  pe->AdvanceModelTime(5, TimeUnit::s);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");

  pe->AdvanceModelTime(5, TimeUnit::min);
}