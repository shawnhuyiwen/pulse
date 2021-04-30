# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from enum import Enum
from pulse.cdm.patient import SENutrition
from pulse.cdm.engine import SEAction, eSwitch, eSide, eGate
from pulse.cdm.scalars import SEScalar0To1, SEScalarArea, SEScalarForce, \
                              SEScalarMassPerVolume, SEScalarPressure, \
                              SEScalarTime, SEScalarVolumePerTime, SEScalarVolume

class SEPatientAction(SEAction):
    def __init__(self):
        super().__init__()

class AnyConsciousRespirationCommand():
    __slots__ = ["_comment"]

    def __init__(self):
        self._comment=""
    def clear(self):
        if self._comment is not None:
         self._comment = ""
    def set_comment(self, comment:str):
        self._comment=comment
    def get_comment(self):
        return self._comment
    def has_comment(self):
        return self._comment is not None

class SEAcuteRespiratoryDistressSyndromeExacerbation(SEPatientAction):
    __slots__ = ["_severity", "_left_lung_affected", "_right_lung_affected"]
    def __init__(self):
        super().__init__()
        self._severity = None
        self._left_lung_affected = None
        self._right_lung_affected = None

    def clear(self):
        if self._severity is not None:
            self._severity.invalidate()
        if self._left_lung_affected is not None:
            self._left_lung_affected.invalidate()
        if self._right_lung_affected is not None:
            self._right_lung_affected.invalidate()
    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def is_valid(self):
        return self.has_severity() and self.has_left_lung_affected() and self.has_right_lung_affected()

    def get_left_lung_affected(self):
        if not self._left_lung_affected:
            self._left_lung_affected = SEScalar0To1()
        return self._left_lung_affected

    def has_left_lung_affected(self):
        return self._left_lung_affected is not None

    def get_right_lung_affected(self):
        if not self._right_lung_affected:
            self._right_lung_affected = SEScalar0To1()
        return self._right_lung_affected

    def has_right_lung_affected(self):
        return self._right_lung_affected is not None
    def __repr__(self):
        return ("Acute Respiratory Distress Syndrome Exacerbation\n"
                "  Severity: {}\n"
                "  Right Lung Affected: {}\n"
                "  Left Lung Affected: {}").format(self._severity, self._right_lung_affected, self._left_lung_affected)

class SEAcuteStress(SEPatientAction):
    __slots__ = ["_severity"]

    def __init__(self):
        super().__init__()
        self._severity = None

    def clear(self):
        super().clear()
        if self._severity is not None:
            self._severity.invalidate()

    def is_valid(self):
        return self.has_severity()

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def __repr__(self):
        return ("Acute Stress\n"
                "  Severity: {}").format(self._severity)

class SEAirwayObstruction(SEPatientAction):
    __slots__ = ["_severity"]

    def __init__(self):
        super().__init__()
        self._severity = None

    def clear(self):
        super().clear()
        if self._severity is not None:
            self._severity.invalidate()

    def is_valid(self):
        return self.has_severity()

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def __repr__(self):
        return ("Airway Obstruction\n"
                "  Severity: {}").format(self._severity)

class SEAsthmaAttack(SEPatientAction):
    __slots__ = ["_severity"]

    def __init__(self):
        super().__init__()
        self._severity = None

    def clear(self):
        super().clear()
        if self._severity is not None:
            self._severity.invalidate()

    def is_valid(self):
        return self.has_severity()

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def __repr__(self):
        return ("Asthma Attack\n"
                "  Severity: {}").format(self._severity)

class eBrainInjuryType(Enum):
    Diffuse = 0
    LeftFocal = 1
    RightFocal = 2
class SEBrainInjury(SEPatientAction):
    __slots__ = ["_severity", "_injury_type"]

    def __init__(self):
        super().__init__()
        self._severity = None
        self._injury_type = eBrainInjuryType.Diffuse

    def clear(self):
        super().clear()
        if self._severity is not None:
            self._severity.invalidate()
        self._injury_type = eBrainInjuryType.Diffuse

    def get_injury_type(self):
        return self._injury_type

    def has_injury_type(self):
        return self._injury_type is not None

    def set_injury_type(self, injury_type: eBrainInjuryType):
        self._injury_type = injury_type

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity

    def is_valid(self):
        return self.has_severity() and self.has_injury_type()

    def __repr__(self):
        return ("Brain Injury\n"
                "  Severity: {}\n"
                "  Injury Type: {}").format(self._severity, self._injury_type)

