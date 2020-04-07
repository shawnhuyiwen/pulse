/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

int main(int argc, char* argv[])
{

  try
  {
    std::string mode;
    std::string file;
    if (argc <= 1)
    {
      // Adjust bools to run what ever mode you want with out input
      if(true)
       mode = "genData";
      else if (true)
      {
        mode = "state";
        std::vector<std::string> state_files;
        ListFiles("./states/multiplex_ventilation/", state_files, ".json");
        if (state_files.empty())
        {
          std::cerr << "Need to provide run mode and associated parameters" << std::endl;
          return 1;
        }
        else
          file = state_files[0];
      }
    }
    else
    {
      mode = argv[1];
    }
    // convert string to back to lower case
    std::for_each(mode.begin(), mode.end(), [](char& c) { c = ::tolower(c); });
    if (mode == "gendata")
    {
      MVController mvc("MultiplexVentilationDataGen.log");
      return !mvc.GenerateStabilizedPatients();
    }
    else if (mode == "scenario" || mode == "state")
    {
      MVController mvc("MultiplexVentilation.log");
      pulse::multiplex_ventilator::bind::SimulationData sim;
      if (mode == "state")
      {
        
        double pip = 0;
        double FiO2 = 0;
        // Parse the file name to get our ventilator settings
        size_t peepIdx = file.find("peep=")+5;
        size_t _peepIdx = file.find(".", peepIdx);
        double peep = std::atof(file.substr(peepIdx, _peepIdx).c_str());
        sim.set_respirationrate_per_min(mvc.DefaultRespirationRate_Per_Min());
        sim.set_ieratio(mvc.DefaultIERatio());
        sim.set_pip_cmh2o(pip);
        sim.set_peep_cmh2o(peep);
        sim.set_fio2(FiO2);
        sim.add_patientcomparisons()->mutable_soloventilation()->set_statefile(file);
        sim.add_patientcomparisons()->mutable_soloventilation()->set_statefile(file);
      }
      else
      {
        google::protobuf::util::JsonParseOptions opts;
        google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
        {
          std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
        });
        std::string content = ReadFile(file, SerializationFormat::JSON);
        if (content.empty())
        {
          std::cerr << "Unable to read file \n" << file;
          return 1;
        }
        google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(content, &sim, opts);
        if (!stat.ok())
        {
          std::cerr << "Unable to parse json in file \n" << file;
          return 1;
        }
      }

      return !mvc.RunSimulation(sim);
    }
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
