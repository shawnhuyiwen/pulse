# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from enum import Enum
from pulse.cdm.engine import SEAction, eSwitch, eSide, eGate
from pulse.cdm.scalars import SEScalar0To1, SEScalarVolumePerTime, SEScalarVolume, SEScalarMassPerVolume

class SEPatientAction(SEAction):
    def __init__(self):
        super().__init__()


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

class SEBronchoConstriction(SEPatientAction):
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
        return ("BronchoConstriction\n"
                "  Severity: {}").format(self._severity)

class SEDyspnea(SEPatientAction):
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

class SERespiratoryFatigue(SEPatientAction):
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
    __slots__ = ["_admin_route", "_concentration", "_dose", "_substance"]

    def __init__(self):
        super().__init__()
        self._admin_route = eSubstance_Administration.Intravenous
        self._dose = None
        self._concentration = None
        self._substance = None

    def clear(self):
        super().clear()
        self._admin_route = eSubstance_Administration.Intravenous
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
                "  Concentration: {}\n"
                "  Dose: {}\n"
                "  Substance: {}").format(self._concentration, self._dose, self._substance)
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