class SEBronchoconstriction(SEPatientAction):
    __slots__ = ["_severity"]

    def __init__(self):
        super().__init__()
        self._severity = None

    def clear(self):
        super().clear()
        if self._severity is not None:
            self._severity.invalidate()

    def is_valid(self):
        return self.has_severity()

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def __repr__(self):
        return ("Bronchoconstriction\n"
                "  Severity: {}").format(self._severity)

class SECardiacArrest(SEPatientAction):
    __slots__ = ["_state"]

    def __init__(self):
        super().__init__()
        self._state = eSwitch.Off
    def clear(self):
        super().clear()
        self._state = eSwitch.Off
    def is_valid(self):
        return True
    def set_state(self, state: eSwitch):
        self._state = state
    def get_state(self):
        return self._state
    def __repr__(self):
        return ("Cardiac Arrest\n"
         "  State: {}").format(self._state)

class SEChestCompressionForce(SEPatientAction):
    __slots__ = ["_force"]

    def __init__(self):
        super().__init__()
        self._force = None

    def clear(self):
        super().clear()
        if self._force is not None:
            self._force.invalidate()

    def is_valid(self):
        return self.has_force()

    def has_force(self):
        return self._force is not None
    def get_force(self):
        if self._force is None:
            self._force = SEScalarForce()
        return self._force

    def __repr__(self):
        return ("Chest Compression\n"
                "  Force: {}").format(self._force)

class SEChestCompressionForceScale(SEPatientAction):
    __slots__ = ["_force_scale","_force_period"]

    def __init__(self):
        super().__init__()
        self._force_scale = None
        self._force_period = None

    def clear(self):
        super().clear()
        if self._force_scale is not None:
            self._force_scale.invalidate()
        if self._force_period is not None:
            self._force_period.invalidate()

    def is_valid(self):
        return self.has_force_scale() and self.has_force_period()

    def has_force_scale(self):
        return self._force_scale is not None
    def get_force_scale(self):
        if self._force_scale is None:
            self._force_scale = SEScalar0To1()
        return self._force_scale

    def has_force_period(self):
        return self._force_period is not None
    def get_force_period(self):
        if self._force_period is None:
            self._force_period = SEScalarTime()
        return self._force_period

    def __repr__(self):
        return ("Chest Compression\n"
                "  Force Scale: {}\n"
                "  Force Period: {}").format(self._force_scale, self._force_period)

class SEChestOcclusiveDressing(SEPatientAction):
    __slots__ = ["_state", "_side"]

    def __init__(self):
        super().__init__()
        self._state = eSwitch.Off
        self._side = eSide.NullSide

    def clear(self):
        super().clear()
        self._state = eSwitch.Off
        self._side = eSide.NullSide

    def is_valid(self):
        return self.has_side();
    def get_side(self):
        return self._side
    def set_side(self, side: eSide):
        self._side = side
    def has_side(self):
        return self._side is not eSide.NullSide
    def get_state(self):
        return self._state
    def set_state(self, state: eSwitch):
        self._state = state
    def has_state(self):
        return self._state is not eSwitch.NullSwitch
    def __repr__(self):
        return ("Chest Occlusive Dressing\n"
                "  State: {}\n"
                "  Side: {}").format(self._state, self._side)

class SEChronicObstructivePulmonaryDiseaseExacerbation(SEPatientAction):
    __slots__ = ["_bronchitis_severity", "_emphysema_severity"]

    def __init__(self):
        super().__init__()
        self._bronchitis_severity = None
        self._emphysema_severity = None
    def clear(self):
        super().clear()
        if self._bronchitis_severity is not None:
            self._bronchitis_severity.invalidate()
        if self._emphysema_severity is not None:
            self._emphysema_severity.invalidate()
    def is_valid(self):
        return self.has_emphysema_severity() and self.has_bronchitis_severity()
    def has_bronchitis_severity(self):
        return self._bronchitis_severity is not None
    def get_bronchitis_severity(self):
        if self._bronchitis_severity is None:
            self._bronchitis_severity = SEScalar0To1()
        return self._bronchitis_severity
    def has_emphysema_severity(self):
        return self._emphysema_severity is not None
    def get_emphysema_severity(self):
        if self._emphysema_severity is None:
            self._emphysema_severity = SEScalar0To1()
        return self._emphysema_severity
    def __repr__(self):
        return ("COPD Exacerbation\n"
                "  Emphysema Severity: {}]\n"
                "  Bronchitis Severity: {}").format(self._emphysema_severity, self._bronchitis_severity)

