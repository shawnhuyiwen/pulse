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
  TimingProfile profiler;
  profiler.Start("Total");

  DeleteDirectory(SoloDir, true);
  // Loop parameters
  float minCompliance_L_Per_cmH2O = 0.010f;
  float maxCompliance_L_Per_cmH2O = 0.050f;
  float stepCompliance_L_Per_cmH2O = 0.01f;
  int minPEEP_cmH2O = 10;
  int maxPEEP_cmH2O = 20;
  int stepPEEP_cmH2O = 5;
  float minImpairment = 0.3f;
  float maxImpairment = 0.9f;
  float stepImpairment = 0.025f;

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
  pulse::study::multiplex_ventilation::bind::PatientStateListData patients;
  google::protobuf::util::JsonPrintOptions opts;
  opts.add_whitespace = true;
  opts.preserve_proto_field_names = true;
  // Hook up protobuf error handler to our handler
  google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
  {
    std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
  });

  unsigned int totalIterations = (rint((maxCompliance_L_Per_cmH2O - minCompliance_L_Per_cmH2O) / stepCompliance_L_Per_cmH2O) + 1) *
    ((maxPEEP_cmH2O - minPEEP_cmH2O) / stepPEEP_cmH2O + 1) *
    (rint((maxImpairment - minImpairment) / stepImpairment + 1));
  
  unsigned int currentIteration = 0;

  for (float compliance_L_Per_cmH2O = minCompliance_L_Per_cmH2O; compliance_L_Per_cmH2O <= maxCompliance_L_Per_cmH2O; compliance_L_Per_cmH2O += stepCompliance_L_Per_cmH2O)
  {
    overrides.AddScalarProperty("RespiratoryCompliance", compliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);

    for (int PEEP_cmH2O = minPEEP_cmH2O; PEEP_cmH2O <= maxPEEP_cmH2O; PEEP_cmH2O += stepPEEP_cmH2O)
    {
      // RC circuit charging equation
      // Assume tube resistances are negligable
      double targetTidalVolume_mL = 6.0 * 75.3; // Aaron - What's the best way to get the ideal body weight from the patient?
      double breathPeriod_s = 60.0 / m_RespirationRate_Per_Min;
      double inspiratoryPeriod_s = m_IERatio * breathPeriod_s / (1.f + m_IERatio);
      double targetTidalVolume_L = targetTidalVolume_mL / 1000.0;
      int PIP_cmH2O = int(targetTidalVolume_L / (compliance_L_Per_cmH2O * (1.0 - exp(-inspiratoryPeriod_s / (m_Resistance_cmH2O_s_Per_L * compliance_L_Per_cmH2O)))) + PEEP_cmH2O);

      // Set up the ventilator
      mv.GetPeakInspiratoryPressure().SetValue(PIP_cmH2O, PressureUnit::cmH2O);
      mv.GetPositiveEndExpiredPressure().SetValue(PEEP_cmH2O, PressureUnit::cmH2O);
      FiO2->GetFractionAmount().SetValue(m_AmbientFiO2); // Set our FiO2 to Atmosphere

      Info("Calculated PIP of " + std::to_string(PIP_cmH2O) + "(cmH2O) for a tidal volume of " + to_scientific_notation(targetTidalVolume_mL) + "(mL) and a PEEP of " + std::to_string(PEEP_cmH2O) + "(cmH2O)");

      for (float currentImpairment = minImpairment; currentImpairment <= maxImpairment; currentImpairment += stepImpairment) //Aaron - This needs to be maxImpairment instead of 1.0, but rounding screws it up
      {
        Info("\n########################### PATIENT " + std::to_string(currentIteration + 1) + " OF " + std::to_string(totalIterations) + " ###########################");

        // Construct our engine
        baseName = "comp="+to_scientific_notation(compliance_L_Per_cmH2O)+"_peep="+std::to_string(PEEP_cmH2O)+"_pip="+std::to_string(PIP_cmH2O)+"_imp="+to_scientific_notation(currentImpairment);
        Info("Creating engine " + baseName);
        auto engine = CreatePulseEngine(SoloDir+"/log/"+baseName+".log");
        engine->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);
        TrackData(*engine->GetEngineTracker(), SoloDir+"/csv/"+baseName + ".csv");

        // Add our initial actions
        impairedAlveolarExchange.GetSeverity().SetValue(currentImpairment);
        pulmonaryShunt.GetSeverity().SetValue(currentImpairment);
        engine->ProcessAction(dyspnea);
        engine->ProcessAction(intubation);
        engine->ProcessAction(overrides);
        engine->ProcessAction(mvc);
        engine->ProcessAction(impairedAlveolarExchange);
        engine->ProcessAction(pulmonaryShunt);
        StabilizeSpO2(*engine);


        float  targetSpO2 = 0.92f;
        double previousFiO2 = FiO2->GetFractionAmount().GetValue();
        double previousSpO2 = engine->GetBloodChemistrySystem()->GetOxygenSaturation();
        double currentFiO2 = previousFiO2;
        double currentSpO2 = previousSpO2;
        Info("Starting stabilization at " + to_scientific_notation(previousSpO2) + " SpO2");

        while (true)
        {
          StabilizeSpO2(*engine);
          currentSpO2 = engine->GetBloodChemistrySystem()->GetOxygenSaturation();
          if (currentSpO2 >= targetSpO2)
            break;

          if (currentSpO2 < previousSpO2)// Going down...
          {
            if (currentSpO2 < 0.85)
              currentFiO2 += 0.1;
            else
              currentFiO2 += 0.025;
          }
          else
          {
            if (currentSpO2 < 0.88)
              currentFiO2 += 0.05;
            else
              currentFiO2 += 0.01;
          }
          if (currentFiO2 > 0.9995)
          {
            currentFiO2 = 0.9995;
            break;
          }

          if (currentFiO2 > 0.5)
            targetSpO2 = 0.89f;

          Info("Setting FiO2 to " + to_scientific_notation(currentFiO2) + " with an SpO2 of " + to_scientific_notation(currentSpO2));
          FiO2->GetFractionAmount().SetValue(currentFiO2);
          engine->ProcessAction(mvc);
          previousSpO2 = currentSpO2;
          previousFiO2 = currentFiO2;
        }
        Info("Engine stabilized at an SpO2 of " + to_scientific_notation(currentSpO2)+"; FiO2 of "+to_scientific_notation(currentFiO2));

        // Save our state
        baseName += "_FiO2="+to_scientific_notation(currentFiO2);
        Info("Saving engine state" + baseName+".json");
        engine->SerializeToFile(SoloDir+baseName+".json", SerializationFormat::JSON);
        // Append to our "list" of generated states
        auto patientData = patients.add_patients();
        patientData->set_statefile(SoloDir+baseName+".json");
        patientData->set_compliance_l_per_cmh2o(compliance_L_Per_cmH2O);
        patientData->set_impairmentfraction(currentImpairment);
        patientData->set_peep_cmh2o(PEEP_cmH2O);
        patientData->set_pip_cmh2o(mv.GetPeakInspiratoryPressure().GetValue(PressureUnit::cmH2O));
        patientData->set_fio2(FiO2->GetFractionAmount().GetValue());
        patientData->set_oxygensaturation(engine->GetBloodChemistrySystem()->GetOxygenSaturation());
        patientData->set_tidalvolume_l(engine->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::L));
        patientData->set_endtidalcarbondioxidepressure_cmh2o(engine->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::cmH2O));
        auto AortaO2 = engine->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetSubstanceQuantity(*engine->GetSubstanceManager().GetSubstance("Oxygen"));
        patientData->set_arterialoxygenpartialpressure_mmhg(AortaO2->GetPartialPressure(PressureUnit::mmHg));
        patientData->set_carricoindex_mmhg(engine->GetRespiratorySystem()->GetCarricoIndex(PressureUnit::mmHg));
        delete engine.release();
        //RunSoloState(SoloDir+baseName+".json", SoloDir+"csv/ext/"+baseName+".json", 120);

        currentIteration++;
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
  Info("It took " + to_scientific_notation(profiler.GetElapsedTime_s("Total")) + "s to generate data");
  profiler.Clear();
  return WriteFile(out, BaseDir+"solo_ventilated_states_list.json", SerializationFormat::JSON);
}