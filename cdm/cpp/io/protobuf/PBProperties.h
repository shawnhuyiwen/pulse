/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
   // Forward Declaire SE and Protobuf Property Types
CDM_BIND_DECL(ScalarData)
CDM_BIND_DECL(Scalar0To1Data)
CDM_BIND_DECL(ScalarAmountData)
CDM_BIND_DECL(ScalarAmountPerMassData)
CDM_BIND_DECL(ScalarAmountPerTimeData)
CDM_BIND_DECL(ScalarAmountPerVolumeData)
CDM_BIND_DECL(ScalarAreaData)
CDM_BIND_DECL(ScalarAreaPerTimePressureData)
CDM_BIND_DECL(ScalarElectricCapacitanceData)
CDM_BIND_DECL(ScalarElectricChargeData)
CDM_BIND_DECL(ScalarElectricCurrentData)
CDM_BIND_DECL(ScalarElectricInductanceData)
CDM_BIND_DECL(ScalarElectricPotentialData)
CDM_BIND_DECL(ScalarElectricResistanceData)
CDM_BIND_DECL(ScalarEnergyData)
CDM_BIND_DECL(ScalarEnergyPerAmountData)
CDM_BIND_DECL(ScalarEnergyPerMassData)
CDM_BIND_DECL(ScalarFlowComplianceData)
CDM_BIND_DECL(ScalarFlowElastanceData)
CDM_BIND_DECL(ScalarFlowInertanceData)
CDM_BIND_DECL(ScalarFlowResistanceData)
CDM_BIND_DECL(ScalarForceData)
CDM_BIND_DECL(ScalarFrequencyData)
CDM_BIND_DECL(ScalarHeatCapacitanceData)
CDM_BIND_DECL(ScalarHeatCapacitancePerAmountData)
CDM_BIND_DECL(ScalarHeatCapacitancePerMassData)
CDM_BIND_DECL(ScalarHeatConductanceData)
CDM_BIND_DECL(ScalarHeatConductancePerAreaData)
CDM_BIND_DECL(ScalarHeatInductanceData)
CDM_BIND_DECL(ScalarHeatResistanceData)
CDM_BIND_DECL(ScalarHeatResistanceAreaData)
CDM_BIND_DECL(ScalarInversePressureData)
CDM_BIND_DECL(ScalarInverseVolumeData)
CDM_BIND_DECL(ScalarLengthData)
CDM_BIND_DECL(ScalarLengthPerTimeData)
CDM_BIND_DECL(ScalarLengthPerTimePressureData)
CDM_BIND_DECL(ScalarMassData)
CDM_BIND_DECL(ScalarMassPerAmountData)
CDM_BIND_DECL(ScalarMassPerAreaTimeData)
CDM_BIND_DECL(ScalarMassPerMassData)
CDM_BIND_DECL(ScalarMassPerTimeData)
CDM_BIND_DECL(ScalarMassPerVolumeData)
CDM_BIND_DECL(ScalarNegative1To1Data)
CDM_BIND_DECL(ScalarOsmolalityData)
CDM_BIND_DECL(ScalarOsmolarityData)
CDM_BIND_DECL(ScalarPowerData)
CDM_BIND_DECL(ScalarPowerPerAreaTemperatureToTheFourthData)
CDM_BIND_DECL(ScalarPressureData)
CDM_BIND_DECL(ScalarPressurePerVolumeData)
CDM_BIND_DECL(ScalarPressureTimePerAreaData)
CDM_BIND_DECL(ScalarPressureTimePerVolumeAreaData)
CDM_BIND_DECL(ScalarTemperatureData)
CDM_BIND_DECL(ScalarTimeData)
CDM_BIND_DECL(ScalarVolumeData)
CDM_BIND_DECL(ScalarVolumePerPressureData)
CDM_BIND_DECL(ScalarVolumePerTimeData)
CDM_BIND_DECL(ScalarVolumePerTimeAreaData)
CDM_BIND_DECL(ScalarVolumePerTimeMassData)
CDM_BIND_DECL(ScalarVolumePerTimePressureData)
CDM_BIND_DECL(ScalarVolumePerTimePressureAreaData)

