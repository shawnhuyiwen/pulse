# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.environment_conditions import SEEnvironmentCondition, \
                                             SEInitialEnvironmentalConditions
from pulse.cdm.bind.EnvironmentConditions_pb2 import EnvironmentConditionData, \
                                                     InitialEnvironmentalConditionsData

from pulse.cdm.io.condition import serialize_condition_to_bind, serialize_condition_from_bind
from pulse.cdm.io.environment import *

def serialize_environment_condition_to_bind(src: SEEnvironmentCondition, dst: EnvironmentConditionData):
    serialize_condition_to_bind(src,dst.Condition)

def serialize_environment_condition_from_bind(src: EnvironmentConditionData, dst: SEEnvironmentCondition):
    serialize_condition_from_bind(src.Condition, dst)

def serialize_initial_environmental_conditions_to_bind(src: SEInitialEnvironmentalConditions, dst: InitialEnvironmentalConditionsData):
    serialize_environment_condition_to_bind(src, dst.EnvironmentCondition)
    if src.has_environmental_conditions_file():
        dst.EnvironmentalConditionsFile = src.get_environmental_conditions_file()
    elif src.has_environmental_conditions():
        serialize_environmental_conditions_to_bind(src.get_environmental_conditions(), dst.EnvironmentalConditions)

def serialize_initial_environmental_conditions_from_bind(src: InitialEnvironmentalConditionsData, dst: SEInitialEnvironmentalConditions):
    serialize_environment_condition_from_bind(src, dst)
    raise Exception("serialize_initial_environmental_conditions_from_bind not implemented")
