# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.study.bind.MultiplexVentilation_pb2 import  *
from pulse.study.multiplex_ventilation.engine import MVEngine

def HowTo_UseEngine():
    # Create an engine
    mve = MVEngine("./test_results/multiplex_ventilation/MVEngine.log", True)

    sim = SimulationData()
    sim.ID = 42
    sim.OutputBaseFilename = "./test_results/multiplex_ventilation/mvpy"
    # Use these ventilator settings
    sim.RespirationRate_Per_min = 20
    sim.IERatio = 0.5
    sim.PIP_cmH2O = 28
    sim.PEEP_cmH2O = 10
    sim.FiO2 = 0.23
    # Add Patient 0
    p0 = sim.PatientComparisons.add().MultiplexVentilation
    p0.ID = 0
    p0.Compliance_mL_Per_cmH2O = 10
    p0.Resistance_cmH2O_s_Per_L = 5
    p0.ImpairmentFraction = 0.5
    # Add Patient 1
    p1 = sim.PatientComparisons.add().MultiplexVentilation
    p1.ID = 1
    p1.Compliance_mL_Per_cmH2O = 20
    p1.Resistance_cmH2O_s_Per_L = 5
    p1.ImpairmentFraction = 0.75

    mve.create_engine(sim)

    for n in range(60):
        mve.advance_time_s(2, sim)
        print("Patient0 SpO2 : "+str(sim.PatientComparisons[0].MultiplexVentilation.OxygenSaturation))
        print("Patient1 SpO2 : "+str(sim.PatientComparisons[1].MultiplexVentilation.OxygenSaturation))

    mve.destroy()



HowTo_UseEngine()