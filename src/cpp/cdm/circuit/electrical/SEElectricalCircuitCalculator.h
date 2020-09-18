/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
//CDM_BIND_DECL(ElectricalCircuitData)
#include "circuit/SECircuitCalculator.h"
#include "circuit/electrical/SEElectricalCircuit.h"

class CDM_DECL SEElectricalCircuitCalculator : public SECircuitCalculator<SEElectricalCircuit, SEElectricalCircuitNode, SEElectricalCircuitPath, SEElectricalBlackBox, ElectricCapacitanceUnit, ElectricCurrentUnit, ElectricInductanceUnit, ElectricPotentialUnit, ElectricChargeUnit, ElectricResistanceUnit>
{
public:

  SEElectricalCircuitCalculator(Logger* logger);
  SEElectricalCircuitCalculator(const ElectricCapacitanceUnit&, const ElectricCurrentUnit&, const ElectricInductanceUnit&, const ElectricPotentialUnit&, const ElectricChargeUnit&, const ElectricResistanceUnit&, Logger* logger);
  virtual ~SEElectricalCircuitCalculator();
};
