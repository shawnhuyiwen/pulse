# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from pulse.cdm.engine import eSerializationFormat
from google.protobuf import json_format

from pulse.cdm.engine import SEAction, SECondition, SEConditionManager
from pulse.cdm.bind.Actions_pb2 import ActionData
from pulse.cdm.bind.Conditions_pb2 import ConditionData
from pulse.cdm.bind.Engine_pb2 import ActionListData, AnyActionData, \
                                      AnyConditionData, ConditionListData, \
                                      PatientConfigurationData
# Environment
from pulse.cdm.environment import eSurroundingType, SEEnvironmentalConditions, \
                                  SEAppliedTemperature, SEActiveConditioning
from pulse.cdm.environment_conditions import SEEnvironmentCondition, \
                                             SEInitialEnvironmentalConditions
from pulse.cdm.bind.Environment_pb2 import EnvironmentalConditionsData, \
                                           AppliedTemperatureData, \
                                           ActiveConditioningData
from pulse.cdm.bind.EnvironmentActions_pb2 import EnvironmentActionData, \
                                                  AnyEnvironmentActionData, \
                                                  ChangeEnvironmentConditionsData
from pulse.cdm.bind.EnvironmentConditions_pb2 import EnvironmentConditionData, \
                                                     AnyEnvironmentConditionData, \
                                                     InitialEnvironmentConditionsData
# Substances
from pulse.cdm.bind.Substance_pb2 import SubstanceConcentrationData, SubstanceFractionData
# Patient
from pulse.cdm.patient import SEPatient, eSex, SEPatientConfiguration
from pulse.cdm.bind.Patient_pb2 import PatientData
# Patient Actions
from pulse.cdm.patient_actions import SEPatientAction
from pulse.cdm.bind.PatientActions_pb2 import PatientActionData
from pulse.cdm.patient_actions import SEExercise, SEHemorrhage
from pulse.cdm.bind.PatientActions_pb2 import ExerciseData, HemorrhageData

# Scalars
from pulse.cdm.scalars import SEScalar, SEScalar0To1, SEScalarNegative1To1, \
                              SEScalarArea, AreaUnit, \
                              SEScalarFrequency, FrequencyUnit, \
                              SEScalarHeatResistanceArea, HeatResistanceAreaUnit, \
                              SEScalarInversePressure, InversePressureUnit, \
                              SEScalarLength, LengthUnit, \
                              SEScalarLengthPerTime, LengthPerTimeUnit, \
                              SEScalarMass, MassUnit, \
                              SEScalarMassPerAmount, MassPerAmountUnit, \
                              SEScalarMassPerAreaTime, MassPerAreaTimeUnit, \
                              SEScalarMassPerTime, MassPerTimeUnit, \
                              SEScalarMassPerVolume, MassPerVolumeUnit, \
                              SEScalarPower, PowerUnit, \
                              SEScalarPressure, PressureUnit, \
                              SEScalarTemperature, TemperatureUnit, \
                              SEScalarTime, TimeUnit, \
                              SEScalarVolume, VolumeUnit, \
                              SEScalarVolumePerTime, VolumePerTimeUnit, \
                              SEScalarVolumePerTimeMass, VolumePerTimeMassUnit, \
                              SEScalarVolumePerTimePressure, VolumePerTimePressureUnit
from pulse.cdm.bind.Properties_pb2 import ScalarData, Scalar0To1Data, ScalarNegative1To1Data, \
                                          ScalarAreaData,ScalarFrequencyData,ScalarHeatResistanceAreaData, \
                                          ScalarInversePressureData, ScalarLengthData, ScalarLengthPerTimeData, \
                                          ScalarMassData, ScalarMassPerAmountData, ScalarMassPerAreaTimeData, \
                                          ScalarMassPerTimeData, ScalarMassPerVolumeData, ScalarPowerData, \
                                          ScalarPressureData, ScalarTemperatureData, ScalarTimeData, \
                                          ScalarVolumeData, ScalarVolumePerTimeData, ScalarVolumePerTimeMassData, \
                                          ScalarVolumePerTimePressureData

# Actions
def _serialize_action_to_bind(src: SEAction, dst: ActionData):
    dst.Comment = src.get_comment()
