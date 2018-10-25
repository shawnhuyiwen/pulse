/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "EngineHowTo.h"

#include "properties/SEScalarTime.h"

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
  if (!pe->SerializeFromFile("./states/StandardMale@0s.pba", ASCII))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  pe->AdvanceModelTime(5, TimeUnit::s);
}