/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Properties.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBProperties.h"

#include "properties/SEScalar0To1.h"
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
#include "properties/SEScalarNegative1To1.h"
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

#include "properties/SEFunctionElectricPotentialVsTime.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SEHistogramFractionVsLength.h"

#include "properties/SERunningAverage.h"

void PBProperty::Load(const CDM_BIND::ScalarData& src, SEScalar& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarData& src, SEScalar& dst)
{
  dst.SetValue(src.value());
  if (!src.unit().empty())
  {
    if (src.unit() != "unitless")
      throw CommonDataModelException("CDM_BIND::Scalar API is intended to be unitless, You are trying to load a ScalarData with a unit defined");
  }
  dst.m_readOnly = src.readonly();
}
CDM_BIND::ScalarData* PBProperty::Unload(const SEScalar& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarData* dst = new CDM_BIND::ScalarData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalar& src, CDM_BIND::ScalarData& dst)
{
  dst.set_value(src.m_value);
  dst.set_readonly(src.m_readOnly);
}
void PBProperty::Serialize(const CDM_BIND::ScalarData& src, SEUnitScalar& dst)
{
  if (!src.unit().empty())
    dst.SetValue(src.value(), *dst.GetCompoundUnit(src.unit()));
  else
    throw CommonDataModelException("ScalarQuantity attempted to load a ScalarData with no unit, must have a unit.");
  dst.m_readOnly = src.readonly();
}
void PBProperty::Serialize(const SEUnitScalar& src, CDM_BIND::ScalarData& dst)
{
  dst.set_value(src.m_value);
  if (src.HasUnit())
    dst.set_unit(src.GetUnit()->GetString());
  else
    throw CommonDataModelException("ScalarQuantity attempted to unload a ScalarData with no unit, must have a unit.");
  dst.set_readonly(src.m_readOnly);
}


void PBProperty::Load(const CDM_BIND::Scalar0To1Data& src, SEScalar0To1& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::Scalar0To1Data& src, SEScalar0To1& dst)
{
  PBProperty::Serialize(src.scalar0to1(), dst);
}
CDM_BIND::Scalar0To1Data* PBProperty::Unload(const SEScalar0To1& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::Scalar0To1Data* dst = new CDM_BIND::Scalar0To1Data();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalar0To1& src, CDM_BIND::Scalar0To1Data& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalar0to1());
}

void PBProperty::Load(const CDM_BIND::ScalarAmountData& src, SEScalarAmount& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarAmountData& src, SEScalarAmount& dst)
{
  PBProperty::Serialize(src.scalaramount(), dst);
}
CDM_BIND::ScalarAmountData* PBProperty::Unload(const SEScalarAmount& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarAmountData* dst = new CDM_BIND::ScalarAmountData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAmount& src, CDM_BIND::ScalarAmountData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalaramount());
}


void PBProperty::Load(const CDM_BIND::ScalarAmountPerMassData& src, SEScalarAmountPerMass& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarAmountPerMassData& src, SEScalarAmountPerMass& dst)
{
  PBProperty::Serialize(src.scalaramountpermass(), dst);
}
CDM_BIND::ScalarAmountPerMassData* PBProperty::Unload(const SEScalarAmountPerMass& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarAmountPerMassData* dst = new CDM_BIND::ScalarAmountPerMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAmountPerMass& src, CDM_BIND::ScalarAmountPerMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalaramountpermass());
}

void PBProperty::Load(const CDM_BIND::ScalarAmountPerTimeData& src, SEScalarAmountPerTime& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarAmountPerTimeData& src, SEScalarAmountPerTime& dst)
{
  PBProperty::Serialize(src.scalaramountpertime(), dst);
}
CDM_BIND::ScalarAmountPerTimeData* PBProperty::Unload(const SEScalarAmountPerTime& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarAmountPerTimeData* dst = new CDM_BIND::ScalarAmountPerTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAmountPerTime& src, CDM_BIND::ScalarAmountPerTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalaramountpertime());
}

void PBProperty::Load(const CDM_BIND::ScalarAmountPerVolumeData& src, SEScalarAmountPerVolume& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarAmountPerVolumeData& src, SEScalarAmountPerVolume& dst)
{
  PBProperty::Serialize(src.scalaramountpervolume(), dst);
}
CDM_BIND::ScalarAmountPerVolumeData* PBProperty::Unload(const SEScalarAmountPerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarAmountPerVolumeData* dst = new CDM_BIND::ScalarAmountPerVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAmountPerVolume& src, CDM_BIND::ScalarAmountPerVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalaramountpervolume());
}

void PBProperty::Load(const CDM_BIND::ScalarAreaData& src, SEScalarArea& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarAreaData& src, SEScalarArea& dst)
{
  PBProperty::Serialize(src.scalararea(), dst);
}
CDM_BIND::ScalarAreaData* PBProperty::Unload(const SEScalarArea& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarAreaData* dst = new CDM_BIND::ScalarAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarArea& src, CDM_BIND::ScalarAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalararea());
}

