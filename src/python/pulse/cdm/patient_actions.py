# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from enum import Enum
from pulse.cdm.engine import SEAction, eSwitch, eSide
from pulse.cdm.scalars import SEScalar0To1, SEScalarVolumePerTime

class SEPatientAction(SEAction):
    def __init__(self):
        super().__init__()

class SEPatientActionSeverity(SEAction):
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

class SEPatientActionLung(SEPatientActionSeverity):
    def __init__(self):
        super().__init__()
        self.__slots__ = super().__slots__().extend(["_left_lung_affected", "_right_lung_affected"])
        self._severity = None
        self._left_lung_affected = None
        self._right_lung_affected = None

    def clear(self):
        super().clear()
        if self._left_lung_affected is not None:
            self._left_lung_affected.invalidate()
        if self._right_lung_affected is not None:
            self._right_lung_affected.invalidate()

    def is_valid(self):
        return super().is_valid() and self.has_left_lung_affected() and self.has_right_lung_affected()

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

class SEAcuteRespiratoryDistressSyndromeExacerbation(SEPatientActionLung):

    def __repr__(self):
        return ("Acute Respiratory Distress Syndrome Exacerbation\n"
                "  Severity: {}\n"
                "  Right Lung Affected: {}\n"
                "  Left Lung Affected: {}").format(self._severity, self._right_lung_affected, self._left_lung_affected)


class SEAcuteStress(SEPatientActionSeverity):
    def __repr__(self):
        return ("Acute Stress\n"
                "  Severity: {}").format(self._severity)


class SEAirwayObstruction(SEPatientActionSeverity):
    def __repr__(self):
        return ("Airway Obstruction\n"
                "  Severity: {}").format(self._severity)


class SEAsthmaAttack(SEPatientActionSeverity):
    def __repr__(self):
        return ("Asthma Attack\n"
                "  Severity: {}").format(self._severity)

class eBrainInjuryType(Enum):
    Diffuse = 0
    LeftFocal = 1
    RightFocal = 2

class SEBrainInjury(SEPatientActionSeverity):
    def __init__(self):
        super().__init__()
        self.__slots__ = super().__slots__().extend(["_injury_type"])
        self._injury_type = eBrainInjuryType.Diffuse

    def clear(self):
        super().clear()
        self._injury_type = eBrainInjuryType.Diffuse

    def get_injury_type(self):
        return self._injury_type

    def has_injury_type(self):
        return self._injury_type is not None

    def set_injury_type(self, injury_type: eBrainInjuryType):
        self._injury_type = injury_type

    def __repr__(self):
        return ("Brain Injury\n"
                "  Severity: {}\n"
                "  Injury Type: {}").format(self._severity, self._injury_type)

class SEBronchoConstriction(SEPatientActionSeverity):
    def __repr__(self):
        return ("BronchoConstriction\n"
                "  Severity: {}").format(self._severity)

class SEDyspnea(SEPatientActionSeverity):
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
    __slots__ = ["_compartment","_rate","_type"]

    def __init__(self):
        super().__init__()
        self._compartment = None
        self._rate = None
        self._type = eHemorrhageType.External

    def clear(self):
        super().clear()
        self._compartment = None
        if self._rate is not None:
            self._rate.invalidate()
        self._type = eHemorrhageType.External

    def is_valid(self):
        return self.has_rate() and self.has_compartment()

    def has_compartment(self):
        return self._compartment is not None
    def get_compartment(self):
        return self._compartment
    def set_compartment(self, compartment: str):
        self._compartment = compartment
    def invalidate_compartment(self):
        self._compartment = None

    def has_rate(self):
        return False if self._rate is None else self._rate.is_valid()
    def get_rate(self):
        if self._rate is None:
            self._rate = SEScalarVolumePerTime()
        return self._rate

    def get_type(self):
        return self._type
    def set_type(self, type: eHemorrhageType):
        self._type = type

    def __repr__(self):
        return ("Hemorrhage\n"
                "  Type: {}\n"
                "  Compartment: {}\n"
                "  Rate: {}").format(self._type,self._compartment,self._rate)


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
        self._type = eHemorrhageType.Off

    def get_type(self):
        return self._type

    def set_type(self, type: eIntubationType):
        self._type = type

    def is_valid(self):
        return self._type is not eIntubationType.Off

    def __repr__(self):
        return ("Intubation\n"
                "  Type: {}\n").format(self._type)


class SELobarPneumoniaExacerbation(SEPatientActionLung):
    def __repr__(self):
        return ("Lobar Pneumonia Exacerbation\n"
                "  Severity: {}\n"
                "  Right Lung Affected: {}\n"
                "  Left Lung Affected: {}").format(self._severity, self._right_lung_affected, self._left_lung_affected)

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
                "  Side: {}").format(self.state, self._side)