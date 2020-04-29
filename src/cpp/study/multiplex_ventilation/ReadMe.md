## Multiplex Ventilation Study

The COVID-19 pandemic is stretching medical resources internationally, including ventilator shortages.
The possibility of needing to ventilate multiple patients with a single ventilator raises patient health concerns.
This study explores patient compatibility and ventilator settings during multiplex ventilation.

Provided is a new custom simulation engine capable of simulating simultaneous ventilation of two or more patients in parallel by a single ventilator.
This multiplex ventilation engine was used to perform a sensitivity analysis for clinical parameters during pressure mode mechanical ventilation without flow modulation.

As well as the C++ provide in this folder, we also used several python scripts for simulation results analysis that can be found [here](https://gitlab.kitware.com/physiology/engine/-/tree/3.x/src/python/pulse/study/multiplex_ventilation).

---

### Building

The multiplex engine will be built as part of the general Pulse build.

---

### Data Model

The data model for this study can be found [here](https://gitlab.kitware.com/physiology/engine/-/blob/3.x/src/schema/pulse/study/multiplex_ventilation/bind/MultiplexVentilation.proto).

The data requests used for CSV files can be found [here](https://gitlab.kitware.com/physiology/engine/-/blob/3.x/src/cpp/study/multiplex_ventilation/MVEngine.cpp#L612).

---

### Data

All data associated with our study can be found [here](https://data.kitware.com/#collection/59849c788d777f7d33e9c084/folder/5e979e5a9014a6d84e12b150).
Zips include: json files of SimulationListData and PatientListData (as defined in our data model), as well as log and csv files for all patients.

  - <b>multiplex_ventilation.zip</b> : All data generated for our initial study of the parameter space
  - <b>multiplex_ventilation_final_full_study.zip</b> : All data generated for the full study 
  - <b>MultiplexExampleRunPaperData.zip</b> : Individual runs used in creating tables and figures in our paper
  - <b>multiplex_simlist_results.json.zip</b> : Just the SimulationListData results json file
  - <b>solo_simlist_results.json.zip</b> : Just the SimulationListData results json file
 
In general, we only viewed plots from a random sampling of the CSV files to ensure our algorithms were performing as expected.
The majority of our analysis was done using the multiplex and solo simlist json files in python. 

---

### Running

The majority of the study used the MultiplexEngineDriver to generate all data used.
This command line application has several modes of use which will be explained in sections below.

#### Initial investigation

Our initial investigation used sparse sampling of the combined mechanical ventilator and COVID-19 patient parameter space to understand what patterns emerged.
These simulations combined various ventilator PEEP settings with patient compliance and impairment severity values.

This was implemented as a two step process: 
  
##### 1) Simulation of patients with various levels of diseases states on their own ventilator

We first generated a patient set with a combination of impairment, compliance, and PEEP setting and increasing FiO2 values until they reach a homeostatic point with an SpO2 above 89% and serialize to disk.

To generate this data set, execute the driver with the following arguments:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ MultiplexEngineDriver genData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will create the following folders : 

  - <b>bin/test_results/multiplex_ventilation/solo_states</b> : All the stabilized Pulse Engine states generated (Single patient on a single ventilator)
  - <b>bin/test_results/multiplex_ventilation/solo_states/log</b> : All pulse logs associated with each of the engine states
  - <b>bin/test_results/multiplex_ventilation/solo_states/csv</b> : All pulse csv files associated with each engine state run

An accompanying PatientStateListData json file will be created : bin/test_results/multiplex_ventilation/solo_ventilated_states_list.json
This file contains all the final patient values of interest for all generated patients

##### 2) Simulation of paired patients with various ventilator settings

A python script was then used to read in the generated python file, and generate a SimulationListData json file containing all the combinations needed for this initial study.
For each patient comination, three separate simulations were added to the SimulationList using each patient’s individual ventilator settings and average values; 
unless the patient was paired with itself (i.e., exact twins), in which case, only one simulation was added to the list.

To run the generated SimulationListData json file, execute the driver with the following arguments:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ MultiplexEngineDriver sim_list ./test_results/multiplex_ventilation/sim_list.json
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will generate a ./test_results/multiplex_ventilation/sim_list_results.json file that contains all the final stabilized values of patient combinations.

This sim_list_results.json file was used in an ad hoc analysis and led to our full study design.

#### Simulation of patient combinations where the PIP and FiO2 have the best chance of positive outcomes for both patients

Both data generation and multiplex engine execution is performed by one mode.

To generate the final data set, execute the driver with the following arguments:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ MultiplexEngineDriver gensimlist
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will create the following folders : 

  - <b>bin/test_results/multiplex_ventilation/simulations</b> : Will contain a folder for each run simulation that will containe both logs and csv files for each patient

Two accompanying SimulationListData json file will be created : 
  - bin/test_results/multiplex_ventilation/simulations/multiplex_simlist_results.json
  - bin/test_results/multiplex_ventilation/simulations/solo_simlist_results.json

This file was used for analyis using the algorithms implemented in the [analysis.py](https://gitlab.kitware.com/physiology/engine/-/blob/3.x/src/python/pulse/study/multiplex_ventilation/analysis.py) file.
Note we use PyCharm, and we provide a .idea project in the src/python directory.
You will need to build Pulse with the Pulse_PYTHON_API CMake flag enabled, and add `install/bin` folder to your PyCharm interpreter path.

### Other run modes

The main method support two more arguments:

  - single : This is used for testing purposes
  - manual : This is coded to generate a single initial studay data set for 2 or more patients. This mode was used to generate the MultiplexExampleRunPaperData data set. 

