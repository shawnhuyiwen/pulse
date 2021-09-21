# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from pulse.cdm.scalars import SEScalar, SEScalarPressure, SEScalarPressurePerVolume, \
                              SEScalarVolume, SEScalarVolumePerPressure


class SESegment():
    __slots__ = ["_begin_volume",
                 "_end_volume"]

    def __init__(self):
        self._begin_volume = None
        self._end_volume = None

    def clear(self):
        if self._begin_volume is not None:
            self._begin_volume.invalidate()
        if self._end_volume is not None:
            self._end_volume.invalidate()

    def copy(self, src):
        if isinstance(SESegment, src):
            if src.has_begin_volume():
                self.get_begin_volume().set(src.get_begin_volume())
            if src.has_end_volume():
                self.get_end_volume().set(src.get_end_volume())
        else:
            raise Exception("Provided argument must be a SESegment")

    def is_valid(self):
        return self.has_begin_volume() and self.has_end_volume()

    def has_begin_volume(self):
        return False if self._begin_volume is None else self._begin_volume.is_valid()
    def get_begin_volume(self):
        if self._begin_volume is None:
            self._begin_volume = SEScalarVolume()
        return self._begin_volume

    def has_end_volume(self):
        return False if self._end_volume is None else self._end_volume.is_valid()
    def get_end_volume(self):
        if self._end_volume is None:
            self._end_volume = SEScalarVolume()
        return self._end_volume


class SESegmentConstant(SESegment):
    __slots__ = ["_compliance"]

    def __init__(self):
        super().__init__()
        self._compliance = None

    def clear(self):
        super().clear()
        if self._compliance is not None:
            self._compliance.invalidate()

    def copy(self, src):
        if isinstance(SESegmentConstant, src):
            super().copy(src)
            if src.has_compliance():
                self.get_compliance().set(src.get_compliance())
        else:
            raise Exception("Provided argument must be a SESegmentConstant")

    def is_valid(self):
        return super().is_valid() and self.has_compliance()

    def has_compliance(self):
        return False if self._compliance is None else self._compliance.is_valid()
    def get_compliance(self):
        if self._compliance is None:
            self._compliance = SEScalarVolumePerPressure()
        return self._compliance


class SESegmentLinear(SESegment):
    __slots__ = ["_slope",
                 "_y_intercept"]

    def __init__(self):
        super().__init__()
        self._slope = None
        self._y_intercept = None

    def clear(self):
        super().clear()
        if self._slope is not None:
            self._slope.invalidate()
        if self._y_intercept is not None:
            self._y_intercept.invalidate()

    def copy(self, src):
        if isinstance(SESegmentLinear, src):
            super().copy(src)
            if src.has_slope():
                self.get_slope().set(src.get_slope())
            if src.has_y_intercept():
                self.get_y_intercept().set(src.get_y_intercept())
        else:
            raise Exception("Provided argument must be a SESegmentLinear")

    def is_valid(self):
        return super().is_valid() and self.has_slope() and self.has_y_intercept()

    def has_slope(self):
        return False if self._slope is None else self._slope.is_valid()
    def get_slope(self):
        if self._slope is None:
            self._slope = SEScalarPressurePerVolume()
        return self._slope

    def has_y_intercept(self):
        return False if self._y_intercept is None else self._y_intercept.is_valid()
    def get_y_intercept(self):
        if self._y_intercept is None:
            self._y_intercept = SEScalarPressure()
        return self._y_intercept


