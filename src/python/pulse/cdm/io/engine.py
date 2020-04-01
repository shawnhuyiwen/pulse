# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import SEDataRequestManager, SEDataRequest, SEConditionManager
from pulse.cdm.bind.Engine_pb2 import ActionListData, AnyActionData, \
                                      AnyConditionData, ConditionListData, \
                                      PatientConfigurationData, \
                                      DataRequestData, DataRequestManagerData

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.equipment_actions import SEEquipmentAction

from pulse.cdm.io.patient import *
from pulse.cdm.io.patient_actions import *
from pulse.cdm.io.environment_actions import *
from pulse.cdm.io.environment_conditions import *
from pulse.cdm.io.mechanical_ventilator_actions import *

def serialize_condition_manager_to_string(condition_manager: SEConditionManager, fmt: eSerializationFormat):
    dst = ConditionListData()
    serialize_condition_manager_to_bind(condition_manager, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_condition_manager_to_bind(condition_manager: SEConditionManager, dst: ConditionListData):
    if (condition_manager.is_empty()):
        return ""
    if condition_manager.has_initial_environmental_conditions():
        any_condition = AnyConditionData()
        serialize_initial_environmental_conditions_to_bind(condition_manager.get_initial_environmental_conditions(),
                                                            any_condition.EnvironmentCondition.InitialEnvironmentalConditions)
        dst.AnyCondition.append(any_condition)

def serialize_actions_to_string(actions: [], fmt: eSerializationFormat):
    action_list = ActionListData()
    for action in actions:
        if not action.is_valid():
            print("Skipping invalid action")
            continue
        print(action)
        any_action = AnyActionData()
        if isinstance(action, SEPatientAction):
            if isinstance(action, SEExercise):
                serialize_exercise_to_bind(action, any_action.PatientAction.Exercise)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEHemorrhage):
                serialize_hemorrhage_to_bind(action, any_action.PatientAction.Hemorrhage)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAsthmaAttack):
                serialize_asthma_attack_to_bind(action, any_action.PatientAction.AsthmaAttack)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAcuteRespiratoryDistressSyndromeExacerbation):
                serialize_acute_respiratory_exacerbation_to_bind(action, any_action.PatientAction.AcuteRespiratoryDistressSyndromeExacerbation)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAcuteStress):
                serialize_acute_stress_to_bind(action, any_action.PatientAction.AcuteStress)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAirwayObstruction):
                serialize_airway_obstruction_to_bind(action, any_action.PatientAction.AirwayObstruction)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBrainInjury):
                serialize_brain_injury_to_bind(action, any_action.PatientAction.BrainInjury)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBronchoConstriction):
                serialize_broncho_constriction_to_bind(action, any_action.PatientAction.Bronchoconstriction)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SECardiacArrest):
                serialize_cardiac_arrest_to_bind(action, any_action.PatientAction.CardiacArrest)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestOcclusiveDressing):
                serialize_chest_occlusive_dressing_to_bind(action, any_action.PatientAction.ChestOcclusiveDressing)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEDyspnea):
                serialize_dsypnea_to_bind(action, any_action.PatientAction.Dyspnea)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEIntubation):
                serialize_intubation_to_bind(action, any_action.PatientAction.Intubation)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SELobarPneumoniaExacerbation):
                serialize_lobar_pneumonia_exacerbation_to_bind(action, any_action.PatientAction.LobarPneumoniaExacerbation)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilation):
                serialize_mechanical_ventilation_to_bind(action, any_action.PatientAction.MechanicalVentilation)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SENeedleDecompression):
                serialize_needle_decompression_to_bind(action, any_action.PatientAction.NeedleDecompression)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEPericardialEffusion):
                serialize_pericardial_effusion_to_bind(action, any_action.PatientAction.PericardialEffusion)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SERespiratoryFatigue):
                serialize_respiratory_fatigue_to_bind(action, any_action.PatientAction.RespiratoryFatigue)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceBolus):
                serialize_substance_bolus_to_bind(action, any_action.PatientAction.SubstanceBolus)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceInfusion):
                serialize_substance_infusion_to_bind(action, any_action.PatientAction.SubstanceInfusion)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceCompoundInfusion):
                serialize_substance_compound_infusion_to_bind(action, any_action.PatientAction.SubstanceCompoundInfusion)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SESupplementalOxygen):
                serialize_supplemental_oxygen_to_bind(action, any_action.PatientAction.SupplementalOxygen)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SETensionPneumothorax):
                serialize_tension_pneumothorax_to_bind(action, any_action.PatientAction.TensionPneumothorax)
                action_list.AnyAction.append(any_action)
                continue
        if isinstance(action, SEEnvironmentAction):
            if isinstance(action, SEChangeEnvironmentalConditions):
                serialize_change_environmental_conditions_to_bind(action, any_action.EnvironmentAction.ChangeEnvironmentalConditions)
                action_list.AnyAction.append(any_action)
                continue
            if isinstance(action, SEThermalApplication):
                serialize_thermal_application_to_bind(action, any_action.EnvironmentAction.ThermalApplication)
                action_list.AnyAction.append(any_action)
                continue
        if isinstance(action, SEEquipmentAction):
            if isinstance(action, SEMechanicalVentilatorConfiguration):
                serialize_mechanical_ventilator_configuration_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorConfiguration)
                action_list.AnyAction.append(any_action)
                continue
    return json_format.MessageToJson(action_list,True,True)

