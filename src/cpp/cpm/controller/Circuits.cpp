/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "controller/Circuits.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"

PulseCircuits::PulseCircuits(PulseData& data) : SECircuitManager(data.GetLogger()), m_data(data)
{
  Clear();
}

PulseCircuits::~PulseCircuits()
{
  Clear();
}

void PulseCircuits::Clear()
{
  SECircuitManager::Clear(); 
  m_CombinedCardiovascularCircuit = nullptr;
  m_CardiovascularCircuit = nullptr;
  m_RenalCircuit = nullptr;
  m_RespiratoryCircuit = nullptr;
  m_AnesthesiaMachineCircuit = nullptr;
  m_MechanicalVentilatorCircuit = nullptr;
  m_CombinedRespiratoryAnesthesiaCircuit = nullptr;
  m_CombinedRespiratoryInhalerCircuit = nullptr;
  m_CombinedRespiratoryNasalCannulaCircuit = nullptr;
  m_CombinedRespiratorySimpleMaskCircuit = nullptr;
  m_CombinedRespiratoryNonRebreatherMaskCircuit = nullptr;
  m_CombinedRespiratoryMechanicalVentilationCircuit = nullptr;
  m_CombinedRespiratoryMechanicalVentilatorCircuit = nullptr;
  m_TemperatureCircuit = nullptr;
  m_InternalTemperatureCircuit = nullptr;
  m_ExternalTemperatureCircuit = nullptr;
}

void PulseCircuits::StateChange()
{
  m_CombinedCardiovascularCircuit = GetFluidCircuit(pulse::Circuits::FullCardiovascular);
  if (m_CombinedCardiovascularCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::FullCardiovascular));
  }
  m_CardiovascularCircuit = GetFluidCircuit(pulse::Circuits::Cardiovascular);
  if (m_CardiovascularCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::Cardiovascular));
  }
  m_RenalCircuit = GetFluidCircuit(pulse::Circuits::Renal);
  if (m_RenalCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::Renal));
  }
  m_RespiratoryCircuit = GetFluidCircuit(pulse::Circuits::Respiratory);
  if (m_RespiratoryCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::Respiratory));
  }
  m_AnesthesiaMachineCircuit = GetFluidCircuit(pulse::Circuits::AnesthesiaMachine);
  if (m_AnesthesiaMachineCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::AnesthesiaMachine));
  }
  m_MechanicalVentilatorCircuit = GetFluidCircuit(pulse::Circuits::MechanicalVentilator);
  if (m_MechanicalVentilatorCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::MechanicalVentilator));
  }
  m_CombinedRespiratoryAnesthesiaCircuit = GetFluidCircuit(pulse::Circuits::RespiratoryAnesthesia);
  if (m_CombinedRespiratoryAnesthesiaCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::RespiratoryAnesthesia));
  }
  m_CombinedRespiratoryInhalerCircuit = GetFluidCircuit(pulse::Circuits::RespiratoryInhaler);
  if (m_CombinedRespiratoryInhalerCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::RespiratoryInhaler));
  }
  m_CombinedRespiratoryMechanicalVentilationCircuit = GetFluidCircuit(pulse::Circuits::RespiratoryMechanicalVentilation);
  if (m_CombinedRespiratoryMechanicalVentilationCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::RespiratoryMechanicalVentilation));
  }
  m_CombinedRespiratoryMechanicalVentilatorCircuit = GetFluidCircuit(pulse::Circuits::RespiratoryMechanicalVentilator);
  if (m_CombinedRespiratoryMechanicalVentilatorCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::RespiratoryMechanicalVentilator));
  }
  m_CombinedRespiratoryNasalCannulaCircuit = GetFluidCircuit(pulse::Circuits::RespiratoryNasalCannula);
  if (m_CombinedRespiratoryNasalCannulaCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::RespiratoryNasalCannula));
  }
  m_CombinedRespiratoryNonRebreatherMaskCircuit = GetFluidCircuit(pulse::Circuits::RespiratoryNonRebreatherMask);
  if (m_CombinedRespiratoryNonRebreatherMaskCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::RespiratoryNonRebreatherMask));
  }
  m_CombinedRespiratorySimpleMaskCircuit = GetFluidCircuit(pulse::Circuits::RespiratorySimpleMask);
  if (m_CombinedRespiratorySimpleMaskCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::RespiratorySimpleMask));
  }
  m_TemperatureCircuit = GetThermalCircuit(pulse::Circuits::Temperature);
  if (m_TemperatureCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::Temperature));
  }
  m_InternalTemperatureCircuit = GetThermalCircuit(pulse::Circuits::InternalTemperature);
  if (m_InternalTemperatureCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::InternalTemperature));
  }
  m_ExternalTemperatureCircuit = GetThermalCircuit(pulse::Circuits::ExternalTemperature);
  if (m_ExternalTemperatureCircuit == nullptr)
  {
    Error("Could not find circuit : " + std::string(pulse::Circuits::ExternalTemperature));
  }
}  

