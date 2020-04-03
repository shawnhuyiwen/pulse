/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

MVController::MVController(const std::string& logFileName, const std::string& data_dir) : Loggable(new Logger(logFileName))
{
  m_SpareAdvanceTime_s = 0;
  for (size_t i=0; i<1; i++)
  {
    MVPulseController* pc = new MVPulseController(logFileName + "_p" + std::to_string(i), data_dir);
    pc->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);
    m_Patients.push_back(pc);
  }

  SetupMultiplexVentilation();
}

MVController::~MVController()
{
  DELETE_VECTOR(m_Patients);
}

void MVController::SetupMultiplexVentilation()
{
  // TODO Copy all the Respiratory circuits to our circuit manager
  // TODO Copy all the Respiratory graphs to our compartment manager
}

void MVController::AdvanceModelTime(double time, const TimeUnit& unit)
{
  double time_s = Convert(time, unit, TimeUnit::s) + m_SpareAdvanceTime_s;
  int count = (int)(time_s / 0.02);
  for (int i = 0; i < count; i++)
  {
    PreProcess();
    Process();
    PostProcess();
  }
  m_SpareAdvanceTime_s = time_s - (count * 0.02);
}

void MVController::PreProcess()
{
  for (MVPulseController* pc : m_Patients)
    pc->PreProcess();
}
void MVController::Process()
{
  for (MVPulseController* pc : m_Patients)
    pc->Process();
}
void MVController::PostProcess()
{
  for (MVPulseController* pc : m_Patients)
    pc->PostProcess();
}
