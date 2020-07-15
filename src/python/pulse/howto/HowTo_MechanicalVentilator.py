# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine
from pulse.cdm.mechanical_ventilator_actions import SEMechanicalVentilatorConfiguration
from pulse.cdm.scalars import PressureUnit, TimeUnit, MassPerVolumeUnit
from pulse.cdm.mechanical_ventilator import eConnection, eDriverWaveform

def HowTo_MechanicalVentilator():
    pulse = PulsePhysiologyEngine()
    pulse.set_log_filename("./test_results/pypulse_MechanicalVentilator.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.pbb", None):
        print("Unable to load initial state file")
        return

    # Get default data at time 0s from the engine
    results = pulse.pull_data()
    print(results)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

    # Apply the mechanical ventilator
    mechanical_ventilator_cfg = SEMechanicalVentilatorConfiguration()
    mechanical_ventilator_cfg.set_comment("Attach the mechanical ventilator")
    # You could just point to an mechanical ventilator configuration file if you want (no examples)
    # mechanical_ventilator.set_configuration_file("/path/to/file")
    mechanical_ventilator = mechanical_ventilator_cfg.get_configuration()
    # Not all properties have to be set
    # Properties not included will retain their current value
    mechanical_ventilator.set_connection(eConnection.Mask)
    mechanical_ventilator.set_inspiration_waveform(eDriverWaveform.Square)
    mechanical_ventilator.set_expiration_waveform(eDriverWaveform.Square)
    mechanical_ventilator.get_peak_inspiratory_pressure().set_value(21, PressureUnit.cmH2O)
    mechanical_ventilator.get_positive_end_expired_pressure().set_value(10, PressureUnit.cmH2O)

    # Translate ventilator settings
    respiration_rate = 20
    ie_ratio = 0.5
    total_period_s = 60 / respiration_rate
    inspiratory_period_s = ie_ratio * total_period_s / (1 + ie_ratio)
    expiratory_period_s = total_period_s - inspiratory_period_s
    mechanical_ventilator.get_inspiration_machine_trigger_time().set_value(expiratory_period_s, TimeUnit.s)
    mechanical_ventilator.get_expiration_cycle_time().set_value(inspiratory_period_s, TimeUnit.s)

    mechanical_ventilator.remove_fraction_inspired_gasses()
    mechanical_ventilator.get_fraction_inspired_gas("Oxygen").get_fraction_amount().set_value(0.5)
    # Note Gaseous Drugs can be added to the ambient gas mixture this way
    # You can also provide an aerosolized substance to the mixture
    mechanical_ventilator.get_concentration_inspired_aerosol("Albuterol").get_concentration().set_value(12, MassPerVolumeUnit.ug_Per_mL)
    pulse.process_action(mechanical_ventilator_cfg)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)


HowTo_MechanicalVentilator()