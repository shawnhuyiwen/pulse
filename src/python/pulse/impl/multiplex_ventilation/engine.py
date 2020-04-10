# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.impl.bind.MultiplexVentilator_pb2 import *
from google.protobuf import json_format
import re
import ntpath
import subprocess
from pathlib import Path

def run_simulation_list(sim_list: SimulationListData):
    # Write out that as a json object
    out = json_format.MessageToJson(sim_list)
    input_file = sim_list.OutputRootDir + "sim_list.json"
    file = open(input_file, "w")
    n = file.write(out)
    file.close()
    print("Running Simulation : "+input_file+"...")
    #FNULL = open(os.devnull, 'w')  # use this if you want to suppress output to stdout from the subprocess
    #subprocess.call(input_file, stdout=FNULL, stderr=FNULL, shell=False)
    subprocess.call(["MultiplexVentilationDriver","sim_list",input_file], shell=False, timeout=None)
    return None, None

# Our state files have a specific naming convention for encoding the PatientStateData
# This will extract the data from the state file name into a PatientStateData object
def extract_patient_data(filename: str, patient_state: PatientStateData):
    patient_state.StateFile = filename
    # Initial Patient Conditions
    patient_state.Compliance_L_Per_cmH2O = float(re.findall(r'comp=\d+\.\d+', filename)[0][5:])
    patient_state.ImpairmentFraction = float(re.findall(r'imp=\d+\.\d+', filename)[0][4:])
    # Ventilator Settings for stabilizing those coniditions
    patient_state.RespirationRate_Per_min = 20  # Constant in our C++ engine data generation algorithm
    patient_state.IERatio = 0.5  # Constant in our C++ engine data generation algorithm
    patient_state.PIP_cmH2O = int(re.findall(r'pip=\d+', filename)[0][4:])
    patient_state.PEEP_cmH2O = int(re.findall(r'peep=\d+', filename)[0][5:])
    patient_state.FiO2 = float(re.findall(r'FiO2=\d+\.\d+', filename)[0][5:])

# This will add 3 new multiplex ventilation simulations to the list between 2 patients
# 1. Both patients will be driven by one ventilator using the settings for patient 0
# 2. Both patients will be driven by one ventilator using the settings for patient 1
# 3. Both patients will be driven by one ventilator using the average of settings between both patients
def add_ventilation_comparison(sim_list: SimulationListData, patient0_state_file: str, patient1_state_file: str):

    # Where results will be placed for multiplexing these patients
    # Create a new directory of the combination of their state file names
    p0_name = ntpath.splitext(ntpath.basename(patient0_state_file))[0]
    p1_name = ntpath.splitext(ntpath.basename(patient1_state_file))[0]
    simulation_results_root = sim_list.OutputRootDir+p0_name+"+"+p1_name+"/"
    Path(simulation_results_root).mkdir(parents=True, exist_ok=True)

    sim0 = sim_list.Simulations.add()
    #  Setup the first patient
    sim0_p0 = sim0.PatientComparisons.add().SoloVentilation
    extract_patient_data(patient0_state_file, sim0_p0)
    #  Setup the second patient
    sim0_p1 = sim0.PatientComparisons.add().SoloVentilation
    extract_patient_data(patient1_state_file, sim0_p1)
    # Set the ventilator settings based on patient 0
    sim0.RespirationRate_Per_min = 20
    sim0.IERatio = 0.5
    sim0.PIP_cmH2O = sim0_p0.PIP_cmH2O
    sim0.PEEP_cmH2O = sim0_p0.PEEP_cmH2O
    sim0.FiO2 = sim0_p0.FiO2
    # Create a string encoding based on ventilator settings name the results
    sim0_base="pip="+str(sim0.PIP_cmH2O)+"_peep="+str(sim0.PEEP_cmH2O)+"_FiO2="+str(sim0.FiO2)+"_"
    sim0.OutputBaseFilename = simulation_results_root+"/"+sim0_base

    sim1 = sim_list.Simulations.add()
    #  Setup the first patient
    sim1_p0 = sim1.PatientComparisons.add().SoloVentilation
    extract_patient_data(patient0_state_file, sim1_p0)
    #  Setup the second patient
    sim1_p1 = sim1.PatientComparisons.add().SoloVentilation
    extract_patient_data(patient1_state_file, sim1_p1)
    # Set the ventilator settings based on patient 1
    sim1.RespirationRate_Per_min = 20
    sim1.IERatio = 0.5
    sim1.PIP_cmH2O = sim1_p1.PIP_cmH2O
    sim1.PEEP_cmH2O = sim1_p1.PEEP_cmH2O
    sim1.FiO2 = sim1_p1.FiO2
    # Create a string encoding based on ventilator settings name the results
    sim1_base = "pip=" + str(sim1.PIP_cmH2O) + "_peep=" + str(sim1.PEEP_cmH2O) + "_FiO2=" + str(sim1.FiO2) + "_"
    sim1.OutputBaseFilename = simulation_results_root + "/" + sim1_base

    sim2 = sim_list.Simulations.add()
    #  Setup the first patient
    sim2_p0 = sim2.PatientComparisons.add().SoloVentilation
    extract_patient_data(patient0_state_file, sim2_p0)
    #  Setup the second patient
    sim2_p1 = sim2.PatientComparisons.add().SoloVentilation
    extract_patient_data(patient1_state_file, sim2_p1)
    # Set the ventilator settings based on the average between patient 0 and 1 settings
    sim2.RespirationRate_Per_min = 20
    sim2.IERatio = 0.5
    sim2.PIP_cmH2O = (sim0_p0.PIP_cmH2O + sim1_p1.PIP_cmH2O) * 0.5
    sim2.PEEP_cmH2O = (sim0_p0.PEEP_cmH2O + sim1_p1.PEEP_cmH2O) * 0.5
    sim2.FiO2 = (sim0_p0.FiO2 + sim1_p1.FiO2) * 0.5
    # Create a string encoding based on ventilator settings name the results
    sim2_base = "pip=" + str(sim2.PIP_cmH2O) + "_peep=" + str(sim2.PEEP_cmH2O) + "_FiO2=" + str(sim2.FiO2) + "_"
    sim2.OutputBaseFilename = simulation_results_root + "/" + sim2_base

if __name__ == '__main__':

    BaseDir = "./test_results/multiplex_ventilation/"
    SoloStatesDir = BaseDir + "solo_states/"

    simulations = SimulationListData()
    simulations.OutputRootDir = BaseDir + "simulations/"
    add_ventilation_comparison(simulations,
                               SoloStatesDir+"comp=0.025_peep=18_pip=36_imp=0.6_FiO2=0.21.json",
                               SoloStatesDir+"comp=0.035_peep=20_pip=32_imp=0.9_FiO2=0.9995.json")
    run_simulation_list(simulations)
