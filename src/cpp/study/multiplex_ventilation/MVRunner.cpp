/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVRunner.h"

MVRunner::MVRunner(const std::string& logfileName, const std::string& dataDir) : Loggable(logfileName)
{
  m_DataDir = dataDir;
  m_SimulationList = nullptr;
  m_SimulationResultsList = nullptr;
}
MVRunner::~MVRunner()
{
  SAFE_DELETE(m_SimulationList);
  SAFE_DELETE(m_SimulationResultsList);
}

bool MVRunner::Run(pulse::study::multiplex_ventilation::bind::SimulationListData& simList)
{
  m_SimulationResultsListFile = simList.outputrootdir()+"/simlist_results.json";
  SAFE_DELETE(m_SimulationList);
  SAFE_DELETE(m_SimulationResultsList);
  m_SimulationList = &simList;
  m_SimulationResultsList = new pulse::study::multiplex_ventilation::bind::SimulationListData();
  bool b = Run();
  m_SimulationList = nullptr;
  SAFE_DELETE(m_SimulationResultsList);
  return b;
}

bool MVRunner::Run(const std::string& filename, SerializationFormat f)
{
  if (!SerializeFromFile(filename, f))
    return false;
  return Run();
}
bool MVRunner::Run()
{
  TimingProfile profiler;
  profiler.Start("Total");

  // Get the ID's of simulations we need to run
  m_SimulationsToRun.clear();
  for (int i = 0; i < m_SimulationList->simulations_size(); i++)
    m_SimulationsToRun.insert(m_SimulationList->simulations()[i].id());
  // Remove any id's we have in the results
  if (m_SimulationResultsList->simulations_size() > 0)
  {
    Info("Found " + std::to_string(m_SimulationResultsList->simulations_size()) + " previously run simulations");
    for (int i = 0; i < m_SimulationResultsList->simulations_size(); i++)
      m_SimulationsToRun.erase(m_SimulationResultsList->simulations()[i].id());
  }

  int numSimsToRun = m_SimulationList->simulations_size() - m_SimulationResultsList->simulations_size();
  if (numSimsToRun == 0)
  {
    Info("All simulations are run in the results file");
    return true;
  }
  size_t processor_count = 1;// std::thread::hardware_concurrency();
  if (processor_count == 0)
  {
    Fatal("Unable to detect number of processors");
    return false;
  }
  // Let's not kill the computer this is running on...
  if(processor_count > 1)
    processor_count -= 1; 
  // Let's not kick off more threads than we need
  if (processor_count > numSimsToRun)
    processor_count = numSimsToRun;
  Info("Starting " + std::to_string(processor_count) + " Threads to run "+std::to_string(m_SimulationsToRun.size())+" simulations");
  // Crank up our threads
  for (size_t p = 0; p < processor_count; p++)
    m_Threads.push_back(std::thread(&MVRunner::ControllerLoop, this));

  for (size_t p = 0; p < processor_count; p++)
    m_Threads[p].join();

  Info("It took " + to_scientific_notation(profiler.GetElapsedTime_s("Total")) + "s to run this simulation list");
  profiler.Clear();
  return true;
}

void MVRunner::ControllerLoop()
{
  pulse::study::multiplex_ventilation::bind::SimulationData* sim;
  while (true)
  {
    sim = GetNextSimulation();
    if (sim == nullptr)
      break;
    RunSimulationToStableSpO2(*sim);
    FinalizeSimulation(*sim);
  }
}

