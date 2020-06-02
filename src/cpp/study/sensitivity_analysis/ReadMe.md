## Sensitivity Analysis Study

Sensitivity analysis and uncertainty quantification are important when using computational models to simulate the cardiovascular system.
The cardiovascular system in Pulse is represented with a lumped parameter model, where discrete lumped elements are learned during the simulation.
Uncertainty in simulation outputs can arise from uncertainty in the model inputs (eg. patient height, weight, age, etc.) and these learned lumped parameters.
This study uses sensitivity analysis to explore how cardiovascular simulation outputs vary from uncertain lumped parameter elements.

As well as the C++ provide in this folder, we also used several python scripts for simulation results analysis that can be found [here](INSERT LINK)

---

### Building

The multiplex engine will be built as part of the general Pulse build.

---

### Data Model

The data model for this study can be found [here](INSERT LINK).

The data requests used for CSV files can be found [here](INSERT LINK).

---

### Data

This data used in this study consists of combinations of lumped parameter values that override the learned values during the simulation.
The lumped parameter values are generated in [Python](INSERT LINK) and stored in a JSON file.

---

### Running

The study used Python to generate combinations of lumped parameter values that used were used to override learned values in the SensitivityAnalysisDriver.
This command line application has two modes of use which will be explained in sections below.
  
##### 1) Simulation of a single patient with hardcoded overrides for testing purposes

To simulate a single patient with hardcoded overrides, use the following command:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ SensitivityAnalysisEngineDriver single
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will output data in the following folder: 

  - <b>bin/test_results/sensitivity_analysis</b>
  
The folder will contain a CSV file containing time varying data and a JSON file containing data at the end of the simulation.

##### 2) Simulation of combinations of lumped parameters

A python script was then used to create combinations of lumped parameter overrides and generate a SimulationListData json file containing all the combinations needed for this study.

To run the generated SimulationListData json file, execute the driver with the following arguments:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ SensitivityAnalysisEngineDriver sim_list ./test_results/sensitivity_analysis/sim_list.json
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will generate a ./test_results/sensitivity/simulations/sim_list_results.json file that contains all the final stabilized values of lumped parameter combinations.
CSV files for each parameter combination will also be generated.

This sim_list_results.json file was used for data analysis.


