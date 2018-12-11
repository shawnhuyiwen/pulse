/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
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
#include "bind/cpp/pulse/PulseState.pb.h"
#include "PulseConfiguration.h"
#include "controller/Engine.h"
#include "controller/Substances.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceCompound.h"
#include "engine/SEEngineTracker.h"
#include "engine/SECondition.h"
#include "engine/SEConditionManager.h"
#include "engine/SEAction.h"
#include "engine/SEActionManager.h"
#include "engine/SEDataRequestManager.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"


void PBPulseState::Load(const pulse::proto::StateData& src, PulseEngine& dst, const SEScalarTime* simTime, const SEEngineConfiguration* config)
{
  PBPulseState::Serialize(src, dst,simTime,config);
}
bool PBPulseState::Serialize(const pulse::proto::StateData& src, PulseEngine& dst, const SEScalarTime* simTime, const SEEngineConfiguration* config)
{
  std::stringstream ss;
  dst.m_State = EngineState::NotReady;
  // First Get the substances reset and ready
  dst.m_Substances->Reset();
  // Substances //
  for (int i = 0; i < src.activesubstance_size(); i++)
  {
    const cdm::SubstanceData& subData = src.activesubstance()[i];
    SESubstance* sub = dst.m_Substances->GetSubstance(subData.name());
    if (sub == nullptr)
    {
      sub = new SESubstance(dst.GetLogger());
      dst.m_Substances->AddSubstance(*sub);
    }
    PBSubstance::Load(subData, *sub);
    dst.m_Substances->AddActiveSubstance(*sub);
  }
  // Compounds //
  for (int i = 0; i < src.activecompound_size(); i++)
  {
    const cdm::SubstanceCompoundData& cmpdData = src.activecompound()[i];
    SESubstanceCompound* cmpd = dst.m_Substances->GetCompound(cmpdData.name());
    if (cmpd == nullptr)
      cmpd = new SESubstanceCompound(dst.GetLogger());
    PBSubstance::Load(cmpdData, *cmpd, *dst.m_Substances);
    dst.m_Substances->AddActiveCompound(*cmpd);
  }

  // We could preserve the tracker, but I think I want to force the user to set it up
  // again, they should have the data tracks (or easily get them), and they should
  // Set it back up, and set or reset the results file they are using
  if (src.has_datarequestmanager())
  {
    dst.m_EngineTrack->GetDataRequestManager().Clear();
    PBEngine::Load(src.datarequestmanager(), dst.m_EngineTrack->GetDataRequestManager(), *dst.m_Substances);
    dst.m_EngineTrack->ForceConnection();// I don't want to rest the file because I would loose all my data
  }

  if (simTime != nullptr)
  {
    dst.m_CurrentTime->Set(*simTime);
    dst.m_SimulationTime->Set(*simTime);
  }
  else
  {
    if (src.has_simulationtime())
    {
      PBProperty::Load(src.simulationtime(), *dst.m_CurrentTime);
      PBProperty::Load(src.simulationtime(), *dst.m_SimulationTime);
    }
    else
    {
      dst.m_CurrentTime->SetValue(0, TimeUnit::s);
      dst.m_SimulationTime->SetValue(0, TimeUnit::s);
    }
  }
  dst.m_AirwayMode = (eAirwayMode)src.airwaymode();
  if (src.intubation() == (cdm::eSwitch)eSwitch::NullSwitch)
    ss << "Pulse State must have none null intubation state";
  dst.m_Intubation = (eSwitch)src.intubation();

  /// Patient //  
  if (!src.has_patient())
    ss << "PulseState must have a patient" << std::endl;
  else
    PBPatient::Load(src.patient(), *dst.m_Patient);
  // Conditions //
  dst.m_Conditions->Clear();
  if (src.has_conditions())
  {
    for (int i = 0; i < src.conditions().anycondition_size(); i++)
    {
      SECondition* c = PBCondition::Load(src.conditions().anycondition()[i], *dst.m_Substances);
      dst.m_Conditions->ProcessCondition(*c);
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

  // Circuit Manager //
  if (!src.has_circuitmanager())
    ss << "PulseState must have a circuit manager" << std::endl;
  else
    PBCircuit::Load(src.circuitmanager(), *dst.m_Circuits);
  // Compartment Manager //
  if (!src.has_compartmentmanager())
    ss << "PulseState must have a compartment manager" << std::endl;
  else
    PBCompartment::Load(src.compartmentmanager(), *dst.m_Compartments, dst.m_Circuits.get());
  // Configuration //
  if (!src.has_configuration())
    ss << "PulseState must have a configuration" << std::endl;
  else
    PBPulseConfiguration::Load(src.configuration(), *dst.m_Config);
  if (config != nullptr)
  {// Merge in any provided configuration parameters, I hope you know what you are doing....
    const PulseConfiguration* peConfig = dynamic_cast<const PulseConfiguration*>(config);
    if (peConfig == nullptr)
    {
      dst.Error("Configuration is not a Pulse configuration object, ignoring it");
    }
    else
    {
      dst.m_Config->Merge(*peConfig);
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
  if (dst.m_EventHandler != nullptr)
    dst.SetEventHandler(dst.m_EventHandler);
  if (dst.m_AdvanceHandler != nullptr)
    dst.SetAdvanceHandler(dst.m_AdvanceHandler);


  // It helps to unload what you just loaded and to a compare if you have issues
  //SaveState("WhatIJustLoaded.json");

  // Good to go, save it off and carry on!
  dst.m_State = EngineState::Active;
  // TODO CheckDataRequirements/IsValid() or something
  return true;
}

pulse::proto::StateData* PBPulseState::Unload(const PulseEngine& src)
{
  pulse::proto::StateData* dst = new pulse::proto::StateData();
  PBPulseState::Serialize(src, *dst);
  return dst;
}
bool PBPulseState::Serialize(const PulseEngine& src, pulse::proto::StateData& dst)
{
  dst.set_airwaymode((pulse::proto::eAirwayMode)src.m_AirwayMode);
  dst.set_intubation((cdm::eSwitch)src.m_Intubation);
  dst.set_allocated_simulationtime(PBProperty::Unload(*src.m_SimulationTime));
  if (src.m_EngineTrack->GetDataRequestManager().HasDataRequests())
    dst.set_allocated_datarequestmanager(PBEngine::Unload(src.m_EngineTrack->GetDataRequestManager()));
  // Patient
  dst.set_allocated_patient(PBPatient::Unload(*src.m_Patient));
  // Conditions
  dst.set_allocated_conditions(PBEngine::Unload(*src.m_Conditions));
  // Actions
  dst.set_allocated_activeactions(PBEngine::Unload(*src.m_Actions));
  // Active Substances/Compounds
  for (SESubstance* s : src.m_Substances->GetActiveSubstances())
    dst.mutable_activesubstance()->AddAllocated(PBSubstance::Unload(*s));
  for (SESubstanceCompound* c : src.m_Substances->GetActiveCompounds())
    dst.mutable_activecompound()->AddAllocated(PBSubstance::Unload(*c));
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
  // Compartments
  dst.set_allocated_compartmentmanager(PBCompartment::Unload(*src.m_Compartments));
  // Configuration
  dst.set_allocated_configuration(PBPulseConfiguration::Unload(*src.m_Config));
  // Circuitsk
  dst.set_allocated_circuitmanager(PBCircuit::Unload(*src.m_Circuits));
  return true;
}

bool PBPulseState::SerializeToString(const PulseEngine& src, std::string& output, SerializationFormat m)
{
  pulse::proto::StateData data;
  PBPulseState::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBPulseState::SerializeToFile(const PulseEngine& src, const std::string& filename, SerializationFormat m)
{
  pulse::proto::StateData data;
  PBPulseState::Serialize(src, data);
  std::string content;
  PBPulseState::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBPulseState::SerializeFromString(const std::string& src, PulseEngine& dst, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config)
{
  pulse::proto::StateData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBPulseState::Load(data, dst, simTime, config);
  return true;
}
bool PBPulseState::SerializeFromFile(const std::string& filename, PulseEngine& dst, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBPulseState::SerializeFromString(content, dst, m, simTime, config);
}