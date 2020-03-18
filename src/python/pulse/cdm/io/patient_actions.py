# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.io.action import serialize_action_from_bind, serialize_action_to_bind

from pulse.cdm.patient_actions import *
from pulse.cdm.bind.PatientActions_pb2 import *
from pulse.cdm.io.scalars import *

def serialize_patient_action_to_bind(src: SEPatientAction, dst: PatientActionData):
    serialize_action_to_bind(src,dst.Action)

def serialize_patient_action_from_bind(src: PatientActionData, dst: SEPatientAction):
    raise Exception("serialize_patient_action_from_bind not implemented")


def serialize_exercise_to_bind(src: SEExercise, dst: ExerciseData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_intensity(), dst.Intensity)

def serialize_exercise_from_bind(src: ExerciseData, dst: SEExercise):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_exercise_from_bind not implemented")


def serialize_asthma_attack_to_bind(src: SEAsthmaAttack, dst: AsthmaAttackData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_asthma_attack_from_bind(src: AsthmaAttackData, dst: SEAsthmaAttack):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_asthma_attack_from_bind not implemented")


def serialize_acute_respiratory_exacerbation_to_bind(src: SEAcuteRespiratoryDistressSyndromeExacerbation,
                                    dst: AcuteRespiratoryDistressSyndromeExacerbationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    serialize_scalar_0to1_to_bind(src.get_left_lung_affected(), dst.LeftLungAffected)
    serialize_scalar_0to1_to_bind(src.get_right_lung_affected(), dst.RightLungAffected)

def serialize_acute_respiratory_exacerbation_from_bind(src: AsthmaAttackData, dst: SEAsthmaAttack):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_acute_respiratory_exacerbation_from_bind not implemented")


def serialize_acute_stress_to_bind(src: SEAcuteStress, dst: AcuteStressData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_acute_stress_from_bind(src:AcuteStressData, dst: SEAcuteStress):
    serialize_patient_action_from_bind(src.PatientAction, dst)


def serialize_airway_obstruction_to_bind(src: SEAirwayObstruction, dst: AirwayObstructionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_airway_obstruction_from_bind(src:AirwayObstructionData, dst: SEAirwayObstruction):
    serialize_patient_action_from_bind(src.PatientAction, dst)


def serialize_brain_injury_to_bind(src:SEBrainInjury, dst: BrainInjuryData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    dst.Type = src.get_injury_type()

def serialize_brain_injury_from_bind(src: BrainInjuryData, dst: SEBrainInjury):
    serialize_patient_action_from_bind(src.PatientAction, dst)


def serialize_hemorrhage_to_bind(src: SEHemorrhage, dst: HemorrhageData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Compartment = src.get_compartment()
    dst.Type = src.get_type().value
    serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)

def serialize_hemorrhage_from_bind(src: HemorrhageData, dst: SEHemorrhage):
    raise Exception("serialize_hemorrhage_from_bind not implemented")