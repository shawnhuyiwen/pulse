/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/CommonDefs.h"
#include "cdm/scenario/SEScenarioExec.h"

/**
* @brief Create a Pulse engine and execute the scenario opts
*/
class PULSE_DECL PulseScenarioExec
{
public:
  static bool Execute(PhysiologyEngine& pe, SEScenarioExec& opts);
};
