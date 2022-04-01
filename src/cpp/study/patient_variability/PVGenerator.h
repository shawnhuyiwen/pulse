/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PVEngine.h"

namespace pulse::study::patient_variability
{
  class PVGenerator : public Loggable
  {
  public:
    PVGenerator(const std::string& logfileName, const std::string& dataDir = ".");
    virtual ~PVGenerator();

    bool Run(const std::string& stateDir, const std::string listFilename);

    static bool GenerateStabilizedPatient(pulse::study::bind::patient_variability::PatientStateData& pData, bool logToConsole);

  protected:
    bool SerializeToString(pulse::study::bind::patient_variability::PatientStateListData& src, std::string& dst, eSerializationFormat f) const;
    bool SerializeToFile(pulse::study::bind::patient_variability::PatientStateListData& src, const std::string& filename) const;
    bool SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::PatientStateListData& dst, eSerializationFormat f);
    bool SerializeFromFile(const std::string& filename);

    void GeneratePatientList(const std::string& stateDir);
    void ControllerLoop();
    void FinalizePatient(pulse::study::bind::patient_variability::PatientStateData& sim);
    pulse::study::bind::patient_variability::PatientStateData* GetNextPatient();

    std::string m_DataDir;
    std::mutex  m_mutex;
    bool m_Running;

    std::string m_PatientStateListFile;
    std::set<unsigned int> m_PatientsToRun;
    std::vector<std::thread>   m_Threads;
    pulse::study::bind::patient_variability::PatientStateListData* m_PatientList;
    pulse::study::bind::patient_variability::PatientStateListData* m_CompletedPatientList;
  };
}