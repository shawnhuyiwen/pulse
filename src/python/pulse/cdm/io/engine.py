# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from typing import List

from pulse.cdm.engine import SEDataRequestManager, SEDataRequest, SEDataRequested, \
                             SEConditionManager, SEEngineInitialization, SEValidationTarget, \
                             SESegmentValidationTarget, SETimeSeriesValidationTarget
from pulse.cdm.bind.Engine_pb2 import AnyActionData, \
                                      ActionListData, ActionMapData, \
                                      AnyConditionData, ConditionListData, \
                                      PatientConfigurationData, \
                                      DataRequestData, DataRequestManagerData, \
                                      DataRequestedData, DataRequestedListData, \
                                      EngineInitializationData, EngineInitializationListData, \
                                      LogMessagesData, ValidationTargetData, \
                                      SegmentValidationTargetData, SegmentValidationTargetListData, \
                                      TimeSeriesValidationTargetData, TimeSeriesValidationTargetListData
from pulse.cdm.bind.Events_pb2 import ActiveEventListData, EventChangeListData

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.equipment_actions import SEEquipmentAction
from pulse.cdm.engine import SEEventChange, eEvent

from pulse.cdm.io.action import *
from pulse.cdm.io.patient_actions import *
from pulse.cdm.io.patient_conditions import *
from pulse.cdm.io.environment_actions import *
from pulse.cdm.io.environment_conditions import *
from pulse.cdm.io.ecmo_actions import *
from pulse.cdm.io.mechanical_ventilator_actions import *

def serialize_event_change_list_to_bind(src: [], dst: EventChangeListData):
    raise Exception("serialize_event_change_list_to_bind not implemented")
def serialize_event_change_list_to_string(src: []):
    raise Exception("serialize_event_change_list_to_string not implemented")

def serialize_event_change_list_from_string(string: str, fmt: eSerializationFormat):
    src = EventChangeListData()
    json_format.Parse(string, src)
    return serialize_event_change_list_from_bind(src)
def serialize_event_change_list_from_bind(src: EventChangeListData):
    event_changes = []
    for ecd in src.Change:
        ec = SEEventChange()
        ec.event = eEvent(ecd.Event).name
        ec.active = ecd.Active
        event_changes.append(ec)
    return event_changes

def serialize_active_event_list_to_bind(src: [], dst: ActiveEventListData):
    raise Exception("serialize_active_event_list_to_bind not implemented")
def serialize_active_event_list_to_string(src: []):
    raise Exception("serialize_active_event_list_to_string not implemented")

def serialize_active_event_list_from_string(string: str, fmt: eSerializationFormat):
    src = ActiveEventListData()
    json_format.Parse(string, src)
    return serialize_active_event_list_from_bind(src)
def serialize_active_event_list_from_bind(src: ActiveEventListData):
    active_events = {}
    for ae in src.ActiveEvent:
        active_events[eEvent(ae.Event).name] = ae.Duration.ScalarTime.Value
    return active_events

def serialize_log_messages_to_bind(src: {}, dst: LogMessagesData):
    raise Exception("serialize_log_messages_to_bind not implemented")
def serialize_log_messages_to_string(src: []):
    raise Exception("serialize_log_messages_to_string not implemented")

def serialize_log_messages_from_string(string: str, fmt: eSerializationFormat):
    src = LogMessagesData()
    json_format.Parse(string, src)
    return serialize_log_messages_from_bind(src)
def serialize_log_messages_from_bind(src: LogMessagesData):
    log_messages = { 'Debug':[], 'Info':[], 'Warning':[], 'Fatal':[], 'Error':[], }
    for msg in src.DebugMessages:
        log_messages['Debug'].append(msg)
    for msg in src.InfogMessages:
        log_messages['Info'].append(msg)
    for msg in src.WarningMessages:
        log_messages['Warning'].append(msg)
    for msg in src.FatalMessages:
        log_messages['Fatal'].append(msg)
    for msg in src.ErrorMessages:
        log_messages['Error'].append(msg)

    return log_messages

