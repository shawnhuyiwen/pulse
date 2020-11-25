/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "circuit/SECircuitNode.h"
class SEFluidBlackBox;

class CDM_DECL SEFluidCircuitNode : public SECircuitNode<FLUID_CIRCUIT_NODE>
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;  
protected:
  SEFluidCircuitNode(const std::string& name, Logger* logger);
public:
  virtual ~SEFluidCircuitNode();

  virtual void Clear(); //clear memory

  virtual bool HasPressure() const;
  virtual SEScalarPressure& GetPressure();
  virtual double GetPressure(const PressureUnit& unit) const;

  virtual bool HasNextPressure() const;
  virtual SEScalarPressure& GetNextPressure();
  virtual double GetNextPressure(const PressureUnit& unit) const;

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual bool HasNextVolume() const;
  virtual SEScalarVolume& GetNextVolume();
  virtual double GetNextVolume(const VolumeUnit& unit) const;

  virtual bool HasVolumeBaseline() const;
  virtual SEScalarVolume& GetVolumeBaseline();
  virtual double GetVolumeBaseline(const VolumeUnit& unit) const;
};
