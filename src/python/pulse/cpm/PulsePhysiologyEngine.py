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
                                serialize_active_event_list_from_string, \
                                serialize_log_messages_from_string


class PulsePhysiologyEngine:
    __slots__ = ['__pulse', "_is_ready", "_dt_s",
                 "_results", "_results_template",
                 "_event_handler", "_log_forward",
                 "_spare_time_s"]

    def __init__(self):
        self._is_ready = False
        self._log_forward = None
        self._event_handler = None
        self._results = {}
        self._results_template = {}
        self.__pulse = PyPulse.Engine()
        self._spare_time_s = 0
        self._dt_s = 0.02 # self.__pulse.get_timestep("s")
        # Timestep only gets set after on load/initialize

    def serialize_from_file(self, state_file: str,
                                  data_request_mgr: SEDataRequestManager,
                                  format: eSerializationFormat):
        # Process requests and setup our results structure
        drm = self._process_requests(data_request_mgr, format)
        if format == eSerializationFormat.BINARY:
            fmt = PyPulse.serialization_format.binary
        else:
            fmt = PyPulse.serialization_format.json
        self._is_ready = self.__pulse.serialize_from_file(state_file, drm, fmt)
        self._dt_s = self.__pulse.get_timestep("s")
        if self._is_ready:
            self._pull(True)
        return self._is_ready

    def serialize_to_file(self, state_file: str, format: eSerializationFormat):
        if self._is_ready:
            return self.__pulse.serialize_to_file(state_file, format)
        return False


    def serialize_from_string(self, state: str,
                                    data_request_mgr: SEDataRequestManager,
                                    format: eSerializationFormat):
        # Process requests and setup our results structure
        drm = self._process_requests(data_request_mgr, format)
        if format == eSerializationFormat.BINARY:
            fmt = PyPulse.serialization_format.binary
        else:
            fmt = PyPulse.serialization_format.json
        self._is_ready = self.__pulse.serialize_from_string(state, drm, fmt)
        self._dt_s = self.__pulse.get_timestep("s")
        if self._is_ready:
            self._pull(True)
        return self._is_ready

    def serialize_to_string(self, format: eSerializationFormat):
        if self._is_ready:
            return self.__pulse.serialize_to_string(format)
        return None

    def initialize_engine(self, patient_configuration: SEPatientConfiguration,
                                data_request_mgr: SEDataRequestManager,
                                data_dir: str="./"):
        # Process requests and setup our results structure
        drm = self._process_requests(data_request_mgr, eSerializationFormat.JSON)
        pc = serialize_patient_configuration_to_string(patient_configuration, eSerializationFormat.JSON)
        self._is_ready = self.__pulse.initialize_engine(pc, drm, PyPulse.serialization_format.json, data_dir)
        self._dt_s = self.__pulse.get_timestep("s")
        if self._is_ready:
            self._pull(True)
        return self._is_ready

    def set_event_handler(self, event_handler: IEventHandler):
        self._event_handler = event_handler
        self.__pulse.keep_event_changes( self._event_handler is not None )

    def set_log_listener(self, log_forward: ILoggerForward):
        self._log_forward = log_forward
        self.__pulse.keep_log_messages( self._log_forward is not None )

    def log_to_console(self, b: bool):
        self.__pulse.log_to_console( b )

    def set_log_filename(self, filename: str):
        self.__pulse.set_log_filename( filename )

    def _advance_time(self):
        if self.__pulse.advance_timestep():
            self._process_events()
            self._process_logs()
            return True
        return False

    def _pull(self, clean: bool):
        if clean:
            self._results = copy.deepcopy(self._results_template)
        timestep_result = self.__pulse.pull_data()
        for index, value in enumerate(self._results.keys()):
            self._results[value].append(timestep_result[index])

    def advance_time(self):
        if not self._is_ready:
            return False
        if self._advance_time():
            self._pull(True)
            return True
        return False

    def advance_time_s(self, duration_s: float):
        if not self._is_ready:
            return False
        total_time = duration_s + self._spare_time_s
        num_steps = int(total_time / self._dt_s)
        for n in range(num_steps):
            if not self._advance_time():
                return False
        self._pull(True)
        self._spare_time_s = total_time - (num_steps * self._dt_s)
        return True

    def advance_time_sample_per_s(self, duration_s: float, rate_s: float):
        if not self._is_ready:
            return False
        total_time = duration_s + self._spare_time_s
        num_steps = int(total_time / self._dt_s)
        sample_times = range(0, num_steps, math.floor(rate_s / self._dt_s))
        self._results = copy.deepcopy(self._results_template)
        for n in range(num_steps):
            if not self._advance_time():
                return False
            if n in sample_times:
                self._pull(False)
        self._spare_time_s = total_time - (num_steps * self._dt_s)
        return True

    def pull_data(self):
        return self._results

    def pull_active_events(self):
        events = self.__pulse.pull_active_events(PyPulse.serialization_format.json)
        if events:
            active_events = serialize_active_event_list_from_string(events, eSerializationFormat.JSON)
            return active_events
        return None

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
        #print(json)
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
