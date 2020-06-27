# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import PyPulse
from pulse.cdm.engine import SEAction, eSerializationFormat
from pulse.cdm.io.engine import serialize_actions_to_string

from pulse.study.sensitivity_analysis.bind.SensitivityAnalysis_pb2 import *
from google.protobuf import json_format

class MVEngine:
    __slots__ = ['__pulse']

    def __init__(self, log_file="", write_to_console=True, data_root="."):
        self.__pulse = PyPulse.MVEngine(log_file, write_to_console, data_root)

    def create_engine(self, simulation: SimulationData):
        # Process requests and setup our results structure
        out = json_format.MessageToJson(simulation)
        return self.__pulse.create_engine(out, PyPulse.serialization_format.json)

    def advance_time_s(self, duration_s: float, sim: SimulationData):
        num_steps = int(duration_s / 0.02)
        for n in range(num_steps):
            if not self.__pulse.advance_time(0.02):
                return False
        json = self.__pulse.get_patient_state(PyPulse.serialization_format.json)
        json_format.Parse(json, sim)
        return True

    def process_action(self, action: SEAction):
        actions = [action]
        self.process_actions(actions)

    def process_actions(self, actions: []):
        json = serialize_actions_to_string(actions,eSerializationFormat.JSON)
        #print(json)
        self.__pulse.process_actions(json,PyPulse.serialization_format.json)

    def destroy(self):
        self.__pulse.destroy_engine()