void PBProperty::Load(const CDM_BIND::ScalarAreaPerTimePressureData& src, SEScalarAreaPerTimePressure& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarAreaPerTimePressureData& src, SEScalarAreaPerTimePressure& dst)
{
  PBProperty::Serialize(src.scalarareapertimepressure(), dst);
}
CDM_BIND::ScalarAreaPerTimePressureData* PBProperty::Unload(const SEScalarAreaPerTimePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarAreaPerTimePressureData* dst = new CDM_BIND::ScalarAreaPerTimePressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAreaPerTimePressure& src, CDM_BIND::ScalarAreaPerTimePressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarareapertimepressure());
}

void PBProperty::Load(const CDM_BIND::ScalarElectricCapacitanceData& src, SEScalarElectricCapacitance& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarElectricCapacitanceData& src, SEScalarElectricCapacitance& dst)
{
  PBProperty::Serialize(src.scalarelectriccapacitance(), dst);
}
CDM_BIND::ScalarElectricCapacitanceData* PBProperty::Unload(const SEScalarElectricCapacitance& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarElectricCapacitanceData* dst = new CDM_BIND::ScalarElectricCapacitanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricCapacitance& src, CDM_BIND::ScalarElectricCapacitanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectriccapacitance());
}

void PBProperty::Load(const CDM_BIND::ScalarElectricChargeData& src, SEScalarElectricCharge& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarElectricChargeData& src, SEScalarElectricCharge& dst)
{
  PBProperty::Serialize(src.scalarelectriccharge(), dst);
}
CDM_BIND::ScalarElectricChargeData* PBProperty::Unload(const SEScalarElectricCharge& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarElectricChargeData* dst = new CDM_BIND::ScalarElectricChargeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricCharge& src, CDM_BIND::ScalarElectricChargeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectriccharge());
}

void PBProperty::Load(const CDM_BIND::ScalarElectricCurrentData& src, SEScalarElectricCurrent& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarElectricCurrentData& src, SEScalarElectricCurrent& dst)
{
  PBProperty::Serialize(src.scalarelectriccurrent(), dst);
}
CDM_BIND::ScalarElectricCurrentData* PBProperty::Unload(const SEScalarElectricCurrent& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarElectricCurrentData* dst = new CDM_BIND::ScalarElectricCurrentData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricCurrent& src, CDM_BIND::ScalarElectricCurrentData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectriccurrent());
}

void PBProperty::Load(const CDM_BIND::ScalarElectricInductanceData& src, SEScalarElectricInductance& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarElectricInductanceData& src, SEScalarElectricInductance& dst)
{
  PBProperty::Serialize(src.scalarelectricinductance(), dst);
}
CDM_BIND::ScalarElectricInductanceData* PBProperty::Unload(const SEScalarElectricInductance& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarElectricInductanceData* dst = new CDM_BIND::ScalarElectricInductanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricInductance& src, CDM_BIND::ScalarElectricInductanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectricinductance());
}

void PBProperty::Load(const CDM_BIND::ScalarElectricPotentialData& src, SEScalarElectricPotential& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarElectricPotentialData& src, SEScalarElectricPotential& dst)
{
  PBProperty::Serialize(src.scalarelectricpotential(), dst);
}
CDM_BIND::ScalarElectricPotentialData* PBProperty::Unload(const SEScalarElectricPotential& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarElectricPotentialData* dst = new CDM_BIND::ScalarElectricPotentialData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricPotential& src, CDM_BIND::ScalarElectricPotentialData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectricpotential());
}

void PBProperty::Load(const CDM_BIND::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst)
{
  PBProperty::Serialize(src.scalarelectricresistance(), dst);
}
CDM_BIND::ScalarElectricResistanceData* PBProperty::Unload(const SEScalarElectricResistance& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarElectricResistanceData* dst = new CDM_BIND::ScalarElectricResistanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricResistance& src, CDM_BIND::ScalarElectricResistanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectricresistance());
}

void PBProperty::Load(const CDM_BIND::ScalarEnergyData& src, SEScalarEnergy& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarEnergyData& src, SEScalarEnergy& dst)
{
  PBProperty::Serialize(src.scalarenergy(), dst);
}
CDM_BIND::ScalarEnergyData* PBProperty::Unload(const SEScalarEnergy& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarEnergyData* dst = new CDM_BIND::ScalarEnergyData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarEnergy& src, CDM_BIND::ScalarEnergyData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarenergy());
}

void PBProperty::Load(const CDM_BIND::ScalarEnergyPerAmountData& src, SEScalarEnergyPerAmount& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarEnergyPerAmountData& src, SEScalarEnergyPerAmount& dst)
{
  PBProperty::Serialize(src.scalarenergyperamount(), dst);
}
CDM_BIND::ScalarEnergyPerAmountData* PBProperty::Unload(const SEScalarEnergyPerAmount& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarEnergyPerAmountData* dst = new CDM_BIND::ScalarEnergyPerAmountData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarEnergyPerAmount& src, CDM_BIND::ScalarEnergyPerAmountData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarenergyperamount());
}

void PBProperty::Load(const CDM_BIND::ScalarEnergyPerMassData& src, SEScalarEnergyPerMass& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarEnergyPerMassData& src, SEScalarEnergyPerMass& dst)
{
  PBProperty::Serialize(src.scalarenergypermass(), dst);
}
CDM_BIND::ScalarEnergyPerMassData* PBProperty::Unload(const SEScalarEnergyPerMass& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarEnergyPerMassData* dst = new CDM_BIND::ScalarEnergyPerMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarEnergyPerMass& src, CDM_BIND::ScalarEnergyPerMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarenergypermass());
}

