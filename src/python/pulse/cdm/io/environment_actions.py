# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.environment_actions import SEEnvironmentAction, \
                                          SEChangeEnvironmentalConditions, \
                                          SEThermalApplication
from pulse.cdm.bind.EnvironmentActions_pb2 import EnvironmentActionData, \
                                                  ChangeEnvironmentalConditionsData, \
                                                  ThermalApplicationData

from pulse.cdm.io.action import serialize_action_from_bind, serialize_action_to_bind
from pulse.cdm.io.environment import *

def serialize_environment_action_to_bind(src: SEEnvironmentAction, dst: EnvironmentActionData):
    serialize_action_to_bind(src,dst.Action)

def serialize_environment_action_from_bind(src: EnvironmentActionData, dst: SEEnvironmentAction):
    serialize_action_from_bind(src.Action,dst)

def serialize_change_environmental_conditions_to_bind(src: SEChangeEnvironmentalConditions, dst: ChangeEnvironmentalConditionsData):
    serialize_environment_action_to_bind(src, dst.EnvironmentAction)
    if src.has_environmental_conditions_file():
        dst.EnvironmentalConditionsFile = src.get_environmental_conditions_file()
    elif src.has_environmental_conditions():
        serialize_environmental_conditions_to_bind(src.get_environmental_conditions(), dst.EnvironmentalConditions)

def serialize_change_environmental_conditions_from_bind(src: ChangeEnvironmentalConditionsData, dst: SEChangeEnvironmentalConditions):
    serialize_environment_action_from_bind(src.EnvironmentAction, dst)
    raise Exception("serialize_change_environmental_conditions_from_bind not implemented")

def serialize_thermal_application_to_bind(src: SEThermalApplication, dst: ThermalApplicationData):
    serialize_environment_action_to_bind(src, dst.EnvironmentAction)
    dst.AppendToPrevious = not src.get_clear_contents()
    if src.has_active_heating():
        serialize_active_conditioning_to_bind(src.get_active_heating(), dst.ActiveHeating)
    if src.has_active_cooling():
        serialize_active_conditioning_to_bind(src.get_active_cooling(), dst.ActiveCooling)
    if src.has_applied_temperature():
        serialize_applied_temperature_to_bind(src.get_applied_temperature(), dst.AppliedTemperature)

def serialize_thermal_application_from_bind(src: ThermalApplicationData, dst: SEThermalApplication):
    serialize_environment_action_from_bind(src.EnvironmentAction, dst)
    raise Exception("serialize_thermal_application_from_bind not implemented")