CDM_BIND_DECL(FunctionData)
CDM_BIND_DECL(FunctionElectricPotentialVsTimeData)
CDM_BIND_DECL(FunctionVolumeVsTimeData)

CDM_BIND_DECL(HistogramData)
CDM_BIND_DECL(HistogramFractionVsLengthData)

CDM_BIND_DECL2(RunningAverage)

class CDM_DECL PBProperty
{
public:

  // Scalars
  static void Load(const cdm::ScalarData& src, SEScalar& dst);
  static cdm::ScalarData* Unload(const SEScalar& src);
  static void Serialize(const cdm::ScalarData& src, SEScalar& dst);
  static void Serialize(const SEScalar& src, cdm::ScalarData& dst);
  static void Serialize(const cdm::ScalarData& src, SEUnitScalar& dst);
  static void Serialize(const SEUnitScalar& src, cdm::ScalarData& dst);

  static void Load(const cdm::Scalar0To1Data& src, SEScalar0To1& dst);
  static cdm::Scalar0To1Data* Unload(const SEScalar0To1& src);
  static void Serialize(const cdm::Scalar0To1Data& src, SEScalar0To1& dst);
  static void Serialize(const SEScalar0To1& src, cdm::Scalar0To1Data& dst);

  static void Load(const cdm::ScalarAmountData& src, SEScalarAmount& dst);
  static cdm::ScalarAmountData* Unload(const SEScalarAmount& src);
  static void Serialize(const cdm::ScalarAmountData& src, SEScalarAmount& dst);
  static void Serialize(const SEScalarAmount& src, cdm::ScalarAmountData& dst);

  static void Load(const cdm::ScalarAmountPerMassData& src, SEScalarAmountPerMass& dst);
  static cdm::ScalarAmountPerMassData* Unload(const SEScalarAmountPerMass& src);
  static void Serialize(const cdm::ScalarAmountPerMassData& src, SEScalarAmountPerMass& dst);
  static void Serialize(const SEScalarAmountPerMass& src, cdm::ScalarAmountPerMassData& dst);

  static void Load(const cdm::ScalarAmountPerTimeData& src, SEScalarAmountPerTime& dst);
  static cdm::ScalarAmountPerTimeData* Unload(const SEScalarAmountPerTime& src);
  static void Serialize(const cdm::ScalarAmountPerTimeData& src, SEScalarAmountPerTime& dst);
  static void Serialize(const SEScalarAmountPerTime& src, cdm::ScalarAmountPerTimeData& dst);

  static void Load(const cdm::ScalarAmountPerVolumeData& src, SEScalarAmountPerVolume& dst);
  static cdm::ScalarAmountPerVolumeData* Unload(const SEScalarAmountPerVolume& src);
  static void Serialize(const cdm::ScalarAmountPerVolumeData& src, SEScalarAmountPerVolume& dst);
  static void Serialize(const SEScalarAmountPerVolume& src, cdm::ScalarAmountPerVolumeData& dst);

  static void Load(const cdm::ScalarAreaData& src, SEScalarArea& dst);
  static cdm::ScalarAreaData* Unload(const SEScalarArea& src);
  static void Serialize(const cdm::ScalarAreaData& src, SEScalarArea& dst);
  static void Serialize(const SEScalarArea& src, cdm::ScalarAreaData& dst);

  static void Load(const cdm::ScalarAreaPerTimePressureData& src, SEScalarAreaPerTimePressure& dst);
  static cdm::ScalarAreaPerTimePressureData* Unload(const SEScalarAreaPerTimePressure& src);
  static void Serialize(const cdm::ScalarAreaPerTimePressureData& src, SEScalarAreaPerTimePressure& dst);
  static void Serialize(const SEScalarAreaPerTimePressure& src, cdm::ScalarAreaPerTimePressureData& dst);

  static void Load(const cdm::ScalarElectricCapacitanceData& src, SEScalarElectricCapacitance& dst);
  static cdm::ScalarElectricCapacitanceData* Unload(const SEScalarElectricCapacitance& src);
  static void Serialize(const cdm::ScalarElectricCapacitanceData& src, SEScalarElectricCapacitance& dst);
  static void Serialize(const SEScalarElectricCapacitance& src, cdm::ScalarElectricCapacitanceData& dst);

