# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.study.multiplex_ventilation.bind.MultiplexVentilation_pb2 import *
from google.protobuf import json_format
from mpl_toolkits.mplot3d import Axes3D
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy.optimize as opt
import matplotlib.cm as cmx
import matplotlib.tri as mtri
from scipy import interpolate


def plot_all_parameters(simulations: SimulationListData):
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
        p0_TV = p0.TidalVolume_mL / 75.3 # mL/kg ideal weight
        p1_TV = p1.TidalVolume_mL / 75.3 # mL/kg ideal weight
        if True: #p0.AchievedStabilization and p1.AchievedStabilization:
            if 6.5 >= p0_TV >= 5.5 and \
               6.5 >= p1_TV >= 5.5 and \
               p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89 and \
               p0.ArterialOxygenPartialPressure_mmHg < 120 and p1.ArterialOxygenPartialPressure_mmHg < 120:
                x_green.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                y_green.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
            elif (7.5 >= p0_TV >= 4.5) and (7.5 >= p1_TV >= 4.5) and \
                 p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89 and \
                 p0.ArterialOxygenPartialPressure_mmHg < 200 and p1.ArterialOxygenPartialPressure_mmHg < 200:
                    x_yellow.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                    y_yellow.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
            else:
                x_red.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                y_red.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
    plt.style.use('seaborn-whitegrid')
    plt.title("Differences in Lung Mechanics and Oxygen Transfer")
    plt.xlabel("Compliance Delta (mL_Per_cmH2O)")
    plt.ylabel("PaO2/FiO2 Delta (mmHg)")
    plt.plot(x_green, y_green, 'o', color='green', alpha=0.3)
    plt.plot(x_yellow, y_yellow, 'o', color='yellow', alpha=0.3)
    plt.plot(x_red, y_red, 'o', color='red', alpha=0.3)
    plt.show()


def plot_by_parameter(simulations: SimulationListData):
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
        p0_TV = p0.TidalVolume_mL / 75.3 # mL/kg ideal weight
        p1_TV = p1.TidalVolume_mL / 75.3 # mL/kg ideal weight
        compliance_diff = p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O
        carrico_diff = p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg
        if 6.5 >= p0_TV >= 5.5 and 6.5 >= p1_TV >= 5.5:
            x_green.append(compliance_diff)
            y_green.append(carrico_diff)
        elif 7.5 >= p0_TV >= 4.5 and 7.5 >= p1_TV >= 4.5:
            x_yellow.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
            y_yellow.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
        else:
            x_red.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
            y_red.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
    plot1 = plt.figure(1)
    plt.style.use('seaborn-whitegrid')
    plt.title("Due to Tidal Volume")
    plt.xlabel("Compliance Delta (mL_Per_cmH2O)")
    plt.ylabel("PaO2/FiO2 Delta (mmHg)")
    plt.plot(x_green, y_green, 'o', color='green', alpha=0.3)
    plt.plot(x_yellow, y_yellow, 'o', color='yellow', alpha=0.3)
    plt.plot(x_red, y_red, 'o', color='red', alpha=0.3)
    plot1.show()

    x_green = []
    y_green = []
    x_yellow = []
    y_yellow = []
    x_red = []
    y_red = []
    for sim in simulations.Simulations:
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        p1 = sim.PatientComparisons[1].MultiplexVentilation
        p0_TV = p0.TidalVolume_mL / 75.3 # mL/kg ideal weight
        p1_TV = p1.TidalVolume_mL / 75.3 # mL/kg ideal weight
        if True: #p0.AchievedStabilization and p1.AchievedStabilization:
            if p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89:
                x_green.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                y_green.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
            else:
                x_red.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                y_red.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
    plot2 = plt.figure(2)
    plt.style.use('seaborn-whitegrid')
    plt.title("Due to SpO2")
    plt.xlabel("Compliance Delta (mL_Per_cmH2O)")
    plt.ylabel("PaO2/FiO2 Delta (mmHg)")
    plt.plot(x_green, y_green, 'o', color='green', alpha=0.3)
    plt.plot(x_yellow, y_yellow, 'o', color='yellow', alpha=0.3)
    plt.plot(x_red, y_red, 'o', color='red', alpha=0.3)
    plot2.show()

    x_green = []
    y_green = []
    x_yellow = []
    y_yellow = []
    x_red = []
    y_red = []
    for sim in simulations.Simulations:
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        p1 = sim.PatientComparisons[1].MultiplexVentilation
        p0_TV = p0.TidalVolume_mL / 75.3 # mL/kg ideal weight
        p1_TV = p1.TidalVolume_mL / 75.3 # mL/kg ideal weight
        if p0.ArterialOxygenPartialPressure_mmHg < 120 and p1.ArterialOxygenPartialPressure_mmHg < 120 and \
           (p0.AchievedStabilization and p1.AchievedStabilization):
            x_green.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
            y_green.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
        elif p0.ArterialOxygenPartialPressure_mmHg < 200 and p1.ArterialOxygenPartialPressure_mmHg < 200 and \
             (p0.AchievedStabilization and p1.AchievedStabilization):
                x_yellow.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                y_yellow.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
        elif p0.ArterialOxygenPartialPressure_mmHg >= 200 or p1.ArterialOxygenPartialPressure_mmHg >= 200:
            x_red.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
            y_red.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
    plot3 = plt.figure(3)
    plt.style.use('seaborn-whitegrid')
    plt.title("Due to PaO2")
    plt.xlabel("Compliance Delta (mL_Per_cmH2O)")
    plt.ylabel("PaO2/FiO2 Delta (mmHg)")
    plt.plot(x_green, y_green, 'o', color='green', alpha=0.3)
    plt.plot(x_yellow, y_yellow, 'o', color='yellow', alpha=0.3)
    plt.plot(x_red, y_red, 'o', color='red', alpha=0.3)
    plot3.show()

    plt.show()


