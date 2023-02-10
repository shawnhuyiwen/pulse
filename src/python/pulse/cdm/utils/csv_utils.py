# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

def compute_means(csv_filename: str, headers: [str], start_time_s=0, end_time_s=-1):
    means = []

    # Load up the csv file, pull the headers, and compute the mean of values from start to end

    i = 0
    for header in headers:
        means.append(i)
        i = i + 1

    return means