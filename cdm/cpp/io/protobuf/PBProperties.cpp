/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBProperties.h"
#include "bind/cdm/Properties.pb.h"

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
#include "properties/SEScalarFlowCompliance.h"
#include "properties/SEScalarFlowElastance.h"
#include "properties/SEScalarFlowInertance.h"
#include "properties/SEScalarFlowResistance.h"
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

void PBProperty::Load(const cdm::ScalarData& src, SEScalar& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarData& src, SEScalar& dst)
{
  dst.Clear();
  dst.SetValue(src.value());
  if (!src.unit().empty())
  {
    if (src.unit() != "unitless")
      throw CommonDataModelException("CDM::Scalar API is intended to be unitless, You are trying to load a ScalarData with a unit defined");
  }
  dst.m_readOnly = src.readonly();
}
cdm::ScalarData* PBProperty::Unload(const SEScalar& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarData* dst = new cdm::ScalarData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalar& src, cdm::ScalarData& dst)
{
  dst.set_value(src.m_value);
  dst.set_readonly(src.m_readOnly);
}
void PBProperty::Serialize(const cdm::ScalarData& src, SEUnitScalar& dst)
{
  dst.Clear();
  if (!src.unit().empty())
    dst.SetValue(src.value(), *dst.GetCompoundUnit(src.unit()));
  else
    throw CommonDataModelException("ScalarQuantity attempted to load a ScalarData with no unit, must have a unit.");
  dst.m_readOnly = src.readonly();
}
void PBProperty::Serialize(const SEUnitScalar& src, cdm::ScalarData& dst)
{
  dst.set_value(src.m_value);
  if (src.HasUnit())
    dst.set_unit(src.GetUnit()->GetString());
  else
    throw CommonDataModelException("ScalarQuantity attempted to unload a ScalarData with no unit, must have a unit.");
  dst.set_readonly(src.m_readOnly);
}


void PBProperty::Load(const cdm::Scalar0To1Data& src, SEScalar0To1& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::Scalar0To1Data& src, SEScalar0To1& dst)
{
  PBProperty::Serialize(src.scalar0to1(), dst);
}
cdm::Scalar0To1Data* PBProperty::Unload(const SEScalar0To1& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::Scalar0To1Data* dst = new cdm::Scalar0To1Data();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalar0To1& src, cdm::Scalar0To1Data& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalar0to1());
}

void PBProperty::Load(const cdm::ScalarAmountData& src, SEScalarAmount& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarAmountData& src, SEScalarAmount& dst)
{
  PBProperty::Serialize(src.scalaramount(), dst);
}
cdm::ScalarAmountData* PBProperty::Unload(const SEScalarAmount& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarAmountData* dst = new cdm::ScalarAmountData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAmount& src, cdm::ScalarAmountData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalaramount());
}


void PBProperty::Load(const cdm::ScalarAmountPerMassData& src, SEScalarAmountPerMass& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarAmountPerMassData& src, SEScalarAmountPerMass& dst)
{
  PBProperty::Serialize(src.scalaramountpermass(), dst);
}
cdm::ScalarAmountPerMassData* PBProperty::Unload(const SEScalarAmountPerMass& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarAmountPerMassData* dst = new cdm::ScalarAmountPerMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAmountPerMass& src, cdm::ScalarAmountPerMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalaramountpermass());
}

void PBProperty::Load(const cdm::ScalarAmountPerTimeData& src, SEScalarAmountPerTime& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarAmountPerTimeData& src, SEScalarAmountPerTime& dst)
{
  PBProperty::Serialize(src.scalaramountpertime(), dst);
}
cdm::ScalarAmountPerTimeData* PBProperty::Unload(const SEScalarAmountPerTime& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarAmountPerTimeData* dst = new cdm::ScalarAmountPerTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAmountPerTime& src, cdm::ScalarAmountPerTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalaramountpertime());
}

void PBProperty::Load(const cdm::ScalarAmountPerVolumeData& src, SEScalarAmountPerVolume& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarAmountPerVolumeData& src, SEScalarAmountPerVolume& dst)
{
  PBProperty::Serialize(src.scalaramountpervolume(), dst);
}
cdm::ScalarAmountPerVolumeData* PBProperty::Unload(const SEScalarAmountPerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarAmountPerVolumeData* dst = new cdm::ScalarAmountPerVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAmountPerVolume& src, cdm::ScalarAmountPerVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalaramountpervolume());
}

void PBProperty::Load(const cdm::ScalarAreaData& src, SEScalarArea& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarAreaData& src, SEScalarArea& dst)
{
  PBProperty::Serialize(src.scalararea(), dst);
}
cdm::ScalarAreaData* PBProperty::Unload(const SEScalarArea& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarAreaData* dst = new cdm::ScalarAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarArea& src, cdm::ScalarAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalararea());
}

