# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.study.bind.MultiplexVentilation_pb2 import *
from google.protobuf import json_format

import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import pandas as pd
import statsmodels.api as sm
import seaborn as sns

from scipy import interpolate
from statsmodels.formula.api import ols
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import MaxNLocator
from matplotlib.colors import BoundaryNorm


def parse_data(simulations):
    # This assumes there are 2 patients being compared

    # Continuous patient variables
    names_continuous = []
    
    delta_compliance = []
    names_continuous.append("C(stat) Mismatch (cmH2O)")
    delta_O2SatIndex = []
    names_continuous.append("OSI Mismatch (mmHg)")
    detla_MeanAirwayPressure_cmH2O = []
    names_continuous.append("MAP Mismatch (cmH2O)")
    delta_ImpairmentFraction = []
    names_continuous.append("DIF Mismatch")
    delta_CarricoIndex_mmHg = []
    names_continuous.append("PF Ratio Mismatch (mmHg)")
    delta_SFRatio = []
    names_continuous.append("SF Ratio Mismatch")
    delta_OxygenationIndex = []
    names_continuous.append("OI Mismatch")
    delta_EndTidalCarbonDioxidePressure_mmHg = []
    names_continuous.append("EtCO2 Mismatch (mmHg)")
    delta_AlveolarArterialGradient_mmHg = []
    names_continuous.append("A-a Gradient Mismatch (mmHg)")

    # Categorical outcome variables
    names_categorical = []

    combined_outcome = []
    names_categorical.append("Combined Outcome")
    tv_outcome = []
    names_categorical.append("TV Outcome")
    SpO2_outcome = []
    names_categorical.append("SpO2 Outcome")
    PaO2_outcome = []
    names_categorical.append("PaO2 Outcome")

    z_norm = []

    fraction_failed = []
    num_9_failed = 0
    num_8_failed = 0
    num_7_failed = 0

    total_simulations = 0
    total_used_simulations = 0

    for sim in simulations.Simulations:
        total_simulations = total_simulations + 1
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        p1 = sim.PatientComparisons[1].MultiplexVentilation
        p0_TV_mL_Per_kg = p0.TidalVolume_mL / p0.IdealBodyWeight_kg
        p1_TV_mL_Per_kg = p1.TidalVolume_mL / p1.IdealBodyWeight_kg

        if not sim.AchievedStabilization:
            # Too impaired to stabilize on their own, so it won't tell us anything
            fraction = 0
            if not p0.AchievedStabilization:
                fraction = p0.ImpairmentFraction
                fraction_failed.append(p0.ImpairmentFraction)
            if not p1.AchievedStabilization:
                fraction = p1.ImpairmentFraction
                fraction_failed.append(p1.ImpairmentFraction)
            if fraction > 0.85:
                num_9_failed = num_9_failed + 1
            elif fraction > 0.75:
                num_8_failed = num_8_failed + 1
            elif fraction > 0.65:
                num_7_failed = num_7_failed + 1
        else:
            total_used_simulations = total_used_simulations + 1
            delta_compliance.append(abs(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O))
            delta_O2SatIndex.append(abs(p0.OxygenSaturationIndex_mmHg - p1.OxygenSaturationIndex_mmHg))
            detla_MeanAirwayPressure_cmH2O.append(abs(p0.MeanAirwayPressure_cmH2O - p1.MeanAirwayPressure_cmH2O))
            delta_ImpairmentFraction.append(abs(p0.ImpairmentFraction - p1.ImpairmentFraction))
            delta_CarricoIndex_mmHg.append(abs(p0.CarricoIndex_mmHg - p1.CarricoIndex_mmHg))
            delta_SFRatio.append(abs(p0.SFRatio - p1.SFRatio))
            delta_OxygenationIndex.append(abs(p0.OxygenationIndex - p1.OxygenationIndex))
            delta_EndTidalCarbonDioxidePressure_mmHg.append(abs(p0.EndTidalCarbonDioxidePressure_mmHg - p1.EndTidalCarbonDioxidePressure_mmHg))
            delta_AlveolarArterialGradient_mmHg.append(abs(p0.AlveolarArterialGradient_mmHg - p1.AlveolarArterialGradient_mmHg))

            if 6.5 >= p0_TV_mL_Per_kg >= 5.5 and \
               6.5 >= p1_TV_mL_Per_kg >= 5.5 and \
               p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89 and \
               p0.ArterialOxygenPartialPressure_mmHg < 120 and p1.ArterialOxygenPartialPressure_mmHg < 120:
                    z_norm.append(1.0)
                    combined_outcome.append("Positive")
            elif (7.5 >= p0_TV_mL_Per_kg >= 4.5) and (7.5 >= p1_TV_mL_Per_kg >= 4.5) and \
                 p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89 and \
                 p0.ArterialOxygenPartialPressure_mmHg < 200 and p1.ArterialOxygenPartialPressure_mmHg < 200:
                    z_norm.append(0.5)
                    combined_outcome.append("Less Positive")
            else:
                z_norm.append(0.0)
                combined_outcome.append("Negative")

            # TV Only
            if 6.5 >= p0_TV_mL_Per_kg >= 5.5 and 6.5 >= p1_TV_mL_Per_kg >= 5.5:
                tv_outcome.append("Positive")
            elif 7.5 >= p0_TV_mL_Per_kg >= 4.5 and 7.5 >= p1_TV_mL_Per_kg >= 4.5:
                tv_outcome.append("Less Positive")
            else:
                tv_outcome.append("Negative")

            # SpO2 Only
            if p0.OxygenSaturation >= 0.89 and p1.OxygenSaturation >= 0.89:
                SpO2_outcome.append("Positive")
            else:
                SpO2_outcome.append("Negative")

            # PaO2 Only
            if p0.ArterialOxygenPartialPressure_mmHg < 120 and p1.ArterialOxygenPartialPressure_mmHg < 120:
                PaO2_outcome.append("Positive")
            elif p0.ArterialOxygenPartialPressure_mmHg < 200 and p1.ArterialOxygenPartialPressure_mmHg < 200:
                PaO2_outcome.append("Less Positive")
            else:
                PaO2_outcome.append("Negative")

    print('total simulations = ', total_simulations)
    print('total used simulations = ', total_used_simulations)
    print('total unused simulations = ', total_simulations - total_used_simulations)
    print('total impairment 0.9 fails = ', num_9_failed)
    print('total impairment 0.8 fails = ', num_8_failed)
    print('total impairment 0.7 fails = ', num_7_failed)

    # Make sure this order matches names_continuous!
    numpy_continuous = np.array([delta_compliance, delta_O2SatIndex, detla_MeanAirwayPressure_cmH2O, delta_ImpairmentFraction,
                           delta_CarricoIndex_mmHg, delta_SFRatio, delta_OxygenationIndex,
                           delta_EndTidalCarbonDioxidePressure_mmHg, delta_AlveolarArterialGradient_mmHg])
    numpy_continuous = numpy_continuous.astype(float)
    # Make sure this order matches names_categorical!
    numpy_data = np.concatenate((numpy_continuous, [combined_outcome, tv_outcome, SpO2_outcome, PaO2_outcome]))
    numpy_data = np.transpose(numpy_data)
    names = names_continuous + names_categorical
    df = pd.DataFrame(data=numpy_data, columns=names)
    df[names_continuous] = df[names_continuous].astype(float)

    delta_compliance = np.asarray(delta_compliance)
    delta_O2SatIndex = np.asarray(delta_O2SatIndex)
    z_norm = np.asarray(z_norm)

    return delta_compliance, delta_O2SatIndex, z_norm, df, numpy_continuous, names_continuous, names_categorical


