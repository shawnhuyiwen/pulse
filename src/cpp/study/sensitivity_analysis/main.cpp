/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "SARunner.h"

int main(int argc, char* argv[])
{
  try
  {
    std::string mode = ""; // Default is to run the manual else below
    if (argc <= 1)
    {
      // Adjust comments to run the mode you want
      //mode = "genData";
      //mode = "manual";
      mode = "single";
    }
    else
    {
      mode = argv[1];
      //mode = "genData";
    };
    // convert string to back to lower case
    std::for_each(mode.begin(), mode.end(), [](char& c) { c = ::tolower(c); });
    if (mode == "single")
    {
      SARunner sar("./test_results/sesitivity_analysis/SensitivityAnalysisRunner.log");
      pulse::study::sensitivity_analysis::bind::SimulationListData simList;
      simList.set_outputrootdir("./test_results/sesitivity_analysis/");
      auto sim = simList.add_simulation();
      sim->set_id(42);
      sim->set_outputbasefilename("single");
      // Add some overrides
      auto c = sim->mutable_overrides()->add_scalaroverride();
      c->set_name("Compliance");
      c->set_value(0.5);
      c->set_unit(VolumePerPressureUnit::mL_Per_mmHg.GetString());

      auto r = sim->mutable_overrides()->add_scalaroverride();
      r->set_name("Resistance");
      r->set_value(0.5);
      r->set_unit(PressureTimePerVolumeUnit::mmHg_s_Per_mL.GetString());

      sar.Run(simList,"_simlist.json");
    }
    else if (mode == "gensimlist")
    {
      SARunner sar("./test_results/sesitivity_analysis/SensitivityAnalysisRunner.log");
      pulse::study::sensitivity_analysis::bind::SimulationListData simList;
      simList.set_outputrootdir("./test_results/sesitivity_analysis");

      // TODO Lots of loops adding runs to our simList

      std::cout << "Generated " << simList.simulation_size() << " simulations" << std::endl;
      return !sar.Run(simList, "simlist_results.json");
    }
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