void PBProperty::Load(const cdm::ScalarAreaPerTimePressureData& src, SEScalarAreaPerTimePressure& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarAreaPerTimePressureData& src, SEScalarAreaPerTimePressure& dst)
{
  PBProperty::Serialize(src.scalarareapertimepressure(), dst);
}
cdm::ScalarAreaPerTimePressureData* PBProperty::Unload(const SEScalarAreaPerTimePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarAreaPerTimePressureData* dst = new cdm::ScalarAreaPerTimePressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarAreaPerTimePressure& src, cdm::ScalarAreaPerTimePressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarareapertimepressure());
}

void PBProperty::Load(const cdm::ScalarElectricCapacitanceData& src, SEScalarElectricCapacitance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarElectricCapacitanceData& src, SEScalarElectricCapacitance& dst)
{
  PBProperty::Serialize(src.scalarelectriccapacitance(), dst);
}
cdm::ScalarElectricCapacitanceData* PBProperty::Unload(const SEScalarElectricCapacitance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricCapacitanceData* dst = new cdm::ScalarElectricCapacitanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricCapacitance& src, cdm::ScalarElectricCapacitanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectriccapacitance());
}

void PBProperty::Load(const cdm::ScalarElectricChargeData& src, SEScalarElectricCharge& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarElectricChargeData& src, SEScalarElectricCharge& dst)
{
  PBProperty::Serialize(src.scalarelectriccharge(), dst);
}
cdm::ScalarElectricChargeData* PBProperty::Unload(const SEScalarElectricCharge& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricChargeData* dst = new cdm::ScalarElectricChargeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricCharge& src, cdm::ScalarElectricChargeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectriccharge());
}

void PBProperty::Load(const cdm::ScalarElectricCurrentData& src, SEScalarElectricCurrent& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarElectricCurrentData& src, SEScalarElectricCurrent& dst)
{
  PBProperty::Serialize(src.scalarelectriccurrent(), dst);
}
cdm::ScalarElectricCurrentData* PBProperty::Unload(const SEScalarElectricCurrent& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricCurrentData* dst = new cdm::ScalarElectricCurrentData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricCurrent& src, cdm::ScalarElectricCurrentData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectriccurrent());
}

void PBProperty::Load(const cdm::ScalarElectricInductanceData& src, SEScalarElectricInductance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarElectricInductanceData& src, SEScalarElectricInductance& dst)
{
  PBProperty::Serialize(src.scalarelectricinductance(), dst);
}
cdm::ScalarElectricInductanceData* PBProperty::Unload(const SEScalarElectricInductance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricInductanceData* dst = new cdm::ScalarElectricInductanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricInductance& src, cdm::ScalarElectricInductanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectricinductance());
}

void PBProperty::Load(const cdm::ScalarElectricPotentialData& src, SEScalarElectricPotential& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarElectricPotentialData& src, SEScalarElectricPotential& dst)
{
  PBProperty::Serialize(src.scalarelectricpotential(), dst);
}
cdm::ScalarElectricPotentialData* PBProperty::Unload(const SEScalarElectricPotential& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricPotentialData* dst = new cdm::ScalarElectricPotentialData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricPotential& src, cdm::ScalarElectricPotentialData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectricpotential());
}

void PBProperty::Load(const cdm::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst)
{
  PBProperty::Serialize(src.scalarelectricresistance(), dst);
}
cdm::ScalarElectricResistanceData* PBProperty::Unload(const SEScalarElectricResistance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricResistanceData* dst = new cdm::ScalarElectricResistanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarElectricResistance& src, cdm::ScalarElectricResistanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarelectricresistance());
}

void PBProperty::Load(const cdm::ScalarEnergyData& src, SEScalarEnergy& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarEnergyData& src, SEScalarEnergy& dst)
{
  PBProperty::Serialize(src.scalarenergy(), dst);
}
cdm::ScalarEnergyData* PBProperty::Unload(const SEScalarEnergy& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarEnergyData* dst = new cdm::ScalarEnergyData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarEnergy& src, cdm::ScalarEnergyData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarenergy());
}

void PBProperty::Load(const cdm::ScalarEnergyPerAmountData& src, SEScalarEnergyPerAmount& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarEnergyPerAmountData& src, SEScalarEnergyPerAmount& dst)
{
  PBProperty::Serialize(src.scalarenergyperamount(), dst);
}
cdm::ScalarEnergyPerAmountData* PBProperty::Unload(const SEScalarEnergyPerAmount& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarEnergyPerAmountData* dst = new cdm::ScalarEnergyPerAmountData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarEnergyPerAmount& src, cdm::ScalarEnergyPerAmountData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarenergyperamount());
}

void PBProperty::Load(const cdm::ScalarEnergyPerMassData& src, SEScalarEnergyPerMass& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarEnergyPerMassData& src, SEScalarEnergyPerMass& dst)
{
  PBProperty::Serialize(src.scalarenergypermass(), dst);
}
cdm::ScalarEnergyPerMassData* PBProperty::Unload(const SEScalarEnergyPerMass& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarEnergyPerMassData* dst = new cdm::ScalarEnergyPerMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarEnergyPerMass& src, cdm::ScalarEnergyPerMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarenergypermass());
}

