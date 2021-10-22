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
          converged = false;
      }
      if (converged)
      {
        // We have converged, save out cfg
        log.Info("We have successfully tuned the circuit!");
        return 0;
      }
      // Compute new modifiers and generate again
      // TODO - Ye Han
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
