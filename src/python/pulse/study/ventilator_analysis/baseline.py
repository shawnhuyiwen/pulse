# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

def data():
    return \
    {
        "patientSettings" : {
            "Healthy" : {
                "Resistance" : 13,
                "ResistanceUnits": "cmH2O-s/L",
                "Compliance" : 50,
                "ComplianceUnits": "mL/cmH2O",
                "MusclePressure" : 13,
                "MusclePressureUnits": "cmH2O",
                "SpontaneousRespirationRate" : 15,
                "SpontaneousRespirationRateUnits": "bpm",
                "NeuralInspiratoryTime" : 0.9,
                "NeuralInspiratoryTimeUnits": "s"
            },
            "Restrictive" : {
                "Resistance": 20,
                "ResistanceUnits": "cmH2O-s/L",
                "Compliance": 30,
                "ComplianceUnits": "mL/cmH2O",
                "MusclePressure": 8,
                "MusclePressureUnits": "cmH2O",
                "SpontaneousRespirationRate": 25,
                "SpontaneousRespirationRateUnits": "bpm",
                "NeuralInspiratoryTime": 0.6,
                "NeuralInspiratoryTimeUnits": "s"
            },
            "Obstructive" : {
                "Resistance": 21,
                "ResistanceUnits": "cmH2O-s/L",
                "Compliance": 53,
                "ComplianceUnits": "mL/cmH2O",
                "MusclePressure": 14,
                "MusclePressureUnits": "cmH2O",
                "SpontaneousRespirationRate": 18,
                "SpontaneousRespirationRateUnits": "bpm",
                "NeuralInspiratoryTime": 1.0,
                "NeuralInspiratoryTimeUnits": "s"
            }
        },
        "ventilatorSettings" :{
            "Healthy_VC_AC": {
                "TidalVolume": 900,
                "TidalVolumeUnits": "mL",
                "InspiratoryTime": 1.0,
                "InspiratoryTimeUnits": "s",
                "RespirationRate" : 12,
                "RespirationRateUnits" : "/min",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "Flow": 60,
                "FlowUnits": "L/min",
                "LeakSeverity": 0.49,
                "LeakSeverityUnits": "fraction"
            },
            "Healthy_PC_AC": {
                "InspiratoryTime": 1.0,
                "InspiratoryTimeUnits": "s",
                "RespirationRate": 12,
                "RespirationRateUnits": "/min",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "PeakInspiratoryPressure": 19,
                "PeakInspiratoryPressureUnits": "cmH2O",
                "LeakSeverity": 0.47,
                "LeakSeverityUnits": "fraction"
            },
            "Healthy_CPAP": {
                "DeltaPressureSupport": 10,
                "DeltaPressureSupportUnits": "cmH2O",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "Slope": 0.2,
                "SlopeUnits": "s",
                "LeakSeverity": 0.42,
                "LeakSeverityUnits": "fraction"
            },
            "Restrictive_VC_AC": {
                "TidalVolume": 550,
                "TidalVolumeUnits": "mL",
                "InspiratoryTime": 1.1,
                "InspiratoryTimeUnits": "s",
                "RespirationRate": 12,
                "RespirationRateUnits": "/min",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "Flow": 40,
                "FlowUnits": "L/min",
                "LeakSeverity": 0.38,
                "LeakSeverityUnits": "fraction"
            },
            "Restrictive_PC_AC": {
                "InspiratoryTime": 1.1,
                "InspiratoryTimeUnits": "s",
                "RespirationRate": 12,
                "RespirationRateUnits": "/min",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "PeakInspiratoryPressure": 23,
                "PeakInspiratoryPressureUnits": "cmH2O",
                "LeakSeverity": 0.32,
                "LeakSeverityUnits": "fraction"
            },
            "Restrictive_CPAP": {
                "DeltaPressureSupport": 10,
                "DeltaPressureSupportUnits": "cmH2O",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "Slope": 0.2,
                "SlopeUnits": "s",
                "LeakSeverity": 0.26,
                "LeakSeverityUnits": "fraction"
            },
            "Obstructive_VC_AC": {
                "TidalVolume": 500,
                "TidalVolumeUnits": "mL",
                "InspiratoryTime": 1.1,
                "InspiratoryTimeUnits": "s",
                "RespirationRate": 12,
                "RespirationRateUnits": "/min",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "Flow": 40,
                "FlowUnits": "L/min",
                "LeakSeverity": 0.37,
                "LeakSeverityUnits": "fraction"
            },
            "Obstructive_PC_AC": {
                "InspiratoryTime": 1.2,
                "InspiratoryTimeUnits": "s",
                "RespirationRate": 12,
                "RespirationRateUnits": "/min",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "PeakInspiratoryPressure": 12,
                "PeakInspiratoryPressureUnits": "cmH2O",
                "LeakSeverity": 0.36,
                "LeakSeverityUnits": "fraction"
            },
            "Obstructive_CPAP": {
                "DeltaPressureSupport": 10,
                "DeltaPressureSupportUnits": "cmH2O",
                "PositiveEndExpiredPressure": 5,
                "PositiveEndExpiredPressureUnits": "cmH2O",
                "Slope": 0.2,
                "SlopeUnits": "s",
                "LeakSeverity": 0.45,
                "LeakSeverityUnits": "fraction"
            }
        },
        "validationData" : {
            "Healthy_VC_AC": {
                "MechanicalVentilator-RespirationRate(1/min)" : 15,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 7.9,
                "MechanicalVentilator-TidalVolume(mL)" : 900,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 960,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 829,
                "MechanicalVentilator-LeakFraction": 0.13
            },
            "Healthy_PC_AC": {
                "MechanicalVentilator-RespirationRate(1/min)": 15,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 8.6,
                "MechanicalVentilator-TidalVolume(mL)": 1047,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 1116,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 985,
                "MechanicalVentilator-LeakFraction": 0.12
            },
            "Healthy_CPAP": {
                "MechanicalVentilator-RespirationRate(1/min)": 15,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 6.9,
                "MechanicalVentilator-TidalVolume(mL)": 905,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 941,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 827,
                "MechanicalVentilator-LeakFraction": 0.11
            },
            "Restrictive_VC_AC": {
                "MechanicalVentilator-RespirationRate(1/min)": 25,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 13,
                "MechanicalVentilator-TidalVolume(mL)": 550,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 596,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 526,
                "MechanicalVentilator-LeakFraction": 0.11
            },
            "Restrictive_PC_AC": {
                "MechanicalVentilator-RespirationRate(1/min)": 25,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 12,
                "MechanicalVentilator-TidalVolume(mL)": 528,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 579,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 509,
                "MechanicalVentilator-LeakFraction": 0.13
            },
            "Restrictive_CPAP": {
                "MechanicalVentilator-RespirationRate(1/min)": 25,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 10,
                "MechanicalVentilator-TidalVolume(mL)": 387,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 410,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 365,
                "MechanicalVentilator-LeakFraction": 0.08
            },
            "Obstructive_VC_AC": {
                "MechanicalVentilator-RespirationRate(1/min)": 18,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 7.3,
                "MechanicalVentilator-TidalVolume(mL)": 493,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 518,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 462,
                "MechanicalVentilator-LeakFraction": 0.11
            },
            "Obstructive_PC_AC": {
                "MechanicalVentilator-RespirationRate(1/min)": 18,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 7.8,
                "MechanicalVentilator-TidalVolume(mL)": 571,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 604,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 543,
                "MechanicalVentilator-LeakFraction": 0.11
            },
            "Obstructive_CPAP": {
                "MechanicalVentilator-RespirationRate(1/min)": 18,
                "MechanicalVentilator-MeanAirwayPressure(cmH2O)": 8.5,
                "MechanicalVentilator-TidalVolume(mL)": 657,
                "MechanicalVentilator-InspiratoryTidalVolume(mL)": 714,
                "MechanicalVentilator-ExpiratoryTidalVolume(mL)": 635,
                "MechanicalVentilator-LeakFraction": 0.15
            }
        }
    }