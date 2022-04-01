/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"

using namespace pulse::study::patient_variability;

int main(int argc, char* argv[])
{
 

  PVRunner pvr("./test_results/patient_variability/PatientVariabilityRunner.log");
  pulse::study::bind::patient_variability::SimulationListData simList;
  simList.set_outputrootdir("./test_results/patient_variability");

  return 0;
}