  static void Load(const cdm::ScalarElectricChargeData& src, SEScalarElectricCharge& dst);
  static cdm::ScalarElectricChargeData* Unload(const SEScalarElectricCharge& src);
  static void Serialize(const cdm::ScalarElectricChargeData& src, SEScalarElectricCharge& dst);
  static void Serialize(const SEScalarElectricCharge& src, cdm::ScalarElectricChargeData& dst);

  static void Load(const cdm::ScalarElectricCurrentData& src, SEScalarElectricCurrent& dst);
  static cdm::ScalarElectricCurrentData* Unload(const SEScalarElectricCurrent& src);
  static void Serialize(const cdm::ScalarElectricCurrentData& src, SEScalarElectricCurrent& dst);
  static void Serialize(const SEScalarElectricCurrent& src, cdm::ScalarElectricCurrentData& dst);

  static void Load(const cdm::ScalarElectricInductanceData& src, SEScalarElectricInductance& dst);
  static cdm::ScalarElectricInductanceData* Unload(const SEScalarElectricInductance& src);
  static void Serialize(const cdm::ScalarElectricInductanceData& src, SEScalarElectricInductance& dst);
  static void Serialize(const SEScalarElectricInductance& src, cdm::ScalarElectricInductanceData& dst);

  static void Load(const cdm::ScalarElectricPotentialData& src, SEScalarElectricPotential& dst);
  static cdm::ScalarElectricPotentialData* Unload(const SEScalarElectricPotential& src);
  static void Serialize(const cdm::ScalarElectricPotentialData& src, SEScalarElectricPotential& dst);
  static void Serialize(const SEScalarElectricPotential& src, cdm::ScalarElectricPotentialData& dst);

  static void Load(const cdm::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst);
  static cdm::ScalarElectricResistanceData* Unload(const SEScalarElectricResistance& src);
  static void Serialize(const cdm::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst);
  static void Serialize(const SEScalarElectricResistance& src, cdm::ScalarElectricResistanceData& dst);

  static void Load(const cdm::ScalarEnergyData& src, SEScalarEnergy& dst);
  static cdm::ScalarEnergyData* Unload(const SEScalarEnergy& src);
  static void Serialize(const cdm::ScalarEnergyData& src, SEScalarEnergy& dst);
  static void Serialize(const SEScalarEnergy& src, cdm::ScalarEnergyData& dst);

  static void Load(const cdm::ScalarEnergyPerAmountData& src, SEScalarEnergyPerAmount& dst);
  static cdm::ScalarEnergyPerAmountData* Unload(const SEScalarEnergyPerAmount& src);
  static void Serialize(const cdm::ScalarEnergyPerAmountData& src, SEScalarEnergyPerAmount& dst);
  static void Serialize(const SEScalarEnergyPerAmount& src, cdm::ScalarEnergyPerAmountData& dst);

  static void Load(const cdm::ScalarEnergyPerMassData& src, SEScalarEnergyPerMass& dst);
  static cdm::ScalarEnergyPerMassData* Unload(const SEScalarEnergyPerMass& src);
  static void Serialize(const cdm::ScalarEnergyPerMassData& src, SEScalarEnergyPerMass& dst);
  static void Serialize(const SEScalarEnergyPerMass& src, cdm::ScalarEnergyPerMassData& dst);

  static void Load(const cdm::ScalarFlowComplianceData& src, SEScalarFlowCompliance& dst);
  static cdm::ScalarFlowComplianceData* Unload(const SEScalarFlowCompliance& src);
  static void Serialize(const cdm::ScalarFlowComplianceData& src, SEScalarFlowCompliance& dst);
  static void Serialize(const SEScalarFlowCompliance& src, cdm::ScalarFlowComplianceData& dst);

  static void Load(const cdm::ScalarFlowElastanceData& src, SEScalarFlowElastance& dst);
  static cdm::ScalarFlowElastanceData* Unload(const SEScalarFlowElastance& src);
  static void Serialize(const cdm::ScalarFlowElastanceData& src, SEScalarFlowElastance& dst);
  static void Serialize(const SEScalarFlowElastance& src, cdm::ScalarFlowElastanceData& dst);

