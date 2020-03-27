# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.io.condition import *

from pulse.cdm.patient_conditions import *
from pulse.cdm.bind.PatientConditions_pb2 import *
from pulse.cdm.io.scalars import *

#################################################################
def serialize_patient_condition_to_bind(src: SEPatientCondition, dst: PatientConditionData):
    serialize_condition_to_bind(src,dst.condition)

def serialize_patient_condition_from_bind(src: PatientConditionData, dst: SEPatientCondition):
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################
def serialize_acute_respiratory_distress_syndrome_to_bind(src: SEAcuteRespiratoryDistressSyndrome,
                                                          dst: AcuteRespiratoryDistressSyndromeData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    serialize_scalar_0to1_to_bind(src.get_left_lung_affected(), dst.LeftLungAffected)
    serialize_scalar_0to1_to_bind(src.get_right_lung_affected(), dst.RightLungAffected)
def serialize_acute_respiratory_distress_syndrome_from_bind(src: AcuteRespiratoryDistressSyndromeData,
                                                            dst:SEAcuteRespiratoryDistressSyndrome):
    serialize_patient_condition_from_bind(src.PatientAction, dst)

#################################################################
def serialize_chronic_anemia_to_bind(src: SEChronicAnemia , dst: ChronicAnemiaData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_reduction_factor(), dst.ReductionFactor)
def serialize_chronic_anemia_from_bind(src: ChronicAnemiaData , dst: SEChronicAnemia):
    serialize_patient_condition_from_bind(src.PatientAction, dst)

#################################################################
#def serialize_chronic_heart_failure_to_bind(src: SEChronicHeartFailure , dst:)
#    raise Exception("serialize_patient_condition_from_bind not implemented")
#def serialize_chronic_heart_failure_from_bind(src: , dst:):
#    raise Exception("serialize_patient_condition_from_bind not implemented")
#################################################################


def serialize_chronic_obstructive_pulmonary_disease_to_bind(src:  SEChronicObstructivePulmonaryDisease,
                                                            dst: ChronicObstructivePulmonaryDiseaseData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_bronchitis_severity(), dst.BronchitisSeverity)
    serialize_scalar_0to1_to_bind(src.get_emphysema_severity(), dst.EmphysemaSeverity)
def serialize_chronic_obstructive_pulmonary_disease_from_bind(src: ChronicObstructivePulmonaryDiseaseData,
                                                              dst:SEChronicObstructivePulmonaryDisease):
    serialize_patient_condition_from_bind(src.PatientAction, dst)
#################################################################


def serialize_chronic_pericardial_effusion_to_bind(src: SEChronicPericardialEffusion,
                                                   dst: ChronicPericardialEffusionData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_accumulated_volume(), dst.AccumulatedVolume)
def serialize_chronic_pericardial_effusion_from_bind(src: ChronicPericardialEffusionData,
                                                     dst: SEChronicPericardialEffusion):
    serialize_patient_condition_from_bind(src.PatientAction, dst)
#################################################################


def serialize_chronic_renal_stenosis_to_bind(src: SEChronicRenalStenosis, dst: ChronicRenalStenosisData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_left_kidney_severity(), dst.LeftKidneySeverity)
    serialize_scalar_0to1_to_bind(src.get_right_kidney_severity(), dst.RightKidneySeverity)
def serialize_chronic_renal_stenosis_from_bind(src: ChronicRenalStenosisData , dst: SEChronicRenalStenosis):
    serialize_patient_condition_from_bind(src.PatientAction, dst)
#################################################################


#def serialize_chronic_ventricular_systolic_dysfunction_to_bind(src: , dst:):
#def serialize_chronic_ventricular_systolic_dysfunction_from_bind(src: , dst:):
#################################################################


def serialize_impaired_alveolar_exchange_to_bind(src: SEImpairedAlveolarExchange , dst: ImpairedAlveolarExchangeData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_impaired_fraction(), dst.ImpairedFraction)
    serialize_scalar_area_to_bind(src.get_impaired_surface_area(), dst.ImpairedSurfaceArea)
def serialize_impaired_alveolar_exchange_from_bind(src: ImpairedAlveolarExchangeData, dst: SEImpairedAlveolarExchange):
    serialize_patient_condition_from_bind(src.PatientAction, dst)
#################################################################


def serialize_lobar_pneumonia_to_bind(src: SELobarPneumonia, dst: LobarPneumoniaData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    serialize_scalar_0to1_to_bind(src.get_left_lung_affected(), dst.LeftLungAffected)
    serialize_scalar_0to1_to_bind(src.get_right_lung_affected(), dst.RightLungAffected)
def serialize_lobar_pneumonia_from_bind(src: LobarPneumoniaData, dst: SELobarPneumonia):
    serialize_patient_condition_from_bind(src.PatientAction, dst)
#################################################################


def serialize_pulmonary_fibrosis_to_bind(src: SEPulmonaryFibrosis , dst: PulmonaryFibrosisData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
def serialize_pulmonary_fibrosis_from_bind(src: PulmonaryFibrosisData, dst: SEPulmonaryFibrosis):
    serialize_patient_condition_from_bind(src.PatientAction, dst)
#################################################################


def serialize_sepsis_to_bind(src: SESepsis, dst:SepsisData):
    serialize_patient_condition_to_bind(src, dst.PatientCondition)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
def serialize_sepsis_from_bind(src: SepsisData, dst: SESepsis):
    serialize_patient_condition_from_bind(src.PatientAction, dst)
