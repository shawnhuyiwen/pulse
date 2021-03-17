/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cpm/bind/PulseState.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBPulseState.h"
#include "io/protobuf/PBPulseConfiguration.h"
#include "io/protobuf/PBPulsePhysiology.h"
#include "io/protobuf/PBPulseEnvironment.h"
#include "io/protobuf/PBPulseEquipment.h"
#include "io/protobuf/PBCircuit.h"
#include "io/protobuf/PBCompartment.h"
#include "io/protobuf/PBSubstance.h"
#include "io/protobuf/PBPatient.h"
#include "io/protobuf/PBConditions.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBEngine.h"
#include "io/protobuf/PBCircuit.h"
#include "io/protobuf/PBCompartment.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "PulseConfiguration.h"
#include "controller/Engine.h"
#include "controller/Substances.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "physiology/Saturation.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceCompound.h"
#include "engine/SEEngineTracker.h"
#include "engine/SECondition.h"
#include "engine/SEConditionManager.h"
#include "engine/SEAction.h"
#include "engine/SEActionManager.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEEventManager.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"


void PBPulseState::Load(const PULSE_BIND::StateData& src, PulseController& dst, const SEEngineConfiguration* config)
{
  PBPulseState::Serialize(src, dst,config);
}
bool PBPulseState::Serialize(const PULSE_BIND::StateData& src, PulseController& dst, const SEEngineConfiguration* config)
{
  std::stringstream ss;
  dst.m_State = EngineState::NotReady;
  // First Get the substances reset and ready
  dst.m_Substances->Clear();
  // Load up our Substances //
  for (int i = 0; i < src.substance_size(); i++)
  {
    const CDM_BIND::SubstanceData& subData = src.substance()[i];
    SESubstance* sub = dst.m_Substances->GetSubstance(subData.name());
    PBSubstance::Load(subData, *sub);
  }
  for (int i = 0; i < src.activesubstance_size(); i++)
  {
    SESubstance* sub = dst.m_Substances->GetSubstance(src.activesubstance()[i]);
    if (sub == nullptr)
    {
      dst.Error("Active substance not found : " + src.activesubstance()[i]);
      return false;
    }
    dst.m_Substances->AddActiveSubstance(*sub);
  }
  // Load up our Compounds //
  for (int i = 0; i < src.compound_size(); i++)
  {
    const CDM_BIND::SubstanceCompoundData& cmpdData = src.compound()[i];
    SESubstanceCompound* cmpd = dst.m_Substances->GetCompound(cmpdData.name());
    PBSubstance::Load(cmpdData, *cmpd, *dst.m_Substances);
  }
  for (int i = 0; i < src.activecompound_size(); i++)
  {
    SESubstanceCompound* cmpd = dst.m_Substances->GetCompound(src.activecompound()[i]);
    if (cmpd == nullptr)
    {
      dst.Error("Active substance compound not found : " + src.activecompound()[i]);
      return false;
    }
    dst.m_Substances->AddActiveCompound(*cmpd);
  }
  if (!dst.m_Substances->Setup())
    return false;
  if(!dst.m_SaturationCalculator->Setup())
    return false;

  // We could preserve the tracker, but I think I want to force the user to set it up
  // again, they should have the data tracks (or easily get them), and they should
  // Set it back up, and set or reset the results file they are using
  if (src.has_datarequestmanager())
  {
    dst.m_EngineTrack->GetDataRequestManager().Clear();
    PBEngine::Load(src.datarequestmanager(), dst.m_EngineTrack->GetDataRequestManager(), *dst.m_Substances);
    dst.m_EngineTrack->ForceConnection();// I don't want to rest the file because I would loose all my data
  }

  if (src.has_simulationtime())
  {
    PBProperty::Load(src.simulationtime(), dst.m_CurrentTime);
    PBProperty::Load(src.simulationtime(), dst.m_SimulationTime);
  }
  else
  {
    dst.m_CurrentTime.SetValue(0, TimeUnit::s);
    dst.m_SimulationTime.SetValue(0, TimeUnit::s);
  }

  dst.m_AirwayMode = (eAirwayMode)src.airwaymode();
  if (src.intubation() == (CDM_BIND::eSwitch)eSwitch::NullSwitch)
    ss << "Pulse State must have none null intubation state";
  dst.m_Intubation = (eSwitch)src.intubation();

  /// Patient //
  if (!src.has_currentpatient() || !src.has_initialpatient())
    ss << "PulseState must have a patient" << std::endl;
  else
  {
    PBPatient::Load(src.currentpatient(), *dst.m_CurrentPatient);
    PBPatient::Load(src.initialpatient(), *dst.m_InitialPatient);
  }
  // Conditions //
  dst.m_Conditions->Clear();
  if (src.has_conditions())
  {
    for (int i = 0; i < src.conditions().anycondition_size(); i++)
    {
      SECondition* c = PBCondition::Load(src.conditions().anycondition()[i], *dst.m_Substances);
      dst.m_Conditions->Copy(*c, *dst.m_Substances);
      delete c;
    }
  }
  // Actions //
  dst.m_Actions->Clear();
  if (src.has_activeactions())
  {
    for (int i = 0; i < src.activeactions().anyaction_size(); i++)
    {
      SEAction* a = PBAction::Load(src.activeactions().anyaction()[i], *dst.m_Substances);
      dst.m_Actions->ProcessAction(*a);
      delete a;
    }
  }
  // Active Events //
  dst.m_EventManager->Clear();
  if (src.has_activeevents())
  {
    SEScalarTime time;
    for (int i = 0; i < src.activeevents().activeevent_size(); i++)
    {
      const CDM_BIND::ActiveEventData& e = src.activeevents().activeevent()[i];
      if (e.has_duration())
        PBProperty::Load(e.duration(), time);
      else
      {
        dst.m_ss << "Active event " << CDM_BIND::eEvent_Name(e.event()) << " does not have time associated with it";
        dst.Debug(dst.m_ss);
        time.SetValue(0, TimeUnit::s);
      }
      dst.m_EventManager->OverrideActiveState((eEvent)e.event(), time);
    }
  }

  // Circuit Manager //
  if (!src.has_circuitmanager())
    ss << "PulseState must have a circuit manager" << std::endl;
  else
    PBCircuit::Load(src.circuitmanager(), *dst.m_Circuits);
  // Compartment Manager //
  if (!src.has_compartmentmanager())
    ss << "PulseState must have a compartment manager" << std::endl;
  else
    PBCompartment::Load(src.compartmentmanager(), *dst.m_Compartments, dst.m_Circuits);
  // Configuration //
  if (!src.has_configuration())
    ss << "PulseState must have a configuration" << std::endl;
  else
    PBPulseConfiguration::Load(src.configuration(), *dst.m_Config, *dst.m_Substances);
  if (config != nullptr)
  {// Merge in any provided configuration parameters, I hope you know what you are doing....
    const PulseConfiguration* peConfig = dynamic_cast<const PulseConfiguration*>(config);
    if (peConfig == nullptr)
    {
      dst.Error("Configuration is not a Pulse configuration object, ignoring it");
    }
    else
    {
      dst.m_Config->Merge(*peConfig, *dst.m_Substances);
    }
  }
  /////////////
  // Systems //
  /////////////
  if (!src.has_bloodchemistry())
    ss << "Missing Blood Chemistry State" << std::endl;
  else
    PBPulsePhysiology::Load(src.bloodchemistry(), *dst.m_BloodChemistrySystem);
  if (!src.has_cardiovascular())
    ss << "Missing Cardiovascular State" << std::endl;
  else
    PBPulsePhysiology::Load(src.cardiovascular(), *dst.m_CardiovascularSystem);
  if (!src.has_drug())
    ss << "Missing Drug State" << std::endl;
  else
    PBPulsePhysiology::Load(src.drug(), *dst.m_DrugSystem);
  if (!src.has_endocrine())
    ss << "Missing Endocrine State" << std::endl;
  else
    PBPulsePhysiology::Load(src.endocrine(), *dst.m_EndocrineSystem);
  if (!src.has_energy())
    ss << "Missing Energy State" << std::endl;
  else
    PBPulsePhysiology::Load(src.energy(), *dst.m_EnergySystem);
  if (!src.has_gastrointestinal())
    ss << "Missing Gastrointestinal State" << std::endl;
  else
    PBPulsePhysiology::Load(src.gastrointestinal(), *dst.m_GastrointestinalSystem);
  if (!src.has_hepatic())
    ss << "Missing Hepatic State" << std::endl;
  else
    PBPulsePhysiology::Load(src.hepatic(), *dst.m_HepaticSystem);
  if (!src.has_nervous())
    ss << "Missing Nervous State" << std::endl;
  else
    PBPulsePhysiology::Load(src.nervous(), *dst.m_NervousSystem);
  if (!src.has_renal())
    ss << "Missing Renal State" << std::endl;
  else
    PBPulsePhysiology::Load(src.renal(), *dst.m_RenalSystem);
  if (!src.has_respiratory())
    ss << "Missing Respiratory State" << std::endl;
  else
    PBPulsePhysiology::Load(src.respiratory(), *dst.m_RespiratorySystem);
  if (!src.has_tissue())
    ss << "Missing Tissue State" << std::endl;
  else
    PBPulsePhysiology::Load(src.tissue(), *dst.m_TissueSystem);
  if (!src.has_environment())
    ss << "Missing Environment State" << std::endl;
  else
    PBPulseEnvironment::Load(src.environment(), *dst.m_Environment);
  if (!src.has_anesthesiamachine())
    ss << "Missing Anesthesia Machine State" << std::endl;
  else
    PBPulseEquipment::Load(src.anesthesiamachine(), *dst.m_AnesthesiaMachine);
  if (!src.has_electrocardiogram())
    ss << "Missing ECG State" << std::endl;
  else
    PBPulseEquipment::Load(src.electrocardiogram(), *dst.m_ECG);
  if (!src.has_inhaler())
    ss << "Missing Inhaler State" << std::endl;
  else
    PBPulseEquipment::Load(src.inhaler(), *dst.m_Inhaler);
  if (!src.has_mechanicalventilator())
    ss << "Missing Mechanical Ventilator State" << std::endl;
  else
    PBPulseEquipment::Load(src.mechanicalventilator(), *dst.m_MechanicalVentilator);

  if (!ss.str().empty())
  {
    dst.Error(ss);
    return false;
  }
  // Make sure links are correct with our modes
  dst.m_Compartments->UpdateAirwayGraph();
  dst.m_Compartments->GetActiveRespiratoryGraph();
  dst.m_Compartments->GetActiveAerosolGraph();

  // If we had any handlers, reinform the listening classes
  if (dst.m_AdvanceHandler != nullptr)
    dst.SetAdvanceHandler(dst.m_AdvanceHandler);


  // It helps to unload what you just loaded and do a compare if you have issues
  //SerializeToFile(dst, "WhatIJustLoaded.json");

  // Good to go, save it off and carry on!
  dst.m_State = EngineState::Active;
  // TODO CheckDataRequirements/IsValid() or something
  return true;
}

