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
    dst.Type = src.get_injury_type().value

def serialize_brain_injury_from_bind(src: BrainInjuryData, dst: SEBrainInjury):
    serialize_patient_action_from_bind(src.PatientAction, dst)


def serialize_broncho_constriction_to_bind(src: SEBronchoConstriction, dst: BronchoconstrictionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)


def serialize_broncho_constriction_from_bind(src: BronchoconstrictionData, dst: SEBronchoConstriction):
    serialize_patient_action_from_bind(src.PatientAction, dst)


def serialize_chest_occlusive_dressing_to_bind(src: SEChestOcclusiveDressing, dst: ChestOcclusiveDressingData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Side = src.get_side().value
    dst.State = src.get_state().value
def serialize_chest_occlusive_dressing_from_bind(src:ChestOcclusiveDressingData, dst: SEChestOcclusiveDressing):
    serialize_patient_action_from_bind(src.PatientAction, dst)


def serialize_dsypnea_to_bind(src: SEDyspnea, dst: DyspneaData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_dsypnea_from_bind(src:DyspneaData, dst: SEDyspnea):
    serialize_patient_action_from_bind(dst.PatientAction, src)


def serialize_hemorrhage_to_bind(src: SEHemorrhage, dst: HemorrhageData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Compartment = src.get_compartment()
    dst.Type = src.get_type().value
    serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)

def serialize_hemorrhage_from_bind(src: HemorrhageData, dst: SEHemorrhage):
    raise Exception("serialize_hemorrhage_from_bind not implemented")


def serialize_intubation_to_bind(src:SEIntubation, dst:IntubationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Type = src.get_type().value
def serialize_intubation_from_bind(src:IntubationData, dst:SEIntubation):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_lobar_pneumonia_exacerbation_to_bind(src:SELobarPneumoniaExacerbation, dst: LobarPneumoniaExacerbationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    serialize_scalar_0to1_to_bind(src.get_right_lung_affected(), dst.RightLungAffected)
    serialize_scalar_0to1_to_bind(src.get_left_lung_affected(), dst.LeftLungAffected)
def serialize_lobar_pneumonia_exacerbation_from_bind(src:LobarPneumoniaExacerbationData, dst: SELobarPneumoniaExacerbation ):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_needle_decompression_to_bind(src: SENeedleDecompression, dst: NeedleDecompressionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Side = src.get_side().value
    dst.State = src.get_state().value
def serialize_needle_decompression_from_bind(src:NeedleDecompressionData, dst: SENeedleDecompression):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_pericardial_effusion_to_bind(src: SEPericardialEffusion, dst:PericardialEffusionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_volume_per_time_to_bind(src.get_effusion_rate(), dst.EffusionRate)
def serialize_pericardial_effusion_from_bind(src: PericardialEffusionData, dst: SEPericardialEffusion):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_respiratory_fatigue_to_bind(src:SERespiratoryFatigue, dst: RespiratoryFatigueData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_respiratory_fatigue_from_bind(src: RespiratoryFatigueData, dst: SERespiratoryFatigue):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_substance_bolus_to_bind(src:SESubstanceBolus, dst: SubstanceBolusData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_mass_per_volume_to_bind(src.get_concentration(), dst.Concentration)
    serialize_scalar_volume_to_bind(src.get_dose(), dst.Dose)
    dst.Substance = src.get_substance()
    dst.AdministrationRoute = src.get_admin_route().value

def serialize_substance_bolus_from_bind(src: SubstanceBolusData, dst: SESubstanceBolus):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_substance_compound_infusion_to_bind(src:SESubstanceCompoundInfusion, dst: SubstanceCompoundInfusionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_volume_to_bind(src.get_bag_volume(), dst.BagVolume)
    serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)
    dst.SubstanceCompound = src.get_compound()

def serialize_substance_compound_infusion_from_bind(src: SubstanceCompoundInfusionData, dst: SESubstanceCompoundInfusion):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_substance_infusion_to_bind(src:SESubstanceInfusion, dst: SubstanceInfusionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_mass_per_volume_to_bind(src.get_concentration(), dst.Concentration)
    serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)
    dst.Substance = src.get_substance()

def serialize_substance_infusion_from_bind(src: SubstanceInfusionData, dst: SESubstanceInfusion):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_supplemental_oxygen_to_bind(src: SESupplementalOxygen, dst: SupplementalOxygenData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_volume_per_time_to_bind(src.get_flow(), dst.Flow)
    serialize_scalar_volume_to_bind(src.get_volume(), dst.Volume)
    dst.Device = src.get_device().value
def serialize_supplemental_oxygen_from_bind(src: SupplementalOxygenData, dst: SESupplementalOxygen):
    serialize_patient_action_from_bind(src.PatientAction, dst)

def serialize_tension_pneumothorax_to_bind(src: SETensionPneumothorax, dst: TensionPneumothoraxData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    dst.Side = src.get_side().value
    dst.Type = src.get_type().value
def serialize_tension_pneumothorax_from_bind(src: TensionPneumothoraxData, dst: SETensionPneumothorax):
    serialize_patient_action_from_bind(src.PatientAction, dst)