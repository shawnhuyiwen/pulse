/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
#include "physiology/BloodChemistry.h"
#include "physiology/Cardiovascular.h"
#include "physiology/Drugs.h"
#include "physiology/Endocrine.h"
#include "physiology/Energy.h"
#include "physiology/Gastrointestinal.h"
#include "physiology/Hepatic.h"
#include "physiology/Nervous.h"
#include "physiology/Renal.h"
#include "physiology/Respiratory.h"
#include "physiology/Saturation.h"
#include "physiology/Tissue.h"
#include "environment/Environment.h"
#include "equipment/AnesthesiaMachine.h"
#include "equipment/ECG.h"
#include "equipment/Inhaler.h"
#include "equipment/MechanicalVentilator.h"

#include "engine/SEEngineTracker.h"
#include "engine/SEDataRequestManager.h"

#include "substance/SESubstanceTransport.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"

#include "utils/DataTrack.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerPressure.h"

MVController::MVController(const std::string& logFileName, const std::string& data_dir) : Loggable(new Logger(logFileName))
{
  m_BaseFileName = logFileName.substr(0, logFileName.length() - 4);
}
MVController::~MVController()
{

}

bool MVController::Run(std::vector<std::string>& patients)
{
  SESubstanceManager* subMgr;
  bool   enableMultiplexVentilation;
  double timeStep_s = 0.02;
  double currentTime_s = 0;

  SEFluidCircuit* multiplexVentilationCircuit;
  SEGasCompartmentGraph* multiplexVentilationGraph;
  SEFluidCircuitCalculator* calculator;
  SEGasTransporter* transporter;
  SELiquidTransporter* aerosolTransporter;

  std::vector<PulseController*> engines;

  calculator = new SEFluidCircuitCalculator(VolumePerPressureUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s,
    PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O,
    VolumeUnit::L, PressureTimePerVolumeUnit::cmH2O_s_Per_L, GetLogger());
  transporter = new SEGasTransporter(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, GetLogger());
  //aerosolTransporter = new SELiquidTransporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, GetLogger());

  // jbw Grab whatever nodes/compartments you need to connect everything
  SEFluidCircuitNode* connectionNode = nullptr; // ex. Set this in a loop below
  SEGasCompartment* connectionCmpt = nullptr;// ex. Set this in a loop below
  int i=0;
  for (std::string state : patients)
  {
    PulseController* pc = new PulseController(m_BaseFileName+"_p"+std::to_string(i++)+".log");
    pc->SerializeFromFile(state, SerializationFormat::JSON);

    // Build our multiplex circuit
    // Let's add the first mechanical ventilator circuit to our circuit
    if (i == 0)
    {
      subMgr = &pc->GetSubstances();
      for (SEFluidCircuitNode* node : pc->GetCircuits().GetMechanicalVentilatorCircuit().GetNodes())
        multiplexVentilationCircuit->AddNode(*node);
      for (SEFluidCircuitPath* path : pc->GetCircuits().GetMechanicalVentilatorCircuit().GetPaths())
        multiplexVentilationCircuit->AddPath(*path);
      for (SEGasCompartment* cmpt : pc->GetCompartments().GetMechanicalVentilatorGraph().GetCompartments())
        multiplexVentilationGraph->AddCompartment(*cmpt);
      for (SEGasCompartmentLink* link : pc->GetCompartments().GetMechanicalVentilatorGraph().GetLinks())
        multiplexVentilationGraph->AddLink(*link);
    }
    // Add all the nodes/paths/compartments/links to our circuit/graph
    for (SEFluidCircuitNode* node : pc->GetCircuits().GetRespiratoryCircuit().GetNodes())
      multiplexVentilationCircuit->AddNode(*node);
    for (SEFluidCircuitPath* path : pc->GetCircuits().GetRespiratoryCircuit().GetPaths())
      multiplexVentilationCircuit->AddPath(*path);
    for (SEGasCompartment* cmpt : pc->GetCompartments().GetRespiratoryGraph().GetCompartments())
      multiplexVentilationGraph->AddCompartment(*cmpt);
    for (SEGasCompartmentLink* link : pc->GetCompartments().GetRespiratoryGraph().GetLinks())
      multiplexVentilationGraph->AddLink(*link);

    // jbw Add new path/link to connect patient respiratory system to mechanical ventilator

    pc->GetEngineTracker().GetDataRequestManager().SetResultsFilename(m_BaseFileName+"_p"+std::to_string(i)+".csv");
    pc->GetEngineTracker().GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PeakInspiratoryPressure", PressureUnit::cmH2O);
    pc->GetEngineTracker().GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PositiveEndExpiredPressure", PressureUnit::cmH2O);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("TotalRespiratoryModelCompliance", VolumePerPressureUnit::L_Per_cmH2O);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::L);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::cmH2O);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryExpiratoryRatio");
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
    // TODO Add any probes we may want... 
    pc->GetEngineTracker().SetupRequests();
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
      if(pc->GetEvents().IsEventActive(eEvent::IrreversibleState))
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
      //pc->GetEngineTracker().GetDataTrack().Probe("AlveolarImpairmentSeverity",
      //  m_ImpairedAlveolarExchangeExacerbation->GetImpairedFraction().GetValue());
      //pc->GetEngineTracker().GetDataTrack().Probe("FiO2", m_FiO2->GetFractionAmount().GetValue());
      pc->GetEngineTracker().TrackData(currentTime_s);
    }
    currentTime_s += timeStep_s;

    // TODO Check to see if we are stable
  }
  // TODO Write out a results file
  DELETE_VECTOR(engines);
  return true;
}
