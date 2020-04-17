/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVGenerator.h"

MVGenerator::MVGenerator(const std::string& logfileName, const std::string& dataDir) : Loggable(logfileName)
{
  m_DataDir = dataDir;
  m_PatientList = nullptr;
  m_CompletedPatientList = nullptr;
}
MVGenerator::~MVGenerator()
{
  SAFE_DELETE(m_PatientList);
  SAFE_DELETE(m_CompletedPatientList);
}

void MVGenerator::GeneratePatientList()
{
  int id = 0;
  SAFE_DELETE(m_PatientList);
  m_PatientList = new pulse::study::multiplex_ventilation::bind::PatientStateListData();

  unsigned int totalIterations = (((m_MaxCompliance_mL_Per_cmH2O - m_MinCompliance_mL_Per_cmH2O) / m_StepCompliance_mL_Per_cmH2O) + 1) *
                                  ((m_MaxPEEP_cmH2O - m_MinPEEP_cmH2O) / m_StepPEEP_cmH2O + 1) *
                                  (unsigned int((m_MaxImpairment - m_MinImpairment) / m_StepImpairment + 1.0));

  SEScalarPressureTimePerVolume r;
  r.SetValue(m_Resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double m_Resistance_cmH2O_s_Per_mL = r.GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_mL);

  for (int compliance_mL_Per_cmH2O = m_MinCompliance_mL_Per_cmH2O; compliance_mL_Per_cmH2O <= m_MaxCompliance_mL_Per_cmH2O; compliance_mL_Per_cmH2O += m_StepCompliance_mL_Per_cmH2O)
  {
    for (int PEEP_cmH2O = m_MinPEEP_cmH2O; PEEP_cmH2O <= m_MaxPEEP_cmH2O; PEEP_cmH2O += m_StepPEEP_cmH2O)
    {
      // RC circuit charging equation
      // Assume tube resistances are negligable
      double targetTidalVolume_mL = 6.0 * 75.3; // Aaron - What's the best way to get the ideal body weight from the patient?
      double breathPeriod_s = 60.0 / m_RespirationRate_Per_Min;
      double inspiratoryPeriod_s = m_IERatio * breathPeriod_s / (1.f + m_IERatio);
      
      int PIP_cmH2O = int(targetTidalVolume_mL / (compliance_mL_Per_cmH2O * (1.0 - exp(-inspiratoryPeriod_s / (m_Resistance_cmH2O_s_Per_mL * compliance_mL_Per_cmH2O)))) + PEEP_cmH2O);

      for (double currentImpairment = m_MinImpairment; currentImpairment <= m_MaxImpairment; currentImpairment += m_StepImpairment) //Aaron - This needs to be maxImpairment instead of 1.0, but rounding screws it up
      {
        auto patientData = m_PatientList->add_patients();
        patientData->set_id(id++);
        patientData->set_compliance_ml_per_cmh2o(compliance_mL_Per_cmH2O);
        patientData->set_resistance_cmh2o_s_per_l(m_Resistance_cmH2O_s_Per_L);
        patientData->set_impairmentfraction(currentImpairment);
        patientData->set_respirationrate_per_min(m_RespirationRate_Per_Min);
        patientData->set_ieratio(m_IERatio);
        patientData->set_peep_cmh2o(PEEP_cmH2O);
        patientData->set_pip_cmh2o(PIP_cmH2O);
      }
    }
  }
}

bool MVGenerator::Run(const std::string& stateDir, const std::string listFilename, SerializationFormat f)
{
  TimingProfile profiler;
  profiler.Start("Total");
  m_PatientStateListFile = listFilename;

  GeneratePatientList();
  // Get the ID's of Patients we need to run
  m_PatientsToRun.clear();
  for (int i = 0; i < m_PatientList->patients_size(); i++)
    m_PatientsToRun.insert(m_PatientList->patients()[i].id());

  // Remove any id's we have in the results
  SAFE_DELETE(m_CompletedPatientList);
  m_CompletedPatientList = new pulse::study::multiplex_ventilation::bind::PatientStateListData();
  if (FileExists(listFilename))
  {
    if (!SerializeFromFile(listFilename, f))
      return false;
    if (m_CompletedPatientList->patients_size() > 0)
    {
      Info("Found " + std::to_string(m_CompletedPatientList->patients_size()) + " previously run Patients");
        for (int i = 0; i < m_CompletedPatientList->patients_size(); i++)
          m_PatientsToRun.erase(m_CompletedPatientList->patients()[i].id());
    }
  }

  int numPatientsToRun = m_PatientList->patients_size() - m_CompletedPatientList->patients_size();
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
  Info("Starting " + std::to_string(processor_count) + " Threads to generate "+std::to_string(numPatientsToRun)+" patients");
  // Crank up our threads
  for (size_t p = 0; p < processor_count; p++)
    m_Threads.push_back(std::thread(&MVGenerator::ControllerLoop, this));

  for (size_t p = 0; p < processor_count; p++)
    m_Threads[p].join();

  Info("It took " + to_scientific_notation(profiler.GetElapsedTime_s("Total")) + "s to run this Patient list");
  profiler.Clear();
  return true;
}

void MVGenerator::ControllerLoop()
{
  MVController mvc(m_DataDir);
  mvc.GetLogger()->LogToConsole(false);
  pulse::study::multiplex_ventilation::bind::PatientStateData* p;

  while (true)
  {
    p = GetNextPatient();
    if (p == nullptr)
      break;
    mvc.GenerateStabilizedPatient(*p);
    FinalizePatient(*p);
  }
}

pulse::study::multiplex_ventilation::bind::PatientStateData* MVGenerator::GetNextPatient()
{
  m_mutex.lock();
  pulse::study::multiplex_ventilation::bind::PatientStateData* p = nullptr;
  if (!m_PatientsToRun.empty())
  {
    int id = *m_PatientsToRun.begin();
    for (int i=0; i< m_PatientList->patients_size(); i++)
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

void MVGenerator::FinalizePatient(pulse::study::multiplex_ventilation::bind::PatientStateData& p)
{
  m_mutex.lock();
  auto ps = m_CompletedPatientList->mutable_patients()->Add();
  ps->CopyFrom(p);
  SerializeToFile(*m_CompletedPatientList, m_PatientStateListFile, SerializationFormat::JSON);
  Info("Completed Patient " + std::to_string(m_CompletedPatientList->patients_size()) + " of " + std::to_string(m_PatientList->patients_size()));
  m_mutex.unlock();
}

bool MVGenerator::SerializeToString(pulse::study::multiplex_ventilation::bind::PatientStateListData& src, std::string& output, SerializationFormat f) const
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
bool MVGenerator::SerializeToFile(pulse::study::multiplex_ventilation::bind::PatientStateListData& src, const std::string& filename, SerializationFormat f) const
{
  std::string content;
  if (!SerializeToString(src, content, f))
    return false;
  return WriteFile(content, filename, SerializationFormat::JSON);
}
bool MVGenerator::SerializeFromString(const std::string& src, pulse::study::multiplex_ventilation::bind::PatientStateListData& dst, SerializationFormat f)
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
bool MVGenerator::SerializeFromFile(const std::string& filename, SerializationFormat f)
{
  std::string content = ReadFile(filename, SerializationFormat::JSON);
  if (content.empty())
  {
    Error("Unable to read file " + filename);
    return false;
  }
  if (!SerializeFromString(content, *m_CompletedPatientList, f))
    return false;
  return true;
}
