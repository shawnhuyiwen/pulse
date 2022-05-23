/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

#include "cdm/scenario/SEScenario.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/actions/SEArrhythmia.h"
#include "cdm/patient/actions/SEIntubation.h"
#include "cdm/patient/actions/SEChestCompressionForceScale.h"
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAutomated.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskInstantaneous.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskSqueeze.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"


//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for going into cardiac arrest
/// and optionally using CPR and or a Bag,Valve,Mask equipment on the patient
///
/// \details
/// Refer to the SEArrhythmia, SEBagValveMask class
//--------------------------------------------------------------------------------------------------
void HowToACLS()
{
  std::map<eHeartRhythm, std::string> startArrhythmias =
  {
    {eHeartRhythm::FineVentricularFibrillation, "fine_vfib"},
    //{eHeartRhythm::CoarseVentricularFibrillation, "coarse_vfib"}
  };
  std::vector<size_t> arrestLengths_min = { 2,4 };
  std::map<eHeartRhythm, std::string> endArrhythmias =
  {
    {eHeartRhythm::NormalSinus, "sinus"},
    //{eHeartRhythm::SinusTachycardia, "tachycardia"},
    //{eHeartRhythm::SinusBradycardia, "bradycardia"},
    //{eHeartRhythm::SinusPulselessElectricalActivity, "pea"}
  };
  std::vector<std::string> interventions =
  {
    "no",
    "good_cpr",
    //"bad_cpr",
    "good_cpr_bvm",
    //"bad_cpr_bvm"
  };
  size_t interventionTime_s = 5*60;
  size_t extraBVMTime = 60; // BVM time after the heart starts back up

  SEAdvanceTime adv;
  SEArrhythmia  arrhythmia;
  SEBagValveMaskConfiguration bvm;
  SEBagValveMaskSqueeze bvmSqueeze;
  SEChestCompressionForceScale cpr;

  Logger logger("./test_results/howto/HowTo_ACLS.cpp/HowTo_ACLS.log");
  logger.LogToConsole(true);

  double simStep_s = 0.5;

  for (auto start : startArrhythmias)
  {
    for (size_t arrestLength_min : arrestLengths_min)
    {
      for (auto end : endArrhythmias)
      {
        for (auto ivn : interventions)
        {
          bool performBVM = false;
          bool performBadCPR = false;
          bool performGoodCPR = false;

          std::string name;
          if (ivn == "no")
            name = start.second + "_for_" + std::to_string(arrestLength_min) + "m_to_" + end.second + "_with_" + ivn + "_intervention";
          else
            name = start.second + "_for_" + std::to_string(arrestLength_min) + "m_then_" + std::to_string(interventionTime_s / 60) + "m_of_" + ivn + "_to_" + end.second;

          logger.Info("Creating scenario " + name);

          SEScenario sce(&logger);
          sce.SetName(name);
          sce.SetEngineStateFile("./states/StandardMale@0s.json");
          //sce.GetPatientConfiguration().SetPatientFile("./patients/StandardMale.json");
          SEDataRequestManager& dMgr = sce.GetDataRequestManager();
          dMgr.CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
          dMgr.CreatePhysiologyDataRequest("ArterialPressure", PressureUnit::mmHg);
          dMgr.CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::mmHg);
          dMgr.CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
          dMgr.CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
          dMgr.CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
          dMgr.CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
          dMgr.CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
          dMgr.CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
          dMgr.CreatePhysiologyDataRequest("CarbonDioxideProductionRate", VolumePerTimeUnit::mL_Per_min);
          dMgr.CreatePhysiologyDataRequest("OxygenConsumptionRate", VolumePerTimeUnit::mL_Per_min);
          dMgr.CreatePhysiologyDataRequest("OxygenSaturation");
          dMgr.CreateSubstanceDataRequest("Oxygen", "AlveolarTransfer", VolumePerTimeUnit::mL_Per_s);
          dMgr.CreateSubstanceDataRequest("CarbonDioxide", "AlveolarTransfer", VolumePerTimeUnit::mL_Per_s);
          dMgr.CreateGasCompartmentDataRequest("Carina", "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
          dMgr.CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "Oxygen", "PartialPressure");
          dMgr.CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "CarbonDioxide", "PartialPressure");
          dMgr.CreateECGDataRequest("Lead3ElectricPotential", ElectricPotentialUnit::mV);

          dMgr.CreatePhysiologyDataRequest("BaroreceptorHeartRateScale");
          dMgr.CreatePhysiologyDataRequest("BaroreceptorHeartElastanceScale");
          dMgr.CreatePhysiologyDataRequest("BaroreceptorResistanceScale");
          dMgr.CreatePhysiologyDataRequest("BaroreceptorComplianceScale");
          dMgr.CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
          dMgr.CreatePhysiologyDataRequest("SystemicVascularResistance", PressureTimePerVolumeUnit::mmHg_s_Per_mL);

          double simTime_s = 0;

          adv.GetTime().SetValue(5, TimeUnit::s);
          sce.AddAction(adv);
          simTime_s += adv.GetTime(TimeUnit::s);
          logger.Info("  -Advancing to " + PULSE_CDM::to_string(simTime_s) + "s");

          arrhythmia.SetRhythm(start.first);
          sce.AddAction(arrhythmia);
          logger.Info("  -Setting heart rhythm to " + eHeartRhythm_Name(start.first));

          adv.GetTime().SetValue(arrestLength_min, TimeUnit::min);
          sce.AddAction(adv);
          simTime_s += adv.GetTime(TimeUnit::s);
          logger.Info("  -Advancing to " + PULSE_CDM::to_string(simTime_s) + "s");

          if (ivn != "no")
          {
            if (ivn.find("good_cpr") != std::string::npos)
            {
              performGoodCPR = true;
            }
            else if (ivn.find("bad_cpr") != std::string::npos)
            {
              performBadCPR = true;
            }

            if (ivn.find("bvm") != std::string::npos)
            {
              performBVM = true;
              SEIntubation intubate;
              intubate.SetType(eIntubation_Type::Tracheal);
              sce.AddAction(intubate);

              adv.GetTime().SetValue(1, TimeUnit::s);
              sce.AddAction(adv);
              simTime_s += adv.GetTime(TimeUnit::s);
              logger.Info("  -Advancing to " + PULSE_CDM::to_string(simTime_s) + "s");

              bvm.GetConfiguration().SetConnection(eSwitch::On);
              bvm.GetConfiguration().GetValvePositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
              sce.AddAction(bvm);

              adv.GetTime().SetValue(1, TimeUnit::s);
              sce.AddAction(adv);
              simTime_s += adv.GetTime(TimeUnit::s);
              logger.Info("  -Advancing to " + PULSE_CDM::to_string(simTime_s) + "s");
            }

            size_t bvmSqueezeCnt = 10;
            size_t doSqueeze = bvmSqueezeCnt;
            // Advance for a set amount of time in Increments of a half second
            for (double t = 0; t < interventionTime_s; t += simStep_s)
            {
              if (performBVM && doSqueeze == bvmSqueezeCnt++)
              {
                bvmSqueezeCnt = 1;
                logger.Info("  -Adding BVM Squeeze @ " + PULSE_CDM::to_string(simTime_s));
                bvmSqueeze.GetExpiratoryPeriod().SetValue(1.0, TimeUnit::s);
                bvmSqueeze.GetInspiratoryPeriod().SetValue(1.0, TimeUnit::s);
                sce.AddAction(bvmSqueeze);
              }

              if (performGoodCPR)
              {
                logger.Info("  -Adding Good CPR Compression @ " + PULSE_CDM::to_string(simTime_s));
                cpr.GetForceScale().SetValue(0.6228);
                sce.AddAction(cpr);
              }

              if (performBadCPR)
              {
                logger.Info("  -Adding Bad CPR Compression @ " + PULSE_CDM::to_string(simTime_s));
                cpr.GetForceScale().SetValue(0.2228);
                sce.AddAction(cpr);
              }

              adv.GetTime().SetValue(simStep_s, TimeUnit::s);
              sce.AddAction(adv);
              simTime_s += adv.GetTime(TimeUnit::s);
            }
          }

          arrhythmia.SetRhythm(end.first);
          sce.AddAction(arrhythmia);
          logger.Info("  -Setting heart rhythm to " + eHeartRhythm_Name(end.first));

          // Extra BVM Time
          if (extraBVMTime>0)
          {
            size_t bvmSqueezeCnt = 10;
            size_t doSqueeze = bvmSqueezeCnt;
            // Advance for a set amount of time in Increments of a half second
            for (double t = 0; t < extraBVMTime; t += simStep_s)
            {
              if (doSqueeze == bvmSqueezeCnt++)
              {
                bvmSqueezeCnt = 1;
                logger.Info("  -Adding BVM Squeeze @ " + PULSE_CDM::to_string(simTime_s));
                bvmSqueeze.GetExpiratoryPeriod().SetValue(1.0, TimeUnit::s);
                bvmSqueeze.GetInspiratoryPeriod().SetValue(1.0, TimeUnit::s);
                sce.AddAction(bvmSqueeze);
              }

              adv.GetTime().SetValue(simStep_s, TimeUnit::s);
              sce.AddAction(adv);
              simTime_s += adv.GetTime(TimeUnit::s);
            }
          }

          adv.GetTime().SetValue(5, TimeUnit::min);
          sce.AddAction(adv);
          simTime_s += adv.GetTime(TimeUnit::s);
          logger.Info("  -Advancing to " + PULSE_CDM::to_string(simTime_s) + "s");

          sce.SerializeToFile("./test_results/howto/HowTo_ACLS.cpp/" + name + ".json");
        }
      }
    }
  }
}
