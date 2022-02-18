# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.ecmo_actions import SEECMOConfiguration
from pulse.cdm.scalars import FrequencyUnit, PressureUnit, PressureTimePerVolumeUnit, \
                              TimeUnit, VolumeUnit, VolumePerPressureUnit, VolumePerTimeUnit
from pulse.cdm.ecmo import eSwitch
from pulse.cdm.engine import SEDataRequest, SEDataRequestManager
from pulse.cdm.patient_actions import SEDyspnea


def HowTo_ECMO():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_ECMO.py.log")
    pulse.log_to_console(True)

    data_requests = [
        SEDataRequest.create_physiology_request("RespirationRate", unit=FrequencyUnit.Per_min),
        SEDataRequest.create_physiology_request("TidalVolume", unit=VolumeUnit.mL),
        SEDataRequest.create_physiology_request("TotalLungVolume", unit=VolumeUnit.mL),
        SEDataRequest.create_physiology_request("ExpiratoryPulmonaryResistance", unit=PressureTimePerVolumeUnit.cmH2O_s_Per_L),
        SEDataRequest.create_physiology_request("InspiratoryPulmonaryResistance", unit=PressureTimePerVolumeUnit.cmH2O_s_Per_L),
        SEDataRequest.create_physiology_request("PulmonaryCompliance", unit=VolumePerPressureUnit.L_Per_cmH2O),
        SEDataRequest.create_physiology_request("TotalPulmonaryVentilation", unit=VolumePerTimeUnit.L_Per_min),
        # Ventilator Monitor Data
        SEDataRequest.create_ecmo_request("AirwayPressure", unit=PressureUnit.cmH2O),
        SEDataRequest.create_ecmo_request("EndTidalCarbonDioxideFraction"),
        SEDataRequest.create_ecmo_request("EndTidalCarbonDioxidePressure", unit=PressureUnit.cmH2O),
        SEDataRequest.create_ecmo_request("EndTidalOxygenFraction"),
        SEDataRequest.create_ecmo_request("EndTidalOxygenPressure", unit=PressureUnit.cmH2O),
        SEDataRequest.create_ecmo_request("ExpiratoryFlow", unit=VolumePerTimeUnit.L_Per_s),
        SEDataRequest.create_ecmo_request("ExpiratoryTidalVolume", unit=VolumeUnit.L),
        SEDataRequest.create_ecmo_request("InspiratoryExpiratoryRatio"),
        SEDataRequest.create_ecmo_request("InspiratoryFlow", unit=VolumePerTimeUnit.L_Per_s),
        SEDataRequest.create_ecmo_request("InspiratoryTidalVolume", unit=VolumeUnit.L),
        SEDataRequest.create_ecmo_request("IntrinsicPositiveEndExpiredPressure", unit=PressureUnit.cmH2O),
        SEDataRequest.create_ecmo_request("LeakFraction"),
        SEDataRequest.create_ecmo_request("MeanAirwayPressure", unit=PressureUnit.cmH2O),
        SEDataRequest.create_ecmo_request("PeakInspiratoryPressure", unit=PressureUnit.cmH2O),
        SEDataRequest.create_ecmo_request("PlateauPressure", unit=PressureUnit.cmH2O),
        SEDataRequest.create_ecmo_request("PositiveEndExpiratoryPressure", unit=PressureUnit.cmH2O),
        SEDataRequest.create_ecmo_request("RespirationRate", unit=FrequencyUnit.Per_min),
        SEDataRequest.create_ecmo_request("TidalVolume", unit=VolumeUnit.L),
        SEDataRequest.create_ecmo_request("TotalLungVolume", unit=VolumeUnit.L),
        SEDataRequest.create_ecmo_request("TotalPulmonaryVentilation", unit=VolumePerTimeUnit.L_Per_s)
    ]

    data_mgr = SEDataRequestManager(data_requests)
    data_mgr.set_results_filename("./test_results/howto/HowTo_ECMO.py.csv")

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/StandardMale@0s.pbb", data_mgr):
        print("Unable to load initial state file")
        return

    # Get default data at time 0s from the engine
    results = pulse.pull_data()
    data_mgr.to_console(results)

    # Give the patient Dyspnea
    dsypnea = SEDyspnea()
    dsypnea.set_comment("Patient's dsypnea occurs")
    dsypnea.get_severity().set_value(1.0)
    pulse.process_action(dsypnea)
    pulse.advance_time_s(10)
    # Get the values of the data you requested at this time
    results = pulse.pull_data()
    # And write it out to the console
    data_mgr.to_console(results)


HowTo_ECMO()
