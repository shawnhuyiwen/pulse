/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "MVEngine.h"

class MVGenerator : public Loggable
{
  friend class MVController;
public:
  MVGenerator(const std::string& logfileName, const std::string& dataDir = ".");
  virtual ~MVGenerator();

  void SetComplianceIterations_mL_per_cmH2O(int min, int max, int step);
  void SetPEEPIterations_cmH2O(int min, int max, int step);
  void SetImpairmentIterations(double min, double max, double step);

  bool Run(const std::string& stateDir, const std::string listFilename);

  static bool StabilizeSpO2(PhysiologyEngine& eng);
  static bool GenerateStabilizedPatient(pulse::study::bind::multiplex_ventilation::PatientStateData& pData, bool logToConsole);


  static double DefaultResistance_cmH2O_s_Per_L() { return 5; }
  static double DefaultRespirationRate_Per_Min() { return 20; }
  static double DefaultIERatio() { return 0.5; }
protected:
  bool SerializeToString(pulse::study::bind::multiplex_ventilation::PatientStateListData& src, std::string& dst, SerializationFormat f) const;
  bool SerializeToFile(pulse::study::bind::multiplex_ventilation::PatientStateListData& src, const std::string& filename) const;
  bool SerializeFromString(const std::string& src, pulse::study::bind::multiplex_ventilation::PatientStateListData& dst, SerializationFormat f);
  bool SerializeFromFile(const std::string& filename);

  void GeneratePatientList();
  void ControllerLoop();
  void FinalizePatient(pulse::study::bind::multiplex_ventilation::PatientStateData& sim);
  pulse::study::bind::multiplex_ventilation::PatientStateData* GetNextPatient();

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
  float m_AmbientFiO2 = 0.21f;

  std::string m_DataDir;
  std::mutex  m_mutex;
  bool m_Running;

  std::string m_PatientStateListFile;
  std::set<int> m_PatientsToRun;
  std::vector<std::thread>   m_Threads;
  pulse::study::bind::multiplex_ventilation::PatientStateListData* m_PatientList;
  pulse::study::bind::multiplex_ventilation::PatientStateListData* m_CompletedPatientList;
};
