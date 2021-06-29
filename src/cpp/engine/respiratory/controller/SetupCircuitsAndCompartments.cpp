/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/respiratory/controller/Controller.h"
#include "cpm/PulseConfiguration.h"
#include "cpm/controller/Circuits.h"
#include "cpm/controller/Compartments.h"
#include "cpm/controller/Substances.h"
#include "cpm/environment/Environment.h"

#include "cdm/system/environment/SEEnvironmentalConditions.h"

#include "cdm/circuit/SECircuitManager.h"
#include "cdm/circuit/fluid/SEFluidCircuit.h"
#include "cdm/circuit/fluid/SEFluidCircuitNode.h"
#include "cdm/circuit/fluid/SEFluidCircuitPath.h"

#include "cdm/compartment/fluid/SEGasCompartment.h"
#include "cdm/compartment/fluid/SEGasCompartmentLink.h"
#include "cdm/compartment/fluid/SEGasCompartmentGraph.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartmentLink.h"
#include "cdm/compartment/fluid/SELiquidCompartmentGraph.h"

bool Controller::CreateCircuitsAndCompartments()
{
  m_Circuits->Clear();
  m_Compartments->Clear();
  m_Compartments->Setup();

  // This node is shared between air based circuits
  SEFluidCircuitNode& Ambient = m_Circuits->CreateFluidNode(pulse::EnvironmentNode::Ambient);
  Ambient.GetNextVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  Ambient.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  SEGasCompartment& gEnvironment = m_Compartments->CreateGasCompartment(pulse::EnvironmentCompartment::Ambient);
  gEnvironment.MapNode(Ambient);
  SELiquidCompartment& lEnvironment = m_Compartments->CreateLiquidCompartment(pulse::EnvironmentCompartment::Ambient);
  lEnvironment.MapNode(Ambient);
  Ambient.SetAsReferenceNode();

  m_Environment->Clear();
  m_Environment->Initialize();

  // Below is what we need to initialize the system if we don't have substances
  // But Respiratory::TuneCircuit still assumes substances are in the system
  //double atmPressure_cmH20 = 1033.23;// 1 atm
  //if (GetConfiguration().HasInitialEnvironmentalConditions() && GetConfiguration().GetInitialEnvironmentalConditions()->HasAtmosphericPressure())
  //  atmPressure_cmH20 = GetConfiguration().GetInitialEnvironmentalConditions()->GetAtmosphericPressure(PressureUnit::cmH2O);
  //gEnvironment.GetPressure().SetValue(atmPressure_cmH20, PressureUnit::cmH2O);

  SetupRespiratory();
  SetupMechanicalVentilator();

  // TODO We might want to update state change to only check compartments we should have
  // Not sure how I would update this specify which compartments we expect to have in our engine
  m_Compartments->StateChange();
  return true;
}
