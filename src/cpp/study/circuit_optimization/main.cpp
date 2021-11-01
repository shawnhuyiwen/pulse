/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CircuitOptimizer.h"

#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/utils/TimingProfile.h"

using namespace pulse::study::circuit_optimization;

int main(int argc, char* argv[])
{
  try
  {
    Logger log;
    log.SetLogFile("./test_results/circuit_optimization.log");
    CircuitOptimizer opt(&log);

    // Target data will eventually be read in from disk
    SEDataRequestManager drMgr(&log);
    std::vector<SEValidationTarget> data;
    SEValidationTarget aortaVolume(&log);
    aortaVolume.SetType(SEValidationTarget::Type::Mean);
    aortaVolume.SetRangeMin(283.7);
    aortaVolume.SetRangeMax(283.7);
    aortaVolume.SetDataRequest(
      &drMgr.CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "Volume", VolumeUnit::mL));
    data.push_back(aortaVolume);
    SEValidationTarget aortaFlow(&log);
    aortaFlow.SetType(SEValidationTarget::Type::Mean);
    aortaFlow.SetRangeMin(94.7);
    aortaFlow.SetRangeMax(94.7);
    aortaFlow.SetDataRequest(
      &drMgr.CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "InFlow", VolumePerTimeUnit::mL_Per_s));
    data.push_back(aortaFlow);

    // Start with the default modifiers
    PulseConfiguration cfg(&log);
    std::map<std::string, double> currentModifiers;
    // Cardio
    currentModifiers[pulse::VascularCompartment::Aorta] = 1.12;
    currentModifiers[pulse::VascularCompartment::LeftArm] = 1.15;
    currentModifiers[pulse::VascularCompartment::RightArm] = 1;
    currentModifiers[pulse::VascularCompartment::Bone] = 1.02;
    currentModifiers[pulse::VascularCompartment::Brain] = 1.0;
    currentModifiers[pulse::VascularCompartment::Fat] = 1.02;
//    currentModifiers[pulse::VascularCompartment::LeftHeart] = 0.0008;
//    currentModifiers[pulse::VascularCompartment::RightHeart] = 0.04225;
    currentModifiers[pulse::VascularCompartment::Kidneys] = 1.5;
    currentModifiers[pulse::VascularCompartment::LargeIntestine] = 1.05;
    currentModifiers[pulse::VascularCompartment::LeftLeg] = 1.1;
    currentModifiers[pulse::VascularCompartment::RightLeg] = 1.00;
    currentModifiers[pulse::VascularCompartment::Liver] = 1.1;
    currentModifiers[pulse::VascularCompartment::Muscle] = 1.15;
    currentModifiers[pulse::VascularCompartment::Myocardium] = 0.95;
    currentModifiers[pulse::VascularCompartment::RightPulmonaryArteries] = 1.0;
    currentModifiers[pulse::VascularCompartment::RightPulmonaryVeins] = 1.0;
    currentModifiers[pulse::VascularCompartment::LeftPulmonaryArteries] = 1.0;
    currentModifiers[pulse::VascularCompartment::LeftPulmonaryCapillaries] = 1.0;
    currentModifiers[pulse::VascularCompartment::LeftPulmonaryVeins] = 1.0;
    currentModifiers[pulse::VascularCompartment::Skin] = 1.0;
    currentModifiers[pulse::VascularCompartment::SmallIntestine] = 1.14;
    currentModifiers[pulse::VascularCompartment::Splanchnic] = 0.95;
    currentModifiers[pulse::VascularCompartment::Spleen] = 0.95;

//    // Compliance modifiers
//    double venaCavaComplianceTuning = 0.9;
//    double InitialComplianceHeartRight = 1.0 / 0.0243;
//    double InitialComplianceHeartLeft = 1.0 / 0.049;

//    // Volume modifiers
//    double VolumeModifierAorta = 1.16722 * 1.018749, VolumeModifierBrain = 0.998011 * 1.038409, VolumeModifierBone = 1.175574 * 0.985629, VolumeModifierFat = 1.175573 * 0.986527;
//    double VolumeModifierLargeIntestine = 1.17528 * 0.985609, VolumeModifierArmL = 1.175573 * 0.986529, VolumeModifierKidneyL = 0.737649 * 0.954339, VolumeModifierLegL = 1.175573 * 0.986529;
//    double VolumeModifierPulmArtL = 0.855566 * 1.095697, VolumeModifierPulmCapL = 0.724704 * 1.079139, VolumeModifierPulmVeinL = 0.548452 * 1.056844 * 1.062, VolumeModifierLiver = 1.157475 * 0.991848;
//    double VolumeModifierMuscle = 1.175573 * 0.986529, VolumeModifierMyocard = 1.175564 * 0.986531, VolumeModifierArmR = 1.175573 * 0.986529, VolumeModifierKidneyR = 0.737649 * 0.954339;
//    double VolumeModifierLegR = 1.175573 * 0.986529, VolumeModifierPulmArtR = 0.756158 * 1.121167, VolumeModifierPulmCapR = 0.602545 * 1.118213, VolumeModifierPulmVeinR = 0.395656 * 1.11424 * 1.11;
//    double VolumeModifierSkin = 1.007306 * 1.035695, VolumeModifierSmallIntestine = 1.17528 * 0.986509, VolumeModifierSplanchnic = 1.17528 * 0.986509, VolumeModifierSpleen = 1.17528 * 0.986509;
//    double VolumeModifierVenaCava = 0.66932 * 1.134447;

    for (auto const & [key, val] : currentModifiers) {
      cfg.GetModifiers()[key] = val;
    }

    // Loop
    bool converged;
    for(unsigned int i=0; i<1; i++)// Maximum loops
    {
      // 1. Generate data with these modifiers
      if (!opt.GenerateData(cfg, data))
      {
        log.Fatal("Error generating data");
        break;
      }
      // 2. Look at the error of each target
      //    (I am just making up convergence criteria)
      converged = true;
      for (SEValidationTarget& vt : data)
      {
        if (vt.GetError() > 10.0)// Just a guess here...
        {
          converged = false;
          break;
        }
      }
      if (converged)
      {
        // We have converged, save out cfg
        log.Info("We have successfully tuned the circuit!");
        return 0;
      }
      // Compute new modifiers and generate again
      // TODO - Ye Han
      std::map<std::string, double> newModifiers = opt.ComputeNewModifiers(cfg, data, currentModifiers);
      for (auto const & [key, val] : newModifiers)
      {
        currentModifiers[key] = val;
        cfg.GetModifiers()[key] = val;
      }
    }
    log.Error("Unable to converge to optimum circuit");
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 1;
}
