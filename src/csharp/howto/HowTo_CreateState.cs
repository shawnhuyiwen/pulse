/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using Pulse;
using Pulse.CDM;

namespace HowTo_CreateState
{
  class Example
  {
    public static void Run()
    {
      PulseEngine pulse = new PulseEngine();
      pulse.LogToConsole(true);
      SEPatientConfiguration cfg = new SEPatientConfiguration();
      // Grab the patient and fill in some data
      SEPatient patient = cfg.GetPatient();
      patient.SetName("Bengey");
      patient.SetSex(ePatient_Sex.Male);
      patient.GetAge().SetValue(30, TimeUnit.yr);
      patient.GetWeight().SetValue(200, MassUnit.lb);
      patient.GetHeight().SetValue(74, LengthUnit.inch);
      patient.GetBodyFatFraction().SetValue(0.16);
      patient.GetSystolicArterialPressureBaseline().SetValue(120, PressureUnit.mmHg);
      patient.GetDiastolicArterialPressureBaseline().SetValue(72, PressureUnit.mmHg);
      patient.GetHeartRateBaseline().SetValue(67, FrequencyUnit.Per_min);
      patient.GetRespirationRateBaseline().SetValue(12, FrequencyUnit.Per_min);

      if (!pulse.InitializeEngine(cfg, new SEDataRequestManager()))
      {
        Console.WriteLine("Error Initializing Pulse!");
        return;
      }
      pulse.SerializeToFile("./test_results/howto/HowTo_CreatedState.json");
    }
  }
}
