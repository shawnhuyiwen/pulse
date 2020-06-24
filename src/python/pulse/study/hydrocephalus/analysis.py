# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import math
from pulse.study.bind.Hydrocephalus_pb2 import *
from google.protobuf import json_format

def do_something_interesting(simulations):
    # This is a simple sanity check (Can be removed)
    for sim in simulations.Simulation:
        if not math.isclose(sim.IntracranialSpaceVolumeBaseline_mL, sim.IntracranialSpaceVolume_mL):
            print("Simulation "+str(sim.ID)+" volumes do not match ")
        if not sim.AchievedStabilization:
            print("Simulation "+str(sim.ID)+" did not stabilize")

if __name__ == '__main__':
    # Load up a result set
    results_file = "./test_results/hydrocephalus/simlist_results.json"

    with open(results_file) as f:
        json = f.read()
    results = SimulationListData()
    json_format.Parse(json, results)

    do_something_interesting(results)
