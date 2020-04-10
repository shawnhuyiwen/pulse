/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

int main(int argc, char* argv[])
{

  try
  {
    std::string mode = ""; // Default is to run the manual else below
    if (argc <= 1)
    {
      // Adjust comments to run the mode you want
      //mode = "genData";
      //mode = "twinsy";
      //mode = "sim";
    }
    else
    {
      mode = argv[1];
    }
    // convert string to back to lower case
    std::for_each(mode.begin(), mode.end(), [](char& c) { c = ::tolower(c); });
    if (mode == "gendata")
    {
      MVController mvc("./states/multiplex_ventilation/MultiplexVentilationDataGen.log");
      return !mvc.GenerateStabilizedPatients();
    }
    else
    {
      MVController mvc("./states/multiplex_ventilation/MultiplexVentilation.log");
      pulse::multiplex_ventilator::bind::SimulationData sim;
      sim.set_respirationrate_per_min(mvc.DefaultRespirationRate_Per_Min());
      sim.set_ieratio(mvc.DefaultIERatio());
      
      if (mode == "twinsy")
      {
        std::string filePath;
        std::vector<std::string> state_files;
        ListFiles("./states/multiplex_ventilation/solo_states", state_files, ".json");
        if (state_files.empty())
        {
          std::cerr << "Need to provide run mode and associated parameters" << std::endl;
          return 1;
        }
        else
          filePath = state_files[0];
        
        double pip_cmH2O;
        double peep_cmH2O;
        double FiO2;
        std::string fileName;
        mvc.ExtractVentilatorSettings(filePath, fileName, pip_cmH2O, peep_cmH2O, FiO2);
        sim.set_respirationrate_per_min(mvc.DefaultRespirationRate_Per_Min());
        sim.set_ieratio(mvc.DefaultIERatio());
        sim.set_pip_cmh2o(pip_cmH2O);
        sim.set_peep_cmh2o(peep_cmH2O);
        sim.set_fio2(FiO2);
        sim.add_patientcomparisons()->mutable_soloventilation()->set_statefile(filePath);
        sim.add_patientcomparisons()->mutable_soloventilation()->set_statefile(filePath);
        // Flag is to run the solo patients on their own for 2 mins to get results, before multiplexing the patients
        // Note if you are running the same state file, it will only be run once
        return !mvc.RunSimulation(sim);
      }
      else if(mode == "sim")
      {
        if (argc != 3)
        {
          std::cerr << "Please provide a SimulationData json file" << std::endl;
          return 1;
        }
        std::string simFile = argv[2];
        google::protobuf::util::JsonParseOptions opts;
        google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
        {
          std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
        });
        std::string content = ReadFile(simFile, SerializationFormat::JSON);
        if (content.empty())
        {
          std::cerr << "Unable to read file \n" << simFile;
          return 1;
        }
        google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(content, &sim, opts);
        if (!stat.ok())
        {
          std::cerr << "Unable to parse json in file \n" << simFile;
          return 1;
        }
        return !mvc.RunSimulation(sim);
      }
      else if (mode == "sim_list")
      {
        if (argc != 3)
        {
          std::cerr << "Please provide a SimulationListData json file" << std::endl;
          return 1;
        }
        pulse::multiplex_ventilator::bind::SimulationListData sim_list;
        std::string simFile = argv[2];
        google::protobuf::util::JsonParseOptions opts;
        google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
        {
          std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
        });
        std::string content = ReadFile(simFile, SerializationFormat::JSON);
        if (content.empty())
        {
          std::cerr << "Unable to read file \n" << simFile;
          return 1;
        }
        google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(content, &sim_list, opts);
        if (!stat.ok())
        {
          std::cerr << "Unable to parse json in file \n" << simFile;
          return 1;
        }
        for (int s = 0; s < sim_list.simulations_size(); s++)
        {
          pulse::multiplex_ventilator::bind::SimulationData* simulation = &(*sim_list.mutable_simulations())[s];
          // TODO Thread this
          // TODO Don't run if we already have run this simulation
          if (!mvc.RunSimulation(*simulation))
          {
            std::cerr << "Error running simulation" << std::endl;
          }
        }
        // Let's write the file back out
        //std::string simFile

      }
      else // Manaual
      {
        // Change this in the code
        double p0_pip_cmH2O;
        double p0_peep_cmH2O;
        double p0_FiO2;
        std::string p0_name;
        std::string patient0 = mvc.SoloDir + "comp=0.025_peep=18_pip=36_imp=0.6_FiO2=0.21.json";
        mvc.ExtractVentilatorSettings(patient0, p0_name, p0_pip_cmH2O, p0_peep_cmH2O, p0_FiO2);

        double p1_pip_cmH2O;
        double p1_peep_cmH2O;
        double p1_FiO2;
        std::string p1_name;
        std::string patient1 = mvc.SoloDir + "comp=0.035_peep=20_pip=32_imp=0.9_FiO2=0.9995.json";
        mvc.ExtractVentilatorSettings(patient1, p1_name, p1_pip_cmH2O, p1_peep_cmH2O, p1_FiO2);

        std::string solo_patient0_base_path = mvc.ResultsDir+p0_name+"+"+p1_name+"/pip="+to_scientific_notation(p0_pip_cmH2O)+
                                                                                 "_peep="+to_scientific_notation(p0_peep_cmH2O)+
                                                                                 "_FiO2="+to_scientific_notation(p0_FiO2)+"_solo_patient_0";
        mvc.RunSoloState(patient0, solo_patient0_base_path, 120);
        std::string solo_patient1_base_path = mvc.ResultsDir+p0_name+"+"+p1_name+"/pip="+to_scientific_notation(p1_pip_cmH2O)+
                                                                                 "_peep="+to_scientific_notation(p1_peep_cmH2O)+
                                                                                 "_FiO2="+to_scientific_notation(p1_FiO2)+"_solo_patient_1";
        mvc.RunSoloState(patient1, solo_patient1_base_path, 120);

        std::string sim0_base_path = mvc.ResultsDir+p0_name+"+"+p1_name+"/pip="+to_scientific_notation(p0_pip_cmH2O)+
                                                                        "_peep="+to_scientific_notation(p0_peep_cmH2O)+
                                                                        "_FiO2="+to_scientific_notation(p0_FiO2)+"_";
        sim.set_outputbasefilename(sim0_base_path);
        sim.set_pip_cmh2o(p0_pip_cmH2O);
        sim.set_peep_cmh2o(p0_peep_cmH2O);
        sim.set_fio2(p0_FiO2);
        sim.add_patientcomparisons()->mutable_soloventilation()->set_statefile(patient0);
        sim.add_patientcomparisons()->mutable_soloventilation()->set_statefile(patient1);
        mvc.RunSimulation(sim);

        std::string sim1_base_path = mvc.ResultsDir+p0_name+"+"+p1_name+"/pip="+to_scientific_notation(p1_pip_cmH2O)+
                                                                        "_peep="+to_scientific_notation(p1_peep_cmH2O)+
                                                                        "_FiO2="+to_scientific_notation(p1_FiO2)+"_";
        sim.set_outputbasefilename(sim1_base_path);
        sim.set_pip_cmh2o(p1_pip_cmH2O);
        sim.set_peep_cmh2o(p1_peep_cmH2O);
        sim.set_fio2(p1_FiO2);
        mvc.RunSimulation(sim);


        double pipAvg_cmH2O = (p0_pip_cmH2O+p1_pip_cmH2O)*0.5;
        double peepAvg_cmH2O = (p0_peep_cmH2O+p1_peep_cmH2O)*0.5;
        double FiO2Avg = (p0_FiO2+p1_FiO2)*0.5;
        std::string sim2_base_path =mvc.ResultsDir+p0_name+"+"+p1_name+"/pip="+to_scientific_notation(pipAvg_cmH2O)+
                                                                       "_peep="+to_scientific_notation(peepAvg_cmH2O)+
                                                                       "_FiO2="+to_scientific_notation(FiO2Avg)+"_";
        sim.set_outputbasefilename(sim2_base_path);
        sim.set_pip_cmh2o(pipAvg_cmH2O);
        sim.set_peep_cmh2o(peepAvg_cmH2O);
        sim.set_fio2(FiO2Avg);
        mvc.RunSimulation(sim);

        std::ofstream plots;
        plots.open(mvc.ResultsDir + p0_name + "+" + p1_name + "/plot_pairs.config");
        plots << solo_patient0_base_path<<"Results.csv, "<< solo_patient1_base_path<<"Results.csv\n";
        plots << sim0_base_path<<"multiplex_patient_0Results.csv, "<< sim0_base_path << "multiplex_patient_1Results.csv\n";
        plots << sim1_base_path<<"multiplex_patient_0Results.csv, "<< sim1_base_path << "multiplex_patient_1Results.csv\n";
        plots << sim2_base_path<<"multiplex_patient_0Results.csv, "<< sim2_base_path << "multiplex_patient_1Results.csv\n";
        plots.close();
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
