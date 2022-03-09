# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.ecmo import SEECMOSettings
from pulse.cdm.bind.ECMO_pb2 import ECMOSettingsData
from pulse.cdm.bind.Substance_pb2 import SubstanceFractionData, SubstanceConcentrationData

from pulse.cdm.io.scalars import *

def serialize_ecmo_settings_to_bind(src: SEECMOSettings, dst: ECMOSettingsData):
    dst.InflowLocation = src.get_inflow_location().value
    dst.OutflowLocation = src.get_outflow_location().value
    if src.has_oxygenator_volume():
        serialize_scalar_volume_to_bind(src.get_oxygenator_volume(), dst.OxygenatorVolume)
    if src.has_transfusion_flow():
        serialize_scalar_volume_per_time_to_bind(src.get_transfusion_flow(), dst.TransfusionFlow)

    if src.has_substance_compound():
        dst.SubstanceCompound = src.get_substance_compound()

    for subc in src.get_substance_concentrations():
        sc = SubstanceConcentrationData()
        sc.Name = subc.get_substance()
        serialize_scalar_mass_per_volume_to_bind(subc.get_concentration(), sc.Concentration)
        dst.SubstanceConcentrations.append(sc)

def serialize_ecmo_from_bind(src: ECMOSettingsData, dst: SEECMOSettings):
    raise Exception("serialize_ecmo_from_bind not implemented")