# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import PyPulse
from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.engine import IEventHandler, ILoggerForward
from pulse.cdm.engine import SEAction, eSerializationFormat, SEDataRequestManager, SEDataRequest

class PulsePhysiologyEnginePoolPatient:
    __slots__ = ["_id", "_is_active",
                 "actions",
                 "active_events",
                 "data_request_mgr",
                 "results",
                 "patient_configuration",
                 "initial_patient",
                 "state_filename",
                 "event_handler",
                 "log_to_console",
                 "log_forward",
                 "log_filename",
                 "_results_template"]

    def __init__(self, id: int):
        self.id = id
        self.is_active = False
        self.actions = []
        self.log_forward = None
        self.log_filename = None
        self.log_to_console = False
        self.event_handler = None
        self.results = {}
        self._results_template = {}
        self.patient_configuration = None
        self.state_filename = None

    def get_id(self):
        return self._id
    def is_active(self):
        return self._is_active

class PulsePhysiologyEnginePool:
    __slots__ = ["__pool", "_is_active",
                 "_dt_s", "_spare_time_s",
                 "_patients"]

    def __init__(self, num_threads=None):
        self.__pool = PyPulse.EnginePool()
        self._is_active = False
        self._spare_time_s = 0
        self._dt_s = 0.02 # self.__pool.get_timestep("s")
        self._patients = {}

    def log_to_console(self, b: bool):
        pass

    def create_patient(self, id :int):
        if self._is_active:
            raise Exception("Engine pool has already been initialized")
        if id in self._patients.keys():
            raise Exception("Engine pool already has patient id "+str(id))
        pp = PulsePhysiologyEnginePoolPatient(id)
        self._patients[id] = pp
        return pp

    def initialize_engines(self):
        if self._is_active:
            raise Exception("Engine pool has already been initialized")

        #self.__pool.keep_event_changes( self._event_handler is not None )
        #self.__pool.keep_log_messages( self._log_forward is not None )
        return False

    def get_patients(self):
        return self._patients

    def serialize_to_strings(self):
        if not self._is_active:
            raise Exception("Engine pool is not initialized")

    def serialize_to_files(self):
        if not self._is_active:
            raise Exception("Engine pool is not initialized")

    def get_initial_patients(self):
        if not self._is_active:
            raise Exception("Engine pool is not initialized")

    # Will consider dynamic addition later
    #def add_engine(self, patient_configurations: SEPatientConfiguration):
    #    pass
    # Will consider dynamic removal later
    #def remove_engine(self, id: int):
    #    pass

    def advance_time_s(self, duration_s: float):
        pass

    def pull_data(self):
        pass

    def pull_active_events(self):
        pass

    # process the same action on all engines, is that useful?
    def process_action(self, SEAction):
        pass

    # process all the actions provided on pool patient action arrays
    def process_actions(self):
        pass



