/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarPressureTimeSquaredPerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"

SEFluidCircuitPath::SEFluidCircuitPath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name) :
  SECircuitPath<FLUID_CIRCUIT_PATH>(src, tgt, name),
  m_FluidSourceNode(src), m_FluidTargetNode(tgt)
{

}
SEFluidCircuitPath::~SEFluidCircuitPath()
{
  Clear();
}

void SEFluidCircuitPath::Clear()
{
  SECircuitPath::Clear();
}

////////////////////////////////
// Fluid Resistance Types//
////////////////////////////////

bool SEFluidCircuitPath::HasResistance() const
{
  return SECircuitPath::HasResistance();
}
SEScalarPressureTimePerVolume& SEFluidCircuitPath::GetResistance()
{
  return SECircuitPath::GetResistance();
}
double SEFluidCircuitPath::GetResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_Resistance == nullptr)
    return SEScalar::dNaN();
  return m_Resistance->GetValue(unit);
}
bool SEFluidCircuitPath::HasNextResistance() const
{
  return SECircuitPath::HasNextResistance();
}
SEScalarPressureTimePerVolume& SEFluidCircuitPath::GetNextResistance()
{
  return SECircuitPath::GetNextResistance();
}
double SEFluidCircuitPath::GetNextResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_NextResistance == nullptr)
    return SEScalar::dNaN();
  return m_NextResistance->GetValue(unit);
}
bool SEFluidCircuitPath::HasResistanceBaseline() const
{
  return SECircuitPath::HasResistanceBaseline();
}
SEScalarPressureTimePerVolume& SEFluidCircuitPath::GetResistanceBaseline()
{
  return SECircuitPath::GetResistanceBaseline();
}
double SEFluidCircuitPath::GetResistanceBaseline(const PressureTimePerVolumeUnit& unit) const
{
  if (m_ResistanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceBaseline->GetValue(unit);
}

//////////////////////////////////
// Fluid Capacitance Types //
//////////////////////////////////

bool SEFluidCircuitPath::HasCompliance() const
{
  return HasCapacitance();
}
SEScalarVolumePerPressure& SEFluidCircuitPath::GetCompliance()
{
  return GetCapacitance();
}
double SEFluidCircuitPath::GetCompliance(const VolumePerPressureUnit& unit) const
{
  if (m_Capacitance == nullptr)
    return SEScalar::dNaN();
  return m_Capacitance->GetValue(unit);
}
bool SEFluidCircuitPath::HasNextCompliance() const
{
  return HasNextCapacitance();
}
SEScalarVolumePerPressure& SEFluidCircuitPath::GetNextCompliance()
{
  return GetNextCapacitance();
}
double SEFluidCircuitPath::GetNextCompliance(const VolumePerPressureUnit& unit) const
{
  if (m_NextCapacitance == nullptr)
    return SEScalar::dNaN();
  return m_NextCapacitance->GetValue(unit);
}
bool SEFluidCircuitPath::HasComplianceBaseline() const
{
  return HasCapacitanceBaseline();
}
SEScalarVolumePerPressure& SEFluidCircuitPath::GetComplianceBaseline()
{
  return GetCapacitanceBaseline();
}
double SEFluidCircuitPath::GetComplianceBaseline(const VolumePerPressureUnit& unit) const
{
  if (m_CapacitanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_CapacitanceBaseline->GetValue(unit);
}

/////////////////////////////////
// Fluid Inductance Types //
/////////////////////////////////

bool SEFluidCircuitPath::HasInertance() const
{
  return HasInductance();
}
SEScalarPressureTimeSquaredPerVolume& SEFluidCircuitPath::GetInertance()
{
  return GetInductance();
}
double SEFluidCircuitPath::GetInertance(const PressureTimeSquaredPerVolumeUnit& unit) const
{
  if (m_Inductance == nullptr)
    return SEScalar::dNaN();
  return m_Inductance->GetValue(unit);
}
bool SEFluidCircuitPath::HasNextInertance() const
{
  return HasNextInductance();
}
SEScalarPressureTimeSquaredPerVolume& SEFluidCircuitPath::GetNextInertance()
{
  return GetNextInductance();
}
double SEFluidCircuitPath::GetNextInertance(const PressureTimeSquaredPerVolumeUnit& unit) const
{
  if (m_NextInductance == nullptr)
    return SEScalar::dNaN();
  return m_NextInductance->GetValue(unit);
}
bool SEFluidCircuitPath::HasInertanceBaseline() const
{
  return HasInductanceBaseline();
}
SEScalarPressureTimeSquaredPerVolume& SEFluidCircuitPath::GetInertanceBaseline()
{
  return GetInductanceBaseline();
}
double SEFluidCircuitPath::GetInertanceBaseline(const PressureTimeSquaredPerVolumeUnit& unit) const
{
  if (m_InductanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_InductanceBaseline->GetValue(unit);
}


///////////////////////////
// Fluid Flux Types //
///////////////////////////
bool SEFluidCircuitPath::HasFlow() const
{
  return HasFlux();
}
SEScalarVolumePerTime& SEFluidCircuitPath::GetFlow()
{
  return GetFlux();
}
double SEFluidCircuitPath::GetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Flux == nullptr)
    return SEScalar::dNaN();
  return m_Flux->GetValue(unit);
}
bool SEFluidCircuitPath::HasNextFlow() const
{
  return HasNextFlux();
}
SEScalarVolumePerTime& SEFluidCircuitPath::GetNextFlow()
{
  return GetNextFlux();
}
double SEFluidCircuitPath::GetNextFlow(const VolumePerTimeUnit& unit) const
{
  if (m_NextFlux == nullptr)
    return SEScalar::dNaN();
  return m_NextFlux->GetValue(unit);
}
bool SEFluidCircuitPath::HasFlowSource() const
{
  return HasFluxSource();
}
SEScalarVolumePerTime& SEFluidCircuitPath::GetFlowSource()
{
  return GetFluxSource();
}
double SEFluidCircuitPath::GetFlowSource(const VolumePerTimeUnit& unit) const
{
  if (m_FluxSource == nullptr)
    return SEScalar::dNaN();
  return m_FluxSource->GetValue(unit);
}
bool SEFluidCircuitPath::HasNextFlowSource() const
{
  return HasNextFluxSource();
}
SEScalarVolumePerTime& SEFluidCircuitPath::GetNextFlowSource()
{
  return GetNextFluxSource();
}
double SEFluidCircuitPath::GetNextFlowSource(const VolumePerTimeUnit& unit) const
{
  if (m_NextFluxSource == nullptr)
    return SEScalar::dNaN();
  return m_NextFluxSource->GetValue(unit);
}
bool SEFluidCircuitPath::HasFlowSourceBaseline() const
{
  return HasFluxSourceBaseline();
}
SEScalarVolumePerTime& SEFluidCircuitPath::GetFlowSourceBaseline()
{
  return GetFluxSourceBaseline();
}
double SEFluidCircuitPath::GetFlowSourceBaseline(const VolumePerTimeUnit& unit) const
{
  if (m_FluxSourceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_FluxSourceBaseline->GetValue(unit);
}

////////////////////////////////
// Fluid Potential Types //
////////////////////////////////
bool SEFluidCircuitPath::HasPressureSource() const
{
  return HasPotentialSource();
}
SEScalarPressure& SEFluidCircuitPath::GetPressureSource()
{
  return GetPotentialSource();
}
double SEFluidCircuitPath::GetPressureSource(const PressureUnit& unit) const
{
  if (m_PotentialSource == nullptr)
    return SEScalar::dNaN();
  return m_PotentialSource->GetValue(unit);
}
bool SEFluidCircuitPath::HasNextPressureSource() const
{
  return HasNextPotentialSource();
}
SEScalarPressure& SEFluidCircuitPath::GetNextPressureSource()
{
  return GetNextPotentialSource();
}
double SEFluidCircuitPath::GetNextPressureSource(const PressureUnit& unit) const
{
  if (m_NextPotentialSource == nullptr)
    return SEScalar::dNaN();
  return m_NextPotentialSource->GetValue(unit);
}
bool SEFluidCircuitPath::HasPressureSourceBaseline() const
{
  return HasPotentialSourceBaseline();
}
SEScalarPressure& SEFluidCircuitPath::GetPressureSourceBaseline()
{
  return GetPotentialSourceBaseline();
}
double SEFluidCircuitPath::GetPressureSourceBaseline(const PressureUnit& unit) const
{
  if (m_PotentialSourceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_PotentialSourceBaseline->GetValue(unit);
}
bool SEFluidCircuitPath::HasValveBreakdownPressure() const
{
  return HasValveBreakdownPotential();
}
SEScalarPressure& SEFluidCircuitPath::GetValveBreakdownPressure()
{
  return GetValveBreakdownPotential();
}
double SEFluidCircuitPath::GetValveBreakdownPressure(const PressureUnit& unit) const
{
  if (m_ValveBreakdownPotential == nullptr)
    return SEScalar::dNaN();
  return m_ValveBreakdownPotential->GetValue(unit);
}