def serialize_actions_to_string(actions: [], fmt: eSerializationFormat):
    action_list = ActionListData()
    for action in actions:
        if not action.is_valid():
            print("Skipping invalid action")
            continue
        print(action)
        any_action = AnyActionData()
        if isinstance(action, SEExercise):
            _serialize_exercise_to_bind(action, any_action.PatientAction.Exercise)
            action_list.AnyAction.append(any_action)
            continue
        if isinstance(action, SEHemorrhage):
            _serialize_hemorrhage_to_bind(action, any_action.PatientAction.Hemorrhage)
            action_list.AnyAction.append(any_action)
            continue
    return json_format.MessageToJson(action_list,True,True)

# Conditions
def _serialize_condition_to_bind(src: SECondition, dst: ConditionData):
    dst.Comment = src.get_comment()
def serialize_condition_manager_to_string(condition_manager: SEConditionManager, fmt: eSerializationFormat):
    dst = ConditionListData()
    _serialize_condition_manager_to_bind(condition_manager, dst)
    return json_format.MessageToJson(dst,True,True)
def _serialize_condition_manager_to_bind(condition_manager: SEConditionManager, dst: ConditionListData):
    if (condition_manager.is_empty()):
        return ""
    if condition_manager.has_initial_environmental_conditions():
        any_condition = AnyConditionData()
        _serialize_initial_environmental_conditions_to_bind(condition_manager.get_initial_environmental_conditions(),
                                                           any_condition.EnvironmentCondition.InitialEnvironmentConditions)

        dst.AnyCondition.append(any_condition)


# Environmental Conditions
def serialize_environmental_conditions_to_string(src: SEEnvironmentalConditions, fmt: eSerializationFormat):
    dst = EnvironmentalConditionsData()
    _serialize_environmental_conditions_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_environmental_conditions_to_file(src: SEEnvironmentalConditions, filename: str, fmt: eSerializationFormat):
    string = serialize_environmental_conditions_to_string(src, fmt)
    file = open(filename, "w")
    n = file.write(string)
    file.close()
def serialize_environmental_conditions_from_string(string: str, dst: SEEnvironmentalConditions, fmt: eSerializationFormat):
    src = EnvironmentalConditionsData()
    json_format.Parse(string, src)
    _serialize_environmental_conditions_from_bind(src,dst)
def serialize_environmental_conditions_from_file(filename: str, dst: SEEnvironmentalConditions, fmt: eSerializationFormat):
    with open(filename) as f:
        string = f.read()
    serialize_environmental_conditions_from_string(string, dst, fmt)

def _serialize_environmental_conditions_to_bind(src: SEEnvironmentalConditions, dst: EnvironmentalConditionsData):
    dst.SurroundingType = src.get_surrounding_type().value
    if src.has_air_density():
        _serialize_scalar_mass_per_volume_to_bind(src.get_air_density(), dst.AirDensity)
    if src.has_air_velocity():
        _serialize_scalar_length_per_time_to_bind(src.get_air_velocity(), dst.AirVelocity)
    if src.has_ambient_temperature():
        _serialize_scalar_temperature_to_bind(src.get_ambient_temperature(), dst.AmbientTemperature)
    if src.has_atmospheric_pressure():
        _serialize_scalar_pressure_to_bind(src.get_atmospheric_pressure(), dst.AtmosphericPressure)
    if src.has_clothing_resistance():
        _serialize_scalar_heat_resistance_area_to_bind(src.get_clothing_resistance(), dst.ClothingResistance)
    if src.has_emissivity():
        _serialize_scalar_0to1_to_bind(src.get_emissivity(), dst.Emissivity)
    if src.has_mean_radiant_temperature():
        _serialize_scalar_temperature_to_bind(src.get_mean_radiant_temperature(), dst.MeanRadiantTemperature)
    if src.has_relative_humidity():
        _serialize_scalar_0to1_to_bind(src.get_relative_humidity(), dst.RelativeHumidity)
    if src.has_respiration_ambient_temperature():
        _serialize_scalar_temperature_to_bind(src.get_respiration_ambient_temperature(), dst.RespirationAmbientTemperature)
    for aGas in src.get_ambient_gasses():
        sf = SubstanceFractionData()
        sf.Name = aGas.get_substance()
        _serialize_scalar_0to1_to_bind(aGas.get_fraction_amount(), sf.Amount)
        dst.AmbientGas.append(sf)
    for aAerosol in src.get_ambient_aerosols():
        sc = SubstanceConcentrationData()
        sc.Name = aAerosol.get_substance()
        _serialize_scalar_mass_per_volume_to_bind(aAerosol.get_concentration(), sc.Concentration)
        dst.AmbientAerosol.append(sc)
