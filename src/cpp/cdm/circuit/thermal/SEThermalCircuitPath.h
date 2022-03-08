/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/circuit/SECircuitPath.h"
#include "cdm/circuit/thermal/SEThermalCircuitNode.h"

class CDM_DECL SEThermalCircuitPath : public SECircuitPath<THERMAL_CIRCUIT_PATH>
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;  
protected:
  SEThermalCircuitPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name);
public:  
  virtual ~SEThermalCircuitPath();

  void Clear() override; //clear memory

  SEThermalCircuitNode& GetSourceNode() const override;
  SEThermalCircuitNode& GetTargetNode() const override;

  virtual bool HasHeatTransferRate() const;
  virtual SEScalarPower& GetHeatTransferRate();
  virtual double GetHeatTransferRate(const PowerUnit& unit) const;
  virtual bool HasNextHeatTransferRate() const;
  virtual SEScalarPower& GetNextHeatTransferRate();
  virtual double GetNextHeatTransferRate(const PowerUnit& unit) const;

  bool HasResistance() const override;
  SEScalarHeatResistance& GetResistance() override;
  virtual double GetResistance(const HeatResistanceUnit& unit) const;
  bool HasNextResistance() const override;
  SEScalarHeatResistance& GetNextResistance() override;
  virtual double GetNextResistance(const HeatResistanceUnit& unit) const;
  bool HasResistanceBaseline() const override;
  SEScalarHeatResistance& GetResistanceBaseline() override;
  virtual double GetResistanceBaseline(const HeatResistanceUnit& unit) const;
  virtual void RemoveResistance() override;

  bool HasCapacitance() const override;
  SEScalarHeatCapacitance& GetCapacitance() override;
  virtual double GetCapacitance(const HeatCapacitanceUnit& unit) const;
  bool HasNextCapacitance() const override;
  SEScalarHeatCapacitance& GetNextCapacitance() override;
  virtual double GetNextCapacitance(const HeatCapacitanceUnit& unit) const;
  bool HasCapacitanceBaseline() const override;
  SEScalarHeatCapacitance& GetCapacitanceBaseline() override;
  virtual double GetCapacitanceBaseline(const HeatCapacitanceUnit& unit) const;
  virtual void RemoveCapacitance() override;

  bool HasInductance() const override;
  SEScalarHeatInductance& GetInductance() override;
  virtual double GetInductance(const HeatInductanceUnit& unit) const;
  bool HasNextInductance() const override;
  SEScalarHeatInductance& GetNextInductance() override;
  virtual double GetNextInductance(const HeatInductanceUnit& unit) const;
  bool HasInductanceBaseline() const override;
  SEScalarHeatInductance& GetInductanceBaseline() override;
  virtual double GetInductanceBaseline(const HeatInductanceUnit& unit) const;
  virtual void RemoveInductance() override;

  virtual bool HasHeatSource() const;
  virtual SEScalarPower& GetHeatSource();
  virtual double GetHeatSource(const PowerUnit& unit) const;
  virtual bool HasNextHeatSource() const;
  virtual SEScalarPower& GetNextHeatSource();
  virtual double GetNextHeatSource(const PowerUnit& unit) const;
  virtual bool HasHeatSourceBaseline() const;
  virtual SEScalarPower& GetHeatSourceBaseline();
  virtual double GetHeatSourceBaseline(const PowerUnit& unit) const;
  virtual void RemoveHeatSource();

  virtual bool HasTemperatureSource() const;
  virtual SEScalarTemperature& GetTemperatureSource();
  virtual double GetTemperatureSource(const TemperatureUnit& unit) const;
  virtual bool HasNextTemperatureSource() const;
  virtual SEScalarTemperature& GetNextTemperatureSource();
  virtual double GetNextTemperatureSource(const TemperatureUnit& unit) const;
  virtual bool HasTemperatureSourceBaseline() const;
  virtual SEScalarTemperature& GetTemperatureSourceBaseline();
  virtual double GetTemperatureSourceBaseline(const TemperatureUnit& unit) const;
  virtual void RemoveTemperatureSource();

  virtual bool HasValveBreakdownTemperature() const;
  virtual SEScalarTemperature& GetValveBreakdownTemperature();
  virtual double GetValveBreakdownTemperature(const TemperatureUnit& unit) const;

protected:
  SEThermalCircuitNode& m_ThermalSourceNode;
  SEThermalCircuitNode& m_ThermalTargetNode;
};