  static void Load(const cdm::ScalarFlowInertanceData& src, SEScalarFlowInertance& dst);
  static cdm::ScalarFlowInertanceData* Unload(const SEScalarFlowInertance& src);
  static void Serialize(const cdm::ScalarFlowInertanceData& src, SEScalarFlowInertance& dst);
  static void Serialize(const SEScalarFlowInertance& src, cdm::ScalarFlowInertanceData& dst);

  static void Load(const cdm::ScalarFlowResistanceData& src, SEScalarFlowResistance& dst);
  static cdm::ScalarFlowResistanceData* Unload(const SEScalarFlowResistance& src);
  static void Serialize(const cdm::ScalarFlowResistanceData& src, SEScalarFlowResistance& dst);
  static void Serialize(const SEScalarFlowResistance& src, cdm::ScalarFlowResistanceData& dst);

  static void Load(const cdm::ScalarForceData& src, SEScalarForce& dst);
  static cdm::ScalarForceData* Unload(const SEScalarForce& src);
  static void Serialize(const cdm::ScalarForceData& src, SEScalarForce& dst);
  static void Serialize(const SEScalarForce& src, cdm::ScalarForceData& dst);

  static void Load(const cdm::ScalarFrequencyData& src, SEScalarFrequency& dst);
  static cdm::ScalarFrequencyData* Unload(const SEScalarFrequency& src);
  static void Serialize(const cdm::ScalarFrequencyData& src, SEScalarFrequency& dst);
  static void Serialize(const SEScalarFrequency& src, cdm::ScalarFrequencyData& dst);

  static void Load(const cdm::ScalarHeatCapacitanceData& src, SEScalarHeatCapacitance& dst);
  static cdm::ScalarHeatCapacitanceData* Unload(const SEScalarHeatCapacitance& src);
  static void Serialize(const cdm::ScalarHeatCapacitanceData& src, SEScalarHeatCapacitance& dst);
  static void Serialize(const SEScalarHeatCapacitance& src, cdm::ScalarHeatCapacitanceData& dst);

  static void Load(const cdm::ScalarHeatCapacitancePerAmountData& src, SEScalarHeatCapacitancePerAmount& dst);
  static cdm::ScalarHeatCapacitancePerAmountData* Unload(const SEScalarHeatCapacitancePerAmount& src);
  static void Serialize(const cdm::ScalarHeatCapacitancePerAmountData& src, SEScalarHeatCapacitancePerAmount& dst);
  static void Serialize(const SEScalarHeatCapacitancePerAmount& src, cdm::ScalarHeatCapacitancePerAmountData& dst);
  
  static void Load(const cdm::ScalarHeatCapacitancePerMassData& src, SEScalarHeatCapacitancePerMass& dst);
  static cdm::ScalarHeatCapacitancePerMassData* Unload(const SEScalarHeatCapacitancePerMass& src);
  static void Serialize(const cdm::ScalarHeatCapacitancePerMassData& src, SEScalarHeatCapacitancePerMass& dst);
  static void Serialize(const SEScalarHeatCapacitancePerMass& src, cdm::ScalarHeatCapacitancePerMassData& dst);

  static void Load(const cdm::ScalarHeatConductanceData& src, SEScalarHeatConductance& dst);
  static cdm::ScalarHeatConductanceData* Unload(const SEScalarHeatConductance& src);
  static void Serialize(const cdm::ScalarHeatConductanceData& src, SEScalarHeatConductance& dst);
  static void Serialize(const SEScalarHeatConductance& src, cdm::ScalarHeatConductanceData& dst);

  static void Load(const cdm::ScalarHeatConductancePerAreaData& src, SEScalarHeatConductancePerArea& dst);
  static cdm::ScalarHeatConductancePerAreaData* Unload(const SEScalarHeatConductancePerArea& src);
  static void Serialize(const cdm::ScalarHeatConductancePerAreaData& src, SEScalarHeatConductancePerArea& dst);
  static void Serialize(const SEScalarHeatConductancePerArea& src, cdm::ScalarHeatConductancePerAreaData& dst);

