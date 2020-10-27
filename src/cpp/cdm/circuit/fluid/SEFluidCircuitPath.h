/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "circuit/SECircuitPath.h"
#include "circuit/fluid/SEFluidCircuitNode.h"

class CDM_DECL SEFluidCircuitPath : public SECircuitPath<FLUID_CIRCUIT_PATH>
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;  
protected:
  SEFluidCircuitPath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name);
public:
  virtual ~SEFluidCircuitPath();

  virtual void Clear(); //clear memory

  virtual SEFluidCircuitNode& GetSourceNode() const { return m_FluidSourceNode; }
  virtual SEFluidCircuitNode& GetTargetNode() const { return m_FluidTargetNode; }

  virtual bool HasResistance() const;
  virtual SEScalarPressureTimePerVolume& GetResistance();
  virtual double GetResistance(const PressureTimePerVolumeUnit& unit) const;
  virtual bool HasNextResistance() const;
  virtual SEScalarPressureTimePerVolume& GetNextResistance();
  virtual double GetNextResistance(const PressureTimePerVolumeUnit& unit) const;
  virtual bool HasResistanceBaseline() const;
  virtual SEScalarPressureTimePerVolume& GetResistanceBaseline();
  virtual double GetResistanceBaseline(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasCompliance() const;
  virtual SEScalarVolumePerPressure& GetCompliance();
  virtual double GetCompliance(const VolumePerPressureUnit& unit) const;
  virtual bool HasNextCompliance() const;
  virtual SEScalarVolumePerPressure& GetNextCompliance();
  virtual double GetNextCompliance(const VolumePerPressureUnit& unit) const;
  virtual bool HasComplianceBaseline() const;
  virtual SEScalarVolumePerPressure& GetComplianceBaseline();
  virtual double GetComplianceBaseline(const VolumePerPressureUnit& unit) const;

  virtual bool HasInertance() const;
  virtual SEScalarPressureTimeSquaredPerVolume& GetInertance();
  virtual double GetInertance(const PressureTimeSquaredPerVolumeUnit& unit) const;
  virtual bool HasNextInertance() const;
  virtual SEScalarPressureTimeSquaredPerVolume& GetNextInertance();
  virtual double GetNextInertance(const PressureTimeSquaredPerVolumeUnit& unit) const;
  virtual bool HasInertanceBaseline() const;
  virtual SEScalarPressureTimeSquaredPerVolume& GetInertanceBaseline();
  virtual double GetInertanceBaseline(const PressureTimeSquaredPerVolumeUnit& unit) const;

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;
  virtual bool HasNextFlow() const;
  virtual SEScalarVolumePerTime& GetNextFlow();
  virtual double GetNextFlow(const VolumePerTimeUnit& unit) const; 
  virtual bool HasFlowSource() const;
  virtual SEScalarVolumePerTime& GetFlowSource();
  virtual double GetFlowSource(const VolumePerTimeUnit& unit) const;
  virtual bool HasNextFlowSource() const;
  virtual SEScalarVolumePerTime& GetNextFlowSource();
  virtual double GetNextFlowSource(const VolumePerTimeUnit& unit) const;
  virtual bool HasFlowSourceBaseline() const;
  virtual SEScalarVolumePerTime& GetFlowSourceBaseline();
  virtual double GetFlowSourceBaseline(const VolumePerTimeUnit& unit) const;

  virtual bool HasPressureSource() const;
  virtual SEScalarPressure& GetPressureSource();
  virtual double GetPressureSource(const PressureUnit& unit) const;
  virtual bool HasNextPressureSource() const;
  virtual SEScalarPressure& GetNextPressureSource();
  virtual double GetNextPressureSource(const PressureUnit& unit) const;
  virtual bool HasPressureSourceBaseline() const;
  virtual SEScalarPressure& GetPressureSourceBaseline();
  virtual double GetPressureSourceBaseline(const PressureUnit& unit) const;
  virtual bool HasValveBreakdownPressure() const;
  virtual SEScalarPressure& GetValveBreakdownPressure();
  virtual double GetValveBreakdownPressure(const PressureUnit& unit) const;

protected:
  SEFluidCircuitNode& m_FluidSourceNode;
  SEFluidCircuitNode& m_FluidTargetNode;
};
