/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEProperty.h"

SEProperty::SEProperty()
{

}

SEProperty::~SEProperty()
{

}

#include "cdm/properties/SEScalarAmount.h"
#include "cdm/properties/SEScalarAmountPerMass.h"
#include "cdm/properties/SEScalarAmountPerTime.h"
#include "cdm/properties/SEScalarAmountPerVolume.h"
#include "cdm/properties/SEScalarArea.h"
#include "cdm/properties/SEScalarAreaPerTimePressure.h"
#include "cdm/properties/SEScalarElectricCapacitance.h"
#include "cdm/properties/SEScalarElectricCharge.h"
#include "cdm/properties/SEScalarElectricCurrent.h"
#include "cdm/properties/SEScalarElectricInductance.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarElectricResistance.h"
#include "cdm/properties/SEScalarEnergy.h"
#include "cdm/properties/SEScalarEnergyPerAmount.h"
#include "cdm/properties/SEScalarEnergyPerMass.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"
#include "cdm/properties/SEScalarPressureTimeSquaredPerVolume.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarForce.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarHeatCapacitance.h"
#include "cdm/properties/SEScalarHeatCapacitancePerAmount.h"
#include "cdm/properties/SEScalarHeatCapacitancePerMass.h"
#include "cdm/properties/SEScalarHeatConductance.h"
#include "cdm/properties/SEScalarHeatConductancePerArea.h"
#include "cdm/properties/SEScalarHeatInductance.h"
#include "cdm/properties/SEScalarHeatResistance.h"
#include "cdm/properties/SEScalarHeatResistanceArea.h"
#include "cdm/properties/SEScalarInversePressure.h"
#include "cdm/properties/SEScalarInverseVolume.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarLengthPerTime.h"
#include "cdm/properties/SEScalarLengthPerTimePressure.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerAmount.h"
#include "cdm/properties/SEScalarMassPerAreaTime.h"
#include "cdm/properties/SEScalarMassPerMass.h"
#include "cdm/properties/SEScalarMassPerTime.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarOsmolality.h"
#include "cdm/properties/SEScalarOsmolarity.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"
#include "cdm/properties/SEScalarPressureTimePerArea.h"
#include "cdm/properties/SEScalarPressureTimePerVolumeArea.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolumePerTimeArea.h"
#include "cdm/properties/SEScalarVolumePerTimeMass.h"
#include "cdm/properties/SEScalarVolumePerTimePressure.h"
#include "cdm/properties/SEScalarVolumePerTimePressureArea.h"

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
