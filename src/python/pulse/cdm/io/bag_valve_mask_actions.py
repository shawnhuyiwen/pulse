# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.BagValveMaskActions_pb2 import BagValveMaskActionData, \
                                                   BagValveMaskConfigurationData, \
                                                   BagValveMaskAutomatedData, \
                                                   BagValveMaskInstantaneousData, \
                                                   BagValveMaskSqueezeData
from pulse.cdm.bag_valve_mask_actions import SEBagValveMaskAction, \
                                             SEBagValveMaskConfiguration, \
                                             SEBagValveMaskAutomated, \
                                             SEBagValveMaskInstantaneous, \
                                             SEBagValveMaskSqueeze
from pulse.cdm.io.bag_valve_mask import serialize_bag_valve_mask_to_bind,\
                                        serialize_bag_valve_mask_from_bind
from pulse.cdm.io.equipment_actions import serialize_equipment_action_from_bind, \
                                           serialize_equipment_action_to_bind
from pulse.cdm.io.scalars import serialize_scalar_to_bind, \
                                 serialize_scalar_frequency_to_bind, \
                                 serialize_scalar_pressure_to_bind, \
                                 serialize_scalar_time_to_bind, \
                                 serialize_scalar_volume_to_bind, \
                                 serialize_scalar_volume_per_time_to_bind


def serialize_bag_valve_mask_action_to_bind(src: SEBagValveMaskAction, dst:BagValveMaskActionData):
    serialize_equipment_action_to_bind(src,dst.EquipmentAction)

def serialize_bag_valve_mask_action_from_bind(src: BagValveMaskActionData, dst: SEBagValveMaskAction):
    serialize_equipment_action_from_bind(src.EquipmentAction, dst)


def serialize_bag_valve_mask_configuration_to_bind(src: SEBagValveMaskConfiguration, dst: BagValveMaskConfigurationData):
    serialize_bag_valve_mask_action_to_bind(src, dst.BagValveMaskAction)
    if src.has_configuration_file():
        dst.ConfigurationFile = src.get_configuration_file()
    elif src.has_configuration():
        serialize_bag_valve_mask_to_bind(src.get_configuration(), dst.Configuration)

def serialize_bag_valve_mask_configuration_from_bind(src: BagValveMaskConfigurationData, dst: SEBagValveMaskConfiguration):
    serialize_bag_valve_mask_action_from_bind(src.BagValveMaskAction, dst)
    raise Exception("serialize_bag_valve_mask_configuration_from_bind not implemented")


def serialize_bag_valve_mask_automated_to_bind(src: SEBagValveMaskAutomated, dst: BagValveMaskAutomatedData):
    serialize_bag_valve_mask_action_to_bind(src, dst.BagValveMaskAction)
    if src.has_breath_frequency():
        serialize_scalar_frequency_to_bind(src.get_breath_frequency(), dst.BreathFrequency)
    if src.has_inspiratory_expiratory_ratio():
        serialize_scalar_to_bind(src.get_inspiratory_expiratory_ratio(), dst.InspiratoryExpiratoryRatio)
    if src.has_squeeze_pressure():
        serialize_scalar_pressure_to_bind(src.get_squeeze_pressure(), dst.SqueezePressure)
    if src.has_squeeze_volume():
        serialize_scalar_volume_to_bind(src.get_squeeze_volume(), dst.SqueezeVolume)

def serialize_bag_valve_mask_automated_from_bind(src: BagValveMaskAutomatedData, dst: SEBagValveMaskAutomated):
    serialize_bag_valve_mask_action_from_bind(src.BagValveMaskAction, dst)
    raise Exception("serialize_bag_valve_mask_automated_from_bind not implemented")


def serialize_bag_valve_mask_instantaneous_to_bind(src: SEBagValveMaskInstantaneous, dst: BagValveMaskInstantaneousData):
    serialize_bag_valve_mask_action_to_bind(src, dst.BagValveMaskAction)
    if src.has_flow():
        serialize_scalar_volume_per_time_to_bind(src.get_flow(), dst.Flow)
    if src.has_flow():
        serialize_scalar_pressure_to_bind(src.get_pressure(), dst.Pressure)

def serialize_bag_valve_mask_instantaneous_from_bind(src: BagValveMaskInstantaneousData, dst: SEBagValveMaskInstantaneous):
    serialize_bag_valve_mask_action_from_bind(src.BagValveMaskAction, dst)
    raise Exception("serialize_bag_valve_mask_instantaneous_from_bind not implemented")


def serialize_bag_valve_mask_squeeze_to_bind(src: SEBagValveMaskSqueeze, dst: BagValveMaskSqueezeData):
    serialize_bag_valve_mask_action_to_bind(src, dst.BagValveMaskAction)
    if src.has_squeeze_pressure():
        serialize_scalar_pressure_to_bind(src.get_squeeze_pressure(), dst.SqueezePressure)
    if src.has_squeeze_volume():
        serialize_scalar_volume_to_bind(src.get_squeeze_volume(), dst.SqueezeVolume)
    if src.has_expiratory_period():
        serialize_scalar_time_to_bind(src.get_expiratory_period(), dst.ExpiratoryPeriod)
    if src.has_inspiratory_period():
        serialize_scalar_time_to_bind(src.get_inspiratory_period(), dst.InspiratoryPeriod)

def serialize_bag_valve_mask_squeeze_from_bind(src: BagValveMaskSqueezeData, dst: SEBagValveMaskSqueeze):
    serialize_bag_valve_mask_action_from_bind(src.BagValveMaskAction, dst)
    raise Exception("serialize_bag_valve_mask_squeeze_from_bind not implemented")