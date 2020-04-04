/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

#include "physiology/BloodChemistry.h"
#include "physiology/Cardiovascular.h"
#include "physiology/Drugs.h"
#include "physiology/Endocrine.h"
#include "physiology/Energy.h"
#include "physiology/Gastrointestinal.h"
#include "physiology/Hepatic.h"
#include "physiology/Nervous.h"
#include "physiology/Renal.h"
#include "physiology/Respiratory.h"
#include "physiology/Saturation.h"
#include "physiology/Tissue.h"
#include "environment/Environment.h"
#include "equipment/AnesthesiaMachine.h"
#include "equipment/ECG.h"
#include "equipment/Inhaler.h"
#include "equipment/MechanicalVentilator.h"

#include "engine/SEEngineTracker.h"
#include "engine/SEDataRequestManager.h"
#include "utils/DataTrack.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerPressure.h"

MVController::MVController(const std::string& logFileName, const std::string& data_dir) : Loggable(new Logger(logFileName))
{
  m_TimeStep_s = 0.02;
  m_CurrentTime_s = 0;
  m_SpareAdvanceTime_s = 0;

  m_SubMgr = new SESubstanceManager(GetLogger());
  m_SubMgr->LoadSubstanceDirectory();
  SESubstance* Oxygen = m_SubMgr->GetSubstance("Oxygen");

  m_Dyspnea = new SEDyspnea();
  m_Dyspnea->GetSeverity().SetValue(1.0);

  m_Intubation = new SEIntubation();
  m_Intubation->SetType(eIntubation_Type::Tracheal);

  m_ImpairedAlveolarExchangeExacerbation = new SEImpairedAlveolarExchangeExacerbation();
  m_ImpairedAlveolarExchangeExacerbation->GetImpairedFraction().SetValue(0.25);

  m_MechacicalVentilatorConfiguration = new SEMechanicalVentilatorConfiguration(*m_SubMgr);
  auto& config = m_MechacicalVentilatorConfiguration->GetConfiguration();
  config.SetConnection(eMechanicalVentilator_Connection::Tube);
  config.SetControl(eMechanicalVentilator_Control::P_CMV);
  config.SetDriverWaveform(eMechanicalVentilator_DriverWaveform::Square);
  config.GetRespiratoryRate().SetValue(20, FrequencyUnit::Per_min);
  config.GetInspiratoryExpiratoryRatio().SetValue(0.5);
  config.GetPeakInspiratoryPressure().SetValue(28, PressureUnit::cmH2O);
  config.GetPositiveEndExpiredPressure().SetValue(10, PressureUnit::cmH2O);
  m_FiO2 = &config.GetFractionInspiredGas(*Oxygen);
  m_FiO2->GetFractionAmount().SetValue(0.3);

  std::string baseFileName = logFileName.substr(0, logFileName.length() - 4);
  for (size_t i=0; i<2; i++)
  {
    PulseController* pc = new PulseController(baseFileName+"_p"+std::to_string(i)+".log", data_dir);
    pc->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);

    pc->GetEngineTracker().GetDataRequestManager().SetResultsFilename(baseFileName+"_p"+std::to_string(i)+".csv");
    // FiO2 and Impaired Alveolar Exchange are also manually updated with a Probe below
    pc->GetEngineTracker().GetDataTrack().Probe("AlveolarImpairmentSeverity",
      m_ImpairedAlveolarExchangeExacerbation->GetImpairedFraction().GetValue());
    pc->GetEngineTracker().GetDataTrack().Probe("FiO2", m_FiO2->GetFractionAmount().GetValue());
    pc->GetEngineTracker().GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PeakInspiratoryPressure", PressureUnit::cmH2O);
    pc->GetEngineTracker().GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PositiveEndExpiredPressure", PressureUnit::cmH2O);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("TotalRespiratoryModelCompliance", VolumePerPressureUnit::L_Per_cmH2O);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::L);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::cmH2O);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryExpiratoryRatio");
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
    pc->GetEngineTracker().GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
    pc->GetEngineTracker().SetupRequests();

    pc->ProcessAction(*m_Dyspnea);
    pc->ProcessAction(*m_Intubation);
    pc->ProcessAction(*m_ImpairedAlveolarExchangeExacerbation);
    pc->ProcessAction(*m_MechacicalVentilatorConfiguration);
    // TODO Compliance Override

    m_Patients.push_back(pc);
  }

  SetupMultiplexVentilation();
}

MVController::~MVController()
{
  SAFE_DELETE(m_SubMgr);
  SAFE_DELETE(m_Dyspnea);
  SAFE_DELETE(m_Intubation);
  SAFE_DELETE(m_ImpairedAlveolarExchangeExacerbation);
  SAFE_DELETE(m_MechacicalVentilatorConfiguration);
  DELETE_VECTOR(m_Patients);
}