  static void Load(const cdm::ScalarHeatInductanceData& src, SEScalarHeatInductance& dst);
  static cdm::ScalarHeatInductanceData* Unload(const SEScalarHeatInductance& src);
  static void Serialize(const cdm::ScalarHeatInductanceData& src, SEScalarHeatInductance& dst);
  static void Serialize(const SEScalarHeatInductance& src, cdm::ScalarHeatInductanceData& dst);

  static void Load(const cdm::ScalarHeatResistanceData& src, SEScalarHeatResistance& dst);
  static cdm::ScalarHeatResistanceData* Unload(const SEScalarHeatResistance& src);
  static void Serialize(const cdm::ScalarHeatResistanceData& src, SEScalarHeatResistance& dst);
  static void Serialize(const SEScalarHeatResistance& src, cdm::ScalarHeatResistanceData& dst);

  static void Load(const cdm::ScalarHeatResistanceAreaData& src, SEScalarHeatResistanceArea& dst);
  static cdm::ScalarHeatResistanceAreaData* Unload(const SEScalarHeatResistanceArea& src);
  static void Serialize(const cdm::ScalarHeatResistanceAreaData& src, SEScalarHeatResistanceArea& dst);
  static void Serialize(const SEScalarHeatResistanceArea& src, cdm::ScalarHeatResistanceAreaData& dst);

  static void Load(const cdm::ScalarInversePressureData& src, SEScalarInversePressure& dst);
  static cdm::ScalarInversePressureData* Unload(const SEScalarInversePressure& src);
  static void Serialize(const cdm::ScalarInversePressureData& src, SEScalarInversePressure& dst);
  static void Serialize(const SEScalarInversePressure& src, cdm::ScalarInversePressureData& dst);

  static void Load(const cdm::ScalarInverseVolumeData& src, SEScalarInverseVolume& dst);
  static cdm::ScalarInverseVolumeData* Unload(const SEScalarInverseVolume& src);
  static void Serialize(const cdm::ScalarInverseVolumeData& src, SEScalarInverseVolume& dst);
  static void Serialize(const SEScalarInverseVolume& src, cdm::ScalarInverseVolumeData& dst);

  static void Load(const cdm::ScalarLengthData& src, SEScalarLength& dst);
  static cdm::ScalarLengthData* Unload(const SEScalarLength& src);
  static void Serialize(const cdm::ScalarLengthData& src, SEScalarLength& dst);
  static void Serialize(const SEScalarLength& src, cdm::ScalarLengthData& dst);

  static void Load(const cdm::ScalarLengthPerTimeData& src, SEScalarLengthPerTime& dst);
  static cdm::ScalarLengthPerTimeData* Unload(const SEScalarLengthPerTime& src);
  static void Serialize(const cdm::ScalarLengthPerTimeData& src, SEScalarLengthPerTime& dst);
  static void Serialize(const SEScalarLengthPerTime& src, cdm::ScalarLengthPerTimeData& dst);

  static void Load(const cdm::ScalarLengthPerTimePressureData& src, SEScalarLengthPerTimePressure& dst);
  static cdm::ScalarLengthPerTimePressureData* Unload(const SEScalarLengthPerTimePressure& src);
  static void Serialize(const cdm::ScalarLengthPerTimePressureData& src, SEScalarLengthPerTimePressure& dst);
  static void Serialize(const SEScalarLengthPerTimePressure& src, cdm::ScalarLengthPerTimePressureData& dst);

  static void Load(const cdm::ScalarMassData& src, SEScalarMass& dst);
  static cdm::ScalarMassData* Unload(const SEScalarMass& src);
  static void Serialize(const cdm::ScalarMassData& src, SEScalarMass& dst);
  static void Serialize(const SEScalarMass& src, cdm::ScalarMassData& dst);

  static void Load(const cdm::ScalarMassPerAmountData& src, SEScalarMassPerAmount& dst);
  static cdm::ScalarMassPerAmountData* Unload(const SEScalarMassPerAmount& src);
  static void Serialize(const cdm::ScalarMassPerAmountData& src, SEScalarMassPerAmount& dst);
  static void Serialize(const SEScalarMassPerAmount& src, cdm::ScalarMassPerAmountData& dst);

