/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

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
bool MVController::GenerateStabilizedPatients()
{
  // Loop parameters
  float minCompliance_L_Per_cmH2O = 0.010f;
  float maxCompliance_L_Per_cmH2O = 0.050f;
  float stepCompliance_L_Per_cmH2O = 0.005f;
  int minPEEP_cmH2O = 10;
  int maxPEEP_cmH2O = 20;
  int stepPEEP_cmH2O = 2;
  float minImpairment = 0.3f;
  float maxImpairment = 0.9f;
  float stepImpairment = 0.3f;

  SESubstanceManager subMgr(GetLogger());
  subMgr.LoadSubstanceDirectory();
  SESubstance* Oxygen = subMgr.GetSubstance("Oxygen");

  SEDyspnea dyspnea;
  dyspnea.GetSeverity().SetValue(1.0);

  SEIntubation intubation;
  intubation.SetType(eIntubation_Type::Tracheal);

  SEOverrides overrides;

  SEImpairedAlveolarExchangeExacerbation impairedAlveolarExchange;
  SEPulmonaryShuntExacerbation pulmonaryShunt;

  overrides.AddScalarProperty("RespiratoryResistance", m_Resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SEMechanicalVentilatorConfiguration mvc(subMgr);
  auto& mv = mvc.GetConfiguration();
  mv.SetConnection(eMechanicalVentilator_Connection::Tube);
  mv.SetControl(eMechanicalVentilator_Control::PC_CMV);
  mv.SetDriverWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.GetRespiratoryRate().SetValue(m_RespirationRate_Per_Min, FrequencyUnit::Per_min);
  mv.GetInspiratoryExpiratoryRatio().SetValue(m_IERatio);
  mv.GetPeakInspiratoryPressure().SetValue(minPEEP_cmH2O, PressureUnit::cmH2O);
  mv.GetPositiveEndExpiredPressure().SetValue(minPEEP_cmH2O, PressureUnit::cmH2O);
  SESubstanceFraction* FiO2 = &mv.GetFractionInspiredGas(*Oxygen);
  FiO2->GetFractionAmount().SetValue(m_AmbientFiO2);

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

  double breathPeriod_s = 60.0 / m_RespirationRate_Per_Min;

  for (float c = minCompliance_L_Per_cmH2O; c <= maxCompliance_L_Per_cmH2O; c += stepCompliance_L_Per_cmH2O)
  {
    overrides.AddScalarProperty("RespiratoryCompliance", c, VolumePerPressureUnit::L_Per_cmH2O);

    double currentPIP_cmH2O = 0;
    for (int peep = minPEEP_cmH2O; peep <= maxPEEP_cmH2O; peep += stepPEEP_cmH2O)
    {
      mv.GetInspiratoryExpiratoryRatio().SetValue(peep);

      // Step the PIP until we get a TidalVolume between 6 ml/kg (ideal body weight)
      double targetTidalVolume_mL = 6.0 * 77.0; //Aaron, how do I get the patient's ideal body weight here?
      double currentTidalVolume_mL = 0.0;
      currentPIP_cmH2O = peep;
      while (currentTidalVolume_mL < targetTidalVolume_mL)
      {
        currentPIP_cmH2O = peep + 1.0;
        // Aaron - simulation for 2 breathPeriod_s
        // Test TV is at the target
        // Make sure the TV has actually changed - sometimes when it's too low it won't register as a breath
        // currentTidalVolume_mL = get TV
      }

      // This PIP is now good for all patients with this compliance and PEEP combination
      mv.GetPeakInspiratoryPressure().SetValue(currentPIP_cmH2O, PressureUnit::cmH2O);

      double currentFiO2 = m_AmbientFiO2;
      for (float i = minImpairment; i <= maxImpairment; i += stepImpairment)
      {
        impairedAlveolarExchange.GetSeverity().SetValue(i);
        pulmonaryShunt.GetSeverity().SetValue(i);

        // Construct our engine name
        baseName = "comp="+std::to_string(c)+"_imp="+to_scientific_notation(i)+"_peep="+std::to_string(peep);
        Info("Creating engine " + baseName);
        auto pulse = CreatePulseEngine("./states/multiplex_ventilation/" + baseName + ".log");
        pulse->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);

        // Step the FiO2 until we get an SpO2 of > 90
        double SpO2 = 0.1;
        while (SpO2 < 0.9)
        {
          if (currentFiO2 > 0.995)
          {
            currentFiO2 = 0.995;
            break;
          }

          //Start with the prvious FiO2 because this should require a higher value to acheive our desired SpO2
          FiO2->GetFractionAmount().SetValue(currentFiO2);

          // Run until stable
          double previousSPO2 = 0.0;
          double criteria = 0.01; //TODO: We need to play with this
          while (abs(SpO2 - previousSPO2) > criteria)
          {
            previousSPO2 = SpO2;
            //Aaron - simulation for a breathPeriod_s
            //SpO2 = get spO2
          }

          currentFiO2 += 0.01;
        }

        // Save our state
        baseName += "_pip="+std::to_string(currentPIP_cmH2O)+"_FiO2="+std::to_string(currentFiO2);
        Info("Saving engine state" + baseName+".json");
        pulse->SerializeToFile("./states/multiplex_ventilation/" + baseName + ".json", SerializationFormat::JSON);
        // Append to our "list" of generated states
        auto patientData = patients.add_patients();
        patientData->set_statefile("./states/multiplex_ventilation/" + baseName + ".json");
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
  Info("Created "+std::to_string(patients.patients_size()) + " patients");
  return WriteFile(out, "mv_solo_ventilated_patients.json", SerializationFormat::JSON);
}