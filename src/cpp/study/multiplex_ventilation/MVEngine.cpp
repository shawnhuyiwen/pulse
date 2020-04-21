/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVEngine.h"
#include "engine/SEActionManager.h"
#include "engine/SEPatientActionCollection.h"

const std::string Dir::Base = "./test_results/multiplex_ventilation/";
const std::string Dir::Solo = Dir::Base + "solo_states/";
const std::string Dir::Results = Dir::Base + "simulations/";

MVEngine::MVEngine(std::string const& logfile, bool cout_enabled, std::string const& data_dir) : Loggable(new Logger(logfile))
{
  m_TimeStep_s = 0.02;
  m_SubMgr = nullptr;
  m_Oxygen = nullptr;

  m_CircuitMgr = nullptr;
  m_MultiplexVentilationCircuit = nullptr;
  m_Calculator = nullptr;

  m_CmptMgr = nullptr;
  m_MultiplexVentilationGraph = nullptr;
  m_Transporter = nullptr;

  m_DataDir = data_dir;
  m_SimulationData = nullptr;

  m_MVC = nullptr;
  m_FiO2 = nullptr;

  myLogger = true;
  GetLogger()->LogToConsole(cout_enabled);
  Info("Logging to console : " + cout_enabled ? "True" : "False");
}
MVEngine::~MVEngine()
{
  DestroyEngines();
  SAFE_DELETE(m_SimulationData);
}

void MVEngine::DestroyEngines()
{
  m_CurrentTime_s = 0;
  SAFE_DELETE(m_SubMgr);
  m_Oxygen = nullptr;

  SAFE_DELETE(m_CircuitMgr);
  m_MultiplexVentilationCircuit = nullptr;
  SAFE_DELETE(m_Calculator);

  SAFE_DELETE(m_CmptMgr);
  m_MultiplexVentilationGraph = nullptr;
  SAFE_DELETE(m_Transporter);

  DELETE_VECTOR(m_Engines);
  m_AortaO2s.clear();
  m_AortaCO2s.clear();
  SAFE_DELETE(m_MVC);
  m_FiO2 = nullptr;
}

void MVEngine::HandleEvent(eEvent e, bool active, const SEScalarTime* simTime)
{

}

bool MVEngine::CreateEngine(const std::string& simulationDataStr, SerializationFormat fmt)
{
  SAFE_DELETE(m_SimulationData);
  m_SimulationData = new pulse::study::multiplex_ventilation::bind::SimulationData();
  SerializeFromString(simulationDataStr, *m_SimulationData, fmt);
  return CreateEngine(*m_SimulationData);
}

