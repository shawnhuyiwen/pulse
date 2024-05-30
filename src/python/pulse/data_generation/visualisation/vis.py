import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
sim_patient_response_df = pd.read_csv("clean_data.csv")

# Extract data from the DataFrame
time_steps = sim_patient_response_df['Time(s)'] / 60  # Convert time to minutes
systolic_pressure = sim_patient_response_df['SystolicArterialPressure(mmHg)']
diastolic_pressure = sim_patient_response_df['DiastolicArterialPressure(mmHg)']
heart_rate = sim_patient_response_df['HeartRate(1/min)']
plasma_concentration = sim_patient_response_df['Propofol-PlasmaConcentration(ug/L)']

# Smooth the systolic and diastolic pressures using a rolling mean
window_size = 5
smoothed_systolic_pressure = systolic_pressure.rolling(window_size, min_periods=1).mean()
smoothed_diastolic_pressure = diastolic_pressure.rolling(window_size, min_periods=1).mean()
smoothed_heart_rate = heart_rate.rolling(window_size, min_periods=1).mean()

# Define base values (for example purposes)
base_systolic_pressure = systolic_pressure.iloc[0]
base_diastolic_pressure = diastolic_pressure.iloc[0]
base_heart_rate = heart_rate.iloc[0]
base_plasma_concentration = plasma_concentration.iloc[0]

# Define a consistent color palette for each subplot
colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']

# Create a smaller figure with subplots
fig, axes = plt.subplots(2, 2, figsize=(10, 8), sharex=True)

# Plot plasma concentration
axes[0, 0].plot(time_steps, plasma_concentration, label='Plasma Concentration', color=colors[0], alpha=0.8)
axes[0, 0].axhline(y=base_plasma_concentration, color=colors[2], linestyle='--', label='Base Plasma Concentration')
axes[0, 0].set_ylabel('Plasma Concentration (ug/L)')
axes[0, 0].set_title('Plasma Concentration Over Time')
axes[0, 0].legend(loc='upper left')
# Add vertical lines
axes[0, 0].axvline(x=10, color='gray', linestyle='--', alpha=0.5)
axes[0, 0].axvline(x=25, color='gray', linestyle='--', alpha=0.5)
axes[0, 0].axvline(x=40, color='gray', linestyle='--', alpha=0.5)

# Plot heart rate and smoothed heart rate
axes[0, 1].plot(time_steps, heart_rate, label='Heart Rate', color=colors[0], alpha=0.8)
axes[0, 1].plot(time_steps, smoothed_heart_rate, label='Smoothed Heart Rate', color=colors[1])
axes[0, 1].axhline(y=base_heart_rate, color=colors[2], linestyle='--', label='Base Heart Rate')
axes[0, 1].set_ylabel('Heart Rate (1/min)')
axes[0, 1].set_title('Heart Rate Over Time')
axes[0, 1].legend(loc='lower left')
# Add vertical lines
axes[0, 1].axvline(x=10, color='gray', linestyle='--', alpha=0.5)
axes[0, 1].axvline(x=25, color='gray', linestyle='--', alpha=0.5)
axes[0, 1].axvline(x=40, color='gray', linestyle='--', alpha=0.5)

# Plot systolic arterial pressure and smoothed pressure
axes[1, 0].plot(time_steps, systolic_pressure, label='Systolic Arterial Pressure', color=colors[0], alpha=0.8)
axes[1, 0].plot(time_steps, smoothed_systolic_pressure, label='Smoothed Systolic Pressure', color=colors[1])
axes[1, 0].axhline(y=base_systolic_pressure, color=colors[2], linestyle='--', label='Base Systolic Pressure')
axes[1, 0].set_xlabel('Time (min)')
axes[1, 0].set_ylabel('Systolic Pressure (mmHg)')
axes[1, 0].set_title('Systolic Arterial Pressure Over Time')
axes[1, 0].legend(loc='lower left')
# Add vertical lines
axes[1, 0].axvline(x=10, color='gray', linestyle='--', alpha=0.5)
axes[1, 0].axvline(x=25, color='gray', linestyle='--', alpha=0.5)
axes[1, 0].axvline(x=40, color='gray', linestyle='--', alpha=0.5)

# Plot diastolic arterial pressure and smoothed pressure
axes[1, 1].plot(time_steps, diastolic_pressure, label='Diastolic Arterial Pressure', color=colors[0], alpha=0.8)
axes[1, 1].plot(time_steps, smoothed_diastolic_pressure, label='Smoothed Diastolic Pressure', color=colors[1])
axes[1, 1].axhline(y=base_diastolic_pressure, color=colors[2], linestyle='--', label='Base Diastolic Pressure')
axes[1, 1].set_xlabel('Time (min)')
axes[1, 1].set_ylabel('Diastolic Pressure (mmHg)')
axes[1, 1].set_title('Diastolic Arterial Pressure Over Time')
axes[1, 1].legend(loc='lower left')
# Add vertical lines
axes[1, 1].axvline(x=10, color='gray', linestyle='--', alpha=0.5)
axes[1, 1].axvline(x=25, color='gray', linestyle='--', alpha=0.5)
axes[1, 1].axvline(x=40, color='gray', linestyle='--', alpha=0.5)

# Adjust layout and display the plot
plt.tight_layout()
plt.show()
