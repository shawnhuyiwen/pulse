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
  int minCompliance = 10;
  int maxCompliance = 50;
  int stepCompliance = 10;
  float minImpairment = 0.0;
  float maxImpairment = 1.0;
  float stepImpairment = 0.1f;
  int minPEEP = 12;
  int maxPEEP = 20;
  int stepPEEP = 2;
  int minPIP = 10;
  int maxPIP = 40;
  int stepPIP = 2;
  float minFiO2 = 0.10f;
  float maxFiO2 = 0.60f;
  float stepFiO2 = 0.05f;

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

  SEMechanicalVentilatorConfiguration mvc(subMgr);
  auto& mv = mvc.GetConfiguration();
  mv.SetConnection(eMechanicalVentilator_Connection::Tube);
  mv.SetControl(eMechanicalVentilator_Control::P_CMV);
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

  for (int c=minCompliance; c<maxCompliance; c+=stepCompliance)
  {
    overrides.AddScalarProperty("LungCompliance", c, VolumePerPressureUnit::L_Per_cmH2O);

    for (float i=minImpairment; i<maxImpairment; i+=stepImpairment)
    {
      // jbw Convert i to the appropriate action severity
      impairedAlveolarExchange.GetSeverity().SetValue(i);// jbw support severity!
      pulmonaryShunt.GetSeverity().SetValue(i);

      for (int peep=minPEEP; peep<maxPEEP; peep+=stepPEEP)
      {
        mv.GetInspiratoryExpiratoryRatio().SetValue(peep);

        // Construct our engine name
        baseName = "comp="+std::to_string(c)+"_imp="+to_scientific_notation(i)+"_peep="+std::to_string(peep);
        logger.Info("Creating engine " + baseName);
        auto pulse = CreatePulseEngine("./states/multiplex_ventilation/"+baseName+".log");
        //pulse->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);

        // Step the PIP until we get a TidalVolume between 6-8 mL
        for (int pip=minPIP; pip<maxPIP; pip+=stepPIP)
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
  return WriteFile(out, "mv_solo_ventilated_patients.json", SerializationFormat::JSON);
}