bool MVEngine::CreateEngine(pulse::study::multiplex_ventilation::bind::SimulationData& sim)
{
  try
  {
    if (!m_Engines.empty())
    {
      Error("The controller currently has engine allocated, please reset the controller to start new set of engines");
      return false;
    }
    // Figure out where to put results
    std::string outDir = sim.outputbasefilename();
    if (outDir.empty())
      outDir = "";

    m_SubMgr = new SESubstanceManager(GetLogger());
    m_SubMgr->LoadSubstanceDirectory(m_DataDir);
    m_Oxygen = m_SubMgr->GetSubstance("Oxygen");
    m_CmptMgr = new SECompartmentManager(*m_SubMgr);
    m_CircuitMgr = new SECircuitManager(GetLogger());

    m_MultiplexVentilationCircuit = &m_CircuitMgr->CreateFluidCircuit("MultiplexVentilation");
    m_MultiplexVentilationGraph = &m_CmptMgr->CreateGasGraph("MultiplexVentilation");

    m_Calculator = new SEFluidCircuitCalculator(VolumePerPressureUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s,
      PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O,
      VolumeUnit::L, PressureTimePerVolumeUnit::cmH2O_s_Per_L, GetLogger());
    m_Transporter = new SEGasTransporter(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, GetLogger());

    SEFluidCircuitNode* inspiratoryConnectionNode = nullptr;
    SEFluidCircuitNode* expiratoryConnectionNode = nullptr;

    SEGasCompartment* inspiratoryConnectionCompartment = nullptr;
    SEGasCompartment* expiratoryConnectionCompartment = nullptr;

    Info("Creating "+std::to_string(sim.patientcomparisons_size())+" patients");
    for (int p = 0; p < sim.patientcomparisons_size(); p++)
    {
      PulseController* pc = nullptr;
      auto& comparison = (*sim.mutable_patientcomparisons())[p];

      if (comparison.has_soloventilation())
      {
        auto* soloVentilation = comparison.mutable_soloventilation();
        std::string state = soloVentilation->statefile();
        pc = new PulseController(outDir + "multiplex_patient_" + std::to_string(p) + ".log", m_DataDir);
        if (!pc->SerializeFromFile(state, SerializationFormat::JSON))
        {
          Error("Unable to load file : " + state);
          return false;
        }
        pc->GetLogger()->LogToConsole(GetLogger()->IsLoggingToConsole());

        // Fill out our initial solo ventilation data
        soloVentilation->set_oxygensaturation(pc->GetBloodChemistry().GetOxygenSaturation().GetValue());
        soloVentilation->set_tidalvolume_ml(pc->GetRespiratory().GetTidalVolume(VolumeUnit::mL));
        soloVentilation->set_endtidalcarbondioxidepressure_mmhg(pc->GetRespiratory().GetEndTidalCarbonDioxidePressure(PressureUnit::mmHg));
        auto AortaO2 = pc->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetSubstanceQuantity(pc->GetSubstances().GetO2());
        soloVentilation->set_arterialoxygenpartialpressure_mmhg(AortaO2->GetPartialPressure(PressureUnit::mmHg));
        soloVentilation->set_arterialcarbondioxidepartialpressure_mmhg(AortaO2->GetPartialPressure(PressureUnit::mmHg));
        soloVentilation->set_carricoindex_mmhg(pc->GetRespiratory().GetCarricoIndex(PressureUnit::mmHg));
      }
      else if (comparison.has_multiplexventilation())
      {
        auto* multiVentilation = comparison.mutable_multiplexventilation();

        pc = new PulseController(outDir + "multiplex_patient_" + std::to_string(p) + ".log", m_DataDir);
        if (!pc->SerializeFromFile(m_DataDir + "/states/StandardMale@0s.json", SerializationFormat::JSON))
        {
          Error("Unable to load file : StandardMale@0s.json");
          return false;
        }
        pc->GetLogger()->LogToConsole(GetLogger()->IsLoggingToConsole());

        SEDyspnea dyspnea;
        dyspnea.GetSeverity().SetValue(1.0);
        pc->ProcessAction(dyspnea);

        SEIntubation intubation;
        intubation.SetType(eIntubation_Type::Tracheal);
        pc->ProcessAction(intubation);

        SEOverrides overrides;
        overrides.AddScalarProperty("RespiratoryCompliance", multiVentilation->compliance_ml_per_cmh2o(), VolumePerPressureUnit::mL_Per_cmH2O);
        overrides.AddScalarProperty("RespiratoryResistance", multiVentilation->resistance_cmh2o_s_per_l(), PressureTimePerVolumeUnit::cmH2O_s_Per_L);
        pc->ProcessAction(overrides);

        SEImpairedAlveolarExchangeExacerbation impairedAlveolarExchange;
        impairedAlveolarExchange.GetSeverity().SetValue(multiVentilation->impairmentfraction());
        pc->ProcessAction(impairedAlveolarExchange);

        SEPulmonaryShuntExacerbation pulmonaryShunt;
        pulmonaryShunt.GetSeverity().SetValue(multiVentilation->impairmentfraction());
        pc->ProcessAction(pulmonaryShunt);
      }
      else
      {
        Error("Simulation does not have valid comparison object");
        return false;
      }

      // Build our multiplex circuit
      if (p == 0)
      {
        // Let's add the first mechanical ventilator circuit to our circuit
        // This will add the ventialtor and the respiratory system
        for (SEFluidCircuitNode* node : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNodes())
          m_MultiplexVentilationCircuit->ForceAddNode(*node);
        for (SEFluidCircuitPath* path : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPaths())
          m_MultiplexVentilationCircuit->ForceAddPath(*path);
        for (SEGasCompartment* cmpt : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartments())
          m_MultiplexVentilationGraph->ForceAddCompartment(*cmpt);
        for (SEGasCompartmentLink* link : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetLinks())
          m_MultiplexVentilationGraph->ForceAddLink(*link);

        inspiratoryConnectionNode = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::InspiratoryValve);
        expiratoryConnectionNode = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::ExpiratoryValve);

        inspiratoryConnectionCompartment = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartment(pulse::MechanicalVentilatorCompartment::InspiratoryValve);
        expiratoryConnectionCompartment = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartment(pulse::MechanicalVentilatorCompartment::ExpiratoryValve);
      }
      else
      {
        SEFluidCircuitNode* expiratoryValveNode = nullptr;
        SEFluidCircuitNode* inspiratoryValveNode = nullptr;
        // Add all the nodes/paths/compartments/links to our circuit/graph
        for (SEFluidCircuitNode* node : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNodes())
        {
          if (node->GetName() != pulse::MechanicalVentilatorNode::Ventilator) // Don't add the ventilator node
            m_MultiplexVentilationCircuit->ForceAddNode(*node);
          if (node->GetName() == pulse::MechanicalVentilatorNode::ExpiratoryValve) // Need this later
            expiratoryValveNode = node;
          if (node->GetName() == pulse::MechanicalVentilatorNode::InspiratoryValve) // Need this later
            inspiratoryValveNode = node;
        }
        for (SEFluidCircuitPath* path : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPaths())
        {
          if (path->GetName() != pulse::MechanicalVentilatorPath::EnvironmentToVentilator &&     // Don't
            path->GetName() != pulse::MechanicalVentilatorPath::VentilatorToExpiratoryValve && // Add
            path->GetName() != pulse::MechanicalVentilatorPath::VentilatorToInspiratoryValve)  // These
            m_MultiplexVentilationCircuit->ForceAddPath(*path);
        }
        SEGasCompartment* expiratoryValveCmpt = nullptr;
        SEGasCompartment* inspiratoryValveCmpt = nullptr;
        for (SEGasCompartment* cmpt : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartments())
        {
          if (cmpt->GetName() != pulse::MechanicalVentilatorCompartment::MechanicalVentilator)// Don't add the ventilator compartment
            m_MultiplexVentilationGraph->ForceAddCompartment(*cmpt);
          if (cmpt->GetName() == pulse::MechanicalVentilatorCompartment::ExpiratoryValve) // Need this later
            expiratoryValveCmpt = cmpt;
          if (cmpt->GetName() == pulse::MechanicalVentilatorCompartment::InspiratoryValve) // Need this later
            inspiratoryValveCmpt = cmpt;
        }
        for (SEGasCompartmentLink* link : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetLinks())
        {
          if (link->GetName() != pulse::MechanicalVentilatorLink::MechanicalVentilatorToExpiratoryValve && // Don't
            link->GetName() != pulse::MechanicalVentilatorLink::MechanicalVentilatorToInspiratoryValve)  // Add
            m_MultiplexVentilationGraph->ForceAddLink(*link);
        }

        // Connect the tubes to the shared ventilator circuit/graph
        auto& expiratoryConnectionPath = m_MultiplexVentilationCircuit->CreatePath(*expiratoryConnectionNode, *expiratoryValveNode, "expiratoryConnection_" + std::to_string(p));
        auto& expiratoryConnectionLink = m_CmptMgr->CreateGasLink(*expiratoryConnectionCompartment, *expiratoryValveCmpt, "expiratoryConnection_" + std::to_string(p));
        expiratoryConnectionLink.MapPath(expiratoryConnectionPath);
        m_MultiplexVentilationGraph->AddLink(expiratoryConnectionLink);

        auto& inspiratoryConnectionPath = m_MultiplexVentilationCircuit->CreatePath(*inspiratoryConnectionNode, *inspiratoryValveNode, "inspiratoryConnection_p" + std::to_string(p));
        auto& inspiratoryConnectionLink = m_CmptMgr->CreateGasLink(*inspiratoryConnectionCompartment, *inspiratoryValveCmpt, "inspiratoryConnection_p" + std::to_string(p));
        inspiratoryConnectionLink.MapPath(inspiratoryConnectionPath);
        m_MultiplexVentilationGraph->AddLink(inspiratoryConnectionLink);
      }
      TrackData(pc->GetEngineTracker(), outDir + "multiplex_patient_" + std::to_string(p) + "_results.csv");
      auto AortaO2 = pc->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetSubstanceQuantity(pc->GetSubstances().GetO2());
      auto AortaCO2 = pc->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetSubstanceQuantity(pc->GetSubstances().GetCO2());
      m_AortaO2s.push_back(AortaO2);
      m_AortaCO2s.push_back(AortaCO2);
      m_Engines.push_back(pc);
    }
    m_MultiplexVentilationCircuit->StateChange();
    m_MultiplexVentilationGraph->StateChange();

    Info("Configuring Mechanical Ventilator");
    m_MVC = new SEMechanicalVentilatorConfiguration(*m_SubMgr);
    auto& mv = m_MVC->GetConfiguration();
    mv.SetConnection(eMechanicalVentilator_Connection::Tube);
    mv.SetControl(eMechanicalVentilator_Control::PC_CMV);
    mv.SetDriverWaveform(eMechanicalVentilator_DriverWaveform::Square);
    mv.GetRespiratoryRate().SetValue(sim.respirationrate_per_min(), FrequencyUnit::Per_min);
    mv.GetInspiratoryExpiratoryRatio().SetValue(sim.ieratio());
    mv.GetPeakInspiratoryPressure().SetValue(sim.pip_cmh2o(), PressureUnit::cmH2O);
    mv.GetPositiveEndExpiredPressure().SetValue(sim.peep_cmh2o(), PressureUnit::cmH2O);
    m_FiO2 = &mv.GetFractionInspiredGas(*m_Oxygen);
    m_FiO2->GetFractionAmount().SetValue(sim.fio2());
    Info("Processing Action");
    ProcessAction(*m_MVC);
    m_CurrentTime_s = 0;
    m_SpareAdvanceTime_s = 0;
  }
  catch (CommonDataModelException& cdm_ex)
  {
    GetLogger()->Fatal("Exception caught runnning simulation " + sim.outputbasefilename());
    GetLogger()->Fatal(cdm_ex.what());
    std::cerr << cdm_ex.what() << std::endl;
    return false;
  }
  catch (std::exception ex)
  {
    GetLogger()->Fatal("Exception caught runnning simulation " + sim.outputbasefilename());
    GetLogger()->Fatal(ex.what());
    std::cerr << ex.what() << std::endl;
    return false;
  }
  catch (...)
  {
    std::cerr << "Unable to run simulation " << sim.outputbasefilename() << std::endl;
    return false;
  }
  Info("Engine is ready");
  return true;
}