PULSE_BIND::StateData* PBPulseState::Unload(const PulseController& src)
{
  PULSE_BIND::StateData* dst = new PULSE_BIND::StateData();
  PBPulseState::Serialize(src, *dst);
  return dst;
}
bool PBPulseState::Serialize(const PulseController& src, PULSE_BIND::StateData& dst)
{
  dst.set_airwaymode((PULSE_BIND::eAirwayMode)src.m_AirwayMode);
  dst.set_intubation((CDM_BIND::eSwitch)src.m_Intubation);
  dst.set_allocated_simulationtime(PBProperty::Unload(src.m_SimulationTime));
  if (src.m_EngineTrack->GetDataRequestManager().HasDataRequests())
    dst.set_allocated_datarequestmanager(PBEngine::Unload(src.m_EngineTrack->GetDataRequestManager()));
  // Patient
  dst.set_allocated_currentpatient(PBPatient::Unload(*src.m_CurrentPatient));
  dst.set_allocated_initialpatient(PBPatient::Unload(*src.m_InitialPatient));
  // Conditions
  dst.set_allocated_conditions(PBEngine::Unload(*src.m_Conditions));
  // Actions
  dst.set_allocated_activeactions(PBEngine::Unload(*src.m_Actions));
  // Active Events
  SEScalarTime time;
  for (auto itr : src.m_EventManager->GetEventStates())
  {
    if (!itr.second)
      continue;

    CDM_BIND::ActiveEventData* eData = dst.mutable_activeevents()->add_activeevent();
    eData->set_event((CDM_BIND::eEvent)itr.first);
    time.SetValue(src.m_EventManager->GetEventDuration(itr.first, TimeUnit::s), TimeUnit::s);
    eData->set_allocated_duration(PBProperty::Unload(time));
  }
  // Add all Substances/Compunds
  for (SESubstance* s : src.m_Substances->GetSubstances())
    dst.mutable_substance()->AddAllocated(PBSubstance::Unload(*s));
  for (SESubstanceCompound* c : src.m_Substances->GetCompounds())
    dst.mutable_compound()->AddAllocated(PBSubstance::Unload(*c));
  // Track the active Substances/Compounds
  for (SESubstance* s : src.m_Substances->GetActiveSubstances())
    dst.mutable_activesubstance()->Add(s->GetName());
  for (SESubstanceCompound* c : src.m_Substances->GetActiveCompounds())
    dst.mutable_activecompound()->Add(c->GetName());
  // Systems
  dst.set_allocated_bloodchemistry(PBPulsePhysiology::Unload(*src.m_BloodChemistrySystem));
  dst.set_allocated_cardiovascular(PBPulsePhysiology::Unload(*src.m_CardiovascularSystem));
  dst.set_allocated_drug(PBPulsePhysiology::Unload(*src.m_DrugSystem));
  dst.set_allocated_endocrine(PBPulsePhysiology::Unload(*src.m_EndocrineSystem));
  dst.set_allocated_energy(PBPulsePhysiology::Unload(*src.m_EnergySystem));
  dst.set_allocated_gastrointestinal(PBPulsePhysiology::Unload(*src.m_GastrointestinalSystem));
  dst.set_allocated_hepatic(PBPulsePhysiology::Unload(*src.m_HepaticSystem));
  dst.set_allocated_nervous(PBPulsePhysiology::Unload(*src.m_NervousSystem));
  dst.set_allocated_renal(PBPulsePhysiology::Unload(*src.m_RenalSystem));
  dst.set_allocated_respiratory(PBPulsePhysiology::Unload(*src.m_RespiratorySystem));
  dst.set_allocated_tissue(PBPulsePhysiology::Unload(*src.m_TissueSystem));
  dst.set_allocated_environment(PBPulseEnvironment::Unload(*src.m_Environment));
  dst.set_allocated_anesthesiamachine(PBPulseEquipment::Unload(*src.m_AnesthesiaMachine));
  dst.set_allocated_electrocardiogram(PBPulseEquipment::Unload(*src.m_ECG));
  dst.set_allocated_inhaler(PBPulseEquipment::Unload(*src.m_Inhaler));
  dst.set_allocated_mechanicalventilator(PBPulseEquipment::Unload(*src.m_MechanicalVentilator));
  // Compartments
  dst.set_allocated_compartmentmanager(PBCompartment::Unload(*src.m_Compartments));
  // Configuration
  dst.set_allocated_configuration(PBPulseConfiguration::Unload(*src.m_Config));
  // Circuits
  dst.set_allocated_circuitmanager(PBCircuit::Unload(*src.m_Circuits));
  return true;
}

bool PBPulseState::SerializeFromFile(const std::string& filename, PulseController& dst, const SEEngineConfiguration* config)
{
  PULSE_BIND::StateData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBPulseState::Load(data, dst, config);
  return true;
}

bool PBPulseState::SerializeToFile(const PulseController& src, const std::string& filename)
{
  PULSE_BIND::StateData data;
  PBPulseState::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBPulseState::SerializeToString(const PulseController& src, std::string& output, SerializationFormat m)
{
  PULSE_BIND::StateData data;
  PBPulseState::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPulseState::SerializeFromString(const std::string& src, PulseController& dst, SerializationFormat m, const SEEngineConfiguration* config)
{
  PULSE_BIND::StateData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBPulseState::Load(data, dst, config);
  return true;
}