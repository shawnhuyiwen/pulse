/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using Pulse;
using Pulse.CDM;

namespace HowTo_MechanicalVentilator
{
  class Example
  {
    public static void Run()
    {
      // Create our pulse engine
      PulseEngine pulse = new PulseEngine();
      pulse.LogToConsole(true);// Easily view what is happening

      List<SEDataRequest> data_requests = new List<SEDataRequest>
      {
        SEDataRequest.CreatePhysiologyRequest("HeartRate", "1/min"),
        SEDataRequest.CreatePhysiologyRequest("ArterialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("MeanArterialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("SystolicArterialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("DiastolicArterialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("OxygenSaturation"),
        SEDataRequest.CreatePhysiologyRequest("EndTidalCarbonDioxidePressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("RespirationRate", "1/min"),
        SEDataRequest.CreatePhysiologyRequest("SkinTemperature", "degC"),
        SEDataRequest.CreateGasCompartmentSubstanceRequest("Carina", "CarbonDioxide", "PartialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("BloodVolume", "mL"),
        SEDataRequest.CreateECGRequest("Lead3ElectricPotential", "mV"),
      };
      SEDataRequestManager data_mgr = new SEDataRequestManager(data_requests);
      // Create a reference to a double[] that will contain the data returned from Pulse
      double[] data_values;
      // data_values[0] is ALWAYS the simulation time in seconds
      // The rest of the data values are in order of the data_requests list provided

      if (!pulse.SerializeFromFile("./states/Soldier@0s.json", data_mgr, SerializationFormat.JSON))
      {
        Console.WriteLine("Error Initializing Pulse!");
        return;
      }

      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.ToConsole(data_values);

      SEMechanicalVentilatorConfiguration mv_config = new SEMechanicalVentilatorConfiguration();
      SEMechanicalVentilator mv = mv_config.GetConfiguration();
      mv.SetConnection(SEMechanicalVentilator.Connection.Mask);
      mv.SetInspirationWaveform(SEMechanicalVentilator.DriverWaveform.Square);
      mv.SetExpirationWaveform(SEMechanicalVentilator.DriverWaveform.Square);
      mv.GetPeakInspiratoryPressure().SetValue(21.0, PressureUnit.cmH2O);
      mv.GetPositiveEndExpiredPressure().SetValue(10.0, PressureUnit.cmH2O);
      SESubstanceFraction fractionFiO2 = mv.GetFractionInspiredGas("Oxygen");
      fractionFiO2.GetFractionAmount().SetValue(0.5);
      double respirationRate_per_min = 20.0;
      double IERatio = 0.5;

      // Translate ventilator settings
      double totalPeriod_s = 60.0 / respirationRate_per_min;
      double inspiratoryPeriod_s = IERatio * totalPeriod_s / (1 + IERatio);
      double expiratoryPeriod_s = totalPeriod_s - inspiratoryPeriod_s;
      mv.GetInspirationTriggerTime().SetValue(expiratoryPeriod_s, TimeUnit.s);
      mv.GetExpirationCycleTime().SetValue(inspiratoryPeriod_s, TimeUnit.s);
      pulse.ProcessAction(mv_config);

      // Advance some time and print out the vitals
      pulse.AdvanceTime_s(30);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.ToConsole(data_values);
    }
  }
}
