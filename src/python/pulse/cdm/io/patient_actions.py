# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.io.action import serialize_action_from_bind, serialize_action_to_bind

from pulse.cdm.patient_actions import SEPatientAction
from pulse.cdm.bind.PatientActions_pb2 import PatientActionData
from pulse.cdm.patient_actions import SEExercise, SEHemorrhage
from pulse.cdm.bind.PatientActions_pb2 import ExerciseData, HemorrhageData

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

def serialize_hemorrhage_to_bind(src: SEHemorrhage, dst: HemorrhageData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Compartment = src.get_compartment()
    dst.Type = src.get_type().value
    serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)

def serialize_hemorrhage_from_bind(src: HemorrhageData, dst: SEHemorrhage):
    raise Exception("serialize_hemorrhage_from_bind not implemented")