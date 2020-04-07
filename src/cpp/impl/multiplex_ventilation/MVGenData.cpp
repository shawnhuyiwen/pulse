/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

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
  int stepPIP_cmH2O = 1;
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
  overrides.AddScalarProperty("RespiratoryResistance", m_Resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SEImpairedAlveolarExchangeExacerbation impairedAlveolarExchange;
  SEPulmonaryShuntExacerbation pulmonaryShunt;


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
      mv.GetPeakInspiratoryPressure().SetValue(peep, PressureUnit::cmH2O);
      mv.GetPositiveEndExpiredPressure().SetValue(peep, PressureUnit::cmH2O);
      currentPIP_cmH2O = peep;

      // Create an engine here to create a state we can reuse in the next for loop
      baseName = "comp="+to_scientific_notation(c)+"_peep="+std::to_string(peep);
      Info("Creating engine " + baseName);
      auto pip_stepper = CreatePulseEngine("./states/multiplex_ventilation/"+baseName+".log");
      pip_stepper->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);
      // Add our initial actions
      pip_stepper->ProcessAction(dyspnea);
      pip_stepper->ProcessAction(intubation);
      pip_stepper->ProcessAction(overrides);
      pip_stepper->ProcessAction(mvc);

      // Step the PIP until we get a TidalVolume between 6 ml/kg (ideal body weight)
      double targetTidalVolume_mL = 6.0 * pip_stepper->GetPatient().GetIdealBodyWeight(MassUnit::kg);
      double currentTidalVolume_mL = pip_stepper->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL);
      Info("Patient starting with a tidal volume of "+to_scientific_notation(currentTidalVolume_mL)+"(mL)");
      Info("Targeting a tidal volume of "+to_scientific_notation(targetTidalVolume_mL)+"(mL)");
      bool tidalVolumeRegistered = false;
      while (currentTidalVolume_mL < targetTidalVolume_mL || !tidalVolumeRegistered)
      {
        currentPIP_cmH2O += stepPIP_cmH2O;
        Info("..Incrementing PIP to" + std::to_string(currentPIP_cmH2O) + "(cmH2O)");
        mv.GetPeakInspiratoryPressure().SetValue(currentPIP_cmH2O, PressureUnit::cmH2O);
        pip_stepper->ProcessAction(mvc);
        pip_stepper->AdvanceModelTime(breathPeriod_s*2, TimeUnit::s);
        double updatedTidalVolume_mL = pip_stepper->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL);
        if (updatedTidalVolume_mL == currentTidalVolume_mL)
        {
          // The tidal volume was so low that it didnt' register a breath
          tidalVolumeRegistered = false;
        }
        else
        {
          tidalVolumeRegistered = true;
        }
        currentTidalVolume_mL = updatedTidalVolume_mL;
        Info("..Patient is now at a tidal volume of "+std::to_string(currentTidalVolume_mL)+"(mL)");
      }
      Info("Stabilized to a tidal volume of "+to_scientific_notation(currentTidalVolume_mL)+"(mL), with a PIP of "+to_scientific_notation(currentPIP_cmH2O));
      // Save this state out
      Info("Saving engine state" + baseName+".json");
      pip_stepper->SerializeToFile("./states/multiplex_ventilation/" + baseName + ".json", SerializationFormat::JSON);

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