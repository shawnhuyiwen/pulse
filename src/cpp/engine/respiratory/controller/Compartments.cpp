/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulseConfiguration.h"
#include "controller/Controller.h"
#include "controller/Compartments.h"
#include "cpm/controller/Substances.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "substance/SESubstance.h"

#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarAmountPerVolume.h"

Compartments::Compartments(PulseData& data) : PulseCompartments(data)
{
  Clear();
}

Compartments::~Compartments()
{
  Clear();
}

#define SORT_CMPTS(bin, type) \
m_##bin##Compartments.clear(); \
m_##bin##LeafCompartments.clear(); \
for (const std::string& name : pulse::bin##Compartment::GetValues()) \
{ \
  SE##type##Compartment* cmpt = Get##type##Compartment(name); \
  if (cmpt == nullptr) \
  { \
    Warning("Could not find expected " + std::string(#bin) + " compartment, " + name + " in compartment manager"); \
    continue; \
  } \
  m_##bin##Compartments.push_back(cmpt); \
  if (!cmpt->HasChildren()) \
    m_##bin##LeafCompartments.push_back(cmpt); \
} 
void Compartments::StateChange()
{
  SECompartmentManager::StateChange();

  SORT_CMPTS(Pulmonary, Gas);
  SORT_CMPTS(MechanicalVentilator, Gas);

  m_AerosolCompartments.clear();
  m_AerosolLeafCompartments.clear();
  for (const std::string& name : pulse::PulmonaryCompartment::GetValues())
  {
    SELiquidCompartment* cmpt = GetLiquidCompartment(name);
    if (cmpt == nullptr) 
    { 
      Warning("Could not find expected Aerosol compartment, " + name + " in compartment manager"); 
      continue; 
    } 
    m_AerosolCompartments.push_back(cmpt);
    if (!cmpt->HasChildren()) 
      m_AerosolLeafCompartments.push_back(cmpt);
  }

  m_MechanicalVentilatorAerosolCompartments.clear();
  m_MechanicalVentilatorAerosolLeafCompartments.clear();
  for (const std::string& name : pulse::MechanicalVentilatorCompartment::GetValues())
  {
    SELiquidCompartment* cmpt = GetLiquidCompartment(name);
    if (cmpt == nullptr)
    {
      Warning("Could not find expected Aerosol compartment, " + name + " in compartment manager");
      continue;
    }
    m_MechanicalVentilatorAerosolCompartments.push_back(cmpt);
    if (!cmpt->HasChildren())
      m_MechanicalVentilatorAerosolLeafCompartments.push_back(cmpt);
  }

  m_RespiratoryGraph = GetGasGraph(pulse::Graph::Respiratory);
  if (m_RespiratoryGraph == nullptr)
  {
    Error("Could not find required Graph " + std::string(pulse::Graph::Respiratory));
  }

  m_AerosolGraph = GetLiquidGraph(pulse::Graph::Aerosol);
  if (m_AerosolGraph == nullptr)
  {
    Error("Could not find required Graph " + std::string(pulse::Graph::Aerosol));
  }

  m_CombinedRespiratoryMechanicalVentilatorGraph = GetGasGraph(pulse::Graph::RespiratoryAndMechanicalVentilator);
  if (m_CombinedRespiratoryMechanicalVentilatorGraph == nullptr)
  {
    Error("Could not find required Graph " + std::string(pulse::Graph::RespiratoryAndMechanicalVentilator));
  }

  m_CombinedAerosolMechanicalVentilatorGraph = GetLiquidGraph(pulse::Graph::AerosolAndMechanicalVentilator);
  if (m_CombinedAerosolMechanicalVentilatorGraph == nullptr)
  {
    Error("Could not find required Graph " + std::string(pulse::Graph::AerosolAndMechanicalVentilator));
  }
}
