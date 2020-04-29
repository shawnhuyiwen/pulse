# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import SEAction, SEScalarOverride
from pulse.cdm.io.scalars import serialize_scalar_property_to_bind, serialize_scalar_property_from_bind
from pulse.cdm.bind.Actions_pb2 import ActionData, OverridesData

def serialize_action_to_bind(src: SEAction, dst: ActionData):
    if src.has_comment():
        dst.Comment = src.get_comment()

def serialize_action_from_bind(src: ActionData, dst: SEAction):
    dst.set_comment(src.Comment)

def serialize_overrides_to_bind(src:SEScalarOverride, dst:OverridesData):
    serialize_action_to_bind(src.get_action(), dst.Action)
    for override in src.get_scalar_overrides():
        serialize_scalar_property_to_bind(override, dst.ScalarOverride.add())

def serialize_override_from_bind(src: OverridesData, dst: SEScalarOverride):
    raise Exception("serialize_override_from_bind not implemented")
    # Commenting out for now: J.Snyder 04/09/2020
    # dst.set_action(src.Action)
    # for override in src.ScalarOverride():
    #    serialize_scalar_property_from_bind(override, dst.get_scalar_overrides().add())