def _serialize_environmental_conditions_from_bind(src: EnvironmentalConditionsData, dst: SEEnvironmentalConditions):
    dst.set_surrounding_type(eSurroundingType(src.SurroundingType))
    if src.HasField("AirDensity"):
        _serialize_scalar_mass_per_volume_from_bind(src.AirDensity, dst.get_air_density())
    if src.HasField("AirVelocity"):
        _serialize_scalar_length_per_time_from_bind(src.AirVelocity, dst.get_air_velocity())
    if src.HasField("AmbientTemperature"):
        _serialize_scalar_temperature_from_bind(src.AmbientTemperature, dst.get_ambient_temperature())
    if src.HasField("AtmosphericPressure"):
        _serialize_scalar_pressure_from_bind(src.AtmosphericPressure, dst.get_atmospheric_pressure())
    if src.HasField("ClothingResistance"):
        _serialize_scalar_heat_resistance_area_from_bind(src.ClothingResistance, dst.get_clothing_resistance())
    if src.HasField("Emissivity"):
        _serialize_scalar_0to1_from_bind(src.Emissivity, dst.get_emissivity())
    if src.HasField("MeanRadiantTemperature"):
        _serialize_scalar_temperature_from_bind(src.MeanRadiantTemperature, dst.get_mean_radiant_temperature())
    if src.HasField("RelativeHumidity"):
        _serialize_scalar_0to1_from_bind(src.RelativeHumidity, dst.get_relative_humidity())
    if src.HasField("RespirationAmbientTemperature"):
        _serialize_scalar_temperature_from_bind(src.RespirationAmbientTemperature, dst.get_respiration_ambient_temperature())
    for aGas in src.AmbientGas:
        _serialize_scalar_0to1_from_bind(aGas.Amount, dst.get_ambient_gas(aGas.Name).get_fraction_amount())
    for aAerosol in src.AmbientAerosol:
        _serialize_scalar_mass_per_volume_from_bind(aAerosol.Concentration, dst.get_ambient_aerosol(aAerosol.Name).get_concentration())
# Environment Conditions
def _serialize_initial_environmental_conditions_to_bind(src: SEInitialEnvironmentalConditions, dst: EnvironmentalConditionsData):
    if src.has_conditions_file():
        dst.ConditionsFile = src.get_conditions_file()
    elif src.has_conditions():
        _serialize_environmental_conditions_to_bind(src.get_conditions(), dst.Conditions)
def _serialize_initial_environmental_conditions_from_bind(src: EnvironmentalConditionsData, dst: SEInitialEnvironmentalConditions):
    raise Exception("_serialize_initial_environmental_conditions_from_bind not implemented")
# Environment Actions

# Patient
def serialize_patient_to_string(src: SEPatient, fmt: eSerializationFormat):
    dst = PatientData()
    _serialize_patient_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_patient_to_file(src: SEPatient, filename: str, fmt: eSerializationFormat):
    string = serialize_patient_to_string(src, fmt)
    file = open(filename, "w")
    n = file.write(string)
    file.close()
def serialize_patient_from_string(string: str, dst: SEPatient, fmt: eSerializationFormat):
    src = PatientData()
    json_format.Parse(string, src)
    _serialize_patient_from_bind(src,dst)
def serialize_patient_from_file(filename: str, dst: SEPatient, fmt: eSerializationFormat):
    with open(filename) as f:
        string = f.read()
    serialize_patient_from_string(string, dst, fmt)

