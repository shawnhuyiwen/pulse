import matplotlib.pyplot as plt
import pandas as pd

IN_DIR = "underweight_male_sim.csv"
OUT_DIR = "underweight_male_clean.csv"

sim_patient_response_df = pd.read_csv(IN_DIR)

a = []
for i in range(0, len(sim_patient_response_df),500):
    a.append(i)
file = sim_patient_response_df.iloc[a]
f = pd.DataFrame(file)
f.to_csv(OUT_DIR, index=False, encoding='utf_8_sig')

print('Cleaning done!')
