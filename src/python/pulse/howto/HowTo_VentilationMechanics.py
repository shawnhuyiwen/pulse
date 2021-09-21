from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.engine import SEDataRequest, SEDataRequestManager
from pulse.cdm.mechanical_ventilator import eSwitch
from pulse.cdm.mechanical_ventilator_actions import SEMechanicalVentilatorPressureControl, \
                                                    eMechanicalVentilator_PressureControlMode
from pulse.cdm.patient import eSex, SEPatientConfiguration
from pulse.cdm.patient_actions import eIntubationType, SEIntubation, SERespiratoryMechanicsConfiguration
from pulse.cdm.scalars import FrequencyUnit, PressureUnit, PressureTimePerVolumeUnit,\
                              TimeUnit, VolumeUnit, VolumePerPressureUnit

def HowTo_VentilationMechanics():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/pypulse_VentilationMechanics.log")
    pulse.log_to_console(True)

    data_requests = [
        SEDataRequest.create_physiology_request("RespirationRate", "1/min"),
        SEDataRequest.create_physiology_request("TidalVolume", "mL"),
        SEDataRequest.create_physiology_request("TotalLungVolume", "mL"),
        SEDataRequest.create_physiology_request("InspiratoryExpiratoryRatio"),
        SEDataRequest.create_physiology_request("MeanAirwayPressure", "cmH2O"),
        SEDataRequest.create_physiology_request("TransrespiratoryPressure", "cmH2O"),
        SEDataRequest.create_physiology_request("IntrapulmonaryPressure", "cmH2O"),
        SEDataRequest.create_physiology_request("InspiratoryFlow", "L/min"),
        SEDataRequest.create_physiology_request("ExpiratoryPulmonaryResistance", "cmH2O s/mL"),
        SEDataRequest.create_physiology_request("InspiratoryPulmonaryResistance", "cmH2O s/mL"),
        SEDataRequest.create_physiology_request("PulmonaryCompliance", "mL/cmH2O"),
        SEDataRequest.create_physiology_request("TotalPulmonaryVentilation", "mL/min"),
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
        # SEDataRequest.create_mechanical_ventilator_request("PulmonaryCompliance", "L/cmH2O"),
        # SEDataRequest.create_mechanical_ventilator_request("PulmonaryElastance", "cmH2O/L"),
        # SEDataRequest.create_mechanical_ventilator_request("RelativeTotalLungVolume", "L"),
        SEDataRequest.create_mechanical_ventilator_request("RespirationRate", "1/min"),
        SEDataRequest.create_mechanical_ventilator_request("TidalVolume", "L"),
        SEDataRequest.create_mechanical_ventilator_request("TotalLungVolume", "L"),
        SEDataRequest.create_mechanical_ventilator_request("TotalPulmonaryVentilation", "L/s")]

    data_mgr = SEDataRequestManager(data_requests)
    data_mgr.set_results_filename("./test_results/howto/HowTo_VentilationMechanics.py.csv")

    # # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    # if not pulse.serialize_from_file("./states/StandardMale@0s.pbb", data_mgr):
    #     print("Unable to load initial state file")
    #     return

    # You only need to set a sex
    pCfg = SEPatientConfiguration()
    pCfg.get_patient().set_sex(eSex.Male)

    # State serialization is not needed, this initializes instantaneously
    pulse.initialize_engine(pCfg, data_mgr)

    # Since this engine is just the ventilation mechanics
    # You configure the patient respiratory via the RespiratoryMechanics structure
    config = SERespiratoryMechanicsConfiguration()
    mechanics = config.get_settings()
    mechanics.set_active(eSwitch.On)
    # Create a mechanics profile
    resistance_cmH2O_s_Per_L = 13.0
    compliance_mL_Per_cmH2O = 50.0
    musclePressure_cmH2O = 13.0
    respirationRate_bpm = 15.0
    inspiratoryTime_s = 0.9

    # These need to add to 1.0 (100%)
    UpperResistanceFraction = 0.5
    BronchiResistanceFraction = 0.3
    AlveoliDuctResistanceFraction = 0.2

    upperResistance = resistance_cmH2O_s_Per_L - (BronchiResistanceFraction * resistance_cmH2O_s_Per_L + AlveoliDuctResistanceFraction * resistance_cmH2O_s_Per_L) / 2
    bronchiResistance = 2 * (resistance_cmH2O_s_Per_L - upperResistance) - AlveoliDuctResistanceFraction * resistance_cmH2O_s_Per_L
    alveoliDuctResistance = 2 * (resistance_cmH2O_s_Per_L - upperResistance) - bronchiResistance
    sideResistance = bronchiResistance + alveoliDuctResistance

    RespiratorySideCompliance_L_Per_cmH2O = compliance_mL_Per_cmH2O / 2.0
    LungCompliance_L_Per_cmH2O = 2.0 * RespiratorySideCompliance_L_Per_cmH2O
    ChestWallCompliance_L_Per_cmH2O = LungCompliance_L_Per_cmH2O

    totalBreathTime_s = 1.0 / (respirationRate_bpm / 60.0)
    inspiratoryFraction = inspiratoryTime_s / totalBreathTime_s
    expiratoryFraction = 1.0 - inspiratoryFraction

    InspiratoryRiseFraction = inspiratoryFraction
    InspiratoryHoldFraction = 0.0
    InspiratoryReleaseFraction = min(inspiratoryFraction, expiratoryFraction * 0.5)
    InspiratoryToExpiratoryPauseFraction = 1.0 - InspiratoryRiseFraction - InspiratoryReleaseFraction
    ExpiratoryRiseFraction = 0.0
    ExpiratoryHoldFraction = 0.0
    ExpiratoryReleaseFraction = 0.0

    mechanics.get_upper_inspiratory_resistance().set_value(upperResistance, PressureTimePerVolumeUnit.cmH2O_s_Per_L)
    mechanics.get_upper_expiratory_resistance().set_value(upperResistance, PressureTimePerVolumeUnit.cmH2O_s_Per_L)
    mechanics.get_left_inspiratory_resistance().set_value(sideResistance, PressureTimePerVolumeUnit.cmH2O_s_Per_L)
    mechanics.get_left_expiratory_resistance().set_value(sideResistance, PressureTimePerVolumeUnit.cmH2O_s_Per_L)
    mechanics.get_right_inspiratory_resistance().set_value(sideResistance, PressureTimePerVolumeUnit.cmH2O_s_Per_L)
    mechanics.get_right_expiratory_resistance().set_value(sideResistance, PressureTimePerVolumeUnit.cmH2O_s_Per_L)

    lcc = mechanics.get_left_compliance_curve()
    lc = lcc.add_constant_segment()
    lc.get_begin_volume().set_value(float("-inf"), VolumeUnit.mL)
    lc.get_end_volume().set_value(float("inf"), VolumeUnit.mL)
    lc.get_compliance().set_value(RespiratorySideCompliance_L_Per_cmH2O, VolumePerPressureUnit.mL_Per_cmH2O)
    rcc = mechanics.get_right_compliance_curve()
    rc = rcc.add_constant_segment()
    rc.get_begin_volume().set_value(float("-inf"), VolumeUnit.mL)
    rc.get_end_volume().set_value(float("inf"), VolumeUnit.mL)
    rc.get_compliance().set_value(RespiratorySideCompliance_L_Per_cmH2O, VolumePerPressureUnit.mL_Per_cmH2O)

    mechanics.get_inspiratory_rise_time().set_value(InspiratoryRiseFraction * totalBreathTime_s, TimeUnit.s)
    mechanics.get_inspiratory_hold_time().set_value(InspiratoryHoldFraction * totalBreathTime_s, TimeUnit.s)
    mechanics.get_inspiratory_release_time().set_value(InspiratoryReleaseFraction * totalBreathTime_s, TimeUnit.s)
    mechanics.get_inspiratory_to_expiratory_pause_time().set_value(InspiratoryToExpiratoryPauseFraction * totalBreathTime_s, TimeUnit.s)
    mechanics.get_expiratory_rise_time().set_value(ExpiratoryRiseFraction * totalBreathTime_s, TimeUnit.s)
    mechanics.get_expiratory_hold_time().set_value(ExpiratoryHoldFraction * totalBreathTime_s, TimeUnit.s)
    mechanics.get_expiratory_release_time().set_value(ExpiratoryReleaseFraction * totalBreathTime_s, TimeUnit.s)
    mechanics.get_inspiratory_peak_pressure().set_value(-13.0, PressureUnit.cmH2O)
    mechanics.get_expiratory_peak_pressure().set_value(0.0, PressureUnit.cmH2O)
    pulse.process_action(config)

    # Intubate
    intubation = SEIntubation()
    intubation.set_type(eIntubationType.Tracheal)
    pulse.process_action(intubation)

    # Setup the ventilator
    pc_ac = SEMechanicalVentilatorPressureControl()
    pc_ac.set_connection(eSwitch.On)
    pc_ac.set_mode(eMechanicalVentilator_PressureControlMode.AssistedControl)
    pc_ac.get_fraction_inspired_oxygen().set_value(0.21)
    pc_ac.get_inspiratory_period().set_value(1.0, TimeUnit.s)
    pc_ac.get_inspiratory_pressure().set_value(19.0, PressureUnit.cmH2O)
    pc_ac.get_positive_end_expired_pressure().set_value(5.0, PressureUnit.cmH2O)
    pc_ac.get_respiration_rate().set_value(12.0, FrequencyUnit.Per_min)
    pc_ac.get_slope().set_value(0.2, TimeUnit.s)
    pulse.process_action(pc_ac)

    for i in range(12):
        pulse.advance_time_s(10)
        # Get the values of the data you requested at this time
        results = pulse.pull_data()
        # And write it out to the console
        data_mgr.to_console(results)


HowTo_VentilationMechanics()
