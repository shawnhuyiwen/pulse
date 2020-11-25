/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "circuit/SECircuitNode.h"
#include "properties/SEScalarElectricCapacitance.h"
#include "properties/SEScalarElectricCurrent.h"
#include "properties/SEScalarElectricInductance.h"
#include "properties/SEScalarElectricResistance.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarPressureTimeSquaredPerVolume.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarHeatCapacitance.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarHeatInductance.h"
#include "properties/SEScalarHeatResistance.h"

#define CIRCUIT_PATH_TEMPLATE typename BlackBoxType, typename FluxScalar, typename ResistanceScalar, typename CapacitanceScalar, typename InductanceScalar, typename PotentialScalar, typename QuantityScalar
#define CIRCUIT_PATH_TYPES BlackBoxType, FluxScalar,ResistanceScalar,CapacitanceScalar,InductanceScalar,PotentialScalar,QuantityScalar
#define ELECTRICAL_CIRCUIT_PATH SEElectricalBlackBox, SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge
#define FLUID_CIRCUIT_PATH SEFluidBlackBox, SEScalarVolumePerTime, SEScalarPressureTimePerVolume, SEScalarVolumePerPressure, SEScalarPressureTimeSquaredPerVolume, SEScalarPressure, SEScalarVolume
#define THERMAL_CIRCUIT_PATH SEThermalBlackBox, SEScalarPower, SEScalarHeatResistance, SEScalarHeatCapacitance, SEScalarHeatInductance, SEScalarTemperature, SEScalarEnergy

template<CIRCUIT_PATH_TEMPLATE>
class SECircuitPath : public Loggable
{
  friend class PBCircuit;//friend the serialization class
  template<typename NodeType, typename PathType> friend class SECircuit;
protected:
  SECircuitPath(SECircuitNode<CIRCUIT_NODE_TYPES>& src, SECircuitNode<CIRCUIT_NODE_TYPES>& tgt, const std::string& name);
public:
  virtual ~SECircuitPath();

  virtual void Clear();

  virtual std::string GetName() const;

  virtual SECircuitNode<CIRCUIT_NODE_TYPES>& GetSourceNode() const;
  virtual SECircuitNode<CIRCUIT_NODE_TYPES>& GetTargetNode() const;

  virtual bool HasFlux() const;
  virtual FluxScalar& GetFlux();
  virtual bool HasNextFlux() const;
  virtual FluxScalar& GetNextFlux();
  virtual bool HasFluxSource() const;
  virtual FluxScalar& GetFluxSource();
  virtual bool HasNextFluxSource() const;
  virtual FluxScalar& GetNextFluxSource();
  virtual bool HasFluxSourceBaseline() const;
  virtual FluxScalar& GetFluxSourceBaseline();

  virtual bool HasResistance() const;
  virtual ResistanceScalar& GetResistance();
  virtual bool HasNextResistance() const;
  virtual ResistanceScalar& GetNextResistance();
  virtual bool HasResistanceBaseline() const;
  virtual ResistanceScalar& GetResistanceBaseline();

  virtual bool HasCapacitance() const;
  virtual CapacitanceScalar& GetCapacitance();
  virtual bool HasNextCapacitance() const;
  virtual CapacitanceScalar& GetNextCapacitance();
  virtual bool HasCapacitanceBaseline() const;
  virtual CapacitanceScalar& GetCapacitanceBaseline();

  virtual bool HasInductance() const;
  virtual InductanceScalar& GetInductance();
  virtual bool HasNextInductance() const;
  virtual InductanceScalar& GetNextInductance();
  virtual bool HasInductanceBaseline() const;
  virtual InductanceScalar& GetInductanceBaseline();

  virtual bool HasPotentialSource() const;
  virtual PotentialScalar& GetPotentialSource();
  virtual bool HasNextPotentialSource() const;
  virtual PotentialScalar& GetNextPotentialSource();
  virtual bool HasPotentialSourceBaseline() const;
  virtual PotentialScalar& GetPotentialSourceBaseline();
  virtual bool HasValveBreakdownPotential() const;
  virtual PotentialScalar& GetValveBreakdownPotential();
  
  virtual bool HasValidElements() const;
  virtual unsigned short NumberOfElements() const { return m_NumElements; }
  virtual unsigned short NumberOfNextElements() const { return m_NumNextElements; }

  virtual eGate GetSwitch() const;
  virtual void SetSwitch(eGate state);
  virtual void FlipSwitch();
  virtual bool HasSwitch() const;
  virtual void InvalidateSwitch();

  virtual eGate GetNextSwitch() const;
  virtual void SetNextSwitch(eGate state);
  virtual void FlipNextSwitch();
  virtual bool HasNextSwitch() const;
  virtual void InvalidateNextSwitch();

  virtual eGate GetValve() const;
  virtual void SetValve(eGate state);
  virtual void FlipValve();
  virtual bool HasValve() const;
  virtual void InvalidateValve();

  virtual eGate GetNextValve() const;
  virtual void SetNextValve(eGate state);
  virtual void FlipNextValve();
  virtual bool HasNextValve() const;
  virtual void InvalidateNextValve();

  virtual eGate GetPolarizedState() const;
  virtual void SetPolarizedState(eGate state);
  virtual void FlipPolarizedState();
  virtual bool HasPolarizedState() const;
  virtual void InvalidatePolarizedState();

  virtual eGate GetNextPolarizedState() const;
  virtual void SetNextPolarizedState(eGate state);
  virtual void FlipNextPolarizedState();
  virtual bool HasNextPolarizedState() const;
  virtual void InvalidateNextPolarizedState();

  virtual bool HasBlackBox() const { return m_BlackBox != nullptr; }
  virtual BlackBoxType* GetBlackBox() const { return m_BlackBox; }
  virtual void SetBlackBox(BlackBoxType* bb) { m_BlackBox = bb; }

protected:
  std::string                         m_Name;
  SECircuitNode<CIRCUIT_NODE_TYPES>&  m_SourceNode;
  SECircuitNode<CIRCUIT_NODE_TYPES>&  m_TargetNode;
  mutable unsigned short              m_NumElements;
  mutable unsigned short              m_NumNextElements;
  /////////////////////////    
  // Valves and Switches //    
  /////////////////////////    
  eGate                        m_Switch;
  eGate                        m_NextSwitch;
  eGate                        m_Valve;
  eGate                        m_NextValve;
  eGate                        m_PolarizedState;
  eGate                        m_NextPolarizedState;

  ResistanceScalar*            m_Resistance;
  ResistanceScalar*            m_NextResistance;
  ResistanceScalar*            m_ResistanceBaseline;
                 
  CapacitanceScalar*           m_Capacitance;
  CapacitanceScalar*           m_NextCapacitance;
  CapacitanceScalar*           m_CapacitanceBaseline;

  InductanceScalar*            m_Inductance;
  InductanceScalar*            m_NextInductance;
  InductanceScalar*            m_InductanceBaseline;

  FluxScalar*                  m_Flux;
  FluxScalar*                  m_NextFlux;
  FluxScalar*                  m_FluxSource;
  FluxScalar*                  m_NextFluxSource;
  FluxScalar*                  m_FluxSourceBaseline;
                   
  PotentialScalar*             m_PotentialSource;
  PotentialScalar*             m_NextPotentialSource;
  PotentialScalar*             m_PotentialSourceBaseline;
  PotentialScalar*             m_ValveBreakdownPotential;

  BlackBoxType*                m_BlackBox = nullptr;
};