void PulseCircuits::SetReadOnlyFluid(bool b)
{
  SECircuitManager::SetReadOnlyFluid(b);
  // These don't need to be locked
  if (GetFluidNode(pulse::EnvironmentNode::Ambient) != nullptr)
  {
    GetFluidNode(pulse::EnvironmentNode::Ambient)->GetPressure().SetReadOnly(false);
    GetFluidNode(pulse::EnvironmentNode::Ambient)->GetNextPressure().SetReadOnly(false);
  }
  if (GetFluidNode(pulse::RespiratoryNode::Stomach) != nullptr)
  {
    GetFluidNode(pulse::RespiratoryNode::Stomach)->GetPressure().SetReadOnly(false);
    GetFluidNode(pulse::RespiratoryNode::Stomach)->GetNextPressure().SetReadOnly(false);
  }
  if (GetFluidNode(pulse::InhalerNode::Mouthpiece) != nullptr)
  {
    GetFluidNode(pulse::InhalerNode::Mouthpiece)->GetPressure().SetReadOnly(false);
    GetFluidNode(pulse::InhalerNode::Mouthpiece)->GetNextPressure().SetReadOnly(false);
  }
}

SEFluidCircuit& PulseCircuits::GetActiveCardiovascularCircuit()
{
  if (m_CombinedCardiovascularCircuit == nullptr)
    m_CombinedCardiovascularCircuit = &CreateFluidCircuit(pulse::Circuits::FullCardiovascular);
  return *m_CombinedCardiovascularCircuit;
}
SEFluidCircuit& PulseCircuits::GetCardiovascularCircuit()
{
  if (m_CardiovascularCircuit == nullptr)
    m_CardiovascularCircuit = &CreateFluidCircuit(pulse::Circuits::Cardiovascular);
  return *m_CardiovascularCircuit;
}
SEFluidCircuit& PulseCircuits::GetRenalCircuit()
{
  if (m_RenalCircuit == nullptr)
    m_RenalCircuit = &CreateFluidCircuit(pulse::Circuits::Renal);
  return *m_RenalCircuit;
}

