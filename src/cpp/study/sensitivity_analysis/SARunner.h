/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include <thread>
#include "PulsePhysiologyEngine.h"
#include "PulseConfiguration.h"

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
#include "engine/SEPatientConfiguration.h"

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
#include "properties/SERunningAverage.h"
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

#include "io/protobuf/PBActions.h"
PUSH_PROTO_WARNINGS()
#include "pulse/study/bind/SensitivityAnalysis.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS()

class SARunner : public Loggable
{
public:
  SARunner(const std::string& logfileName, const std::string& dataDir = ".");
  virtual ~SARunner();

  bool Run(const std::string& filename, SerializationFormat f);
  bool Run(pulse::study::bind::sensitivity_analysis::SimulationListData& simList);

  bool RunSimulationUntilStable(std::string const& outDir, pulse::study::bind::sensitivity_analysis::SimulationData& sim, const std::string& dataDir="./");
  bool RunSimulation(std::string const& outDir, pulse::study::bind::sensitivity_analysis::SimulationData& sim, const std::string& dataDir = "./");

protected:
  bool Run();
  bool SerializeToString(pulse::study::bind::sensitivity_analysis::SimulationListData& src, std::string& dst, SerializationFormat f) const;
  bool SerializeToFile(pulse::study::bind::sensitivity_analysis::SimulationListData& src, const std::string& filename, SerializationFormat f) const;
  bool SerializeFromString(const std::string& src, pulse::study::bind::sensitivity_analysis::SimulationListData& dst, SerializationFormat f);
  bool SerializeFromFile(const std::string& filename, pulse::study::bind::sensitivity_analysis::SimulationListData& dst, SerializationFormat f);

  void ControllerLoop();
  void FinalizeSimulation(pulse::study::bind::sensitivity_analysis::SimulationData& sim);
  pulse::study::bind::sensitivity_analysis::SimulationData* GetNextSimulation();

  std::mutex  m_mutex;
  bool m_Running;

  std::string m_OutDir;
  std::string m_DataDir;
  std::string m_SimulationResultsListFile;
  std::set<int> m_SimulationsToRun;
  std::vector<std::thread>   m_Threads;
  pulse::study::bind::sensitivity_analysis::SimulationListData* m_SimulationList;
  pulse::study::bind::sensitivity_analysis::SimulationListData* m_SimulationResultsList;

private:
  // struct to hold new running average data
  struct RunningAverages
  {
    RunningAverages(const SELiquidCompartment* c, const PressureUnit& u)
    {
      cmpt = c;
      pUnit = &u;
    }
    RunningAverages(const SELiquidCompartment* c, const VolumePerTimeUnit& u)
    {
      cmpt = c;
      fUnit = &u;
    }
    RunningAverages(const SELiquidSubstanceQuantity* sq, const PressureUnit& u)
    {
      subQ = sq;
      pUnit = &u;
    }
    double instantaneousAverage=0.;
    const SELiquidCompartment* cmpt = nullptr;
    const SELiquidSubstanceQuantity* subQ = nullptr;
    const PressureUnit* pUnit = nullptr;
    const VolumePerTimeUnit* fUnit = nullptr;
    SERunningAverage runningAverage;

    double Sample()
    {
      if (cmpt != nullptr)
      {
        if (fUnit != nullptr)
          return runningAverage.Sample(cmpt->GetInFlow(*fUnit));
        else if (pUnit != nullptr)
          return runningAverage.Sample(cmpt->GetPressure(*pUnit));
      }
      else if(subQ != nullptr)
        return runningAverage.Sample(subQ->GetPartialPressure(*pUnit));
      return 0;// We should not get here
    }
  };
};