bool MVEngine::AdvanceTime(double time_s)
{
  if (m_Engines.empty())
  {
    Error("No engines have been allocated yet...");
    return false;
  }

  time_s += m_SpareAdvanceTime_s;
  int count = (int)(time_s / m_TimeStep_s);
  m_SpareAdvanceTime_s = time_s - (count * m_TimeStep_s);

  for (int i = 0; i < count; i++)
  {
    for (PulseController* pc : m_Engines)
    {
      if (pc->GetEvents().IsEventActive(eEvent::IrreversibleState))
        return false;
    }
    // PreProcess
    for (PulseController* pc : m_Engines)
      ((Environment&)pc->GetEnvironment()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Cardiovascular&)pc->GetCardiovascular()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Inhaler&)pc->GetInhaler()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Respiratory&)pc->GetRespiratory()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Gastrointestinal&)pc->GetGastrointestinal()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Hepatic&)pc->GetHepatic()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Renal&)pc->GetRenal()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Nervous&)pc->GetNervous()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Energy&)pc->GetEnergy()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Endocrine&)pc->GetEndocrine()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Drugs&)pc->GetDrugs()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((Tissue&)pc->GetTissue()).PreProcess();
    for (PulseController* pc : m_Engines)
      ((BloodChemistry&)pc->GetBloodChemistry()).PreProcess();
    // Since this is the last preprocess,
    // Check if we are in mechanical ventilator mode
    int vent_count = 0;
    bool enableMultiplexVentilation = false;
    for (PulseController* pc : m_Engines)
    {
      ((ECG&)pc->GetECG()).PreProcess();
      if (pc->GetAirwayMode() == eAirwayMode::MechanicalVentilator)
        vent_count++;
    }
    if (vent_count > 0)
    {
      if (vent_count == m_Engines.size())
        enableMultiplexVentilation = true;
      else
      {
        Fatal("Engines are out of sync");
        return false;
      }
    }

    // Process
    for (PulseController* pc : m_Engines)
      ((Environment&)pc->GetEnvironment()).Process();
    for (PulseController* pc : m_Engines)
      ((Cardiovascular&)pc->GetCardiovascular()).Process();
    for (PulseController* pc : m_Engines)
      ((Inhaler&)pc->GetInhaler()).Process();
    if (enableMultiplexVentilation)
    {
      // Solve the multiplex circuit
      m_Calculator->Process(*m_MultiplexVentilationCircuit, m_TimeStep_s);
      // Transport the multiplex graph
      m_Transporter->Transport(*m_MultiplexVentilationGraph, m_TimeStep_s);
      // TODO Consider adding aerosol support

      // TODO Copy mv node/path/cmpt/link values to engines>1
      // We are using engine 1's MV circuit/graph, so its up to date
      // The other engines MV circuit/graph needs to be manually updated
      // BUT, since the MV methodology does not use any circuit/graph data in its implementation
      // we DO NOT need to keep it up to date.
      // This is an intentional application specific optimization
    }
    for (PulseController* pc : m_Engines)
      ((Respiratory&)pc->GetRespiratory()).Process(!enableMultiplexVentilation);
    for (PulseController* pc : m_Engines)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).Process();
    for (PulseController* pc : m_Engines)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).Process();
    for (PulseController* pc : m_Engines)
      ((Gastrointestinal&)pc->GetGastrointestinal()).Process();
    for (PulseController* pc : m_Engines)
      ((Hepatic&)pc->GetHepatic()).Process();
    for (PulseController* pc : m_Engines)
      ((Renal&)pc->GetRenal()).Process();
    for (PulseController* pc : m_Engines)
      ((Nervous&)pc->GetNervous()).Process();
    for (PulseController* pc : m_Engines)
      ((Energy&)pc->GetEnergy()).Process();
    for (PulseController* pc : m_Engines)
      ((Endocrine&)pc->GetEndocrine()).Process();
    for (PulseController* pc : m_Engines)
      ((Drugs&)pc->GetDrugs()).Process();
    for (PulseController* pc : m_Engines)
      ((Tissue&)pc->GetTissue()).Process();
    for (PulseController* pc : m_Engines)
      ((BloodChemistry&)pc->GetBloodChemistry()).Process();
    for (PulseController* pc : m_Engines)
      ((ECG&)pc->GetECG()).Process();

    // PostProcess
    for (PulseController* pc : m_Engines)
      ((Environment&)pc->GetEnvironment()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Cardiovascular&)pc->GetCardiovascular()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Inhaler&)pc->GetInhaler()).PostProcess();
    if (enableMultiplexVentilation)
    {
      m_Calculator->PostProcess(*m_MultiplexVentilationCircuit);
    }
    for (PulseController* pc : m_Engines)
      ((Respiratory&)pc->GetRespiratory()).PostProcess(!enableMultiplexVentilation);
    for (PulseController* pc : m_Engines)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Gastrointestinal&)pc->GetGastrointestinal()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Hepatic&)pc->GetHepatic()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Renal&)pc->GetRenal()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Nervous&)pc->GetNervous()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Energy&)pc->GetEnergy()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Endocrine&)pc->GetEndocrine()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Drugs&)pc->GetDrugs()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((Tissue&)pc->GetTissue()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((BloodChemistry&)pc->GetBloodChemistry()).PostProcess();
    for (PulseController* pc : m_Engines)
      ((ECG&)pc->GetECG()).PostProcess();

    // Increment Times and track data
    for (PulseController* pc : m_Engines)
    {
      pc->GetEvents().UpdateEvents(pc->GetTimeStep());
      ((SEScalarTime&)pc->GetEngineTime()).Increment(pc->GetTimeStep());
      ((SEScalarTime&)pc->GetSimulationTime()).Increment(pc->GetTimeStep());
      pc->GetEngineTracker().TrackData(m_CurrentTime_s);
    }
    m_CurrentTime_s += m_TimeStep_s;
  }
  return true;
}

