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
import seaborn as sns
import pandas as pd
import matplotlib.gridspec as gridspec
import matplotlib.image as mpimg
from scipy import stats


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


def parse_data2(simulations):
    # This assumes there are 2 patients being compared

    delta_compliance = []
    delta_O2SatIndex = []
    delta_AirwayPressure_cmH2O = []
    delta_ImpairmentFraction = []
    delta_CarricoIndex_mmHg = []
    delta_OxygenSaturation = []
    delta_SFRatio = []
    delta_OxygenationIndex = []
    delta_EndTidalCarbonDioxidePressure_mmHg = []
    delta_AlveolarArterialGradient_mmHg = []
    z_norm = []
    index = []
    tv_index = []
    SpO2_index = []
    PaO2_index = []
    count = 0
    max = 0

    for sim in simulations.Simulations:
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        p1 = sim.PatientComparisons[1].MultiplexVentilation
        p0_TV = p0.TidalVolume_mL / 75.3 # mL/kg ideal weight
        p1_TV = p1.TidalVolume_mL / 75.3 # mL/kg ideal weight
        if sim.FiO2 > 0.98 and (p0.OxygenSaturation < 0.89 or p1.OxygenSaturation < 0.89):
            # Too impaired on their own, so it won't tell us anything
            pass
        else:
            delta_compliance.append(abs(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O))
            delta_O2SatIndex.append(abs(p0.OxygenSaturationIndex_cmH2O - p1.OxygenSaturationIndex_cmH2O))
            delta_AirwayPressure_cmH2O.append(abs(p0.AirwayPressure_cmH2O - p1.AirwayPressure_cmH2O))
            delta_ImpairmentFraction.append(abs(p0.ImpairmentFraction - p1.ImpairmentFraction))
            delta_CarricoIndex_mmHg.append(abs(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg))
            delta_OxygenSaturation.append(abs(p0.OxygenSaturation - p1.OxygenSaturation))
            delta_SFRatio.append(abs(p0.SFRatio - p1.SFRatio))
            delta_OxygenationIndex.append(abs(p0.OxygenationIndex - p1.OxygenationIndex))
            delta_EndTidalCarbonDioxidePressure_mmHg.append(abs(p0.EndTidalCarbonDioxidePressure_mmHg - p1.EndTidalCarbonDioxidePressure_mmHg))
            delta_AlveolarArterialGradient_mmHg.append(abs(p0.AlveolarArterialGradient_mmHg - p1.AlveolarArterialGradient_mmHg))

            if p0.ArterialOxygenPartialPressure_mmHg >= 200 or p1.ArterialOxygenPartialPressure_mmHg >= 200:
                count = count + 1

            if p0.ArterialOxygenPartialPressure_mmHg > max:
                max = p0.ArterialOxygenPartialPressure_mmHg
            if p1.ArterialOxygenPartialPressure_mmHg > max:
                max = p1.ArterialOxygenPartialPressure_mmHg

            if 6.5 >= p0_TV >= 5.5 and \
               6.5 >= p1_TV >= 5.5 and \
               p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89:# and \
               #p0.ArterialOxygenPartialPressure_mmHg < 120 and p1.ArterialOxygenPartialPressure_mmHg < 120:
                z_norm.append(1.0)
                index.append("Positive")
            elif (7.5 >= p0_TV >= 4.5) and (7.5 >= p1_TV >= 4.5) and \
                 p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89:# and \
                 #p0.ArterialOxygenPartialPressure_mmHg < 200 and p1.ArterialOxygenPartialPressure_mmHg < 200:
                    z_norm.append(0.5)
                    index.append("Less Positive")
            else:
                z_norm.append(0.0)
                index.append("Negative")

            # TV Only
            if 6.5 >= p0_TV >= 5.5 and 6.5 >= p1_TV >= 5.5:
                tv_index.append("Positive")
            elif 7.5 >= p0_TV >= 4.5 and 7.5 >= p1_TV >= 4.5:
                tv_index.append("Less Positive")
            else:
                tv_index.append("Negative")

            # SpO2 Only
            if p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89:
                SpO2_index.append("Positive")
            else:
                SpO2_index.append("Negative")

            # PaO2 Only
            if p0.ArterialOxygenPartialPressure_mmHg < 120 and p1.ArterialOxygenPartialPressure_mmHg < 120:
                PaO2_index.append("Positive")
            # elif p0.ArterialOxygenPartialPressure_mmHg < 200 and p1.ArterialOxygenPartialPressure_mmHg < 200:
            #     PaO2_index.append("Less Positive")
            else:
                PaO2_index.append("Negative")

    #xy = np.column_stack((x, y))
    #xy = np.transpose(xy)

    delta_compliance = np.asarray(delta_compliance)
    delta_O2SatIndex = np.asarray(delta_O2SatIndex)
    z_norm = np.asarray(z_norm)

    numpy_data = np.array([delta_compliance, delta_O2SatIndex, delta_AirwayPressure_cmH2O, delta_ImpairmentFraction, delta_CarricoIndex_mmHg, delta_SFRatio, delta_OxygenationIndex, delta_EndTidalCarbonDioxidePressure_mmHg, delta_AlveolarArterialGradient_mmHg, index, tv_index, SpO2_index, PaO2_index])
    numpy_data = np.transpose(numpy_data)
    df = pd.DataFrame(data=numpy_data, columns=["delta_compliance", "delta_O2SatIndex", "delta_AirwayPressure_cmH2O", "delta_ImpairmentFraction", "delta_CarricoIndex_mmHg", "delta_SFRatio", "delta_OxygenationIndex", "delta_EndTidalCarbonDioxidePressure_mmHg", "delta_AlveolarArterialGradient_mmHg", "Outcome", "TV Outcome", "SpO2 Outcome", "PaO2 Outcome"])
    df['delta_compliance'] = df['delta_compliance'].astype(float)
    df['delta_O2SatIndex'] = df['delta_O2SatIndex'].astype(float)
    df['delta_AirwayPressure_cmH2O'] = df['delta_AirwayPressure_cmH2O'].astype(float)
    df['delta_ImpairmentFraction'] = df['delta_ImpairmentFraction'].astype(float)
    df['delta_CarricoIndex_mmHg'] = df['delta_CarricoIndex_mmHg'].astype(float)
    df['delta_SFRatio'] = df['delta_SFRatio'].astype(float)
    df['delta_OxygenationIndex'] = df['delta_OxygenationIndex'].astype(float)
    df['delta_EndTidalCarbonDioxidePressure_mmHg'] = df['delta_EndTidalCarbonDioxidePressure_mmHg'].astype(float)
    df['delta_AlveolarArterialGradient_mmHg'] = df['delta_AlveolarArterialGradient_mmHg'].astype(float)

    corr_data = np.array([delta_compliance, delta_O2SatIndex, delta_AirwayPressure_cmH2O, delta_ImpairmentFraction,
                           delta_CarricoIndex_mmHg, delta_SFRatio, delta_OxygenationIndex, delta_EndTidalCarbonDioxidePressure_mmHg, delta_AlveolarArterialGradient_mmHg])
    corr_data = np.transpose(corr_data)
    corr_df = pd.DataFrame(data=corr_data, columns=["C Mismatch (cmH2O)", "SpO2 Index Mismatch", "MAP Mismatch (cmH2O)",
                                                "Impairment Mismatch", "PaO2/FiO2 Mismatch (mmHg)",
                                                "SpO2/FiO2 Mismatch", "O2 Index Mismatch", "ETCO2 Mismatch (mmHg)", "A-a Gradient Mismatch"])
    corr_df['C Mismatch (cmH2O)'] = corr_df['C Mismatch (cmH2O)'].astype(float)
    corr_df['SpO2 Index Mismatch'] = corr_df['SpO2 Index Mismatch'].astype(float)
    corr_df['MAP Mismatch (cmH2O)'] = corr_df['MAP Mismatch (cmH2O)'].astype(float)
    corr_df['Impairment Mismatch'] = corr_df['Impairment Mismatch'].astype(float)
    corr_df['PaO2/FiO2 Mismatch (mmHg)'] = corr_df['PaO2/FiO2 Mismatch (mmHg)'].astype(float)
    corr_df['SpO2/FiO2 Mismatch'] = corr_df['SpO2/FiO2 Mismatch'].astype(float)
    corr_df['O2 Index Mismatch'] = corr_df['O2 Index Mismatch'].astype(float)
    corr_df['ETCO2 Mismatch (mmHg)'] = corr_df['ETCO2 Mismatch (mmHg)'].astype(float)
    corr_df['A-a Gradient Mismatch'] = corr_df['A-a Gradient Mismatch'].astype(float)

    #jbw - All this correlation stuff should be it's own function

    # Because 3, can't use corr function
    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_compliance,
                          df[df.Outcome == 'Less Positive'].delta_compliance,
                          df[df.Outcome == 'Negative'].delta_compliance)
    print('delta_compliance: ', F)

    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_O2SatIndex,
                           df[df.Outcome == 'Less Positive'].delta_O2SatIndex,
                           df[df.Outcome == 'Negative'].delta_O2SatIndex)
    print('delta_O2SatIndex: ', F)

    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_AirwayPressure_cmH2O,
                          df[df.Outcome == 'Less Positive'].delta_AirwayPressure_cmH2O,
                          df[df.Outcome == 'Negative'].delta_AirwayPressure_cmH2O)
    print('delta_AirwayPressure_cmH2O: ', F)

    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_ImpairmentFraction,
                          df[df.Outcome == 'Less Positive'].delta_ImpairmentFraction,
                          df[df.Outcome == 'Negative'].delta_ImpairmentFraction)
    print('delta_ImpairmentFraction: ', F)

    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_CarricoIndex_mmHg,
                          df[df.Outcome == 'Less Positive'].delta_CarricoIndex_mmHg,
                          df[df.Outcome == 'Negative'].delta_CarricoIndex_mmHg)
    print('delta_CarricoIndex_mmHg: ', F)

    # F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_OxygenSaturation,
    #                       df[df.Outcome == 'Less Positive'].delta_OxygenSaturation,
    #                       df[df.Outcome == 'Negative'].delta_OxygenSaturation)
    # print('delta_OxygenSaturation: ', F)

    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_SFRatio,
                          df[df.Outcome == 'Less Positive'].delta_SFRatio,
                          df[df.Outcome == 'Negative'].delta_SFRatio)
    print('delta_SFRatio: ', F)

    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_OxygenationIndex,
                          df[df.Outcome == 'Less Positive'].delta_OxygenationIndex,
                          df[df.Outcome == 'Negative'].delta_OxygenationIndex)
    print('delta_OxygenationIndex: ', F)

    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_EndTidalCarbonDioxidePressure_mmHg,
                          df[df.Outcome == 'Less Positive'].delta_EndTidalCarbonDioxidePressure_mmHg,
                          df[df.Outcome == 'Negative'].delta_EndTidalCarbonDioxidePressure_mmHg)
    print('delta_EndTidalCarbonDioxidePressure_mmHg: ', F)

    F, p = stats.f_oneway(df[df.Outcome == 'Positive'].delta_AlveolarArterialGradient_mmHg,
                          df[df.Outcome == 'Less Positive'].delta_AlveolarArterialGradient_mmHg,
                          df[df.Outcome == 'Negative'].delta_AlveolarArterialGradient_mmHg)
    print('delta_AlveolarArterialGradient_mmHg: ', F)

    #Need something measurable on their own ventilator, independent of ventilator setting changes (pressures and FiO2), consistent accross ventilator settings - O2 sat not good because of mismatch

    #Axes?  Are they correlated with each other?  That's where you get compliance and O2SatIndex!!!
    # Orthogonally coorolated, correlated in different dimension?

    corr = corr_df.corr(method='pearson')
    corr = corr.round(2)

    mask = np.zeros_like(corr, dtype=np.bool)
    mask[np.triu_indices_from(mask)] = True
    f, ax = plt.subplots(figsize=(15, 15))
    heatmap = sns.heatmap(corr, mask=mask, square=True, linewidths=.5, cmap='coolwarm', cbar_kws={'shrink':.5, 'ticks':[-1, -.5, 0, 0.5, 1]}, vmin=-1, vmax=1, annot=True, annot_kws={'size': 22})
    # add the column names as labels
    ax.set_yticklabels(corr.columns, rotation=0)
    ax.set_xticklabels(corr.columns)
    sns.set_style({'xtick.bottom': True}, {'ytick.left': True})

    plt.tight_layout()

    f.savefig('Correlations.png')

    return delta_compliance, delta_O2SatIndex, z_norm, df