def parse_data(simulations):
    # This assumes there are 2 patients being compared

    x = []
    y = []
    z_num = []
    z_norm = []
    for sim in simulations.Simulations:
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        p1 = sim.PatientComparisons[1].MultiplexVentilation
        p0_TV = p0.TidalVolume_mL / 75.3 # mL/kg ideal weight
        p1_TV = p1.TidalVolume_mL / 75.3 # mL/kg ideal weight
        if True: #p0.AchievedStabilization and p1.AchievedStabilization:
            if 6.5 >= p0_TV >= 5.5 and \
               6.5 >= p1_TV >= 5.5 and \
               p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89 and \
               p0.ArterialOxygenPartialPressure_mmHg < 120 and p1.ArterialOxygenPartialPressure_mmHg < 120:
                x.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                y.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
                z_norm.append(1.0)
            elif (7.5 >= p0_TV >= 4.5) and (7.5 >= p1_TV >= 4.5) and \
                 p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89 and \
                 p0.ArterialOxygenPartialPressure_mmHg < 200 and p1.ArterialOxygenPartialPressure_mmHg < 200:
                    x.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                    y.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
                    z_norm.append(0.5)
            else:
                x.append(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O)
                y.append(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg)
                z_norm.append(0.0)

    #xy = np.column_stack((x, y))
    #xy = np.transpose(xy)

    x = np.asarray(x)
    y = np.asarray(y)
    z_norm = np.asarray(z_norm)

    return x, y, z_norm


def Interpolate(x, y, z):
    xnew, ynew = np.mgrid[-20:20:100j, -500:500:100j]
    tck = interpolate.bisplrep(x, y, z, kx=2, ky=2)
    #jbw - get r squared value
    znew = interpolate.bisplev(xnew[:, 0], ynew[0, :], tck)
    #znorm = znew / np.linalg.norm(znew)
    znorm = (znew-np.min(znew))/np.ptp(znew)

    return xnew, ynew, znorm

def Plot(x, y, z, xnew, ynew, znorm):
    fig, axs = plt.subplots(1, 2)
    fig.suptitle('Patient 1 vs Patient 2 Outcomes', fontsize=16)

    axs[0].set_aspect=20/500
    axs[0].scatter(x, y, marker='o', c=z, cmap='RdYlGn', alpha=0.3)
    axs[0].set_xlim([-20, 20])
    axs[0].set_ylim([-500, 500])
    axs[0].set_title("Simulation Results")
    axs[0].set_xlabel("Respiratory Compliance Difference (mL/cmH2O)")
    axs[0].set_ylabel("PaO2/FiO2 Difference (mmHg)")

    #jbw - Add KDE plots of all three colors in both axes (probably using seaborn)

    axs[1].set_aspect=20/500
    axs[1].pcolor(xnew, ynew, znorm, cmap='RdYlGn')
    #plt.colorbar()
    axs[1].set_title("Interpolated Results")
    axs[1].set_xlabel("Respiratory Compliance Difference (mL/cmH2O)")
    axs[1].set_ylabel("PaO2/FiO2 Difference (mmHg)")

    plt.show()


def plot_interpolate(simulations: SimulationListData):
    x, y, z = parse_data(simulations)

    xnew, ynew, znew = Interpolate(x, y, z)

    Plot(x, y, z, xnew, ynew, znew)


if __name__ == '__main__':
    # Load up a result set
    results_file = "./test_results/multiplex_ventilation/simulations/simlist_results.json"
    with open(results_file) as f:
        json = f.read()
    results = SimulationListData()
    json_format.Parse(json, results)
    # Make some plots
    #plot_all_parameters(results)
    #plot_by_parameter(results)
    plot_interpolate(results)