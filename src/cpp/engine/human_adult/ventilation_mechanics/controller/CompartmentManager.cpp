/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/ventilation_mechanics/controller/CompartmentManager.h"

#include "cdm/compartment/fluid/SEGasCompartment.h"
#include "cdm/compartment/fluid/SEGasCompartmentLink.h"
#include "cdm/compartment/fluid/SEGasCompartmentGraph.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarAmountPerVolume.h"

namespace pulse { namespace human_adult_ventilation_mechanics
{
  CompartmentManager::CompartmentManager(pulse::Data& data) : pulse::CompartmentManager(data)
  {
    Clear();
  }

  CompartmentManager::~CompartmentManager()
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
  void CompartmentManager::StateChange()
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
END_NAMESPACE_EX