  static void Load(const cdm::ScalarMassPerAreaTimeData& src, SEScalarMassPerAreaTime& dst);
  static cdm::ScalarMassPerAreaTimeData* Unload(const SEScalarMassPerAreaTime& src);
  static void Serialize(const cdm::ScalarMassPerAreaTimeData& src, SEScalarMassPerAreaTime& dst);
  static void Serialize(const SEScalarMassPerAreaTime& src, cdm::ScalarMassPerAreaTimeData& dst);

  static void Load(const cdm::ScalarMassPerMassData& src, SEScalarMassPerMass& dst);
  static cdm::ScalarMassPerMassData* Unload(const SEScalarMassPerMass& src);
  static void Serialize(const cdm::ScalarMassPerMassData& src, SEScalarMassPerMass& dst);
  static void Serialize(const SEScalarMassPerMass& src, cdm::ScalarMassPerMassData& dst);

  static void Load(const cdm::ScalarMassPerTimeData& src, SEScalarMassPerTime& dst);
  static cdm::ScalarMassPerTimeData* Unload(const SEScalarMassPerTime& src);
  static void Serialize(const cdm::ScalarMassPerTimeData& src, SEScalarMassPerTime& dst);
  static void Serialize(const SEScalarMassPerTime& src, cdm::ScalarMassPerTimeData& dst);

  static void Load(const cdm::ScalarMassPerVolumeData& src, SEScalarMassPerVolume& dst);
  static cdm::ScalarMassPerVolumeData* Unload(const SEScalarMassPerVolume& src);
  static void Serialize(const cdm::ScalarMassPerVolumeData& src, SEScalarMassPerVolume& dst);
  static void Serialize(const SEScalarMassPerVolume& src, cdm::ScalarMassPerVolumeData& dst);

  static void Load(const cdm::ScalarNegative1To1Data& src, SEScalarNegative1To1& dst);
  static cdm::ScalarNegative1To1Data* Unload(const SEScalarNegative1To1& src);
  static void Serialize(const cdm::ScalarNegative1To1Data& src, SEScalarNegative1To1& dst);
  static void Serialize(const SEScalarNegative1To1& src, cdm::ScalarNegative1To1Data& dst);

  static void Load(const cdm::ScalarOsmolalityData& src, SEScalarOsmolality& dst);
  static cdm::ScalarOsmolalityData* Unload(const SEScalarOsmolality& src);
  static void Serialize(const cdm::ScalarOsmolalityData& src, SEScalarOsmolality& dst);
  static void Serialize(const SEScalarOsmolality& src, cdm::ScalarOsmolalityData& dst);

  static void Load(const cdm::ScalarOsmolarityData& src, SEScalarOsmolarity& dst);
  static cdm::ScalarOsmolarityData* Unload(const SEScalarOsmolarity& src);
  static void Serialize(const cdm::ScalarOsmolarityData& src, SEScalarOsmolarity& dst);
  static void Serialize(const SEScalarOsmolarity& src, cdm::ScalarOsmolarityData& dst);

  static void Load(const cdm::ScalarPowerData& src, SEScalarPower& dst);
  static cdm::ScalarPowerData* Unload(const SEScalarPower& src);
  static void Serialize(const cdm::ScalarPowerData& src, SEScalarPower& dst);
  static void Serialize(const SEScalarPower& src, cdm::ScalarPowerData& dst);

  static void Load(const cdm::ScalarPowerPerAreaTemperatureToTheFourthData& src, SEScalarPowerPerAreaTemperatureToTheFourth& dst);
  static cdm::ScalarPowerPerAreaTemperatureToTheFourthData* Unload(const SEScalarPowerPerAreaTemperatureToTheFourth& src);
  static void Serialize(const cdm::ScalarPowerPerAreaTemperatureToTheFourthData& src, SEScalarPowerPerAreaTemperatureToTheFourth& dst);
  static void Serialize(const SEScalarPowerPerAreaTemperatureToTheFourth& src, cdm::ScalarPowerPerAreaTemperatureToTheFourthData& dst);