def serialize_condition_manager_to_string(condition_manager: SEConditionManager, fmt: eSerializationFormat):
    dst = ConditionListData()
    serialize_condition_manager_to_bind(condition_manager, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_condition_manager_to_bind(condition_manager: SEConditionManager, dst: ConditionListData):
    if (condition_manager.is_empty()):
        return ""
    if condition_manager.has_acute_respiratory_distress_syndrome():
        any_condition = AnyConditionData()
        serialize_acute_respiratory_distress_syndrome_to_bind(condition_manager.get_acute_respiratory_distress_syndrome(),
                                                            any_condition.PatientCondition.AcuteRespiratoryDistressSyndrome)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_anemia():
        any_condition = AnyConditionData()
        serialize_chronic_anemia_to_bind(condition_manager.get_chronic_anemia(),
                                         any_condition.PatientCondition.ChronicAnemia)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_obstructive_pulmonary_disease():
        any_condition = AnyConditionData()
        serialize_chronic_obstructive_pulmonary_disease_to_bind(condition_manager.get_chronic_obstructive_pulmonary_disease(),
                                                                any_condition.PatientCondition.ChronicObstructivePulmonaryDisease)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_pericardial_effusion():
        any_condition = AnyConditionData()
        serialize_chronic_pericardial_effusion_to_bind(condition_manager.get_chronic_pericardial_effusion(),
                                                       any_condition.PatientCondition.ChronicPericardialEffusion)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_renal_stenosis():
        any_condition = AnyConditionData()
        serialize_chronic_renal_stenosis_to_bind(condition_manager.get_chronic_renal_stenosis(),
                                                 any_condition.PatientCondition.ChronicRenalStenosis)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_ventricular_systolic_dysfunction():
        any_condition = AnyConditionData()
        serialize_chronic_ventricular_systolic_dysfunction_to_bind(condition_manager.get_chronic_ventricular_systolic_dysfunction(),
                                                                   any_condition.PatientCondition.ChronicVentricularSystolicDysfunction)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_impaired_alveolar_exchange():
        any_condition = AnyConditionData()
        serialize_impaired_alveolar_exchange_to_bind(condition_manager.get_impaired_alveolar_exchange(),
                                                     any_condition.PatientCondition.ImpairedAlveolarExchange)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_lobar_pneumonia():
        any_condition = AnyConditionData()
        serialize_lobar_pneumonia_to_bind(condition_manager.get_lobar_pneumonia(),
                                          any_condition.PatientCondition.LobarPneumonia)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_pulmonary_fibrosis():
        any_condition = AnyConditionData()
        serialize_pulmonary_fibrosis_to_bind(condition_manager.get_pulmonary_fibrosis(),
                                             any_condition.PatientCondition.PulmonaryFibrosis)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_pulmonary_shunt():
        any_condition = AnyConditionData()
        serialize_pulmonary_shunt_to_bind(condition_manager.get_pulmonary_shunt(),
                                          any_condition.PatientCondition.PulmonaryShunt)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_sepsis():
        any_condition = AnyConditionData()
        serialize_sepsis_to_bind(condition_manager.get_sepsis(),
                                 any_condition.PatientCondition.Sepsis)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_initial_environmental_conditions():
        any_condition = AnyConditionData()
        serialize_initial_environmental_conditions_to_bind(condition_manager.get_initial_environmental_conditions(),
                                                           any_condition.EnvironmentCondition.InitialEnvironmentalConditions)
        dst.AnyCondition.append(any_condition)

def serialize_actions_to_bind(src: [], dst: ActionListData):

    for action in src:
        if not action.is_valid():
            print("Skipping invalid action")
            continue
        #print(action)
        any_action = AnyActionData()
        if isinstance(action, SEAdvanceTime):
            serialize_advance_time_to_bind(action, any_action.AdvanceTime)
            dst.AnyAction.append(any_action)
            continue
        if isinstance(action, SEPatientAction):
            if isinstance(action, SEAcuteRespiratoryDistressSyndromeExacerbation):
                serialize_acute_respiratory_distress_syndrome_exacerbation_to_bind(action, any_action.PatientAction.AcuteRespiratoryDistressSyndromeExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAcuteStress):
                serialize_acute_stress_to_bind(action, any_action.PatientAction.AcuteStress)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAirwayObstruction):
                serialize_airway_obstruction_to_bind(action, any_action.PatientAction.AirwayObstruction)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEArrhythmia):
                serialize_arrhythmia_to_bind(action, any_action.PatientAction.Arrhythmia)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEArrhythmia):
                serialize_arrhythmia_to_bind(action, any_action.PatientAction.Arrhythmia)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAsthmaAttack):
                serialize_asthma_attack_to_bind(action, any_action.PatientAction.AsthmaAttack)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBrainInjury):
                serialize_brain_injury_to_bind(action, any_action.PatientAction.BrainInjury)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBronchoconstriction):
                serialize_bronchoconstriction_to_bind(action, any_action.PatientAction.Bronchoconstriction)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestCompression):
                serialize_chest_compression_to_bind(action, any_action.PatientAction.ChestCompression)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestCompressionInstantaneous):
                serialize_chest_compression_instantaneous_to_bind(action, any_action.PatientAction.ChestCompressionInstantaneous)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestCompressionAutomated):
                serialize_chest_compression_automated_to_bind(action, any_action.PatientAction.ChestCompressionAutomated)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestOcclusiveDressing):
                serialize_chest_occlusive_dressing_to_bind(action, any_action.PatientAction.ChestOcclusiveDressing)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChronicObstructivePulmonaryDiseaseExacerbation):
                serialize_chronic_obstructive_pulmonary_disease_exacerbation_to_bind(action, any_action.PatientAction.ChronicObstructivePulmonaryDiseaseExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEConsciousRespiration):
                serialize_conscious_respiration_to_bind(action, any_action.PatientAction.ConsciousRespiration)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEConsumeNutrients):
                serialize_consume_nutrients_to_bind(action, any_action.PatientAction.ConsumeNutrients)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEDyspnea):
                serialize_dsypnea_to_bind(action, any_action.PatientAction.Dyspnea)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEExercise):
                serialize_exercise_to_bind(action, any_action.PatientAction.Exercise)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEHemorrhage):
                serialize_hemorrhage_to_bind(action, any_action.PatientAction.Hemorrhage)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEHemothorax):
                serialize_hemothorax_to_bind(action, any_action.PatientAction.Hemothorax)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEImpairedAlveolarExchangeExacerbation):
                serialize_impaired_alveolar_exchange_exacerbation_to_bind(action, any_action.PatientAction.ImpairedAlveolarExchangeExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEIntubation):
                serialize_intubation_to_bind(action, any_action.PatientAction.Intubation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SELobarPneumoniaExacerbation):
                serialize_lobar_pneumonia_exacerbation_to_bind(action, any_action.PatientAction.LobarPneumoniaExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilation):
                serialize_mechanical_ventilation_to_bind(action, any_action.PatientAction.MechanicalVentilation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SENeedleDecompression):
                serialize_needle_decompression_to_bind(action, any_action.PatientAction.NeedleDecompression)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEPericardialEffusion):
                serialize_pericardial_effusion_to_bind(action, any_action.PatientAction.PericardialEffusion)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEPulmonaryShuntExacerbation):
                serialize_pulmonary_shunt_exacerbation_to_bind(action, any_action.PatientAction.PulmonaryShuntExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SERespiratoryFatigue):
                serialize_respiratory_fatigue_to_bind(action, any_action.PatientAction.RespiratoryFatigue)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SERespiratoryMechanicsConfiguration):
                serialize_respiratory_mechanics_configuration_to_bind(action, any_action.PatientAction.RespiratoryMechanicsConfiguration)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceBolus):
                serialize_substance_bolus_to_bind(action, any_action.PatientAction.SubstanceBolus)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceInfusion):
                serialize_substance_infusion_to_bind(action, any_action.PatientAction.SubstanceInfusion)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceCompoundInfusion):
                serialize_substance_compound_infusion_to_bind(action, any_action.PatientAction.SubstanceCompoundInfusion)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SESupplementalOxygen):
                serialize_supplemental_oxygen_to_bind(action, any_action.PatientAction.SupplementalOxygen)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SETensionPneumothorax):
                serialize_tension_pneumothorax_to_bind(action, any_action.PatientAction.TensionPneumothorax)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SETubeThoracostomy):
                serialize_tube_thoracostomy_to_bind(action, any_action.PatientAction.TubeThoracostomy)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEUrinate):
                serialize_urinate_to_bind(action, any_action.PatientAction.Urinate)
                dst.AnyAction.append(any_action)
                continue
            print("Uknown Patient Action")
        if isinstance(action, SEEnvironmentAction):
            if isinstance(action, SEChangeEnvironmentalConditions):
                serialize_change_environmental_conditions_to_bind(action, any_action.EnvironmentAction.ChangeEnvironmentalConditions)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEThermalApplication):
                serialize_thermal_application_to_bind(action, any_action.EnvironmentAction.ThermalApplication)
                dst.AnyAction.append(any_action)
                continue
        if isinstance(action, SEEquipmentAction):
            if isinstance(action, SEMechanicalVentilatorConfiguration):
                serialize_mechanical_ventilator_configuration_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorConfiguration)
                dst.AnyAction.append(any_action)
                continue
            # if isinstance(action, SEMechanicalVentilatorMode):
            #     serialize_mechanical_ventilator_mode_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorMode)
            #     dst.AnyAction.append(any_action)
            #     continue
            if isinstance(action, SEMechanicalVentilatorContinuousPositiveAirwayPressure):
                serialize_mechanical_ventilator_continuous_positive_airway_pressure_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorContinuousPositiveAirwayPressure)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorPressureControl):
                serialize_mechanical_ventilator_pressure_control_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorPressureControl)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorVolumeControl):
                serialize_mechanical_ventilator_volume_control_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorVolumeControl)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorHold):
                serialize_mechanical_ventilator_hold_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorHold)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorLeak):
                serialize_mechanical_ventilator_leak_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorLeak)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEECMOConfiguration):
                serialize_ecmo_configuration_to_bind(action, any_action.EquipmentAction.ECMOConfiguration)
                dst.AnyAction.append(any_action)
                continue
            print("Unknown Equipment Action")

