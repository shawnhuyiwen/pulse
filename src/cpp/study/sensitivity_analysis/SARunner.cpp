/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "SARunner.h"

SARunner::SARunner(const std::string& logfileName, const std::string& dataDir) : Loggable(logfileName)
{
  m_DataDir = dataDir;
  m_SimulationList = nullptr;
  m_SimulationResultsList = nullptr;
}
SARunner::~SARunner()
{
  SAFE_DELETE(m_SimulationList);
  SAFE_DELETE(m_SimulationResultsList);
}

bool SARunner::Run(pulse::study::sensitivity_analysis::bind::SimulationListData& simList, const std::string& resultsFilename)
{
  m_SimulationResultsListFile = simList.outputrootdir()+"/"+resultsFilename;
  SAFE_DELETE(m_SimulationList);
  SAFE_DELETE(m_SimulationResultsList);
  m_SimulationList = &simList;
  m_SimulationResultsList = new pulse::study::sensitivity_analysis::bind::SimulationListData();
  if (FileExists(m_SimulationResultsListFile))
  {
    if (!SerializeFromFile(m_SimulationResultsListFile, *m_SimulationResultsList, SerializationFormat::JSON))
    {
      GetLogger()->Warning("Unable to read found results file");
    }
  }
  bool b = Run();
  m_SimulationList = nullptr;
  SAFE_DELETE(m_SimulationResultsList);
  return b;
}

bool SARunner::Run(const std::string& filename, SerializationFormat f)
{
  SAFE_DELETE(m_SimulationList);
  SAFE_DELETE(m_SimulationResultsList);
  m_SimulationList = new pulse::study::sensitivity_analysis::bind::SimulationListData();
  m_SimulationResultsList = new pulse::study::sensitivity_analysis::bind::SimulationListData();

  if (!SerializeFromFile(filename, *m_SimulationList, f))
    return false;
  // Let's try to read in a results file
  m_SimulationResultsListFile = filename.substr(0, filename.length() - 5) + "_results.json";
  if (FileExists(m_SimulationResultsListFile))
  {
    if (!SerializeFromFile(m_SimulationResultsListFile, *m_SimulationResultsList, f))
    {
      GetLogger()->Warning("Unable to read found results file");
    }
  }
  return Run();
}
bool SARunner::Run()
{
  TimingProfile profiler;
  profiler.Start("Total");

  // Get the ID's of simulations we need to run
  m_SimulationsToRun.clear();
  for (int i = 0; i < m_SimulationList->simulation_size(); i++)
    m_SimulationsToRun.insert(m_SimulationList->simulation()[i].id());
  // Remove any id's we have in the results
  if (m_SimulationResultsList->simulation_size() > 0)
  {
    Info("Found " + to_string(m_SimulationResultsList->simulation_size()) + " previously run simulations");
    for (int i = 0; i < m_SimulationResultsList->simulation_size(); i++)
      m_SimulationsToRun.erase(m_SimulationResultsList->simulation()[i].id());
  }

  int numSimsToRun = m_SimulationList->simulation_size() - m_SimulationResultsList->simulation_size();
  if (numSimsToRun == 0)
  {
    Info("All simulations are run in the results file");
    return true;
  }
  size_t processor_count = std::thread::hardware_concurrency();
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
  Info("Starting " + to_string(processor_count) + " Threads to run "+ to_string(m_SimulationsToRun.size())+" simulations");
  // Crank up our threads
  for (size_t p = 0; p < processor_count; p++)
    m_Threads.push_back(std::thread(&SARunner::ControllerLoop, this));

  for (size_t p = 0; p < processor_count; p++)
    m_Threads[p].join();

  Info("It took " + to_string(profiler.GetElapsedTime_s("Total")) + "s to run this simulation list");
  profiler.Clear();
  return true;
}

void SARunner::ControllerLoop()
{
  pulse::study::sensitivity_analysis::bind::SimulationData* sim=nullptr;
  while (true)
  {
    try
    {
      sim = GetNextSimulation();
      if (sim == nullptr)
        break;
      RunSimulationUntilStable(*sim);
      FinalizeSimulation(*sim);
    }
    catch (CommonDataModelException& cdm_ex)
    {
      GetLogger()->Fatal("Exception caught runnning simulation " + sim->outputbasefilename());
      GetLogger()->Fatal(cdm_ex.what());
      std::cerr << cdm_ex.what() << std::endl;
    }
    catch (std::exception ex)
    {
      GetLogger()->Fatal("Exception caught runnning simulation " + sim->outputbasefilename());
      GetLogger()->Fatal(ex.what());
      std::cerr << ex.what() << std::endl;
    }
    catch (...)
    {
      std::cerr << "Unable to run simulation " << sim->outputbasefilename() << std::endl;
    }
  }
}

