/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
//CDM_BIND_DECL(FluidCircuitData)
#include "circuit/SECircuitCalculator.h"
#include "circuit/fluid/SEFluidCircuit.h"

class CDM_DECL SEFluidCircuitCalculator : public SECircuitCalculator<SEFluidCircuit, SEFluidCircuitNode, SEFluidCircuitPath, SEFluidBlackBox, VolumePerPressureUnit, VolumePerTimeUnit, PressureTimeSquaredPerVolumeUnit, PressureUnit, VolumeUnit, PressureTimePerVolumeUnit>
{
public:

  SEFluidCircuitCalculator(Logger* logger);
  SEFluidCircuitCalculator(const VolumePerPressureUnit&, const VolumePerTimeUnit&, const PressureTimeSquaredPerVolumeUnit&, const PressureUnit&, const VolumeUnit&, const PressureTimePerVolumeUnit&, Logger* logger);
  virtual ~SEFluidCircuitCalculator();
};