bool MVRunner::RunSimulationToStableSpO2(pulse::study::multiplex_ventilation::bind::SimulationData& sim, const std::string& dataDir)
{
  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");

  MVEngine mve(sim.outputbasefilename() + ".log", false, dataDir);
  if (!mve.CreateEngine(sim))
    return false;

  double timeStep_s = mve.m_TimeStep_s;
  double currentTime_s = 0;

  int stabilizationPasses = 0;
  int totalIterations = 0;
  double stabilizationTimer_s = 0;
  double stabilizationCheck_s = 2;
  // Let's shoot for with in 0.25% for 10s straight
  double pctDiffSpO2 = 0.25;
  std::vector<double> previsouSpO2s;
  for (PulseController* pc : mve.m_Engines)
    previsouSpO2s.push_back(pc->GetBloodChemistry().GetOxygenSaturation().GetValue());

  double statusTimer_s = 0;  // Current time of this status cycle
  double statusStep_s = 60; // How long did it take to simulate this much time

  int minIterations = (int)(60 / timeStep_s);
  int maxIterations = (int)(300 / timeStep_s);
  while (true)
  {
    totalIterations++;
    if (totalIterations == minIterations)
      mve.GetLogger()->Info("Minimum Runtime Achieved");

    if (!mve.AdvanceTime(timeStep_s))
      return false;
    // Increment our timers
    currentTime_s += timeStep_s;
    statusTimer_s += timeStep_s;
    // Start stabilization timer after minimum run time
    if (totalIterations > minIterations)
      stabilizationTimer_s += timeStep_s;

    // How are we running?
    if (statusTimer_s > statusStep_s)
    {
      statusTimer_s = 0;
      mve.GetLogger()->Info("Current Time is " + to_scientific_notation(currentTime_s) + "s, it took " + to_scientific_notation(profiler.GetElapsedTime_s("Status")) + "s to simulate the past " + to_scientific_notation(statusStep_s) + "s");
      profiler.Reset("Status");
    }

    // Check to see if we are stable
    if (stabilizationTimer_s > stabilizationCheck_s)
    {
      size_t idx = 0;
      bool allPassed = true;
      for (PulseController* pc : mve.m_Engines)
      {
        double currentSpO2 = pc->GetBloodChemistry().GetOxygenSaturation().GetValue();
        double pctDiff = GeneralMath::PercentDifference(previsouSpO2s[idx++], currentSpO2);
        if (pctDiff > pctDiffSpO2)
          allPassed = false;
      }
      if (allPassed)
        stabilizationPasses++;
      else
      {
        idx = 0;
        stabilizationPasses = 0;
        for (PulseController* pc : mve.m_Engines)
        {
          previsouSpO2s[idx++] = pc->GetBloodChemistry().GetOxygenSaturation().GetValue();
        }
      }
      stabilizationTimer_s = 0;
      if (stabilizationPasses == 5)
      {
        break;
        double stabilizationTime = (totalIterations - minIterations) * timeStep_s;
        mve.GetLogger()->Info("Stabilization took " + to_scientific_notation(stabilizationTime) + "s to for this simulation");
      }
      if (totalIterations > maxIterations)
      {
        break;
        mve.GetLogger()->Info("Reached maximum iterations. Ending simulation.");
      }
    }
  }
  mve.GetSimulationState(sim);
  // Add our stabilization numbers to the results
  for (int p = 0; p < sim.patientcomparisons_size(); p++)
  {
    PulseController* pc = mve.m_Engines[p];
    double SpO2 = pc->GetBloodChemistry().GetOxygenSaturation().GetValue();
    auto* multiVentilation = (*sim.mutable_patientcomparisons())[p].mutable_multiplexventilation();
    multiVentilation->set_achievedstabilization(!(totalIterations > maxIterations));
    multiVentilation->set_stabilizationtime_s((totalIterations - minIterations) * timeStep_s);
    multiVentilation->set_oxygensaturationstabilizationtrend((SpO2 - previsouSpO2s[p]) / timeStep_s);
  }
  mve.GetLogger()->Info("It took " + to_scientific_notation(profiler.GetElapsedTime_s("Total")) + "s to run this simulation");

  profiler.Clear();
  return true;
}