bool SARunner::RunSimulationUntilStable(pulse::study::sensitivity_analysis::bind::SimulationData& sim, const std::string& dataDir)
{
  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");

  auto pulse = CreatePulseEngine(sim.outputbasefilename() + ".log", dataDir);

  // TODO amb Clean this up
  PulseConfiguration cfg(pulse->GetSubstanceManager());
  cfg.EnableNervousFeedback(eSwitch::Off); // Turn off nervous system feedback (ex. baroreceptors)
  if (!pulse->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON, nullptr, &cfg))
    return false;

  // No logging to console (when threaded)
  pulse->GetLogger()->LogToConsole(false);
  // Setup data requests
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  // TODO BF add everything we want in the csv file here

  // Apply Overrides
  // Get Aaron when you get to this point

  double timeStep_s = pulse->GetTimeStep(TimeUnit::s);
  double currentTime_s = 0;

  int stabilizationPasses = 0;
  int totalIterations = 0;
  double stabilizationTimer_s = 0;
  double stabilizationCheck_s = 2;
  // TODO BF Convert this to a CV based stabilization
  // Let's shoot for with in 0.25% for 10s straight
  double pctDiffSpO2 = 0.25;

  double previousSpO2s;
  previousSpO2s = pulse->GetBloodChemistrySystem()->GetOxygenSaturation();

  double statusTimer_s = 0;  // Current time of this status cycle
  double statusStep_s = 60; // How long did it take to simulate this much time

  int minIterations = (int)(60 / timeStep_s);
  int maxIterations = (int)(300 / timeStep_s);
  while (true)
  {
    totalIterations++;
    if (totalIterations == minIterations)
      pulse->GetLogger()->Info("Minimum Runtime Achieved");

    pulse->AdvanceModelTime(timeStep_s, TimeUnit::s);
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
      pulse->GetLogger()->Info("Current Time is " + cdm::to_string(currentTime_s) + "s, it took " + cdm::to_string(profiler.GetElapsedTime_s("Status")) + "s to simulate the past " + cdm::to_string(statusStep_s) + "s");
      profiler.Reset("Status");
    }

    // Check to see if we are stable
    if (stabilizationTimer_s > stabilizationCheck_s)
    {
      double currentSpO2 = pulse->GetBloodChemistrySystem()->GetOxygenSaturation();
      double pctDiff = GeneralMath::PercentDifference(previousSpO2s, currentSpO2);
      if (pctDiff <= pctDiffSpO2)
        stabilizationPasses++;
      else
      {
        stabilizationPasses = 0;
        previousSpO2s = currentSpO2;
      }

      stabilizationTimer_s = 0;
      if (stabilizationPasses == 5)
      {
        break;
        double stabilizationTime = (totalIterations - minIterations) * timeStep_s;
        pulse->GetLogger()->Info("Stabilization took " + cdm::to_string(stabilizationTime) + "s to for this simulation");
      }
      if (totalIterations > maxIterations)
      {
        pulse->GetLogger()->Info("Reached maximum iterations. Ending simulation.");
        break;
      }
    }
  }

  // Fill out our results
  sim.set_achievedstabilization(!(totalIterations > maxIterations));
  sim.set_stabilizationtime_s(profiler.GetElapsedTime_s("Total"));
  sim.set_cardiacoutput_l_per_min(pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::L_Per_min));
  // TODO BF Add the rest of our outputs here
  
  pulse->GetLogger()->Info("It took " + cdm::to_string(sim.stabilizationtime_s()) + "s to run this simulation");

  profiler.Clear();
  return true;
}

pulse::study::sensitivity_analysis::bind::SimulationData* SARunner::GetNextSimulation()
{
  m_mutex.lock();
  pulse::study::sensitivity_analysis::bind::SimulationData* sim = nullptr;
  if (!m_SimulationsToRun.empty())
  {
    size_t id = *m_SimulationsToRun.begin();
    for (int i = 0; i < m_SimulationList->simulation_size(); i++)
    {
      sim = &(*m_SimulationList->mutable_simulation())[i];
      if (sim->id() == id)
        break;
    }
    Info("Simulating ID " + to_string(id)+" "+sim->outputbasefilename());
    m_SimulationsToRun.erase(id);
  }
  m_mutex.unlock();
  return sim;
}

void SARunner::FinalizeSimulation(pulse::study::sensitivity_analysis::bind::SimulationData& sim)
{
  m_mutex.lock();
  auto rSim = m_SimulationResultsList->mutable_simulation()->Add();
  rSim->CopyFrom(sim);
  SerializeToFile(*m_SimulationResultsList, m_SimulationResultsListFile, SerializationFormat::JSON);
  Info("Completed Simulation " + to_string(m_SimulationResultsList->simulation_size()) + " of " + to_string(m_SimulationList->simulation_size()));
  if (sim.achievedstabilization())
    Info("  Stabilized : " + sim.outputbasefilename());
  else
    Info("  FAILED STABILIZATION : " + sim.outputbasefilename());
  m_mutex.unlock();
}

bool SARunner::SerializeToString(pulse::study::sensitivity_analysis::bind::SimulationListData& src, std::string& output, SerializationFormat f) const
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
bool SARunner::SerializeToFile(pulse::study::sensitivity_analysis::bind::SimulationListData& src, const std::string& filename, SerializationFormat f) const
{
  std::string content;
  if (!SerializeToString(src, content, f))
    return false;
  return WriteFile(content, filename, SerializationFormat::JSON);
}
bool SARunner::SerializeFromString(const std::string& src, pulse::study::sensitivity_analysis::bind::SimulationListData& dst, SerializationFormat f)
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
bool SARunner::SerializeFromFile(const std::string& filename, pulse::study::sensitivity_analysis::bind::SimulationListData& dst, SerializationFormat f)
{
  std::string content = ReadFile(filename, SerializationFormat::JSON);
  if (content.empty())
  {
    Error("Unable to read file " + filename);
    return false;
  }
  return SerializeFromString(content, dst, f);
}