def _serialize_patient_to_bind(src: SEPatient, dst: PatientData):
    dst.Name = src.get_name()
    dst.Sex = src.get_sex().value
    if src.has_age():
        _serialize_scalar_time_to_bind(src.get_age(), dst.Age)
    if src.has_weight():
        _serialize_scalar_mass_to_bind(src.get_weight(), dst.Weight)
    if src.has_height():
        _serialize_scalar_length_to_bind(src.get_height(), dst.Height)
    if src.has_body_density():
        _serialize_scalar_mass_per_volume_to_bind(src.get_body_density(), dst.BodyDensity)
    if src.has_body_fat_fraction():
        _serialize_scalar_0to1_to_bind(src.get_body_fat_fraction(), dst.BodyFatFraction)
    if src.has_ideal_body_weight():
        _serialize_scalar_mass_to_bind(src.get_ideal_body_weight(), dst.IdealBodyWeight)
    if src.has_lean_body_mass():
        _serialize_scalar_mass_to_bind(src.get_lean_body_mass(), dst.LeanBodyMass)
    if src.has_alveoli_surface_area():
        _serialize_scalar_area_to_bind(src.get_alveoli_surface_area(), dst.AlveoliSurfaceArea)
    if src.has_right_lung_ratio():
        _serialize_scalar_0to1_to_bind(src.get_right_lung_ratio(), dst.RightLungRatio)
    if src.has_skin_surface_area():
        _serialize_scalar_area_to_bind(src.get_skin_surface_area(), dst.SkinSurfaceArea)
    if src.has_basal_metabolic_rate():
        _serialize_scalar_power_to_bind(src.get_basal_metabolic_rate(), dst.BasalMetabolicRate)
    if src.has_blood_volume_baseline():
        _serialize_scalar_volume_to_bind(src.get_blood_volume_baseline(), dst.BloodVolumeBaseline)
    if src.has_diastolic_arterial_pressure_baseline():
        _serialize_scalar_pressure_to_bind(src.get_diastolic_arterial_pressure_baseline(), dst.DiastolicArterialPressureBaseline)
    if src.has_heart_rate_baseline():
        _serialize_scalar_frequency_to_bind(src.get_heart_rate_baseline(), dst.HeartRateBaseline)
    if src.has_mean_arterial_pressure_baseline():
        _serialize_scalar_pressure_to_bind(src.get_mean_arterial_pressure_baseline(), dst.MeanArterialPressureBaseline)
    if src.has_respiration_rate_baseline():
        _serialize_scalar_frequency_to_bind(src.get_respiration_rate_baseline(), dst.RespirationRateBaseline)
    if src.has_systolic_arterial_pressure_baseline():
        _serialize_scalar_pressure_to_bind(src.get_systolic_arterial_pressure_baseline(), dst.SystolicArterialPressureBaseline)
    if src.has_tidal_volume_baseline():
        _serialize_scalar_volume_to_bind(src.get_tidal_volume_baseline(), dst.TidalVolumeBaseline)
    if src.has_heart_rate_maximum():
        _serialize_scalar_frequency_to_bind(src.get_heart_rate_maximum(), dst.HeartRateMaximum)
    if src.has_heart_rate_minimum():
        _serialize_scalar_frequency_to_bind(src.get_heart_rate_minimum(), dst.HeartRateMinimum)
    if src.has_expiratory_reserve_volume():
        _serialize_scalar_volume_to_bind(src.get_expiratory_reserve_volume(), dst.ExpiratoryReserveVolume)
    if src.has_functional_residual_capacity():
        _serialize_scalar_volume_to_bind(src.get_functional_residual_capacity(), dst.FunctionalResidualCapacity)
    if src.has_inspiratory_capacity():
        _serialize_scalar_volume_to_bind(src.get_inspiratory_capacity(), dst.InspiratoryCapacity)
    if src.has_inspiratory_reserve_volume():
        _serialize_scalar_volume_to_bind(src.get_inspiratory_reserve_volume(), dst.InspiratoryReserveVolume)
    if src.has_residual_volume():
        _serialize_scalar_volume_to_bind(src.get_residual_volume(), dst.ResidualVolume)
    if src.has_total_lung_capacity():
        _serialize_scalar_volume_to_bind(src.get_total_lung_capacity(), dst.TotalLungCapacity)
    if src.has_vital_capacity():
        _serialize_scalar_volume_to_bind(src.get_vital_capacity(), dst.VitalCapacity)
