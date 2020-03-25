# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
import PyPulse
from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.engine import SEAction, eSerializationFormat, SEDataRequestManager, SEDataRequest
from pulse.cdm.io.engine import serialize_actions_to_string, \
                                serialize_patient_configuration_to_string, \
                                serialize_data_request_manager_to_string

class PulsePhysiologyEngine:
    __slots__ = ['__pulse', "results"]

    def __init__(self, log_file="", write_to_console=True, data_root="."):
        self.results = {}
        self.__pulse = PyPulse.Engine(log_file, write_to_console, data_root)

    def serialize_from_file(self, state_file: str, data_request_mgr: SEDataRequestManager, format: eSerializationFormat,
                            start_time: float = 0):
        # Process requests and setup our results structure
        drm = self.process_requests(data_request_mgr, format)
        if format == eSerializationFormat.BINARY:
            fmt = PyPulse.serialization_format.binary
        else:
            fmt = PyPulse.serialization_format.json
        return self.__pulse.serialize_from_file(state_file, drm, fmt, start_time)

    def initialize_engine(self, patient_configuration: SEPatientConfiguration, data_request_mgr: SEDataRequestManager):
        # Process requests and setup our results structure
        drm = self.process_requests(data_request_mgr, eSerializationFormat.JSON)
        pc = serialize_patient_configuration_to_string(patient_configuration, eSerializationFormat.JSON)
        return self.__pulse.initialize_engine(pc, drm, PyPulse.serialization_format.json)

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

    def process_requests(self, data_request_mgr, fmt: eSerializationFormat):
        if data_request_mgr is None:
            data_request_mgr = SEDataRequestManager()
            data_request_mgr.set_data_requests([
                SEDataRequest.create_ecg_request("Lead3ElectricPotential", "mV"),
                SEDataRequest.create_physiology_request("HeartRate", "1/min"),
                SEDataRequest.create_physiology_request("ArterialPressure", "mmHg"),
                SEDataRequest.create_physiology_request("MeanArterialPressure", "mmHg"),
                SEDataRequest.create_physiology_request("SystolicArterialPressure", "mmHg"),
                SEDataRequest.create_physiology_request("DiastolicArterialPressure", "mmHg"),
                SEDataRequest.create_physiology_request("OxygenSaturation"),
                SEDataRequest.create_physiology_request("EndTidalCarbonDioxidePressure", "mmHg"),
                SEDataRequest.create_physiology_request("RespirationRate", "1/min"),
                SEDataRequest.create_physiology_request("CoreTemperature", "degC"),
                SEDataRequest.create_gas_compartment_substance_request("Carina", "CarbonDioxide", "PartialPressure", "mmHg"),
                SEDataRequest.create_physiology_request("BloodVolume", "mL")
            ])
        # Simulation time is always the first result.
        self.results["SimulationTime(s)"] = 0
        for data_request in data_request_mgr.get_data_requests():
            self.results[data_request.to_string()] = 0
        return serialize_data_request_manager_to_string(data_request_mgr, fmt)

    def process_action(self, action: SEAction):
        actions = [action]
        self.process_actions(actions)

    def process_actions(self, actions: []):
        json = serialize_actions_to_string(actions,eSerializationFormat.JSON)
        print(json)
        self.__pulse.process_actions(json,PyPulse.serialization_format.json)


