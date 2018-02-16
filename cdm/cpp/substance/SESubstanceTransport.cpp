/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"

#include "substance/SESubstanceTransport.h"

#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"

SEGasTransporter::SEGasTransporter(Logger* logger) : SESubstanceTransporter<SEGasTransportGraph, VolumePerTimeUnit, VolumeUnit, VolumeUnit, NoUnit>(
  VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, NoUnit::unitless, logger)
{

}
SEGasTransporter::SEGasTransporter(const VolumePerTimeUnit& fUnit, const VolumeUnit& qUnit, const VolumeUnit& eUnit, Logger* logger)
  : SESubstanceTransporter<SEGasTransportGraph, VolumePerTimeUnit, VolumeUnit, VolumeUnit, NoUnit>(fUnit, qUnit, eUnit, NoUnit::unitless, logger)
{

}
SEGasTransporter::~SEGasTransporter()
{

}

SELiquidTransporter::SELiquidTransporter(Logger* logger) : SESubstanceTransporter<SELiquidTransportGraph, VolumePerTimeUnit, VolumeUnit, MassUnit, MassPerVolumeUnit>(
  VolumePerTimeUnit::L_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, logger)
{

}
SELiquidTransporter::SELiquidTransporter(const VolumePerTimeUnit& fUnit, const VolumeUnit& qUnit, const MassUnit& eUnit, const MassPerVolumeUnit& iUnit, Logger* logger)
  : SESubstanceTransporter<SELiquidTransportGraph, VolumePerTimeUnit, VolumeUnit, MassUnit, MassPerVolumeUnit>(fUnit, qUnit, eUnit, iUnit, logger)
{

}
SELiquidTransporter::~SELiquidTransporter()
{

}