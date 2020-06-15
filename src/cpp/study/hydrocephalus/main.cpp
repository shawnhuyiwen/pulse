/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "HRunner.h"

int main(int argc, char* argv[])
{
  try
  {
    std::string mode = ""; // Default is to run the manual else below
    if (argc <= 1)
    {
      // Adjust comments to run the mode you want
      mode = "single";
    }
    else
    {
      HRunner hr("./test_results/hydrocephalus/HydrocephalusRunner.log");
      hr.Run(argv[1], SerializationFormat::JSON);
      return 0;
    };
    // convert string to back to lower case
    std::for_each(mode.begin(), mode.end(), [](char& c) { c = ::tolower(c); });
    if (mode == "single")
    {
      HRunner hr("./test_results/hydrocephalus/HydrocephalusRunner.log");
      pulse::study::bind::hydrocephalus::SimulationListData simList;
      simList.set_outputrootdir("./test_results/hydrocephalus/");

      auto sim = simList.add_simulation();
      sim->set_id(42);
      sim->set_name("single");
      sim->set_cerebrospinalfluidabsorptionrate_ml_per_min(50);
      sim->set_cerebrospinalfluidproductionrate_ml_per_min(10);
      sim->set_intracranialspacecompliance_ml_per_mmhg(70);
      sim->set_intracranialspacevolume_ml(150);
      hr.Run(simList);
      return 0;
    }
    std::cerr << "Unsupported mode : " << mode << std::endl;
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  return 1;
}
