/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"

#include "PulseEngine.h"
#include "engine/PulseConfiguration.h"
#include "engine/PulseScenario.h"

#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/patient/actions/SEHemorrhage.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/utils/ConfigParser.h"
#include "cdm/utils/CSV.h"
#include "cdm/utils/DataTrack.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/GeneralMath.h"
#include "cdm/utils/TimingProfile.h"

using namespace pulse;

namespace pulse::study::patient_variability
{
  PVRunner::PVRunner(const std::string& rootDir, Logger* logger) : Loggable(logger)
  {
    m_RootDir = rootDir;
    m_PatientList = nullptr;
    m_PatientResultsList = nullptr;
  }
  PVRunner::~PVRunner()
  {
    SAFE_DELETE(m_PatientList);
    SAFE_DELETE(m_PatientResultsList);
  }

  bool PVRunner::Run(const std::string& filename)
  {
    SAFE_DELETE(m_PatientList);
    SAFE_DELETE(m_PatientResultsList);
    m_PatientList = new pulse::study::bind::patient_variability::PatientStateListData();
    m_PatientResultsList = new pulse::study::bind::patient_variability::PatientStateListData();
    if (!SerializeFromFile(filename, *m_PatientList))
      return false;

    return Run();
  }
  bool PVRunner::Run(pulse::study::bind::patient_variability::PatientStateListData& simList)
  {
    SAFE_DELETE(m_PatientList);
    SAFE_DELETE(m_PatientResultsList);
    m_PatientList = &simList;
    m_PatientResultsList = new pulse::study::bind::patient_variability::PatientStateListData();

    bool b = Run();
    m_PatientList = nullptr;
    SAFE_DELETE(m_PatientResultsList);
    return b;
  }

  
  bool PVRunner::Run()
  {
    if (m_PatientList->patient_size() == 0)
      return true; // Nothing to do

    TimingProfile profiler;
    profiler.Start("Total");

    if (m_PatientList->patient()[0].has_validation())
    {
      // Generate json files for standard male and female validation data from baselines
      // If json files already exist don't redo that work
      if (!FileExists(m_RootDir + "/standard_results.json"))
      {
        std::string command = "cmake -DTYPE:STRING=validateFolder -DARG1:STRING=./verification/scenarios/validation/systems -DARG2:STRING=false -P run.cmake";
        std::system(command.c_str());

        // Standard male
        pulse::study::bind::patient_variability::PatientStateListData standardResults;
        std::vector<std::string> standardPatients{ "StandardMale", "StandardFemale" };
        for (auto& standard : standardPatients)
        {
          auto patient = standardResults.add_patient();
          patient->set_outputbasefilename(standard);
          std::vector<std::string> validation_files;
          ListFiles("./verification/scenarios/validation/systems", validation_files, true, "-" + standard + "ValidationResults.json");
          if (!AggregateResults(*patient, validation_files, GetLogger()))
          {
            GetLogger()->Warning("Unable to aggregate results for " + standard);
          }
        }
        SerializeToFile(standardResults, m_RootDir + "/standard_results.json");
      }
    }

    if (SerializationFormat == eSerializationFormat::JSON)
      m_PatientResultsListFile = m_RootDir + "/patient_results.json";
    else
      m_PatientResultsListFile = m_RootDir + "/patient_results.pbb";
    if (FileExists(m_PatientResultsListFile))
    {
      if (PostProcessOnly)
      {
        if (std::remove(m_PatientResultsListFile.c_str()) != 0)
          Error("Unable to remove file " + m_PatientResultsListFile);
      }
      else if (!SerializeFromFile(m_PatientResultsListFile, *m_PatientResultsList))
      {
        GetLogger()->Warning("Unable to read found results file");
      }
    }

    // Get the ID's of Patients we need to run
    m_PatientsToRun.clear();
    for (int i = 0; i < m_PatientList->patient_size(); i++)
      m_PatientsToRun.insert(m_PatientList->patient()[i].id());
    // Remove any id's we have in the results
    if (m_PatientResultsList->patient_size() > 0)
    {
      Info("Found " + std::to_string(m_PatientResultsList->patient_size()) + " previously run Patients");
      for (int i = 0; i < m_PatientResultsList->patient_size(); i++)
        m_PatientsToRun.erase(m_PatientResultsList->patient()[i].id());
    }

    size_t numSimsToRun = m_PatientList->patient_size() - m_PatientResultsList->patient_size();
    if (numSimsToRun == 0)
    {
      Info("All Patients are run in the results file");
      return true;
    }
    size_t processor_count = std::thread::hardware_concurrency();
    if (processor_count == 0)
    {
      Fatal("Unable to detect number of processors");
      return false;
    }
    // Let's not kill the computer this is running on...
    if (processor_count > 1)
      processor_count -= 1;
    // Let's not kick off more threads than we need
    if (processor_count > numSimsToRun)
      processor_count = numSimsToRun;
    Info("Starting " + std::to_string(processor_count) + " Threads to run " + std::to_string(m_PatientsToRun.size()) + " Patients");
    // Crank up our threads
    for (size_t p = 0; p < processor_count; p++)
      m_Threads.push_back(std::thread(&PVRunner::ControllerLoop, this));

    for (size_t p = 0; p < processor_count; p++)
      m_Threads[p].join();

    Info("It took " + std::to_string(profiler.GetElapsedTime_s("Total")) + "s to run this Patient list");
    profiler.Clear();
    return true;
  }

