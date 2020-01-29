# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
import PyPulse
from pulse.cdm.engine import SEAction, eSerializationFormat
from pulse.cdm.io import SerializeActions

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

    def advance_time(self):
        return self.__pulse.advance_timestep()

    def advance_time_s(self, duration_s: float):
        for n in range(500):
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
        json = SerializeActions(actions,eSerializationFormat.JSON)
        print(json)
        self.__pulse.process_actions(json,PyPulse.serialization_format.json)


