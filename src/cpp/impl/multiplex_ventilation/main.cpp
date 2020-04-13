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
    else if (mode == "sim_list")
    {
      if (argc != 3)
      {
        std::cerr << "Please provide a SimulationListData json file" << std::endl;
        return 1;
      }
      MVRunner mvr("./states/multiplex_ventilation/MultiplexVentilationRunner.log");
      return !mvr.Run(argv[2], SerializationFormat::JSON);
    }
    else // Manual
    {
      MVController mvc("./states/multiplex_ventilation/ManualMultiplexVentilation.log", ".");
      pulse::multiplex_ventilator::bind::SimulationData sim;
      sim.set_respirationrate_per_min(mvc.DefaultRespirationRate_Per_Min());
      sim.set_ieratio(mvc.DefaultIERatio());

      std::vector<std::string> patients;
      patients.push_back("comp=0.025_peep=18_pip=36_imp=0.6_FiO2=0.21");
      patients.push_back("comp=0.035_peep=20_pip=32_imp=0.9_FiO2=0.9995");
      patients.push_back("comp=0.035_peep=18_pip=30_imp=0.3_FiO2=0.21");
      patients.push_back("comp=0.04_peep=12_pip=23_imp=0.9_FiO2=0.9995");

      double soloRunTime_s = 120;


      std::vector<std::string> names;
      std::vector<double> pips_cmH2O;
      std::vector<double> peeps_cmH2O;
      std::vector<double> FiO2s;
      std::vector<std::string> solo_patient_base_paths;
      std::string combined_name = "";
      std::ofstream plots;
      unsigned int iter = 0;

      // Figure out the name
      for (std::string patient : patients)
      {
        double pip_cmH2O;
        double peep_cmH2O;
        double FiO2;
        std::string name;
        patient = mvc.SoloDir + patient + ".json";
        mvc.ExtractVentilatorSettings(patient, name, pip_cmH2O, peep_cmH2O, FiO2);
        combined_name += name;
        if (iter != patients.size() - 1)
        {
          combined_name += "+";
        }
        iter++;
      }

      // The combined name is too long for more than 2 patients
      // This can be commented out if you have 2 patients
      combined_name = "manual";

      // Run solo patients
      iter = 0;
      for (std::string patient : patients)
      {
        double pip_cmH2O;
        double peep_cmH2O;
        double FiO2;
        std::string name;
        patient = mvc.SoloDir + patient + ".json";
        mvc.ExtractVentilatorSettings(patient, name, pip_cmH2O, peep_cmH2O, FiO2);

        std::string solo_patient_base_path = mvc.ResultsDir + combined_name + "/pip=" + to_scientific_notation(pip_cmH2O) +
          "_peep=" + to_scientific_notation(peep_cmH2O) +
          "_FiO2=" + to_scientific_notation(FiO2) + "_solo_patient_" + std::to_string(iter);

        mvc.RunSoloState(patient, solo_patient_base_path, soloRunTime_s);

        sim.add_patientcomparisons()->mutable_soloventilation()->set_statefile(patient);

        plots << solo_patient_base_path << "Results.csv\n";

        names.push_back(name);
        pips_cmH2O.push_back(pip_cmH2O);
        peeps_cmH2O.push_back(peep_cmH2O);
        FiO2s.push_back(FiO2);
        solo_patient_base_paths.push_back(solo_patient_base_path);

        iter++;
      }

      plots.open(mvc.ResultsDir + combined_name + "/plot_pairs.config");

      iter = 0;
      for (std::string solo_patient_base_path : solo_patient_base_paths)
      {
        if (iter == solo_patient_base_paths.size() - 1)
        {
          plots << solo_patient_base_path << "Results.csv\n";
        }
        else
        {
          plots << solo_patient_base_path << "Results.csv, ";
        }

        iter++;
      }

      // Run combined patients with each ventilator setting
      iter = 0;
      for (std::string patient : patients)
      {
        std::string name = names.at(iter);
        double pip_cmH2O = pips_cmH2O.at(iter);
        double peep_cmH2O = peeps_cmH2O.at(iter);
        double FiO2 = FiO2s.at(iter);

        std::string sim_base_path = mvc.ResultsDir + combined_name + "/pip=" + to_scientific_notation(pip_cmH2O) +
          "_peep=" + to_scientific_notation(peep_cmH2O) +
          "_FiO2=" + to_scientific_notation(FiO2) + "_";
        sim.set_outputbasefilename(sim_base_path);

        sim.set_pip_cmh2o(pip_cmH2O);
        sim.set_peep_cmh2o(peep_cmH2O);
        sim.set_fio2(FiO2);

        mvc.RunSimulation(sim);

        unsigned int iter2 = 0;
        for (std::string patient : patients)
        {
          if (iter2 == patients.size() - 1)
          {
            plots << sim_base_path << "multiplex_patient_" << iter2 << "_results.csv\n";
          }
          else
          {
            plots << sim_base_path << "multiplex_patient_" << iter2 << "_results.csv, ";
          }
          iter2++;
        }

        iter++;
      }

      // Run combined patients with average ventilator settings
      double pipAvg_cmH2O = 0.0;
      double peepAvg_cmH2O = 0.0;
      double FiO2Avg = 0.0;
      iter = 0;
      for (std::string patient : patients)
      {
        pipAvg_cmH2O += pips_cmH2O.at(iter);
        peepAvg_cmH2O += peeps_cmH2O.at(iter);
        FiO2Avg += FiO2s.at(iter);
        iter++;
      }
      pipAvg_cmH2O /= double(iter);
      peepAvg_cmH2O /= double(iter);
      FiO2Avg /= double(iter);

      std::string sim_base_path = mvc.ResultsDir + combined_name + "/pip=" + to_scientific_notation(pipAvg_cmH2O) +
        "_peep=" + to_scientific_notation(peepAvg_cmH2O) +
        "_FiO2=" + to_scientific_notation(FiO2Avg) + "_";
      sim.set_outputbasefilename(sim_base_path);
      sim.set_pip_cmh2o(pipAvg_cmH2O);
      sim.set_peep_cmh2o(peepAvg_cmH2O);
      sim.set_fio2(FiO2Avg);
      mvc.RunSimulation(sim);

      unsigned int iter2 = 0;
      for (std::string patient : patients)
      {
        if (iter2 == patients.size() - 1)
        {
          plots << sim_base_path << "multiplex_patient_" << iter2 << "_results.csv\n";
        }
        else
        {
          plots << sim_base_path << "multiplex_patient_" << iter2 << "_results.csv, ";
        }
        iter2++;
      }

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
