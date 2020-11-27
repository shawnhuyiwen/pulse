/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "circuit/SECircuitNode.h"
class SEElectricalBlackBox;

class CDM_DECL SEElectricalCircuitNode : public SECircuitNode<ELECTRICAL_CIRCUIT_NODE>
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;
protected:
  SEElectricalCircuitNode(const std::string& name, Logger* logger);
public:
  virtual ~SEElectricalCircuitNode();

  virtual void Clear(); //clear memory

public:
  virtual bool HasVoltage() const;
  virtual SEScalarElectricPotential& GetVoltage();
  virtual double GetVoltage(const ElectricPotentialUnit& unit) const;

  virtual bool HasNextVoltage() const;
  virtual SEScalarElectricPotential& GetNextVoltage();
  virtual double GetNextVoltage(const ElectricPotentialUnit& unit) const;


  virtual bool HasCharge() const;
  virtual SEScalarElectricCharge& GetCharge();
  virtual double GetCharge(const ElectricChargeUnit& unit) const;

  virtual bool HasNextCharge() const;
  virtual SEScalarElectricCharge& GetNextCharge();
  virtual double GetNextCharge(const ElectricChargeUnit& unit) const;

  virtual bool HasChargeBaseline() const;
  virtual SEScalarElectricCharge& GetChargeBaseline();
  virtual double GetChargeBaseline(const ElectricChargeUnit& unit) const;
};
