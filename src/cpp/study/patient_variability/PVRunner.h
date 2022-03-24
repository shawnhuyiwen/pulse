/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include <thread>
#include "PulseEngine.h"

#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/TimingProfile.h"

#include "cdm/io/protobuf/PBActions.h"
PUSH_PROTO_WARNINGS
#include "pulse/study/bind/PatientVariability.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS

namespace pulse::study::patient_variability
{
  class PVRunner : public Loggable
  {
  public:
    PVRunner(const std::string& logfileName, const std::string& dataDir = ".");
    virtual ~PVRunner();

    bool Run(const std::string& filename, eSerializationFormat f);
    bool Run(pulse::study::bind::patient_variability::SimulationListData& simList);

    bool RunSimulationUntilStable(std::string const& outDir, pulse::study::bind::patient_variability::SimulationData& sim, const std::string& dataDir = "./");

  protected:
    bool Run();
    bool SerializeToString(pulse::study::bind::patient_variability::SimulationListData& src, std::string& dst, eSerializationFormat f) const;
    bool SerializeToFile(pulse::study::bind::patient_variability::SimulationListData& src, const std::string& filename, eSerializationFormat f) const;
    bool SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::SimulationListData& dst, eSerializationFormat f);
    bool SerializeFromFile(const std::string& filename, pulse::study::bind::patient_variability::SimulationListData& dst, eSerializationFormat f);

    void ControllerLoop();
    void FinalizeSimulation(pulse::study::bind::patient_variability::SimulationData& sim);
    pulse::study::bind::patient_variability::SimulationData* GetNextSimulation();

    std::mutex  m_mutex;
    bool m_Running;

    std::string m_OutDir;
    std::string m_DataDir;
    std::string m_SimulationResultsListFile;
    std::set<unsigned int> m_SimulationsToRun;
    std::vector<std::thread>   m_Threads;
    pulse::study::bind::patient_variability::SimulationListData* m_SimulationList;
    pulse::study::bind::patient_variability::SimulationListData* m_SimulationResultsList;
  };
}