void MVEngine::SetFiO2(double FiO2)
{
  m_FiO2->GetFractionAmount().SetValue(FiO2);
  ProcessAction(*m_MVC);
}

bool MVEngine::ProcessAction(const SEAction& a)
{
  if (m_Engines.empty())
  {
    Error("No engines have been allocated yet...");
    return false;
  }
  bool b = true;
  for (PulseController* pc : m_Engines)
    if (!pc->ProcessAction(a))
      b = false;
  return b;
}
bool MVEngine::ProcessActions(std::string const& actions, SerializationFormat format)
{
  if (m_Engines.empty())
  {
    Error("No engines have been allocated yet...");
    return false;
  }

  bool success = true;
  if (actions.empty())
    return success;

  try
  {
    std::vector<SEAction*> vActions;
    if (!SEActionManager::SerializeFromString(actions, vActions, format, *m_SubMgr))
      return false;

    for (const SEAction* a : vActions)
    {
      if (!ProcessAction(*a))
        success = false;
      delete a;
    }
  }
  catch (CommonDataModelException& ex)
  {
    success = false;
    GetLogger()->Error(ex.what());
  }
  catch (std::exception& ex)
  {
    success = false;
    GetLogger()->Error(ex.what());
  }
  catch (...)
  {
    success = false;
  }
  return success;
}