def Interpolate2(x, y, z):
    x_bound = 18
    y_bound = 4

    xnew, ynew = np.mgrid[0:x_bound:100j, 0:y_bound:100j]
    tck, fp, ier, msg = interpolate.bisplrep(x, y, z, kx=2, ky=2, full_output=1)
    print('The weighted sum of squared residuals of the spline approximation = ',fp)
    znew = interpolate.bisplev(xnew[:, 0], ynew[0, :], tck)
    #znew = znew / np.linalg.norm(znew)
    #znew = (znew-np.min(znew))/np.ptp(znew)
    znew = np.clip(znew, 0, 1)

    return xnew, ynew, znew


def Plot2(x, y, z, xnew, ynew, znew, df):
    x_bound = 22
    y_bound = 8

    x_label = "Respiratory Compliance Mismatch (mL/cmH2O)"
    y_label = "Oxygen Saturation Index Mismatch (mmHg)"

    grid0 = sns.JointGrid(x='delta_compliance', y='delta_O2SatIndex', data=df, xlim=(0, x_bound), ylim=(0, y_bound))
    # jbw - Update the pallet
    g0 = grid0.plot_joint(sns.scatterplot, marker=1, s=150, hue='Outcome', hue_order=['Positive', 'Less Positive', 'Negative'],  data=df, palette=['green', 'yellow', 'red'])
    g0.set_axis_labels(x_label, y_label)
    g0.fig.suptitle("Combined Bounds Outcome")

    gridsize = 100

    sns.kdeplot(df.loc[df['Outcome'] == 'Positive', 'delta_compliance'], ax=g0.ax_marg_x, legend=False, color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Outcome'] == 'Less Positive', 'delta_compliance'], ax=g0.ax_marg_x, legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Outcome'] == 'Negative', 'delta_compliance'], ax=g0.ax_marg_x, legend=False, color='red', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Outcome'] == 'Positive', 'delta_O2SatIndex'], ax=g0.ax_marg_y, vertical=True, legend=False, color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Outcome'] == 'Less Positive', 'delta_O2SatIndex'], ax=g0.ax_marg_y, vertical=True, legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Outcome'] == 'Negative', 'delta_O2SatIndex'], ax=g0.ax_marg_y, vertical=True, legend=False, color='red', shade=True, gridsize=gridsize)

    grid1 = sns.JointGrid(x='delta_compliance', y='delta_O2SatIndex', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g1 = grid1.plot_joint(sns.scatterplot, marker=1, s=150, hue='TV Outcome', hue_order=['Positive', 'Less Positive', 'Negative'], data=df, palette=['green', 'yellow', 'red'])
    g1.set_axis_labels(x_label, y_label)
    g1.fig.suptitle("TV Bounds Outcome")

    sns.kdeplot(df.loc[df['TV Outcome'] == 'Positive', 'delta_compliance'], ax=g1.ax_marg_x, legend=False, color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Less Positive', 'delta_compliance'], ax=g1.ax_marg_x, legend=False,
                color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Negative', 'delta_compliance'], ax=g1.ax_marg_x, legend=False, color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Positive', 'delta_O2SatIndex'], ax=g1.ax_marg_y, vertical=True, legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Less Positive', 'delta_O2SatIndex'], ax=g1.ax_marg_y, vertical=True,
                legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Negative', 'delta_O2SatIndex'], ax=g1.ax_marg_y, vertical=True, legend=False,
                color='red', shade=True, gridsize=gridsize)

    grid2 = sns.JointGrid(x='delta_compliance', y='delta_O2SatIndex', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g2 = grid2.plot_joint(sns.scatterplot, marker=1, s=150, hue='SpO2 Outcome', hue_order=['Positive', 'Negative'], data=df, palette=['green', 'red'])
    g2.set_axis_labels(x_label, y_label)
    g2.fig.suptitle("SpO2 Bounds Outcome")

    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Positive', 'delta_compliance'], ax=g2.ax_marg_x, legend=False,
                color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Negative', 'delta_compliance'], ax=g2.ax_marg_x, legend=False, color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Positive', 'delta_O2SatIndex'], ax=g2.ax_marg_y, vertical=True,
                legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Negative', 'delta_O2SatIndex'], ax=g2.ax_marg_y, vertical=True,
                legend=False,
                color='red', shade=True, gridsize=gridsize)

    grid3 = sns.JointGrid(x='delta_compliance', y='delta_O2SatIndex', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g3 = grid3.plot_joint(sns.scatterplot, marker=1, s=150, hue='PaO2 Outcome', hue_order=['Positive', 'Less Positive', 'Negative'], data=df, palette=['green', 'yellow', 'red'])
    g3.set_axis_labels(x_label, y_label)
    g3.fig.suptitle("PaO2 Bounds Outcome")

    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Positive', 'delta_compliance'], ax=g3.ax_marg_x, legend=False,
                color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Less Positive', 'delta_compliance'], ax=g3.ax_marg_x, legend=False,
                color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Negative', 'delta_compliance'], ax=g3.ax_marg_x, legend=False, color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Positive', 'delta_O2SatIndex'], ax=g3.ax_marg_y, vertical=True,
                legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Less Positive', 'delta_O2SatIndex'], ax=g3.ax_marg_y, vertical=True,
                legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Negative', 'delta_O2SatIndex'], ax=g3.ax_marg_y, vertical=True,
                legend=False,
                color='red', shade=True, gridsize=gridsize)

    ############### 2. SAVE PLOTS IN MEMORY TEMPORALLY
    g0.savefig('g0.png')
    plt.close(g0.fig)

    g1.savefig('g1.png')
    plt.close(g1.fig)

    g2.savefig('g2.png')
    plt.close(g2.fig)

    g3.savefig('g3.png')
    plt.close(g3.fig)

    ############### 3. CREATE YOUR SUBPLOTS FROM TEMPORAL IMAGES
    f, axarr = plt.subplots(2, 2, figsize=(10, 10))
    #f.suptitle("Combined Outcomes: Patient 1 vs Patient 2 Clinical Measurements")

    axarr[0, 0].imshow(mpimg.imread('g0.png', format="png"))
    axarr[0, 1].imshow(mpimg.imread('g1.png', format="png"))
    axarr[1, 0].imshow(mpimg.imread('g3.png', format="png"))
    axarr[1, 1].imshow(mpimg.imread('g2.png', format="png"))

    # turn off x and y axis
    [ax.set_axis_off() for ax in axarr.ravel()]

    plt.tight_layout()

    plt.savefig('Outcome_Bounds_Comparison.png')

    x_bound = 18
    y_bound = 4

    fig, axs = plt.subplots(1, 2, figsize=(15, 6))
    fig.suptitle('Combined Outcomes: Patient 1 vs Patient 2 Clinical Measurements', fontsize=14)

    axs[0].set_aspect=x_bound/y_bound
    axs[0].scatter(x, y, marker=1, s=250, c=z, cmap='RdYlGn')
    axs[0].set_xlim([0, x_bound])
    axs[0].set_ylim([0, y_bound])
    axs[0].set_title("Simulation Results")
    axs[0].set_xlabel(x_label)
    axs[0].set_ylabel(y_label)

    axs[1].set_aspect=20/y_bound
    axs[1].pcolor(xnew, ynew, znew, cmap='RdYlGn')
    #axs[1].contour(xnew, ynew, znew, colors='k')
    axs[1].set_title("Interpolated Results")
    axs[1].set_xlabel(x_label)
    axs[1].set_ylabel(y_label)

    #plt.tight_layout()

    plt.savefig('Interpolation.png')

    plt.show()

    #jbw - save to disk