void PBProperty::Load(const cdm::ScalarFlowComplianceData& src, SEScalarFlowCompliance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarFlowComplianceData& src, SEScalarFlowCompliance& dst)
{
  PBProperty::Serialize(src.scalarflowcompliance(), dst);
}
cdm::ScalarFlowComplianceData* PBProperty::Unload(const SEScalarFlowCompliance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarFlowComplianceData* dst = new cdm::ScalarFlowComplianceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarFlowCompliance& src, cdm::ScalarFlowComplianceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarflowcompliance());
}

void PBProperty::Load(const cdm::ScalarFlowElastanceData& src, SEScalarFlowElastance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarFlowElastanceData& src, SEScalarFlowElastance& dst)
{
  PBProperty::Serialize(src.scalarflowelastance(), dst);
}
cdm::ScalarFlowElastanceData* PBProperty::Unload(const SEScalarFlowElastance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarFlowElastanceData* dst = new cdm::ScalarFlowElastanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarFlowElastance& src, cdm::ScalarFlowElastanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarflowelastance());
}

void PBProperty::Load(const cdm::ScalarFlowInertanceData& src, SEScalarFlowInertance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarFlowInertanceData& src, SEScalarFlowInertance& dst)
{
  PBProperty::Serialize(src.scalarflowinertance(), dst);
}
cdm::ScalarFlowInertanceData* PBProperty::Unload(const SEScalarFlowInertance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarFlowInertanceData* dst = new cdm::ScalarFlowInertanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarFlowInertance& src, cdm::ScalarFlowInertanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarflowinertance());
}

void PBProperty::Load(const cdm::ScalarFlowResistanceData& src, SEScalarFlowResistance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarFlowResistanceData& src, SEScalarFlowResistance& dst)
{
  PBProperty::Serialize(src.scalarflowresistance(), dst);
}
cdm::ScalarFlowResistanceData* PBProperty::Unload(const SEScalarFlowResistance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarFlowResistanceData* dst = new cdm::ScalarFlowResistanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarFlowResistance& src, cdm::ScalarFlowResistanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarflowresistance());
}

void PBProperty::Load(const cdm::ScalarForceData& src, SEScalarForce& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarForceData& src, SEScalarForce& dst)
{
  PBProperty::Serialize(src.scalarforce(), dst);
}
cdm::ScalarForceData* PBProperty::Unload(const SEScalarForce& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarForceData* dst = new cdm::ScalarForceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarForce& src, cdm::ScalarForceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarforce());
}

void PBProperty::Load(const cdm::ScalarFrequencyData& src, SEScalarFrequency& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarFrequencyData& src, SEScalarFrequency& dst)
{
  PBProperty::Serialize(src.scalarfrequency(), dst);
}
cdm::ScalarFrequencyData* PBProperty::Unload(const SEScalarFrequency& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarFrequencyData* dst = new cdm::ScalarFrequencyData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarFrequency& src, cdm::ScalarFrequencyData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarfrequency());
}

void PBProperty::Load(const cdm::ScalarHeatCapacitanceData& src, SEScalarHeatCapacitance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarHeatCapacitanceData& src, SEScalarHeatCapacitance& dst)
{
  PBProperty::Serialize(src.scalarheatcapacitance(), dst);
}
cdm::ScalarHeatCapacitanceData* PBProperty::Unload(const SEScalarHeatCapacitance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatCapacitanceData* dst = new cdm::ScalarHeatCapacitanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatCapacitance& src, cdm::ScalarHeatCapacitanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatcapacitance());
}

void PBProperty::Load(const cdm::ScalarHeatCapacitancePerAmountData& src, SEScalarHeatCapacitancePerAmount& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarHeatCapacitancePerAmountData& src, SEScalarHeatCapacitancePerAmount& dst)
{
  PBProperty::Serialize(src.scalarheatcapacitanceperamount(), dst);
}
cdm::ScalarHeatCapacitancePerAmountData* PBProperty::Unload(const SEScalarHeatCapacitancePerAmount& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatCapacitancePerAmountData* dst = new cdm::ScalarHeatCapacitancePerAmountData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatCapacitancePerAmount& src, cdm::ScalarHeatCapacitancePerAmountData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatcapacitanceperamount());
}

void PBProperty::Load(const cdm::ScalarHeatCapacitancePerMassData& src, SEScalarHeatCapacitancePerMass& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarHeatCapacitancePerMassData& src, SEScalarHeatCapacitancePerMass& dst)
{
  PBProperty::Serialize(src.scalarheatcapacitancepermass(), dst);
}
cdm::ScalarHeatCapacitancePerMassData* PBProperty::Unload(const SEScalarHeatCapacitancePerMass& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatCapacitancePerMassData* dst = new cdm::ScalarHeatCapacitancePerMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatCapacitancePerMass& src, cdm::ScalarHeatCapacitancePerMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatcapacitancepermass());
}

void PBProperty::Load(const cdm::ScalarHeatConductanceData& src, SEScalarHeatConductance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarHeatConductanceData& src, SEScalarHeatConductance& dst)
{
  PBProperty::Serialize(src.scalarheatconductance(), dst);
}
cdm::ScalarHeatConductanceData* PBProperty::Unload(const SEScalarHeatConductance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatConductanceData* dst = new cdm::ScalarHeatConductanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatConductance& src, cdm::ScalarHeatConductanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatconductance());
}

