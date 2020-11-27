## Sensitivity Analysis and Parameter Optimization Study

Sensitivity analysis and parameter optimization are important tools when performing physiologically relevant patient-specific simulations.
The work performed here focuses specifically on the coupled cardiovascular-respiratory systems. The systems in Pulse are represented with lumped parameter
models, consisting of discrete circuit elements analagous to an electrical circuit. The values of these circuit elements influence simulation results, but
these values cannot be measured clinically, This study uses sensitivity analysis as a path towards improved lumped parameter optimization.

As well as the C++ files provided in this folder, we also used several python scripts for simulation results analysis that can be found [here](https://gitlab.kitware.com/physiology/engine/-/tree/study/cv_sensitivity/src/python/pulse/study/sensitivity_analysis)

---

### Building

The sensitivity analysis engine will be built as part of the general Pulse build.

---

### Data Model

The data model for this study can be found [here](https://gitlab.kitware.com/physiology/engine/-/blob/study/cv_sensitivity/src/schema/pulse/study/bind/SensitivityAnalysis.proto).

---

### Data

The data used in this study consists of combinations of lumped parameter values that override the learned values during the simulation.
The lumped parameter values are generated in [Python](https://gitlab.kitware.com/physiology/engine/-/tree/study/cv_sensitivity/src/python/pulse/study/sensitivity_analysis) and stored in a JSON file.

---

### Running

The study used Python to generate combinations of lumped parameter values that were used to override learned values in the SensitivityAnalysisDriver.
This command line application has two modes of use which will be explained in sections below.
  
##### 1) Simulation of a single patient with hardcoded overrides for testing purposes

To simulate a single patient with hardcoded overrides, use the following command:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ SensitivityAnalysisEngineDriver single
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will output data in the following folder: 

  - <b>bin/test_results/sensitivity_analysis</b>
  
The folder will contain a JSON file containing data at the end of the simulation.

##### 2) Simulation of combinations of lumped parameters

A python script was then used to create combinations of lumped parameter overrides and generate a SimulationListData json file containing all the combinations needed for this study.

To run the generated SimulationListData json file, execute the driver with the following arguments:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ SensitivityAnalysisEngineDriver sim_list ./test_results/sensitivity_analysis/sim_list.json
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will generate a ./test_results/sensitivity/simulations/sim_list_results.json file that contains all the final stabilized values of lumped parameter combinations.
CSV files for each parameter combination will also be generated.

This sim_list_results.json file was used for data analysis.

---
### Analysis
All data analysis was performed with Python scripts found [here]https://gitlab.kitware.com/physiology/engine/-/tree/study/cv_sensitivity/src/python/pulse/study/sensitivity_analysis