SEFluidCircuit& PulseCircuits::GetActiveRespiratoryCircuit()
{
  switch (m_data.GetAirwayMode())
  {
  case eAirwayMode::Free:
    return *m_RespiratoryCircuit;
  case eAirwayMode::AnesthesiaMachine:
    return *m_CombinedRespiratoryAnesthesiaCircuit;
  case eAirwayMode::Inhaler:
    return *m_CombinedRespiratoryInhalerCircuit;
  case eAirwayMode::MechanicalVentilation:
    return *m_CombinedRespiratoryMechanicalVentilationCircuit;
  case eAirwayMode::MechanicalVentilator:
    return *m_CombinedRespiratoryMechanicalVentilatorCircuit;
  case eAirwayMode::NasalCannula:
    return *m_CombinedRespiratoryNasalCannulaCircuit;
  case eAirwayMode::NonRebreatherMask:
    return *m_CombinedRespiratoryNonRebreatherMaskCircuit;
  case eAirwayMode::SimpleMask:
    return *m_CombinedRespiratorySimpleMaskCircuit;
  default:
    throw CommonDataModelException("Unknown airway mode");
  }
}
SEFluidCircuit& PulseCircuits::GetRespiratoryCircuit()
{
  if (m_RespiratoryCircuit == nullptr)
    m_RespiratoryCircuit = &CreateFluidCircuit(pulse::Circuits::Respiratory);
  return *m_RespiratoryCircuit;
}
SEFluidCircuit& PulseCircuits::GetAnesthesiaMachineCircuit()
{
  if (m_AnesthesiaMachineCircuit == nullptr)
    m_AnesthesiaMachineCircuit = &CreateFluidCircuit(pulse::Circuits::AnesthesiaMachine);
  return *m_AnesthesiaMachineCircuit;
}
SEFluidCircuit& PulseCircuits::GetMechanicalVentilatorCircuit()
{
  if (m_MechanicalVentilatorCircuit == nullptr)
    m_MechanicalVentilatorCircuit = &CreateFluidCircuit(pulse::Circuits::MechanicalVentilator);
  return *m_MechanicalVentilatorCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndAnesthesiaMachineCircuit()
{
  if (m_CombinedRespiratoryAnesthesiaCircuit == nullptr)
    m_CombinedRespiratoryAnesthesiaCircuit = &CreateFluidCircuit(pulse::Circuits::RespiratoryAnesthesia);
  return *m_CombinedRespiratoryAnesthesiaCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndInhalerCircuit()
{
  if (m_CombinedRespiratoryInhalerCircuit == nullptr)
    m_CombinedRespiratoryInhalerCircuit = &CreateFluidCircuit(pulse::Circuits::RespiratoryInhaler);
  return *m_CombinedRespiratoryInhalerCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndMechanicalVentilationCircuit()
{
  if (m_CombinedRespiratoryMechanicalVentilationCircuit == nullptr)
    m_CombinedRespiratoryMechanicalVentilationCircuit = &CreateFluidCircuit(pulse::Circuits::RespiratoryMechanicalVentilation);
  return *m_CombinedRespiratoryMechanicalVentilationCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndMechanicalVentilatorCircuit()
{
  if (m_CombinedRespiratoryMechanicalVentilatorCircuit == nullptr)
    m_CombinedRespiratoryMechanicalVentilatorCircuit = &CreateFluidCircuit(pulse::Circuits::RespiratoryMechanicalVentilator);
  return *m_CombinedRespiratoryMechanicalVentilatorCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndNasalCannulaCircuit()
{
  if (m_CombinedRespiratoryNasalCannulaCircuit == nullptr)
    m_CombinedRespiratoryNasalCannulaCircuit = &CreateFluidCircuit(pulse::Circuits::RespiratoryNasalCannula);
  return *m_CombinedRespiratoryNasalCannulaCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndNonRebreatherMaskCircuit()
{
  if (m_CombinedRespiratoryNonRebreatherMaskCircuit == nullptr)
    m_CombinedRespiratoryNonRebreatherMaskCircuit = &CreateFluidCircuit(pulse::Circuits::RespiratoryNonRebreatherMask);
  return *m_CombinedRespiratoryNonRebreatherMaskCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndSimpleMaskCircuit()
{
  if (m_CombinedRespiratorySimpleMaskCircuit == nullptr)
    m_CombinedRespiratorySimpleMaskCircuit = &CreateFluidCircuit(pulse::Circuits::RespiratorySimpleMask);
  return *m_CombinedRespiratorySimpleMaskCircuit;
}

SEThermalCircuit& PulseCircuits::GetTemperatureCircuit()
{
  if (m_TemperatureCircuit == nullptr)
    m_TemperatureCircuit = &CreateThermalCircuit(pulse::Circuits::Temperature);
  return *m_TemperatureCircuit;
}
SEThermalCircuit& PulseCircuits::GetInternalTemperatureCircuit()
{
  if (m_InternalTemperatureCircuit == nullptr)
    m_InternalTemperatureCircuit = &CreateThermalCircuit(pulse::Circuits::InternalTemperature);
  return *m_InternalTemperatureCircuit;
}
SEThermalCircuit& PulseCircuits::GetExternalTemperatureCircuit()
{
  if (m_ExternalTemperatureCircuit == nullptr)
    m_ExternalTemperatureCircuit = &CreateThermalCircuit(pulse::Circuits::ExternalTemperature);
  return *m_ExternalTemperatureCircuit;
}