class SEConsciousRespiration(SEPatientAction):
    __slots__ = ['_commands', "_start_immediately"]

    def __init__(self):
        super().__init__()
        self._commands=[]
        self._start_immediately = False
    def clear(self):
        super().clear()
        if len(self._commands):
            self._commands = []
    def is_valid(self):
        validity = len(self._commands) > 0
        for command in self._commands:
            validity = validity and command.is_valid()
        return validity
    def set_start_immediately(self, start: bool):
        self._start_immediately = start
    def get_start_immediately(self):
        return self._start_immediately
    def has_commands(self):
        return len(self._commands) > 0
    def get_commands(self):
        return self._commands
    def get_active_command(self):
        if len(self._commands):
            return self._commands[0]
    def remove_command(self, index:int = 0):
        del self._commands[index]
    def remove_active_command(self):
        self.remove_command(0)

    def add_forced_inhale(self,inhale_period: SEScalarTime,
                           volume_fraction: SEScalar0To1,
                           hold_period:SEScalarTime = None,
                           release_period:SEScalarTime = None):
        self._commands.append(SEForcedInhale(inhale_period=inhale_period,
                                             volume_fraction= volume_fraction,
                                             hold_period=hold_period,
                                             release_period=release_period))
    def add_forced_exhale(self,exhale_period:SEScalarTime,
                           volume_fraction:SEScalar0To1,
                           hold_period:SEScalarTime = None,
                           release_period:SEScalarTime = None):
        self._commands.append(SEForcedExhale(exhale_period = exhale_period,
                                             volume_fraction= volume_fraction,
                                             hold_period=hold_period,
                                             release_period=release_period))
    def add_forced_pause(self, hold_period:SEScalarTime=None):
        self._commands.append(SEForcedPause(hold_period))

    def __repr__(self):
        return ("Conscious Respiration:\n"
                "  Commands: [{}]").format(self._commands)


class SEForcedPause(AnyConsciousRespirationCommand):
    __slots__ = ["_period"]

    def __init__(self, period: SEScalarTime = None):
        super().__init__()
        self._period = period
    def clear(self):
        super().clear()
        if self._period is not None:
            self._period.invalidate()
    def is_valid(self):
        return self.has_period()
    def has_period(self):
        return self._period is not None
    def get_period(self):
        if self._period is None:
            self._period = SEScalarTime();
        return self._period
    def __repr__(self):
        return ("Breath Hold\n"
                "  Comment: {}\n"
                "  Period: {}").format(self._comment, self._period)

class SEUseInhaler(AnyConsciousRespirationCommand):
    __slots__ = []
    def __init__(self):
        super().__init__()
    def clear(self):
        super().clear()
    def __repr__(self):
        return ("Use Inhaler\n"
                "  Comment: {}").format(self._comment)

class SEForcedInhale(AnyConsciousRespirationCommand):
    __slots__ = ["_inhale_period", "_release_period", "_hold_period", "_inspiratory_capacity_fraction"]

    def __init__(self,
                 inhale_period: SEScalarTime = None,
                 volume_fraction: SEScalar0To1 = None,
                 hold_period: SEScalarTime = None,
                 release_period: SEScalarTime = None):
        super().__init__()
        self._inhale_period = inhale_period
        self._release_period = release_period
        self._hold_period = hold_period
        self._inspiratory_capacity_fraction = volume_fraction
    def clear(self):
        super().clear()
        if self._inhale_period is not None:
          self._inhale_period.invalidate()
        if self._release_period is not None:
          self._release_period.invalidate()
        if self._hold_period is not None:
          self._hold_period.invalidate()
        if self._inspiratory_capacity_fraction is not None:
          self._inspiratory_capacity_fraction.invalidate()
    def is_valid(self):
        return self.has_inspiratory_capacity_fraction() and self.has_inhale_period()
    def get_hold_period(self):
        if self._hold_period is None:
            self._hold_period = SEScalarTime()
        return self._hold_period
    def has_hold_period(self):
        return self._hold_period
    def get_inhale_period(self):
        if self._inhale_period is None:
            self._inhale_period = SEScalarTime()
        return self._inhale_period
    def has_inhale_period(self):
        return self._inhale_period
    def get_release_period(self):
        if self._release_period is None:
            self._release_period = SEScalarTime()
        return self._release_period
    def has_release_period(self):
        return self._release_period
    def get_inspiratory_capacity_fraction(self):
        if self._inspiratory_capacity_fraction is None:
            self._inspiratory_capacity_fraction = SEScalar0To1()
        return self._inspiratory_capacity_fraction
    def has_inspiratory_capacity_fraction(self):
        return self._inspiratory_capacity_fraction is not None
    def __repr__(self):
        return ("Forced Inhale\n"
                "  Inspiratory Capacity Fraction: {}\n"
                "  Inhale Period: {}\n"
                "  Hold Period: {}\n"
                "  Release Period: {}").format(self._inspiratory_capacity_fraction, self._inhale_period,
                                              self._hold_period, self._release_period)


