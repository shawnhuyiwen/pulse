# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from google.protobuf import json_format

from pulse.cdm.scenario import SEScenario
from pulse.cdm.bind.Scenario_pb2 import ScenarioData

from pulse.cdm.io.engine import (
    serialize_actions_to_bind,
    serialize_data_request_manager_to_bind,
    serialize_patient_configuration_to_bind
)


def serialize_scenario_to_string(src: SEScenario, fmt: eSerializationFormat):
    dst = ScenarioData()
    serialize_scenario_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_scenario_to_file(src: SEScenario, filename: str):
    string = serialize_scenario_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    n = file.write(string)
    file.close()

def serialize_scenario_from_string(string: str, dst: SEScenario, fmt: eSerializationFormat):
    src = ScenarioData()
    json_format.Parse(string, src)
    serialize_scenario_from_bind(src,dst)

def serialize_scenario_from_file(filename: str, dst: SEScenario):
    with open(filename) as f:
        string = f.read()
    serialize_scenario_from_string(string, dst, eSerializationFormat.JSON)

def serialize_scenario_to_bind(src: SEScenario, dst: ScenarioData):
    if src.has_name():
        dst.Name = src.get_name()

    if src.has_description():
        dst.Description = src.get_description()

    if src.has_patient_configuration():
        serialize_patient_configuration_to_bind(src.get_patient_configuration(), dst.PatientConfiguration)
    elif src.has_engine_state():
        dst.EngineStateFile = src.get_engine_state()

    if src.get_data_request_manager().has_data_requests():
        serialize_data_request_manager_to_bind(src.get_data_request_manager(), dst.DataRequestManager)

    dst.DataRequestFile.extend(src.get_data_request_files())

    actionListData = ActionListData()
    serialize_actions_to_bind(src.get_actions(), actionListData)
    dst.AnyAction = actionListData.AnyAction

def serialize_scenario_from_bind(src: ScenarioData, dst: SEScenario):
    raise Exception("serialize_scenario_from_bind not implemented")
