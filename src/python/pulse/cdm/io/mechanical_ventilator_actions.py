# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.MechanicalVentilatorActions_pb2 import MechanicalVentilatorActionData, \
                                                           MechanicalVentilatorConfigurationData
from pulse.cdm.mechanical_ventilator_actions import SEMechanicalVentilatorAction, \
                                                    SEMechanicalVentilatorConfiguration
from pulse.cdm.io.mechanical_ventilator import serialize_mechanical_ventilator_to_bind,\
                                               serialize_mechanical_ventilator_from_bind
from pulse.cdm.io.equipment_actions import serialize_equipment_action_from_bind, \
                                           serialize_equipment_action_to_bind

def serialize_mechanical_ventilator_action_to_bind(src: SEMechanicalVentilatorAction, dst:MechanicalVentilatorActionData):
    serialize_equipment_action_to_bind(src,dst.EquipmentAction)

def serialize_mechanical_ventilator_action_from_bind(src: MechanicalVentilatorActionData, dst: SEMechanicalVentilatorAction):
    serialize_equipment_action_from_bind(src.EquipmentAction, dst)

def serialize_mechanical_ventilator_configuration_to_bind(src: SEMechanicalVentilatorConfiguration, dst: MechanicalVentilatorConfigurationData):
    serialize_mechanical_ventilator_action_to_bind(src, dst.MechanicalVentilatorAction)
    if src.has_settings_file():
        dst.SettingsFile = src.get_settings_file()
    elif src.has_settings():
        serialize_mechanical_ventilator_to_bind(src.get_settings(), dst.Settings)

def serialize_mechanical_ventilator_configuration_from_bind(src: MechanicalVentilatorConfigurationData, dst: SEMechanicalVentilatorConfiguration):
    serialize_mechanical_ventilator_action_from_bind(src.MechanicalVentilatorAction, dst)
    raise Exception("serialize_exercise_from_bind not implemented")