std::string MVEngine::GetSimulationState(SerializationFormat fmt)
{
  if (!GetSimulationState(*m_SimulationData))
    return "";

  std::string content;
  SerializeToString(*m_SimulationData, content, SerializationFormat::JSON);
  return content;
}
bool MVEngine::GetSimulationState(pulse::study::multiplex_ventilation::bind::SimulationData& sim)
{
  if (m_Engines.empty())
  {
    Error("No engines have been allocated yet...");
    return false;
  }

  for (int p = 0; p < sim.patientcomparisons_size(); p++)
  {
    PulseController* pc = m_Engines[p];
    auto* multiVentilation = (*sim.mutable_patientcomparisons())[p].mutable_multiplexventilation();
    // For Completeness, Write out the ventilator settings

    multiVentilation->set_respirationrate_per_min(pc->GetMechanicalVentilator().GetRespiratoryRate(FrequencyUnit::Per_min));
    multiVentilation->set_ieratio(pc->GetMechanicalVentilator().GetInspiratoryExpiratoryRatio().GetValue());
    multiVentilation->set_peep_cmh2o(pc->GetMechanicalVentilator().GetPositiveEndExpiredPressure(PressureUnit::cmH2O));
    multiVentilation->set_pip_cmh2o(pc->GetMechanicalVentilator().GetPeakInspiratoryPressure(PressureUnit::cmH2O));
    multiVentilation->set_fio2(pc->GetMechanicalVentilator().GetFractionInspiredGas(pc->GetSubstances().GetO2()).GetFractionAmount().GetValue());
    // Write out all the vitals
    multiVentilation->set_airwayflow_l_per_min(pc->GetRespiratory().GetInspiratoryFlow(VolumePerTimeUnit::L_Per_min));
    multiVentilation->set_airwaypressure_cmh2o(pc->GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Mouth)->GetPressure(PressureUnit::cmH2O));
    multiVentilation->set_alveolararterialgradient_mmhg(pc->GetRespiratory().GetAlveolarArterialGradient(PressureUnit::mmHg));
    multiVentilation->set_arterialcarbondioxidepartialpressure_mmhg(m_AortaCO2s[p]->GetPartialPressure(PressureUnit::mmHg));
    multiVentilation->set_arterialoxygenpartialpressure_mmhg(m_AortaO2s[p]->GetPartialPressure(PressureUnit::mmHg));
    multiVentilation->set_carricoindex_mmhg(pc->GetRespiratory().GetCarricoIndex(PressureUnit::mmHg));
    multiVentilation->set_endtidalcarbondioxidepressure_mmhg(pc->GetRespiratory().GetEndTidalCarbonDioxidePressure(PressureUnit::mmHg));
    multiVentilation->set_idealbodyweight_kg(pc->GetCurrentPatient().GetIdealBodyWeight(MassUnit::kg));
    multiVentilation->set_meanairwaypressure_cmh2o(pc->GetRespiratory().GetMeanAirwayPressure(PressureUnit::cmH2O));
    multiVentilation->set_oxygenationindex(pc->GetRespiratory().GetOxygenationIndex().GetValue());
    multiVentilation->set_oxygensaturation(pc->GetBloodChemistry().GetOxygenSaturation().GetValue());
    multiVentilation->set_oxygensaturationindex_cmh2o(pc->GetRespiratory().GetOxygenSaturationIndex().GetValue(PressureUnit::cmH2O));
    multiVentilation->set_sfratio(pc->GetRespiratory().GetSaturationAndFractionOfInspiredOxygenRatio().GetValue());
    multiVentilation->set_shuntfraction(pc->GetBloodChemistry().GetShuntFraction().GetValue());
    multiVentilation->set_tidalvolume_ml(pc->GetRespiratory().GetTidalVolume(VolumeUnit::mL));
    multiVentilation->set_totallungvolume_ml(pc->GetRespiratory().GetTotalLungVolume(VolumeUnit::mL));
    // Update the sim
    if (p == 0)
    {
      sim.set_respirationrate_per_min(multiVentilation->respirationrate_per_min());
      sim.set_ieratio(multiVentilation->ieratio());
      sim.set_peep_cmh2o(multiVentilation->peep_cmh2o());
      sim.set_pip_cmh2o(multiVentilation->pip_cmh2o());
      sim.set_fio2(multiVentilation->fio2());
    }
  }
  return true;
}

