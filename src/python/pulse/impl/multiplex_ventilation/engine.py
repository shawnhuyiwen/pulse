# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.impl.bind.MultiplexVentilator_pb2 import *
from google.protobuf import json_format
import re
import json
import ntpath
import itertools
import subprocess
import pandas as pd
from pathlib import Path

def run_simulation_list(sim_list: SimulationListData):
    # Write out that as a json object
    out = json_format.MessageToJson(sim_list)
    input_file = sim_list.OutputRootDir + "sim_list.json"
    #file = open(input_file, "w")
    #n = file.write(out)
    #file.close()
    print("Running Simulation : "+input_file+"...")
    #FNULL = open(os.devnull, 'w')  # use this if you want to suppress output to stdout from the subprocess
    #subprocess.call(input_file, stdout=FNULL, stderr=FNULL, shell=False)
    #subprocess.call(["MultiplexVentilationDriver","sim_list",input_file], shell=False, timeout=None)
    return input_file

# Our state files have a specific naming convention for encoding the PatientStateData
# This will extract the data from the state file name into a PatientStateData object
def extract_patient_data(filename: str, patient_state: PatientStateData):
    patient_state.StateFile = filename
    # Initial Patient Conditions
    patient_state.Compliance_L_Per_cmH2O = float(re.findall(r'comp=\d+\.\d+', filename)[0][5:])
    if filename.find('imp=0_'):
        patient_state.ImpairmentFraction = 0
    else:
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

def get_patient_pairs(solo_patients_file):
    # values of patients to compare
    patient_pair_sweeps = {
        'Compliance_L_Per_cmH2O': [0.01, 0.02, 0.05],
        'PEEP_cmH2O': [10, 16, 20],
        'ImpairmentFraction': [0.0,  0.3,  0.6, 0.9],
    }
    # read in the patient json file
    patientsfile = open(solo_patients_file, 'r')
    # convert it to pandas dataframe (i.e. table)
    patientdata = json.load(patientsfile)['Patients']
    patients = pd.read_json(path_or_buf=json.dumps(patientdata), orient='records')

    # correct no impairment case (from NaN to 0.0)
    patients.fillna(value=0.0, axis='columns', inplace=True)
    # find the patient records of interest (use df.query)
    # handle non-exact impairments (0.2, 0.4, 0.6, 0.8)
    individuals = []
    for compliance in patient_pair_sweeps['Compliance_L_Per_cmH2O']:
        for PEEP in patient_pair_sweeps['PEEP_cmH2O']:
            for impairment in patient_pair_sweeps['ImpairmentFraction']:
                query_string = 'Compliance_L_Per_cmH2O >= {compliance_lower} & \
                                Compliance_L_Per_cmH2O < {compliance_upper} & \
                                PEEP_cmH2O == {PEEP} & \
                                ImpairmentFraction >= {impairment_lower} & \
                                ImpairmentFraction < {impairment_upper} \
                                '.format(compliance_lower=(compliance-0.005),
                                         compliance_upper=(compliance+0.005),
                                         PEEP=PEEP,
                                         impairment_lower=impairment,
                                         impairment_upper=(impairment+0.1)
                                )
                patient_record = patients.query(
                                        query_string
                                        ).to_dict(orient='records')[0]
                individuals.append(patient_record['StateFile'])

    # return record of pairs
    return list(itertools.combinations_with_replacement(individuals, 2))

def plot_simulation_results(simulations: SimulationListData):
    for sim in simulations.Simulations:
        p0_csv = sim.OutputBaseFilename+"multiplex_patient_0_results.csv"
        p1_csv = sim.OutputBaseFilename+"multiplex_patient_1_results.csv"
        print(p0_csv)
        print(p1_csv)

if __name__ == '__main__':
    # The Initial patient solo ventilation states should have already been generated
    BaseDir = "./test_results/multiplex_ventilation/"
    pairs = get_patient_pairs(BaseDir+"solo_ventilated_states_list.json")
    # Run all these pairs
    simulations = SimulationListData()
    simulations.OutputRootDir = BaseDir + "simulations/"
    add_ventilation_comparison(simulations, pairs[0][0], pairs[0][1])
    results_file = run_simulation_list(simulations)
    # Read in the results file
    with open(results_file) as f:
        json = f.read()
    results = SimulationListData()
    json_format.Parse(json, results)
    plot_simulation_results(results)
