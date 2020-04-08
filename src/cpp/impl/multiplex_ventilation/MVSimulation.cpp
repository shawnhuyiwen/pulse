/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

bool MVController::RunSimulation(pulse::multiplex_ventilator::bind::SimulationData& sim)
{
  SESubstanceManager* subMgr;
  bool   enableMultiplexVentilation;
  double timeStep_s = 0.02;
  double currentTime_s = 0;

  SEFluidCircuit* multiplexVentilationCircuit=nullptr;
  SEGasCompartmentGraph* multiplexVentilationGraph = nullptr;
  SEFluidCircuitCalculator* calculator = nullptr;
  SEGasTransporter* transporter = nullptr;
  SELiquidTransporter* aerosolTransporter = nullptr;

  std::vector<PulseController*> engines;

  calculator = new SEFluidCircuitCalculator(VolumePerPressureUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s,
    PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O,
    VolumeUnit::L, PressureTimePerVolumeUnit::cmH2O_s_Per_L, GetLogger());
  transporter = new SEGasTransporter(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, GetLogger());
  //aerosolTransporter = new SELiquidTransporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, GetLogger());

  SEFluidCircuitNode* inspiratoryConnectionNode = nullptr;
  SEFluidCircuitNode* expiratoryConnectionNode = nullptr;

  SEGasCompartment* inspiratoryConnectionCompartment = nullptr;
  SEGasCompartment* expiratoryConnectionCompartment = nullptr;

  int i = 0;
  for (int p=0; p<sim.patientcomparisons_size(); p++)
  {
    std::string state = sim.patientcomparisons()[p].soloventilation().statefile();
    PulseController* pc = new PulseController(m_BaseFileName + "_p" + std::to_string(i++) + ".log");
    pc->SerializeFromFile(state, SerializationFormat::JSON);

    // Build our multiplex circuit
    if (i == 0)
    {
      // Let's add the first mechanical ventilator circuit to our circuit
      // This will add the ventialtor and the respiratory system
      subMgr = &pc->GetSubstances();
      for (SEFluidCircuitNode* node : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNodes())
        multiplexVentilationCircuit->ForceAddNode(*node);
      for (SEFluidCircuitPath* path : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPaths())
        multiplexVentilationCircuit->ForceAddPath(*path);
      for (SEGasCompartment* cmpt : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartments())
        multiplexVentilationGraph->ForceAddCompartment(*cmpt);
      for (SEGasCompartmentLink* link : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetLinks())
        multiplexVentilationGraph->ForceAddLink(*link);

      inspiratoryConnectionNode = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::InspiratoryValve);
      expiratoryConnectionNode = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::ExpiratoryValve);

      inspiratoryConnectionCompartment = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartment(pulse::MechanicalVentilatorCompartment::InspiratoryValve);
      expiratoryConnectionCompartment = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartment(pulse::MechanicalVentilatorCompartment::ExpiratoryValve);
    }
    else
    {
      // Add all the nodes/paths/compartments/links to our circuit/graph
      for (SEFluidCircuitNode* node : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNodes())
        multiplexVentilationCircuit->ForceAddNode(*node);
      for (SEFluidCircuitPath* path : pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPaths())
        multiplexVentilationCircuit->ForceAddPath(*path);
      for (SEGasCompartment* cmpt : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartments())
        multiplexVentilationGraph->ForceAddCompartment(*cmpt);
      for (SEGasCompartmentLink* link : pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetLinks())
        multiplexVentilationGraph->ForceAddLink(*link);

      // Remove the ventilator, since we're using the first patient's to share
      // Aaron - Do these need a ForceRemove?
      SEFluidCircuitNode* node = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::Ventilator);
      multiplexVentilationCircuit->RemoveNode(*node);
      SEFluidCircuitPath* path = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::EnvironmentToVentilator);
      multiplexVentilationCircuit->RemovePath(*path);
      path = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::VentilatorToExpiratoryValve);
      multiplexVentilationCircuit->RemovePath(*path);
      path = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::VentilatorToInspiratoryValve);
      multiplexVentilationCircuit->RemovePath(*path);

      SEGasCompartment* compartment = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartment(pulse::MechanicalVentilatorCompartment::MechanicalVentilator);
      multiplexVentilationGraph->RemoveCompartment(*compartment);
      SEGasCompartmentLink* link = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetLink(pulse::MechanicalVentilatorLink::MechanicalVentilatorToExpiratoryValve);
      multiplexVentilationGraph->RemoveLink(*link);
      link = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetLink(pulse::MechanicalVentilatorLink::MechanicalVentilatorToInspiratoryValve);
      multiplexVentilationGraph->RemoveLink(*link);

      // Connect the tubes to the shared ventilator circuit/graph
      node = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::InspiratoryValve);
      path = &multiplexVentilationCircuit->CreatePath(*inspiratoryConnectionNode, *node, "???"); //Aaron - this needs a unique name for this patient
      node = pc->GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::ExpiratoryValve);
      path = &multiplexVentilationCircuit->CreatePath(*expiratoryConnectionNode, *node, "???"); //Aaron - this needs a unique name for this patient

      //Aaron it doesn't like these commented out lines for some reason
      compartment = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartment(pulse::MechanicalVentilatorCompartment::InspiratoryValve);
      //link = &multiplexVentilationGraph->CreateGasLink(*inspiratoryConnectionCompartment, *compartment, "???"); //Aaron - this needs a unique name for this patient
      compartment = pc->GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph().GetCompartment(pulse::MechanicalVentilatorCompartment::ExpiratoryValve);
      ///link = &multiplexVentilationGraph->CreateGasLink(*expiratoryConnectionCompartment, *compartment, "???"); //Aaron - this needs a unique name for this patient
    }

    TrackData(pc->GetEngineTracker(), m_BaseFileName+"_p"+std::to_string(i)+".csv");
    engines.push_back(pc);
  }
  multiplexVentilationCircuit->StateChange();
  multiplexVentilationGraph->StateChange();

  SEMechanicalVentilatorConfiguration config(*subMgr);

  int vent_count;
  int count = (int)(60 / timeStep_s);
  for (int i = 0; i < count; i++)
  {
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
      calculator->Process(*multiplexVentilationCircuit, timeStep_s);
      // Transport the multiplex graph
      transporter->Transport(*multiplexVentilationGraph, timeStep_s);
      // TODO Consider adding aerosol support

      // TODO Copy mv node/path/cmpt/link values to engines>1
      // We are using engine 1's MV circuit/graph, so its up to date
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
    for (PulseController* pc : engines)
      ((Respiratory&)pc->GetRespiratory()).PostProcess();
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
    currentTime_s += timeStep_s;

    // TODO Check to see if we are stable
  }
  // TODO Write out a results file
  DELETE_VECTOR(engines);
  return true;
}