double MVEngine::GetMinSpO2()
{
  double SpO2;
  double minSpO2 = 1.0;
  for (PulseController* pc : m_Engines)
  {
    SpO2 = pc->GetBloodChemistry().GetOxygenSaturation().GetValue();
    if (SpO2 < minSpO2)
      minSpO2 = SpO2;
  }
  return minSpO2;
}

double MVEngine::GetMinPAO2_mmHg()
{
  double PAO2_mmHg;
  double minPAO2_mmHg = 1000;
  for (SELiquidSubstanceQuantity* aortaO2 : m_AortaO2s)
  {
    PAO2_mmHg = aortaO2->GetPartialPressure(PressureUnit::mmHg);
    if (PAO2_mmHg < minPAO2_mmHg)
      minPAO2_mmHg = PAO2_mmHg;
  }
  return minPAO2_mmHg;
}

void MVEngine::TrackData(SEEngineTracker& trkr, const std::string& csv_filename)
{
  trkr.GetDataRequestManager().SetResultsFilename(csv_filename);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("ExpiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  trkr.GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PeakInspiratoryPressure", PressureUnit::cmH2O);
  trkr.GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PositiveEndExpiredPressure", PressureUnit::cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("TotalRespiratoryModelCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("CarricoIndex", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryExpiratoryRatio");
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("ShuntFraction");
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("AlveolarArterialGradient", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryFlow", VolumePerTimeUnit::L_Per_min);

  trkr.GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::PulmonaryCompartment::Mouth, "Pressure", PressureUnit::cmH2O);

  SESubstance* O2 = trkr.GetSubstanceManager().GetSubstance("Oxygen");
  SESubstance* CO2 = trkr.GetSubstanceManager().GetSubstance("CarbonDioxide");
  trkr.GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, *O2, "PartialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, *CO2, "PartialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::Mouth, *O2, "PartialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::Mouth, *CO2, "PartialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::LeftAlveoli, *O2, "PartialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::LeftAlveoli, *CO2, "PartialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::RightAlveoli, *O2, "PartialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::RightAlveoli, *CO2, "PartialPressure", PressureUnit::mmHg);

  trkr.SetupRequests();
}

bool MVEngine::SerializeToString(pulse::study::multiplex_ventilation::bind::SimulationData& src, std::string& dst, SerializationFormat f)
{
  google::protobuf::util::JsonPrintOptions printOpts;
  printOpts.add_whitespace = true;
  printOpts.preserve_proto_field_names = true;
  printOpts.always_print_primitive_fields = true;
  return google::protobuf::util::MessageToJsonString(src, &dst, printOpts).ok();
}
bool MVEngine::SerializeFromString(const std::string& src, pulse::study::multiplex_ventilation::bind::SimulationData& dst, SerializationFormat f)
{
  google::protobuf::util::JsonParseOptions parseOpts;
  google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
  {
    std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
  });
  google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(src, &dst, parseOpts);
  if (!stat.ok())
  {
    std::cerr << "Unable to parse json in string : "<<stat.ToString()<<std::endl;
    return false;
  }
  return true;
}


