## Multiplex Ventilation Study

The SARS-CoV-2 pandemic is stretching medical resources internationally,including ventilator shortages.
The possibility of needing to ventilate multiple patients with a single ventilator raises patient health concerns.
This study explores patient compatibility and ventilator settings during multi-plex ventilation.

Provided is a new custom simulation engine capable of simulating simultaneous ventilation of two or more patients in parallel by a single ventilator.
This multiplex ventilation engine was used to perform a sensitivity analysis for clinical parameters during pressure mode mechanical ventilation without flow modulation.

As well as the C++ provide in this folder, we also used severl python scripts that can be found [here]()

### Building

The multiplex engine will be built as part of the general Pulse build.

### Data Model

The data model for this study can be found [here]()

The data requests used for CSV files can be found [here]()

### Running

The majority of the study used the MultiplexEngineDriver to generate all data used.
This command line application has several modes of use which will be explained in sections below.

#### Initial investigation

Our initial investigation used sparse sampling of the combined mechanical ventilator and COVID-19 patient parameter space to understand what patterns emerged.
These simulations combined various ventilator PEEP settings with patient compliance and impairment severity values.

This was implemented as a two step process: 
  
##### Simulation of patients with various levels of diseases states on their own ventilator

We first generated a patient set with a combination of impairment, compliance and PEEP setting and increasing FiO2 values until they reach a homeostatic point with an SpO2 above 89% and serialize to disk.

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

A python script was then used to read in the generated python file, and generate a SimulationListData json file containing all the combinations needed for this initial study.
For each patient comination, three separate simulations were added to the SimulationList using each patient’s individual ventilator settings and average values.
Not that if the patient was combined with itself, only one simulation was added to the list. (Paired with itself)

To run the generated SimulationListData json file, execute the driver with the following arguments:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ MultiplexEngineDriver sim_list ./test_results/multiplex_ventilation/sim_list.json
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will generate a ./test_results/multiplex_ventilation/sim_list_results.json file that contains all the final stabilized values of patient combinations.

This sim_list_results.json file was used in an ad hoc analysis that analysis lead to design our full study.

#### Simulation of patient combinations where the PIP and FiO2 have the best chance of positive outcomes for both patients

Both data generation and multiplex engine execution is performed by one mode.

To generate the final data set, execute the driver with the following arguments:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ MultiplexEngineDriver gensimlist
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will create the following folders : 

  - <b>bin/test_results/multiplex_ventilation/simulations</b> : Will contain a folder for each run simulation that will containe both logs and csv files for each patient

An accompanying SimulationListData json file will be created : bin/test_results/multiplex_ventilation/simulations/simlist_results.json

This file was used for analyis using the algorithms implemented in the [analysis.py]() file

