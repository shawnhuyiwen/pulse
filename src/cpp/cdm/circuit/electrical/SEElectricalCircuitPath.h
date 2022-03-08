/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/circuit/SECircuitPath.h"
#include "cdm/circuit/electrical/SEElectricalCircuitNode.h"

class CDM_DECL SEElectricalCircuitPath : public SECircuitPath<ELECTRICAL_CIRCUIT_PATH>
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;  
protected:
  SEElectricalCircuitPath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const std::string& name);
public:
  virtual ~SEElectricalCircuitPath();

  void Clear() override; //clear memory

  SEElectricalCircuitNode& GetSourceNode() const override { return m_ElectricalSourceNode; }
  SEElectricalCircuitNode& GetTargetNode() const override { return m_ElectricalTargetNode; }

  virtual bool HasCurrent() const;
  virtual SEScalarElectricCurrent& GetCurrent();
  virtual double GetCurrent(const ElectricCurrentUnit& unit) const;
  virtual bool HasNextCurrent() const;
  virtual SEScalarElectricCurrent& GetNextCurrent();
  virtual double GetNextCurrent(const ElectricCurrentUnit& unit) const;

  bool HasResistance() const override;
  SEScalarElectricResistance& GetResistance() override;
  virtual double GetResistance(const ElectricResistanceUnit& unit) const;
  bool HasNextResistance() const override;
  SEScalarElectricResistance& GetNextResistance() override;
  virtual double GetNextResistance(const ElectricResistanceUnit& unit) const;
  bool HasResistanceBaseline() const override;
  SEScalarElectricResistance& GetResistanceBaseline() override;
  virtual double GetResistanceBaseline(const ElectricResistanceUnit& unit) const;
  virtual void RemoveResistance() override;

  bool HasCapacitance() const override;
  SEScalarElectricCapacitance& GetCapacitance() override;
  virtual double GetCapacitance(const ElectricCapacitanceUnit& unit) const;
  bool HasNextCapacitance() const override;
  SEScalarElectricCapacitance& GetNextCapacitance() override;
  virtual double GetNextCapacitance(const ElectricCapacitanceUnit& unit) const;
  bool HasCapacitanceBaseline() const override;
  SEScalarElectricCapacitance& GetCapacitanceBaseline() override;
  virtual double GetCapacitanceBaseline(const ElectricCapacitanceUnit& unit) const;
  virtual void RemoveCapacitance() override;

  bool HasInductance() const override;
  SEScalarElectricInductance& GetInductance() override;
  virtual double GetInductance(const ElectricInductanceUnit& unit) const;
  bool HasNextInductance() const override;
  SEScalarElectricInductance& GetNextInductance() override; 
  virtual double GetNextInductance(const ElectricInductanceUnit& unit) const;
  bool HasInductanceBaseline() const override;
  SEScalarElectricInductance& GetInductanceBaseline() override;
  virtual double GetInductanceBaseline(const ElectricInductanceUnit& unit) const;
  virtual void RemoveInductance() override;

  virtual bool HasCurrentSource() const;
  virtual SEScalarElectricCurrent& GetCurrentSource();
  virtual double GetCurrentSource(const ElectricCurrentUnit& unit) const;
  virtual bool HasNextCurrentSource() const;
  virtual SEScalarElectricCurrent& GetNextCurrentSource();
  virtual double GetNextCurrentSource(const ElectricCurrentUnit& unit) const;
  virtual bool HasCurrentSourceBaseline() const;
  virtual SEScalarElectricCurrent& GetCurrentSourceBaseline();
  virtual double GetCurrentSourceBaseline(const ElectricCurrentUnit& unit) const;
  virtual void RemoveCurrentSource();

  virtual bool HasVoltageSource() const;
  virtual SEScalarElectricPotential& GetVoltageSource();
  virtual double GetVoltageSource(const ElectricPotentialUnit& unit) const;
  virtual bool HasNextVoltageSource() const;
  virtual SEScalarElectricPotential& GetNextVoltageSource();
  virtual double GetNextVoltageSource(const ElectricPotentialUnit& unit) const;
  virtual bool HasVoltageSourceBaseline() const;
  virtual SEScalarElectricPotential& GetVoltageSourceBaseline();
  virtual double GetVoltageSourceBaseline(const ElectricPotentialUnit& unit) const;
  virtual void RemoveVoltageSource();

  virtual bool HasValveBreakdownVoltage() const;
  virtual SEScalarElectricPotential& GetValveBreakdownVoltage();
  virtual double GetValveBreakdownVoltage(const ElectricPotentialUnit& unit) const;


protected:
  SEElectricalCircuitNode& m_ElectricalSourceNode;
  SEElectricalCircuitNode& m_ElectricalTargetNode;
};
