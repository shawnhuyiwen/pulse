/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"
#include "PVGenerator.h"

#include "cdm/utils/FileUtils.h"

using namespace pulse::study::patient_variability;

int main(int argc, char* argv[])
{
  std::string mode = ""; // Default is to run the manual else below
  if (argc <= 1)
  {
    // Adjust comments to run the mode you want
    //mode = "clear";
    mode = "genPatients";
  }
  else
  {
    mode = argv[1];
  }
  
  if(mode == "genPatients")
  {
    PVGenerator pvg("./test_results/patient_variability/PatientVariabilityGen.log");
    pulse::study::bind::patient_variability::SimulationListData simList;
    simList.set_outputrootdir("./test_results/patient_variability");

    return !pvg.Run("./test_results/patient_variability", "./test_results/patient_variability/states_list.json");
  }
  else if(mode == "clear")
  {
    DeleteDirectory("./test_results/patient_variability");
  }


  return 0;
}