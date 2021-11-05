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

    // TODO
    // Import target data from disk
    // TODO import targets for the particular circuit we are optimizing
    // Currently CircuitOptimizer only runs the CV circuit, so only those targets should be used

    SEDataRequestManager drMgr(&log);
    std::vector<SEValidationTarget> targets;
    SEValidationTarget aortaVolume(&log);
    aortaVolume.SetType(eValidationTargetType::Mean);
    aortaVolume.SetRangeMin(283.7);
    aortaVolume.SetRangeMax(283.7);
    aortaVolume.SetDataRequest(
      &drMgr.CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "Volume", VolumeUnit::mL));
    targets.push_back(aortaVolume);
    SEValidationTarget aortaFlow(&log);
    aortaFlow.SetType(eValidationTargetType::Mean);
    aortaFlow.SetRangeMin(94.7);
    aortaFlow.SetRangeMax(94.7);
    aortaFlow.SetDataRequest(
      &drMgr.CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "InFlow", VolumePerTimeUnit::mL_Per_s));
    targets.push_back(aortaFlow);

    CircuitOptimizer opt(&log);
    if(!opt.ConvergeToHemodynamicsTargets(1000, targets))
      log.Error("Unable to converge to optimum circuit");
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 1;
}