def plot_interpolation(x, y, z):
    x_bound = 22
    y_bound = 4.5
    x_label = "Respiratory Compliance Mismatch (mL/cmH2O)"
    y_label = "Oxygen Saturation Index Mismatch (mmHg)"

    xnew, ynew = np.mgrid[0:x_bound:100j, 0:y_bound:100j]
    tck, fp, ier, msg = interpolate.bisplrep(x, y, z, kx=1, ky=1, full_output=1)
    print('The weighted sum of squared residuals of the spline approximation = ',fp)
    znew = interpolate.bisplev(xnew[:, 0], ynew[0, :], tck)
    #znew = znew / np.linalg.norm(znew)
    #znew = (znew-np.min(znew))/np.ptp(znew)
    znew = np.clip(znew, 0, 1)

    fig, axs = plt.subplots(1, 2, figsize=(15, 6))
    fig.suptitle('Combined Outcomes: Patient 1 vs Patient 2 Clinical Measurements', fontsize=14)

    axs[0].set_aspect = x_bound / y_bound
    axs[0].scatter(x, y, marker=1, s=250, c=z, cmap='RdYlGn')
    axs[0].set_xlim([0, x_bound])
    axs[0].set_ylim([0, y_bound])
    axs[0].set_title("Simulation Results")
    axs[0].set_xlabel(x_label)
    axs[0].set_ylabel(y_label)

    axs[1].set_aspect = 20 / y_bound
    axs[1].pcolor(xnew, ynew, znew, cmap='RdYlGn')
    # axs[1].contour(xnew, ynew, znew, colors='k')
    axs[1].set_title("Interpolated Results")
    axs[1].set_xlabel(x_label)
    axs[1].set_ylabel(y_label)

    # plt.tight_layout()

    plt.savefig('Multiplex_Interpolated.png')


