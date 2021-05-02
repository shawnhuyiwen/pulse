# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import PyPulse
from pulse.cdm.engine import SEEngineInitialization, SEEnginePoolEngine, \
                             SEAction, SEAdvanceTime, eSerializationFormat

from pulse.cdm.io.engine import serialize_engine_initializations_to_string, \
                                serialize_engine_pool_result_list_from_string, \
                                serialize_engine_pool_actions_to_string

from pulse.cdm.scalars import TimeUnit

class PulsePhysiologyEnginePool:
    __slots__ = ["__pool", "_is_active",
                 "_dt_s", "_spare_time_s",
                 "_engines"]

    def __init__(self, num_threads=None):
        self.__pool = PyPulse.EnginePool()
        self._is_active = False
        self._spare_time_s = 0
        self._dt_s = 0.02 # self.__pool.get_timestep("s")
        self._engines = {}

    def append_engine(self, engine: SEEngineInitialization):
        if self._is_active:
            # TODO consider dynamic addition later
            raise Exception("Engine pool has already been initialized")
        if engine.id in self._engines.keys():
            raise Exception("Engine pool already has patient id "+str(engine.id))
        pool_engine = SEEnginePoolEngine()
        pool_engine.engine_initialization = engine
        self._engines[engine.id] = pool_engine
        return pool_engine

    def initialize_engines(self):
        if self._is_active:
            raise Exception("Engine pool has already been initialized")
        ec = []
        for e in self._engines.values():
            ec.append(e.engine_initialization)
        json = serialize_engine_initializations_to_string(ec, eSerializationFormat.JSON)
        result_json = self.__pool.initialize_engines(json, eSerializationFormat.JSON)
        serialize_engine_pool_result_list_from_string(result_json, self._engines, eSerializationFormat.JSON)
        # TODO if at least 1 engine is active, we are good!
        for e in self._engines.values():
            e.is_active = True # TODO assume we are good until comms are hooked up
        self._is_active = True
        return True

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
        for e in self._engines:
            if e.is_active:
                e.actions.append(a)
        self.process_actions()

    # process all the actions provided on each pool engine action array
    def process_actions(self):
        json = serialize_engine_pool_actions_to_string(self._engines.values(), eSerializationFormat.JSON)
        self.__pool.process_actions(json, eSerializationFormat.JSON)
        print(json)
        for e in self._engines.values():
            for a in e.actions:
                if isinstance(a, SEAdvanceTime):
                    self._pull_data()
                    return

    def _pull_data(self):
        # result_json = self.__pool.pull_data(json, eSerializationFormat.JSON)
        # serialize_engine_pool_result_list_from_string("", self._engines, eSerializationFormat.JSON)
        pass