def serialize_actions_to_string(actions: [], fmt: eSerializationFormat):
    action_list = ActionListData()
    serialize_actions_to_bind(actions, action_list)
    return json_format.MessageToJson(action_list,True,True)

def serialize_action_map_to_string(src: {int, SEAction}, fmt: eSerializationFormat):
    dst = ActionMapData()
    for e in src:
        serialize_actions_to_bind(e.actions, dst.ActionMap[e.get_id()])
    return json_format.MessageToJson(dst, True, True)

# Patient Configuration
def serialize_patient_configuration_to_string(src: SEPatientConfiguration, fmt: eSerializationFormat):
    dst = PatientConfigurationData()
    serialize_patient_configuration_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_patient_configuration_to_file(src: SEPatientConfiguration, filename: str):
    string = serialize_patient_configuration_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    n = file.write(string)
    file.close()

def serialize_patient_configuration_from_string(string: str, dst: SEPatientConfiguration, fmt: eSerializationFormat):
    src = PatientConfigurationData()
    json_format.Parse(string, src)
    serialize_patient_configuration_from_bind(src,dst)

def serialize_patient_configuration_from_file(filename: str, dst: SEPatientConfiguration):
    with open(filename) as f:
        string = f.read()
    serialize_patient_configuration_from_string(string, dst, eSerializationFormat.JSON)