void PBProperty::Load(const CDM_BIND::ScalarForceData& src, SEScalarForce& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarForceData& src, SEScalarForce& dst)
{
  PBProperty::Serialize(src.scalarforce(), dst);
}
CDM_BIND::ScalarForceData* PBProperty::Unload(const SEScalarForce& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarForceData* dst = new CDM_BIND::ScalarForceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarForce& src, CDM_BIND::ScalarForceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarforce());
}

void PBProperty::Load(const CDM_BIND::ScalarFrequencyData& src, SEScalarFrequency& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarFrequencyData& src, SEScalarFrequency& dst)
{
  PBProperty::Serialize(src.scalarfrequency(), dst);
}
CDM_BIND::ScalarFrequencyData* PBProperty::Unload(const SEScalarFrequency& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarFrequencyData* dst = new CDM_BIND::ScalarFrequencyData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarFrequency& src, CDM_BIND::ScalarFrequencyData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarfrequency());
}

void PBProperty::Load(const CDM_BIND::ScalarHeatCapacitanceData& src, SEScalarHeatCapacitance& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarHeatCapacitanceData& src, SEScalarHeatCapacitance& dst)
{
  PBProperty::Serialize(src.scalarheatcapacitance(), dst);
}
CDM_BIND::ScalarHeatCapacitanceData* PBProperty::Unload(const SEScalarHeatCapacitance& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarHeatCapacitanceData* dst = new CDM_BIND::ScalarHeatCapacitanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatCapacitance& src, CDM_BIND::ScalarHeatCapacitanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatcapacitance());
}

void PBProperty::Load(const CDM_BIND::ScalarHeatCapacitancePerAmountData& src, SEScalarHeatCapacitancePerAmount& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarHeatCapacitancePerAmountData& src, SEScalarHeatCapacitancePerAmount& dst)
{
  PBProperty::Serialize(src.scalarheatcapacitanceperamount(), dst);
}
CDM_BIND::ScalarHeatCapacitancePerAmountData* PBProperty::Unload(const SEScalarHeatCapacitancePerAmount& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarHeatCapacitancePerAmountData* dst = new CDM_BIND::ScalarHeatCapacitancePerAmountData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatCapacitancePerAmount& src, CDM_BIND::ScalarHeatCapacitancePerAmountData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatcapacitanceperamount());
}

void PBProperty::Load(const CDM_BIND::ScalarHeatCapacitancePerMassData& src, SEScalarHeatCapacitancePerMass& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarHeatCapacitancePerMassData& src, SEScalarHeatCapacitancePerMass& dst)
{
  PBProperty::Serialize(src.scalarheatcapacitancepermass(), dst);
}
CDM_BIND::ScalarHeatCapacitancePerMassData* PBProperty::Unload(const SEScalarHeatCapacitancePerMass& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarHeatCapacitancePerMassData* dst = new CDM_BIND::ScalarHeatCapacitancePerMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatCapacitancePerMass& src, CDM_BIND::ScalarHeatCapacitancePerMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatcapacitancepermass());
}

void PBProperty::Load(const CDM_BIND::ScalarHeatConductanceData& src, SEScalarHeatConductance& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarHeatConductanceData& src, SEScalarHeatConductance& dst)
{
  PBProperty::Serialize(src.scalarheatconductance(), dst);
}
CDM_BIND::ScalarHeatConductanceData* PBProperty::Unload(const SEScalarHeatConductance& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarHeatConductanceData* dst = new CDM_BIND::ScalarHeatConductanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatConductance& src, CDM_BIND::ScalarHeatConductanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatconductance());
}

void PBProperty::Load(const CDM_BIND::ScalarHeatConductancePerAreaData& src, SEScalarHeatConductancePerArea& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarHeatConductancePerAreaData& src, SEScalarHeatConductancePerArea& dst)
{
  PBProperty::Serialize(src.scalarheatconductanceperarea(), dst);
}
CDM_BIND::ScalarHeatConductancePerAreaData* PBProperty::Unload(const SEScalarHeatConductancePerArea& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarHeatConductancePerAreaData* dst = new CDM_BIND::ScalarHeatConductancePerAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatConductancePerArea& src, CDM_BIND::ScalarHeatConductancePerAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatconductanceperarea());
}

void PBProperty::Load(const CDM_BIND::ScalarHeatInductanceData& src, SEScalarHeatInductance& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarHeatInductanceData& src, SEScalarHeatInductance& dst)
{
  PBProperty::Serialize(src.scalarheatinductance(), dst);
}
CDM_BIND::ScalarHeatInductanceData* PBProperty::Unload(const SEScalarHeatInductance& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarHeatInductanceData* dst = new CDM_BIND::ScalarHeatInductanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatInductance& src, CDM_BIND::ScalarHeatInductanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatinductance());
}

void PBProperty::Load(const CDM_BIND::ScalarHeatResistanceData& src, SEScalarHeatResistance& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarHeatResistanceData& src, SEScalarHeatResistance& dst)
{
  PBProperty::Serialize(src.scalarheatresistance(), dst);
}
CDM_BIND::ScalarHeatResistanceData* PBProperty::Unload(const SEScalarHeatResistance& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarHeatResistanceData* dst = new CDM_BIND::ScalarHeatResistanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatResistance& src, CDM_BIND::ScalarHeatResistanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatresistance());
}

