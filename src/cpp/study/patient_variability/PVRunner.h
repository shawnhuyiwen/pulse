/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include <thread>
#include <map>

#include "PulseEngine.h"

PUSH_PROTO_WARNINGS
#include "pulse/study/bind/PatientVariability.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS
#include "io/protobuf/PBUtils.h"

namespace pulse::study::patient_variability
{
  class PVRunner : public Loggable
  {
  public:
    PVRunner(const std::string& rootDir, Logger* logger=nullptr);
    virtual ~PVRunner();

    bool PostProcessOnly = false;
    eSerializationFormat SerializationFormat = eSerializationFormat::JSON;

    bool Run(const std::string& filename);
    bool Run(pulse::study::bind::patient_variability::PatientStateListData& patients);

  protected:
    bool Run();
    bool SerializeToString(pulse::study::bind::patient_variability::PatientStateListData& src, std::string& dst) const;
    bool SerializeToFile(pulse::study::bind::patient_variability::PatientStateListData& src, const std::string& filename) const;
    bool SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::PatientStateListData& dst);
    bool SerializeFromFile(const std::string& filename, pulse::study::bind::patient_variability::PatientStateListData& dst);

    void ControllerLoop();
    bool RunPatient(pulse::study::bind::patient_variability::PatientStateData& patient);
    pulse::study::bind::patient_variability::PatientStateData* GetNextPatient();

    bool AggregateResults(pulse::study::bind::patient_variability::PatientStateData& patient, const std::vector<std::string>& validation_files, Logger* logger);

    std::mutex  m_mutex;

    std::string m_RootDir;
    std::string m_PatientResultsListFile;
    std::set<unsigned int> m_PatientsToRun;
    std::vector<std::thread>   m_Threads;
    pulse::study::bind::patient_variability::PatientStateListData* m_PatientList;
    pulse::study::bind::patient_variability::PatientStateListData* m_PatientResultsList;

  };
}