class SEForcedExhale(AnyConsciousRespirationCommand):
    __slots__ = ["_exhale_period", "_release_period", "_hold_period", "_expiratory_capacity_fraction"]

    def __init__(self, exhale_period: SEScalarTime = None,
                 volume_fraction:SEScalar0To1 = None,
                 hold_period: SEScalarTime=None,
                 release_period: SEScalarTime=None):
        super().__init__()
        self._exhale_period = exhale_period
        self._release_period = release_period
        self._hold_period = hold_period
        self._expiratory_capacity_fraction = volume_fraction
    def clear(self):
        super().clear()
        if self._exhale_period is not None:
          self._exhale_period.invalidate()
        if self._release_period is not None:
          self._release_period.invalidate()
        if self._hold_period is not None:
          self._hold_period.invalidate()
        if self._expiratory_capacity_fraction is not None:
          self._expiratory_capacity_fraction.invalidate()
    def is_valid(self):
        return self.has_expiratory_capacity_fraction() and self.has_exhale_period()
    def get_hold_period(self):
        if self._hold_period is None:
            self._hold_period = SEScalarTime()
        return self._hold_period
    def has_hold_period(self):
        return self._hold_period
    def get_exhale_period(self):
        if self._exhale_period is None:
            self._exhale_period = SEScalarTime()
        return self._exhale_period
    def has_exhale_period(self):
        return self._exhale_period
    def get_release_period(self):
        if self._release_period is None:
            self._release_period = SEScalarTime()
        return self._release_period
    def has_release_period(self):
        return self._release_period
    def get_expiratory_capacity_fraction(self):
        if self._expiratory_capacity_fraction is None:
            self._expiratory_capacity_fraction = SEScalar0To1()
        return self._expiratory_capacity_fraction
    def has_expiratory_capacity_fraction(self):
        return self._expiratory_capacity_fraction is not None
    def __repr__(self):
        return ("Forced Exhale\n"
                "  Expiratory Capacity Fraction: {}\n"
                "  Exhale Period: {}\n"
                "  Hold Period: {}\n"
                "  Release Period: {}").format(self._expiratory_capacity_fraction, self._exhale_period,
                                              self._hold_period, self._release_period)

class SEConsumeNutrients(SEPatientAction):
    __slots__ = ["_nutrition_file",
                 "_nutrition"]

    def __init__(self):
        super().__init__()
        self._nutrition_file = None
        self._nutrition = None

    def clear(self):
        self._nutrition_file = None
        self._nutrition = None

    def copy(self, src):
        if not isinstance(SEConsumeNutrients, src):
            raise Exception("Provided argument must be a SEConsumeNutrients")
        self.clear()
        self._nutrition_file = src._nutrition_file
        self._nutrition.copy(src._nutrition)

    def is_valid(self):
        return self.has_nutrition() or self.has_nutrition_file()

    def is_active(self):
        return True;

    def has_nutrition_file(self):
        return self._nutrition_file is not None
    def get_nutrition_file(self):
        return self._nutrition_file
    def set_nutrition_file(self, filename: str):
        self._nutrition_file = filename

    def has_nutrition(self):
        return self._nutrition is not None
    def get_nutrition(self):
        if self._nutrition is None:
            self._nutrition = SENutrition()
        return self._nutrition

