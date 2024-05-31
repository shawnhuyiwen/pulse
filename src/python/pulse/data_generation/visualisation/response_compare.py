import matplotlib.pyplot as plt
import pandas as pd

# File paths
IN_STAND_DIR = "standard_male_clean.csv"
IN_OVER_DIR = "overweight_male_clean.csv"
IN_UNDER_DIR = "underweight_male_clean.csv"

# Read the CSV files
standard_male_df = pd.read_csv(IN_STAND_DIR)
overweight_male_df = pd.read_csv(IN_OVER_DIR)
underweight_male_df = pd.read_csv(IN_UNDER_DIR)

# Extract data from the DataFrame
time_steps = standard_male_df['Time(s)'] / 60  # Convert time to minutes
systolic_pressure_standard = standard_male_df['SystolicArterialPressure(mmHg)']
diastolic_pressure_standard = standard_male_df['DiastolicArterialPressure(mmHg)']
heart_rate_standard = standard_male_df['HeartRate(1/min)']
plasma_concentration_standard = standard_male_df['Propofol-PlasmaConcentration(ug/L)']

systolic_pressure_overweight = overweight_male_df['SystolicArterialPressure(mmHg)']
diastolic_pressure_overweight = overweight_male_df['DiastolicArterialPressure(mmHg)']
heart_rate_overweight = overweight_male_df['HeartRate(1/min)']
plasma_concentration_overweight = overweight_male_df['Propofol-PlasmaConcentration(ug/L)']

systolic_pressure_underweight = underweight_male_df['SystolicArterialPressure(mmHg)']
diastolic_pressure_underweight = underweight_male_df['DiastolicArterialPressure(mmHg)']
heart_rate_underweight = underweight_male_df['HeartRate(1/min)']
plasma_concentration_underweight = underweight_male_df['Propofol-PlasmaConcentration(ug/L)']

# Smooth the pressures and heart rates using a rolling mean
window_size = 5
smoothed_systolic_pressure_standard = systolic_pressure_standard.rolling(window_size, min_periods=1).mean()
smoothed_diastolic_pressure_standard = diastolic_pressure_standard.rolling(window_size, min_periods=1).mean()
smoothed_heart_rate_standard = heart_rate_standard.rolling(window_size, min_periods=1).mean()

smoothed_systolic_pressure_overweight = systolic_pressure_overweight.rolling(window_size, min_periods=1).mean()
smoothed_diastolic_pressure_overweight = diastolic_pressure_overweight.rolling(window_size, min_periods=1).mean()
smoothed_heart_rate_overweight = heart_rate_overweight.rolling(window_size, min_periods=1).mean()

smoothed_systolic_pressure_underweight = systolic_pressure_underweight.rolling(window_size, min_periods=1).mean()
smoothed_diastolic_pressure_underweight = diastolic_pressure_underweight.rolling(window_size, min_periods=1).mean()
smoothed_heart_rate_underweight = heart_rate_underweight.rolling(window_size, min_periods=1).mean()

# Define base values (for example purposes)
base_systolic_pressure_standard = systolic_pressure_standard.iloc[0]
base_diastolic_pressure_standard = diastolic_pressure_standard.iloc[0]
base_heart_rate_standard = heart_rate_standard.iloc[0]
base_plasma_concentration_standard = plasma_concentration_standard.iloc[0]

base_systolic_pressure_overweight = systolic_pressure_overweight.iloc[0]
base_diastolic_pressure_overweight = diastolic_pressure_overweight.iloc[0]
base_heart_rate_overweight = heart_rate_overweight.iloc[0]
base_plasma_concentration_overweight = plasma_concentration_overweight.iloc[0]

base_systolic_pressure_underweight = systolic_pressure_underweight.iloc[0]
base_diastolic_pressure_underweight = diastolic_pressure_underweight.iloc[0]
base_heart_rate_underweight = heart_rate_underweight.iloc[0]
base_plasma_concentration_underweight = plasma_concentration_underweight.iloc[0]

# Define a consistent color palette for each subplot
colors_standard = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
colors_overweight = ['#9467bd', '#8c564b', '#e377c2', '#7f7f7f']
colors_underweight = ['#bcbd22', '#17becf', '#9edae5', '#f7b6d2']

# Create a smaller figure with subplots
fig, axes = plt.subplots(2, 2, figsize=(12, 10), sharex=True)

# Plot plasma concentration
axes[0, 0].plot(time_steps, plasma_concentration_standard, label='Standard', color=colors_standard[1], alpha=0.8)
axes[0, 0].plot(time_steps, plasma_concentration_overweight, label='Overweight', color=colors_overweight[1], alpha=0.8)
axes[0, 0].plot(time_steps, plasma_concentration_underweight, label='Underweight', color=colors_underweight[1], alpha=0.8)
axes[0, 0].axhline(y=base_plasma_concentration_standard, color=colors_standard[2], linestyle='--', label='Base Plasma Concentration')
# Add vertical lines
axes[0, 0].axvline(x=10, color='gray', linestyle='--', alpha=0.5)
axes[0, 0].axvline(x=25, color='gray', linestyle='--', alpha=0.5)
axes[0, 0].axvline(x=40, color='gray', linestyle='--', alpha=0.5)
axes[0, 0].set_ylabel('Plasma Concentration (ug/L)')
axes[0, 0].set_title('Plasma Concentration Over Time')
axes[0, 0].legend(loc='upper left')