def plot_outcome_comparisons(x, y, z, df):
    x_bound = 22
    y_bound = 4.5
    x_label = "Respiratory Compliance Mismatch (mL/cmH2O)"
    y_label = "Oxygen Saturation Index Mismatch (mmHg)"

    grid0 = sns.JointGrid(x='C(stat) Mismatch (cmH2O)', y='OSI Mismatch (mmHg)', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g0 = grid0.plot_joint(sns.scatterplot, marker=1, s=150, hue='Combined Outcome',
                          hue_order=['Positive', 'Less Positive', 'Negative'], data=df,
                          palette=['green', 'yellow', 'red'])
    g0.set_axis_labels(x_label, y_label)
    g0.fig.suptitle("Combined Bounds Outcome")

    gridsize = 100

    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Positive', 'C(stat) Mismatch (cmH2O)'], ax=g0.ax_marg_x, legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Less Positive', 'C(stat) Mismatch (cmH2O)'], ax=g0.ax_marg_x,
                legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Negative', 'C(stat) Mismatch (cmH2O)'], ax=g0.ax_marg_x, legend=False,
                color='red', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Positive', 'OSI Mismatch (mmHg)'], ax=g0.ax_marg_y, vertical=True,
                legend=False, color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Less Positive', 'OSI Mismatch (mmHg)'], ax=g0.ax_marg_y,
                vertical=True, legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Negative', 'OSI Mismatch (mmHg)'], ax=g0.ax_marg_y, vertical=True,
                legend=False, color='red', shade=True, gridsize=gridsize)

    grid1 = sns.JointGrid(x='C(stat) Mismatch (cmH2O)', y='OSI Mismatch (mmHg)', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g1 = grid1.plot_joint(sns.scatterplot, marker=1, s=150, hue='TV Outcome',
                          hue_order=['Positive', 'Less Positive', 'Negative'], data=df,
                          palette=['green', 'yellow', 'red'])
    g1.set_axis_labels(x_label, y_label)
    g1.fig.suptitle("TV Bounds Outcome")

    sns.kdeplot(df.loc[df['TV Outcome'] == 'Positive', 'C(stat) Mismatch (cmH2O)'], ax=g1.ax_marg_x, legend=False,
                color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Less Positive', 'C(stat) Mismatch (cmH2O)'], ax=g1.ax_marg_x, legend=False,
                color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Negative', 'C(stat) Mismatch (cmH2O)'], ax=g1.ax_marg_x, legend=False,
                color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Positive', 'OSI Mismatch (mmHg)'], ax=g1.ax_marg_y, vertical=True,
                legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Less Positive', 'OSI Mismatch (mmHg)'], ax=g1.ax_marg_y, vertical=True,
                legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Negative', 'OSI Mismatch (mmHg)'], ax=g1.ax_marg_y, vertical=True,
                legend=False,
                color='red', shade=True, gridsize=gridsize)

    grid2 = sns.JointGrid(x='C(stat) Mismatch (cmH2O)', y='OSI Mismatch (mmHg)', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g2 = grid2.plot_joint(sns.scatterplot, marker=1, s=150, hue='SpO2 Outcome', hue_order=['Positive', 'Negative'],
                          data=df, palette=['green', 'red'])
    g2.set_axis_labels(x_label, y_label)
    g2.fig.suptitle("SpO2 Bounds Outcome")

    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Positive', 'C(stat) Mismatch (cmH2O)'], ax=g2.ax_marg_x, legend=False,
                color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Negative', 'C(stat) Mismatch (cmH2O)'], ax=g2.ax_marg_x, legend=False,
                color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Positive', 'OSI Mismatch (mmHg)'], ax=g2.ax_marg_y, vertical=True,
                legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Negative', 'OSI Mismatch (mmHg)'], ax=g2.ax_marg_y, vertical=True,
                legend=False,
                color='red', shade=True, gridsize=gridsize)

    grid3 = sns.JointGrid(x='C(stat) Mismatch (cmH2O)', y='OSI Mismatch (mmHg)', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g3 = grid3.plot_joint(sns.scatterplot, marker=1, s=150, hue='PaO2 Outcome',
                          hue_order=['Positive', 'Less Positive', 'Negative'], data=df,
                          palette=['green', 'yellow', 'red'])
    g3.set_axis_labels(x_label, y_label)
    g3.fig.suptitle("PaO2 Bounds Outcome")

    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Positive', 'C(stat) Mismatch (cmH2O)'], ax=g3.ax_marg_x, legend=False,
                color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Less Positive', 'C(stat) Mismatch (cmH2O)'], ax=g3.ax_marg_x,
                legend=False,
                color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Negative', 'C(stat) Mismatch (cmH2O)'], ax=g3.ax_marg_x, legend=False,
                color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Positive', 'OSI Mismatch (mmHg)'], ax=g3.ax_marg_y, vertical=True,
                legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Less Positive', 'OSI Mismatch (mmHg)'], ax=g3.ax_marg_y, vertical=True,
                legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Negative', 'OSI Mismatch (mmHg)'], ax=g3.ax_marg_y, vertical=True,
                legend=False,
                color='red', shade=True, gridsize=gridsize)

    # Save plots
    g0.savefig('g0.png')
    plt.close(g0.fig)

    g1.savefig('g1.png')
    plt.close(g1.fig)

    g2.savefig('g2.png')
    plt.close(g2.fig)

    g3.savefig('g3.png')
    plt.close(g3.fig)

    # Create subplots from saved plots
    f, axarr = plt.subplots(2, 2, figsize=(10, 10))
    #f.suptitle("Combined Outcomes: Patient 1 vs Patient 2 Clinical Measurements")

    axarr[0, 0].imshow(mpimg.imread('g0.png', format="png"))
    axarr[0, 1].imshow(mpimg.imread('g1.png', format="png"))
    axarr[1, 0].imshow(mpimg.imread('g3.png', format="png"))
    axarr[1, 1].imshow(mpimg.imread('g2.png', format="png"))

    # turn off x and y axis
    [ax.set_axis_off() for ax in axarr.ravel()]

    plt.tight_layout()

    plt.savefig('Multiplex_Outcome_Comparisons.png')


def plot_OSI_distribution(simulations):
    # This assumes there are 2 patients being compared

    O2SatIndex = []
    ImpairmentFraction = []

    for sim in simulations.Simulations:
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        p1 = sim.PatientComparisons[1].MultiplexVentilation

        O2SatIndex.append(p0.OxygenSaturationIndex_mmHg)
        O2SatIndex.append(p1.OxygenSaturationIndex_mmHg)
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
    plt.savefig('Multiplex_Oxygen_Saturation_Index_Distributions.png')


def plot_FiO2_distribution(simulations):
    FiO2 = []
    ImpairmentFraction = []

    for sim in simulations.Simulations:
        # Only look at patients that achieved SpO2 >=0.89
        if sim.AchievedStabilization:
            FiO2.append(sim.FiO2)
            ImpairmentFraction.append(sim.PatientComparisons[0].MultiplexVentilation.ImpairmentFraction)

    FiO2 = np.asarray(FiO2)
    ImpairmentFraction = np.asarray(ImpairmentFraction)

    numpy_data = np.array([ImpairmentFraction, FiO2])
    numpy_data = np.transpose(numpy_data)

    numpy_data = np.sort(numpy_data, 0)
    grouped_data = np.split(numpy_data[:, 1], np.cumsum(np.unique(numpy_data[:, 0], return_counts=True)[1])[:-1])
    uniqueValues, indicesList = np.unique(numpy_data[:, 0], return_index=True)
    uniqueValues = np.around(uniqueValues, decimals=1)

    fig, ax = plt.subplots()
    ax.boxplot(grouped_data, labels=uniqueValues)
    ax.yaxis.grid(True, linestyle='-', which='major', color='lightgrey', alpha=0.5)
    ax.set_title('Fraction of Inspired Oxygen Required to Achieve ~89% Oxygen Saturation')
    ax.set_xlabel('Diffusion Impairment Factor')
    ax.set_ylabel('Fraction of Inspired Oxygen')
    plt.savefig('Multiplex_Fraction_Inspired_Oxygen_Distributions.png')


def tabulate_FiO2_distribution(simulations):
    file = open('Multiplex_Inspired_Oxygen_Distribution.txt', 'w')
    all_combinations = []
    FiO2 = []
    ImpairmentFraction = []

    for sim in simulations.Simulations:
        compliance = sim.PatientComparisons[0].MultiplexVentilation.Compliance_mL_Per_cmH2O
        impairment_fraction = sim.PatientComparisons[0].MultiplexVentilation.ImpairmentFraction
        impairment_fraction = np.around(impairment_fraction, decimals=1)
        all_combinations.append(f"comp={compliance}_imp={impairment_fraction}")

        # Only look at patients that achieved SpO2 >=0.89
        if sim.AchievedStabilization:
            FiO2.append(sim.FiO2)
            ImpairmentFraction.append(impairment_fraction)

    unique_list = []
    for x in all_combinations:
        # check if exists in unique_list or not
        if x not in unique_list:
            unique_list.append(x)
    total_combinations = len(unique_list) # This should be 287 because 41 different compliances and 7 different impairments

    FiO2 = np.asarray(FiO2)
    ImpairmentFraction = np.asarray(ImpairmentFraction)

    numpy_data = np.array([ImpairmentFraction, FiO2])
    numpy_data = np.transpose(numpy_data)

    numpy_data = np.sort(numpy_data, 0)
    grouped_data = np.split(numpy_data[:, 1], np.cumsum(np.unique(numpy_data[:, 0], return_counts=True)[1])[:-1])
    uniqueValues, indicesList = np.unique(numpy_data[:, 0], return_index=True)
    uniqueValues = np.around(uniqueValues, decimals=1)

    print('Total patients = ', total_combinations)
    print('Total patients per impairment = ', total_combinations / 7) #jbw - Find the number of unique impairments better

    total_simulations = 258

    text = 'Patients that achieved >89% SpO2: '
    print(text)
    print(text, file=file)
    index = 0
    for DIF in uniqueValues:
        num_stable = len(grouped_data[index])
        percent_stable = num_stable / total_simulations * 100
        mean = np.mean(grouped_data[index])
        text = f"DIF = {DIF}: mean = {mean} for {percent_stable}%"
        print(text)
        print(text, file=file)
        index = index + 1

    file.close()


def map_parameter_correlation(numpy_continuous, names_continuous):
    # Heat map
    numpy_continuous = np.transpose(numpy_continuous)
    corr_df = pd.DataFrame(data=numpy_continuous, columns=names_continuous)

    corr = corr_df.corr(method='pearson')
    corr = corr.round(2)

    f, ax = plt.subplots(figsize=(10, 6))
    hm = sns.heatmap(round(corr, 2), annot=True, ax=ax, cmap="coolwarm", fmt='.2f', linewidths=.05, square=True)
    f.subplots_adjust(top=0.93)

    plt.tight_layout()

    f.savefig('Multiplex_Fraction_Inspired_Oxygen_Correlations.png')


def tabulate_outcome_correlation(df, names_continuous):
    name = []
    eta_squared = []
    file = open('Multiplex_Outcome_Correlations.txt', 'w')
    for continuous_variable in names_continuous:
        mod = ols("Q(continuous_variable) ~ C(Q('Combined Outcome'))", data=df).fit()
        # print(mod.summary())
        aov_table = sm.stats.anova_lm(mod, typ=2)
        aov = aov_table
        aov['mean_sq'] = aov[:]['sum_sq'] / aov[:]['df']
        aov['eta_sq'] = aov[:-1]['sum_sq'] / sum(aov['sum_sq'])
        aov['omega_sq'] = (aov[:-1]['sum_sq'] - (aov[:-1]['df'] * aov['mean_sq'][-1])) / (
                    sum(aov['sum_sq']) + aov['mean_sq'][-1])
        cols = ['sum_sq', 'df', 'mean_sq', 'F', 'PR(>F)', 'eta_sq', 'omega_sq']
        aov = aov[cols]
        pd.set_option('display.width', 1000)
        print(continuous_variable)
        print(continuous_variable, file=file)
        with pd.option_context('display.max_columns', None):
            print(aov, '\n')
            print(aov, file=file)
            print('\n', file=file)
        name.append(continuous_variable)
        eta_squared.append(aov['eta_sq'][0])
    numpy_data = np.array([name, eta_squared])
    numpy_data = np.transpose(numpy_data)
    numpy_data = numpy_data[numpy_data[:, 1].argsort()]
    print(numpy_data)
    print(numpy_data, file=file)
    file.close()


def tabulate_edge_cases(simulations):
    green_combinations = []
    yellow_combinations = []
    red_combinations = []

    for sim in simulations.Simulations:
        p0 = sim.PatientComparisons[0].MultiplexVentilation
        compliance = p0.Compliance_mL_Per_cmH2O
        impairment_fraction = p0.ImpairmentFraction
        impairment_fraction = np.around(impairment_fraction, decimals=1)
        p0_TV_mL_Per_kg = p0.TidalVolume_mL / p0.IdealBodyWeight_kg
        patient_string = f"comp={compliance}_imp={impairment_fraction}"
        if 6.5 >= p0_TV_mL_Per_kg >= 5.5: # Green criteria
            if sim.AchievedStabilization:
                green_combinations.append(patient_string)
            else:
                # Red based on impairment alone
                red_combinations.append(patient_string)
        elif 7.5 >= p0_TV_mL_Per_kg >= 4.5: # Yellow criteria
            if sim.AchievedStabilization:
                yellow_combinations.append(patient_string)
            else:
                # Red based on impairment alone
                red_combinations.append(patient_string)
        # else they're red from tidal volume

    unique_green_combinations = []
    for x in green_combinations:
        # check if exists in unique_list or not
        if x not in unique_green_combinations:
            unique_green_combinations.append(x)

    unique_yellow_combinations = []
    for x in yellow_combinations:
        # check if exists in unique_list or not
        if x not in unique_yellow_combinations:
            unique_yellow_combinations.append(x)

    unique_red_combinations = []
    for x in red_combinations:
        # check if exists in unique_list or not
        if x not in unique_red_combinations:
            unique_red_combinations.append(x)

    yellow_edge_cases = list(set(unique_red_combinations).intersection(yellow_combinations))
    yellow_edge_cases.sort()
    print('Potential missed yellows:', yellow_edge_cases)

    red_edge_cases = list(set(unique_red_combinations).intersection(green_combinations))
    red_edge_cases.sort()
    print('Potential missed greens:', red_edge_cases)


def plot_OSI_surface():
    num_increments = 10

    FiO2 = np.arange(0.21, 1.0, (1.0-0.21)/num_increments)
    SpO2_fraction = np.arange(0.5, 1.0, (1.0-0.5)/num_increments)
    SpO2_percent = SpO2_fraction * 100
    MAP_cmH2O = np.arange(0, 40, (40-0)/num_increments)
    MAP_mmHg = MAP_cmH2O * 0.735559

    x, y, z = np.meshgrid(FiO2, MAP_mmHg, SpO2_fraction, indexing='ij')
    OSI_mmHg = x * y / z

    # Use different units for plotting
    x, y, z = np.meshgrid(FiO2, MAP_cmH2O, SpO2_percent, indexing='ij')

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_title("Oxygen Saturation Index")
    ax.set_xlabel('FiO2')
    ax.set_ylabel('MAP (cmH2O)')
    ax.set_zlabel('SpO2 (%)')
    p = ax.scatter(x, y, z, c=OSI_mmHg.flatten(), cmap='coolwarm')
    fig.colorbar(p)

    plt.savefig('OSI_Visualization.png')

    # --------------------------------------------------------------
    # make these smaller to increase the resolution
    num_increments = 100
    dx = (1.0 - 0.21) / num_increments
    dy = (40 - 0) / num_increments

    FiO2 = np.arange(0.21, 1.0, (1.0 - 0.21) / num_increments)
    MAP_cmH2O = np.arange(0, 40, (40 - 0) / num_increments)
    MAP_mmHg = MAP_cmH2O * 0.735559

    # generate 2 2d grids for the x & y bounds
    x, y = np.meshgrid(FiO2, MAP_mmHg)
    z = x * y / 0.9
    x, y = np.meshgrid(FiO2, MAP_cmH2O)

    # x and y are bounds, so z should be the value *inside* those bounds.
    # Therefore, remove the last value from the z array.
    z = z[:-1, :-1]
    levels = MaxNLocator(nbins=15).tick_values(z.min(), z.max())

    cmap = plt.get_cmap('coolwarm')
    fig, (ax0, ax1) = plt.subplots(ncols=2)

    # contours are *point* based plots, so convert our bound into point
    # centers
    cf = ax0.contourf(x[:-1, :-1] + dx / 2.,
                      y[:-1, :-1] + dy / 2., z, levels=levels,
                      cmap=cmap)
    fig.colorbar(cf, ax=ax0)
    ax0.set_title('SpO2 = 90%')
    ax0.set_xlabel('FiO2')
    ax0.set_ylabel('MAP (cmH2O)')
    #ax0.contour(x[:-1, :-1], y[:-1, :-1], z, colors='k')

    x, y = np.meshgrid(FiO2, MAP_mmHg)
    z = x * y / 0.95
    x, y = np.meshgrid(FiO2, MAP_cmH2O)

    # x and y are bounds, so z should be the value *inside* those bounds.
    # Therefore, remove the last value from the z array.
    z = z[:-1, :-1]

    # contours are *point* based plots, so convert our bound into point
    # centers
    cf = ax1.contourf(x[:-1, :-1] + dx / 2.,
                      y[:-1, :-1] + dy / 2., z, levels=levels,
                      cmap=cmap)
    fig.colorbar(cf, ax=ax1)
    ax1.set_title('SpO2 = 95%')
    ax1.set_xlabel('FiO2')
    ax1.set_ylabel('MAP (cmH2O)')

    fig.suptitle('OSI (mmHg)', fontsize=14)

    plt.savefig('OSI_Visualization_SpO2_90.png')


if __name__ == '__main__':
    # Load up a result set
    paired_results_file = "./test_results/multiplex_ventilation/simulations/multiplex_simlist_results.json"
    solo_results_file = "./test_results/multiplex_ventilation/simulations/solo_simlist_results.json"

    with open(paired_results_file) as f:
        json = f.read()
    results = SimulationListData()
    json_format.Parse(json, results)

    with open(solo_results_file) as solo_f:
        solo_json = solo_f.read()
    solo_results = SimulationListData()
    json_format.Parse(solo_json, solo_results)

    tabulate_FiO2_distribution(solo_results)
    plot_FiO2_distribution(solo_results)
    tabulate_edge_cases(solo_results)

    plot_OSI_distribution(results)

    x, y, z, df, numpy_continuous, names_continuous, names_categorical = parse_data(results)
    tabulate_outcome_correlation(df, names_continuous)
    map_parameter_correlation(numpy_continuous, names_continuous)
    plot_outcome_comparisons(x, y, z, df)
    plot_interpolation(x, y, z)

    # plot_OSI_surface()

    plt.show()