class SEDyspnea(SEPatientAction):
    __slots__ = ["_severity"]

    def __init__(self):
        super().__init__()
        self._severity = None

    def clear(self):
        super().clear()
        if self._severity is not None:
            self._severity.invalidate()

    def is_valid(self):
        return self.has_severity()

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity

    def __repr__(self):
        return ("Dyspnea\n"
                "  Severity: {}").format(self._severity)

class SEExercise(SEPatientAction):
    __slots__ = ["_intensity"]

    def __init__(self):
        super().__init__()
        self._intensity = None

    def clear(self):
        super().clear()
        if self._intensity is not None:
            self._intensity.invalidate()

    def is_valid(self):
        return self.has_intensity()

    def has_intensity(self):
        return False if self._intensity is None else self._intensity.is_valid()
    def get_intensity(self):
        if self._intensity is None:
            self._intensity = SEScalar0To1()
        return self._intensity

    def __repr__(self):
        return ("Exercise\n"
                "  Intensity: {}").format(self._intensity)

class eHemorrhageType(Enum):
    External = 0
    Internal = 1
class SEHemorrhage(SEPatientAction):
    __slots__ = ["_type","_compartment","_flow_rate","_severity"]

    def __init__(self):
        super().__init__()
        self._type = eHemorrhageType.External
        self._compartment = None
        self._flow_rate = None
        self._severity = None

    def clear(self):
        super().clear()
        self._type = eHemorrhageType.External
        self._compartment = None
        if self._flow_rate is not None:
            self._flow_rate.invalidate()
        if self._severity is not None:
            self._severity.invalidate()

    def is_valid(self):
        return self.has_compartment() and (self.has_flow_rate() or self.has_severity())

    def get_type(self):
        return self._type
    def set_type(self, type: eHemorrhageType):
        self._type = type

    def has_compartment(self):
        return self._compartment is not None
    def get_compartment(self):
        return self._compartment
    def set_compartment(self, compartment: str):
        self._compartment = compartment
    def invalidate_compartment(self):
        self._compartment = None

    def has_flow_rate(self):
        return False if self._flow_rate is None else self._flow_rate.is_valid()
    def get_flow_rate(self):
        if self._flow_rate is None:
            self._flow_rate = SEScalarVolumePerTime()
        return self._flow_rate

    def has_severity(self):
        return False if self._severity is None else self._severity.is_valid()
    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity

    def __repr__(self):
        return ("Hemorrhage\n"
                "  Type: {}\n"
                "  Compartment: {}\n"
                "  Flow Rate: {}\n"
                "  Severity: {}").format(self._type,self._compartment,self._flow_rate, self._severity)

class SEImpairedAlveolarExchangeExacerbation(SEPatientAction):
    __slots__ = ["_impaired_surface_area", "_impaired_fraction", "_severity"]

    def __init__(self):
        super().__init__()
        self._impaired_fraction = None
        self._impaired_surface_area = None
        self._severity = None
    def clear(self):
        super().clear()
        if self._impaired_fraction is not None:
            self._impaired_fraction.invalidate()
        if self._impaired_surface_area is not None:
            self._impaired_surface_area.invalidate()
        if self._severity is not None:
            self._severity.invalidate()
    def is_valid(self):
        return self.has_impaired_surface_area() or \
               self.has_impaired_fraction() or \
               self.has_severity()
    def has_impaired_surface_area(self):
        return self._impaired_surface_area is not None
    def get_impaired_surface_area(self):
        if self._impaired_surface_area is None:
            self._impaired_surface_area = SEScalarArea()
        return self._impaired_surface_area
    def has_impaired_fraction(self):
        return self._impaired_fraction is not None
    def get_impaired_fraction(self):
        if self._impaired_fraction is None:
            self._impaired_fraction = SEScalar0To1()
        return self._impaired_fraction
    def has_severity(self):
        return self._severity is not None
    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def __repr__(self):
        return ("Impaired Alveolar Exchange Exacerbation\n"
                "  Impaired Fraction: {}\n"
                "  Impaired Surface Area: {}\n"
                "  Severity: {}").format(self._impaired_fraction, self._impaired_surface_area, self._severity)

