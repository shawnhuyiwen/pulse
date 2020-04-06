# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.impl.bind.MultiplexVentilator_pb2 import  *
from google.protobuf import json_format

def HowTo_UseEngine():
    # Let's load up a data file from disk
    f = open("./mv_solo_ventilated_patients.json", "rb")
    json = f.read()
    f.close()
    # Parse it into our protobuf object
    patient_list = PatientStateListData()
    json_format.Parse(json, patient_list)
    # Let's examine the data
    for p in patient_list.Patients:
        print(p.Compliance_L_Per_cmH2O)


HowTo_UseEngine()