void PBProperty::Load(const CDM_BIND::ScalarHeatResistanceAreaData& src, SEScalarHeatResistanceArea& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarHeatResistanceAreaData& src, SEScalarHeatResistanceArea& dst)
{
  PBProperty::Serialize(src.scalarheatresistancearea(), dst);
}
CDM_BIND::ScalarHeatResistanceAreaData* PBProperty::Unload(const SEScalarHeatResistanceArea& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarHeatResistanceAreaData* dst = new CDM_BIND::ScalarHeatResistanceAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatResistanceArea& src, CDM_BIND::ScalarHeatResistanceAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatresistancearea());
}

void PBProperty::Load(const CDM_BIND::ScalarInversePressureData& src, SEScalarInversePressure& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarInversePressureData& src, SEScalarInversePressure& dst)
{
  PBProperty::Serialize(src.scalarinversepressure(), dst);
}
CDM_BIND::ScalarInversePressureData* PBProperty::Unload(const SEScalarInversePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarInversePressureData* dst = new CDM_BIND::ScalarInversePressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarInversePressure& src, CDM_BIND::ScalarInversePressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarinversepressure());
}

void PBProperty::Load(const CDM_BIND::ScalarInverseVolumeData& src, SEScalarInverseVolume& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarInverseVolumeData& src, SEScalarInverseVolume& dst)
{
  PBProperty::Serialize(src.scalarinversevolume(), dst);
}
CDM_BIND::ScalarInverseVolumeData* PBProperty::Unload(const SEScalarInverseVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarInverseVolumeData* dst = new CDM_BIND::ScalarInverseVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarInverseVolume& src, CDM_BIND::ScalarInverseVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarinversevolume());
}

void PBProperty::Load(const CDM_BIND::ScalarLengthData& src, SEScalarLength& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarLengthData& src, SEScalarLength& dst)
{
  PBProperty::Serialize(src.scalarlength(), dst);
}
CDM_BIND::ScalarLengthData* PBProperty::Unload(const SEScalarLength& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarLengthData* dst = new CDM_BIND::ScalarLengthData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarLength& src, CDM_BIND::ScalarLengthData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarlength());
}

void PBProperty::Load(const CDM_BIND::ScalarLengthPerTimeData& src, SEScalarLengthPerTime& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarLengthPerTimeData& src, SEScalarLengthPerTime& dst)
{
  PBProperty::Serialize(src.scalarlengthpertime(), dst);
}
CDM_BIND::ScalarLengthPerTimeData* PBProperty::Unload(const SEScalarLengthPerTime& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarLengthPerTimeData* dst = new CDM_BIND::ScalarLengthPerTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarLengthPerTime& src, CDM_BIND::ScalarLengthPerTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarlengthpertime());
}

void PBProperty::Load(const CDM_BIND::ScalarLengthPerTimePressureData& src, SEScalarLengthPerTimePressure& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarLengthPerTimePressureData& src, SEScalarLengthPerTimePressure& dst)
{
  PBProperty::Serialize(src.scalarlengthpertimepressure(), dst);
}
CDM_BIND::ScalarLengthPerTimePressureData* PBProperty::Unload(const SEScalarLengthPerTimePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarLengthPerTimePressureData* dst = new CDM_BIND::ScalarLengthPerTimePressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarLengthPerTimePressure& src, CDM_BIND::ScalarLengthPerTimePressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarlengthpertimepressure());
}

void PBProperty::Load(const CDM_BIND::ScalarMassData& src, SEScalarMass& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarMassData& src, SEScalarMass& dst)
{
  PBProperty::Serialize(src.scalarmass(), dst);
}
CDM_BIND::ScalarMassData* PBProperty::Unload(const SEScalarMass& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarMassData* dst = new CDM_BIND::ScalarMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMass& src, CDM_BIND::ScalarMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmass());
}

void PBProperty::Load(const CDM_BIND::ScalarMassPerAmountData& src, SEScalarMassPerAmount& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarMassPerAmountData& src, SEScalarMassPerAmount& dst)
{
  PBProperty::Serialize(src.scalarmassperamount(), dst);
}
CDM_BIND::ScalarMassPerAmountData* PBProperty::Unload(const SEScalarMassPerAmount& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarMassPerAmountData* dst = new CDM_BIND::ScalarMassPerAmountData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerAmount& src, CDM_BIND::ScalarMassPerAmountData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmassperamount());
}

void PBProperty::Load(const CDM_BIND::ScalarMassPerAreaTimeData& src, SEScalarMassPerAreaTime& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarMassPerAreaTimeData& src, SEScalarMassPerAreaTime& dst)
{
  PBProperty::Serialize(src.scalarmassperareatime(), dst);
}
CDM_BIND::ScalarMassPerAreaTimeData* PBProperty::Unload(const SEScalarMassPerAreaTime& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarMassPerAreaTimeData* dst = new CDM_BIND::ScalarMassPerAreaTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerAreaTime& src, CDM_BIND::ScalarMassPerAreaTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmassperareatime());
}