void PBProperty::Load(const cdm::ScalarHeatConductancePerAreaData& src, SEScalarHeatConductancePerArea& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarHeatConductancePerAreaData& src, SEScalarHeatConductancePerArea& dst)
{
  PBProperty::Serialize(src.scalarheatconductanceperarea(), dst);
}
cdm::ScalarHeatConductancePerAreaData* PBProperty::Unload(const SEScalarHeatConductancePerArea& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatConductancePerAreaData* dst = new cdm::ScalarHeatConductancePerAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatConductancePerArea& src, cdm::ScalarHeatConductancePerAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatconductanceperarea());
}

void PBProperty::Load(const cdm::ScalarHeatInductanceData& src, SEScalarHeatInductance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarHeatInductanceData& src, SEScalarHeatInductance& dst)
{
  PBProperty::Serialize(src.scalarheatinductance(), dst);
}
cdm::ScalarHeatInductanceData* PBProperty::Unload(const SEScalarHeatInductance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatInductanceData* dst = new cdm::ScalarHeatInductanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatInductance& src, cdm::ScalarHeatInductanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatinductance());
}

void PBProperty::Load(const cdm::ScalarHeatResistanceData& src, SEScalarHeatResistance& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarHeatResistanceData& src, SEScalarHeatResistance& dst)
{
  PBProperty::Serialize(src.scalarheatresistance(), dst);
}
cdm::ScalarHeatResistanceData* PBProperty::Unload(const SEScalarHeatResistance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatResistanceData* dst = new cdm::ScalarHeatResistanceData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatResistance& src, cdm::ScalarHeatResistanceData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatresistance());
}

void PBProperty::Load(const cdm::ScalarHeatResistanceAreaData& src, SEScalarHeatResistanceArea& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarHeatResistanceAreaData& src, SEScalarHeatResistanceArea& dst)
{
  PBProperty::Serialize(src.scalarheatresistancearea(), dst);
}
cdm::ScalarHeatResistanceAreaData* PBProperty::Unload(const SEScalarHeatResistanceArea& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatResistanceAreaData* dst = new cdm::ScalarHeatResistanceAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarHeatResistanceArea& src, cdm::ScalarHeatResistanceAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarheatresistancearea());
}

void PBProperty::Load(const cdm::ScalarInversePressureData& src, SEScalarInversePressure& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarInversePressureData& src, SEScalarInversePressure& dst)
{
  PBProperty::Serialize(src.scalarinversepressure(), dst);
}
cdm::ScalarInversePressureData* PBProperty::Unload(const SEScalarInversePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarInversePressureData* dst = new cdm::ScalarInversePressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarInversePressure& src, cdm::ScalarInversePressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarinversepressure());
}

void PBProperty::Load(const cdm::ScalarInverseVolumeData& src, SEScalarInverseVolume& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarInverseVolumeData& src, SEScalarInverseVolume& dst)
{
  PBProperty::Serialize(src.scalarinversevolume(), dst);
}
cdm::ScalarInverseVolumeData* PBProperty::Unload(const SEScalarInverseVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarInverseVolumeData* dst = new cdm::ScalarInverseVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarInverseVolume& src, cdm::ScalarInverseVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarinversevolume());
}

void PBProperty::Load(const cdm::ScalarLengthData& src, SEScalarLength& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarLengthData& src, SEScalarLength& dst)
{
  PBProperty::Serialize(src.scalarlength(), dst);
}
cdm::ScalarLengthData* PBProperty::Unload(const SEScalarLength& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarLengthData* dst = new cdm::ScalarLengthData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarLength& src, cdm::ScalarLengthData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarlength());
}

void PBProperty::Load(const cdm::ScalarLengthPerTimeData& src, SEScalarLengthPerTime& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarLengthPerTimeData& src, SEScalarLengthPerTime& dst)
{
  PBProperty::Serialize(src.scalarlengthpertime(), dst);
}
cdm::ScalarLengthPerTimeData* PBProperty::Unload(const SEScalarLengthPerTime& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarLengthPerTimeData* dst = new cdm::ScalarLengthPerTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarLengthPerTime& src, cdm::ScalarLengthPerTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarlengthpertime());
}

void PBProperty::Load(const cdm::ScalarLengthPerTimePressureData& src, SEScalarLengthPerTimePressure& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarLengthPerTimePressureData& src, SEScalarLengthPerTimePressure& dst)
{
  PBProperty::Serialize(src.scalarlengthpertimepressure(), dst);
}
cdm::ScalarLengthPerTimePressureData* PBProperty::Unload(const SEScalarLengthPerTimePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarLengthPerTimePressureData* dst = new cdm::ScalarLengthPerTimePressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarLengthPerTimePressure& src, cdm::ScalarLengthPerTimePressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarlengthpertimepressure());
}

