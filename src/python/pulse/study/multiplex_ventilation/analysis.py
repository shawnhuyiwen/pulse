# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.study.multiplex_ventilation.bind.MultiplexVentilation_pb2 import *

import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import pandas as pd
import statsmodels.api as sm
import seaborn as sns

from google.protobuf import json_format
from scipy import stats
from scipy import interpolate
from statsmodels.formula.api import ols


def parse_data(simulations):
    # This assumes there are 2 patients being compared

    # Continuous patient variables
    names_continuous = []
    
    delta_compliance = []
    names_continuous.append("C(stat) Mismatch (cmH2O)")
    delta_O2SatIndex = []
    names_continuous.append("OSI Mismatch (cmH2O)")
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

    for sim in simulations.Simulations:
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
            delta_compliance.append(abs(p0.Compliance_mL_Per_cmH2O - p1.Compliance_mL_Per_cmH2O))
            delta_O2SatIndex.append(abs(p0.OxygenSaturationIndex_cmH2O - p1.OxygenSaturationIndex_cmH2O))
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

    numpy_continuous = np.array([delta_compliance, delta_O2SatIndex, detla_MeanAirwayPressure_cmH2O, delta_ImpairmentFraction,
                           delta_CarricoIndex_mmHg, delta_SFRatio, delta_OxygenationIndex,
                           delta_EndTidalCarbonDioxidePressure_mmHg, delta_AlveolarArterialGradient_mmHg])
    numpy_continuous = numpy_continuous.astype(float)
    numpy_data = np.concatenate((numpy_continuous, [combined_outcome, tv_outcome, SpO2_outcome, PaO2_outcome]))
    numpy_data = np.transpose(numpy_data)
    names = names_continuous + names_categorical
    df = pd.DataFrame(data=numpy_data, columns=names)
    df[names_continuous] = df[names_continuous].astype(float)

    delta_compliance = np.asarray(delta_compliance)
    delta_O2SatIndex = np.asarray(delta_O2SatIndex)
    z_norm = np.asarray(z_norm)

    return delta_compliance, delta_O2SatIndex, z_norm, df, numpy_continuous, names_continuous, names_categorical


def Interpolate(x, y, z):
    x_bound = 22
    y_bound = 6
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

    plt.savefig('Interpolation.png')