def _serialize_patient_from_bind(src: PatientData, dst: SEPatient):
    dst.set_name(src.Name)
    dst.set_sex(eSex(src.Sex))
    if src.HasField("Age"):
        _serialize_scalar_time_from_bind(src.Age, dst.get_age())
    if src.HasField("Weight"):
        _serialize_scalar_mass_from_bind(src.Weight, dst.get_weight())
    if src.HasField("Height"):
        _serialize_scalar_length_from_bind(src.Height, dst.get_height())
    if src.HasField("BodyDensity"):
        _serialize_scalar_mass_per_volume_from_bind(src.BodyDensity, dst.get_body_density())
    if src.HasField("BodyFatFraction"):
        _serialize_scalar_0to1_from_bind(src.BodyFatFraction, dst.get_body_fat_fraction())
    if src.HasField("IdealBodyWeight"):
        _serialize_scalar_mass_from_bind(src.IdealBodyWeight, dst.get_ideal_body_weight())
    if src.HasField("LeanBodyMass"):
        _serialize_scalar_mass_from_bind(src.LeanBodyMass, dst.get_lean_body_mass())
    if src.HasField("AlveoliSurfaceArea"):
        _serialize_scalar_area_from_bind(src.AlveoliSurfaceArea, dst.get_alveoli_surface_area())
    if src.HasField("RightLungRatio"):
        _serialize_scalar_0to1_from_bind(src.RightLungRatio, dst.get_right_lung_ratio())
    if src.HasField("SkinSurfaceArea"):
        _serialize_scalar_area_from_bind(src.SkinSurfaceArea, dst.get_skin_surface_area())
    if src.HasField("BasalMetabolicRate"):
        _serialize_scalar_power_from_bind(src.BasalMetabolicRate, dst.get_basal_metabolic_rate())
    if src.HasField("BloodVolumeBaseline"):
        _serialize_scalar_volume_from_bind(src.BloodVolumeBaseline, dst.get_blood_volume_baseline())
    if src.HasField("DiastolicArterialPressureBaseline"):
        _serialize_scalar_pressure_from_bind(src.DiastolicArterialPressureBaseline, dst.get_diastolic_arterial_pressure_baseline())
    if src.HasField("HeartRateBaseline"):
        _serialize_scalar_frequency_from_bind(src.HeartRateBaseline, dst.get_heart_rate_baseline())
    if src.HasField("MeanArterialPressureBaseline"):
        _serialize_scalar_pressure_from_bind(src.MeanArterialPressureBaseline, dst.get_mean_arterial_pressure_baseline())
    if src.HasField("RespirationRateBaseline"):
        _serialize_scalar_frequency_from_bind(src.RespirationRateBaseline, dst.get_respiration_rate_baseline())
    if src.HasField("SystolicArterialPressureBaseline"):
        _serialize_scalar_pressure_from_bind(src.SystolicArterialPressureBaseline, dst.get_systolic_arterial_pressure_baseline())
    if src.HasField("TidalVolumeBaseline"):
        _serialize_scalar_volume_from_bind(src.TidalVolumeBaseline, dst.get_tidal_volume_baseline())
    if src.HasField("HeartRateMaximum"):
        _serialize_scalar_frequency_from_bind(src.HeartRateMaximum, dst.get_heart_rate_maximum())
    if src.HasField("HeartRateMinimum"):
        _serialize_scalar_frequency_from_bind(src.HeartRateMinimum, dst.get_heart_rate_minimum())
    if src.HasField("ExpiratoryReserveVolume"):
        _serialize_scalar_volume_from_bind(src.ExpiratoryReserveVolume, dst.get_expiratory_reserve_volume())
    if src.HasField("FunctionalResidualCapacity"):
        _serialize_scalar_volume_from_bind(src.FunctionalResidualCapacity, dst.get_functional_residual_capacity())
    if src.HasField("InspiratoryCapacity"):
        _serialize_scalar_volume_from_bind(src.InspiratoryCapacity, dst.get_inspiratory_capacity())
    if src.HasField("InspiratoryReserveVolume"):
        _serialize_scalar_volume_from_bind(src.InspiratoryReserveVolume, dst.get_inspiratory_reserve_volume())
    if src.HasField("ResidualVolume"):
        _serialize_scalar_volume_from_bind(src.ResidualVolume, dst.get_residual_volume())
    if src.HasField("TotalLungCapacity"):
        _serialize_scalar_volume_from_bind(src.TotalLungCapacity, dst.get_total_lung_capacity())
    if src.HasField("VitalCapacity"):
        _serialize_scalar_volume_from_bind(src.VitalCapacity, dst.get_vital_capacity())
# Patient Configuration
def serialize_patient_configuration_to_string(src: SEPatientConfiguration, fmt: eSerializationFormat):
    dst = PatientConfigurationData()
    _serialize_patient_configuration_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_patient_configuration_to_file(src: SEPatientConfiguration, filename: str, fmt: eSerializationFormat):
    string = serialize_patient_configuration_to_string(src, fmt)
    file = open(filename, "w")
    n = file.write(string)
    file.close()