void PBProperty::Load(const cdm::ScalarMassData& src, SEScalarMass& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarMassData& src, SEScalarMass& dst)
{
  PBProperty::Serialize(src.scalarmass(), dst);
}
cdm::ScalarMassData* PBProperty::Unload(const SEScalarMass& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarMassData* dst = new cdm::ScalarMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMass& src, cdm::ScalarMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmass());
}

void PBProperty::Load(const cdm::ScalarMassPerAmountData& src, SEScalarMassPerAmount& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarMassPerAmountData& src, SEScalarMassPerAmount& dst)
{
  PBProperty::Serialize(src.scalarmassperamount(), dst);
}
cdm::ScalarMassPerAmountData* PBProperty::Unload(const SEScalarMassPerAmount& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarMassPerAmountData* dst = new cdm::ScalarMassPerAmountData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerAmount& src, cdm::ScalarMassPerAmountData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmassperamount());
}

void PBProperty::Load(const cdm::ScalarMassPerAreaTimeData& src, SEScalarMassPerAreaTime& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarMassPerAreaTimeData& src, SEScalarMassPerAreaTime& dst)
{
  PBProperty::Serialize(src.scalarmassperareatime(), dst);
}
cdm::ScalarMassPerAreaTimeData* PBProperty::Unload(const SEScalarMassPerAreaTime& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarMassPerAreaTimeData* dst = new cdm::ScalarMassPerAreaTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerAreaTime& src, cdm::ScalarMassPerAreaTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmassperareatime());
}

void PBProperty::Load(const cdm::ScalarMassPerMassData& src, SEScalarMassPerMass& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarMassPerMassData& src, SEScalarMassPerMass& dst)
{
  PBProperty::Serialize(src.scalarmasspermass(), dst);
}
cdm::ScalarMassPerMassData* PBProperty::Unload(const SEScalarMassPerMass& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarMassPerMassData* dst = new cdm::ScalarMassPerMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerMass& src, cdm::ScalarMassPerMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmasspermass());
}

void PBProperty::Load(const cdm::ScalarMassPerTimeData& src, SEScalarMassPerTime& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarMassPerTimeData& src, SEScalarMassPerTime& dst)
{
  PBProperty::Serialize(src.scalarmasspertime(), dst);
}
cdm::ScalarMassPerTimeData* PBProperty::Unload(const SEScalarMassPerTime& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarMassPerTimeData* dst = new cdm::ScalarMassPerTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerTime& src, cdm::ScalarMassPerTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmasspertime());
}

void PBProperty::Load(const cdm::ScalarMassPerVolumeData& src, SEScalarMassPerVolume& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarMassPerVolumeData& src, SEScalarMassPerVolume& dst)
{
  PBProperty::Serialize(src.scalarmasspervolume(), dst);
}
cdm::ScalarMassPerVolumeData* PBProperty::Unload(const SEScalarMassPerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarMassPerVolumeData* dst = new cdm::ScalarMassPerVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarMassPerVolume& src, cdm::ScalarMassPerVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarmasspervolume());
}

void PBProperty::Load(const cdm::ScalarNegative1To1Data& src, SEScalarNegative1To1& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarNegative1To1Data& src, SEScalarNegative1To1& dst)
{
  PBProperty::Serialize(src.scalarnegative1to1(), dst);
}
cdm::ScalarNegative1To1Data* PBProperty::Unload(const SEScalarNegative1To1& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarNegative1To1Data* dst = new cdm::ScalarNegative1To1Data();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarNegative1To1& src, cdm::ScalarNegative1To1Data& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarnegative1to1());
}

void PBProperty::Load(const cdm::ScalarOsmolalityData& src, SEScalarOsmolality& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarOsmolalityData& src, SEScalarOsmolality& dst)
{
  PBProperty::Serialize(src.scalarosmolality(), dst);
}
cdm::ScalarOsmolalityData* PBProperty::Unload(const SEScalarOsmolality& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarOsmolalityData* dst = new cdm::ScalarOsmolalityData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarOsmolality& src, cdm::ScalarOsmolalityData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarosmolality());
}

void PBProperty::Load(const cdm::ScalarOsmolarityData& src, SEScalarOsmolarity& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarOsmolarityData& src, SEScalarOsmolarity& dst)
{
  PBProperty::Serialize(src.scalarosmolarity(), dst);
}
cdm::ScalarOsmolarityData* PBProperty::Unload(const SEScalarOsmolarity& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarOsmolarityData* dst = new cdm::ScalarOsmolarityData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarOsmolarity& src, cdm::ScalarOsmolarityData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarosmolarity());
}

void PBProperty::Load(const cdm::ScalarPowerData& src, SEScalarPower& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarPowerData& src, SEScalarPower& dst)
{
  PBProperty::Serialize(src.scalarpower(), dst);
}
cdm::ScalarPowerData* PBProperty::Unload(const SEScalarPower& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarPowerData* dst = new cdm::ScalarPowerData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPower& src, cdm::ScalarPowerData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpower());
}

