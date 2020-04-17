/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "MVController.h"

class MVGenerator : public Loggable
{
  friend class MVController;
public:
  MVGenerator(const std::string& logfileName, const std::string& dataDir = ".");
  virtual ~MVGenerator();

  void SetComplianceIterations_mL_per_cmH2O(int min, int max, int step);
  void SetPEEPIterations_cmH2O(int min, int max, int step);
  void SetImpairmentIterations(double min, double max, double step);

  bool Run(const std::string& stateDir, const std::string listFilename, SerializationFormat f);

protected:
  bool SerializeToString(pulse::study::multiplex_ventilation::bind::PatientStateListData& src, std::string& dst, SerializationFormat f) const;
  bool SerializeToFile(pulse::study::multiplex_ventilation::bind::PatientStateListData& src, const std::string& filename, SerializationFormat f) const;
  bool SerializeFromString(const std::string& src, pulse::study::multiplex_ventilation::bind::PatientStateListData& dst, SerializationFormat f);
  bool SerializeFromFile(const std::string& filename, SerializationFormat f);

  void GeneratePatientList();
  void ControllerLoop();
  void FinalizePatient(pulse::study::multiplex_ventilation::bind::PatientStateData& sim);
  pulse::study::multiplex_ventilation::bind::PatientStateData* GetNextPatient();

  int m_MinCompliance_mL_Per_cmH2O  = 10;
  int m_MaxCompliance_mL_Per_cmH2O  = 50;
  int m_StepCompliance_mL_Per_cmH2O = 10;

  int m_MinPEEP_cmH2O  = 10;
  int m_MaxPEEP_cmH2O  = 20;
  int m_StepPEEP_cmH2O = 5;

  double m_MinImpairment  = 0.3;
  double m_MaxImpairment  = 0.9;
  double m_StepImpairment = 0.025;

  // Constants
  int   m_Resistance_cmH2O_s_Per_L = 5;
  int   m_RespirationRate_Per_Min = 20;
  float m_IERatio = 0.5f;
  float m_AmbientFiO2 = 0.21f;

  std::string m_DataDir;
  std::mutex  m_mutex;
  bool m_Running;

  std::string m_PatientStateListFile;
  std::set<int> m_PatientsToRun;
  std::vector<std::thread>   m_Threads;
  pulse::study::multiplex_ventilation::bind::PatientStateListData* m_PatientList;
  pulse::study::multiplex_ventilation::bind::PatientStateListData* m_CompletedPatientList;
};
