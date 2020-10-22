/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "properties/SEScalar.h"

enum class eBlackBox_Property_Type {
  Provided = 0, Imposed
};
extern const std::string& eBlackBox_Property_Type_Name(eBlackBox_Property_Type m);

// Template types are all based on circuits (not compartments)

#define BLACK_BOX_TEMPLATE typename NodeType, typename PathType, \
                           typename FluxScalar, typename PotentialScalar, typename QuantityScalar, \
                           typename FluxUnit, typename PotentialUnit, typename QuantityUnit

#define BLACK_BOX_TYPES NodeType, PathType, \
                        FluxScalar, PotentialScalar, QuantityScalar, \
                        FluxUnit,PotentialUnit,QuantityUnit

#define ELECTRICAL_BLACK_BOX SEElectricalCircuitNode, SEElectricalCircuitPath, \
                             SEScalarElectricCurrent, SEScalarElectricPotential, SEScalarElectricCharge, \
                             ElectricCurrentUnit, ElectricPotentialUnit, ElectricChargeUnit

#define MAP_ELECTRICAL_BLACK_BOX SEElectricalBlackBox,SEElectricalCircuitNode, SEElectricalCircuitPath

#define FLUID_BLACK_BOX SEFluidCircuitNode, SEFluidCircuitPath, \
                        SEScalarVolumePerTime, SEScalarPressure, SEScalarVolume, \
                        VolumePerTimeUnit, PressureUnit, VolumeUnit

#define MAP_FLUID_BLACK_BOX SEFluidBlackBox,SEFluidCircuitNode, SEFluidCircuitPath

#define THERMAL_BLACK_BOX SEThermalCircuitNode, SEThermalCircuitPath, \
                          SEScalarPower, SEScalarTemperature, SEScalarEnergy, \
                          PowerUnit, TemperatureUnit, EnergyUnit

#define MAP_THERMAL_BLACK_BOX SEThermalBlackBox,SEThermalCircuitNode, SEThermalCircuitPath

#define CCT typename CT, typename NT, typename PT, typename BBT, \
            typename CU, typename FU, typename IU, \
            typename PU, typename QU, typename RU

template<BLACK_BOX_TEMPLATE>
class CDM_DECL SEBlackBox : public Loggable
{
  friend class SEBlackBoxManager;
  friend class CommonDataModelTest;
  template<CCT> friend class SECircuitCalculator;
protected:
  SEBlackBox(const std::string& name, Logger* logger) : Loggable(logger), m_Name(name) {}
public:
  virtual ~SEBlackBox() {}

  virtual void Clear() { }

  virtual std::string GetName() const { return m_Name; }

  virtual void ProvideSourceFlux() { m_SourceFlux = eBlackBox_Property_Type::Provided; }
  virtual bool IsSourceFluxImposed() const { return m_SourceFlux == eBlackBox_Property_Type::Imposed; }
  virtual void ImposeSourceFlux(double v, const FluxUnit& unit)
  {
    m_SourceFlux = eBlackBox_Property_Type::Imposed;
    ValueOverride(m_SourcePath->GetNextFlux(), v, unit);
  }
  virtual void ImposeSourceFlux(const FluxScalar& s)
  {
    m_SourceFlux = eBlackBox_Property_Type::Imposed;
    Override<FluxUnit>(s,m_SourcePath->GetNextFlux());
  }
  virtual double GetSourceFlux(const FluxUnit& unit) const { return m_SourcePath->GetNextFlux().GetValue(unit); }

  virtual void ProvideTargetFlux() { m_TargetFlux = eBlackBox_Property_Type::Provided; }
  virtual bool IsTargetFluxImposed() const { return m_TargetFlux == eBlackBox_Property_Type::Imposed; }
  virtual void ImposeTargetFlux(double v, const FluxUnit& unit)
  {
    m_TargetFlux = eBlackBox_Property_Type::Imposed;
    ValueOverride(m_TargetPath->GetNextFlux(), v, unit);
  }
  virtual void ImposeTargetFlux(const FluxScalar& s)
  {
    m_TargetFlux = eBlackBox_Property_Type::Imposed;
    Override<FluxUnit>(s, m_SourcePath->GetNextFlux());
  }
  virtual double GetTargetFlux(const FluxUnit& unit) const { return m_TargetPath->GetNextFlux().GetValue(unit); }

  virtual void ProvidePotential() { m_Potential = eBlackBox_Property_Type::Provided; }
  virtual bool IsPotentialImposed() const { return m_Potential == eBlackBox_Property_Type::Imposed; }
  virtual void ImposePotential(double v, const PotentialUnit& unit)
  {
    m_Potential = eBlackBox_Property_Type::Imposed;
    ValueOverride(m_Node->GetNextPotential(), v, unit);
  }
  virtual void ImposePotential(const PotentialScalar& s)
  {
    m_Potential = eBlackBox_Property_Type::Imposed;
    Override<PotentialUnit>(s, m_Node->GetNextPotential());
  }
  virtual double GetPotential(const PotentialUnit& unit) const { return m_Node->GetNextPotential().GetValue(unit); }

