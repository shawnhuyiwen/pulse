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

bool SARunner::Run(pulse::study::sensitivity_analysis::bind::SimulationListData& simList)
{
  m_OutDir = simList.outputrootdir();
  m_SimulationResultsListFile = m_OutDir+"/simlist_results.json";
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
  m_OutDir = m_SimulationList->outputrootdir();
  m_SimulationResultsListFile = m_OutDir+"/simlist_results.json";
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

  // Ensure our output dir exists
  CreateFilePath(m_OutDir);

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
      RunSimulationUntilStable(m_OutDir, *sim);
      FinalizeSimulation(*sim);
    }
    catch (CommonDataModelException& cdm_ex)
    {
      GetLogger()->Fatal("Exception caught runnning simulation " + sim->name());
      GetLogger()->Fatal(cdm_ex.what());
      std::cerr << cdm_ex.what() << std::endl;
    }
    catch (std::exception ex)
    {
      GetLogger()->Fatal("Exception caught runnning simulation " + sim->name());
      GetLogger()->Fatal(ex.what());
      std::cerr << ex.what() << std::endl;
    }
    catch (...)
    {
      std::cerr << "Unable to run simulation " << sim->name() << std::endl;
    }
  }
}

bool SARunner::RunSimulationUntilStable(std::string const& outDir, pulse::study::sensitivity_analysis::bind::SimulationData& sim, const std::string& dataDir)
{
  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");

  auto pulse = CreatePulseEngine(outDir+"/"+cdm::to_string(sim.id())+" - "+sim.name()+".log", dataDir);

  // TODO amb Clean this up (cfg should have a default ctor that makes its own Sub Mgr)
  PulseConfiguration cfg(pulse->GetSubstanceManager());
  cfg.EnableNervousFeedback(eSwitch::Off); // Turn off nervous system feedback (ex. baroreceptors)
  if (!pulse->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON, nullptr, &cfg))
    return false;

  // No logging to console (when threaded)
  pulse->GetLogger()->LogToConsole(true);
  // Setup data requests
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ArterialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CentralVenousPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CerebralBloodFlow", VolumePerTimeUnit::L_Per_min);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CerebralPerfusionPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartEjectionFraction");
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartStrokeVolume", VolumeUnit::mL);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("IntracranialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryArterialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryCapillariesWedgePressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryDiastolicArterialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryMeanArterialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryMeanCapillaryFlow", VolumePerTimeUnit::L_Per_min);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonarySystolicArterialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystemicVascularResistance", PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  pulse->GetEngineTracker()->GetDataRequestManager().SetResultsFilename(outDir+"/"+cdm::to_string(sim.id())+" - "+sim.name()+".csv");
  pulse->GetEngineTracker()->SetupRequests();

  // Serialize Overrides
  SEOverrides overrides;
  PBAction::Load(sim.overrides(), overrides);
  // Apply Overrides (Note using Force, as these values are locked (for good reason)
  // But we know what we are doing, right?
  PulseController* pc = ((PulseEngine*)pulse.get())->GetController();
  SEFluidCircuit& cv = pc->GetCircuits().GetActiveCardiovascularCircuit();
  for (auto& sp : overrides.GetScalarProperties())
  {
    SEFluidCircuitPath* path = cv.GetPath(sp.name);
    if (path == nullptr)
    {
      Error("No path found for override "+sp.name+", for "+cdm::to_string(sim.id())+" - "+sim.name());
      return false;
    }
    if (PressureTimePerVolumeUnit::IsValidUnit(sp.unit))
    {// Assume its a resistor
      const PressureTimePerVolumeUnit& u = PressureTimePerVolumeUnit::GetCompoundUnit(sp.unit);
      path->GetResistance().ForceValue(sp.value, u);
      path->GetResistanceBaseline().ForceValue(sp.value, u);
    }
    else if (VolumePerPressureUnit::IsValidUnit(sp.unit))
    {
      const VolumePerPressureUnit& u = VolumePerPressureUnit::GetCompoundUnit(sp.unit);
      path->GetCompliance().ForceValue(sp.value, u);
      path->GetNextCompliance().ForceValue(sp.value, u);
      path->GetComplianceBaseline().ForceValue(sp.value, u);
    }
    else
    {
      Error("Could not process override " + sp.name + ", for " + cdm::to_string(sim.id()) + " - " + sim.name());
      return false;
    }
  }

  // Run until stable
  // Let's shoot for with in 0.25% for 10s straight
  double timeStep_s = pulse->GetTimeStep(TimeUnit::s);
  double stabPercentTolerance = 0.25;
  double stabCheckTime_s = 10.0;
  double time_s = 0;
  double maxTime_s = 2000;
  // Here are our variable we will check for stability
  double previoustMap_mmHg = pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg);
  double previousSystolic_mmHg = pulse->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg);
  double previousDiastolic_mmHg = pulse->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg);
  double previousCardiacOutput_mL_Per_min = pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min);
  //double previousMeanCVP_mmHg = pulse->GetCardiovascularSystem()->GetMeanCentralVenousPressure(PressureUnit::mmHg);
  double previousBlood_mL = pulse->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL);

  bool stable = false;
  double stableTime_s = 0;
  while (!stable)
  {
      pulse->AdvanceModelTime(timeStep_s, TimeUnit::s);

      double currentMap_mmHg = pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg);
      double currentSystolic_mmHg = pulse->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg);
      double currentDiastolic_mmHg = pulse->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg);
      double currentCardiacOutput_mL_Per_min = pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min);
      //double currentMeanCVP_mmHg = pulse->GetCardiovascularSystem()->GetMeanCentralVenousPressure(PressureUnit::mmHg);
      double currentBlood_mL = pulse->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL);

      stableTime_s += timeStep_s;
      bool stableMAP = true;
      if (GeneralMath::PercentDifference(previoustMap_mmHg, currentMap_mmHg) > stabPercentTolerance)
      {
          stableTime_s = 0; previoustMap_mmHg = currentMap_mmHg; stableMAP = false;
      }
      bool stableSystolic = true;
      if (GeneralMath::PercentDifference(previousSystolic_mmHg, currentSystolic_mmHg) > stabPercentTolerance)
      {
          stableTime_s = 0; previousSystolic_mmHg = currentSystolic_mmHg; stableSystolic = false;
      }
      bool stableDiastolic = true;
      if (GeneralMath::PercentDifference(previousDiastolic_mmHg, currentDiastolic_mmHg) > stabPercentTolerance)
      {
          stableTime_s = 0; previousDiastolic_mmHg = currentDiastolic_mmHg; stableDiastolic = false;
      }
      bool stableCO = true;
      if (GeneralMath::PercentDifference(previousCardiacOutput_mL_Per_min, currentCardiacOutput_mL_Per_min) > stabPercentTolerance)
      {
          stableTime_s = 0; previousCardiacOutput_mL_Per_min = currentCardiacOutput_mL_Per_min; stableCO = false;
      }
      //bool stableMeanCVP = true;
      //if (GeneralMath::PercentDifference(tgt_meanCVP_mmHg, meanCVP_mmHg) > 0.25)
      //  { stableTime_s = 0; tgt_meanCVP_mmHg = meanCVP_mmHg; stableMeanCVP = false; }
      bool stableBloodVol = true;
      if (GeneralMath::PercentDifference(previousBlood_mL, currentBlood_mL) > stabPercentTolerance)
      {
          stableTime_s = 0; previousBlood_mL = currentBlood_mL; stableBloodVol = false;
      }

      if (stableTime_s > stabCheckTime_s)
      {
          stable = true;
          pulse->GetLogger()->Info("We are stable at " + cdm::to_string(time_s));
      }
      if (time_s > maxTime_s)
      {
          pulse->GetLogger()->Info("Could not stabilize this configuration");
          break;
      }
  }

  // TODO BF Check that what we overrided is still set to the same value
  // This ensures the engine did not do anything to our resistances for some reason
  // If they are different, we need to return false and look into why

  // Fill out our results
  sim.set_achievedstabilization(stable);
  sim.set_stabilizationtime_s(profiler.GetElapsedTime_s("Total"));
  sim.set_arterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg));
  sim.set_bloodvolume_ml(pulse->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL));
  sim.set_cardiacoutput_l_per_min(pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::L_Per_min));
  sim.set_centralvenouspressure_mmhg(pulse->GetCardiovascularSystem()->GetCentralVenousPressure(PressureUnit::mmHg));
  sim.set_cerebralbloodflow_l_per_min(pulse->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::L_Per_min));
  sim.set_cerebralperfusionpressure_mmhg(pulse->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg));
  sim.set_diastolicarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg));
  sim.set_heartejectionfraction(pulse->GetCardiovascularSystem()->GetHeartEjectionFraction());
  sim.set_heartrate_bpm(pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min));
  sim.set_heartstrokevolume_ml(pulse->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL));
  sim.set_intracranialpressure_mmhg(pulse->GetCardiovascularSystem()->GetIntracranialPressure(PressureUnit::mmHg));
  sim.set_meanarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg));
  sim.set_pulmonaryarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetPulmonaryArterialPressure(PressureUnit::mmHg));
  sim.set_pulmonarycapillarieswedgepressure_mmhg(pulse->GetCardiovascularSystem()->GetPulmonaryCapillariesWedgePressure(PressureUnit::mmHg));
  sim.set_pulmonarydiastolicarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetPulmonaryDiastolicArterialPressure(PressureUnit::mmHg));
  sim.set_pulmonarymeanarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetPulmonaryMeanArterialPressure(PressureUnit::mmHg));
  sim.set_pulmonarymeancapillaryflow_l_per_min(pulse->GetCardiovascularSystem()->GetPulmonaryMeanCapillaryFlow(VolumePerTimeUnit::L_Per_min));
  sim.set_pulmonarysystolicarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetPulmonarySystolicArterialPressure(PressureUnit::mmHg));
  sim.set_systolicarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg));
  sim.set_systemicvascularresistance_mmhg_s_per_l(pulse->GetCardiovascularSystem()->GetSystemicVascularResistance(PressureTimePerVolumeUnit::mmHg_s_Per_mL));
  
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
    Info("Simulating Run " + to_string(id)+" : "+sim->name());
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
    Info("  Stabilized Run " + to_string(sim.id()) + " : " + sim.name());
  else
    Info("  FAILED STABILIZATION FOR RUN " + to_string(sim.id()) + " : " + sim.name());
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
