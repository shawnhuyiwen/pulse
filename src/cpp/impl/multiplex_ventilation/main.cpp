/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVRunner.h"

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
    }
    else
    {
      mode = argv[1];
      //mode = "genData"
    };
    // convert string to back to lower case
    std::for_each(mode.begin(), mode.end(), [](char& c) { c = ::tolower(c); });
    if (mode == "gendata")
    {
      MVController mvc("./states/multiplex_ventilation/MultiplexVentilationDataGen.log");
      return !mvc.GenerateStabilizedPatients();
    }
    else if(mode == "sim_list")
    {
      if (argc != 3)
      {
        std::cerr << "Please provide a SimulationListData json file" << std::endl;
        return 1;
      }
      MVRunner mvr("./states/multiplex_ventilation/MultiplexVentilationRunner.log");
      return !mvr.Run(argv[2], SerializationFormat::JSON);
    }
    else // Manaual
    {
      MVController mvc("./states/multiplex_ventilation/ManualMultiplexVentilation.log", ".");
      pulse::multiplex_ventilator::bind::SimulationData sim;
      sim.set_respirationrate_per_min(mvc.DefaultRespirationRate_Per_Min());
      sim.set_ieratio(mvc.DefaultIERatio());

      // Change this in the code
      double p0_pip_cmH2O;
      double p0_peep_cmH2O;
      double p0_FiO2;
      std::string p0_name;
      std::string patient0 = mvc.SoloDir + "comp=0.01_peep=10_pip=55_imp=0.6_FiO2=0.21.json";
      mvc.ExtractVentilatorSettings(patient0, p0_name, p0_pip_cmH2O, p0_peep_cmH2O, p0_FiO2);

      double p1_pip_cmH2O;
      double p1_peep_cmH2O;
      double p1_FiO2;
      std::string p1_name;
      std::string patient1 = mvc.SoloDir + "comp=0.05_peep=10_pip=19_imp=0.3_FiO2=0.21.json";
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
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
