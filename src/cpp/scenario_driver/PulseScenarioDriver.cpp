/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseScenarioDriver.h"
#include "PulsePhysiologyEngine.h"
#include "PulseScenarioExec.h"

int main(int argc, char* argv[])
{
  try
  {
    if (argc <= 1)
    {
      std::cerr << "Need scenario file or config file to execute" << std::endl;
      return 1;
    }
    PulseScenarioExec::Run(argv[1]);
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