  void PVRunner::ControllerLoop()
  {
    CreatePath(m_RootDir);

    pulse::study::bind::patient_variability::PatientStateData* patient = nullptr;
    while (true)
    {
      try
      {
        patient = GetNextPatient();
        if (patient == nullptr)
          break;

        Info("Running Patient " + std::to_string(patient->id())+" "+patient->outputbasefilename());
        if(!RunPatient(*patient))
          GetLogger()->Error("Unable to run patient " + patient->outputbasefilename());
      }
      catch (CommonDataModelException& cdm_ex)
      {
        GetLogger()->Fatal("Exception caught runnning Patient " + patient->outputbasefilename());
        GetLogger()->Fatal(cdm_ex.what());
        std::cerr << cdm_ex.what() << std::endl;
      }
      catch (std::exception& ex)
      {
        GetLogger()->Fatal("Exception caught runnning Patient " + patient->outputbasefilename());
        GetLogger()->Fatal(ex.what());
        std::cerr << ex.what() << std::endl;
      }
      catch (...)
      {
        std::cerr << "Unable to run Patient " << patient->outputbasefilename() << std::endl;
      }
    }
  }

  enum class eHemorrhageState
  {
    None = 0,
    Start,
    TrackStart,
    TrackTriage,
    Finish
  };