class SESegmentParabolic(SESegment):
    __slots__ = ["_coefficient1",
                 "_coefficient2",
                 "_coefficient3",
                 "_coefficient4"]

    def __init__(self):
        super().__init__()
        self._coefficient1 = None
        self._coefficient2 = None
        self._coefficient3 = None
        self._coefficient4 = None

    def clear(self):
        super().clear()
        if self._coefficient1 is not None:
            self._coefficient1.invalidate()
        if self._coefficient2 is not None:
            self._coefficient2.invalidate()
        if self._coefficient3 is not None:
            self._coefficient3.invalidate()
        if self._coefficient4 is not None:
            self._coefficient4.invalidate()

    def copy(self, src):
        if isinstance(SESegmentParabolic, src):
            super().copy(src)
            if src.has_coefficient1():
                self.get_coefficient1().set(src.get_coefficient1())
            if src.has_coefficient2():
                self.get_coefficient2().set(src.get_coefficient2())
            if src.has_coefficient3():
                self.get_coefficient3().set(src.get_coefficient3())
            if src.has_coefficient4():
                self.get_coefficient4().set(src.get_coefficient4())
        else:
            raise Exception("Provided argument must be a SESegmentParabolic")

    def is_valid(self):
        return super().is_valid() \
            and self.has_coefficient1() \
            and self.has_coefficient2() \
            and self.has_coefficient3() \
            and self.has_coefficient4()

    def has_coefficient1(self):
        return False if self._coefficient1 is None else self._coefficient1.is_valid()
    def get_coefficient1(self):
        if self._coefficient1 is None:
            self._coefficient1 = SEScalar()
        return self._coefficient1

    def has_coefficient2(self):
        return False if self._coefficient2 is None else self._coefficient2.is_valid()
    def get_coefficient2(self):
        if self._coefficient2 is None:
            self._coefficient2 = SEScalar()
        return self._coefficient2

    def has_coefficient3(self):
        return False if self._coefficient3 is None else self._coefficient3.is_valid()
    def get_coefficient3(self):
        if self._coefficient3 is None:
            self._coefficient3 = SEScalar()
        return self._coefficient3

    def has_coefficient4(self):
        return False if self._coefficient4 is None else self._coefficient4.is_valid()
    def get_coefficient4(self):
        if self._coefficient4 is None:
            self._coefficient4 = SEScalar()
        return self._coefficient4


class SESegmentSigmoidal(SESegment):
    __slots__ = ["_lower_corner",
                 "_upper_corner",
                 "_baseline_compliance"]

    def __init__(self):
        super().__init__()
        self._lower_corner = None
        self._upper_corner = None
        self._baseline_compliance = None

    def clear(self):
        super().clear()
        if self._lower_corner is not None:
            self._lower_corner.invalidate()
        if self._upper_corner is not None:
            self._upper_corner.invalidate()
        if self._baseline_compliance is not None:
            self._baseline_compliance.invalidate()

    def copy(self, src):
        if isinstance(SESegmentSigmoidal, src):
            super().copy()
            if src.has_lower_corner():
                self.get_lower_corner().set(src.get_lower_corner())
            if src.has_upper_corner():
                self.get_upper_corner().set(src.get_upper_corner())
            if src.has_baseline_compliance():
                self.get_baseline_compliance().set(src.get_baseline_compliance())
        else:
            raise Exception("Provided argument must be a SESegmentSigmoidal")

    def is_valid(self):
        return super().is_valid() and \
            self.has_lower_corner() and \
            self.has_upper_corner() and \
            self.has_baseline_compliance()

    def has_lower_corner(self):
        return False if self._lower_corner is None else self._lower_corner.is_valid()
    def get_lower_corner(self):
        if self._lower_corner is None:
            self._lower_corner = SEScalarPressure()
        return self._lower_corner

    def has_upper_corner(self):
        return False if self._upper_corner is None else self._upper_corner.is_valid()
    def get_upper_corner(self):
        if self._upper_corner is None:
            self._upper_corner = SEScalarPressure()
        return self._upper_corner

    def has_baseline_compliance(self):
        return False if self._baseline_compliance is None else self._baseline_compliance.is_valid()
    def get_baseline_compliance(self):
        if self._baseline_compliance is None:
            self._baseline_compliance = SEScalarVolumePerPressure()
        return self._baseline_compliance

class SECurve():
    __slots__ = ["_segments"]

    def __init__(self):
        self._segments = []

    def copy(self, src):
        if isinstance(SECurve, src) and src.has_segment():
            self._segments.clear()
            for s in src.get_segments():
                if isinstance(SESegmentConstant, s):
                    self.add_constant_segment().copy(s)
                elif isinstance(SESegmentLinear, s):
                    self.add_linear_segment().copy(s)
                if isinstance(SESegmentParabolic, s):
                    self.add_parabolic_segment().copy(s)
                if isinstance(SESegmentSigmoidal, s):
                    self.add_sigmoidal_segment().copy(s)
        else:
            raise Exception("Provided argument must be a SECurve with one or more segments")

    def invalidate(self):
        self._segments.clear()

    def is_valid(self):
        if not self.has_segment():
            return False
        for s in self._segments:
            if not s.is_valid():
                return False
        return True

    def has_segment(self):
        return len(self._segments) > 0

    def add_constant_segment(self):
        s = SESegmentConstant()
        self._segments.append(s)
        return s

    def add_linear_segment(self):
        s = SESegmentLinear()
        self._segments.append(s)
        return s

    def add_parabolic_segment(self):
        s = SESegmentParabolic()
        self._segments.append(s)
        return s

    def add_sigmoidal_segment(self):
        s = SESegmentSigmoidal()
        self._segments.append(s)
        return s

    def get_segments(self):
        return self._segments