void PBProperty::Load(const cdm::ScalarPowerPerAreaTemperatureToTheFourthData& src, SEScalarPowerPerAreaTemperatureToTheFourth& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarPowerPerAreaTemperatureToTheFourthData& src, SEScalarPowerPerAreaTemperatureToTheFourth& dst)
{
  PBProperty::Serialize(src.scalarpowerperareatemperaturetothefourth(), dst);
}
cdm::ScalarPowerPerAreaTemperatureToTheFourthData* PBProperty::Unload(const SEScalarPowerPerAreaTemperatureToTheFourth& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarPowerPerAreaTemperatureToTheFourthData* dst = new cdm::ScalarPowerPerAreaTemperatureToTheFourthData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPowerPerAreaTemperatureToTheFourth& src, cdm::ScalarPowerPerAreaTemperatureToTheFourthData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpowerperareatemperaturetothefourth());
}

void PBProperty::Load(const cdm::ScalarPressureData& src, SEScalarPressure& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarPressureData& src, SEScalarPressure& dst)
{
  PBProperty::Serialize(src.scalarpressure(), dst);
}
cdm::ScalarPressureData* PBProperty::Unload(const SEScalarPressure& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarPressureData* dst = new cdm::ScalarPressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressure& src, cdm::ScalarPressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressure());
}

void PBProperty::Load(const cdm::ScalarPressurePerVolumeData& src, SEScalarPressurePerVolume& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarPressurePerVolumeData& src, SEScalarPressurePerVolume& dst)
{
  PBProperty::Serialize(src.scalarpressurepervolume(), dst);
}
cdm::ScalarPressurePerVolumeData* PBProperty::Unload(const SEScalarPressurePerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarPressurePerVolumeData* dst = new cdm::ScalarPressurePerVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressurePerVolume& src, cdm::ScalarPressurePerVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressurepervolume());
}

void PBProperty::Load(const cdm::ScalarPressureTimePerAreaData& src, SEScalarPressureTimePerArea& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarPressureTimePerAreaData& src, SEScalarPressureTimePerArea& dst)
{
  PBProperty::Serialize(src.scalarpressuretimeperarea(), dst);
}
cdm::ScalarPressureTimePerAreaData* PBProperty::Unload(const SEScalarPressureTimePerArea& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarPressureTimePerAreaData* dst = new cdm::ScalarPressureTimePerAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressureTimePerArea& src, cdm::ScalarPressureTimePerAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressuretimeperarea());
}

void PBProperty::Load(const cdm::ScalarPressureTimePerVolumeAreaData& src, SEScalarPressureTimePerVolumeArea& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarPressureTimePerVolumeAreaData& src, SEScalarPressureTimePerVolumeArea& dst)
{
  PBProperty::Serialize(src.scalarpressuretimepervolumearea(), dst);
}
cdm::ScalarPressureTimePerVolumeAreaData* PBProperty::Unload(const SEScalarPressureTimePerVolumeArea& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarPressureTimePerVolumeAreaData* dst = new cdm::ScalarPressureTimePerVolumeAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarPressureTimePerVolumeArea& src, cdm::ScalarPressureTimePerVolumeAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarpressuretimepervolumearea());
}

void PBProperty::Load(const cdm::ScalarTemperatureData& src, SEScalarTemperature& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarTemperatureData& src, SEScalarTemperature& dst)
{
  PBProperty::Serialize(src.scalartemperature(), dst);
}
cdm::ScalarTemperatureData* PBProperty::Unload(const SEScalarTemperature& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarTemperatureData* dst = new cdm::ScalarTemperatureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarTemperature& src, cdm::ScalarTemperatureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalartemperature());
}

void PBProperty::Load(const cdm::ScalarTimeData& src, SEScalarTime& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarTimeData& src, SEScalarTime& dst)
{
  PBProperty::Serialize(src.scalartime(), dst);
}
cdm::ScalarTimeData* PBProperty::Unload(const SEScalarTime& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarTimeData* dst = new cdm::ScalarTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarTime& src, cdm::ScalarTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalartime());
}

void PBProperty::Load(const cdm::ScalarVolumeData& src, SEScalarVolume& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarVolumeData& src, SEScalarVolume& dst)
{
  PBProperty::Serialize(src.scalarvolume(), dst);
}
cdm::ScalarVolumeData* PBProperty::Unload(const SEScalarVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarVolumeData* dst = new cdm::ScalarVolumeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolume& src, cdm::ScalarVolumeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolume());
}

void PBProperty::Load(const cdm::ScalarVolumePerPressureData& src, SEScalarVolumePerPressure& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarVolumePerPressureData& src, SEScalarVolumePerPressure& dst)
{
  PBProperty::Serialize(src.scalarvolumeperpressure(), dst);
}
cdm::ScalarVolumePerPressureData* PBProperty::Unload(const SEScalarVolumePerPressure& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarVolumePerPressureData* dst = new cdm::ScalarVolumePerPressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerPressure& src, cdm::ScalarVolumePerPressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumeperpressure());
}

void PBProperty::Load(const cdm::ScalarVolumePerTimeData& src, SEScalarVolumePerTime& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarVolumePerTimeData& src, SEScalarVolumePerTime& dst)
{
  PBProperty::Serialize(src.scalarvolumepertime(), dst);
}
cdm::ScalarVolumePerTimeData* PBProperty::Unload(const SEScalarVolumePerTime& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarVolumePerTimeData* dst = new cdm::ScalarVolumePerTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTime& src, cdm::ScalarVolumePerTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertime());
}