def serialize_patient_configuration_to_bind(src: SEPatientConfiguration, dst: PatientConfigurationData):
    if src.has_patient_file():
        dst.PatientFile = src.get_patient_file()
    elif src.has_patient():
        serialize_patient_to_bind(src.get_patient(), dst.Patient)
    if src.has_conditions():
        serialize_condition_manager_to_bind(src.get_conditions(), dst.Conditions)
    dst.DataRoot = src.get_data_root_dir()

def serialize_patient_configuration_from_bind(src: PatientConfigurationData, dst: SEPatientConfiguration):
    raise Exception("serialize_patient_configuration_from_bind not implemented")


# Data Requests and Manager
def serialize_data_request_to_bind(src: SEDataRequest, dst: DataRequestData):
    if src.has_action_name():
        dst.ActionName = src.get_action_name()
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

def serialize_data_request_manager_to_file(src: SEDataRequestManager, filename: str):
    string = serialize_data_request_manager_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    n = file.write(string)
    file.close()
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
    if src.has_validation_targets():
        for vt in src.get_validation_targets():
            dst_vt = ValidationTargetData()
            serialize_validation_target_to_bind(vt, dst_vt)
            dst.ValidationTarget.append(dst_vt)
    dst.ResultsFilename = src.get_results_filename()

def serialize_data_request_manager_from_bind(src: DataRequestManagerData, dst: SEDataRequestManager):
    raise Exception("serialize_data_request_manager_from_bind not implemented")

def serialize_engine_initialization_to_bind(src: SEEngineInitialization, dst: EngineInitializationData):
    if src.id is not None:
        dst.ID = src.id
    if src.patient_configuration is not None:
        serialize_patient_configuration_to_bind(src.patient_configuration, dst.PatientConfiguration)
    elif src.state_filename is not None:
        dst.StateFilename = src.state_filename
    elif src.state is not None:
        dst.State = src.state
    else:
        raise Exception("Invalid Engine Configuration, must provide StartType")
    if src.data_request_mgr is not None:
        serialize_data_request_manager_to_bind(src.data_request_mgr, dst.DataRequestManager)
    dst.LogFilename = src.log_filename
    dst.KeepLogMessages = src.keep_log_messages
    dst.KeepEventChanges = src.keep_event_changes