void PBProperty::Load(const CDM_BIND::ScalarMassPerMassData& src, SEScalarMassPerMass& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarMassPerMassData& src, SEScalarMassPerMass& dst)
{
  PBProperty::Serialize(src.scalarmasspermass(), dst);
}
CDM_BIND::ScalarMassPerMassData* PBProperty::Unload(const SEScalarMassPerMass& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarMassPerMassData* dst = new CDM_BIND::ScalarMassPerMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerMass& src, CDM_BIND::ScalarMassPerMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmasspermass());
}

void PBProperty::Load(const CDM_BIND::ScalarMassPerTimeData& src, SEScalarMassPerTime& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarMassPerTimeData& src, SEScalarMassPerTime& dst)
{
  PBProperty::Serialize(src.scalarmasspertime(), dst);
}
CDM_BIND::ScalarMassPerTimeData* PBProperty::Unload(const SEScalarMassPerTime& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarMassPerTimeData* dst = new CDM_BIND::ScalarMassPerTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerTime& src, CDM_BIND::ScalarMassPerTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmasspertime());
}

void PBProperty::Load(const CDM_BIND::ScalarMassPerVolumeData& src, SEScalarMassPerVolume& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarMassPerVolumeData& src, SEScalarMassPerVolume& dst)
{
  PBProperty::Serialize(src.scalarmasspervolume(), dst);
}
CDM_BIND::ScalarMassPerVolumeData* PBProperty::Unload(const SEScalarMassPerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarMassPerVolumeData* dst = new CDM_BIND::ScalarMassPerVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerVolume& src, CDM_BIND::ScalarMassPerVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmasspervolume());
}

void PBProperty::Load(const CDM_BIND::ScalarNegative1To1Data& src, SEScalarNegative1To1& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarNegative1To1Data& src, SEScalarNegative1To1& dst)
{
  PBProperty::Serialize(src.scalarnegative1to1(), dst);
}
CDM_BIND::ScalarNegative1To1Data* PBProperty::Unload(const SEScalarNegative1To1& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarNegative1To1Data* dst = new CDM_BIND::ScalarNegative1To1Data();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarNegative1To1& src, CDM_BIND::ScalarNegative1To1Data& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarnegative1to1());
}

void PBProperty::Load(const CDM_BIND::ScalarOsmolalityData& src, SEScalarOsmolality& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarOsmolalityData& src, SEScalarOsmolality& dst)
{
  PBProperty::Serialize(src.scalarosmolality(), dst);
}
CDM_BIND::ScalarOsmolalityData* PBProperty::Unload(const SEScalarOsmolality& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarOsmolalityData* dst = new CDM_BIND::ScalarOsmolalityData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarOsmolality& src, CDM_BIND::ScalarOsmolalityData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarosmolality());
}

void PBProperty::Load(const CDM_BIND::ScalarOsmolarityData& src, SEScalarOsmolarity& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarOsmolarityData& src, SEScalarOsmolarity& dst)
{
  PBProperty::Serialize(src.scalarosmolarity(), dst);
}
CDM_BIND::ScalarOsmolarityData* PBProperty::Unload(const SEScalarOsmolarity& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarOsmolarityData* dst = new CDM_BIND::ScalarOsmolarityData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarOsmolarity& src, CDM_BIND::ScalarOsmolarityData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarosmolarity());
}

void PBProperty::Load(const CDM_BIND::ScalarPowerData& src, SEScalarPower& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarPowerData& src, SEScalarPower& dst)
{
  PBProperty::Serialize(src.scalarpower(), dst);
}
CDM_BIND::ScalarPowerData* PBProperty::Unload(const SEScalarPower& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarPowerData* dst = new CDM_BIND::ScalarPowerData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPower& src, CDM_BIND::ScalarPowerData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpower());
}

void PBProperty::Load(const CDM_BIND::ScalarPowerPerAreaTemperatureToTheFourthData& src, SEScalarPowerPerAreaTemperatureToTheFourth& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarPowerPerAreaTemperatureToTheFourthData& src, SEScalarPowerPerAreaTemperatureToTheFourth& dst)
{
  PBProperty::Serialize(src.scalarpowerperareatemperaturetothefourth(), dst);
}
CDM_BIND::ScalarPowerPerAreaTemperatureToTheFourthData* PBProperty::Unload(const SEScalarPowerPerAreaTemperatureToTheFourth& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarPowerPerAreaTemperatureToTheFourthData* dst = new CDM_BIND::ScalarPowerPerAreaTemperatureToTheFourthData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPowerPerAreaTemperatureToTheFourth& src, CDM_BIND::ScalarPowerPerAreaTemperatureToTheFourthData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpowerperareatemperaturetothefourth());
}

void PBProperty::Load(const CDM_BIND::ScalarPressureData& src, SEScalarPressure& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarPressureData& src, SEScalarPressure& dst)
{
  PBProperty::Serialize(src.scalarpressure(), dst);
}
CDM_BIND::ScalarPressureData* PBProperty::Unload(const SEScalarPressure& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarPressureData* dst = new CDM_BIND::ScalarPressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressure& src, CDM_BIND::ScalarPressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressure());
}