void PBProperty::Load(const cdm::ScalarVolumePerTimeAreaData& src, SEScalarVolumePerTimeArea& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarVolumePerTimeAreaData& src, SEScalarVolumePerTimeArea& dst)
{
  PBProperty::Serialize(src.scalarvolumepertimearea(), dst);
}
cdm::ScalarVolumePerTimeAreaData* PBProperty::Unload(const SEScalarVolumePerTimeArea& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarVolumePerTimeAreaData* dst = new cdm::ScalarVolumePerTimeAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTimeArea& src, cdm::ScalarVolumePerTimeAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertimearea());
}

void PBProperty::Load(const cdm::ScalarVolumePerTimeMassData& src, SEScalarVolumePerTimeMass& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarVolumePerTimeMassData& src, SEScalarVolumePerTimeMass& dst)
{
  PBProperty::Serialize(src.scalarvolumepertimemass(), dst);
}
cdm::ScalarVolumePerTimeMassData* PBProperty::Unload(const SEScalarVolumePerTimeMass& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarVolumePerTimeMassData* dst = new cdm::ScalarVolumePerTimeMassData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTimeMass& src, cdm::ScalarVolumePerTimeMassData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertimemass());
}

void PBProperty::Load(const cdm::ScalarVolumePerTimePressureData& src, SEScalarVolumePerTimePressure& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarVolumePerTimePressureData& src, SEScalarVolumePerTimePressure& dst)
{
  PBProperty::Serialize(src.scalarvolumepertimepressure(), dst);
}
cdm::ScalarVolumePerTimePressureData* PBProperty::Unload(const SEScalarVolumePerTimePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarVolumePerTimePressureData* dst = new cdm::ScalarVolumePerTimePressureData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTimePressure& src, cdm::ScalarVolumePerTimePressureData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertimepressure());
}

void PBProperty::Load(const cdm::ScalarVolumePerTimePressureAreaData& src, SEScalarVolumePerTimePressureArea& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::ScalarVolumePerTimePressureAreaData& src, SEScalarVolumePerTimePressureArea& dst)
{
  PBProperty::Serialize(src.scalarvolumepertimepressurearea(), dst);
}

cdm::ScalarVolumePerTimePressureAreaData* PBProperty::Unload(const SEScalarVolumePerTimePressureArea& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarVolumePerTimePressureAreaData* dst = new cdm::ScalarVolumePerTimePressureAreaData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEScalarVolumePerTimePressureArea& src, cdm::ScalarVolumePerTimePressureAreaData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_scalarvolumepertimepressurearea());
}
///////////////
// Functions //
///////////////

void PBProperty::Load(const cdm::FunctionData& src, SEFunction& dst)
{
  PBProperty::Serialize(src, dst);

  if (!src.dependentunit().empty())
  {
    if (src.dependentunit() != "unitless")
      throw CommonDataModelException("CDM::Function API is intended to be unitless, You are trying to load a dependent axis with a unit defined");
  }
  if (!src.independentunit().empty())
  {
    if (src.independentunit() != "unitless")
      throw CommonDataModelException("CDM::Function API is intended to be unitless, You are trying to load an independent axis with a unit defined");
  }
}
void PBProperty::Serialize(const cdm::FunctionData& src, SEFunction& dst)
{
  dst.Clear();
  for (int i = 0; i < src.dependent().value_size(); i++)
    dst.m_Dependent.push_back(src.dependent().value(i));
  for (int i = 0; i < src.independent().value_size(); i++)
    dst.m_Independent.push_back(src.independent().value(i));
}
cdm::FunctionData* PBProperty::Unload(const SEFunction& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::FunctionData* dst = new cdm::FunctionData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEFunction& src, cdm::FunctionData& dst)
{
  for (size_t i = 0; i < src.m_Dependent.size(); i++)
  {
    dst.mutable_dependent()->add_value(src.m_Dependent[i]);
    dst.mutable_independent()->add_value(src.m_Independent[i]);
  }
}

void PBProperty::Load(const cdm::FunctionElectricPotentialVsTimeData& src, SEFunctionElectricPotentialVsTime& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::FunctionElectricPotentialVsTimeData& src, SEFunctionElectricPotentialVsTime& dst)
{
  PBProperty::Serialize(src.functionelectricpotentialvstime(), dst);
  dst.m_TimeUnit = &TimeUnit::GetCompoundUnit(src.functionelectricpotentialvstime().independentunit());
  dst.m_ElectricPotentialUnit = &ElectricPotentialUnit::GetCompoundUnit(src.functionelectricpotentialvstime().dependentunit());
}

cdm::FunctionElectricPotentialVsTimeData* PBProperty::Unload(const SEFunctionElectricPotentialVsTime& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::FunctionElectricPotentialVsTimeData* dst = new cdm::FunctionElectricPotentialVsTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEFunctionElectricPotentialVsTime& src, cdm::FunctionElectricPotentialVsTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_functionelectricpotentialvstime());
  dst.mutable_functionelectricpotentialvstime()->set_independentunit(src.m_TimeUnit->GetString());
  dst.mutable_functionelectricpotentialvstime()->set_dependentunit(src.m_ElectricPotentialUnit->GetString());
}

