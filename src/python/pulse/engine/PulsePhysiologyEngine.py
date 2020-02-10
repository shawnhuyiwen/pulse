# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
import PyPulse
from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.engine import SEAction, eSerializationFormat
from pulse.cdm.io import serialize_actions_to_string
from pulse.cdm import io

class PulsePhysiologyEngine:
    __slots__ = ['__pulse', "results"]

    def __init__(self, log_file="", write_to_console=True, data_root="."):
        self.results = {}
        self.__pulse = PyPulse.Engine(log_file, write_to_console, data_root)

    def serialize_from_file(self, state_file: str, data_requests, format: eSerializationFormat, start_time: float=0):
        # Process requests and setup our results structure
        self.process_requests(data_requests)
        fmt = PyPulse.serialization_format.json
        if format == eSerializationFormat.BINARY:
            fmt = PyPulse.serialization_format.binary
        return self.__pulse.serialize_from_file(state_file, "", fmt, start_time)

    def initialize_engine(self, patient_configuration: SEPatientConfiguration, data_requests):
        # Process requests and setup our results structure
        self.process_requests(data_requests)
        pc = io.serialize_patient_configuration_to_string(patient_configuration, eSerializationFormat.JSON)
        return self.__pulse.initialize_engine(pc, "", PyPulse.serialization_format.json )

    def advance_time(self):
        return self.__pulse.advance_timestep()

    def advance_time_s(self, duration_s: float):
        # TODO this is assuming duration_s is a factor of 0.02
        num_steps = int(duration_s / 0.02)
        for n in range(num_steps):
            self.__pulse.advance_timestep()

    def pull_data(self):
        values = self.__pulse.pull_data()
        for i, key in enumerate(self.results.keys()):
            self.results[key] = values[i]
        return self.results

    def process_requests(self, data_requests):
        if data_requests is None:
            self.results["SimulationTime(s)"]=0
            self.results["Lead3ElectricPotential(mV)"]=0
            self.results["HeartRate(bpm)"]=0
            self.results["ArterialPressure(mmHg)"]=0
            self.results["MeanArterialPressure(mmHg)"]=0
            self.results["SystolicArterialPressure(mmHg)"]=0
            self.results["DiastolicArterialPressure(mmHg)"]=0
            self.results["OxygenSaturation"]=0
            self.results["EndTidalCarbonDioxidePressure(mmHg)"]=0
            self.results["RespirationRate(bpm)"]=0
            self.results["CoreTemperature(C)"]=0
            self.results["CarinaCO2PartialPressure(mmHg)"]=0
            self.results["BloodVolume(mL)"]=0

    def process_action(self, action: SEAction):
        actions = [action]
        self.process_actions(actions)

    def process_actions(self, actions: []):
        json = serialize_actions_to_string(actions,eSerializationFormat.JSON)
        print(json)
        self.__pulse.process_actions(json,PyPulse.serialization_format.json)


