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
