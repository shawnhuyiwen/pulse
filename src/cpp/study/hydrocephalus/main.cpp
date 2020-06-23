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
    pulse::study::bind::hydrocephalus::SimulationListData simList;
    simList.set_outputrootdir("./test_results/hydrocephalus/");

    auto sim = simList.add_simulation();
    if (mode == "single")
    {
      sim->set_id(42);
      sim->set_name("single");
      sim->set_cerebrospinalfluidabsorptionrate_ml_per_min(50);
      sim->set_cerebrospinalfluidproductionrate_ml_per_min(10);
      sim->set_intracranialspacecompliance_ml_per_mmhg(100);
      sim->set_intracranialspacevolume_ml(200);
      hr.Run(simList);
      return 0;
    }
    else
    {
        //compliance loop
        for (double compliance=35.0; compliance<165.0; compliance+=15)
        {
            sim->set_intracranialspacecompliance_ml_per_mmhg(compliance);
            //volume loop
            for (double volume=70.0; volume<150.0; volume+=15)
            {
                sim->set_intracranialspacevolume_ml(volume);
                //absorpotion loop
                for (double absorptionRate=0.23; absorptionRate<0.52; absorptionRate+=0.05)
                {
                    sim->set_cerebrospinalfluidabsorptionrate_ml_per_min(absorptionRate);
                    //production loop
                    for (double productionRate=0.23; productionRate<0.52; productionRate+=0.05)
                    {
                        sim->set_cerebrospinalfluidproductionrate_ml_per_min(productionRate);
                    }
                }
            }
        }
        hr.Run(simList);
    }
    std::cerr << "Unsupported mode : " << mode << std::endl;
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  return 1;
}
