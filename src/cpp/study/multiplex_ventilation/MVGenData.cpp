/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Generate a set of patients stabilized on a ventilator
/// 1 ventilator for each patient
//--------------------------------------------------------------------------------------------------
bool MVController::GenerateStabilizedPatient(pulse::study::multiplex_ventilation::bind::PatientStateData& pData)
{
  // Construct our engine name
  std::string baseName = "comp="  + to_scientific_notation(pData.compliance_ml_per_cmh2o()) +
                         "_peep=" + to_scientific_notation(pData.peep_cmh2o()) +
                         "_pip="  + to_scientific_notation(pData.pip_cmh2o()) +
                         "_imp="  + to_scientific_notation(pData.impairmentfraction());

  Info("Creating engine " + baseName);
  auto engine = CreatePulseEngine(Dir::Solo + "/log/" + baseName + ".log");
  engine->SerializeFromFile("./states/StandardMale@0s.json", SerializationFormat::JSON);
  TrackData(*engine->GetEngineTracker(), Dir::Solo + "/csv/" + baseName + ".csv");
  engine->GetLogger()->LogToConsole(GetLogger()->IsLoggingToConsole());

  SESubstance* Oxygen = engine->GetSubstanceManager().GetSubstance("Oxygen");

  SEDyspnea dyspnea;
  dyspnea.GetSeverity().SetValue(1.0);

  SEIntubation intubation;
  intubation.SetType(eIntubation_Type::Tracheal);

  SEOverrides overrides;
  overrides.AddScalarProperty("RespiratoryResistance", pData.resistance_cmh2o_s_per_l(), PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  overrides.AddScalarProperty("RespiratoryCompliance", pData.compliance_ml_per_cmh2o(), VolumePerPressureUnit::mL_Per_cmH2O);

  SEImpairedAlveolarExchangeExacerbation impairedAlveolarExchange;
  impairedAlveolarExchange.GetSeverity().SetValue(pData.impairmentfraction());

  SEPulmonaryShuntExacerbation pulmonaryShunt;
  pulmonaryShunt.GetSeverity().SetValue(pData.impairmentfraction());

  SEMechanicalVentilatorConfiguration mvc(engine->GetSubstanceManager());
  auto& mv = mvc.GetConfiguration();
  mv.SetConnection(eMechanicalVentilator_Connection::Tube);
  mv.SetControl(eMechanicalVentilator_Control::PC_CMV);
  mv.SetDriverWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.GetRespiratoryRate().SetValue(pData.respirationrate_per_min(), FrequencyUnit::Per_min);
  mv.GetInspiratoryExpiratoryRatio().SetValue(pData.ieratio());
  mv.GetPeakInspiratoryPressure().SetValue(pData.pip_cmh2o(), PressureUnit::cmH2O);
  mv.GetPositiveEndExpiredPressure().SetValue(pData.peep_cmh2o(), PressureUnit::cmH2O);
  // Start at Atmospheric FiO2
  SESubstanceFraction* FiO2 = &mv.GetFractionInspiredGas(*Oxygen);
  FiO2->GetFractionAmount().SetValue(0.21);

  // Add our initial actions
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
  engine->GetLogger()->Info("Starting stabilization at " + to_scientific_notation(previousSpO2) + " SpO2");

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

    engine->GetLogger()->Info("Setting FiO2 to " + to_scientific_notation(currentFiO2) + " with an SpO2 of " + to_scientific_notation(currentSpO2));
    FiO2->GetFractionAmount().SetValue(currentFiO2);
    engine->ProcessAction(mvc);
    previousSpO2 = currentSpO2;
    previousFiO2 = currentFiO2;
  }
  engine->GetLogger()->Info("Engine stabilized at an SpO2 of " + to_scientific_notation(currentSpO2)+"; FiO2 of "+to_scientific_notation(currentFiO2));

  // Save our state
  baseName += "_FiO2="+to_scientific_notation(currentFiO2);
  engine->GetLogger()->Info("Saving engine state" + baseName+".json");
  engine->SerializeToFile(Dir::Solo+baseName+".json", SerializationFormat::JSON);
  // Append to our "list" of generated states
  pData.set_statefile(Dir::Solo+baseName+".json");
  pData.set_fio2(FiO2->GetFractionAmount().GetValue());
  // Set Vitals
  pData.set_oxygensaturation(engine->GetBloodChemistrySystem()->GetOxygenSaturation());
  pData.set_tidalvolume_ml(engine->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL));
  pData.set_endtidalcarbondioxidepressure_cmh2o(engine->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::cmH2O));
  auto AortaO2 = engine->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetSubstanceQuantity(*engine->GetSubstanceManager().GetSubstance("Oxygen"));
  pData.set_arterialoxygenpartialpressure_mmhg(AortaO2->GetPartialPressure(PressureUnit::mmHg));
  pData.set_carricoindex_mmhg(engine->GetRespiratorySystem()->GetCarricoIndex(PressureUnit::mmHg));
  engine->GetLogger()->Info("#################################################################################################\n\n");
  delete engine.release();
  //RunSoloState(SoloDir+baseName+".json", SoloDir+"csv/ext/"+baseName+".json", 120);

  return true;
}