void PBProperty::Load(const CDM_BIND::ScalarPressurePerVolumeData& src, SEScalarPressurePerVolume& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarPressurePerVolumeData& src, SEScalarPressurePerVolume& dst)
{
  PBProperty::Serialize(src.scalarpressurepervolume(), dst);
}
CDM_BIND::ScalarPressurePerVolumeData* PBProperty::Unload(const SEScalarPressurePerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarPressurePerVolumeData* dst = new CDM_BIND::ScalarPressurePerVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressurePerVolume& src, CDM_BIND::ScalarPressurePerVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressurepervolume());
}

void PBProperty::Load(const CDM_BIND::ScalarPressureTimePerAreaData& src, SEScalarPressureTimePerArea& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarPressureTimePerAreaData& src, SEScalarPressureTimePerArea& dst)
{
  PBProperty::Serialize(src.scalarpressuretimeperarea(), dst);
}
CDM_BIND::ScalarPressureTimePerAreaData* PBProperty::Unload(const SEScalarPressureTimePerArea& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarPressureTimePerAreaData* dst = new CDM_BIND::ScalarPressureTimePerAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressureTimePerArea& src, CDM_BIND::ScalarPressureTimePerAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressuretimeperarea());
}

void PBProperty::Load(const CDM_BIND::ScalarPressureTimePerVolumeData& src, SEScalarPressureTimePerVolume& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarPressureTimePerVolumeData& src, SEScalarPressureTimePerVolume& dst)
{
  PBProperty::Serialize(src.scalarpressuretimepervolume(), dst);
}
CDM_BIND::ScalarPressureTimePerVolumeData* PBProperty::Unload(const SEScalarPressureTimePerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarPressureTimePerVolumeData* dst = new CDM_BIND::ScalarPressureTimePerVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressureTimePerVolume& src, CDM_BIND::ScalarPressureTimePerVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressuretimepervolume());
}

void PBProperty::Load(const CDM_BIND::ScalarPressureTimePerVolumeAreaData& src, SEScalarPressureTimePerVolumeArea& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarPressureTimePerVolumeAreaData& src, SEScalarPressureTimePerVolumeArea& dst)
{
  PBProperty::Serialize(src.scalarpressuretimepervolumearea(), dst);
}
CDM_BIND::ScalarPressureTimePerVolumeAreaData* PBProperty::Unload(const SEScalarPressureTimePerVolumeArea& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarPressureTimePerVolumeAreaData* dst = new CDM_BIND::ScalarPressureTimePerVolumeAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressureTimePerVolumeArea& src, CDM_BIND::ScalarPressureTimePerVolumeAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressuretimepervolumearea());
}

void PBProperty::Load(const CDM_BIND::ScalarPressureTimeSquaredPerVolumeData& src, SEScalarPressureTimeSquaredPerVolume& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarPressureTimeSquaredPerVolumeData& src, SEScalarPressureTimeSquaredPerVolume& dst)
{
  PBProperty::Serialize(src.scalarpressuretimesquaredpervolume(), dst);
}
CDM_BIND::ScalarPressureTimeSquaredPerVolumeData* PBProperty::Unload(const SEScalarPressureTimeSquaredPerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarPressureTimeSquaredPerVolumeData* dst = new CDM_BIND::ScalarPressureTimeSquaredPerVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressureTimeSquaredPerVolume& src, CDM_BIND::ScalarPressureTimeSquaredPerVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressuretimesquaredpervolume());
}

void PBProperty::Load(const CDM_BIND::ScalarTemperatureData& src, SEScalarTemperature& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarTemperatureData& src, SEScalarTemperature& dst)
{
  PBProperty::Serialize(src.scalartemperature(), dst);
}
CDM_BIND::ScalarTemperatureData* PBProperty::Unload(const SEScalarTemperature& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarTemperatureData* dst = new CDM_BIND::ScalarTemperatureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarTemperature& src, CDM_BIND::ScalarTemperatureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalartemperature());
}

void PBProperty::Load(const CDM_BIND::ScalarTimeData& src, SEScalarTime& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarTimeData& src, SEScalarTime& dst)
{
  PBProperty::Serialize(src.scalartime(), dst);
}
CDM_BIND::ScalarTimeData* PBProperty::Unload(const SEScalarTime& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarTimeData* dst = new CDM_BIND::ScalarTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarTime& src, CDM_BIND::ScalarTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalartime());
}

void PBProperty::Load(const CDM_BIND::ScalarVolumeData& src, SEScalarVolume& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarVolumeData& src, SEScalarVolume& dst)
{
  PBProperty::Serialize(src.scalarvolume(), dst);
}
CDM_BIND::ScalarVolumeData* PBProperty::Unload(const SEScalarVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarVolumeData* dst = new CDM_BIND::ScalarVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolume& src, CDM_BIND::ScalarVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolume());
}

void PBProperty::Load(const CDM_BIND::ScalarVolumePerPressureData& src, SEScalarVolumePerPressure& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarVolumePerPressureData& src, SEScalarVolumePerPressure& dst)
{
  PBProperty::Serialize(src.scalarvolumeperpressure(), dst);
}
CDM_BIND::ScalarVolumePerPressureData* PBProperty::Unload(const SEScalarVolumePerPressure& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarVolumePerPressureData* dst = new CDM_BIND::ScalarVolumePerPressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerPressure& src, CDM_BIND::ScalarVolumePerPressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumeperpressure());
}

