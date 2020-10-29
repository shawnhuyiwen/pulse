/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "circuit/SECircuitNode.h"
class SEThermalBlackBox;

class CDM_DECL SEThermalCircuitNode : public SECircuitNode<THERMAL_CIRCUIT_NODE>
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;  
protected:
  SEThermalCircuitNode(const std::string& name, Logger* logger);
public:
  virtual ~SEThermalCircuitNode();

  virtual void Clear(); //clear memory

  virtual bool HasTemperature() const;
  virtual SEScalarTemperature& GetTemperature();
  virtual double GetTemperature(const TemperatureUnit& unit) const;

  virtual bool HasNextTemperature() const;
  virtual SEScalarTemperature& GetNextTemperature();
  virtual double GetNextTemperature(const TemperatureUnit& unit) const;


  virtual bool HasHeat() const;
  virtual SEScalarEnergy& GetHeat();
  virtual double GetHeat(const EnergyUnit& unit) const;

  virtual bool HasNextHeat() const;
  virtual SEScalarEnergy& GetNextHeat();
  virtual double GetNextHeat(const EnergyUnit& unit) const;

  virtual bool HasHeatBaseline() const;
  virtual SEScalarEnergy& GetHeatBaseline();
  virtual double GetHeatBaseline(const EnergyUnit& unit) const;
};
