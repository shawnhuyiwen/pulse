# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from typing import List

from pulse.cdm.properties import SEFunction
from pulse.cdm.bind.Properties_pb2 import FunctionData

def serialize_function_to_bind(src: SEFunction, dst: FunctionData):
    serialize_double_array_to_bind(src.get_dependent(), dst.Dependent)
    serialize_double_array_to_bind(src.get_independent(), dst.Independent)
    dst.DependentUnit = src.get_dependent_unit()
    dst.IndependentUnit = src.get_independent_unit()
def serialize_function_from_bind(src: FunctionData, dst: SEFunction):
    dst.clear()
    serialize_double_array_from_bind(src.Dependent, dst.get_dependent())
    serialize_double_array_from_bind(src.Independent, dst.get_independent())
    dst.set_dependent_unit(src.DependentUnit)
    dst.set_indpendent_unit(src.IndependentUnit)

def serialize_double_array_to_bind(src: List[float], dst: DoubleArrayData):
    for v in src:
        val = dst.Value.add()
        val = v
def serialize_double_array_from_bind(src: DoubleArrayData, dst: List[float]):
    dst = []
    for val in dst.Value:
        dst.append(val)