void PBProperty::Load(const CDM_BIND::ScalarVolumePerTimeData& src, SEScalarVolumePerTime& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarVolumePerTimeData& src, SEScalarVolumePerTime& dst)
{
  PBProperty::Serialize(src.scalarvolumepertime(), dst);
}
CDM_BIND::ScalarVolumePerTimeData* PBProperty::Unload(const SEScalarVolumePerTime& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarVolumePerTimeData* dst = new CDM_BIND::ScalarVolumePerTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTime& src, CDM_BIND::ScalarVolumePerTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertime());
}

void PBProperty::Load(const CDM_BIND::ScalarVolumePerTimeAreaData& src, SEScalarVolumePerTimeArea& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarVolumePerTimeAreaData& src, SEScalarVolumePerTimeArea& dst)
{
  PBProperty::Serialize(src.scalarvolumepertimearea(), dst);
}
CDM_BIND::ScalarVolumePerTimeAreaData* PBProperty::Unload(const SEScalarVolumePerTimeArea& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarVolumePerTimeAreaData* dst = new CDM_BIND::ScalarVolumePerTimeAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTimeArea& src, CDM_BIND::ScalarVolumePerTimeAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertimearea());
}

void PBProperty::Load(const CDM_BIND::ScalarVolumePerTimeMassData& src, SEScalarVolumePerTimeMass& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarVolumePerTimeMassData& src, SEScalarVolumePerTimeMass& dst)
{
  PBProperty::Serialize(src.scalarvolumepertimemass(), dst);
}
CDM_BIND::ScalarVolumePerTimeMassData* PBProperty::Unload(const SEScalarVolumePerTimeMass& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarVolumePerTimeMassData* dst = new CDM_BIND::ScalarVolumePerTimeMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTimeMass& src, CDM_BIND::ScalarVolumePerTimeMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertimemass());
}

void PBProperty::Load(const CDM_BIND::ScalarVolumePerTimePressureData& src, SEScalarVolumePerTimePressure& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarVolumePerTimePressureData& src, SEScalarVolumePerTimePressure& dst)
{
  PBProperty::Serialize(src.scalarvolumepertimepressure(), dst);
}
CDM_BIND::ScalarVolumePerTimePressureData* PBProperty::Unload(const SEScalarVolumePerTimePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarVolumePerTimePressureData* dst = new CDM_BIND::ScalarVolumePerTimePressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTimePressure& src, CDM_BIND::ScalarVolumePerTimePressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertimepressure());
}

void PBProperty::Load(const CDM_BIND::ScalarVolumePerTimePressureAreaData& src, SEScalarVolumePerTimePressureArea& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::ScalarVolumePerTimePressureAreaData& src, SEScalarVolumePerTimePressureArea& dst)
{
  PBProperty::Serialize(src.scalarvolumepertimepressurearea(), dst);
}

CDM_BIND::ScalarVolumePerTimePressureAreaData* PBProperty::Unload(const SEScalarVolumePerTimePressureArea& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::ScalarVolumePerTimePressureAreaData* dst = new CDM_BIND::ScalarVolumePerTimePressureAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTimePressureArea& src, CDM_BIND::ScalarVolumePerTimePressureAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertimepressurearea());
}
///////////////
// Functions //
///////////////

void PBProperty::Load(const CDM_BIND::FunctionData& src, SEFunction& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);

  if (!src.dependentunit().empty())
  {
    if (src.dependentunit() != "unitless")
      throw CommonDataModelException("CDM_BIND::Function API is intended to be unitless, You are trying to load a dependent axis with a unit defined");
  }
  if (!src.independentunit().empty())
  {
    if (src.independentunit() != "unitless")
      throw CommonDataModelException("CDM_BIND::Function API is intended to be unitless, You are trying to load an independent axis with a unit defined");
  }
}
void PBProperty::Serialize(const CDM_BIND::FunctionData& src, SEFunction& dst)
{
  for (int i = 0; i < src.dependent().value_size(); i++)
    dst.m_Dependent.push_back(src.dependent().value(i));
  for (int i = 0; i < src.independent().value_size(); i++)
    dst.m_Independent.push_back(src.independent().value(i));
}
CDM_BIND::FunctionData* PBProperty::Unload(const SEFunction& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::FunctionData* dst = new CDM_BIND::FunctionData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEFunction& src, CDM_BIND::FunctionData& dst)
{
  for (size_t i = 0; i < src.m_Dependent.size(); i++)
  {
    dst.mutable_dependent()->add_value(src.m_Dependent[i]);
    dst.mutable_independent()->add_value(src.m_Independent[i]);
  }
}

void PBProperty::Load(const CDM_BIND::FunctionElectricPotentialVsTimeData& src, SEFunctionElectricPotentialVsTime& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::FunctionElectricPotentialVsTimeData& src, SEFunctionElectricPotentialVsTime& dst)
{
  PBProperty::Serialize(src.functionelectricpotentialvstime(), dst);
  dst.m_TimeUnit = &TimeUnit::GetCompoundUnit(src.functionelectricpotentialvstime().independentunit());
  dst.m_ElectricPotentialUnit = &ElectricPotentialUnit::GetCompoundUnit(src.functionelectricpotentialvstime().dependentunit());
}

