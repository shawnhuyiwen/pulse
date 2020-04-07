/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologyEngine.h"

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

#include "pulse/impl/bind/MultiplexVentilator.pb.h"
#include <google/protobuf/util/json_util.h>

class MVController : public Loggable, protected SEEventHandler
{
public:
  MVController(const std::string& logfileName, const std::string& data_dir = ".");
  virtual ~MVController();


  bool GenerateStabilizedPatients();

  bool RunSimulation(pulse::multiplex_ventilator::bind::SimulationData& sim);

  double DefaultIERatio() const { return m_IERatio; }
  double DefaultRespirationRate_Per_Min() const { return m_RespirationRate_Per_Min; }

protected:
  std::string to_scientific_notation(float f);
  void TrackData(SEEngineTracker& trkr, const std::string& csv_filename);
  void HandleEvent(eEvent e, bool active, const SEScalarTime* simTime = nullptr) override;

  std::string m_BaseFileName;
  // Constants
  int   m_Resistance_cmH2O_s_Per_L = 5;
  int   m_RespirationRate_Per_Min = 20;
  float m_IERatio = 0.5f;
  float m_AmbientFiO2 = 0.21f;
};
