# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from pulse.cdm.engine import eSerializationFormat
from google.protobuf.json_format import MessageToJson

from pulse.cdm.engine import SEAction
from pulse.cdm.bind.Actions_pb2 import ActionData
from pulse.cdm.bind.Engine_pb2 import ActionListData
from pulse.cdm.bind.Engine_pb2 import AnyActionData
# Patient Actions
from pulse.cdm.patient_actions import SEPatientAction
from pulse.cdm.bind.PatientActions_pb2 import PatientActionData
from pulse.cdm.patient_actions import SEHemorrhage
from pulse.cdm.bind.PatientActions_pb2 import HemorrhageData
# Scalars
from pulse.cdm.scalars import SEScalarVolumePerTime
from pulse.cdm.bind.Properties_pb2 import ScalarVolumePerTimeData


def SerializeAction(src: SEAction, dst: ActionData):
    dst.Comment = src.get_comment()
def SerializeActions(actions: [], fmt: eSerializationFormat):
    aD = ActionListData()
    for action in actions:
        if not action.is_valid():
            print("Skipping invalid action")
            continue
        print(action)
        aaD = AnyActionData()
        if isinstance(action, SEHemorrhage):
            SerializeHemorrhage(action, aaD.PatientAction.Hemorrhage)
            aD.AnyAction.append(aaD)
            continue
    return MessageToJson(aD,True,True)

# Patient Actions
def SerializePatientAction(src: SEPatientAction, dst: PatientActionData):
    SerializeAction(src,dst.Action)
def SerializeHemorrhage(src: SEHemorrhage, dst: HemorrhageData):
    SerializePatientAction(src, dst.PatientAction)
    dst.Compartment = src.get_compartment()
    dst.Type = src.get_type().value
    SerializeScalarVolumePerTime(src.get_rate(), dst.Rate)

# Scalars
def SerializeScalarVolumePerTime(src: SEScalarVolumePerTime, dst: ScalarVolumePerTimeData):
    dst.ScalarVolumePerTime.Value = src.get_value()
    dst.ScalarVolumePerTime.Unit = src.get_unit().get_string()