void MVController::SetupMultiplexVentilation()
{
  // TODO Copy all the Respiratory circuits to our circuit manager
  // TODO Copy all the Respiratory graphs to our compartment manager
}

bool MVController::AdvanceModelTime(double time, const TimeUnit& unit)
{
  double time_s = Convert(time, unit, TimeUnit::s) + m_SpareAdvanceTime_s;
  int count = (int)(time_s / m_TimeStep_s);
  for (int i = 0; i < count; i++)
  {
    for (PulseController* pc : m_Patients)
    {
      if(pc->GetEvents().IsEventActive(eEvent::IrreversibleState))
        return false;
    }
    // PreProcess
    for (PulseController* pc : m_Patients)
      ((Environment&)pc->GetEnvironment()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Cardiovascular&)pc->GetCardiovascular()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Inhaler&)pc->GetInhaler()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Respiratory&)pc->GetRespiratory()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Gastrointestinal&)pc->GetGastrointestinal()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Hepatic&)pc->GetHepatic()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Renal&)pc->GetRenal()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Nervous&)pc->GetNervous()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Energy&)pc->GetEnergy()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Endocrine&)pc->GetEndocrine()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Drugs&)pc->GetDrugs()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((Tissue&)pc->GetTissue()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((BloodChemistry&)pc->GetBloodChemistry()).PreProcess();
    for (PulseController* pc : m_Patients)
      ((ECG&)pc->GetECG()).PreProcess();

    // Process
    for (PulseController* pc : m_Patients)
      ((Environment&)pc->GetEnvironment()).Process();
    for (PulseController* pc : m_Patients)
      ((Cardiovascular&)pc->GetCardiovascular()).Process();
    for (PulseController* pc : m_Patients)
      ((Inhaler&)pc->GetInhaler()).Process();
    for (PulseController* pc : m_Patients)
      ((Respiratory&)pc->GetRespiratory()).Process();
    for (PulseController* pc : m_Patients)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).Process();
    for (PulseController* pc : m_Patients)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).Process();
    for (PulseController* pc : m_Patients)
      ((Gastrointestinal&)pc->GetGastrointestinal()).Process();
    for (PulseController* pc : m_Patients)
      ((Hepatic&)pc->GetHepatic()).Process();
    for (PulseController* pc : m_Patients)
      ((Renal&)pc->GetRenal()).Process();
    for (PulseController* pc : m_Patients)
      ((Nervous&)pc->GetNervous()).Process();
    for (PulseController* pc : m_Patients)
      ((Energy&)pc->GetEnergy()).Process();
    for (PulseController* pc : m_Patients)
      ((Endocrine&)pc->GetEndocrine()).Process();
    for (PulseController* pc : m_Patients)
      ((Drugs&)pc->GetDrugs()).Process();
    for (PulseController* pc : m_Patients)
      ((Tissue&)pc->GetTissue()).Process();
    for (PulseController* pc : m_Patients)
      ((BloodChemistry&)pc->GetBloodChemistry()).Process();
    for (PulseController* pc : m_Patients)
      ((ECG&)pc->GetECG()).Process();

    // PostProcess
    for (PulseController* pc : m_Patients)
      ((Environment&)pc->GetEnvironment()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Cardiovascular&)pc->GetCardiovascular()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Inhaler&)pc->GetInhaler()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Respiratory&)pc->GetRespiratory()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((AnesthesiaMachine&)pc->GetAnesthesiaMachine()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((MechanicalVentilator&)pc->GetMechanicalVentilator()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Gastrointestinal&)pc->GetGastrointestinal()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Hepatic&)pc->GetHepatic()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Renal&)pc->GetRenal()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Nervous&)pc->GetNervous()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Energy&)pc->GetEnergy()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Endocrine&)pc->GetEndocrine()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Drugs&)pc->GetDrugs()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((Tissue&)pc->GetTissue()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((BloodChemistry&)pc->GetBloodChemistry()).PostProcess();
    for (PulseController* pc : m_Patients)
      ((ECG&)pc->GetECG()).PostProcess();

    // Increment Times and track data
    for (PulseController* pc : m_Patients)
    {
      pc->GetEvents().UpdateEvents(pc->GetTimeStep());
      ((SEScalarTime&)pc->GetEngineTime()).Increment(pc->GetTimeStep());
      ((SEScalarTime&)pc->GetSimulationTime()).Increment(pc->GetTimeStep());
      //pc->GetEngineTracker().GetDataTrack().Probe("AlveolarImpairmentSeverity",
      //  m_ImpairedAlveolarExchangeExacerbation->GetImpairedFraction().GetValue());
      //pc->GetEngineTracker().GetDataTrack().Probe("FiO2", m_FiO2->GetFractionAmount().GetValue());
      pc->GetEngineTracker().TrackData(m_CurrentTime_s);
    }
    m_CurrentTime_s += m_TimeStep_s;
  }
  m_SpareAdvanceTime_s = time_s - (count * m_TimeStep_s);
  return true;
}