# Plot heart rate and smoothed heart rate
axes[0, 1].plot(time_steps, smoothed_heart_rate_standard, label='Standard', color=colors_standard[1])
axes[0, 1].plot(time_steps, smoothed_heart_rate_overweight, label='Overweight', color=colors_overweight[1])
axes[0, 1].plot(time_steps, smoothed_heart_rate_underweight, label='Underweight', color=colors_underweight[1])

# axes[0, 1].plot(time_steps, heart_rate_standard, label='Standard', color=colors_standard[1])
# axes[0, 1].plot(time_steps, heart_rate_overweight, label='Overweight', color=colors_overweight[1])
# axes[0, 1].plot(time_steps, heart_rate_underweight, label='Underweight', color=colors_underweight[1])

axes[0, 1].axhline(y=base_heart_rate_standard, color=colors_standard[2], linestyle='--', label='Base Heart Rate')
# Add vertical lines
axes[0, 1].axvline(x=10, color='gray', linestyle='--', alpha=0.5)
axes[0, 1].axvline(x=25, color='gray', linestyle='--', alpha=0.5)
axes[0, 1].axvline(x=40, color='gray', linestyle='--', alpha=0.5)
axes[0, 1].set_ylabel('Heart Rate (1/min)')
axes[0, 1].set_title('Heart Rate Over Time')
axes[0, 1].legend(loc='lower left')

# Plot systolic arterial pressure and smoothed pressure
axes[1, 0].plot(time_steps, smoothed_systolic_pressure_standard, label='Standard', color=colors_standard[1])
axes[1, 0].plot(time_steps, smoothed_systolic_pressure_overweight, label='Overweight', color=colors_overweight[1])
axes[1, 0].plot(time_steps, smoothed_systolic_pressure_underweight, label='Underweight', color=colors_underweight[1])

# axes[1, 0].plot(time_steps, systolic_pressure_standard, label='Standard', color=colors_standard[1])
# axes[1, 0].plot(time_steps, systolic_pressure_overweight, label='Overweight', color=colors_overweight[1])
# axes[1, 0].plot(time_steps, systolic_pressure_underweight, label='Underweight', color=colors_underweight[1])

axes[1, 0].axhline(y=base_systolic_pressure_standard, color=colors_standard[2], linestyle='--', label='Base Systolic Pressure')
# Add vertical lines
axes[1, 0].axvline(x=10, color='gray', linestyle='--', alpha=0.5)
axes[1, 0].axvline(x=25, color='gray', linestyle='--', alpha=0.5)
axes[1, 0].axvline(x=40, color='gray', linestyle='--', alpha=0.5)
axes[1, 0].set_xlabel('Time (min)')
axes[1, 0].set_ylabel('Systolic Pressure (mmHg)')
axes[1, 0].set_title('Systolic Arterial Pressure Over Time')
axes[1, 0].legend(loc='lower left')

# Plot diastolic arterial pressure and smoothed pressure
axes[1, 1].plot(time_steps, smoothed_diastolic_pressure_standard, label='Standard', color=colors_standard[1])
axes[1, 1].plot(time_steps, smoothed_diastolic_pressure_overweight, label='Overweight', color=colors_overweight[1])
axes[1, 1].plot(time_steps, smoothed_diastolic_pressure_underweight, label='Underweight', color=colors_underweight[1])

# axes[1, 1].plot(time_steps, diastolic_pressure_standard, label='Standard', color=colors_standard[1])
# axes[1, 1].plot(time_steps, diastolic_pressure_overweight, label='Overweight', color=colors_overweight[1])
# axes[1, 1].plot(time_steps, diastolic_pressure_underweight, label='Underweight', color=colors_underweight[1])

axes[1, 1].axhline(y=base_diastolic_pressure_standard, color=colors_standard[2], linestyle='--', label='Base Diastolic Pressure')
# Add vertical lines
axes[1, 1].axvline(x=10, color='gray', linestyle='--', alpha=0.5)
axes[1, 1].axvline(x=25, color='gray', linestyle='--', alpha=0.5)
axes[1, 1].axvline(x=40, color='gray', linestyle='--', alpha=0.5)
axes[1, 1].set_xlabel('Time (min)')
axes[1, 1].set_ylabel('Diastolic Pressure (mmHg)')
# axes[1, 1].set_title('Smoothed Diastolic Arterial Pressure Over Time')
axes[1, 1].set_title('Diastolic Arterial Pressure Over Time')
axes[1, 1].legend(loc='lower left')

# Adjust layout and display the plot
plt.tight_layout()
plt.show()
