/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "PulsePhysiologyEngine.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creates a Respiratory Engine.
///
/// \details
/// This engine only has the respiratory and mechanical ventilator
/// It is intented to only exercise the respiratory mechanics
//--------------------------------------------------------------------------------------------------
PULSE_DECL std::unique_ptr<PhysiologyEngine> CreateRespiratoryEngine(Logger* logger = nullptr);
