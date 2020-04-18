# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.study.multiplex_ventilation.bind.MultiplexVentilation_pb2 import *
from google.protobuf import json_format
import matplotlib.pyplot as plt

def plot_compliance_vs_pf_ration_deltas(simulations: SimulationListData):
    # This assumes there are 2 patients being compared
    x_green = []
    y_green = []
    x_yellow = []
    y_yellow = []
    x_red = []
    y_red = []
    for sim in simulations.Simulations:
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        p1 = sim.PatientComparisons[1].MultiplexVentilation
        p0_TV = p0.TidalVolume_L * 13.280 # mL/kg ideal weight
        p1_TV = p1.TidalVolume_L * 13.280 # mL/kg ideal weight
        if 6.5 >= p0_TV >= 5.5 and \
           6.5 >= p1_TV >= 5.5 and \
           p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89 and \
           p0.ArterialOxygenPartialPressure_mmHg < 120 and p1.ArterialOxygenPartialPressure_mmHg < 120:
            x_green.append(abs(p0.Compliance_L_Per_cmH2O - p1.Compliance_L_Per_cmH2O))
            y_green.append(abs(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg))
        elif ((7.5 >= p0_TV >= 6.5 or 5.5 >= p0_TV >= 4.5) and (7.5 >= p1_TV >= 4.5)) or \
             ((7.5 >= p1_TV >= 6.5 or 5.5 >= p1_TV >= 4.5) and (7.5 >= p0_TV >= 4.5)) and \
             p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89 and \
             p0.ArterialOxygenPartialPressure_mmHg < 200 and p1.ArterialOxygenPartialPressure_mmHg < 200:
                x_yellow.append(abs(p0.Compliance_L_Per_cmH2O - p1.Compliance_L_Per_cmH2O))
                y_yellow.append(abs(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg))
        elif (7.5 >= p0_TV >= 6.5 or 5.5 >= p0_TV >= 4.5) and (7.5 >= p1_TV >= 4.5):
                x_yellow.append(abs(p0.Compliance_L_Per_cmH2O - p1.Compliance_L_Per_cmH2O))
                y_yellow.append(abs(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg))
        else:
            x_red.append(abs(p0.Compliance_L_Per_cmH2O - p1.Compliance_L_Per_cmH2O))
            y_red.append(abs(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg))
    plt.style.use('seaborn-whitegrid')
    plt.title("Compliance Delta vs PaO2/FiO2 Delta")
    plt.xlabel("Compliance Delta (L_Per_cmH2O)")
    plt.ylabel("PaO2/FiO2 Delta (mmHg)")
    plt.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
    plt.plot(x_green, y_green, 'o', color='green')
    plt.plot(x_yellow, y_yellow, 'o', color='yellow')
    plt.plot(x_red, y_red, 'o', color='red')
    plt.show()


if __name__ == '__main__':
    # Load up a result set
    results_file = "./test_results/multiplex_ventilation/simulations/sim_list_results.json"
    with open(results_file) as f:
        json = f.read()
    results = SimulationListData()
    json_format.Parse(json, results)
    # Make some plots
    plot_compliance_vs_pf_ration_deltas(results)