void PBProperty::Load(const cdm::FunctionVolumeVsTimeData& src, SEFunctionVolumeVsTime& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::FunctionVolumeVsTimeData& src, SEFunctionVolumeVsTime& dst)
{
  PBProperty::Serialize(src.functionvolumevstime(), dst);
  dst.m_TimeUnit = &TimeUnit::GetCompoundUnit(src.functionvolumevstime().independentunit());
  dst.m_VolumeUnit = &VolumeUnit::GetCompoundUnit(src.functionvolumevstime().dependentunit());
}
cdm::FunctionVolumeVsTimeData* PBProperty::Unload(const SEFunctionVolumeVsTime& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::FunctionVolumeVsTimeData* dst = new cdm::FunctionVolumeVsTimeData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEFunctionVolumeVsTime& src, cdm::FunctionVolumeVsTimeData& dst)
{
  PBProperty::Serialize(src, *dst.mutable_functionvolumevstime());
  dst.mutable_functionvolumevstime()->set_independentunit(src.m_TimeUnit->GetString());
  dst.mutable_functionvolumevstime()->set_dependentunit(src.m_VolumeUnit->GetString());
}

////////////////
// Histograms //
////////////////


void PBProperty::Load(const cdm::HistogramData& src, SEHistogram& dst)
{
  PBProperty::Serialize(src, dst);

  if (!src.histogram().dependentunit().empty())
  {
    if (src.histogram().dependentunit() != "unitless")
      throw CommonDataModelException("CDM::Histogram API is intended to be unitless, You are trying to load a dependent axis with a unit defined");
  }
  if (!src.histogram().independentunit().empty())
  {
    if (src.histogram().independentunit() != "unitless")
      throw CommonDataModelException("CDM::Histogram API is intended to be unitless, You are trying to load an independent axis with a unit defined");
  }
}
void PBProperty::Serialize(const cdm::HistogramData& src, SEHistogram& dst)
{
  dst.Clear();
  for (int i = 0; i < src.histogram().dependent().value_size(); i++)
    dst.m_Dependent.push_back(src.histogram().dependent().value(i));
  for (int i = 0; i < src.histogram().independent().value_size(); i++)
    dst.m_Independent.push_back(src.histogram().independent().value(i));
}

cdm::HistogramData* PBProperty::Unload(const SEHistogram& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::HistogramData* dst = new cdm::HistogramData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEHistogram& src, cdm::HistogramData& dst)
{
  for (size_t i = 0; i < src.m_Dependent.size(); i++)
  {
    dst.mutable_histogram()->mutable_dependent()->add_value(src.m_Dependent[i]);
    dst.mutable_histogram()->mutable_independent()->add_value(src.m_Independent[i]);
  }
}

void PBProperty::Load(const cdm::HistogramFractionVsLengthData& src, SEHistogramFractionVsLength& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::HistogramFractionVsLengthData& src, SEHistogramFractionVsLength& dst)
{
  dst.Clear();
  for (int i = 0; i < src.histogramfractionvslength().histogram().dependent().value_size(); i++)
    dst.m_Dependent.push_back(src.histogramfractionvslength().histogram().dependent().value(i));
  for (int i = 0; i < src.histogramfractionvslength().histogram().independent().value_size(); i++)
    dst.m_Independent.push_back(src.histogramfractionvslength().histogram().independent().value(i));
}
cdm::HistogramFractionVsLengthData* PBProperty::Unload(const SEHistogramFractionVsLength& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::HistogramFractionVsLengthData* dst = new cdm::HistogramFractionVsLengthData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SEHistogramFractionVsLength& src, cdm::HistogramFractionVsLengthData& dst)
{
  for (size_t i = 0; i < src.m_Dependent.size(); i++)
    dst.mutable_histogramfractionvslength()->mutable_histogram()->mutable_dependent()->add_value(src.m_Dependent[i]);
  for (size_t i = 0; i < src.m_Independent.size(); i++)
    dst.mutable_histogramfractionvslength()->mutable_histogram()->mutable_independent()->add_value(src.m_Independent[i]);
}

void PBProperty::Load(const cdm::RunningAverageData& src, SERunningAverage& dst)
{
  PBProperty::Serialize(src, dst);
}
void PBProperty::Serialize(const cdm::RunningAverageData& src, SERunningAverage& dst)
{
  dst.m_Sum = src.sum();
  dst.m_NumSamples = src.numsamples();
}
cdm::RunningAverageData* PBProperty::Unload(const SERunningAverage& src)
{
  cdm::RunningAverageData* dst = new cdm::RunningAverageData();
  PBProperty::Serialize(src, *dst);
  return dst;
}
void PBProperty::Serialize(const SERunningAverage& src, cdm::RunningAverageData& dst)
{
  dst.set_sum(src.m_Sum);
  dst.set_numsamples(src.m_NumSamples);
}