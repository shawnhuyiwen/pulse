# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import pandas as pd

def read_csv_into_df(csv_filename: str, replace_slashes: bool=False):
    df = pd.read_csv(csv_filename)
    for column in df.columns[1:]:
        # Convert any strings to NaN
        df[column] = pd.to_numeric(df[column], errors='coerce')
        # Replace slashes in units string
        if replace_slashes:
            df.rename(columns={column: column.replace("/", "_Per_")}, inplace=True)

    return df

def compute_means(csv_filename: str, headers: [str], start_row=0, end_row=-1):
    means = []

    # Load up the csv file, pull the headers, and compute the mean of values from start to end
    df = read_csv_into_df(csv_filename).loc[start_row:end_row]
    means = df[headers].mean().values.tolist()

    return means
