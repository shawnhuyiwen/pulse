# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import os
import sys
from multiprocessing.pool import ThreadPool as Pool

from pulse.cdm.engine import SEDataRequestManager, SEDataRequest
from pulse.cdm.engine import IEventHandler, SEEventChange, ILoggerForward

from pulse.cdm.patient import eSex, SEPatientConfiguration
from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.scalars import FrequencyUnit, PressureUnit, TemperatureUnit, VolumeUnit, VolumePerTimeUnit

# Advance all engines in 10s increments
_advance_time_s = 10.

class PoolPatient:
    __slots__ = ["_id", "is_active",
                 "pulse",
                 "data_request_mgr",
                 "patient_configuration",
                 "state_filename",
                 "actions"]

    def __init__(self, id: int):
        self._id = id
        self.is_active = False
        self.pulse = PulseEngine()
        self.data_request_mgr = None
        self.patient_configuration = None
        self.state_filename = None
        self.actions = []

    def get_id(self):
        return self._id

def initialize_engine(p: PoolPatient):
    print("Initializing p" + str(p.get_id()) + "...")
    p.pulse.set_log_filename("./test_results/howto/pool/p"+str(p.get_id())+".log")
    p.pulse.set_event_handler(local_event_handler(p))
    p.pulse.set_log_listener(local_log_fowrwad(p))
    p.pulse.log_to_console(False) # Too many messages if a lot of engines

    if p.patient_configuration is not None:
        p.patient_configuration.set_data_root_dir("./")
        if not p.pulse.initialize_engine(p.patient_configuration, p.data_request_mgr):
            p.is_active = False
            print("Unable to load stabilize engine")
            return
    elif p.state_filename is not None:
        if not p.pulse.serialize_from_file(p.state_filename, p.data_request_mgr):
            p.is_active = False
            print("Unable to load initial state file")
            return

    print("Engine p"+str(p.get_id())+" initialized")
    p.is_active = True

def process_actions(p: PoolPatient):
    if p.is_active == False:
        return
    p.pulse.process_actions(p.actions)
    p.pulse.advance_time_s(_advance_time_s)

def process_engine_state(p: PoolPatient):
    results = p.pulse.pull_data()
    # Look at our data, add actions
    pass

def runJobPool(func, patients: list):
    # get the amount of cores for multiple runs
    cpus = os.cpu_count()
    if cpus > 1:
        cpus -= 1  # don't use all, so you can still check email...
    print("Creating a pool with "+str(cpus)+" threads")
    with Pool(processes=cpus) as pool:
        pool.map(func, patients)

class local_event_handler(IEventHandler):
    __slots__=["_patient"]
    def __init__(self, patient: PoolPatient):
        super().__init__()
        self._patient = patient
    def handle_event(self, change: SEEventChange):
        # Listen for specific event states you are interested in
        # This occurs on a separate thread, so set flags as needed
        # maybe create and add an action to p.actions
        # Of course, you could just pull active events in process_engine_state
        pass

class local_log_fowrwad(ILoggerForward):
    __slots__=["_patient"]
    def __init__(self, patient: PoolPatient):
        super().__init__()
        self._patient = patient
    def forward_debug(self, msg: str):
        pass
    def forward_info(self, msg: str):
        pass
    def forward_warning(self, msg: str):
        pass
    def forward_error(self, msg: str):
        self._patient.is_active = False
    def forward_fatal(self, msg: str):
        self._patient.is_active = False

def main():

    data_requests = [
        SEDataRequest.create_physiology_request("HeartRate", unit=FrequencyUnit.Per_min),
        SEDataRequest.create_physiology_request("ArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("MeanArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("SystolicArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("DiastolicArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("OxygenSaturation"),
        SEDataRequest.create_physiology_request("EndTidalCarbonDioxidePressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("RespirationRate", unit=FrequencyUnit.Per_min),
        SEDataRequest.create_physiology_request("SkinTemperature", unit=TemperatureUnit.C),
        SEDataRequest.create_physiology_request("CardiacOutput", unit=VolumePerTimeUnit.L_Per_min),
        SEDataRequest.create_physiology_request("BloodVolume", unit=VolumeUnit.mL)
    ]
    data_req_mgr = SEDataRequestManager(data_requests)

    # Create all your patients up front
    patients = []
    # You can create a new patient (See HowTo_EngineUse.py)
    p1 = PoolPatient(1)
    p1.data_request_mgr = data_req_mgr
    p1.patient_configuration = SEPatientConfiguration()
    p1.patient_configuration.get_patient().set_name("p1")
    p1.patient_configuration.get_patient().set_sex(eSex.Male)
    patients.append(p1)
    # Or you can use a preexisting state for a patient
    p2 = PoolPatient(2)
    p2.data_request_mgr = data_req_mgr
    p2.state_filename = "./states/StandardMale@0s.json"
    patients.append(p2)

    print("Initializing patients")
    runJobPool(initialize_engine, patients)
    print("Finished initialization")
    for i in range(12):
        print("Starting iteration "+str(i))
        runJobPool(process_actions, patients)
        print("Examining engine states...")
        runJobPool(process_engine_state, patients)
        # Remove inactive engines from patients?

if __name__ == "__main__":
    print("It is not recommended to use python threading with Pulse")
    print("Pulse is a native library, and each call will use the Python GIL")
    print("The GIL can sometimes not release from a native return, causing your program to lock")
    print("You may be better at handling these sorts of things though...")
    # For more info:
    # https://stackoverflow.com/questions/53855880/why-gil-is-not-synchrionizing-python-threads-that-are-running-native-c-code-in
    main()
    print("DONE")