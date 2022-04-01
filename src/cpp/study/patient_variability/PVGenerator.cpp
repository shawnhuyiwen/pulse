/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVGenerator.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"

namespace pulse::study::patient_variability
{
  PVGenerator::PVGenerator(const std::string& logfileName, const std::string& dataDir) : Loggable(logfileName)
  {
    m_DataDir = dataDir;
    m_PatientList = nullptr;
    m_CompletedPatientList = nullptr;
  }
  PVGenerator::~PVGenerator()
  {
    SAFE_DELETE(m_PatientList);
    SAFE_DELETE(m_CompletedPatientList);
  }

  void PVGenerator::GeneratePatientList(const std::string& stateDir)
  {
    int id = 0;
    SAFE_DELETE(m_PatientList);
    m_PatientList = new pulse::study::bind::patient_variability::PatientStateListData();

    enum Sex { Male=0, Female=1 };

    unsigned int ageMin = 18;
    unsigned int ageMax = 65;
    unsigned int ageStep = 10;

    // Heights in inches
    unsigned int heightMin_male = 65;
    unsigned int heightMax_male = 73;
    unsigned int heightMin_female = 60;
    unsigned int heightMax_female = 67;
    unsigned int heightStep = 3;

    double bmiMin = 18.5;
    double bmiMax = 29;
    double bmiStep = 3;

    unsigned int hrMin = 60;
    unsigned int hrMax = 100;
    unsigned int hrStep = 15;

    unsigned int mapMin = 70;
    unsigned int mapMax = 100;
    unsigned int mapStep = 10;

    unsigned int pulsePressureMin = 30;
    unsigned int pulsePressureMax = 50;
    unsigned int pulsePressureStep = 10;

    for( int sex = Male; sex <= Female; ++sex )
    {
      // Adjust height range based on sex
      unsigned int heightMin = heightMin_male;
      unsigned int heightMax = heightMax_male;
      std::string sex_dir = "male";
      if( sex == Female )
      {
        heightMin = heightMin_female;
        heightMax = heightMax_female;
        sex_dir = "female";
      }
      
      std::string sex_path = stateDir + "/" + sex_dir;

      for( unsigned int age = ageMin; age <= ageMax; age += ageStep )
      {
        std::string age_path = sex_path + "/yr" + std::to_string(age);
        for( unsigned int height = heightMin; height <= heightMax; height += heightStep )
        {
          std::string height_path = age_path + "/in" + std::to_string(height);
          for( double bmi = bmiMin; bmi <= bmiMax; bmi += bmiStep )
          {
            std::string bmi_path = height_path + "/bmi" + std::to_string(bmi);

            // Convert height to meters
            double height_cm = height * 2.54;

            // Caclulate weight (kg) from height (m) and BMI
            double weight = bmi * height_cm * height_cm * 0.0001;

            for( unsigned int hr = hrMin; hr <= hrMax; hr += hrStep )
            {
              std::string hr_path = bmi_path + "/hr" + std::to_string(hr);
              for( unsigned int map = mapMin; map <= mapMax; map += mapStep )
              {
                std::string map_path = hr_path + "/map" + std::to_string(map);
                for( unsigned int pp = pulsePressureMin; pp <= pulsePressureMax; pp += pulsePressureStep )
                {
                  std::string full_dir_path = map_path + "/pp" + std::to_string(pp);
                  CreatePath(full_dir_path);
                  double diastolic = pp - 3 * map;
                  double systolic = pp - diastolic;
                  auto patientData = m_PatientList->add_patients();
                  patientData->set_id(id);
                  patientData->set_age(age);
                  patientData->set_bmi(bmi);
                  patientData->set_diastolicarterialpressure_mmhg(diastolic);
                  patientData->set_heartrate_bpm(hr);
                  patientData->set_height_cm(height_cm);
                  patientData->set_meanarterialpressure_mmhg(map);
                  patientData->set_sex((pulse::study::bind::patient_variability::PatientStateData::eSex)sex);
                  patientData->set_systolicarterialpressure_mmhg(systolic);
                  patientData->set_weight_kg(weight);

                  SEPatient p(nullptr);
                  p.SetName(std::to_string(id++));
                  if(sex == Male)
                  {
                    p.SetSex(ePatient_Sex::Male);
                  }
                  else
                  {
                    p.SetSex(ePatient_Sex::Female);
                  }
                  p.GetAge().SetValue(age, TimeUnit::yr);
                  p.GetHeight().SetValue(height_cm, LengthUnit::cm);
                  p.GetWeight().SetValue(weight, MassUnit::kg);
                  p.GetDiastolicArterialPressureBaseline().SetValue(diastolic, PressureUnit::mmHg);
                  p.GetHeartRateBaseline().SetValue(hr, FrequencyUnit::Per_min);
                  p.GetMeanArterialPressureBaseline().SetValue(map, PressureUnit::mmHg);
                  p.GetSystolicArterialPressureBaseline().SetValue(systolic, PressureUnit::mmHg);
                  p.SerializeToFile(full_dir_path + "/patient.json");
                }
              }
            }
          }
        }
      }
    }
  }