def box_plot(simulations):
    # This assumes there are 2 patients being compared

    O2SatIndex = []
    ImpairmentFraction = []

    for sim in simulations.Simulations:
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        p1 = sim.PatientComparisons[1].MultiplexVentilation

        O2SatIndex.append(p0.OxygenSaturationIndex_cmH2O)
        O2SatIndex.append(p1.OxygenSaturationIndex_cmH2O)
        ImpairmentFraction.append(p0.ImpairmentFraction)
        ImpairmentFraction.append(p1.ImpairmentFraction)

    O2SatIndex = np.asarray(O2SatIndex)
    ImpairmentFraction = np.asarray(ImpairmentFraction)

    numpy_data = np.array([ImpairmentFraction, O2SatIndex])
    numpy_data = np.transpose(numpy_data)

    numpy_data = np.sort(numpy_data, 0)
    grouped_data = np.split(numpy_data[:, 1], np.cumsum(np.unique(numpy_data[:, 0], return_counts=True)[1])[:-1])
    uniqueValues, indicesList = np.unique(numpy_data[:, 0], return_index=True)
    uniqueValues = np.around(uniqueValues, decimals=1)

    fig, ax = plt.subplots()
    ax.boxplot(grouped_data, labels=uniqueValues)
    ax.yaxis.grid(True, linestyle='-', which='major', color='lightgrey', alpha=0.5)
    ax.set_title('Oxygen Saturation Index Distribution')
    ax.set_xlabel('Diffusion Impairment Factor')
    ax.set_ylabel('Oxygen Saturation Index (mmHg)')
    plt.savefig('Distribution.png')
    plt.show()


def plot_interpolate2(simulations: SimulationListData):
    x, y, z, df = parse_data2(simulations)

    xnew, ynew, znew = Interpolate2(x, y, z)

    Plot2(x, y, z, xnew, ynew, znew, df)


if __name__ == '__main__':
    # Load up a result set
    results_file = "./test_results/multiplex_ventilation/simulations/simlist_results_Fix.json"

    with open(results_file) as f:
        json = f.read()
    results = SimulationListData()
    json_format.Parse(json, results)
    # Make some plots
    #plot_all_parameters(results)
    #plot_by_parameter(results)
    #plot_interpolate2(results)
    box_plot(results)