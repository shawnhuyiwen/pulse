# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import PyPulse
from pulse.cdm.engine import SEEngineInitialization, SEDataRequested, \
                             SEAction, SEAdvanceTime, eSerializationFormat

from pulse.cdm.scalars import TimeUnit

class SEEnginePoolEngine: # TODO follow CDM get/set pattern?
    __slots__ = ["is_active",
                 "engine_initialization",
                 "actions",
                 "data_requested"]

    def __init__(self):
        self.is_active = False
        self.engine_initialization = SEEngineInitialization()
        self.actions = []
        self.data_requested = SEDataRequested()

    def get_id(self):
        return self.engine_initialization.id

from pulse.engine.io.PulseEnginePool import serialize_pool_engine_initializations_to_string, \
                                                   serialize_pool_actions_to_string, \
                                                   serialize_pool_data_requested_from_string

class PulseEnginePool:
    __slots__ = ["__pool", "_is_active",
                 "_dt_s", "_spare_time_s",
                 "_engines"]

    def __init__(self, num_threads=0, data_root_dir="./"):
        self.__pool = PyPulse.EnginePool(num_threads, data_root_dir)
        self._is_active = False
        self._spare_time_s = 0
        self._dt_s = 0.02 #self.__pool.get_timestep("s")
        self._engines = {}

    def create_engine(self, id: int):
        if self._is_active:
            # TODO consider dynamic addition later
            raise Exception("Engine pool has already been initialized")
        if id in self._engines.keys():
            raise Exception("Engine pool already has engine id "+str(id))
        pool_engine = SEEnginePoolEngine()
        pool_engine.engine_initialization.id = id
        pool_engine.data_requested.id = id
        self._engines[id] = pool_engine
        return pool_engine

    def initialize_engines(self):
        if self._is_active:
            raise Exception("Engine pool has already been initialized")
        # Setup our data requested values
        for e in self._engines.values():
            e.data_requested.values.clear()
            e.data_requested.values["SimulationTime(s)"] = None
            for data_request in e.engine_initialization.data_request_mgr.get_data_requests():
                e.data_requested.values[data_request.to_string()] = None
        json = serialize_pool_engine_initializations_to_string(self._engines.values(), eSerializationFormat.JSON)
        # If at least 1 engine is active, we will receive true
        self._is_active = self.__pool.initialize_engines(json, PyPulse.serialization_format.json)
        if self._is_active is not False:
            self._pull_data_requested()
        return self._is_active

    def get_engines(self):
        return self._engines

    def get_engine(self, id: int):
        if id not in self._engines.keys():
            raise Exception("Engine id "+str(id)+", does not exist")
        return self._engines[id]

    def remove_engine(self, id: int):
        self.__pool.remove_engine(id)
        del self._engines[id]

    def serialize_engines_to_strings(self):
        if not self._is_active:
            raise Exception("Engine pool is not initialized")
        raise Exception("Not implemented")

    def serialize_engines_to_files(self):
        if not self._is_active:
            raise Exception("Engine pool is not initialized")
        raise Exception("Not implemented")

    def get_initial_patients(self):
        if not self._is_active:
            raise Exception("Engine pool is not initialized")
        raise Exception("Not implemented")

    def advance_time_s(self, duration_s: float):
        adv = SEAdvanceTime()
        adv.get_time().set_value(duration_s, TimeUnit.s)
        self.process_action(adv)

    # process the same action on all engines
    def process_action(self, a: SEAction):
        for e in self._engines.values():
            if e.is_active:
                e.actions.append(a)
        self.process_actions()

    # process all the actions provided on each pool engine action array
    def process_actions(self):
        json = serialize_pool_actions_to_string(self._engines.values(), eSerializationFormat.JSON)
        if not self.__pool.process_actions(json, PyPulse.serialization_format.json):
            return False
        for e in self._engines.values():
            for a in e.actions:
                if isinstance(a, SEAdvanceTime):
                    self._pull_data_requested()
                    return True
        return True

    def _pull_data_requested(self):
        result_json = self.__pool.pull_data_requested(PyPulse.serialization_format.json)
        serialize_pool_data_requested_from_string(result_json, self._engines, eSerializationFormat.JSON)
        return True