  bool PVGenerator::Run(const std::string& stateDir, const std::string listFilename)
  {
    TimingProfile profiler;
    profiler.Start("Total");
    m_PatientStateListFile = listFilename;

    GeneratePatientList(stateDir);
    // Get the ID's of Patients we need to run
    m_PatientsToRun.clear();
    for (int i = 0; i < m_PatientList->patients_size(); i++)
      m_PatientsToRun.insert(m_PatientList->patients()[i].id());

    // Remove any id's we have in the results
    SAFE_DELETE(m_CompletedPatientList);
    m_CompletedPatientList = new pulse::study::bind::patient_variability::PatientStateListData();
    if (FileExists(listFilename))
    {
      if (!SerializeFromFile(listFilename))
        return false;
      if (m_CompletedPatientList->patients_size() > 0)
      {
        Info("Found " + std::to_string(m_CompletedPatientList->patients_size()) + " previously run Patients");
        for (int i = 0; i < m_CompletedPatientList->patients_size(); i++)
          m_PatientsToRun.erase(m_CompletedPatientList->patients()[i].id());
      }
    }

    size_t numPatientsToRun = m_PatientList->patients_size() - m_CompletedPatientList->patients_size();
    if (numPatientsToRun == 0)
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
    if (processor_count > numPatientsToRun)
      processor_count = numPatientsToRun;
    Info("Starting " + std::to_string(processor_count) + " Threads to generate " + std::to_string(numPatientsToRun) + " patients");
    // Crank up our threads
    for (size_t p = 0; p < processor_count; p++)
      m_Threads.push_back(std::thread(&PVGenerator::ControllerLoop, this));

    for (size_t p = 0; p < processor_count; p++)
      m_Threads[p].join();

    Info("It took " + cdm::to_string(profiler.GetElapsedTime_s("Total")) + "s to run this Patient list");
    profiler.Clear();
    return true;
  }

  void PVGenerator::ControllerLoop()
  {
    pulse::study::bind::patient_variability::PatientStateData* p;
    while (true)
    {
      p = GetNextPatient();
      if (p == nullptr)
        break;
      GenerateStabilizedPatient(*p, false);
      FinalizePatient(*p);
    }
  }

  bool PVGenerator::GenerateStabilizedPatient(pulse::study::bind::patient_variability::PatientStateData& pData, bool logToConsole)
  {
    return true;
  }

  pulse::study::bind::patient_variability::PatientStateData* PVGenerator::GetNextPatient()
  {
    m_mutex.lock();
    pulse::study::bind::patient_variability::PatientStateData* p = nullptr;
    if (!m_PatientsToRun.empty())
    {
      auto id = *m_PatientsToRun.begin();
      for (int i = 0; i < m_PatientList->patients_size(); i++)
      {
        p = &(*m_PatientList->mutable_patients())[i];
        if (p->id() == id)
          break;
      }
      Info("Generating Patient ID " + std::to_string(id));
      m_PatientsToRun.erase(id);
    }
    m_mutex.unlock();
    return p;
  }

  void PVGenerator::FinalizePatient(pulse::study::bind::patient_variability::PatientStateData& p)
  {
    m_mutex.lock();
    auto ps = m_CompletedPatientList->mutable_patients()->Add();
    ps->CopyFrom(p);
    SerializeToFile(*m_CompletedPatientList, m_PatientStateListFile);
    Info("Completed Patient " + std::to_string(m_CompletedPatientList->patients_size()) + " of " + std::to_string(m_PatientList->patients_size()));
    m_mutex.unlock();
  }

  bool PVGenerator::SerializeToString(pulse::study::bind::patient_variability::PatientStateListData& src, std::string& output, eSerializationFormat f) const
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
  bool PVGenerator::SerializeToFile(pulse::study::bind::patient_variability::PatientStateListData& src, const std::string& filename) const
  {
    return PBUtils::SerializeToFile(src, filename, GetLogger());
  }
  bool PVGenerator::SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::PatientStateListData& dst, eSerializationFormat f)
  {
    google::protobuf::util::JsonParseOptions parseOpts;
    google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
    {
      std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
    });
    google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(src, &dst, parseOpts);
    if (!stat.ok())
    {
      Error("Unable to parse json in string");
      return false;
    }
    return true;
  }
  bool PVGenerator::SerializeFromFile(const std::string& filename)
  {
    return PBUtils::SerializeFromFile(filename, *m_CompletedPatientList, GetLogger());
  }
}