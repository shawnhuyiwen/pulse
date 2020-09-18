/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "circuit/SECircuitCalculator.h"
#include "circuit/thermal/SEThermalCircuit.h"

class CDM_DECL SEThermalCircuitCalculator : public SECircuitCalculator<SEThermalCircuit, SEThermalCircuitNode, SEThermalCircuitPath, SEThermalBlackBox, HeatCapacitanceUnit, PowerUnit, HeatInductanceUnit, TemperatureUnit, EnergyUnit, HeatResistanceUnit>
{
public:

  SEThermalCircuitCalculator(Logger* logger);
  SEThermalCircuitCalculator(const HeatCapacitanceUnit&, const PowerUnit&, const HeatInductanceUnit&, const TemperatureUnit&, const EnergyUnit&, const HeatResistanceUnit&, Logger* logger);
  virtual ~SEThermalCircuitCalculator();
};
