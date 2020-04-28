# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.io.action import serialize_action_from_bind, serialize_action_to_bind

from pulse.cdm.equipment_actions import SEEquipmentAction
from pulse.cdm.bind.Actions_pb2 import EquipmentActionData

def serialize_equipment_action_to_bind(src: SEEquipmentAction, dst: EquipmentActionData):
    serialize_action_to_bind(src,dst.Action)

def serialize_equipment_action_from_bind(src: EquipmentActionData, dst: SEEquipmentAction):
    serialize_action_from_bind(src.Action, dst)