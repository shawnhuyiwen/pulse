/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "MVEngine.h"

class MVRunner : public Loggable
{
  friend class MVController;
public:
  enum class Mode {StepFiO2, StableSpO2};
  MVRunner(const std::string& logfileName, const std::string& dataDir = ".");
  virtual ~MVRunner();

  bool Run(const std::string& filename, SerializationFormat f, Mode m);
  bool Run(pulse::study::multiplex_ventilation::bind::SimulationListData& simList, Mode m);

  static bool StepSimulationFiO2(pulse::study::multiplex_ventilation::bind::SimulationData& sim, const std::string& dataDir = "./");
  static bool RunSimulationToStableSpO2(pulse::study::multiplex_ventilation::bind::SimulationData& sim, const std::string& dataDir="./");
protected:
  bool Run();
  bool SerializeToString(pulse::study::multiplex_ventilation::bind::SimulationListData& src, std::string& dst, SerializationFormat f) const;
  bool SerializeToFile(pulse::study::multiplex_ventilation::bind::SimulationListData& src, const std::string& filename, SerializationFormat f) const;
  bool SerializeFromString(const std::string& src, pulse::study::multiplex_ventilation::bind::SimulationListData& dst, SerializationFormat f);
  bool SerializeFromFile(const std::string& filename, pulse::study::multiplex_ventilation::bind::SimulationListData& dst, SerializationFormat f);

  void ControllerLoop();
  void FinalizeSimulation(pulse::study::multiplex_ventilation::bind::SimulationData& sim);
  pulse::study::multiplex_ventilation::bind::SimulationData* GetNextSimulation();

  std::mutex  m_mutex;
  bool m_Running;
  Mode m_Mode;

  std::string m_DataDir;
  std::string m_SimulationResultsListFile;
  std::set<int> m_SimulationsToRun;
  std::vector<std::thread>   m_Threads;
  pulse::study::multiplex_ventilation::bind::SimulationListData* m_SimulationList;
  pulse::study::multiplex_ventilation::bind::SimulationListData* m_SimulationResultsList;
};
