/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseScenarioExec.h"
#include "engine/human_adult/whole_body/test/EngineTest.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

#include "cdm/utils/taskrunner/TaskRunner.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/TimingProfile.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/scenario/SEScenarioExec.h"
#include "cdm/properties/SEScalarTime.h"
#include <iomanip>

namespace pulse { namespace human_adult_whole_body
{
  class RunEngineTask : public Task
  {
  public:
    RunEngineTask(std::unique_ptr<PhysiologyEngine> pEngine)
      : m_pEngine(std::move(pEngine))
    {
    }

    virtual void Run() override
    {
      SEPatientConfiguration pc;
      pc.SetPatientFile("StandardMale.json");
      m_pEngine->InitializeEngine(pc);

      double dT_s = m_pEngine->GetTimeStep(TimeUnit::s);

      int count = static_cast<int>(10.0 / dT_s);
      for (int i = 0; i <= count; ++i)
      {
        m_pEngine->AdvanceModelTime();
      }
    }

  private:
    std::unique_ptr<PhysiologyEngine> m_pEngine;
  };

  class RunScenarioTask : public Task
  {
  public:
    static std::mutex ms_initializationMutex;

    RunScenarioTask(const std::string& scenarioFile)
      : m_scenarioFile(scenarioFile)
    {
    }

    virtual void Run() override;

  private:
    std::string m_scenarioFile;
  };

  std::mutex RunScenarioTask::ms_initializationMutex;

  void RunScenarioTask::Run()
  {
    // Set up the log file
    std::string logFile = m_scenarioFile;
    logFile = Replace(logFile, "verification", "bin");
    logFile = Replace(logFile, ".json", ".log");
    // Set up the verification output file  
    std::string dataFile = m_scenarioFile;
    dataFile = Replace(dataFile, "verification", "bin");
    dataFile = Replace(dataFile, ".json", "Results.csv");
    // Delete any results file that may be there
    remove(dataFile.c_str());

    ms_initializationMutex.lock();
    Engine pe;
    ms_initializationMutex.unlock();

    SEScenarioExec execOpts;
    execOpts.SetLogFilename(logFile);
    execOpts.SetDataRequestCSVFilename(dataFile);
    execOpts.SetScenarioFilename(m_scenarioFile);
    PulseScenarioExec::Execute(pe, execOpts);
  }

  void EngineTest::MultiEngineTest(const std::string& sTestDirectory)
  {
    double singleThreadTime = 0.0;
    double multiThreadTime = 0.0;

    {
      TaskRunner runner(1);

      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/Basic1.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/Basic2.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/Basic3.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicCynthia.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicGus.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicHassan.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicJoel.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicNathan.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicSoldier.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicStandardHeavy.json")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicStandardLight.json")));

      std::cout << " *** Running single-threaded ***" << "\n";

      TimingProfile timer;
      timer.Start("timer");

      runner.Run();

      timer.Stop("timer");
      singleThreadTime = timer.GetElapsedTime_s("timer");

      std::cout << " *** Single-threaded ran in " << timer.GetElapsedTime_s("timer") << " s\n";
    }

    {
      TaskRunner runner;

      std::cout << " *** Running multithreaded ***" << "\n";

      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/Basic1.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/Basic2.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/Basic3.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicCynthia.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicGus.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicHassan.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicJoel.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicNathan.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicSoldier.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicStandardHeavy.xml")));
      runner.AddTask(std::unique_ptr<RunScenarioTask>(new RunScenarioTask("../verification/Scenarios/Basic/BasicStandardLight.xml")));

      TimingProfile timer;
      timer.Start("timer");

      runner.Run();

      timer.Stop("timer");
      multiThreadTime = timer.GetElapsedTime_s("timer");

      std::cout << " *** Multithreaded ran in " << timer.GetElapsedTime_s("timer") << " s\n";
    }

    std::cout << "Done\n";

    std::cout << "\n"
      "=============================="
      "=============================="
      "=============================="
      "\n"
      "Single threaded time:      " << singleThreadTime << " s\n"
      "Multi threaded time:       " << multiThreadTime << " s\n"
      "Multi threaded comparison: "
      << std::fixed << std::setprecision(0) << ((multiThreadTime / singleThreadTime) * 100) << "%\n"
      "\n"
      "=============================="
      "=============================="
      "=============================="
      "\n";

    std::cin.get();
  }
END_NAMESPACE_EX