def serialize_patient_configuration_from_string(string: str, dst: SEPatientConfiguration, fmt: eSerializationFormat):
    src = PatientConfigurationData()
    json_format.Parse(string, src)
    _serialize_patient_configuration_from_bind(src,dst)
def serialize_patient_configuration_from_file(filename: str, dst: SEPatientConfiguration, fmt: eSerializationFormat):
    with open(filename) as f:
        string = f.read()
    serialize_patient_configuration_from_string(string, dst, fmt)

def _serialize_patient_configuration_to_bind(src: SEPatientConfiguration, dst: PatientConfigurationData):
    if src.has_patient():
        _serialize_patient_to_bind(src.get_patient(), dst.Patient)
    elif src.has_patient_file():
        dst.PatientFile = src.get_patient_file()
    if src.has_conditions():
        _serialize_condition_manager_to_bind(src.get_conditions(), dst.Conditions)
def _serialize_patient_configuration_from_bind(src: PatientConfigurationData, dst: SEPatientConfiguration):
    raise Exception("_serialize_patient_configuration_from_bind not implemented")
# Patient Conditions
# Patient Actions
def _serialize_patient_action_to_bind(src: SEPatientAction, dst: PatientActionData):
    _serialize_action_to_bind(src,dst.Action)
def _serialize_patient_action_from_bind(src: PatientActionData, dst: SEPatientAction):
    raise Exception("_serialize_patient_action_from_bind not implemented")

def _serialize_exercise_to_bind(src: SEExercise, dst: ExerciseData):
    _serialize_patient_action_to_bind(src, dst.PatientAction)
    _serialize_scalar_0to1_to_bind(src.get_intensity(), dst.Intensity)
def _serialize_exercise_from_bind(src: ExerciseData, dst: SEExercise):
    raise Exception("_serialize_exercise_from_bind not implemented")

def _serialize_hemorrhage_to_bind(src: SEHemorrhage, dst: HemorrhageData):
    _serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Compartment = src.get_compartment()
    dst.Type = src.get_type().value
    _serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)
def _serialize_hemorrhage_from_bind(src: HemorrhageData, dst: SEHemorrhage):
    raise Exception("_serialize_hemorrhage_from_bind not implemented")

# Scalars
def _serialize_scalar_to_bind(src: SEScalar, dst: ScalarData):
    dst.Scalar.Value = src.get_value()
def _serialize_scalar_from_bind(src: ScalarData, dst: SEScalar):
    dst.set_value(src.Scalar.Value)

def _serialize_scalar_0to1_to_bind(src: SEScalar0To1, dst: Scalar0To1Data):
    dst.Scalar0To1.Value = src.get_value()
def _serialize_scalar_0to1_from_bind(src: Scalar0To1Data, dst: SEScalar0To1):
    dst.set_value(src.Scalar0To1.Value)

def _serialize_scalar_negative1to1_to_bind(src: SEScalarNegative1To1, dst: ScalarNegative1To1Data):
    dst.ScalarNegative1To1.Value = src.get_value()
def _serialize_scalar_negative1to1_from_bind(src: ScalarNegative1To1Data, dst: SEScalarNegative1To1):
    dst.set_value(src.ScalarNegative1To1.Value)

def _serialize_scalar_area_to_bind(src: SEScalarArea, dst: ScalarAreaData):
    dst.ScalarArea.Value = src.get_value()
    dst.ScalarArea.Unit = src.get_unit().get_string()
def _serialize_scalar_area_from_bind(src: ScalarAreaData, dst: SEScalarArea):
    dst.set_value(src.ScalarArea.Value, AreaUnit.from_string(src.ScalarArea.Unit))

def _serialize_scalar_frequency_to_bind(src: SEScalarFrequency, dst: ScalarFrequencyData):
    dst.ScalarFrequency.Value = src.get_value()
    dst.ScalarFrequency.Unit = src.get_unit().get_string()
def _serialize_scalar_frequency_from_bind(src: ScalarFrequencyData, dst: SEScalarFrequency):
    dst.set_value(src.ScalarFrequency.Value, FrequencyUnit.from_string(src.ScalarFrequency.Unit))

def _serialize_scalar_heat_resistance_area_to_bind(src: SEScalarHeatResistanceArea, dst: ScalarHeatResistanceAreaData):
    dst.ScalarHeatResistanceArea.Value = src.get_value()
    dst.ScalarHeatResistanceArea.Unit = src.get_unit().get_string()