def serialize_engine_initialization_to_string(src: SEEngineInitialization, fmt: eSerializationFormat):
    dst = EngineInitializationData()
    serialize_engine_initialization_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_engine_initializations_to_string(src: [SEEngineInitialization], fmt: eSerializationFormat):
    dst = EngineInitializationListData()
    for ec in src:
        bind = EngineInitializationData()
        serialize_engine_initialization_to_bind(ec, bind)
        dst.EngineInitialization.append(bind)
    return json_format.MessageToJson(dst, True, True)

def serialize_data_requested_result_from_bind(src: DataRequestedData, dst: SEDataRequested):
    dst.is_active = src.IsActive
    if not dst.is_active:
        return
    # TODO FINISH HIM!!
    # dst.results = src.Results

def serialize_data_requested_list_from_string(src: str, dst: [SEDataRequested], fmt: eSerializationFormat):
    results = DataRequestedListData()
    json_format.Parse(str, results)
    for id,result in results:
        e = dst[id]
        if e is None:
            raise Exception(str(id)+" not found in destination pool")
        serialize_data_requested_result_from_bind(result, e)

# Validation Targets
def serialize_validation_target_to_bind(src: SEValidationTarget, dst: ValidationTargetData):
    dst.Header = src.get_header()
    dst.Reference = src.get_reference()
    dst.Notes = src.get_notes()

    if src.get_comparison_type() == SEValidationTarget.eComparisonType.EqualTo:
        dst.EqualTo = src.get_target()
    elif src.get_comparison_type() == SEValidationTarget.eComparisonType.GreaterThan:
        dst.GreaterThan = src.get_target()
    elif src.get_comparison_type() == SEValidationTarget.eComparisonType.LessThan:
        dst.LessThan = src.get_target()
    elif src.get_comparison_type() == SEValidationTarget.eComparisonType.Increase:
        dst.Trend = ValidationTargetData.eTrend.Increase
    elif src.get_comparison_type() == SEValidationTarget.eComparisonType.Decrease:
        dst.Trend = ValidationTargetData.eTrend.Decrease
    elif src.get_comparison_type() == SEValidationTarget.eComparisonType.Range:
        dst.Range.Minimum = src.get_target_minimum()
        dst.Range.Maximum = src.get_target_maximum()
    else:
        raise ValueError(f"Unknown comparison type: {src.get_comparison_type()}")
def serialize_validation_target_from_bind(src: ValidationTargetData, dst: SEValidationTarget):
    dst.clear()
    dst.set_header(src.Header)
    dst.set_reference(src.Reference)
    dst.set_notes(src.Notes)

def serialize_segment_validation_target_to_bind(src: SESegmentValidationTarget, dst: SegmentValidationTargetData):
    serialize_validation_target_to_bind(src, dst.ValidationTarget)
    dst.Segment = src.get_segment()
def serialize_segment_validation_target_from_bind(src: SegmentValidationTargetData, dst: SESegmentValidationTarget):
    dst.clear()
    serialize_validation_target_from_bind(src.ValidationTarget, dst)
    base = src.ValidationTarget
    if base.HasField("EqualTo"):
        dst.set_equal_to(base.EqualTo, src.Segment)
    elif base.HasField("GreaterThan"):
        dst.set_greater_than(base.GreaterThan, src.Segment)
    elif base.HasField("LessThan"):
        dst.set_less_than(base.LessThan, src.Segment)
    elif base.HasField("Trend"):
        if base.Trend == ValidationTargetData.eTrend.Increase:
            dst.set_increase(src.Segment)
        elif base.Trend == ValidationTargetData.eTrend.Decrease:
            dst.set_decrease(src.Segment)
        else:
            raise ValueError(f"Unknown trend: {base.Trend}")
    elif base.HasField("Range"):
        dst.set_range(base.Range.Minimum, base.Range.Maximum, src.Segment)
    else:
        raise ValueError(f"Unknown expected field: {base.WhichOneOf('Expected')}")
def serialize_segment_validation_target_list_to_bind(src: List[SESegmentValidationTarget], dst: SegmentValidationTargetListData):
    for tgt in src:
        serialize_segment_validation_target_to_bind(tgt, dst.SegmentValidationTarget.add())
