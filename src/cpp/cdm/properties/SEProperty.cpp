/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEProperty.h"

SEProperty::SEProperty()
{
  Clear();
}

SEProperty::~SEProperty()
{
  Clear();
}

void SEProperty::Clear()
{
}

#include "properties/SEScalarAmount.h"
#include "properties/SEScalarAmountPerMass.h"
#include "properties/SEScalarAmountPerTime.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalarAreaPerTimePressure.h"
#include "properties/SEScalarElectricCapacitance.h"
#include "properties/SEScalarElectricCharge.h"
#include "properties/SEScalarElectricCurrent.h"
#include "properties/SEScalarElectricInductance.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEScalarElectricResistance.h"
#include "properties/SEScalarEnergy.h"
#include "properties/SEScalarEnergyPerAmount.h"
#include "properties/SEScalarEnergyPerMass.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarPressureTimeSquaredPerVolume.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarForce.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarHeatCapacitance.h"
#include "properties/SEScalarHeatCapacitancePerAmount.h"
#include "properties/SEScalarHeatCapacitancePerMass.h"
#include "properties/SEScalarHeatConductance.h"
#include "properties/SEScalarHeatConductancePerArea.h"
#include "properties/SEScalarHeatInductance.h"
#include "properties/SEScalarHeatResistance.h"
#include "properties/SEScalarHeatResistanceArea.h"
#include "properties/SEScalarInversePressure.h"
#include "properties/SEScalarInverseVolume.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalarLengthPerTime.h"
#include "properties/SEScalarLengthPerTimePressure.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarMassPerAreaTime.h"
#include "properties/SEScalarMassPerMass.h"
#include "properties/SEScalarMassPerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarOsmolality.h"
#include "properties/SEScalarOsmolarity.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarPowerPerAreaTemperatureToTheFourth.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarPressureTimePerArea.h"
#include "properties/SEScalarPressureTimePerVolumeArea.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolumePerTimeArea.h"
#include "properties/SEScalarVolumePerTimeMass.h"
#include "properties/SEScalarVolumePerTimePressure.h"
#include "properties/SEScalarVolumePerTimePressureArea.h"

double Convert(double d, const CCompoundUnit& from, const CCompoundUnit& to)
{
  if (&from == &to)
    return d;
  // I am assuming we are not going to do Quantity A to Quantity B Conversions
  return CUnitConversionEngine::GetEngine().QuickConvertValue(d, from, to);
}

bool   CompatibleUnits(const CCompoundUnit& from, const CCompoundUnit& to)
{
  if (from == to)
    return true;
  if (from.GetDimension() == to.GetDimension())
    return true;
  // See if the quantity types (Dimensions) are convertable
  double fromExp;
  CCompoundUnit mappingUnit;
  CUnitConversionEngine &uce = CUnitConversionEngine::GetEngine();
  if (uce.GetQuantityConversionParams(from.GetDimension(), to.GetDimension(), fromExp, mappingUnit))
    return true;
  return false;
}

#define IS_A(UNIT) \
  try \
  { \
    auto& ccu = UNIT::GetCompoundUnit(unit); \
    UnitMap[unit] = &ccu; \
    return ccu; \
  } catch (...) {} \

const CCompoundUnit& GetUnit(const std::string& unit)
{
  static std::unordered_map<std::string, const CCompoundUnit*> UnitMap;

  auto u = UnitMap.find(unit);
  if (u != UnitMap.end())
    return *u->second;

  IS_A(AmountUnit);
  IS_A(AmountPerMassUnit);
  IS_A(AmountPerTimeUnit);
  IS_A(AmountPerVolumeUnit);
  IS_A(AreaUnit);
  IS_A(AreaPerTimePressureUnit);
  IS_A(ElectricCapacitanceUnit);
  IS_A(ElectricChargeUnit);
  IS_A(ElectricCurrentUnit);
  IS_A(ElectricInductanceUnit);
  IS_A(ElectricPotentialUnit);
  IS_A(ElectricResistanceUnit);
  IS_A(EnergyUnit);
  IS_A(EnergyPerAmountUnit);
  IS_A(EnergyPerMassUnit);
  IS_A(VolumePerPressureUnit);
  IS_A(PressurePerVolumeUnit);
  IS_A(PressureTimeSquaredPerVolumeUnit);
  IS_A(PressureTimePerVolumeUnit);
  IS_A(ForceUnit);
  IS_A(FrequencyUnit);
  IS_A(HeatCapacitanceUnit);
  IS_A(HeatCapacitancePerAmountUnit);
  IS_A(HeatCapacitancePerMassUnit);
  IS_A(HeatConductanceUnit);
  IS_A(HeatConductancePerAreaUnit);
  IS_A(HeatInductanceUnit);
  IS_A(HeatResistanceUnit);
  IS_A(HeatResistanceAreaUnit);
  IS_A(InversePressureUnit);
  IS_A(InverseVolumeUnit);
  IS_A(LengthUnit);
  IS_A(LengthPerTimeUnit);
  IS_A(LengthPerTimePressureUnit);
  IS_A(MassUnit);
  IS_A(MassPerAmountUnit);
  IS_A(MassPerAreaTimeUnit);
  IS_A(MassPerMassUnit);
  IS_A(MassPerTimeUnit);
  IS_A(MassPerVolumeUnit);
  IS_A(OsmolalityUnit);
  IS_A(OsmolarityUnit);
  IS_A(PowerUnit);
  IS_A(PowerPerAreaTemperatureToTheFourthUnit);
  IS_A(PressureUnit);
  IS_A(PressurePerVolumeUnit);
  IS_A(PressureTimePerAreaUnit);
  IS_A(PressureTimePerVolumeAreaUnit);
  IS_A(TemperatureUnit);
  IS_A(TimeUnit);
  IS_A(VolumeUnit);
  IS_A(VolumePerPressureUnit);
  IS_A(VolumePerTimeUnit);
  IS_A(VolumePerTimeAreaUnit);
  IS_A(VolumePerTimeMassUnit);
  IS_A(VolumePerTimePressureUnit);
  IS_A(VolumePerTimePressureAreaUnit);

  throw CommonDataModelException("Could not map unit " + unit + " to any CDM Unit Type");
}