def _serialize_scalar_heat_resistance_area_from_bind(src: ScalarHeatResistanceAreaData, dst: SEScalarHeatResistanceArea):
    dst.set_value(src.ScalarHeatResistanceArea.Value, HeatResistanceAreaUnit.from_string(src.ScalarHeatResistanceArea.Unit))

def _serialize_scalar_inverse_pressure_to_bind(src: SEScalarInversePressure, dst: ScalarInversePressureData):
    dst.ScalarInversePressure.Value = src.get_value()
    dst.ScalarInversePressure.Unit = src.get_unit().get_string()
def _serialize_scalar_inverse_pressure_from_bind(src: ScalarInversePressureData, dst: SEScalarInversePressure):
    dst.set_value(src.ScalarInversePressure.Value, InversePressureUnit.from_string(src.ScalarInversePressure.Unit))

def _serialize_scalar_length_to_bind(src: SEScalarLength, dst: ScalarLengthData):
    dst.ScalarLength.Value = src.get_value()
    dst.ScalarLength.Unit = src.get_unit().get_string()
def _serialize_scalar_length_from_bind(src: ScalarLengthData, dst: SEScalarLength):
    dst.set_value(src.ScalarLength.Value, LengthUnit.from_string(src.ScalarLength.Unit))

def _serialize_scalar_length_per_time_to_bind(src: SEScalarLengthPerTime, dst: ScalarLengthPerTimeData):
    dst.ScalarLengthPerTime.Value = src.get_value()
    dst.ScalarLengthPerTime.Unit = src.get_unit().get_string()
def _serialize_scalar_length_per_time_from_bind(src: ScalarLengthPerTimeData, dst: SEScalarLengthPerTime):
    dst.set_value(src.ScalarLengthPerTime.Value, LengthPerTimeUnit.from_string(src.ScalarLengthPerTime.Unit))

def _serialize_scalar_mass_to_bind(src: SEScalarMass, dst: ScalarMassData):
    dst.ScalarMass.Value = src.get_value()
    dst.ScalarMass.Unit = src.get_unit().get_string()
def _serialize_scalar_mass_from_bind(src: ScalarMassData, dst: SEScalarMass):
    dst.set_value(src.ScalarMass.Value, MassUnit.from_string(src.ScalarMass.Unit))

def _serialize_scalar_mass_per_amount_to_bind(src: SEScalarMassPerAmount, dst: ScalarMassPerAmountData):
    dst.ScalarMassPerAmount.Value = src.get_value()
    dst.ScalarMassPerAmount.Unit = src.get_unit().get_string()
def _serialize_scalar_mass_per_amount_from_bind(src: ScalarMassPerAmountData, dst: SEScalarMassPerAmount):
    dst.set_value(src.ScalarMassPerAmount.Value, MassPerAmountUnit.from_string(src.ScalarMassPerAmount.Unit))

def _serialize_scalar_mass_per_area_time_to_bind(src: SEScalarMassPerAreaTime, dst: ScalarMassPerAreaTimeData):
    dst.ScalarMassPerAreaTime.Value = src.get_value()
    dst.ScalarMassPerAreaTime.Unit = src.get_unit().get_string()
def _serialize_scalar_mass_per_area_time_from_bind(src: ScalarMassPerAreaTimeData, dst: SEScalarMassPerAreaTime):
    dst.set_value(src.ScalarMassPerAreaTime.Value, MassPerAreaTimeUnit.from_string(src.ScalarMassPerAreaTime.Unit))

def _serialize_scalar_mass_per_time_to_bind(src: SEScalarMassPerTime, dst: ScalarMassPerTimeData):
    dst.ScalarMassPerTime.Value = src.get_value()
    dst.ScalarMassPerTime.Unit = src.get_unit().get_string()
def _serialize_scalar_mass_per_time_from_bind(src: ScalarMassPerTimeData, dst: SEScalarMassPerTime):
    dst.set_value(src.ScalarMassPerTime.Value, MassPerTimeUnit.from_string(src.ScalarMassPerTime.Unit))

def _serialize_scalar_mass_per_volume_to_bind(src: SEScalarMassPerVolume, dst: ScalarMassPerVolumeData):
    dst.ScalarMassPerVolume.Value = src.get_value()
    dst.ScalarMassPerVolume.Unit = src.get_unit().get_string()
