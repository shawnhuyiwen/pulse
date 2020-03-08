# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import SECondition
from pulse.cdm.bind.Conditions_pb2 import ConditionData

def serialize_condition_to_bind(src: SECondition, dst: ConditionData):
    if src.has_comment():
      dst.Comment = src.get_comment()

def serialize_condition_from_bind(src: ConditionData, dst: SECondition):
    if src.Comment is not None:
      dst.set_comment(src.Comment)