class eIntubationType(Enum):
    Off = 0
    Esophageal = 1
    LeftMainstem = 2
    RightMainstem = 3
    Tracheal = 4
class SEIntubation(SEPatientAction):
    __slots__ = ["_type"]

    def __init__(self):
        super().__init__()
        self._type = eIntubationType.Off

    def clear(self):
        super().clear()
        self._type = eIntubationType.Off

    def get_type(self):
        return self._type

    def set_type(self, type: eIntubationType):
        self._type = type

    def is_valid(self):
        return self._type is not eIntubationType.Off

    def __repr__(self):
        return ("Intubation\n"
                "  Type: {}\n").format(self._type)

class SELobarPneumoniaExacerbation(SEPatientAction):
    __slots__ = ["_severity", "_left_lung_affected", "_right_lung_affected"]

    def __init__(self):
        super().__init__()
        self._severity = None
        self._left_lung_affected = None
        self._right_lung_affected = None

    def clear(self):
        if self._severity is not None:
            self._severity.invalidate()
        if self._left_lung_affected is not None:
            self._left_lung_affected.invalidate()
        if self._right_lung_affected is not None:
            self._right_lung_affected.invalidate()

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity

    def is_valid(self):
        return self.has_severity() and self.has_left_lung_affected() and self.has_right_lung_affected()

    def get_left_lung_affected(self):
        if not self._left_lung_affected:
            self._left_lung_affected = SEScalar0To1()
        return self._left_lung_affected

    def has_left_lung_affected(self):
        return self._left_lung_affected is not None

    def get_right_lung_affected(self):
        if not self._right_lung_affected:
            self._right_lung_affected = SEScalar0To1()
        return self._right_lung_affected

    def has_right_lung_affected(self):
        return self._right_lung_affected is not None

    def __repr__(self):
        return ("Lobar Pneumonia Exacerbation\n"
                "  Severity: {}\n"
                "  Right Lung Affected: {}\n"
                "  Left Lung Affected: {}").format(self._severity, self._right_lung_affected, self._left_lung_affected)

class SEMechanicalVentilation(SEPatientAction):
    __slots__ = ["_state", "_flow", "pressure"]
    def __init__(self):
        super().__init__()
        self._state = eSwitch.Off
        self._flow = None
        self._pressure = None
    def clear(self):
        super().clear()
        if self._state is not eSwitch.Off:
            self._state = eSwitch.Off
        if self._flow is not None:
            self._flow.invalidate()
        if self._pressure is not None:
            self._pressure.invalidate()
    def is_valid(self):
        return self.has_pressure() and self.has_flow()
    def get_pressure(self):
        if self._pressure is None:
            self._pressure = SEScalarPressure()
        return self._pressure
    def has_pressure(self):
        return self._pressure is not None
    def get_flow(self):
        if self._flow is None:
            self._flow = SEScalarVolumePerTime()
        return self._flow
    def has_flow(self):
        return self._flow is not None
    def set_state(self, state: eSwitch):
        self._state = state
    def get_state(self):
        return self._state
    def __repr__(self):
        return ("Mechanical Ventilation\n"
                "  State: {} \n"
                "  Flow: {} \n"
                "  Pressure: {}").format(self._state, self._flow, self._pressure)

class SENeedleDecompression(SEPatientAction):
    __slots__ = ["_state", "_side"]

    def __init__(self):
        super().__init__()
        self._state = eSwitch.Off
        self._side = eSide.NullSide

    def clear(self):
        super().clear()
        self._state = eSwitch.Off
        self._side = eSide.NullSide

    def is_valid(self):
        return self.has_side();
    def get_side(self):
        return self._side
    def set_side(self, side: eSide):
        self._side = side
    def has_side(self):
        return self._side is not eSide.NullSide
    def get_state(self):
        return self._state
    def set_state(self, state: eSwitch):
        self._state = state
    def has_state(self):
        return self._state is not eSwitch.NullSwitch
    def __repr__(self):
        return ("Needle Decompression\n"
                "  State: {}\n"
                "  Side: {}").format(self._state, self._side)