  bool PVRunner::RunPatient(pulse::study::bind::patient_variability::PatientStateData& patient)
  {
    TimingProfile profiler;
    profiler.Start("Total");
    profiler.Start("Status");
    std::string csvFilename;
    // For validation runs only
    std::map<std::string, std::vector<std::string>> allScenarioRequests;
    // For hemorrhage runs only
    eHemorrhageState hState = eHemorrhageState::None;
    pulse::study::bind::patient_variability::PatientStateData_HemorrhageData* hemorrhageData = nullptr;
    if (patient.has_hemorrhage())
    {
      hState = eHemorrhageState::Start;
      hemorrhageData = patient.mutable_hemorrhage();
    }

    if(!PostProcessOnly)
    {
      PulseConfiguration cfgChanges;
      auto pulse = CreatePulseEngine();
      // No logging to console (when threaded)
      pulse->GetLogger()->LogToConsole(false);
      // But, do write a log file
      pulse->GetLogger()->SetLogFile(m_RootDir + patient.outputbasefilename() + "/patient.log");

      if (patient.has_validation())
      {
        // Write out the computed patient baseline values so we can see how well we met them
        cfgChanges.EnableWritePatientBaselineFile(eSwitch::On);
        cfgChanges.SetInitialPatientBaselineFilepath(m_RootDir + patient.outputbasefilename() + "/patient.init.json");
        pulse->SetConfigurationOverride(&cfgChanges);

        // Read in all System and Patient validation scenarios
        // Add in all the data requests from all of those files
        csvFilename = m_RootDir + patient.outputbasefilename() + "/AllValidationResults.csv";

        std::string scenario_dir;
        ConfigSet* config = ConfigParser::FileToConfigSet("run.config");
        if (config->HasKey("scenario_dir"))
        {
          scenario_dir = config->GetValue("scenario_dir");

          //Retrieve all patients validation scenarios
          std::vector<std::string> scenario_files;
          ListFiles(scenario_dir + "/validation/patients", scenario_files, true, "Validation.json");

          //Retrieve all systems validation scenarios
          ListFiles(scenario_dir + "/validation/systems", scenario_files, true, "Validation.json");

          //Copy all data requests from each scenario
          for (auto scenario_filepath : scenario_files)
          {
            PulseScenario s(pulse->GetLogger());
            s.SerializeFromFile(scenario_filepath);

            std::string scenario_filename;
            SplitFilenamePath(scenario_filepath, scenario_filename);
            std::string output_csv_file = Replace(scenario_filename, ".json", "Results.csv");
            allScenarioRequests.insert(std::pair<std::string, std::vector<std::string>>(output_csv_file, std::vector<std::string>()));

            for (auto dr : s.GetDataRequestManager().GetDataRequests())
            {
              SEDataRequest& new_dr = pulse->GetEngineTracker()->GetDataRequestManager().CopyDataRequest(*dr);
              allScenarioRequests[output_csv_file].push_back(new_dr.GetHeaderName());
            }
          }
        }
        else
        {
          Error("Cannot find scenario root... cannot add any data requests...");
          return false;
        }
      }
      else if (hemorrhageData)
      {
        csvFilename = m_RootDir + patient.outputbasefilename() + "/HemorrhageResults.csv";
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HemoglobinContent", MassUnit::g);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalHemorrhageRate", VolumePerTimeUnit::mL_Per_s);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalHemorrhagedVolume", VolumeUnit::mL);
      }
      else
      {
        Error("Unknown run mode");
        return false;
      }
      pulse->GetEngineTracker()->GetDataRequestManager().SetResultsFilename(csvFilename);

      // Create our patient
      SEPatientConfiguration pc;
      SEPatient& p = pc.GetPatient();
      p.SetName("Patient"+std::to_string(patient.id()));
      p.SetSex((ePatient_Sex)patient.sex());
      p.GetAge().SetValue(patient.age_yr(), TimeUnit::yr);
      p.GetHeight().SetValue(patient.height_cm(), LengthUnit::cm);
      p.GetWeight().SetValue(patient.weight_kg(), MassUnit::kg);
      p.GetHeartRateBaseline().SetValue(patient.heartrate_bpm(), FrequencyUnit::Per_min);
      p.GetSystolicArterialPressureBaseline().SetValue(patient.systolicarterialpressure_mmhg(), PressureUnit::mmHg);
      p.GetDiastolicArterialPressureBaseline().SetValue(patient.diastolicarterialpressure_mmhg(), PressureUnit::mmHg);
      p.SerializeToFile(m_RootDir + patient.outputbasefilename() + "/patient.json");

      if (!pulse->InitializeEngine(pc))
      {
        patient.set_achievedstabilization(false);
        patient.set_stabilizationtime_s(profiler.GetElapsedTime_s("Total"));
        return false;
      }
      patient.set_achievedstabilization(true);
      patient.set_stabilizationtime_s(profiler.GetElapsedTime_s("Total"));
      pulse->GetLogger()->Info("It took " + cdm::to_string(patient.stabilizationtime_s()) + "s to stabilize this Patient");

      pulse->GetEngineTracker()->TrackData(0);
      double dT_s = pulse->GetTimeStep(TimeUnit::s);
      int count = patient.maxsimulationtime_s() / dT_s;

      for (int i = 0; i < count; i++)
      {
        if (!pulse->AdvanceModelTime()) // Compute 1 time step
          return false;

        double time_s = pulse->GetSimulationTime(TimeUnit::s);
        pulse->GetEngineTracker()->TrackData(time_s);

        // Hemorrhage Mode //
        if (hemorrhageData)
        {
          switch(hState)
          {
          case eHemorrhageState::Start:
          {
            if (time_s > patient.hemorrhage().starttime_s())
            {
              SEHemorrhage hemorrhage;
              hemorrhage.SetExternal(patient.hemorrhage().compartment());
              hemorrhage.GetSeverity().SetValue(patient.hemorrhage().severity());
              pulse->ProcessAction(hemorrhage);
              hState = eHemorrhageState::TrackStart;
            }
            break;
          }
          case eHemorrhageState::TrackStart:
          {
            if (pulse->GetCardiovascularSystem()->HasTotalHemorrhageRate())
            {
              double flowRate_mL_Per_s = pulse->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s);
              if (flowRate_mL_Per_s > 0)
              {
                hemorrhageData->set_initialbloodvolume_ml(pulse->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL));
                hemorrhageData->set_initialheartrate_bpm(pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min));
                hemorrhageData->set_initialcardiacoutput_l_per_min(pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::L_Per_min));
                hemorrhageData->set_initialmeanarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg));
                hemorrhageData->set_initialhemorrhageflow_ml_per_s(pulse->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s));
                hState = eHemorrhageState::TrackTriage;
              }
            }
            break;
          }
          case eHemorrhageState::TrackTriage:
          {
            if (time_s > patient.hemorrhage().triagetime_s())
            {
              hemorrhageData->set_triageheartrate_bpm(pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min));
              hemorrhageData->set_triagecardiacoutput_l_per_min(pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::L_Per_min));
              hemorrhageData->set_triagemeanarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg));
              hemorrhageData->set_triagebloodloss_ml(pulse->GetCardiovascularSystem()->GetTotalHemorrhagedVolume(VolumeUnit::mL));
              hemorrhageData->set_triagehemorrhageflow_ml_per_s(pulse->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s));
              hState = eHemorrhageState::Finish;
            }
            break;
          }
          }

          if (pulse->GetEventManager().IsEventActive(eEvent::CardiovascularCollapse))
          {
            Info("Patient "+std::to_string(patient.id())+" Has Cardiovascular Collapse " + patient.outputbasefilename());
            break;
          }
        }

      }
      patient.set_finalsimulationtime_s(pulse->GetSimulationTime(TimeUnit::s));
      // Hemorrhage Mode //
      if (hemorrhageData)
      {
        hemorrhageData->set_finalheartrate_bpm(pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min));
        hemorrhageData->set_finalcardiacoutput_l_per_min(pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::L_Per_min));
        hemorrhageData->set_finalmeanarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg));
        hemorrhageData->set_finalbloodloss_ml(pulse->GetCardiovascularSystem()->GetTotalHemorrhagedVolume(VolumeUnit::mL));
        hemorrhageData->set_finalhemorrhageflow_ml_per_s(pulse->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s));
      }

      pulse->GetEngineTracker()->ResetFile();// So we can close it

      if (patient.has_validation())
      {
        CSV::SplitCSV(csvFilename, allScenarioRequests);
        if (std::remove(csvFilename.c_str()) != 0)
          Error("Unable to remove file " + csvFilename);
      }
    }// end if(!m_PostProcessOnly)

    if (patient.has_validation())
    {
      std::string command = "cmake -DTYPE:STRING=validateFolder -DARG1:STRING=" + m_RootDir + patient.outputbasefilename() + " -DARG2:STRING=false -P run.cmake";
      std::system(command.c_str());

      //Retrieve all validation json files for patient and serialize data from those files
      std::vector<std::string> validation_files;
      ListFiles(m_RootDir + patient.outputbasefilename(), validation_files, true, "ValidationResults.json");
      if (!AggregateResults(patient, validation_files, GetLogger()))
        return false;
    }

    // Add our results to our results file
    m_mutex.lock();
    auto pResult = m_PatientResultsList->add_patient();
    pResult->CopyFrom(patient);
    SerializeToFile(*m_PatientResultsList, m_PatientResultsListFile);
    Info("Completed Simulation " + std::to_string(m_PatientResultsList->patient_size()) + " of " + std::to_string(m_PatientList->patient_size()));
    if(!PostProcessOnly)
    {
      if (patient.achievedstabilization())
        Info("  Stabilized : " + patient.outputbasefilename());
      else
        Info("  FAILED STABILIZATION : " + patient.outputbasefilename());
    }
    m_mutex.unlock();

    profiler.Clear();
    return true;
  }

  pulse::study::bind::patient_variability::PatientStateData* PVRunner::GetNextPatient()
  {
    m_mutex.lock();
    pulse::study::bind::patient_variability::PatientStateData* patient = nullptr;
    if (!m_PatientsToRun.empty())
    {
      auto id = *m_PatientsToRun.begin();
      for (int i = 0; i < m_PatientList->patient_size(); i++)
      {
        patient = &(*m_PatientList->mutable_patient())[i];
        if (patient->id() == id)
          break;
      }
      m_PatientsToRun.erase(id);
    }
    m_mutex.unlock();
    return patient;
  }

  bool PVRunner::SerializeToString(pulse::study::bind::patient_variability::PatientStateListData& src, std::string& output) const
  {
    google::protobuf::util::JsonPrintOptions printOpts;
    printOpts.add_whitespace = true;
    printOpts.preserve_proto_field_names = true;
    printOpts.always_print_primitive_fields = true;
    if (!google::protobuf::util::MessageToJsonString(src, &output, printOpts).ok())
    {
      Error("Unable to serialize Patient list");
      return false;
    }
    return true;
  }
  bool PVRunner::SerializeToFile(pulse::study::bind::patient_variability::PatientStateListData& src, const std::string& filename) const
  {
    return PBUtils::SerializeToFile(src, filename, GetLogger());
  }
  bool PVRunner::SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::PatientStateListData& dst)
  {
    google::protobuf::util::JsonParseOptions parseOpts;
    google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
    {
      std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
    });
    google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(src, &dst, parseOpts);
    if (!stat.ok())
    {
      Error("Unable to parse json in string : " + stat.ToString());
      return false;
    }
    return true;
  }
  bool PVRunner::SerializeFromFile(const std::string& filename, pulse::study::bind::patient_variability::PatientStateListData& dst)
  {
    return PBUtils::SerializeFromFile(filename, dst, GetLogger());
  }

  bool PVRunner::AggregateResults(pulse::study::bind::patient_variability::PatientStateData& patient, const std::vector<std::string>& validation_files, Logger* logger)
  {
    const auto& vMap = patient.mutable_validation()->mutable_validationmap();
    for(auto validation_filepath: validation_files)
    {
      std::string validation_filename;
      SplitFilenamePath(validation_filepath, validation_filename);

      //Serialize the file contents
      pulse::cdm::bind::PropertyValidationListData vList;
      if(!PBUtils::SerializeFromFile(validation_filepath, vList, logger))
        return false;

      (*vMap)[validation_filename] = vList;
    }
    return true;
  }
}