  static void Load(const cdm::ScalarPressureData& src, SEScalarPressure& dst);
  static cdm::ScalarPressureData* Unload(const SEScalarPressure& src);
  static void Serialize(const cdm::ScalarPressureData& src, SEScalarPressure& dst);
  static void Serialize(const SEScalarPressure& src, cdm::ScalarPressureData& dst);

  static void Load(const cdm::ScalarPressurePerVolumeData& src, SEScalarPressurePerVolume& dst);
  static cdm::ScalarPressurePerVolumeData* Unload(const SEScalarPressurePerVolume& src);
  static void Serialize(const cdm::ScalarPressurePerVolumeData& src, SEScalarPressurePerVolume& dst);
  static void Serialize(const SEScalarPressurePerVolume& src, cdm::ScalarPressurePerVolumeData& dst);

  static void Load(const cdm::ScalarPressureTimePerAreaData& src, SEScalarPressureTimePerArea& dst);
  static cdm::ScalarPressureTimePerAreaData* Unload(const SEScalarPressureTimePerArea& src);
  static void Serialize(const cdm::ScalarPressureTimePerAreaData& src, SEScalarPressureTimePerArea& dst);
  static void Serialize(const SEScalarPressureTimePerArea& src, cdm::ScalarPressureTimePerAreaData& dst);

  static void Load(const cdm::ScalarPressureTimePerVolumeAreaData& src, SEScalarPressureTimePerVolumeArea& dst);
  static cdm::ScalarPressureTimePerVolumeAreaData* Unload(const SEScalarPressureTimePerVolumeArea& src);
  static void Serialize(const cdm::ScalarPressureTimePerVolumeAreaData& src, SEScalarPressureTimePerVolumeArea& dst);
  static void Serialize(const SEScalarPressureTimePerVolumeArea& src, cdm::ScalarPressureTimePerVolumeAreaData& dst);

  static void Load(const cdm::ScalarTemperatureData& src, SEScalarTemperature& dst);
  static cdm::ScalarTemperatureData* Unload(const SEScalarTemperature& src);
  static void Serialize(const cdm::ScalarTemperatureData& src, SEScalarTemperature& dst);
  static void Serialize(const SEScalarTemperature& src, cdm::ScalarTemperatureData& dst);

  static void Load(const cdm::ScalarTimeData& src, SEScalarTime& dst);
  static cdm::ScalarTimeData* Unload(const SEScalarTime& src);
  static void Serialize(const cdm::ScalarTimeData& src, SEScalarTime& dst);
  static void Serialize(const SEScalarTime& src, cdm::ScalarTimeData& dst);

  static void Load(const cdm::ScalarVolumeData& src, SEScalarVolume& dst);
  static cdm::ScalarVolumeData* Unload(const SEScalarVolume& src);
  static void Serialize(const cdm::ScalarVolumeData& src, SEScalarVolume& dst);
  static void Serialize(const SEScalarVolume& src, cdm::ScalarVolumeData& dst);

  static void Load(const cdm::ScalarVolumePerPressureData& src, SEScalarVolumePerPressure& dst);
  static cdm::ScalarVolumePerPressureData* Unload(const SEScalarVolumePerPressure& src);
  static void Serialize(const cdm::ScalarVolumePerPressureData& src, SEScalarVolumePerPressure& dst);
  static void Serialize(const SEScalarVolumePerPressure& src, cdm::ScalarVolumePerPressureData& dst);

  static void Load(const cdm::ScalarVolumePerTimeData& src, SEScalarVolumePerTime& dst);
  static cdm::ScalarVolumePerTimeData* Unload(const SEScalarVolumePerTime& src);
  static void Serialize(const cdm::ScalarVolumePerTimeData& src, SEScalarVolumePerTime& dst);
  static void Serialize(const SEScalarVolumePerTime& src, cdm::ScalarVolumePerTimeData& dst);

  static void Load(const cdm::ScalarVolumePerTimeAreaData& src, SEScalarVolumePerTimeArea& dst);
  static cdm::ScalarVolumePerTimeAreaData* Unload(const SEScalarVolumePerTimeArea& src);
  static void Serialize(const cdm::ScalarVolumePerTimeAreaData& src, SEScalarVolumePerTimeArea& dst);
  static void Serialize(const SEScalarVolumePerTimeArea& src, cdm::ScalarVolumePerTimeAreaData& dst);

