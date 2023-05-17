# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import SEDataRequestManager
from pulse.cdm.patient import SEPatientConfiguration

class SEScenario():
    __slots__ = ["_name", "_description", "_params", "_engine_state_file", "_dr_mgr", "_dr_files", "_actions"]

    def __init__(self):
        self._dr_mgr = SEDataRequestManager()
        self.clear()

    def __repr__(self):
        return f'SEScenario({self._name}, {self._description}, {self._params}, {self._engine_state_file}, ' \
            f'{self._dr_mgr}, {self._dr_files}, {self._actions})'

    def __str__(self):
        return f'SEScenario:\n\tName: {self._name}\n\tDescription: {self._description}\n\tPatient Configuration: ' \
            f'{self._params}\n\tEngine State File: {self._engine_state_file}\n\tData Request Manager: {self._dr_mgr}\n\t' \
            f'Data Request Files: {self._dr_files}\n\tActions: {self._actions}'

    def clear(self):
        self._name = ""
        self._description = ""
        self._params = None
        self._engine_state_file = None
        self._dr_mgr.clear()
        self._dr_files = []
        self._actions = []

    def is_valid(self):
        if len(self._actions) == 0:
            return False
        if not self.has_patient_configuration() and not self.has_engine_state():
            return False
        return True

    def get_name(self):
        return self._name
    def set_name(self, name: str):
        self._name = name
    def has_name(self):
        return len(self._name) > 0
    def invalidate_name(self):
        self._name = ""

    def get_description(self):
        return self._description
    def set_description(self, description: str):
        self._description = description
    def has_description(self):
        return len(self._description) > 0
    def invalidate_description(self):
        self._description = ""

    def get_engine_state(self):
        return self._engine_state_file
    def set_engine_state(self, state_file: str):
        self._engine_state_file = state_file
    def has_engine_state(self):
        if self.has_patient_configuration():
            return False
        return self._engine_state_file is not None and len(self._engine_state_file) > 0
    def invalidate_engine_state(self):
        self._engine_state_file = None

    def get_patient_configuration(self):
        if self._params is None:
            self._params = SEPatientConfiguration()
        return self._params
    def has_patient_configuration(self):
        return self._params is not None and self._params.is_valid()
    def invalidate_patient_configuration(self):
        self._params = None

    def get_actions(self):
        return self._actions

    def get_data_request_manager(self):
        return self._dr_mgr

    def get_data_request_files(self):
        return self._dr_files
