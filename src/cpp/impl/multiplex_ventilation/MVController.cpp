/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

MVController::MVController(const std::string& logFileName, const std::string& data_dir) : Loggable(new Logger(logFileName))
{
  m_BaseFileName = logFileName.substr(0, logFileName.length() - 4);
}
MVController::~MVController()
{

}

std::string MVController::to_scientific_notation(float f)
{
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "%g", f);
  return std::string(buffer);
}
std::string MVController::to_scientific_notation(double d)
{
  return to_scientific_notation(float(d));
}

void MVController::TrackData(SEEngineTracker& trkr, const std::string& csv_filename)
{
  trkr.GetDataRequestManager().SetResultsFilename(csv_filename);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("ExpiratoryPulmonaryResistance ", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryPulmonaryResistance ", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  trkr.GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PeakInspiratoryPressure", PressureUnit::cmH2O);
  trkr.GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PositiveEndExpiredPressure", PressureUnit::cmH2O);
  trkr.GetDataRequestManager().CreatePhysiologyDataRequest("TotalRespiratoryModelCompliance", VolumePerPressureUnit::L_Per_cmH2O);
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

void MVController::HandleEvent(eEvent e, bool active, const SEScalarTime* simTime)
{

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
