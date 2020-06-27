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
#include "physiology/Cardiovascular.h"

#include "engine/SEOverrides.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEPatientConfiguration.h"

#include "substance/SESubstanceManager.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
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
#include "pulse/study/bind/Hydrocephalus.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS()

class HRunner : public Loggable
{
public:
  HRunner(const std::string& logfileName, const std::string& dataDir = ".");
  virtual ~HRunner();

  bool Run(const std::string& filename, SerializationFormat f);
  bool Run(pulse::study::bind::hydrocephalus::SimulationListData& simList);

  bool RunSimulationUntilStable(std::string const& outDir, pulse::study::bind::hydrocephalus::SimulationData& sim, const std::string& dataDir="./");
protected:
  bool Run();
  bool SerializeToString(pulse::study::bind::hydrocephalus::SimulationListData& src, std::string& dst, SerializationFormat f) const;
  bool SerializeToFile(pulse::study::bind::hydrocephalus::SimulationListData& src, const std::string& filename, SerializationFormat f) const;
  bool SerializeFromString(const std::string& src, pulse::study::bind::hydrocephalus::SimulationListData& dst, SerializationFormat f);
  bool SerializeFromFile(const std::string& filename, pulse::study::bind::hydrocephalus::SimulationListData& dst, SerializationFormat f);

  void ControllerLoop();
  void FinalizeSimulation(pulse::study::bind::hydrocephalus::SimulationData& sim);
  pulse::study::bind::hydrocephalus::SimulationData* GetNextSimulation();

  std::mutex  m_mutex;
  bool m_Running;

  std::string m_OutDir;
  std::string m_DataDir;
  std::string m_SimulationResultsListFile;
  std::set<int> m_SimulationsToRun;
  std::vector<std::thread>   m_Threads;
  pulse::study::bind::hydrocephalus::SimulationListData* m_SimulationList;
  pulse::study::bind::hydrocephalus::SimulationListData* m_SimulationResultsList;

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
