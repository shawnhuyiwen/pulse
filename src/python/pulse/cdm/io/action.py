# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import SEAction
from pulse.cdm.bind.Actions_pb2 import ActionData

def serialize_action_to_bind(src: SEAction, dst: ActionData):
    dst.Comment = src.get_comment()

def serialize_action_from_bind(src: ActionData, dst: SEAction):
    dst.set_comment(src.Comment)
