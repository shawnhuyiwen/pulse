/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

void GenerateStabilizedPatients();

int main(int argc, char* argv[])
{
  try
  {
    if (argc <= 1)
    {
      std::cerr << "Need to provide run mode and associated parameters" << std::endl;
      return 1;
    }
    std::string mode = argv[1];
    // convert string to back to lower case
    std::for_each(mode.begin(), mode.end(), [](char& c) { c = ::tolower(c); });
    if (mode == "gendata")
    {
      GenerateStabilizedPatients();
    }
    else if (mode == "combine")
    {
      std::vector<std::string> patients;
      for (int i=2; i<argc; i++)
        patients.push_back(argv[i]);
      if (patients.size() >= 2)
      {
        MVController mvc("MultiplexVentilation.log");
        return !mvc.Run(patients);
      }
      else
      {
        std::cerr << "Provide more than one patient state to combine";
      }
    }
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