class SEPericardialEffusion(SEPatientAction):
    __slots__ = ['_effusion_rate']

    def __init__(self):
        super().__init__()
        self._effusion_rate = None
    def clear(self):
        super().clear()
        if self._effusion_rate is not None:
            self._effusion_rate.invalidate()
    def is_valid(self):
        return self.has_effusion_rate()
    def has_effusion_rate(self):
        return self._effusion_rate is not None
    def get_effusion_rate(self):
        if self._effusion_rate is None:
            self._effusion_rate = SEScalarVolumePerTime()
        return self._effusion_rate
    def __repr__(self):
        return ("Pericardial Effusion\n"
                "  Rate: {}").format(self._effusion_rate)


class SEPulmonaryShuntExacerbation(SEPatientAction):
    __slots__ = ["severity"]

    def __init__(self):
        super().__init__()
        self._severity = None

    def clear(self):
        super().clear()
        if self._severity is not None:
            self._severity.invalidate()

    def is_valid(self):
        return self.has_severity()

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def __repr__(self):
        return ("Pulmonary Shunt Exacerbation\n"
                "  Severity: {}").format(self._severity)

class SERespiratoryFatigue(SEPatientAction):
    __slots__ = ["severity"]

    def __init__(self):
        super().__init__()
        self._severity = None

    def clear(self):
        super().clear()
        if self._severity is not None:
            self._severity.invalidate()

    def is_valid(self):
        return self.has_severity()

    def has_severity(self):
        return self._severity is not None

    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def __repr__(self):
        return ("Respiratory Fatigue\n"
                "  Severity: {}").format(self._severity)

class eSubstance_Administration(Enum):
    Intravenous = 0,
    Epidural = 1
    Intraosseous = 2
    Intraarterial = 3
    Intracardiac = 4
    Intracerebral = 5
    Intracerebroventricular = 6
    Intradermal = 7
    Intramuscular = 8
    Subcutaneous = 9

class SESubstanceBolus(SEPatientAction):
    __slots__ = ["_admin_route", "_admin_duration", "_concentration", "_dose", "_substance"]

    def __init__(self):
        super().__init__()
        self._admin_route = eSubstance_Administration.Intravenous
        self._admin_duration = None
        self._dose = None
        self._concentration = None
        self._substance = None

    def clear(self):
        super().clear()
        self._admin_route = eSubstance_Administration.Intravenous
        if self._admin_duration is not None:
            self._admin_duration.invalidate()
        if self._dose is not None:
            self._dose.invalidate()
        if self._concentration is not None:
            self._concentration.invalidate()
        if self._substance is not None:
            self._substance.invalidate()

    def is_valid(self):
        return self.has_dose() and self.has_concentration() & self.has_substance()

    def has_admin_route(self):
        return self._admin_route is not None
    def set_admin_route(self, route:eSubstance_Administration):
        self._admin_route = route
    def get_admin_route(self):
        return self._admin_route

    def get_admin_duration(self):
        if self._admin_duration is None:
            self._admin_duration = SEScalarTime()
        return self._admin_duration
    def has_admin_duration(self):
        return self._admin_duration is not None

    def get_concentration(self):
        if self._concentration is None:
            self._concentration = SEScalarMassPerVolume()
        return self._concentration
    def has_concentration(self):
        return self._concentration is not None

    def has_dose(self):
        return self._dose is not None
    def get_dose(self):
        if self._dose is None:
            self._dose = SEScalarVolume()
        return self._dose

    def has_substance(self):
        return self._substance is not None
    def get_substance(self):
        return self._substance
    def set_substance(self, substance: str):
        self._substance = substance
    def __repr__(self):
        return ("Substance Bolus\n"
                "  Administration Route: {}\n"
                "  Administration Duration: {}\n"
                "  Concentration: {}\n"
                "  Dose: {}\n"
                "  Substance: {}").format(self._admin_route, self._admin_duration,
                                          self._concentration, self._dose, self._substance)

class SESubstanceCompoundInfusion(SEPatientAction):
    __slots__ = ["_bag_volume", "_rate", "_compound"]

    def __init__(self):
        super().__init__()
        self._bag_volume = None
        self._rate = None
        self._compound = None

    def clear(self):
        super().clear()
        if self._bag_volume is not None:
            self._bag_volume = None
        if self._rate is not None:
            self._rate = None
        if self._compound is not None:
            self._compound = None

    def is_valid(self):
        return self.has_rate() and self.has_compound() and self.has_bag_volume()
    def has_rate(self):
        return self._rate is not None
    def get_rate(self):
        if self._rate is None:
            self._rate = SEScalarVolumePerTime()
        return self._rate

    def has_compound(self):
        return self._rate is not None
    def get_compound(self):
        return self._compound
    def set_compound(self, compound: str):
        self._compound = compound

    def has_bag_volume(self):
        return self._bag_volume is not None
    def get_bag_volume(self):
        if self._bag_volume is None:
            self._bag_volume = SEScalarVolume()
        return self._bag_volume
    def __repr__(self):
        return ("Substance Compound Infusion\n"
                "  Bag Volume: {}\n"
                "  Compound: {}\n"
                "  Rate: {}").format(self._bag_volume, self._compound, self._rate)