pulse::study::multiplex_ventilation::bind::SimulationData* MVRunner::GetNextSimulation()
{
  m_mutex.lock();
  pulse::study::multiplex_ventilation::bind::SimulationData* sim = nullptr;
  if (!m_SimulationsToRun.empty())
  {
    size_t id = *m_SimulationsToRun.begin();
    for (int i = 0; i < m_SimulationList->simulations_size(); i++)
    {
      sim = &(*m_SimulationList->mutable_simulations())[i];
      if (sim->id() == id)
        break;
    }
    Info("Simulating ID " + std::to_string(id));
    m_SimulationsToRun.erase(id);
  }
  m_mutex.unlock();
  return sim;
}

void MVRunner::FinalizeSimulation(pulse::study::multiplex_ventilation::bind::SimulationData& sim)
{
  m_mutex.lock();
  auto rSim = m_SimulationResultsList->mutable_simulations()->Add();
  rSim->CopyFrom(sim);
  SerializeToFile(*m_SimulationResultsList, m_SimulationResultsListFile, SerializationFormat::JSON);
  Info("Completed Simulation " + std::to_string(m_SimulationResultsList->simulations_size()) + " of " + std::to_string(m_SimulationList->simulations_size()));
  std::ofstream plots;
  plots.open(m_SimulationList->outputrootdir() + "/plot_pairs.config", std::fstream::in | std::fstream::out | std::fstream::app);
  plots << sim.outputbasefilename() << "multiplex_patient_0_results.csv, " << sim.outputbasefilename() << "multiplex_patient_1_results.csv\n";
  plots.close();
  m_mutex.unlock();
}

bool MVRunner::SerializeToString(pulse::study::multiplex_ventilation::bind::SimulationListData& src, std::string& output, SerializationFormat f) const
{
  google::protobuf::util::JsonPrintOptions printOpts;
  printOpts.add_whitespace = true;
  printOpts.preserve_proto_field_names = true;
  printOpts.always_print_primitive_fields = true;
  if (!google::protobuf::util::MessageToJsonString(src, &output, printOpts).ok())
  {
    Error("Unable to serialize simulation list");
    return false;
  }
  return true;
}
bool MVRunner::SerializeToFile(pulse::study::multiplex_ventilation::bind::SimulationListData& src, const std::string& filename, SerializationFormat f) const
{
  std::string content;
  if (!SerializeToString(src, content, f))
    return false;
  return WriteFile(content, filename, SerializationFormat::JSON);
}
bool MVRunner::SerializeFromString(const std::string& src, pulse::study::multiplex_ventilation::bind::SimulationListData& dst, SerializationFormat f)
{
  google::protobuf::util::JsonParseOptions parseOpts;
  google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
  {
    std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
  });
  google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(src, &dst, parseOpts);
  if (!stat.ok())
  {
    Error("Unable to parse json in string : "+ stat.ToString());
    return false;
  }
  return true;
}
bool MVRunner::SerializeFromFile(const std::string& filename, SerializationFormat f)
{
  SAFE_DELETE(m_SimulationList);
  SAFE_DELETE(m_SimulationResultsList);
  m_SimulationList = new pulse::study::multiplex_ventilation::bind::SimulationListData();
  m_SimulationResultsList = new pulse::study::multiplex_ventilation::bind::SimulationListData();

  std::string content = ReadFile(filename, SerializationFormat::JSON);
  if (content.empty())
  {
    Error("Unable to read file " + filename);
    return false;
  }
  
  if(!SerializeFromString(content, *m_SimulationList, f))
    return false;
  // Let's try to read in a results file
  m_SimulationResultsListFile = filename.substr(0, filename.length() - 5) + "_results.json";
  if (FileExists(m_SimulationResultsListFile))
  {
    std::string results_content = ReadFile(m_SimulationResultsListFile, SerializationFormat::JSON);
    if (results_content.empty())
      Warning("Unable to read file " + filename);
    else
      if (!SerializeFromString(results_content, *m_SimulationResultsList, f))
        return false;
  }
  return true;
}
