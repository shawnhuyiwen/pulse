/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

MVController::MVController(const std::string& logFileName, const std::string& data_dir) : Loggable(new Logger(logFileName))
{
  BaseDir = "./test_results/multiplex_ventilation/";
  SoloDir = BaseDir+"solo_states/";
  SoloLogDir = BaseDir + "solo_states_logs/";
  ResultsDir = BaseDir + "simulations/";
}
MVController::~MVController()
{

}

void MVController::TrackData(SEEngineTracker& trkr, const std::string& csv_filename)
{
  trkr.GetDataRequestManager().SetResultsFilename(csv_filename);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("ExpiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  trkr.GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PeakInspiratoryPressure", PressureUnit::cmH2O);
  trkr.GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PositiveEndExpiredPressure", PressureUnit::cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("TotalRespiratoryModelCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::L);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::L);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("CarricoIndex", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryExpiratoryRatio");
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  trkr.SetupRequests();
}

bool MVController::ExtractVentilatorSettings(const std::string& filePath, std::string& fileName, double& pip_cmH2O, double& peep_cmH2O, double& FiO2)
{
  // Parse the file name to get our ventilator settings
  // ex. comp=0.01_peep=10_pip=55_imp=0.3_FiO2=0.21
  size_t peepIdx = filePath.find("peep=") + 5;
  size_t _peepIdx = filePath.find("_", peepIdx);
  peep_cmH2O = std::atof(filePath.substr(peepIdx, _peepIdx).c_str());
  size_t pipIdx = filePath.find("pip=") + 4;
  size_t _pipIdx = filePath.find("_", pipIdx);
  pip_cmH2O = std::atof(filePath.substr(pipIdx, _pipIdx).c_str());
  size_t FiO2Idx = filePath.find("FiO2=") + 5;
  size_t _FiO2Idx = filePath.find(".", FiO2Idx);
  FiO2 = std::atof(filePath.substr(FiO2Idx, _FiO2Idx).c_str());

  fileName = filePath.substr(filePath.find_last_of("/") + 1);
  fileName = fileName.substr(0, fileName.length() - 5);
  return true;
}

void MVController::HandleEvent(eEvent e, bool active, const SEScalarTime* simTime)
{

}

bool MVController::RunSoloState(const std::string& stateFile, const std::string& resultBasePath, double duration_s)
{
  std::string logFile = resultBasePath+".log";
  std::string dataFile = resultBasePath+"Results.csv";

  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");
  double statusTime_s = 0;// Current time of this status cycle
  double statusStep_s = 60;//How long did it take to simulate this much time

  double timeStep_s = 0.02;
  double currentTime_s = 0;

  PulseController* pc = new PulseController(logFile);
  pc->SerializeFromFile(stateFile, SerializationFormat::JSON);
  TrackData(pc->GetEngineTracker(), dataFile);
  int count = (int)(duration_s / timeStep_s);
  for (int i = 0; i < count; i++)
  {
    if (pc->GetEvents().IsEventActive(eEvent::IrreversibleState))
      return false;
    pc->AdvanceModelTime();
    pc->GetEngineTracker().TrackData(currentTime_s);
    currentTime_s += timeStep_s;
    statusTime_s += timeStep_s;
    // How are we running?
    if (statusTime_s > statusStep_s)
    {
      statusTime_s = 0;
      Info("Current Time is " + to_scientific_notation(currentTime_s) + "s, it took " + to_scientific_notation(profiler.GetElapsedTime_s("Status")) + "s to simulate the past " + to_scientific_notation(statusStep_s) + "s");
      profiler.Reset("Status");
    }
  }
  Info("It took " + to_scientific_notation(profiler.GetElapsedTime_s("Total")) + "s to run this simulation");
  profiler.Clear();
  return true;
}

bool MVController::StabilizeSpO2(PhysiologyEngine& eng)
{
  // Let's shoot for with in 0.25% for 10s straight
  double currentSpO2 = 0;
  double previsouSpO2 = eng.GetBloodChemistrySystem()->GetOxygenSaturation();
  int passes = 0;
  int totalIterations = 0;
  int passesUnder80 = 0;
  while (passes < 5)
  {
    totalIterations++;
    eng.AdvanceModelTime(2, TimeUnit::s);
    currentSpO2 = eng.GetBloodChemistrySystem()->GetOxygenSaturation();
    if (currentSpO2 < 0.8 && currentSpO2 <= previsouSpO2)
    {
      passesUnder80++;
      if (passesUnder80 > 5)
        return false;
      // Just get out of here
    }
    else
      passesUnder80 = 0;
    double pctDiff = GeneralMath::PercentDifference(previsouSpO2, currentSpO2);
    if (pctDiff < 0.25)
      passes++;
    else
    {
      passes = 0;
      previsouSpO2 = currentSpO2;
    }
  }
  Info("Engine stablized at an SpO2 of " + to_scientific_notation(currentSpO2)+" in "+std::to_string(totalIterations *2)+"(s)");
  return true;
}

std::string to_scientific_notation(float f)
{
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "%g", f);
  return std::string(buffer);
}
std::string to_scientific_notation(double d)
{
  return to_scientific_notation(float(d));
}
