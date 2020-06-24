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

    HRunner hr("./test_results/hydrocephalus/HydrocephalusRunner.log");
    hr.GetLogger()->LogToConsole(true);
    pulse::study::bind::hydrocephalus::SimulationListData simList;
    simList.set_outputrootdir("./test_results/hydrocephalus/");

    if (mode == "single")
    {
      auto sim = simList.add_simulation();
      sim->set_id(42);
      sim->set_name("single");
      sim->set_cerebrospinalfluidabsorptionrate_ml_per_min(50);
      sim->set_cerebrospinalfluidproductionrate_ml_per_min(10);
      sim->set_intracranialspacecompliance_ml_per_mmhg(100);
      sim->set_intracranialspacevolumebaseline_ml(100);
      hr.Run(simList);
      return 0;
    }
    else
    {
      double complianceMin = 35;
      double complianceMax = 155;// 165;
      double complianceStep = 30;//15;

      double volumeMin = 70;
      double volumeMax = 150;
      double volumeStep = 20;// 15;

      double absorptionRateMin = 0.23;
      double absorptionRateMax = 0.53;// 0.52;
      double absorptionRateStep = 0.1;// 0.05;

      double productionRateMin = 0.23;
      double productionRateMax = 0.53;// 0.52;
      double productionRateStep = 0.1;// 0.05;
      size_t cnt = 0;
      //compliance loop
      for (double compliance=complianceMin; compliance < complianceMax; compliance += complianceStep)
      {
        //volume loop
        for (double volume=volumeMin; volume < volumeMax; volume += volumeStep)
        {
          //absorpotion loop
          for (double absorptionRate=absorptionRateMin; absorptionRate < absorptionRateMax; absorptionRate += absorptionRateStep)
          {
            //production loop
            for (double productionRate=productionRateMin; productionRate < productionRateMax; productionRate += productionRateStep)
            {
              auto sim = simList.add_simulation();
              sim->set_id(cnt++);
              sim->set_name("Preliminary Data");
              sim->set_intracranialspacecompliance_ml_per_mmhg(compliance);
              sim->set_intracranialspacevolumebaseline_ml(volume);
              sim->set_cerebrospinalfluidabsorptionrate_ml_per_min(absorptionRate);
              sim->set_cerebrospinalfluidproductionrate_ml_per_min(productionRate);
            }
          }
        }
      }
      hr.GetLogger()->Info("Running a simList with " + std::to_string(simList.simulation_size()) + " runs.");
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