def _serialize_scalar_mass_per_volume_from_bind(src: ScalarMassPerVolumeData, dst: SEScalarMassPerVolume):
    dst.set_value(src.ScalarMassPerVolume.Value, MassPerVolumeUnit.from_string(src.ScalarMassPerVolume.Unit))

def _serialize_scalar_power_to_bind(src: SEScalarPower, dst: ScalarPowerData):
    dst.ScalarPower.Value = src.get_value()
    dst.ScalarPower.Unit = src.get_unit().get_string()
def _serialize_scalar_power_from_bind(src: ScalarPowerData, dst: SEScalarPower):
    dst.set_value(src.ScalarPower.Value, PowerUnit.from_string(src.ScalarPower.Unit))

def _serialize_scalar_pressure_to_bind(src: SEScalarPressure, dst: ScalarPressureData):
    dst.ScalarPressure.Value = src.get_value()
    dst.ScalarPressure.Unit = src.get_unit().get_string()
def _serialize_scalar_pressure_from_bind(src: ScalarPressureData, dst: SEScalarPressure):
    dst.set_value(src.ScalarPressure.Value, PressureUnit.from_string(src.ScalarPressure.Unit))

def _serialize_scalar_temperature_to_bind(src: SEScalarTemperature, dst: ScalarTemperatureData):
    dst.ScalarTemperature.Value = src.get_value()
    dst.ScalarTemperature.Unit = src.get_unit().get_string()
def _serialize_scalar_temperature_from_bind(src: ScalarTemperatureData, dst: SEScalarTemperature):
    dst.set_value(src.ScalarTemperature.Value, TemperatureUnit.from_string(src.ScalarTemperature.Unit))

def _serialize_scalar_time_to_bind(src: SEScalarTime, dst: ScalarTimeData):
    dst.ScalarTime.Value = src.get_value()
    dst.ScalarTime.Unit = src.get_unit().get_string()
def _serialize_scalar_time_from_bind(src: ScalarTimeData, dst: SEScalarTime):
    dst.set_value(src.ScalarTime.Value, TimeUnit.from_string(src.ScalarTime.Unit))

def _serialize_scalar_volume_to_bind(src: SEScalarVolume, dst: ScalarVolumeData):
    dst.ScalarVolume.Value = src.get_value()
    dst.ScalarVolume.Unit = src.get_unit().get_string()
def _serialize_scalar_volume_from_bind(src: ScalarVolumeData, dst: SEScalarVolume):
    dst.set_value(src.ScalarVolume.Value, VolumeUnit.from_string(src.ScalarVolume.Unit))

def _serialize_scalar_volume_per_time_to_bind(src: SEScalarVolumePerTime, dst: ScalarVolumePerTimeData):
    dst.ScalarVolumePerTime.Value = src.get_value()
    dst.ScalarVolumePerTime.Unit = src.get_unit().get_string()
def _serialize_scalar_volume_per_time_from_bind(src: ScalarVolumePerTimeData, dst: SEScalarVolumePerTime):
    dst.set_value(src.ScalarVolumePerTime.Value, VolumePerTimeUnit.from_string(src.ScalarVolumePerTime.Unit))

def _serialize_scalar_volume_per_time_mass_to_bind(src: SEScalarVolumePerTimeMass, dst: ScalarVolumePerTimeMassData):
    dst.ScalarVolumePerTimeMass.Value = src.get_value()
    dst.ScalarVolumePerTimeMass.Unit = src.get_unit().get_string()
def _serialize_scalar_volume_per_time_mass_from_bind(src: ScalarVolumePerTimeMassData, dst: SEScalarVolumePerTimeMass):
    dst.set_value(src.ScalarVolumePerTimeMass.Value, VolumePerTimeMassUnit.from_string(src.ScalarVolumePerTimeMass.Unit))

def _serialize_scalar_volume_per_time_pressure_to_bind(src: SEScalarVolumePerTimePressure, dst: ScalarVolumePerTimePressureData):
    dst.ScalarVolumePerTimePressure.Value = src.get_value()
    dst.ScalarVolumePerTimePressure.Unit = src.get_unit().get_string()
def _serialize_scalar_volume_per_time_pressure_from_bind(src: ScalarVolumePerTimePressureData, dst: SEScalarVolumePerTimePressure):
    dst.set_value(src.ScalarVolumePerTimePressure.Value, VolumePerTimePressureUnit.from_string(src.ScalarVolumePerTimePressure.Unit))
