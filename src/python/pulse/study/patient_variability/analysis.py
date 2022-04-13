from pulse.study.bind.PatientVariability_pb2 import *
from google.protobuf import json_format

from os.path import exists

if __name__ == '__main__':
    # Load up result set
    results = PatientStateListData()
    results_file = "./test_results/patient_variability/patient_results.json"
    if not exists(results_file):
        results_file = "./test_results/patient_variability/patient_results.pbb"
        with open(results_file, "rb") as f:
            binary = f.read()
        results.ParseFromString(binary)
    else:
        with open(results_file) as f:
            json = f.read()
        json_format.Parse(json, results)