bool MVEngine::RunSoloState(const std::string& stateFile, const std::string& resultBasePath, double duration_s, Logger* logger)
{
  std::string logFile = resultBasePath + ".log";
  std::string dataFile = resultBasePath + "Results.csv";

  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");
  double statusTime_s = 0;// Current time of this status cycle
  double statusStep_s = 60;//How long did it take to simulate this much time

  double timeStep_s = 0.02;
  double currentTime_s = 0;

  PulseController pc(logFile);
  pc.SerializeFromFile(stateFile, SerializationFormat::JSON);
  MVEngine::TrackData(pc.GetEngineTracker(), dataFile);
  int count = (int)(duration_s / timeStep_s);
  for (int i = 0; i < count; i++)
  {
    if (pc.GetEvents().IsEventActive(eEvent::IrreversibleState))
      return false;
    pc.AdvanceModelTime();
    pc.GetEngineTracker().TrackData(currentTime_s);
    currentTime_s += timeStep_s;
    statusTime_s += timeStep_s;
    // How are we running?
    if (statusTime_s > statusStep_s)
    {
      statusTime_s = 0;
      logger->Info("Current Time is " + to_scientific_notation(currentTime_s) + "s, it took " + to_scientific_notation(profiler.GetElapsedTime_s("Status")) + "s to simulate the past " + to_scientific_notation(statusStep_s) + "s");
      profiler.Reset("Status");
    }
  }
  logger->Info("It took " + to_scientific_notation(profiler.GetElapsedTime_s("Total")) + "s to run this simulation");
  profiler.Clear();
  return true;
}

