/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CircuitOptimizer.h"

#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/utils/TimingProfile.h"

#include "engine/common/controller/CircuitManager.h"

using namespace pulse::study::circuit_optimization;

int main(int argc, char* argv[])
{
  try
  {
    Logger log;
    log.SetLogFile("./test_results/circuit_optimization.log");
    log.LogToConsole(true);
    std::string targetFilename = "./optimizer/HemodynamicsTargets.json";
    std::string startModifierSet = ""; // default set or from previous run "./test_results/modifier_set_xxxx_xx_x.json";

    SEDataRequestManager drMgr(&log);
    if (!drMgr.SerializeFromFile(targetFilename))
    {
      log.Error("Unable to read file " + targetFilename);
      return 1;
    }

    // Optimization parameters
    double stepRatio = 0.1;
    size_t maxLoops = 10;

    // Manually choose modifiers
    std::vector<std::string> modifiers;
    modifiers.emplace_back(pulse::CardiovascularPath::Aorta3ToAorta1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToLeftArm1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToRightArm1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToBone1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToBrain1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToFat1);
    modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToLargeIntestine1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToLeftLeg1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToRightLeg1);
    modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToLiver1);
    modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToMuscle1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToMyocardium1);
    // modifiers.emplace_back(pulse::CardiovascularPath::RightPulmonaryArteriesToRightPulmonaryCapillaries);
    // modifiers.emplace_back(pulse::CardiovascularPath::RightPulmonaryCapillariesToRightPulmonaryVeins);
    // modifiers.emplace_back(pulse::CardiovascularPath::LeftPulmonaryArteriesToLeftPulmonaryCapillaries);
    // modifiers.emplace_back(pulse::CardiovascularPath::LeftPulmonaryCapillariesToLeftPulmonaryVeins);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToSkin1);
    modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToSmallIntestine1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToSplanchnic1);
    // modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToSpleen1);

    CircuitOptimizer opt(&log);
    if(!opt.ConvergeToHemodynamicsTargets(maxLoops, stepRatio, startModifierSet, drMgr.GetValidationTargets(), modifiers))
      log.Error("Unable to converge to optimum circuit");
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 1;
}