# Patient Configuration
def serialize_patient_configuration_to_string(src: SEPatientConfiguration, fmt: eSerializationFormat):
    dst = PatientConfigurationData()
    serialize_patient_configuration_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_patient_configuration_to_file(src: SEPatientConfiguration, filename: str, fmt: eSerializationFormat):
    string = serialize_patient_configuration_to_string(src, fmt)
    file = open(filename, "w")
    n = file.write(string)
    file.close()

def serialize_patient_configuration_from_string(string: str, dst: SEPatientConfiguration, fmt: eSerializationFormat):
    src = PatientConfigurationData()
    json_format.Parse(string, src)
    serialize_patient_configuration_from_bind(src,dst)

def serialize_patient_configuration_from_file(filename: str, dst: SEPatientConfiguration, fmt: eSerializationFormat):
    with open(filename) as f:
        string = f.read()
    serialize_patient_configuration_from_string(string, dst, fmt)

def serialize_patient_configuration_to_bind(src: SEPatientConfiguration, dst: PatientConfigurationData):
    if src.has_patient():
        serialize_patient_to_bind(src.get_patient(), dst.Patient)
    elif src.has_patient_file():
        dst.PatientFile = src.get_patient_file()
    if src.has_conditions():
        serialize_condition_manager_to_bind(src.get_conditions(), dst.Conditions)

def serialize_patient_configuration_from_bind(src: PatientConfigurationData, dst: SEPatientConfiguration):
    raise Exception("serialize_patient_configuration_from_bind not implemented")


# Data Requests and Manager
def serialize_data_request_to_bind(src: SEDataRequest, dst: DataRequestData):
    if src.has_compartment_name():
        dst.CompartmentName = src.get_compartment_name()
    if src.has_substance_name():
        dst.SubstanceName = src.get_substance_name()
    if src.has_unit():
        dst.Unit = src.get_unit()
    dst.PropertyName = src.get_property_name()
    dst.Category = src.get_category().value

def serialize_data_request_from_bind(src: DataRequestData, dst: SEDataRequest):
    raise Exception("serialize_data_request_from_bind not implemented")

def serialize_data_request_manager_to_string(src: SEDataRequestManager, fmt: eSerializationFormat):
    dst = DataRequestManagerData()
    serialize_data_request_manager_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_data_request_manager_from_string(string: str, dst: SEDataRequestManager, fmt: eSerializationFormat):
    src = DataRequestManagerData()
    json_format.Parse(string, src)
    serialize_data_request_manager_from_bind(src,dst)

def serialize_data_request_manager_to_bind(src: SEDataRequestManager, dst: DataRequestManagerData):
    if src.has_data_requests():
        for dr in src.get_data_requests():
            dst_dr = DataRequestData()
            serialize_data_request_to_bind(dr, dst_dr)
            dst.DataRequest.append(dst_dr)

def serialize_data_request_manager_from_bind(src: DataRequestManagerData, dst: SEDataRequestManager):
    raise Exception("serialize_data_request_manager_from_bind not implemented")