bool ExtractVentilatorSettings(const std::string& filePath, std::string& fileName, double& pip_cmH2O, double& peep_cmH2O, double& FiO2)
{
  // Parse the file name to get our ventilator settings
  // ex. comp=0.01_peep=10_pip=55_imp=0.3_FiO2=0.21
  size_t peepIdx = filePath.find("peep=") + 5;
  size_t _peepIdx = filePath.find("_", peepIdx);
  peep_cmH2O = std::atof(filePath.substr(peepIdx, _peepIdx).c_str());
  size_t pipIdx = filePath.find("pip=") + 4;
  size_t _pipIdx = filePath.find("_", pipIdx);
  pip_cmH2O = std::atof(filePath.substr(pipIdx, _pipIdx).c_str());
  size_t FiO2Idx = filePath.find("FiO2=") + 5;
  size_t _FiO2Idx = filePath.find(".", FiO2Idx);
  FiO2 = std::atof(filePath.substr(FiO2Idx, _FiO2Idx).c_str());

  fileName = filePath.substr(filePath.find_last_of("/") + 1);
  fileName = fileName.substr(0, fileName.length() - 5);
  return true;
}

std::string to_scientific_notation(float f)
{
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "%g", f);
  return std::string(buffer);
}

std::string to_scientific_notation(double d)
{
  return to_scientific_notation(float(d));
}