  virtual void ProvideSourcePotential() { m_SourcePotential = eBlackBox_Property_Type::Provided; }
  virtual bool IsSourcePotentialImposed() const { return m_SourcePotential == eBlackBox_Property_Type::Imposed; }
  virtual void ImposeSourcePotential(double v, const PotentialUnit& unit)
  {
    m_SourcePotential = eBlackBox_Property_Type::Imposed;
    ValueOverride(m_SourceNode->GetNextPotential(), v, unit);
  }
  virtual void ImposeSourcePotential(const PotentialScalar& s)
  {
    m_SourcePotential = eBlackBox_Property_Type::Imposed;
    Override<PotentialUnit>(s, m_SourceNode->GetNextPotential());
  }
  virtual double GetSourcePotential(const PotentialUnit& unit) const { return m_SourceNode->GetNextPotential().GetValue(unit); }

  virtual void ProvideTargetPotential() { m_TargetPotential = eBlackBox_Property_Type::Provided; }
  virtual bool IsTargetPotentialImposed() const { return m_TargetPotential == eBlackBox_Property_Type::Imposed; }
  virtual void ImposeTargetPotential(double v, const PotentialUnit& unit)
  {
    m_TargetPotential = eBlackBox_Property_Type::Imposed;
    ValueOverride(m_TargetNode->GetNextPotential(), v, unit);
  }
  virtual void ImposeTargetPotential(const PotentialScalar& s)
  {
    m_TargetPotential = eBlackBox_Property_Type::Imposed;
    Override<PotentialUnit>(s, m_TargetNode->GetNextPotential());
  }
  virtual double GetTargetPotential(const PotentialUnit& unit) const { return m_TargetNode->GetNextPotential().GetValue(unit); }

  virtual void ProvideQuantity() { m_Quantity = eBlackBox_Property_Type::Provided; }
  virtual bool IsQuantityImposed() const { return m_Quantity == eBlackBox_Property_Type::Imposed; }
  virtual void ImposeQuantity(double v, const QuantityUnit& unit)
  {
    m_Quantity = eBlackBox_Property_Type::Imposed;
    ValueOverride(m_SourceNode->GetNextQuantity(), v, unit);
  }
  virtual void ImposeQuantity(const QuantityScalar& s)
  {
    m_Quantity = eBlackBox_Property_Type::Imposed;
    Override<QuantityUnit>(s, m_SourceNode->GetNextQuantity());
  }
  virtual double GetQuantity(const QuantityUnit& unit) const { return m_Node->GetNextQuantity().GetValue(unit); }

protected:
  // Circuit Elements
  virtual bool HasNode() const { return m_Node != nullptr; }
  virtual NodeType* GetNode() const { return m_Node; }
  virtual void SetNode(NodeType* n) { m_Node = n; }

  virtual bool HasSourceNode() const { return m_SourceNode != nullptr; }
  virtual NodeType* GetSourceNode() const { return m_SourceNode; }
  virtual void SetSourceNode(NodeType* n) { m_SourceNode = n; }

  virtual bool HasTargetNode() const { return m_TargetNode != nullptr; }
  virtual NodeType* GetTargetNode() const { return m_TargetNode; }
  virtual void SetTargetNode(NodeType* n) { m_TargetNode = n; }

  virtual bool HasSourcePath() const { return m_SourcePath != nullptr; }
  virtual PathType* GetSourcePath() const { return m_SourcePath; }
  virtual void SetSourcePath(PathType* p) { m_SourcePath = p; }

  virtual bool HasTargetPath() const { return m_TargetPath != nullptr; }
  virtual PathType* GetTargetPath() const { return m_TargetPath; }
  virtual void SetTargetPath(PathType* p) { m_TargetPath = p; }

  std::string      m_Name;
  // Circuit Elements
  NodeType*        m_Node = nullptr;
  NodeType*        m_SourceNode = nullptr;
  NodeType*        m_TargetNode = nullptr;
  PathType*        m_SourcePath = nullptr;
  PathType*        m_TargetPath = nullptr;
  // Flags
  eBlackBox_Property_Type m_SourceFlux = eBlackBox_Property_Type::Provided;
  eBlackBox_Property_Type m_TargetFlux = eBlackBox_Property_Type::Provided;
  eBlackBox_Property_Type m_SourcePotential = eBlackBox_Property_Type::Provided;
  eBlackBox_Property_Type m_TargetPotential = eBlackBox_Property_Type::Provided;
  eBlackBox_Property_Type m_Potential = eBlackBox_Property_Type::Provided;
  eBlackBox_Property_Type m_Quantity = eBlackBox_Property_Type::Provided;
};
