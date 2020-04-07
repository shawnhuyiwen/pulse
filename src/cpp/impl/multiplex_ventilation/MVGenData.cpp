/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

#include "engine/SEOverrides.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SEImpairedAlveolarExchangeExacerbation.h"
#include "patient/actions/SEPulmonaryShuntExacerbation.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "equipment/mechanical_ventilator/SEMechanicalVentilator.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "utils/FileUtils.h"

#include "pulse/impl/bind/MultiplexVentilator.pb.h"
#include <google/protobuf/util/json_util.h>

std::string to_scientific_notation(float f)
{
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "%g", f);
  return std::string(buffer);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Generate a set of patients stabilized on a ventilator
/// 1 ventilator for each patient
//--------------------------------------------------------------------------------------------------
bool GenerateStabilizedPatients()
{
  float minCompliance_L_Per_cmH2O = 0.010f;
  float maxCompliance_L_Per_cmH2O = 0.050f;
  float stepCompliance_L_Per_cmH2O = 0.005f;
  float minImpairment = 3.0f;
  float maxImpairment = 9.0f;
  float stepImpairment = 0.3f;
  int minPEEP_cmH2O = 10;
  int maxPEEP_cmH2O = 20;
  int stepPEEP_cmH2O = 2;
  int resistance_cmH2O_s_Per_L = 5;

  //TODO: These will be determined through a two-step stabilization process ----
  //Instead of PIP, we should probably use driving pressure (PIP-PEEP)
  int minPIP_cmH2O = 20;
  int maxPIP_cmH2O = 40;
  int stepPIP_cmH2O = 2;
  float minFiO2 = 0.10f;
  float maxFiO2 = 0.95f;
  float stepFiO2 = 0.5f;
  //----------------------------------------------------------------------------

  Logger logger("MultiplexVentilationGenData.log");
  SESubstanceManager subMgr(&logger);
  subMgr.LoadSubstanceDirectory();
  SESubstance* Oxygen = subMgr.GetSubstance("Oxygen");

  SEDyspnea dyspnea;
  dyspnea.GetSeverity().SetValue(1.0);

  SEIntubation intubation;
  intubation.SetType(eIntubation_Type::Tracheal);

  SEOverrides overrides;

  SEImpairedAlveolarExchangeExacerbation impairedAlveolarExchange;
  SEPulmonaryShuntExacerbation pulmonaryShunt;

  overrides.AddScalarProperty("RespiratoryResistance", resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SEMechanicalVentilatorConfiguration mvc(subMgr);
  auto& mv = mvc.GetConfiguration();
  mv.SetConnection(eMechanicalVentilator_Connection::Tube);
  mv.SetControl(eMechanicalVentilator_Control::PC_CMV);
  mv.SetDriverWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.GetRespiratoryRate().SetValue(20, FrequencyUnit::Per_min);
  mv.GetInspiratoryExpiratoryRatio().SetValue(0.5);
  mv.GetPeakInspiratoryPressure().SetValue(28, PressureUnit::cmH2O);
  mv.GetPositiveEndExpiredPressure().SetValue(10, PressureUnit::cmH2O);
  SESubstanceFraction* FiO2 = &mv.GetFractionInspiredGas(*Oxygen);
  FiO2->GetFractionAmount().SetValue(0.3);

  std::string baseName;
  pulse::multiplex_ventilator::bind::PatientStateListData patients;
  google::protobuf::util::JsonPrintOptions opts;
  opts.add_whitespace = true;
  opts.preserve_proto_field_names = true;
  // Hook up protobuf error handler to our handler
  google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
  {
    std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
  });

  for (int c=minCompliance_L_Per_cmH2O; c<=maxCompliance_L_Per_cmH2O; c+=stepCompliance_L_Per_cmH2O)
  {
    overrides.AddScalarProperty("RespiratoryCompliance", c, VolumePerPressureUnit::L_Per_cmH2O);

    for (float i=minImpairment; i<=maxImpairment; i+=stepImpairment)
    {
      impairedAlveolarExchange.GetSeverity().SetValue(i);
      pulmonaryShunt.GetSeverity().SetValue(i);

      for (int peep=minPEEP_cmH2O; peep<=maxPEEP_cmH2O; peep+=stepPEEP_cmH2O)
      {
        mv.GetInspiratoryExpiratoryRatio().SetValue(peep);

        // Construct our engine name
        baseName = "comp="+std::to_string(c)+"_imp="+to_scientific_notation(i)+"_peep="+std::to_string(peep);
        logger.Info("Creating engine " + baseName);
        auto pulse = CreatePulseEngine("./states/multiplex_ventilation/"+baseName+".log");
        pulse->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);

        // Step the PIP until we get a TidalVolume between 6-8 mL
        for (int pip=minPIP_cmH2O; pip<maxPIP_cmH2O; pip+=stepPIP_cmH2O)
        {
          mv.GetPeakInspiratoryPressure().SetValue(pip, PressureUnit::cmH2O);
        }

        // Step the FiO2 until we get an SpO2 of > 90
        for (float o2=minFiO2; o2<maxFiO2; o2+=stepFiO2)
        {
          FiO2->GetFractionAmount().SetValue(o2);
        }

        // Save our state
        logger.Info("Saving engine state"+baseName+".json");
        pulse->SerializeToFile("./states/multiplex_ventilation/"+baseName+".json",SerializationFormat::JSON);
        // Append to our "list" of generated states
        auto patientData = patients.add_patients();
        patientData->set_statefile("./states/multiplex_ventilation/"+baseName+".json");
        patientData->set_compliance_l_per_cmh2o(c);
        patientData->set_impairmentfraction(i);
        patientData->set_peep_cmh2o(peep);
        patientData->set_pip_cmh2o(mv.GetPeakInspiratoryPressure().GetValue(PressureUnit::cmH2O));
        patientData->set_fio2(FiO2->GetFractionAmount().GetValue());
        patientData->set_oxygensaturation(pulse->GetBloodChemistrySystem()->GetOxygenSaturation());
        patientData->set_tidalvolume_l(pulse->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::L));
        patientData->set_endtidalcarbondioxidepressure_cmh2o(pulse->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::cmH2O));
        patientData->set_carricoindex(pulse->GetRespiratorySystem()->GetCarricoIndex(PressureUnit::mmHg));
      }
    }
  }
  // Write out our list file
  std::string out;
  if (!google::protobuf::util::MessageToJsonString(patients, &out, opts).ok())
  {
    std::cerr << "Unable to write generated patients list file\n";
    return false;
  }
  logger.Info("Created "+std::to_string(patients.patients_size()) + " patients");
  return WriteFile(out, "mv_solo_ventilated_patients.json", SerializationFormat::JSON);
}