CDM_BIND::FunctionElectricPotentialVsTimeData* PBProperty::Unload(const SEFunctionElectricPotentialVsTime& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::FunctionElectricPotentialVsTimeData* dst = new CDM_BIND::FunctionElectricPotentialVsTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEFunctionElectricPotentialVsTime& src, CDM_BIND::FunctionElectricPotentialVsTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_functionelectricpotentialvstime());
  dst.mutable_functionelectricpotentialvstime()->set_independentunit(src.m_TimeUnit->GetString());
  dst.mutable_functionelectricpotentialvstime()->set_dependentunit(src.m_ElectricPotentialUnit->GetString());
}

void PBProperty::Load(const CDM_BIND::FunctionVolumeVsTimeData& src, SEFunctionVolumeVsTime& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::FunctionVolumeVsTimeData& src, SEFunctionVolumeVsTime& dst)
{
  PBProperty::Serialize(src.functionvolumevstime(), dst);
  dst.m_TimeUnit = &TimeUnit::GetCompoundUnit(src.functionvolumevstime().independentunit());
  dst.m_VolumeUnit = &VolumeUnit::GetCompoundUnit(src.functionvolumevstime().dependentunit());
}
CDM_BIND::FunctionVolumeVsTimeData* PBProperty::Unload(const SEFunctionVolumeVsTime& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::FunctionVolumeVsTimeData* dst = new CDM_BIND::FunctionVolumeVsTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEFunctionVolumeVsTime& src, CDM_BIND::FunctionVolumeVsTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_functionvolumevstime());
  dst.mutable_functionvolumevstime()->set_independentunit(src.m_TimeUnit->GetString());
  dst.mutable_functionvolumevstime()->set_dependentunit(src.m_VolumeUnit->GetString());
}

////////////////
// Histograms //
////////////////


void PBProperty::Load(const CDM_BIND::HistogramData& src, SEHistogram& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);

  if (!src.histogram().dependentunit().empty())
  {
    if (src.histogram().dependentunit() != "unitless")
      throw CommonDataModelException("CDM_BIND::Histogram API is intended to be unitless, You are trying to load a dependent axis with a unit defined");
  }
  if (!src.histogram().independentunit().empty())
  {
    if (src.histogram().independentunit() != "unitless")
      throw CommonDataModelException("CDM_BIND::Histogram API is intended to be unitless, You are trying to load an independent axis with a unit defined");
  }
}
void PBProperty::Serialize(const CDM_BIND::HistogramData& src, SEHistogram& dst)
{
  for (int i = 0; i < src.histogram().dependent().value_size(); i++)
    dst.m_Dependent.push_back(src.histogram().dependent().value(i));
  for (int i = 0; i < src.histogram().independent().value_size(); i++)
    dst.m_Independent.push_back(src.histogram().independent().value(i));
}

CDM_BIND::HistogramData* PBProperty::Unload(const SEHistogram& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::HistogramData* dst = new CDM_BIND::HistogramData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEHistogram& src, CDM_BIND::HistogramData& dst)
{
  for (size_t i = 0; i < src.m_Dependent.size(); i++)
  {
    dst.mutable_histogram()->mutable_dependent()->add_value(src.m_Dependent[i]);
    dst.mutable_histogram()->mutable_independent()->add_value(src.m_Independent[i]);
  }
}

void PBProperty::Load(const CDM_BIND::HistogramFractionVsLengthData& src, SEHistogramFractionVsLength& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::HistogramFractionVsLengthData& src, SEHistogramFractionVsLength& dst)
{
  for (int i = 0; i < src.histogramfractionvslength().histogram().dependent().value_size(); i++)
    dst.m_Dependent.push_back(src.histogramfractionvslength().histogram().dependent().value(i));
  for (int i = 0; i < src.histogramfractionvslength().histogram().independent().value_size(); i++)
    dst.m_Independent.push_back(src.histogramfractionvslength().histogram().independent().value(i));
}
CDM_BIND::HistogramFractionVsLengthData* PBProperty::Unload(const SEHistogramFractionVsLength& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::HistogramFractionVsLengthData* dst = new CDM_BIND::HistogramFractionVsLengthData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEHistogramFractionVsLength& src, CDM_BIND::HistogramFractionVsLengthData& dst)
{
  for (size_t i = 0; i < src.m_Dependent.size(); i++)
    dst.mutable_histogramfractionvslength()->mutable_histogram()->mutable_dependent()->add_value(src.m_Dependent[i]);
  for (size_t i = 0; i < src.m_Independent.size(); i++)
    dst.mutable_histogramfractionvslength()->mutable_histogram()->mutable_independent()->add_value(src.m_Independent[i]);
}

void PBProperty::Load(const CDM_BIND::RunningAverageData& src, SERunningAverage& dst)
{
  dst.Clear();
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const CDM_BIND::RunningAverageData& src, SERunningAverage& dst)
{
  dst.m_Sum = src.sum();
  dst.m_NumSamples = src.numsamples();
}
CDM_BIND::RunningAverageData* PBProperty::Unload(const SERunningAverage& src)
{
  CDM_BIND::RunningAverageData* dst = new CDM_BIND::RunningAverageData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SERunningAverage& src, CDM_BIND::RunningAverageData& dst)
{
  dst.set_sum(src.m_Sum);
  dst.set_numsamples(src.m_NumSamples);
}