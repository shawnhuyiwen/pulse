/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDataModel.h"
#include "cdm/blackbox/SEBlackBox.h"

#include "cdm/circuit/electrical/SEElectricalCircuitPath.h"
#include "cdm/circuit/fluid/SEFluidCircuitPath.h"
#include "cdm/circuit/thermal/SEThermalCircuitPath.h"

#include "cdm/blackbox/electrical/SEElectricalBlackBox.h"
template class SEBlackBox<FLUID_BLACK_BOX>;
#include "cdm/blackbox/fluid/SEGasBlackBox.h"
#include "cdm/blackbox/fluid/SELiquidBlackBox.h"
template class SEBlackBox<ELECTRICAL_BLACK_BOX>;
#include "cdm/blackbox/thermal/SEThermalBlackBox.h"
template class SEBlackBox<THERMAL_BLACK_BOX>;
