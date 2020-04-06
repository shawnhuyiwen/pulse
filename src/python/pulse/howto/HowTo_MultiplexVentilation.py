# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.impl.bind.MultiplexVentilator_pb2 import  *
from google.protobuf import json_format
import itertools

def HowTo_UseEngine():
    # Let's load up a data file from disk
    f = open("./mv_solo_ventilated_patients.json", "rb")
    json = f.read()
    f.close()
    # Parse it into our protobuf object
    patient_list = PatientStateListData()
    json_format.Parse(json, patient_list)
    # Let's examine the data
    for i, p in enumerate(patient_list.Patients, start=1):
        print("Patient "+str(i))
        print("  - State File : "+p.StateFile)
        print("  - Compliance_L_Per_cmH2O : "+str(p.Compliance_L_Per_cmH2O))
        print("  - ImpairmentFraction : "+str(p.ImpairmentFraction))
        print("  - PEEP_cmH2O : "+str(p.PEEP_cmH2O))
        print("  - PIP_cmH2O : "+str(p.Compliance_L_Per_cmH2O))
        print("  - FiO2 : "+str(p.FiO2))
        print("  - OxygenSaturation : "+str(p.OxygenSaturation))
        print("  - TidalVolume_L : "+str(p.TidalVolume_L))
        print("  - EndTidalCarbonDioxidePressure_cmH2O : "+str(p.EndTidalCarbonDioxidePressure_cmH2O))
        print("  - CarricoIndex : "+str(p.CarricoIndex))

    # Let's create a simulation run with 2 patients and the ventilator setings we want to run
    multiplex_simulation = SimulationData()
    p0 = patient_list.Patients[0]
    patient_1 = multiplex_simulation.PatientComparisons.add()
    patient_1.SoloVentilation.StateFile = p0.StateFile
    p50 = patient_list.Patients[50]
    patient_2 = multiplex_simulation.PatientComparisons.add()
    patient_2.SoloVentilation.StateFile = p50.StateFile
    # Use these ventilator settings
    multiplex_simulation.RespirationRate_Per_min = 20
    multiplex_simulation.IERatio = 0.5
    multiplex_simulation.PIP_cmH2O = 28
    multiplex_simulation.PEEP_cmH2O = 10
    multiplex_simulation.FiO2 = 0.3
    # Write out that as a json object
    out = json_format.MessageToJson(multiplex_simulation)
    file = open("multiplex_ventilation_scenario.json", "w")
    n = file.write(out)
    file.close()

    # Let's write out a sample results file
    # It's the same thing we just wrote, but with values on the solo and multiplex patient
    # The original solo ventilation data will be provided
    patient_1.SoloVentilation.Compliance_L_Per_cmH2O = p0.Compliance_L_Per_cmH2O
    patient_1.SoloVentilation.ImpairmentFraction = p0.ImpairmentFraction
    patient_1.SoloVentilation.PEEP_cmH2O = p0.PEEP_cmH2O
    patient_1.SoloVentilation.PIP_cmH2O = p0.PIP_cmH2O
    patient_1.SoloVentilation.FiO2 = p0.FiO2
    patient_1.SoloVentilation.OxygenSaturation = p0.OxygenSaturation
    patient_1.SoloVentilation.TidalVolume_L = p0.TidalVolume_L
    patient_1.SoloVentilation.EndTidalCarbonDioxidePressure_cmH2O = p0.EndTidalCarbonDioxidePressure_cmH2O
    patient_1.SoloVentilation.CarricoIndex = p0.CarricoIndex
    # Here is the patient_1 values after being hooked to a ventilator with patient_2
    patient_1.MultiplexVentilation.Compliance_L_Per_cmH2O = p0.Compliance_L_Per_cmH2O
    patient_1.MultiplexVentilation.ImpairmentFraction = p0.ImpairmentFraction
    patient_1.MultiplexVentilation.PEEP_cmH2O = p0.PEEP_cmH2O
    patient_1.MultiplexVentilation.PIP_cmH2O = p0.PIP_cmH2O
    patient_1.MultiplexVentilation.FiO2 = p0.FiO2
    patient_1.MultiplexVentilation.OxygenSaturation = 92
    patient_1.MultiplexVentilation.TidalVolume_L = p0.TidalVolume_L
    patient_1.MultiplexVentilation.EndTidalCarbonDioxidePressure_cmH2O = p0.EndTidalCarbonDioxidePressure_cmH2O
    # The original solo ventilation data will be provided
    patient_2.MultiplexVentilation.CarricoIndex = p50.CarricoIndex
    patient_2.SoloVentilation.Compliance_L_Per_cmH2O = p50.Compliance_L_Per_cmH2O
    patient_2.SoloVentilation.ImpairmentFraction = p50.ImpairmentFraction
    patient_2.SoloVentilation.PEEP_cmH2O = p50.PEEP_cmH2O
    patient_2.SoloVentilation.PIP_cmH2O = p50.PIP_cmH2O
    patient_2.SoloVentilation.FiO2 = p50.FiO2
    patient_2.SoloVentilation.OxygenSaturation = p50.OxygenSaturation
    patient_2.SoloVentilation.TidalVolume_L = p50.TidalVolume_L
    patient_2.SoloVentilation.EndTidalCarbonDioxidePressure_cmH2O = p50.EndTidalCarbonDioxidePressure_cmH2O
    patient_2.SoloVentilation.CarricoIndex = p50.CarricoIndex
    # Here is the patient_2 values after being hooked to a ventilator with patient_1
    patient_2.MultiplexVentilation.Compliance_L_Per_cmH2O = p50.Compliance_L_Per_cmH2O
    patient_2.MultiplexVentilation.ImpairmentFraction = p50.ImpairmentFraction
    patient_2.MultiplexVentilation.PEEP_cmH2O = p50.PEEP_cmH2O
    patient_2.MultiplexVentilation.PIP_cmH2O = p50.PIP_cmH2O
    patient_2.MultiplexVentilation.FiO2 = p50.FiO2
    patient_2.MultiplexVentilation.OxygenSaturation = 88
    patient_2.MultiplexVentilation.TidalVolume_L = p50.TidalVolume_L
    patient_2.MultiplexVentilation.EndTidalCarbonDioxidePressure_cmH2O = p50.EndTidalCarbonDioxidePressure_cmH2O
    patient_2.MultiplexVentilation.CarricoIndex = p50.CarricoIndex
    # Write out the example results
    out = json_format.MessageToJson(multiplex_simulation)
    file = open("multiplex_ventilation_scenario_out.json", "w")
    n = file.write(out)
    file.close()
    # Read in the example results
    f = open("./multiplex_ventilation_scenario_out.json", "rb")
    json = f.read()
    f.close()
    # Parse it into our protobuf object
    multiplex_simulation_results = SimulationData()
    json_format.Parse(json, multiplex_simulation_results)
    print("Multiplex ventilation patient 1 spO2 : " + str(multiplex_simulation_results.PatientComparisons[0].MultiplexVentilation.OxygenSaturation))
    print("Multiplex ventilation patient 2 spO2 : " + str(multiplex_simulation_results.PatientComparisons[1].MultiplexVentilation.OxygenSaturation))

    # Here is all combinations of our generated patients
    total_patients = len(patient_list.Patients)
    combos = list(itertools.combinations(range(total_patients), 2))
    print(combos)

HowTo_UseEngine()