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

    std::string filename = "./optimizer/HemodynamicsTargets.json";
    // TODO import targets for the particular circuit we are optimizing
    // Currently CircuitOptimizer only runs the CV circuit, so only those targets should be used

    SEDataRequestManager drMgr(&log);
    if (!drMgr.SerializeFromFile(filename))
    {
      log.Error("Unable to read file " + filename);
      return 1;
    }

    CircuitOptimizer opt(&log);
    if(!opt.ConvergeToHemodynamicsTargets(1000, drMgr.GetValidationTargets()))
      log.Error("Unable to converge to optimum circuit");
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 1;
}
