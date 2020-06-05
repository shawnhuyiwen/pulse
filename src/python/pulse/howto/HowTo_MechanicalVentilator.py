# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine
from pulse.cdm.mechanical_ventilator_actions import SEMechanicalVentilatorConfiguration
from pulse.cdm.scalars import FrequencyUnit, TimeUnit, MassPerVolumeUnit
from pulse.cdm.mechanical_ventilator import eConnection, eControl, eDriverWaveform

def HowTo_MechanicalVentilator():
    pulse = PulsePhysiologyEngine()
    pulse.set_log_filename("./test_results/pypulse_MechanicalVentilator.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON):
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
    mechanical_ventilator.set_connection(eConnection.Off)
    mechanical_ventilator.set_control(eControl.PM_CMV)
    mechanical_ventilator.set_driver_waveform(eDriverWaveform.Square)
    # You can set a breath profile by setting the respiration rate OR breath period
    mechanical_ventilator.get_respiration_rate().set_value(17, FrequencyUnit.Per_min)
    #mechanical_ventilator.get_breath_period().set_value(4, TimeUnit.s)
    # A breath profile MUST also have the I/E ratio set
    mechanical_ventilator.get_inspiratory_experiatory_ratio().set_value(2)
    # Instead of a breath profile, you can set the explicit inspiratory and experiatory periods
    mechanical_ventilator.get_expiratory_period().set_value(2, TimeUnit.s)
    mechanical_ventilator.get_inspriatory_period().set_value(3, TimeUnit.s)
    # PIP and PEEP should always be set
    #mechanical_ventilator.get_peak_inspriatory_pressure().set_value(4, PressureUnit.mmHg)
    #mechanical_ventilator.get_positive_end_expired_pressure().set_value(5, PressureUnit.mmHg)
    # When changing insipred gas mixture, fractions must sum to 1 OR
    # You can only provide O2 (FiO2) and we will set CO2 to normal levels and the remaining will be N2
    mechanical_ventilator.remove_fraction_inspired_gasses()
    mechanical_ventilator.get_fraction_inspired_gas("Nitrogen").get_fraction_amount().set_value(0.7896)
    mechanical_ventilator.get_fraction_inspired_gas("Oxygen").get_fraction_amount().set_value(0.21)
    mechanical_ventilator.get_fraction_inspired_gas("CarbonDioxide").get_fraction_amount().set_value(4.0E-4)
    # Note Gaseous Drugs can be added to the ambient gas mixture this way
    # You can also provide an aerosolized substance to the mixture
    mechanical_ventilator.get_concentration_inspired_aerosol("Albuterol").get_concentration().set_value(12, MassPerVolumeUnit.ug_Per_mL)
    pulse.process_action(mechanical_ventilator_cfg)

    # Advance some time and print out the vitals
    pulse.advance_time_s(300)
    results = pulse.pull_data()
    print(results)


HowTo_MechanicalVentilator()