/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "EngineHowTo.h"

#include "properties/SEScalarTime.h"
#include "engine/SEActionManager.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// A place to do experiment with Pulse
///
/// \details
/// This is a good place to code up something that demonstrates an issue for reproduction and debugging
//--------------------------------------------------------------------------------------------------
void HowToSerialize()
{
  
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("Sandbox.log");
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json", JSON))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  //std::string action_list = "AnyAction: [ { PatientAction: { Hemorrhage: { PatientAction: { Action: { } }, Compartment: \"RightLeg\", Rate: { ScalarVolumePerTime: { Value: 200, Unit: \"mL/min\" } } } } } ]";
  std::string action_list = "{ \"AnyAction\": [ { \"PatientAction\": { \"Hemorrhage\": { \"PatientAction\": { \"Action\": { } }, \"Compartment\": \"RightLeg\", \"Rate\": { \"ScalarVolumePerTime\": { \"Value\": 200, \"Unit\": \"mL/min\" } } } } } ] }";

  std::vector<SEAction*> vActions;
  if (!SEActionManager::SerializeFromString(action_list, vActions, SerializationFormat::JSON, pe->GetSubstanceManager()))
    std::cout << "Dang..." << std::endl;

  SEActionManager am(pe->GetSubstanceManager());
  am.SerializeFromString(action_list, SerializationFormat::JSON);
  am.SerializeToString(action_list, SerializationFormat::JSON);
  std::cout << action_list << std::endl;


  pe->AdvanceModelTime(5, TimeUnit::s);
}