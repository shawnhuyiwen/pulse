# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
import math, copy
import PyPulse
from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.engine import SEAction, eSerializationFormat, SEDataRequestManager, SEDataRequest
from pulse.cdm.engine import IEventHandler, ILoggerForward
from pulse.cdm.io.engine import serialize_actions_to_string, \
                                serialize_patient_configuration_to_string, \
                                serialize_data_request_manager_to_string, \
                                serialize_event_change_list_from_string, \
                                serialize_log_messages_from_string


class PulsePhysiologyEngine:
    __slots__ = ['__pulse', "_is_ready", "_dt_s",
                 "_results", "_results_template",
                 "_event_handler", "_log_forward"]

    def __init__(self, log_file="", write_to_console=True, data_root="."):
        self._is_ready = False
        self._results = {}
        self._results_template = {}
        self._event_handler = None
        self._log_forward = None
        self.__pulse = PyPulse.Engine(log_file, write_to_console, data_root)
        self._dt_s = self.__pulse.get_timestep("s")  # TODO this is assuming duration_s is a factor of 0.02

    def serialize_from_file(self, state_file: str,
                                  data_request_mgr: SEDataRequestManager,
                                  format: eSerializationFormat,
                                  start_time: float = 0):
        # Process requests and setup our results structure
        drm = self._process_requests(data_request_mgr, format)
        if format == eSerializationFormat.BINARY:
            fmt = PyPulse.serialization_format.binary
        else:
            fmt = PyPulse.serialization_format.json
        self._is_ready = self.__pulse.serialize_from_file(state_file, drm, fmt, start_time)
        return self._is_ready

    def serialize_to_file(self, state_file: str, format: eSerializationFormat):
        raise Exception("serialize_to_file not implemented")

    def serialize_from_string(self, state: str,
                                    data_request_mgr: SEDataRequestManager,
                                    format: eSerializationFormat,
                                    start_time: float = 0):
        raise Exception("serialize_from_string not implemented")

    def serialize_to_string(format: eSerializationFormat):
        raise Exception("serialize_to_string not implemented")

    def initialize_engine(self, patient_configuration: SEPatientConfiguration, data_request_mgr: SEDataRequestManager):
        # Process requests and setup our results structure
        drm = self._process_requests(data_request_mgr, eSerializationFormat.JSON)
        pc = serialize_patient_configuration_to_string(patient_configuration, eSerializationFormat.JSON)
        self._is_ready =  self.__pulse.initialize_engine(pc, drm, PyPulse.serialization_format.json)
        return self._is_ready

    def set_event_handler(self, event_handler: IEventHandler):
        self._event_handler = event_handler
        self.__pulse.keep_event_changes( self._event_handler is not None )

    def set_log_listener(self, log_forward: ILoggerForward):
        self._log_forward = log_forward
        self.__pulse.keep_log_messages( self._log_forward is not None )

    def _advance_time_and_pull(self, pull: bool):
        if self.__pulse.advance_timestep():
            self._process_events()
            self._process_logs()
            if pull:
                timestep_result = self.__pulse.pull_data()
                for index, value in enumerate(self._results.keys()):
                    self._results[value].append(timestep_result[index])
                return True
        return False

    def advance_time(self):
        if not self._is_ready:
            return False
        self._results = copy.deepcopy(self._results_template)
        return self._advance_time_and_pull(True)

    def advance_time_s(self, duration_s: float):
        if not self._is_ready:
            return False
        self._results = copy.deepcopy(self._results_template)
        num_steps = int(duration_s / self._dt_s)
        for n in range(num_steps):
            if not self._advance_time_and_pull(True):
                return False
        return True

    def advance_time_r(self, duration_s: float, rate_s: float):
        if not self._is_ready:
            return False
        num_steps = int(duration_s / self._dt_s)
        sample_times = range(0, num_steps, math.floor(rate_s / self._dt_s))
        self._results = copy.deepcopy(self._results_template)
        for n in range(num_steps):
            pull = False
            if n in sample_times:
                pull = True
            if not self._advance_time_and_pull(pull):
                return False
        return True

    def pull_data(self):
        return self._results

    def pull_active_events(self):
        raise Exception("pull_active_events not implemented")

    def _process_requests(self, data_request_mgr, fmt: eSerializationFormat):
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
        self._results_template = {} # Clear all results
        self._results_template["SimulationTime(s)"] = []
        for data_request in data_request_mgr.get_data_requests():
            self._results_template[data_request.to_string()] = []
        return serialize_data_request_manager_to_string(data_request_mgr, fmt)

    def process_action(self, action: SEAction):
        if not self._is_ready:
            return False
        actions = [action]
        self.process_actions(actions)

    def process_actions(self, actions: []):
        if not self._is_ready:
            return False
        json = serialize_actions_to_string(actions,eSerializationFormat.JSON)
        print(json)
        self.__pulse.process_actions(json,PyPulse.serialization_format.json)

    def _process_events(self):
        if self._is_ready and self._event_handler:
            events = self.__pulse.pull_events(PyPulse.serialization_format.json)
            if events:
                event_changes = serialize_event_change_list_from_string(events,eSerializationFormat.JSON)
                for event_change in event_changes:
                    self._event_handler.handle_event(event_change)

    def _process_logs(self):
        if self._is_ready and self._log_forward:
            logs = self.__pulse.pull_log_messages(PyPulse.serialization_format.json)
            if logs:
                log_msgs = serialize_log_messages_from_string(logs,eSerializationFormat.JSON)
                for msg in log_msgs['Debug']:
                    self._log_forward.forward_debug(msg)
                for msg in log_msgs['Info']:
                    self._log_forward.forward_info(msg)
                for msg in log_msgs['Warning']:
                    self._log_forward.forward_warning(msg)
                for msg in log_msgs['Fatal']:
                    self._log_forward.forward_fatal(msg)
                for msg in log_msgs['Error']:
                    self._log_forward.forward_error(msg)
                # log_msgs can be a dict with 'Error' as a key to a [string]