def serialize_segment_validation_target_list_from_bind(src: SegmentValidationTargetListData, dst: List[SESegmentValidationTarget]):
    dst = []
    for tgtData in src.SegmentValidationTarget:
        tgt = SESegmentValidationTarget()
        serialize_segment_validation_target_from_bind(tgtData, tgt)
        dst.append(tgt)
def serialize_segment_validation_target_list_to_string(src: List[SESegmentValidationTarget], fmt: eSerializationFormat):
    dst = SegmentValidationTargetListData()
    serialize_segment_validation_target_list_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_segment_validation_target_list_to_file(src: List[SESegmentValidationTarget], filename: str):
    string = serialize_segment_validation_target_list_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    n = file.write(string)
    file.close()
def serialize_segment_validation_target_list_from_string(string: str, dst: List[SESegmentValidationTarget], fmt: eSerializationFormat):
    src = SegmentValidationTargetListData()
    json_format.Parse(string, src)
    serialize_segment_validation_target_list_from_bind(src, dst)
def serialize_segment_validation_target_list_from_file(filename: str, dst: SESegmentValidationTarget):
    with open(filename) as f:
        string = f.read()
    serialize_segment_validation_target_list_from_string(string, dst, eSerializationFormat.JSON)

def serialize_time_series_validation_target_to_bind(src: SETimeSeriesValidationTarget, dst: TimeSeriesValidationTargetData):
    serialize_validation_target_to_bind(src, dst.ValidationTarget)
    dst.Type = src.get_target_type().value
def serialize_time_series_validation_target_from_bind(src: TimeSeriesValidationTargetData, dst: SETimeSeriesValidationTarget):
    dst.clear()
    serialize_validation_target_from_bind(src.ValidationTarget, dst)
    base = src.ValidationTarget
    if base.HasField("EqualTo"):
        dst.set_equal_to(base.EqualTo, SETimeSeriesValidationTarget.eTargetType(src.Type))
    elif base.HasField("GreaterThan"):
        dst.set_greater_than(base.GreaterThan, SETimeSeriesValidationTarget.eTargetType(src.Type))
    elif base.HasField("LessThan"):
        dst.set_less_than(base.LessThan, SETimeSeriesValidationTarget.eTargetType(src.Type))
    elif base.HasField("Trend"):
        if base.Trend == ValidationTargetData.eTrend.Increase:
            dst.set_increase(SETimeSeriesValidationTarget.eTargetType(src.Type))
        elif base.Trend == ValidationTargetData.eTrend.Decrease:
            dst.set_decrease(SETimeSeriesValidationTarget.eTargetType(src.Type))
        else:
            raise ValueError(f"Unknown trend: {base.Trend}")
    elif base.HasField("Range"):
        dst.set_range(base.Range.Minimum, base.Range.Maximum, SETimeSeriesValidationTarget.eTargetType(src.Type))
    else:
        raise ValueError(f"Unknown expected field: {base.WhichOneOf('Expected')}")
def serialize_time_series_validation_target_list_to_bind(src: List[SETimeSeriesValidationTarget], dst: TimeSeriesValidationTargetListData):
    for tgt in src:
        serialize_time_series_validation_target_to_bind(tgt, dst.TimeSeriesValidationTarget.add())
def serialize_time_series_validation_target_list_from_bind(src: TimeSeriesValidationTargetListData, dst: List[SETimeSeriesValidationTarget]):
    dst = []
    for tgtData in src.TimeSeriesValidationTarget:
        tgt = SETimeSeriesValidationTarget()
        serialize_time_series_validation_target_from_bind(tgtData, tgt)
        dst.append(tgt)
def serialize_time_series_validation_target_list_to_string(src: List[SETimeSeriesValidationTarget], fmt: eSerializationFormat):
    dst = TimeSeriesValidationTargetListData()
    serialize_time_series_validation_target_list_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_time_series_validation_target_list_to_file(src: List[SETimeSeriesValidationTarget], filename: str):
    string = serialize_time_series_validation_target_list_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    n = file.write(string)
    file.close()
def serialize_time_series_validation_target_list_from_string(string: str, dst: List[SETimeSeriesValidationTarget], fmt: eSerializationFormat):
    src = TimeSeriesValidationTargetListData()
    json_format.Parse(string, src)
    serialize_time_series_validation_target_list_from_bind(src, dst)
def serialize_time_series_validation_target_list_from_file(filename: str, dst: List[SETimeSeriesValidationTarget]):
    with open(filename) as f:
        string = f.read()
    serialize_time_series_validation_target_list_from_string(string, dst, eSerializationFormat.JSON)