def Plot(x, y, z, df):
    x_bound = 22
    y_bound = 6
    x_label = "Respiratory Compliance Mismatch (mL/cmH2O)"
    y_label = "Oxygen Saturation Index Mismatch (mmHg)"

    grid0 = sns.JointGrid(x='C(stat) Mismatch (cmH2O)', y='OSI Mismatch (cmH2O)', data=df, xlim=(0, x_bound), ylim=(0, y_bound))
    g0 = grid0.plot_joint(sns.scatterplot, marker=1, s=150, hue='Combined Outcome', hue_order=['Positive', 'Less Positive', 'Negative'],  data=df, palette=['green', 'yellow', 'red'])
    g0.set_axis_labels(x_label, y_label)
    g0.fig.suptitle("Combined Bounds Outcome")

    gridsize = 100

    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Positive', 'C(stat) Mismatch (cmH2O)'], ax=g0.ax_marg_x, legend=False, color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Less Positive', 'C(stat) Mismatch (cmH2O)'], ax=g0.ax_marg_x, legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Negative', 'C(stat) Mismatch (cmH2O)'], ax=g0.ax_marg_x, legend=False, color='red', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Positive', 'OSI Mismatch (cmH2O)'], ax=g0.ax_marg_y, vertical=True, legend=False, color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Less Positive', 'OSI Mismatch (cmH2O)'], ax=g0.ax_marg_y, vertical=True, legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['Combined Outcome'] == 'Negative', 'OSI Mismatch (cmH2O)'], ax=g0.ax_marg_y, vertical=True, legend=False, color='red', shade=True, gridsize=gridsize)

    grid1 = sns.JointGrid(x='C(stat) Mismatch (cmH2O)', y='OSI Mismatch (cmH2O)', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g1 = grid1.plot_joint(sns.scatterplot, marker=1, s=150, hue='TV Outcome', hue_order=['Positive', 'Less Positive', 'Negative'], data=df, palette=['green', 'yellow', 'red'])
    g1.set_axis_labels(x_label, y_label)
    g1.fig.suptitle("TV Bounds Outcome")

    sns.kdeplot(df.loc[df['TV Outcome'] == 'Positive', 'C(stat) Mismatch (cmH2O)'], ax=g1.ax_marg_x, legend=False, color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Less Positive', 'C(stat) Mismatch (cmH2O)'], ax=g1.ax_marg_x, legend=False,
                color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Negative', 'C(stat) Mismatch (cmH2O)'], ax=g1.ax_marg_x, legend=False, color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Positive', 'OSI Mismatch (cmH2O)'], ax=g1.ax_marg_y, vertical=True, legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Less Positive', 'OSI Mismatch (cmH2O)'], ax=g1.ax_marg_y, vertical=True,
                legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['TV Outcome'] == 'Negative', 'OSI Mismatch (cmH2O)'], ax=g1.ax_marg_y, vertical=True, legend=False,
                color='red', shade=True, gridsize=gridsize)

    grid2 = sns.JointGrid(x='C(stat) Mismatch (cmH2O)', y='OSI Mismatch (cmH2O)', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g2 = grid2.plot_joint(sns.scatterplot, marker=1, s=150, hue='SpO2 Outcome', hue_order=['Positive', 'Negative'], data=df, palette=['green', 'red'])
    g2.set_axis_labels(x_label, y_label)
    g2.fig.suptitle("SpO2 Bounds Outcome")

    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Positive', 'C(stat) Mismatch (cmH2O)'], ax=g2.ax_marg_x, legend=False,
                color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Negative', 'C(stat) Mismatch (cmH2O)'], ax=g2.ax_marg_x, legend=False, color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Positive', 'OSI Mismatch (cmH2O)'], ax=g2.ax_marg_y, vertical=True,
                legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['SpO2 Outcome'] == 'Negative', 'OSI Mismatch (cmH2O)'], ax=g2.ax_marg_y, vertical=True,
                legend=False,
                color='red', shade=True, gridsize=gridsize)

    grid3 = sns.JointGrid(x='C(stat) Mismatch (cmH2O)', y='OSI Mismatch (cmH2O)', data=df, xlim=(0, x_bound),
                          ylim=(0, y_bound))
    g3 = grid3.plot_joint(sns.scatterplot, marker=1, s=150, hue='PaO2 Outcome', hue_order=['Positive', 'Less Positive', 'Negative'], data=df, palette=['green', 'yellow', 'red'])
    g3.set_axis_labels(x_label, y_label)
    g3.fig.suptitle("PaO2 Bounds Outcome")

    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Positive', 'C(stat) Mismatch (cmH2O)'], ax=g3.ax_marg_x, legend=False,
                color='green',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Less Positive', 'C(stat) Mismatch (cmH2O)'], ax=g3.ax_marg_x, legend=False,
                color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Negative', 'C(stat) Mismatch (cmH2O)'], ax=g3.ax_marg_x, legend=False, color='red',
                shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Positive', 'OSI Mismatch (cmH2O)'], ax=g3.ax_marg_y, vertical=True,
                legend=False,
                color='green', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Less Positive', 'OSI Mismatch (cmH2O)'], ax=g3.ax_marg_y, vertical=True,
                legend=False, color='yellow', shade=True, gridsize=gridsize)
    sns.kdeplot(df.loc[df['PaO2 Outcome'] == 'Negative', 'OSI Mismatch (cmH2O)'], ax=g3.ax_marg_y, vertical=True,
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


def plot_OSI_distribution(simulations):
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
    plt.savefig('FiO2_Distribution.png')


def tabulate_FiO2_distribution(simulations):
    total_simulations = 258  # This assumes 258 patients per impairment

    file = open('InspiredOxygenDistribution.txt', 'w')
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

    text = 'Patients that achieved >89% SpO2: '
    print(text)
    print(text, file=file)
    index = 0
    for DIF in uniqueValues:
        num_stable = len(grouped_data[index])
        percent_stable = num_stable / total_simulations * 100
        mean = np.mean(grouped_data[index])
        text = f"DIF = {DIF}: mean = {mean} for {num_stable} / {total_simulations} ({percent_stable}%)"
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

    mask = np.zeros_like(corr, dtype=np.bool)
    mask[np.triu_indices_from(mask)] = True
    f, ax = plt.subplots(figsize=(15, 15))
    heatmap = sns.heatmap(corr, mask=mask, square=True, linewidths=.5, cmap='coolwarm',
                          cbar_kws={'shrink': .5, 'ticks': [-1, -.5, 0, 0.5, 1]}, vmin=-1, vmax=1, annot=True,
                          annot_kws={'size': 22})
    # add the column names as labels
    ax.set_yticklabels(corr.columns, rotation=0)
    ax.set_xticklabels(corr.columns)
    sns.set_style({'xtick.bottom': True}, {'ytick.left': True})

    plt.tight_layout()

    f.savefig('FiO2Correlations.png')


def tabulate_outcome_correlation(df, names_continuous):
    name = []
    eta_squared = []
    file = open('OutcomeCorrelations.txt', 'w')
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


if __name__ == '__main__':
    # Load up a result set
    results_file = "./test_results/multiplex_ventilation/simulations/simlist_results_Fix.json"
    solo_results_file = "./test_results/multiplex_ventilation/simulations/solo_simlist_results.json"

    with open(results_file) as f:
        json = f.read()
    results = SimulationListData()
    json_format.Parse(json, results)

    with open(solo_results_file) as solo_f:
        solo_json = solo_f.read()
    solo_results = SimulationListData()
    json_format.Parse(solo_json, solo_results)

    tabulate_FiO2_distribution(solo_results)
    plot_FiO2_distribution(solo_results)
    plot_OSI_distribution(results)

    x, y, z, df, numpy_continuous, names_continuous, names_categorical = parse_data(results)
    tabulate_outcome_correlation(df, names_continuous)
    map_parameter_correlation(numpy_continuous, names_continuous)
    Plot(x, y, z, df)
    Interpolate(x, y, z)
    plt.show()