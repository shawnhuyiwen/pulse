/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

bool MVController::RunSimulation(pulse::study::multiplex_ventilation::bind::SimulationData& sim)
{
  // Figure out where to put results
  std::string outDir = sim.outputbasefilename();
  if(outDir.empty())
    outDir = Dir::Results;

  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");

  std::vector<PulseController*> engines;
  bool   enableMultiplexVentilation = false;
  double timeStep_s = 0.02;
  double currentTime_s = 0;

  SESubstanceManager   subMgr(GetLogger());
  subMgr.LoadSubstanceDirectory();
  SESubstance* Oxygen = subMgr.GetSubstance("Oxygen");
  SECompartmentManager cmptMgr(subMgr);
  SECircuitManager     circuitMgr(GetLogger());

  SEFluidCircuit& multiplexVentilationCircuit=circuitMgr.CreateFluidCircuit("MultiplexVentilation");
  SEGasCompartmentGraph& multiplexVentilationGraph = cmptMgr.CreateGasGraph("MultiplexVentilation");

  SEFluidCircuitCalculator* calculator = new SEFluidCircuitCalculator(VolumePerPressureUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s,
    PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O,
    VolumeUnit::L, PressureTimePerVolumeUnit::cmH2O_s_Per_L, GetLogger());
  SEGasTransporter* transporter = new SEGasTransporter(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, GetLogger());
  //SELiquidTransporter* aerosolTransporter = new SELiquidTransporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, GetLogger());

  SEFluidCircuitNode* inspiratoryConnectionNode = nullptr;
  SEFluidCircuitNode* expiratoryConnectionNode = nullptr;

  SEGasCompartment* inspiratoryConnectionCompartment = nullptr;
  SEGasCompartment* expiratoryConnectionCompartment = nullptr;

  for (int p=0; p<sim.patientcomparisons_size(); p++)
  {
    auto* soloVentilation = (*sim.mutable_patientcomparisons())[p].mutable_soloventilation();
    auto* multiVentilation = (*sim.mutable_patientcomparisons())[p].mutable_multiplexventilation();

    std::string state = soloVentilation->statefile();

    PulseController* pc = new PulseController(outDir+"multiplex_patient_"+std::to_string(p)+".log", m_DataDir);
    if (!pc->SerializeFromFile(state, SerializationFormat::JSON))
    {
      Error("Unable to load file : " + state);
      return false;
    }
    pc->GetLogger()->LogToConsole(GetLogger()->IsLoggingToConsole());

    // Fill out our initial solo ventilation data
    soloVentilation->set_oxygensaturation(pc->GetBloodChemistry().GetOxygenSaturation().GetValue());
    soloVentilation->set_tidalvolume_ml(pc->GetRespiratory().GetTidalVolume(VolumeUnit::mL));
    soloVentilation->set_endtidalcarbondioxidepressure_cmh2o(pc->GetRespiratory().GetEndTidalCarbonDioxidePressure(PressureUnit::cmH2O));
    auto AortaO2 = pc->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetSubstanceQuantity(pc->GetSubstances().GetO2());
    soloVentilation->set_arterialoxygenpartialpressure_mmhg(AortaO2->GetPartialPressure(PressureUnit::mmHg));
    soloVentilation->set_arterialcarbondioxidepartialpressure_mmhg(AortaO2->GetPartialPressure(PressureUnit::mmHg));
    soloVentilation->set_carricoindex_mmhg(pc->GetRespiratory().GetCarricoIndex(PressureUnit::mmHg));
    // Copy our inital conditions and multiplex ventilator values to our multiVentilation structure for completeness
    multiVentilation->set_compliance_ml_per_cmh2o(soloVentilation->compliance_ml_per_cmh2o());
    multiVentilation->set_impairmentfraction(soloVentilation->impairmentfraction());
    multiVentilation->set_respirationrate_per_min(soloVentilation->respirationrate_per_min());
    multiVentilation->set_ieratio(soloVentilation->ieratio());
    multiVentilation->set_peep_cmh2o(sim.peep_cmh2o());
    multiVentilation->set_pip_cmh2o(sim.pip_cmh2o());
    multiVentilation->set_fio2(sim.fio2());

    // Build our multiplex circuit
    if (p == 0)
    {
      // Let's add the first mechanical ventilator circuit to our circuit
      // This will add the ventialtor and the respiratory system
      for (SEFluidCircuitNode* node : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNodes())
        multiplexVentilationCircuit.ForceAddNode(*node);
      for (SEFluidCircuitPath* path : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPaths())
        multiplexVentilationCircuit.ForceAddPath(*path);
      for (SEGasCompartment* cmpt : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartments())
        multiplexVentilationGraph.ForceAddCompartment(*cmpt);
      for (SEGasCompartmentLink* link : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetLinks())
        multiplexVentilationGraph.ForceAddLink(*link);

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
        if(node->GetName() != pulse::MechanicalVentilatorNode::Ventilator) // Don't add the ventilator node
          multiplexVentilationCircuit.ForceAddNode(*node);
        if(node->GetName() == pulse::MechanicalVentilatorNode::ExpiratoryValve) // Need this later
          expiratoryValveNode = node;
        if (node->GetName() == pulse::MechanicalVentilatorNode::InspiratoryValve) // Need this later
          inspiratoryValveNode = node;
      }
      for (SEFluidCircuitPath* path : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPaths())
      {
        if(path->GetName() != pulse::MechanicalVentilatorPath::EnvironmentToVentilator &&     // Don't
           path->GetName() != pulse::MechanicalVentilatorPath::VentilatorToExpiratoryValve && // Add
           path->GetName() != pulse::MechanicalVentilatorPath::VentilatorToInspiratoryValve)  // These
        multiplexVentilationCircuit.ForceAddPath(*path);
      }
      SEGasCompartment* expiratoryValveCmpt = nullptr;
      SEGasCompartment* inspiratoryValveCmpt = nullptr;
      for (SEGasCompartment* cmpt : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartments())
      {
        if(cmpt->GetName() != pulse::MechanicalVentilatorCompartment::MechanicalVentilator)// Don't add the ventilator compartment
          multiplexVentilationGraph.ForceAddCompartment(*cmpt);
        if (cmpt->GetName() == pulse::MechanicalVentilatorCompartment::ExpiratoryValve) // Need this later
          expiratoryValveCmpt = cmpt;
        if (cmpt->GetName() == pulse::MechanicalVentilatorCompartment::InspiratoryValve) // Need this later
          inspiratoryValveCmpt = cmpt;
      }
      for (SEGasCompartmentLink* link : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetLinks())
      {
        if(link->GetName() != pulse::MechanicalVentilatorLink::MechanicalVentilatorToExpiratoryValve && // Don't
           link->GetName() != pulse::MechanicalVentilatorLink::MechanicalVentilatorToInspiratoryValve)  // Add
        multiplexVentilationGraph.ForceAddLink(*link);
      }

      // Connect the tubes to the shared ventilator circuit/graph
      auto& expiratoryConnectionPath = multiplexVentilationCircuit.CreatePath(*expiratoryConnectionNode, *expiratoryValveNode, "expiratoryConnection_"+std::to_string(p));
      auto& expiratoryConnectionLink = cmptMgr.CreateGasLink(*expiratoryConnectionCompartment, *expiratoryValveCmpt, "expiratoryConnection_" + std::to_string(p));
      expiratoryConnectionLink.MapPath(expiratoryConnectionPath);
      multiplexVentilationGraph.AddLink(expiratoryConnectionLink);

      auto& inspiratoryConnectionPath = multiplexVentilationCircuit.CreatePath(*inspiratoryConnectionNode, *inspiratoryValveNode, "inspiratoryConnection_p" + std::to_string(p));
      auto& inspiratoryConnectionLink = cmptMgr.CreateGasLink(*inspiratoryConnectionCompartment, *inspiratoryValveCmpt, "inspiratoryConnection_p"+std::to_string(p));
      inspiratoryConnectionLink.MapPath(inspiratoryConnectionPath);
      multiplexVentilationGraph.AddLink(inspiratoryConnectionLink);
    }
    TrackData(pc->GetEngineTracker(), outDir+"multiplex_patient_"+std::to_string(p)+"_results.csv");
    engines.push_back(pc);
  }
  multiplexVentilationCircuit.StateChange();
  multiplexVentilationGraph.StateChange();

  SEMechanicalVentilatorConfiguration mvc(subMgr);
  auto& mv = mvc.GetConfiguration();
  mv.SetConnection(eMechanicalVentilator_Connection::Tube);
  mv.SetControl(eMechanicalVentilator_Control::PC_CMV);
  mv.SetDriverWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.GetRespiratoryRate().SetValue(sim.respirationrate_per_min(), FrequencyUnit::Per_min);
  mv.GetInspiratoryExpiratoryRatio().SetValue(sim.ieratio());
  mv.GetPeakInspiratoryPressure().SetValue(sim.pip_cmh2o(), PressureUnit::cmH2O);
  mv.GetPositiveEndExpiredPressure().SetValue(sim.peep_cmh2o(), PressureUnit::cmH2O);
  SESubstanceFraction* FiO2 = &mv.GetFractionInspiredGas(*Oxygen);
  FiO2->GetFractionAmount().SetValue(sim.fio2());
  for (PulseController* pc : engines)
    pc->ProcessAction(mvc);

  int stabilizationPasses     = 0;
  int totalIterations         = 0;
  double stabilizationTimer_s = 0;
  double stabilizationCheck_s = 2;
  // Let's shoot for with in 0.25% for 10s straight
  double pctDiffSpO2 = 0.25;
  std::vector<double> previsouSpO2s;
  for (PulseController* pc : engines)
    previsouSpO2s.push_back(pc->GetBloodChemistry().GetOxygenSaturation().GetValue());

  double statusTimer_s        = 0;  // Current time of this status cycle
  double statusStep_s         = 60; // How long did it take to simulate this much time

  int vent_count;
  int minIterations = (int)(60 / timeStep_s);
  int maxIterations = (int)(300 / timeStep_s);
  while (true)
  {
    totalIterations++;
    if(totalIterations == minIterations)
      Info("Minimum Runtime Achieved");

    for (PulseController* pc : engines)
    {
      if (pc->GetEvents().IsEventActive(eEvent::IrreversibleState))
        return false;
    }
    // PreProcess
    for (PulseController* pc : engines)
      ((Environment&)pc->GetEnvironment()).PreProcess();
    for (PulseController* pc : engines)
      ((Cardiovascular&)pc->GetCardiovascular()).PreProcess();
    for (PulseController* pc : engines)
      ((Inhaler&)pc->GetInhaler()).PreProcess();
    for (PulseController* pc : engines)
      ((Respiratory&)pc->GetRespiratory()).PreProcess();
    for (PulseController* pc : engines)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).PreProcess();
    for (PulseController* pc : engines)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).PreProcess();
    for (PulseController* pc : engines)
      ((Gastrointestinal&)pc->GetGastrointestinal()).PreProcess();
    for (PulseController* pc : engines)
      ((Hepatic&)pc->GetHepatic()).PreProcess();
    for (PulseController* pc : engines)
      ((Renal&)pc->GetRenal()).PreProcess();
    for (PulseController* pc : engines)
      ((Nervous&)pc->GetNervous()).PreProcess();
    for (PulseController* pc : engines)
      ((Energy&)pc->GetEnergy()).PreProcess();
    for (PulseController* pc : engines)
      ((Endocrine&)pc->GetEndocrine()).PreProcess();
    for (PulseController* pc : engines)
      ((Drugs&)pc->GetDrugs()).PreProcess();
    for (PulseController* pc : engines)
      ((Tissue&)pc->GetTissue()).PreProcess();
    for (PulseController* pc : engines)
      ((BloodChemistry&)pc->GetBloodChemistry()).PreProcess();
    // Since this is the last preprocess,
    // Check if we are in mechanical ventilator mode
    vent_count = 0;
    enableMultiplexVentilation = false;
    for (PulseController* pc : engines)
    {
      ((ECG&)pc->GetECG()).PreProcess();
      if (pc->GetAirwayMode() == eAirwayMode::MechanicalVentilator)
        vent_count++;
    }
    if (vent_count > 0)
    {
      if (vent_count == engines.size())
        enableMultiplexVentilation = true;
      else
      {
        Fatal("Engines are out of sync");
        return false;
      }
    }

    // Process
    for (PulseController* pc : engines)
      ((Environment&)pc->GetEnvironment()).Process();
    for (PulseController* pc : engines)
      ((Cardiovascular&)pc->GetCardiovascular()).Process();
    for (PulseController* pc : engines)
      ((Inhaler&)pc->GetInhaler()).Process();
    if (enableMultiplexVentilation)
    {
      // Solve the multiplex circuit
      calculator->Process(multiplexVentilationCircuit, timeStep_s);
      // Transport the multiplex graph
      transporter->Transport(multiplexVentilationGraph, timeStep_s);
      // TODO Consider adding aerosol support

      // TODO Copy mv node/path/cmpt/link values to engines>1
      // We are using engine 1's MV circuit/graph, so its up to date
      // The other engines MV circuit/graph needs to be manually updated
      // BUT, since the MV methodology does not use any circuit/graph data in its implementation
      // we DO NOT need to keep it up to date.
      // This is an intentional application specific optimization
    }
    for (PulseController* pc : engines)
      ((Respiratory&)pc->GetRespiratory()).Process(!enableMultiplexVentilation);
    for (PulseController* pc : engines)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).Process();
    for (PulseController* pc : engines)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).Process();
    for (PulseController* pc : engines)
      ((Gastrointestinal&)pc->GetGastrointestinal()).Process();
    for (PulseController* pc : engines)
      ((Hepatic&)pc->GetHepatic()).Process();
    for (PulseController* pc : engines)
      ((Renal&)pc->GetRenal()).Process();
    for (PulseController* pc : engines)
      ((Nervous&)pc->GetNervous()).Process();
    for (PulseController* pc : engines)
      ((Energy&)pc->GetEnergy()).Process();
    for (PulseController* pc : engines)
      ((Endocrine&)pc->GetEndocrine()).Process();
    for (PulseController* pc : engines)
      ((Drugs&)pc->GetDrugs()).Process();
    for (PulseController* pc : engines)
      ((Tissue&)pc->GetTissue()).Process();
    for (PulseController* pc : engines)
      ((BloodChemistry&)pc->GetBloodChemistry()).Process();
    for (PulseController* pc : engines)
      ((ECG&)pc->GetECG()).Process();

    // PostProcess
    for (PulseController* pc : engines)
      ((Environment&)pc->GetEnvironment()).PostProcess();
    for (PulseController* pc : engines)
      ((Cardiovascular&)pc->GetCardiovascular()).PostProcess();
    for (PulseController* pc : engines)
      ((Inhaler&)pc->GetInhaler()).PostProcess();
    if (enableMultiplexVentilation)
    {
      calculator->PostProcess(multiplexVentilationCircuit);
    }
    for (PulseController* pc : engines)
      ((Respiratory&)pc->GetRespiratory()).PostProcess(!enableMultiplexVentilation);
    for (PulseController* pc : engines)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).PostProcess();
    for (PulseController* pc : engines)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).PostProcess();
    for (PulseController* pc : engines)
      ((Gastrointestinal&)pc->GetGastrointestinal()).PostProcess();
    for (PulseController* pc : engines)
      ((Hepatic&)pc->GetHepatic()).PostProcess();
    for (PulseController* pc : engines)
      ((Renal&)pc->GetRenal()).PostProcess();
    for (PulseController* pc : engines)
      ((Nervous&)pc->GetNervous()).PostProcess();
    for (PulseController* pc : engines)
      ((Energy&)pc->GetEnergy()).PostProcess();
    for (PulseController* pc : engines)
      ((Endocrine&)pc->GetEndocrine()).PostProcess();
    for (PulseController* pc : engines)
      ((Drugs&)pc->GetDrugs()).PostProcess();
    for (PulseController* pc : engines)
      ((Tissue&)pc->GetTissue()).PostProcess();
    for (PulseController* pc : engines)
      ((BloodChemistry&)pc->GetBloodChemistry()).PostProcess();
    for (PulseController* pc : engines)
      ((ECG&)pc->GetECG()).PostProcess();

    // Increment Times and track data
    for (PulseController* pc : engines)
    {
      pc->GetEvents().UpdateEvents(pc->GetTimeStep());
      ((SEScalarTime&)pc->GetEngineTime()).Increment(pc->GetTimeStep());
      ((SEScalarTime&)pc->GetSimulationTime()).Increment(pc->GetTimeStep());
      pc->GetEngineTracker().TrackData(currentTime_s);
    }
    // Increment our timers
    currentTime_s += timeStep_s;
    statusTimer_s += timeStep_s;
    // Start stabilization timer after minimum run time
    if (totalIterations > minIterations)
      stabilizationTimer_s += timeStep_s;

    // How are we running?
    if (statusTimer_s > statusStep_s)
    {
      statusTimer_s = 0;
      Info("Current Time is "+to_scientific_notation(currentTime_s)+"s, it took "+to_scientific_notation(profiler.GetElapsedTime_s("Status"))+"s to simulate the past "+to_scientific_notation(statusStep_s)+"s");
      profiler.Reset("Status");
    }

    // Check to see if we are stable
    if (stabilizationTimer_s > stabilizationCheck_s)
    {
      size_t idx=0;
      bool allPassed = true;
      for (PulseController* pc : engines)
      {
        double currentSpO2 = pc->GetBloodChemistry().GetOxygenSaturation().GetValue();
        double pctDiff = GeneralMath::PercentDifference(previsouSpO2s[idx++], currentSpO2);
        if (pctDiff > pctDiffSpO2)
          allPassed = false;
      }
      if (allPassed)
        stabilizationPasses++;
      else
      {
        idx = 0;
        stabilizationPasses = 0;
        for (PulseController* pc : engines)
        {
          previsouSpO2s[idx++]= pc->GetBloodChemistry().GetOxygenSaturation().GetValue();
        }
      }
      stabilizationTimer_s = 0;
      if (stabilizationPasses == 5)
      {
        break;
        double stabilizationTime = (totalIterations - minIterations) * timeStep_s;
        Info("Stabilization took " + to_scientific_notation(stabilizationTime) + "s to for this simulation");
      }
      if (totalIterations > maxIterations)
      {
        break;
        Info("Reached maximum iterations. Ending simulation.");
      }
    }
  }
  // Add our results to the simulation data
  for (int p = 0; p < sim.patientcomparisons_size(); p++)
  {
    PulseController* pc = engines[p];
    double SpO2 = pc->GetBloodChemistry().GetOxygenSaturation().GetValue();
    auto* multiVentilation = (*sim.mutable_patientcomparisons())[p].mutable_multiplexventilation();
    multiVentilation->set_oxygensaturation(SpO2);
    multiVentilation->set_tidalvolume_ml(pc->GetRespiratory().GetTidalVolume(VolumeUnit::mL));
    multiVentilation->set_endtidalcarbondioxidepressure_cmh2o(pc->GetRespiratory().GetEndTidalCarbonDioxidePressure(PressureUnit::cmH2O));
    auto AortaO2 = pc->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetSubstanceQuantity(pc->GetSubstances().GetO2());
    multiVentilation->set_arterialoxygenpartialpressure_mmhg(AortaO2->GetPartialPressure(PressureUnit::mmHg));
    auto AortaCO2 = pc->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetSubstanceQuantity(pc->GetSubstances().GetCO2());
    multiVentilation->set_arterialcarbondioxidepartialpressure_mmhg(AortaCO2->GetPartialPressure(PressureUnit::mmHg));
    multiVentilation->set_carricoindex_mmhg(pc->GetRespiratory().GetCarricoIndex(PressureUnit::mmHg));
    multiVentilation->set_achievedstabilization(!(totalIterations > maxIterations));
    multiVentilation->set_stabilizationtime_s((totalIterations - minIterations)* timeStep_s);
    multiVentilation->set_oxygensaturationstabilizationtrend((SpO2 - previsouSpO2s[p]) / timeStep_s);
  }
  DELETE_VECTOR(engines);
  Info("It took "+to_scientific_notation(profiler.GetElapsedTime_s("Total"))+"s to run this simulation");
  
  profiler.Clear();
  return true;
}
