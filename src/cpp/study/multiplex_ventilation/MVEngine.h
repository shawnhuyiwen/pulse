/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include <thread>
#include "PulsePhysiologyEngine.h"

#include "controller/Engine.h"
#include "controller/Controller.h"
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

#include "patient/SEPatient.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SEImpairedAlveolarExchangeExacerbation.h"
#include "patient/actions/SEPulmonaryShuntExacerbation.h"
#include "equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"

#include "engine/SEOverrides.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEDataRequestManager.h"

#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceTransport.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"

#include "utils/DataTrack.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "utils/FileUtils.h"
#include "utils/GeneralMath.h"
#include "utils/TimingProfile.h"

PUSH_PROTO_WARNINGS()
#include "pulse/study/bind/MultiplexVentilation.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS()
#include "io/protobuf/PBUtils.h"

bool ExtractInitialConditions(const std::string& filePath, double& comp, double& imp);
bool ExtractVentilatorSettings(const std::string& filePath, std::string& fileName, double& pip_cmH2O, double& peep_cmH2O, double& FiO2);

struct Dir
{
  static const std::string Base;
  static const std::string Solo;
  static const std::string Results;
};

class PULSE_DECL MVEngine : public Loggable, public SEEventHandler
{
  friend class MVRunner;
  friend class MVGenerator;
public:
  MVEngine(std::string const& logfile = "", bool cout_enabled = true, std::string const& data_dir = "./");
  ~MVEngine();

  bool CreateEngine(const std::string& simulationDataStr, SerializationFormat fmt);
  bool CreateEngine(pulse::study::bind::multiplex_ventilation::SimulationData& sim);

  bool AdvanceTime(double time_s);

  bool ProcessAction(const SEAction& action);
  bool ProcessActions(std::string const& actions, SerializationFormat format);

  std::string GetSimulationState(SerializationFormat fmt);
  bool GetSimulationState(pulse::study::bind::multiplex_ventilation::SimulationData& sim);

  void DestroyEngines();

  static void TrackData(SEEngineTracker& trkr, const std::string& csv_filename);
  static bool RunSoloState(const std::string& stateFile, const std::string& outDir, double duration_s, Logger& logger);

  static bool SerializeToString(pulse::study::bind::multiplex_ventilation::SimulationData& src, std::string& dst, SerializationFormat f);
  static bool SerializeFromString(const std::string& src, pulse::study::bind::multiplex_ventilation::SimulationData& dst, SerializationFormat f);

protected:
  void SetFiO2(double fio2);
  double GetMinSpO2();
  double GetMinPAO2_mmHg();
  void HandleEvent(eEvent e, bool active, const SEScalarTime* simTime = nullptr) override;

  
  std::string m_DataDir;
  std::vector<PulseEngine*> m_Engines;
  std::vector<PulseController*> m_Controllers;
  std::vector<SELiquidSubstanceQuantity*> m_AortaO2s;
  std::vector<SELiquidSubstanceQuantity*> m_AortaCO2s;
  SEMechanicalVentilatorConfiguration* m_MVC;
  SESubstanceFraction* m_FiO2;


  double                    m_TimeStep_s;
  double                    m_CurrentTime_s;
  double                    m_SpareAdvanceTime_s;
  // Substances
  SESubstanceManager*       m_SubMgr;
  SESubstance*              m_Oxygen;
  // Circuits
  SECircuitManager*         m_CircuitMgr;
  SEFluidCircuit*           m_MultiplexVentilationCircuit;
  SEFluidCircuitCalculator* m_Calculator;
  // Compartments
  SECompartmentManager*     m_CmptMgr;
  SEGasCompartmentGraph*    m_MultiplexVentilationGraph;
  SEGasTransporter*         m_Transporter;

  pulse::study::bind::multiplex_ventilation::SimulationData* m_SimulationData;
};