class SESubstanceInfusion(SEPatientAction):
    __slots__ = ["_concentration", "_rate", "_substance"]

    def __init__(self):
        super().__init__()
        self._concentration = None
        self._rate = None
        self._substance = None

    def clear(self):
        super().clear()
        if self._concentration is not None:
            self._concentration = None
        if self._rate is not None:
            self._rate = None
        if self._substance is not None:
            self._substance = None

    def is_valid(self):
        return self.has_rate() and self.has_substance() and self.has_concentration()
    def has_rate(self):
        return self._rate is not None
    def get_rate(self):
        if self._rate is None:
            self._rate = SEScalarVolumePerTime()
        return self._rate

    def has_substance(self):
        return self._substance is not None
    def get_substance(self):
        return self._substance
    def set_substance(self, substance: str):
        self._substance = substance

    def has_concentration(self):
        return self._concentration is not None
    def get_concentration(self):
        if self._concentration is None:
            self._concentration = SEScalarMassPerVolume()
        return self._concentration
    def __repr__(self):
        return ("Substance Infusion\n"
                "  Concentration: {}\n"
                "  Substance: {}\n"
                "  Rate: {}").format(self._concentration, self._substance, self._rate)

class eDevice(Enum):
    NullDevice = 0
    NasalCannula = 1
    SimpleMask = 2
    NonRebreatherMask = 3

class SESupplementalOxygen(SEPatientAction):
    __slots__ = ["_device", "_flow", "_volume"]

    def __init__(self):
        super().__init__()
        self._device = eDevice.NullDevice
        self._flow   = None
        self._volume = None

    def clear(self):
        self._device = eDevice.NullDevice
        if self._flow is not None:
            self._flow.invalidate()
        if self._volume is not None:
            self._volume.invalidate()

    def is_valid(self):
        return self.has_flow() and self.has_volume()

    def get_device(self):
        return self._device
    def set_device(self, device:eDevice):
        self._device = device

    def has_flow(self):
        return self._flow is not None
    def get_flow(self):
        if self._flow is None:
            self._flow = SEScalarVolumePerTime()
        return self._flow
    def has_volume(self):
        return self._volume is not None
    def get_volume(self):
        if self._volume is None:
            self._volume = SEScalarVolume()
        return self._volume
    def __repr__(self):
        return ("Supplemental Oxygen\n"
                "  Device: {}\n"
                "  Flow: {}\n"
                "  Volume: {}").format(self._device, self._flow, self._volume)

class SETensionPneumothorax(SEPatientAction):
    __slots__ = ["_type", "_side", "_severity"]

    def __init__(self):
        super().__init__()
        self._type = eGate.NullGate
        self._side = eSide.NullSide
        self._severity = None

    def clear(self):
        super().clear()
        self._type = eGate.NullGate
        self._side = eSide.NullSide
        if self._severity is not None:
            self._severity.invalidate()
    def is_valid(self):
        return self.has_type and self.has_side and self.has_severity

    def has_type(self):
        return self._type is not None
    def set_type(self, type: eGate):
        self._type = type
    def get_type(self):
        return self._type

    def has_side(self):
        return self._side is not None
    def set_side(self, side: eSide):
        self._side = side
    def get_side(self):
        return self._side

    def has_severity(self):
        return self._severity is not None
    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity
    def __repr__(self):
        return ("Tension Pneumothroax\n"
                "  Type: {}\n"
                "  Side: {}\n"
                "  Severity: {}").format(self._type, self._side, self._severity)

class SEUrinate(SEPatientAction):
    def __init__(self):
        super().__init__()

    def clear(self):
        super().clear()

    def is_valid(self):
        return True

    def __repr__(self):
        return ("Urinate\n")