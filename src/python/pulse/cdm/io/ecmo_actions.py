# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.ECMOActions_pb2 import ECMOActionData, ECMOConfigurationData
from pulse.cdm.ecmo_actions import SEECMOAction, SEECMOConfiguration
from pulse.cdm.io.ecmo import serialize_ecmo_to_bind, serialize_ecmo_from_bind
from pulse.cdm.io.equipment_actions import serialize_equipment_action_from_bind, \
                                           serialize_equipment_action_to_bind
from pulse.cdm.io.scalars import serialize_scalar_pressure_to_bind, \
                                 serialize_scalar_0to1_to_bind, \
                                 serialize_scalar_time_to_bind, \
                                 serialize_scalar_frequency_to_bind, \
                                 serialize_scalar_volume_per_time_to_bind, \
                                 serialize_scalar_volume_to_bind

def serialize_ecmo_action_to_bind(src: SEECMOAction, dst:ECMOActionData):
    serialize_equipment_action_to_bind(src,dst.EquipmentAction)

def serialize_ecmo_action_from_bind(src: ECMOActionData, dst: SEECMOAction):
    serialize_equipment_action_from_bind(src.EquipmentAction, dst)

def serialize_ecmo_configuration_to_bind(src: SEECMOConfiguration, dst: ECMOConfigurationData):
    serialize_ecmo_action_to_bind(src, dst.ECMOAction)
    if src.has_settings_file():
        dst.SettingsFile = src.get_settings_file()
    elif src.has_settings():
        serialize_ecmo_to_bind(src.get_settings(), dst.Settings)

def serialize_ecmo_configuration_from_bind(src: ECMOConfigurationData, dst: SEECMOConfiguration):
    serialize_ecmo_action_from_bind(src.ECMOAction, dst)
    raise Exception("serialize_ecmo_configuration_from_bind not implemented")