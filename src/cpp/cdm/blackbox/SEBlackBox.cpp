/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "blackbox/SEBlackBox.h"

#include "circuit/electrical/SEElectricalCircuitPath.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "circuit/thermal/SEThermalCircuitPath.h"

#include "blackbox/electrical/SEElectricalBlackBox.h"
template class SEBlackBox<FLUID_BLACK_BOX>;
#include "blackbox/fluid/SEGasBlackBox.h"
#include "blackbox/fluid/SELiquidBlackBox.h"
template class SEBlackBox<ELECTRICAL_BLACK_BOX>;
#include "blackbox/thermal/SEThermalBlackBox.h"
template class SEBlackBox<THERMAL_BLACK_BOX>;
