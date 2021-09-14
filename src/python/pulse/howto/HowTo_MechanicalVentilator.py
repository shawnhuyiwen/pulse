# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.mechanical_ventilator_actions import SEMechanicalVentilatorConfiguration
from pulse.cdm.scalars import PressureUnit, TimeUnit
from pulse.cdm.mechanical_ventilator import eSwitch, eDriverWaveform
from pulse.cdm.engine import SEDataRequest, SEDataRequestManager
from pulse.cdm.patient_actions import SEDyspnea

def HowTo_MechanicalVentilator():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/pypulse_MechanicalVentilator.log")
    pulse.log_to_console(True)

    data_requests = [
        SEDataRequest.create_physiology_request("HeartRate", "1/min"),
        SEDataRequest.create_physiology_request("ArterialPressure", "mmHg"),
        SEDataRequest.create_physiology_request("MeanArterialPressure", "mmHg"),
        SEDataRequest.create_physiology_request("SystolicArterialPressure", "mmHg"),
        SEDataRequest.create_physiology_request("DiastolicArterialPressure", "mmHg"),
        SEDataRequest.create_physiology_request("OxygenSaturation"),
        SEDataRequest.create_physiology_request("EndTidalCarbonDioxidePressure", "mmHg"),
        SEDataRequest.create_physiology_request("RespirationRate", "1/min"),
        SEDataRequest.create_physiology_request("SkinTemperature", "degC"),
        SEDataRequest.create_gas_compartment_substance_request("Carina", "CarbonDioxide", "PartialPressure", "mmHg"),
        SEDataRequest.create_physiology_request("BloodVolume", "mL"),
        SEDataRequest.create_ecg_request("Lead3ElectricPotential", "mV"),
        # Ventilator Monitor Data
        SEDataRequest.create_mechanical_ventilator_request("AirwayPressure", "cmH2O"),
        SEDataRequest.create_mechanical_ventilator_request("EndTidalCarbonDioxideFraction"),
        SEDataRequest.create_mechanical_ventilator_request("EndTidalCarbonDioxidePressure", "cmH2O"),
        SEDataRequest.create_mechanical_ventilator_request("EndTidalOxygenFraction"),
        SEDataRequest.create_mechanical_ventilator_request("EndTidalOxygenPressure", "cmH2O"),
        SEDataRequest.create_mechanical_ventilator_request("ExpiratoryFlow", "L/s"),
        SEDataRequest.create_mechanical_ventilator_request("ExpiratoryTidalVolume", "L"),
        SEDataRequest.create_mechanical_ventilator_request("InspiratoryExpiratoryRatio"),
        SEDataRequest.create_mechanical_ventilator_request("InspiratoryFlow", "L/s"),
        SEDataRequest.create_mechanical_ventilator_request("InspiratoryTidalVolume", "L"),
        SEDataRequest.create_mechanical_ventilator_request("IntrinsicPositiveEndExpiredPressure", "cmH2O"),
        SEDataRequest.create_mechanical_ventilator_request("LeakFraction"),
        SEDataRequest.create_mechanical_ventilator_request("MeanAirwayPressure", "cmH2O"),
        SEDataRequest.create_mechanical_ventilator_request("PeakInspiratoryPressure", "cmH2O"),
        SEDataRequest.create_mechanical_ventilator_request("PlateauPressure", "cmH2O"),
        SEDataRequest.create_mechanical_ventilator_request("PositiveEndExpiratoryPressure", "cmH2O"),
        SEDataRequest.create_mechanical_ventilator_request("RespirationRate", "1/min"),
        SEDataRequest.create_mechanical_ventilator_request("TidalVolume", "L"),
        SEDataRequest.create_mechanical_ventilator_request("TotalLungVolume", "L"),
        SEDataRequest.create_mechanical_ventilator_request("TotalPulmonaryVentilation", "L/s")]

    data_mgr = SEDataRequestManager(data_requests)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/StandardMale@0s.pbb", data_mgr):
        print("Unable to load initial state file")
        return

    # Get default data at time 0s from the engine
    results = pulse.pull_data()
    print(results)

    # Give the patient Dyspnea
    dsypnea = SEDyspnea()
    dsypnea.set_comment("Patient's dsypnea occurs")
    dsypnea.get_severity().set_value(1.0)
    pulse.process_action(dsypnea)
    pulse.advance_time_s(10)
    results = pulse.pull_data()
    print(results)

    # Here is an example of programming a custom ventilator mode
    mechanical_ventilator_cfg = SEMechanicalVentilatorConfiguration()
    mechanical_ventilator_cfg.set_comment("Attach the mechanical ventilator")
    # You could just point to an mechanical ventilator configuration file if you want (no examples)
    # mechanical_ventilator.set_settings_file("/path/to/file")
    mechanical_ventilator = mechanical_ventilator_cfg.get_settings()
    mechanical_ventilator.set_connection(eSwitch.On)
    mechanical_ventilator.set_inspiration_waveform(eDriverWaveform.Square)
    mechanical_ventilator.set_expiration_waveform(eDriverWaveform.Square)
    mechanical_ventilator.get_peak_inspiratory_pressure().set_value(21.0, PressureUnit.cmH2O)
    mechanical_ventilator.get_positive_end_expired_pressure().set_value(10.0, PressureUnit.cmH2O)
    mechanical_ventilator.get_fraction_inspired_gas("Oxygen").get_fraction_amount().set_value(0.5)
    respiration_rate_per_min = 20.0
    ie_ratio = 0.5
    total_period_s = 60.0 / respiration_rate_per_min
    inspiratory_period_s = ie_ratio * total_period_s / (1 + ie_ratio)
    expiratory_period_s = total_period_s - inspiratory_period_s
    mechanical_ventilator.get_inspiration_machine_trigger_time().set_value(expiratory_period_s, TimeUnit.s)
    mechanical_ventilator.get_expiration_cycle_time().set_value(inspiratory_period_s, TimeUnit.s)
    pulse.process_action(mechanical_ventilator_cfg)
    pulse.advance_time_s(10)
    results = pulse.pull_data()
    print(results)


HowTo_MechanicalVentilator()