  static void Load(const cdm::ScalarVolumePerTimeMassData& src, SEScalarVolumePerTimeMass& dst);
  static cdm::ScalarVolumePerTimeMassData* Unload(const SEScalarVolumePerTimeMass& src);
  static void Serialize(const cdm::ScalarVolumePerTimeMassData& src, SEScalarVolumePerTimeMass& dst);
  static void Serialize(const SEScalarVolumePerTimeMass& src, cdm::ScalarVolumePerTimeMassData& dst);

  static void Load(const cdm::ScalarVolumePerTimePressureData& src, SEScalarVolumePerTimePressure& dst);
  static cdm::ScalarVolumePerTimePressureData* Unload(const SEScalarVolumePerTimePressure& src);
  static void Serialize(const cdm::ScalarVolumePerTimePressureData& src, SEScalarVolumePerTimePressure& dst);
  static void Serialize(const SEScalarVolumePerTimePressure& src, cdm::ScalarVolumePerTimePressureData& dst);

  static void Load(const cdm::ScalarVolumePerTimePressureAreaData& src, SEScalarVolumePerTimePressureArea& dst);
  static cdm::ScalarVolumePerTimePressureAreaData* Unload(const SEScalarVolumePerTimePressureArea& src);
  static void Serialize(const cdm::ScalarVolumePerTimePressureAreaData& src, SEScalarVolumePerTimePressureArea& dst);
  static void Serialize(const SEScalarVolumePerTimePressureArea& src, cdm::ScalarVolumePerTimePressureAreaData& dst);

  // Functions
  static void Load(const cdm::FunctionData& src, SEFunction& dst);
  static cdm::FunctionData* Unload(const SEFunction& src);
  static void Serialize(const cdm::FunctionData& src, SEFunction& dst);
  static void Serialize(const SEFunction& src, cdm::FunctionData& dst);

  static void Load(const cdm::FunctionElectricPotentialVsTimeData& src, SEFunctionElectricPotentialVsTime& dst);
  static cdm::FunctionElectricPotentialVsTimeData* Unload(const SEFunctionElectricPotentialVsTime& src);
  static void Serialize(const cdm::FunctionElectricPotentialVsTimeData& src, SEFunctionElectricPotentialVsTime& dst);
  static void Serialize(const SEFunctionElectricPotentialVsTime& src, cdm::FunctionElectricPotentialVsTimeData& dst);

  static void Load(const cdm::FunctionVolumeVsTimeData& src, SEFunctionVolumeVsTime& dst);
  static cdm::FunctionVolumeVsTimeData* Unload(const SEFunctionVolumeVsTime& src);
  static void Serialize(const cdm::FunctionVolumeVsTimeData& src, SEFunctionVolumeVsTime& dst);
  static void Serialize(const SEFunctionVolumeVsTime& src, cdm::FunctionVolumeVsTimeData& dst);

  // Histogram
  static void Load(const cdm::HistogramData& src, SEHistogram& dst);
  static cdm::HistogramData* Unload(const SEHistogram& src);
  static void Serialize(const cdm::HistogramData& src, SEHistogram& dst);
  static void Serialize(const SEHistogram& src, cdm::HistogramData& dst);

  static void Load(const cdm::HistogramFractionVsLengthData& src, SEHistogramFractionVsLength& dst);
  static cdm::HistogramFractionVsLengthData* Unload(const SEHistogramFractionVsLength& src);
  static void Serialize(const cdm::HistogramFractionVsLengthData& src, SEHistogramFractionVsLength& dst);
  static void Serialize(const SEHistogramFractionVsLength& src, cdm::HistogramFractionVsLengthData& dst);


  static void Load(const cdm::RunningAverageData& src, SERunningAverage& dst);
  static cdm::RunningAverageData* Unload(const SERunningAverage& src);
  static void Serialize(const cdm::RunningAverageData& src, SERunningAverage& dst);
  static void Serialize(const SERunningAverage& src, cdm::RunningAverageData& dst);

};