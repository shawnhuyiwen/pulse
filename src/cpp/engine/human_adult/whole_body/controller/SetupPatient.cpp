/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/controller/Controller.h"

#include "cdm/patient/SEPatient.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarArea.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarHeatCapacitance.h"
#include "cdm/properties/SEScalarHeatCapacitancePerMass.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"

namespace pulse { namespace human_adult_whole_body
{
  bool Controller::SetupPatient(SEPatient const& patient)
  {
    m_InitialPatient->Copy(patient);
    m_CurrentPatient->Clear();
    bool b = pulse::human_adult_whole_body::SetupPatient(*m_InitialPatient);
    // Copy the initial patient to the current patient
    m_CurrentPatient->Copy(*m_InitialPatient);

    return b;
  }
  bool SetupPatient(SEPatient& patient)
  {
    bool err = false;
    std::stringstream ss;

    //Sex is the only thing we absolutely need to be defined, the CDM assumes male if not provided

    //AGE ---------------------------------------------------------------
    double age_yr;
    double ageMin_yr = 18.0;
    double ageMax_yr = 65.0;
    double ageStandard_yr = 44.0;
    if (!patient.HasAge())
    {
      patient.GetAge().SetValue(ageStandard_yr, TimeUnit::yr);
      ss << "No patient age set. Using the standard value of " << ageStandard_yr << " years.";
      patient.Info(ss);
    }
    age_yr = patient.GetAge().GetValue(TimeUnit::yr);
    if (age_yr < ageMin_yr)
    {
      ss << "Patient age of " << age_yr << " years is too young. We do not model pediatrics. Minimum age allowed is " << ageMin_yr << " years.";
      patient.Error(ss);
      err = true;
    }
    else if (age_yr > ageMax_yr)
    {
      ss << "Patient age of " << age_yr << " years is too old. We do not model geriatrics. Maximum age allowed is " << ageMax_yr << " years.";
      patient.Error(ss);
      err = true;
    }

    //HEIGHT ---------------------------------------------------------------
    //From CDC values for 20 year olds
    //Mins are 3rd percentile, Maxs are 97th percentile, and standard is 50th percentile
    /// \cite Centers2016clinical
    double heightMinMale_cm = 163.0;
    double heightMaxMale_cm = 190.0;
    double heightStandardMale_cm = 177.0;
    double heightMinFemale_cm = 151.0;
    double heightMaxFemale_cm = 175.5;
    double heightStandardFemale_cm = 163.0;
    //Male
    double heightMin_cm = heightMinMale_cm;
    double heightMax_cm = heightMaxMale_cm;
    double heightStandard_cm = heightStandardMale_cm;
    if (patient.GetSex() == ePatient_Sex::Female)
    {
      //Female
      heightMin_cm = heightMinFemale_cm;
      heightMax_cm = heightMaxFemale_cm;
      heightStandard_cm = heightStandardFemale_cm;
    }
    if (!patient.HasHeight())
    {
      patient.GetHeight().SetValue(heightStandard_cm, LengthUnit::cm);
      ss << "No patient height set. Using the standard value of " << heightStandard_cm << " cm.";
      patient.Info(ss);
    }
    double height_cm = patient.GetHeight().GetValue(LengthUnit::cm);
    double height_ft = Convert(height_cm, LengthUnit::cm, LengthUnit::ft);
    //Check for outrageous values
    if (height_ft < 4.5 || height_ft > 7.0)
    {
      patient.Error("Patient height setting is outrageous. It must be between 4.5 and 7.0 ft");
      err = true;
    }
    if (height_cm < heightMin_cm)
    {
      ss << "Patient height of " << height_cm << " cm is outside of typical ranges - below 3rd percentile (" << heightMax_cm << " cm). No guarantees of model validity.";
      patient.Warning(ss);
    }
    else if (height_cm > heightMax_cm)
    {
      ss << "Patient height of " << height_cm << " cm is outside of typical ranges - above 97th percentile(" << heightMin_cm << " cm). No guarantees of model validity.";
      patient.Warning(ss);
    }

    //IDEAL BODY WEIGHT ----------------------------------------------------
    /// \cite green2017green
    //page 295
    //Devine Formula
    if (patient.HasIdealBodyWeight())
      patient.Warning("Ignorning provided patient ideal body weight. It is determined by weight and body fat fraction.");
    double height_in = patient.GetHeight().GetValue(LengthUnit::in);
    double idealWeight_kg = 0.0;
    if (patient.GetSex() == ePatient_Sex::Female)
    {
      //Female
      idealWeight_kg = 45.5 + 2.3 * (height_in - 60.0);
    }
    else
    {
      //Male
      idealWeight_kg = 50.0 + 2.3 * (height_in - 60.0);
    }
    patient.GetIdealBodyWeight().SetValue(idealWeight_kg, MassUnit::kg);
    ss << "Patient ideal body weight computed and set to " << idealWeight_kg << " kg.";
    patient.Info(ss);

    //WEIGHT ---------------------------------------------------------------
    /// \cite World2006bmi
    double weight_kg;
    double BMI_kg_per_m2;
    double BMIStandard_kg_per_m2 = 21.75;
    double BMIObese_kg_per_m2 = 30.0;
    double BMIOverweight_kg_per_m2 = 25.0;
    double BMIUnderweight_kg_per_m2 = 18.5;
    double BMISeverelyUnderweight_kg_per_m2 = 16.0;
    if (!patient.HasWeight())
    {
      weight_kg = BMIStandard_kg_per_m2 * pow(patient.GetHeight().GetValue(LengthUnit::m), 2);
      patient.GetWeight().SetValue(weight_kg, MassUnit::kg);
      ss << "No patient weight set. Using the standard BMI value of 21.75 kg/m^2, resulting in a weight of " << weight_kg << " kg.";
      patient.Info(ss);
    }
    weight_kg = patient.GetWeight(MassUnit::kg);
    BMI_kg_per_m2 = weight_kg / pow(patient.GetHeight().GetValue(LengthUnit::m), 2);
    if (BMI_kg_per_m2 > BMIObese_kg_per_m2)
    {
      ss << "Patient Body Mass Index (BMI) of " << BMI_kg_per_m2 << "  kg/m^2 is too high. Obese patients must be modeled by adding/using a condition. Maximum BMI allowed is " << BMIObese_kg_per_m2 << " kg/m^2.";
      patient.Error(ss);
      err = true;
    }
    if (BMI_kg_per_m2 > BMIOverweight_kg_per_m2)
    {
      ss << "Patient Body Mass Index (BMI) of " << BMI_kg_per_m2 << " kg/m^2 is overweight. No guarantees of model validity.";
      patient.Warning(ss);
    }
    if (BMI_kg_per_m2 < BMIUnderweight_kg_per_m2)
    {
      ss << "Patient Body Mass Index (BMI) of " << BMI_kg_per_m2 << " kg/m^2 is underweight. No guarantees of model validity.";
      patient.Warning(ss);
    }
    if (BMI_kg_per_m2 < BMISeverelyUnderweight_kg_per_m2)
    {
      ss << "Patient Body Mass Index (BMI) of " << BMI_kg_per_m2 << " kg/m^2 is too low. Severly underweight patients must be modeled by adding/using a condition. Maximum BMI allowed is " << BMISeverelyUnderweight_kg_per_m2 << " kg/m^2.";
      patient.Error(ss);
      err = true;
    }

    //BODY FAT FRACTION ---------------------------------------------------------------
    //From American Council on Exercise
    /// \cite muth2009what
    double fatFraction;
    double fatFractionStandardMale = 0.21;
    double fatFractionStandardFemale = 0.28;
    double fatFractionMaxMale = 0.25; //Obese
    double fatFractionMaxFemale = 0.32; //Obese
    double fatFractionMinMale = 0.02; //Essential fat
    double fatFractionMinFemale = 0.10; //Essential fat
    //Male
    double fatFractionMin = fatFractionMinMale;
    double fatFractionMax = fatFractionMaxMale;
    double fatFractionStandard = fatFractionStandardMale;
    if (patient.GetSex() == ePatient_Sex::Female)
    {
      //Female
      fatFractionMin = fatFractionMinFemale;
      fatFractionMax = fatFractionMaxFemale;
      fatFractionStandard = fatFractionStandardFemale;
    }

    if (!patient.HasBodyFatFraction())
    {
      fatFraction = fatFractionStandard;
      patient.GetBodyFatFraction().SetValue(fatFraction);
      ss << "No patient body fat fraction set. Using the standard value of " << fatFraction << ".";
      patient.Info(ss);
    }
    fatFraction = patient.GetBodyFatFraction().GetValue();
    if (fatFraction > fatFractionMax)
    {
      ss << "Patient body fat fraction of " << fatFraction << " is too high. Obese patients must be modeled by adding/using a condition. Maximum body fat fraction allowed is " << fatFractionMax << ".";
      patient.Error(ss);
      err = true;
    }
    else if (fatFraction < fatFractionMin)
    {
      ss << "Patient body fat fraction  " << fatFraction << " is too low. Patients must have essential fat. Minimum body fat fraction allowed is " << fatFractionMin << ".";
      patient.Error(ss);
      err = true;
    }

    //Lean Body Mass ---------------------------------------------------------------
    if (patient.HasLeanBodyMass())
    {
      ss << "Patient lean body mass cannot be set. It is determined by weight and body fat fraction.";
      patient.Error(ss);
      err = true;
    }
    double leanBodyMass_kg = weight_kg * (1.0 - fatFraction);
    patient.GetLeanBodyMass().SetValue(leanBodyMass_kg, MassUnit::kg);
    ss << "Patient lean body mass computed and set to " << leanBodyMass_kg << " kg.";
    patient.Info(ss);

    //Body Density ---------------------------------------------------------------
    if (patient.HasBodyDensity())
    {
      ss << "Patient body density cannot be set. It is determined using body fat fraction.";
      patient.Error(ss);
      err = true;
    }
    //Using the average of Siri and Brozek formulas
    /// \cite siri1961body 
    /// \cite brovzek1963densitometric
    double SiriBodyDensity_g_Per_cm3 = 4.95 / (fatFraction + 4.50);
    double BrozekBodyDensity_g_Per_cm3 = 4.57 / (fatFraction + 4.142);
    double bodyDensity_g_Per_cm3 = (SiriBodyDensity_g_Per_cm3 + BrozekBodyDensity_g_Per_cm3) / 2.0;
    patient.GetBodyDensity().SetValue(bodyDensity_g_Per_cm3, MassPerVolumeUnit::g_Per_cm3);
    ss << "Patient body density computed and set to " << bodyDensity_g_Per_cm3 << " g/cm^3.";
    patient.Info(ss);

    //Heart Rate ---------------------------------------------------------------
    double heartRate_bpm;
    double heartStandard_bpm = 72.0;
    double heartRateMax_bpm = 100.0;
    double heartRateTachycardia_bpm = 110;
    double heartRateMin_bpm = 60.0;
    double heartRateBradycardia_bpm = 50;
    if (!patient.HasHeartRateBaseline())
    {
      heartRate_bpm = heartStandard_bpm;
      patient.GetHeartRateBaseline().SetValue(heartRate_bpm, FrequencyUnit::Per_min);
      ss << "No patient heart rate baseline set. Using the standard value of " << heartRate_bpm << " bpm.";
      patient.Info(ss);
    }
    heartRate_bpm = patient.GetHeartRateBaseline(FrequencyUnit::Per_min);
    if (heartRate_bpm > heartRateMax_bpm)
    {
      if (heartRate_bpm <= heartRateTachycardia_bpm)
      {
        ss << "Patient heart rate baseline of " << heartRate_bpm << " bpm is tachycardic. Tachycardia heart rate  is [" << heartRateMax_bpm << "," << heartRateTachycardia_bpm << "] bpm.";
        patient.Info(ss);
      }
      else
      {
        ss << "Patient heart rate baseline of " << heartRate_bpm << " bpm is too high. Maximum heart rate baseline allowed is " << heartRateTachycardia_bpm << " bpm.";
        patient.Error(ss);
        err = true;
      }
    }
    else if (heartRate_bpm < heartRateMin_bpm)
    {
      if (heartRate_bpm <= heartRateTachycardia_bpm)
      {
        ss << "Patient heart rate baseline of " << heartRate_bpm << " bpm is bradycardic. Bradycardia heart rate  is [" << heartRateBradycardia_bpm << "," << heartRateMin_bpm << "] bpm.";
        patient.Info(ss);
      }
      else
      {
        ss << "Patient heart rate baseline of " << heartRate_bpm << " is too low. Minimum heart rate baseline allowed is " << heartRateBradycardia_bpm << " bpm.";
        patient.Error(ss);
        err = true;
      }
    }

    //Tanaka H, Monahan KD, Seals DR (January 2001). "Age-predicted maximal heart rate revisited". J. Am. Coll. Cardiol. 37(1): 153�6. doi:10.1016/S0735-1097(00)01054-8.PMID 11153730.
    double computedHeartRateMaximum_bpm = 208.0 - (0.7 * patient.GetAge(TimeUnit::yr));
    if (!patient.HasHeartRateMaximum())
    {
      patient.GetHeartRateMaximum().SetValue(computedHeartRateMaximum_bpm, FrequencyUnit::Per_min);
      ss << "No patient heart rate maximum set. Using a computed value of " << computedHeartRateMaximum_bpm << " bpm.";
      patient.Info(ss);
    }
    else
    {
      if (patient.GetHeartRateMaximum(FrequencyUnit::Per_min) < heartRate_bpm)
      {
        ss << "Patient heart rate maximum must be greater than the baseline heart rate.";
        patient.Error(ss);
        err = true;
      }
      ss << "Specified patient heart rate maximum of " << patient.GetHeartRateMaximum(FrequencyUnit::Per_min) << " bpm differs from computed value of " << computedHeartRateMaximum_bpm << " bpm. No guarantees of model validity.";
      patient.Warning(ss);
    }
    if (!patient.HasHeartRateMinimum())
    {
      patient.GetHeartRateMinimum().SetValue(0.001, FrequencyUnit::Per_min);
      ss << "No patient heart rate minimum set. Using a default value of " << 0.001 << " bpm.";
      patient.Info(ss);
    }
    if (patient.GetHeartRateMinimum(FrequencyUnit::Per_min) > heartRate_bpm)
    {
      ss << "Patient heart rate minimum must be less than the baseline heart rate.";
      patient.Error(ss);
      err = true;
    }

    //Arterial Pressures ---------------------------------------------------------------
    double systolic_mmHg;
    double diastolic_mmHg;
    double systolicStandard_mmHg = 114.0;
    double diastolicStandard_mmHg = 73.5;
    double systolicMax_mmHg = 120.0; //Hypertension
    double diastolicMax_mmHg = 80.0; //Hypertension
    double systolicMin_mmHg = 90.0; //Hypotension
    double diastolicMin_mmHg = 60.0; //Hypotension
    double narrowestPulseFactor = 0.75; //From Wikipedia: Pulse Pressure
    if (!patient.HasSystolicArterialPressureBaseline())
    {
      systolic_mmHg = systolicStandard_mmHg;
      patient.GetSystolicArterialPressureBaseline().SetValue(systolic_mmHg, PressureUnit::mmHg);
      ss << "No patient systolic pressure baseline set. Using the standard value of " << systolic_mmHg << " mmHg.";
      patient.Info(ss);
    }
    systolic_mmHg = patient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
    if (systolic_mmHg < systolicMin_mmHg)
    {
      ss << "Patient systolic pressure baseline of " << systolic_mmHg << " mmHg is too low. Hypotension must be modeled by adding/using a condition. Minimum systolic pressure baseline allowed is " << systolicMin_mmHg << " mmHg.";
      patient.Error(ss);
      err = true;
    }
    else if (systolic_mmHg > systolicMax_mmHg)
    {
      ss << "Patient systolic pressure baseline of " << systolic_mmHg << " mmHg is too high. Hypertension must be modeled by adding/using a condition. Maximum systolic pressure baseline allowed is " << systolicMax_mmHg << " mmHg.";
      patient.Error(ss);
      err = true;
    }

    if (!patient.HasDiastolicArterialPressureBaseline())
    {
      diastolic_mmHg = diastolicStandard_mmHg;
      patient.GetDiastolicArterialPressureBaseline().SetValue(diastolic_mmHg, PressureUnit::mmHg);
      ss << "No patient diastolic pressure baseline set. Using the standard value of " << diastolic_mmHg << " mmHg.";
      patient.Info(ss);
    }
    diastolic_mmHg = patient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
    if (diastolic_mmHg < diastolicMin_mmHg)
    {
      ss << "Patient diastolic pressure baseline of " << diastolic_mmHg << " mmHg is too low. Hypotension must be modeled by adding/using a condition. Minimum diastolic pressure baseline allowed is " << diastolicMin_mmHg << " mmHg.";
      patient.Error(ss);
      err = true;
    }
    else if (diastolic_mmHg > diastolicMax_mmHg)
    {
      ss << "Patient diastolic pressure baseline of " << diastolic_mmHg << " mmHg is too high. Hypertension must be modeled by adding/using a condition. Maximum diastolic pressure baseline allowed is " << diastolicMax_mmHg << " mmHg.";
      patient.Error(ss);
      err = true;
    }

    if (diastolic_mmHg > 0.75 * systolic_mmHg)
    {
      ss << "Patient baseline pulse pressure (systolic vs. diastolic pressure fraction) of " << diastolic_mmHg / systolic_mmHg << " is abnormally narrow. Minimum fraction allowed is " << narrowestPulseFactor << " .";
      patient.Error(ss);
      err = true;
    }

    if (patient.HasMeanArterialPressureBaseline())
    {
      ss << "Patient mean arterial pressure baseline cannot be set. It is determined through homeostatic simulation.";
      patient.Error(ss);
      err = true;
    }
    double MAP_mmHg = 1.0 / 3.0 * systolic_mmHg + 2.0 / 3.0 * diastolic_mmHg;
    patient.GetMeanArterialPressureBaseline().SetValue(MAP_mmHg, PressureUnit::mmHg);

    //Blood Volume ---------------------------------------------------------------
    /// \cite Morgan2006Clinical
    double bloodVolume_mL;
    double computedBloodVolume_mL = 65.6 * pow(weight_kg, 1.02);
    double bloodVolumeMin_mL = computedBloodVolume_mL * 0.85; //Stage 1 Hypovolemia
    double bloodVolumeMax_mL = computedBloodVolume_mL * 1.15; //Just go the same distance on the other side
    if (!patient.HasBloodVolumeBaseline())
    {
      bloodVolume_mL = computedBloodVolume_mL;
      patient.GetBloodVolumeBaseline().SetValue(bloodVolume_mL, VolumeUnit::mL);
      ss << "No patient blood volume baseline set. Using a computed value of " << computedBloodVolume_mL << " mL.";
      patient.Info(ss);
    }
    bloodVolume_mL = patient.GetBloodVolumeBaseline(VolumeUnit::mL);
    if (bloodVolume_mL != computedBloodVolume_mL)
    {
      ss << "Specified patient blood volume baseline of " << bloodVolume_mL << " mL differs from computed value of " << computedBloodVolume_mL << " mL. No guarantees of model validity and there is a good chance the patient will not reach a starting homeostatic point.";
      patient.Warning(ss);
    }
    if (bloodVolume_mL < bloodVolumeMin_mL)
    {
      ss << "Patient blood volume baseline of " << bloodVolume_mL << " mL is too low. Hypovolemia must be modeled by adding/using a condition. Minimum blood volume baseline allowed is " << bloodVolumeMin_mL << " mL.";
      patient.Error(ss);
      err = true;
    }
    else if (bloodVolume_mL > bloodVolumeMax_mL)
    {
      ss << "Patient blood volume baseline of " << bloodVolume_mL << " mL is too high. Excessive volume must be modeled by adding/using a condition. Maximum blood volume baseline allowed is " << bloodVolumeMax_mL << " mL.";
      patient.Error(ss);
      err = true;
    }

    //Respiration Rate ---------------------------------------------------------------
    //Note: This is overwritten after stabilization
    /// \cite green2017green
    double respirationRate_bpm;
    double respirationRateStandard_bpm = 12.0;
    double respirationRateMax_bpm = 20.0;
    double respirationRateMin_bpm = 8.0;
    if (!patient.HasRespirationRateBaseline())
    {
      respirationRate_bpm = respirationRateStandard_bpm;
      patient.GetRespirationRateBaseline().SetValue(respirationRate_bpm, FrequencyUnit::Per_min);
      ss << "No patient respiration rate baseline set. Using the standard value of " << respirationRate_bpm << " bpm.";
      patient.Info(ss);
    }
    respirationRate_bpm = patient.GetRespirationRateBaseline(FrequencyUnit::Per_min);
    if (respirationRate_bpm > respirationRateMax_bpm)
    {
      ss << "Patient respiration rate baseline of " << respirationRate_bpm << " bpm is too high. Non-healthy values must be modeled by adding/using a condition. Maximum respiration rate baseline allowed is " << respirationRateMax_bpm << " bpm.";
      patient.Error(ss);
      err = true;
    }
    else if (respirationRate_bpm < respirationRateMin_bpm)
    {
      ss << "Patient respiration rate baseline of " << respirationRate_bpm << " bpm is too low. Non-healthy values must be modeled by adding/using a condition. Minimum respiration rate baseline allowed is " << respirationRateMin_bpm << " bpm.";
      patient.Error(ss);
      err = true;
    }

    //Right Lung Ratio ---------------------------------------------------------------
    double rightLungRatio;
    double rightLungRatioStandard = 0.525;
    double rightLungRatioMax = 0.60;
    double rightLungRatioMin = 0.50;
    if (!patient.HasRightLungRatio())
    {
      rightLungRatio = rightLungRatioStandard;
      patient.GetRightLungRatio().SetValue(rightLungRatio);
      ss << "No patient right lung ratio set. Using the standard value of " << rightLungRatio << ".";
      patient.Info(ss);
    }
    rightLungRatio = patient.GetRightLungRatio().GetValue();
    if (rightLungRatio > rightLungRatioMax)
    {
      ss << "Patient right lung ratio of " << rightLungRatio << " is too high. Non-healthy values must be modeled by adding/using a condition. Maximum right lung ratio allowed is " << rightLungRatioMax << ".";
      patient.Error(ss);
      err = true;
    }
    else if (rightLungRatio < rightLungRatioMin)
    {
      ss << "Patient right lung ratio of " << rightLungRatio << " is too low. Non-healthy values must be modeled by adding/using a condition. Minimum right lung ratio allowed is " << rightLungRatioMin << ".";
      patient.Error(ss);
      err = true;
    }

    //Respiratory Volumes ---------------------------------------------------------------
    //These are based on weight
    /// \cite ganong1995review
    double totalLungCapacity_L;
    double computedTotalLungCapacity_L = 80.0 * idealWeight_kg / 1000.0;
    if (!patient.HasTotalLungCapacity())
    {
      totalLungCapacity_L = computedTotalLungCapacity_L;
      patient.GetTotalLungCapacity().SetValue(totalLungCapacity_L, VolumeUnit::L);
      ss << "No patient total lung capacity set. Using a computed value of " << computedTotalLungCapacity_L << " L.";
      patient.Info(ss);
    }
    totalLungCapacity_L = patient.GetTotalLungCapacity(VolumeUnit::L);
    if (totalLungCapacity_L != computedTotalLungCapacity_L)
    {
      ss << "Specified total lung capacity of " << totalLungCapacity_L << " L differs from computed value of " << computedTotalLungCapacity_L << " L. No guarantees of model validity.";
      patient.Warning(ss);
    }

    double functionalResidualCapacity_L;
    double computedFunctionalResidualCapacity_L = 30.0 * idealWeight_kg / 1000.0;
    if (!patient.HasFunctionalResidualCapacity())
    {
      functionalResidualCapacity_L = computedFunctionalResidualCapacity_L;
      patient.GetFunctionalResidualCapacity().SetValue(functionalResidualCapacity_L, VolumeUnit::L);
      ss << "No patient functional residual capacity set. Using a computed value of " << computedFunctionalResidualCapacity_L << " L.";
      patient.Info(ss);
    }
    functionalResidualCapacity_L = patient.GetFunctionalResidualCapacity(VolumeUnit::L);
    if (functionalResidualCapacity_L != computedFunctionalResidualCapacity_L)
    {
      ss << "Specified functional residual capacity of " << functionalResidualCapacity_L << " L differs from computed value of " << computedFunctionalResidualCapacity_L << " L. No guarantees of model validity.";
      patient.Warning(ss);
    }

    double residualVolume_L;
    double computRedesidualVolume_L = 16.0 * idealWeight_kg / 1000.0;
    if (!patient.HasResidualVolume())
    {
      residualVolume_L = computRedesidualVolume_L;
      patient.GetResidualVolume().SetValue(residualVolume_L, VolumeUnit::L);
      ss << "No patient residual volume set. Using a computed value of " << computRedesidualVolume_L << " L.";
      patient.Info(ss);
    }
    residualVolume_L = patient.GetResidualVolume(VolumeUnit::L);
    if (residualVolume_L != computRedesidualVolume_L)
    {
      ss << "Specified residual volume of " << residualVolume_L << " L differs from computed value of " << computRedesidualVolume_L << " L. No guarantees of model validity.";
      patient.Warning(ss);
    }

    if (patient.HasTidalVolumeBaseline())
    {
      ss << "Patient tidal volume baseline cannot be set. It is determined through homeostatic simulation.";
      patient.Error(ss);
      err = true;
    }
    if (patient.HasVitalCapacity())
    {
      ss << "Patient vital capacity cannot be set. It is directly computed via other lung volume patient parameters.";
      patient.Error(ss);
      err = true;
    }
    if (patient.HasExpiratoryReserveVolume())
    {
      ss << "Patient expiratory reserve volume cannot be set. It is directly computed via other lung volume patient parameters.";
      patient.Error(ss);
      err = true;
    }
    if (patient.HasInspiratoryReserveVolume())
    {
      ss << "Patient inspiratory reserve volume cannot be set. It is directly computed via other lung volume patient parameters.";
      patient.Error(ss);
      err = true;
    }
    if (patient.HasInspiratoryCapacity())
    {
      ss << "Patient inspiratory capacity cannot be set. It is directly computed via other lung volume patient parameters.";
      patient.Error(ss);
      err = true;
    }

    double tidalVolume_L = 37.0 * idealWeight_kg / 1000.0 - functionalResidualCapacity_L;
    double vitalCapacity = totalLungCapacity_L - residualVolume_L;
    double expiratoryReserveVolume = functionalResidualCapacity_L - residualVolume_L;
    double inspiratoryReserveVolume = totalLungCapacity_L - functionalResidualCapacity_L - tidalVolume_L;
    double inspiratoryCapacity = totalLungCapacity_L - functionalResidualCapacity_L;
    //No negative volumes
    if (totalLungCapacity_L < 0.0 || functionalResidualCapacity_L < 0.0 || residualVolume_L < 0.0 || tidalVolume_L < 0.0 ||
      vitalCapacity < 0.0 || expiratoryReserveVolume < 0.0 || inspiratoryReserveVolume < 0.0 || inspiratoryCapacity < 0.0)
    {
      ss << "All patient lung volumes must be positive.";
      patient.Error(ss);
      err = true;
    }
    patient.GetTidalVolumeBaseline().SetValue(tidalVolume_L, VolumeUnit::L); //This is overwritten after stabilization  
    ss << "Patient tidal volume computed and set to " << tidalVolume_L << " L.";
    patient.Info(ss);

    patient.GetVitalCapacity().SetValue(vitalCapacity, VolumeUnit::L);
    ss << "Patient vital capacity computed and set to " << vitalCapacity << " L.";
    patient.Info(ss);

    patient.GetExpiratoryReserveVolume().SetValue(expiratoryReserveVolume, VolumeUnit::L);
    ss << "Patient expiratory reserve volume computed and set to " << expiratoryReserveVolume << " L.";
    patient.Info(ss);

    patient.GetInspiratoryReserveVolume().SetValue(inspiratoryReserveVolume, VolumeUnit::L);
    ss << "Patient inspiratory reserve volume computed and set to " << inspiratoryReserveVolume << " L.";
    patient.Info(ss);

    patient.GetInspiratoryCapacity().SetValue(inspiratoryCapacity, VolumeUnit::L);
    ss << "Patient inspiratory capacity computed and set to " << inspiratoryCapacity << " L.";
    patient.Info(ss);

    //Alveoli Surface Area ---------------------------------------------------------------
    /// \cite roberts2000gaseous
    double standardAlveoliSurfaceArea_m2 = 70.0;
    double alveoliSurfaceArea_m2;
    //Scale the alveoli surface area based on the size of the patient�s lungs  
    /// \cite ganong1995review
    double standardTotalLungCapacity_L = 6.17; //This is the Total Lung Capacity of our standard patient  
    double computedAlveoliSurfaceArea_m2 = totalLungCapacity_L / standardTotalLungCapacity_L * standardAlveoliSurfaceArea_m2;
    if (!patient.HasAlveoliSurfaceArea())
    {
      alveoliSurfaceArea_m2 = computedAlveoliSurfaceArea_m2;
      patient.GetAlveoliSurfaceArea().SetValue(alveoliSurfaceArea_m2, AreaUnit::m2);
      ss << "No patient alveoli surface area set. Using a computed value of " << computedAlveoliSurfaceArea_m2 << " m^2.";
      patient.Info(ss);
    }
    alveoliSurfaceArea_m2 = patient.GetAlveoliSurfaceArea(AreaUnit::m2);
    if (alveoliSurfaceArea_m2 != computedAlveoliSurfaceArea_m2)
    {
      ss << "Specified alveoli surface area of " << alveoliSurfaceArea_m2 << " m^2 differs from computed value of " << computedAlveoliSurfaceArea_m2 << " m^2. No guarantees of model validity.";
      patient.Warning(ss);
    }

    //Skin Surface Area ---------------------------------------------------------------
    /// \cite du1989formula
    double skinSurfaceArea_m2;
    double computSkinSurfaceArea_m2 = 0.20247 * pow(weight_kg, 0.425) * pow(Convert(height_cm, LengthUnit::cm, LengthUnit::m), 0.725);
    if (!patient.HasSkinSurfaceArea())
    {
      skinSurfaceArea_m2 = computSkinSurfaceArea_m2;
      patient.GetSkinSurfaceArea().SetValue(skinSurfaceArea_m2, AreaUnit::m2);
      ss << "No patient skin surface area set. Using a computed value of " << computSkinSurfaceArea_m2 << " m^2.";
      patient.Info(ss);
    }
    skinSurfaceArea_m2 = patient.GetSkinSurfaceArea(AreaUnit::m2);
    if (skinSurfaceArea_m2 != computSkinSurfaceArea_m2)
    {
      ss << "Specified skin surface area of " << skinSurfaceArea_m2 << " cm differs from computed value of " << computSkinSurfaceArea_m2 << " cm. No guarantees of model validity.";
      patient.Warning(ss);
    }

    //Basal Metabolic Rate ---------------------------------------------------------------
    //The basal metabolic rate is determined from the Harris-Benedict formula, with differences dependent on sex, age, height and mass
    /// \cite roza1984metabolic
    double BMR_kcal_Per_day;
    double computBMR_kcal_Per_day = 88.632 + 13.397 * weight_kg + 4.799 * height_cm - 5.677 * age_yr; //Male
    if (patient.GetSex() == ePatient_Sex::Female)
    {
      computBMR_kcal_Per_day = 447.593 + 9.247 * weight_kg + 3.098 * height_cm - 4.330 * age_yr; //Female
    }
    if (!patient.HasBasalMetabolicRate())
    {
      BMR_kcal_Per_day = computBMR_kcal_Per_day;
      patient.GetBasalMetabolicRate().SetValue(BMR_kcal_Per_day, PowerUnit::kcal_Per_day);

      ss << "No patient basal metabolic rate set. Using a computed value of " << computBMR_kcal_Per_day << " kcal/day.";
      patient.Info(ss);
    }
    BMR_kcal_Per_day = patient.GetBasalMetabolicRate(PowerUnit::kcal_Per_day);
    if (BMR_kcal_Per_day != computBMR_kcal_Per_day)
    {
      ss << "Specified basal metabolic rate of " << BMR_kcal_Per_day << " kcal/day differs from computed value of " << computBMR_kcal_Per_day << " kcal/day. No guarantees of model validity.";
      patient.Warning(ss);
    }

    return !err;
  }
END_NAMESPACE_EX
