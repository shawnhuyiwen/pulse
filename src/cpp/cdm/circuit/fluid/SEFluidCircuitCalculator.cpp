/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"


SEFluidCircuitCalculator::SEFluidCircuitCalculator(Logger* logger) : SECircuitCalculator<SEFluidCircuit,SEFluidCircuitNode,SEFluidCircuitPath, SEFluidBlackBox, VolumePerPressureUnit, VolumePerTimeUnit, PressureTimeSquaredPerVolumeUnit, PressureUnit, VolumeUnit, PressureTimePerVolumeUnit>(
  VolumePerPressureUnit::m3_Per_Pa,VolumePerTimeUnit::m3_Per_s, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3,PressureUnit::Pa,VolumeUnit::m3,PressureTimePerVolumeUnit::Pa_s_Per_m3,logger)
{

}
SEFluidCircuitCalculator::SEFluidCircuitCalculator(const VolumePerPressureUnit& c, const VolumePerTimeUnit& f, const PressureTimeSquaredPerVolumeUnit& i, const PressureUnit& p, const VolumeUnit& q, const PressureTimePerVolumeUnit& r, Logger* logger) : SECircuitCalculator<SEFluidCircuit, SEFluidCircuitNode, SEFluidCircuitPath, SEFluidBlackBox, VolumePerPressureUnit, VolumePerTimeUnit, PressureTimeSquaredPerVolumeUnit, PressureUnit, VolumeUnit, PressureTimePerVolumeUnit>(c, f, i, p, q, r, logger)
{

}
SEFluidCircuitCalculator::~SEFluidCircuitCalculator()
{
  
}
