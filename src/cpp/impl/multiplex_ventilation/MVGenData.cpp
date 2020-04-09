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
  DeleteDirectory(m_BaseFileName+"/solo_states", true);
  DeleteDirectory(m_BaseFileName+"/solo_states_logs", true);
  // Loop parameters
  float minCompliance_L_Per_cmH2O = 0.010f;
  float maxCompliance_L_Per_cmH2O = 0.050f;
  float stepCompliance_L_Per_cmH2O = 0.005f;
  int minPEEP_cmH2O = 10;
  int maxPEEP_cmH2O = 20;
  int stepPEEP_cmH2O = 2;
  float minImpairment = 0.0f;
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
      double inspiratoryPeriod_s = m_IERatio * breathPeriod_s / (1 + m_IERatio);
      double targetTidalVolume_L = targetTidalVolume_mL / 1000.0;
      int PIP_cmH2O = int(targetTidalVolume_L / (compliance_L_Per_cmH2O * (1.0 - exp(-inspiratoryPeriod_s / (m_Resistance_cmH2O_s_Per_L * compliance_L_Per_cmH2O)))) + PEEP_cmH2O);

      // Set up the ventilator
      mv.GetPeakInspiratoryPressure().SetValue(PIP_cmH2O, PressureUnit::cmH2O);
      mv.GetPositiveEndExpiredPressure().SetValue(PEEP_cmH2O, PressureUnit::cmH2O);
      FiO2->GetFractionAmount().SetValue(m_AmbientFiO2); // Set our FiO2 to Atmosphere

      Info("Calculated PIP of " + std::to_string(PIP_cmH2O) + "(cmH2O) for a tidal volume of " + to_scientific_notation(targetTidalVolume_mL) + "(mL) and a PEEP of " + std::to_string(PEEP_cmH2O) + "(cmH2O)");

      double currentFiO2 = FiO2->GetFractionAmount().GetValue();
      for (float currentImpairment = minImpairment; currentImpairment <= 1.0; currentImpairment += stepImpairment) //Aaron - This needs to be maxImpairment instead of 1.0, but rounding screws it up
      {
        Info("\n########################### PATIENT " + std::to_string(currentIteration + 1) + " OF " + std::to_string(totalIterations) + " ###########################");

        // Construct our engine
        baseName = "comp="+ to_scientific_notation(compliance_L_Per_cmH2O)+"_peep="+std::to_string(PEEP_cmH2O)+"_pip="+std::to_string(PIP_cmH2O)+"_imp="+to_scientific_notation(currentImpairment);
        Info("Creating engine " + baseName);
        auto fio2_stepper = CreatePulseEngine(m_BaseFileName + "/solo_states_logs/"+baseName+".log");
        fio2_stepper->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);

        // Add our initial actions
        impairedAlveolarExchange.GetSeverity().SetValue(currentImpairment);
        pulmonaryShunt.GetSeverity().SetValue(currentImpairment);
        fio2_stepper->ProcessAction(dyspnea);
        fio2_stepper->ProcessAction(intubation);
        fio2_stepper->ProcessAction(overrides);
        fio2_stepper->ProcessAction(mvc);
        fio2_stepper->ProcessAction(impairedAlveolarExchange);
        fio2_stepper->ProcessAction(pulmonaryShunt);

        double currentFiO2 = FiO2->GetFractionAmount().GetValue();

        double tolerance_Per_s = 0.001;
        double FiO2_increment = 0.01;

        double previousSpO2 = fio2_stepper->GetBloodChemistrySystem()->GetOxygenSaturation();

        fio2_stepper->AdvanceModelTime(breathPeriod_s, TimeUnit::s);
        double totalSimTime = breathPeriod_s;
        double currentSpO2 = fio2_stepper->GetBloodChemistrySystem()->GetOxygenSaturation();
        bool trendingUp = currentSpO2 > previousSpO2;

        bool max = false;
        double previousFiO2 = currentFiO2;
        while (true)
        {
          fio2_stepper->AdvanceModelTime(breathPeriod_s, TimeUnit::s);
          totalSimTime += breathPeriod_s;
          currentSpO2 = fio2_stepper->GetBloodChemistrySystem()->GetOxygenSaturation();

          if (!trendingUp && currentSpO2 < m_SpO2Target && !max)
          {
            if (currentSpO2 < m_SpO2Target - 0.1) //if it's 10% lower than the target, we can move faster
            {
              currentFiO2 += FiO2_increment;// *5.0;
            }
            else
            {
              currentFiO2 += FiO2_increment;
            }
          }
          else if ((trendingUp && currentSpO2 <= previousSpO2) ||
            (abs(currentSpO2 - previousSpO2) < tolerance_Per_s * breathPeriod_s))
          {
            if (currentSpO2 >= m_SpO2Target || max)
            {
              break;
            }
            else
            {
              currentFiO2 += FiO2_increment;
            }
          }

          if (currentFiO2 != previousFiO2)
          {
            if (currentFiO2 > 0.9995)
            {
              currentFiO2 = 0.9995;
              max = true;
            }
            Info("Setting FiO2 to " + to_scientific_notation(currentFiO2) + " with an SpO2 of " + to_scientific_notation(currentSpO2));
            FiO2->GetFractionAmount().SetValue(currentFiO2);
            fio2_stepper->ProcessAction(mvc);
          }

          trendingUp = currentSpO2 > previousSpO2;
          previousSpO2 = currentSpO2;
          previousFiO2 = currentFiO2;
        }

        Info("Engine stabilized at an SpO2 of " + to_scientific_notation(currentSpO2) + " in " + to_scientific_notation(totalSimTime) + "(s)");

        // Save our state
        baseName += "_FiO2="+to_scientific_notation(currentFiO2);
        Info("Saving engine state" + baseName+".json");
        fio2_stepper->SerializeToFile(m_BaseFileName+"/solo_states/"+baseName+".json", SerializationFormat::JSON);
        // Append to our "list" of generated states
        auto patientData = patients.add_patients();
        patientData->set_statefile(m_BaseFileName+"/solo_states/"+baseName+".json");
        patientData->set_compliance_l_per_cmh2o(compliance_L_Per_cmH2O);
        patientData->set_impairmentfraction(currentImpairment);
        patientData->set_peep_cmh2o(PEEP_cmH2O);
        patientData->set_pip_cmh2o(mv.GetPeakInspiratoryPressure().GetValue(PressureUnit::cmH2O));
        patientData->set_fio2(FiO2->GetFractionAmount().GetValue());
        patientData->set_oxygensaturation(fio2_stepper->GetBloodChemistrySystem()->GetOxygenSaturation());
        patientData->set_tidalvolume_l(fio2_stepper->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::L));
        patientData->set_endtidalcarbondioxidepressure_cmh2o(fio2_stepper->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::cmH2O));
        patientData->set_carricoindex(fio2_stepper->GetRespiratorySystem()->GetCarricoIndex(PressureUnit::mmHg));
        delete fio2_stepper.release();

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
  return WriteFile(out, "mv_solo_ventilated_patients.json", SerializationFormat::JSON);
}