/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using System.IO;
using Pulse;
using Pulse.CDM;

namespace HowTo_UseEngine
{
  public class MyEventHandler : IEventHandler
  {
    public void HandleEvent(SEEventChange change)
    {
      if (change.Event == eEvent.HypovolemicShock && change.Active)
        Console.WriteLine(change.ToString());
    }
  }

  public class MyLogListener : ILogListener
  {
    public void Debug(string msg) { Console.WriteLine("[DEBUG] " + msg); }
    public void Info(string msg) { Console.WriteLine("[INFO] " + msg); }
    public void Warning(string msg) { Console.WriteLine("[WARN] " + msg); }
    public void Error(string msg) { Console.WriteLine("[ERROR] " + msg); }
    public void Fatal(string msg) { Console.WriteLine("[FATAL] " + msg); }
  }

  class Example
  {
    protected enum InitializationType { PatientObject, PatientFile, StateFileName, StateString };

    public static void Run()
    {
      // Here is an array to store Event Changes
      List<SEEventChange> event_changes = new List<SEEventChange>();
      // Here is an array to store Active Events
      List<SEActiveEvent> active_events = new List<SEActiveEvent>();
      // Create a list of Data Requests to specify all the data we want from Pulse
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
      // In addition to getting this data back via this API
      // You can have Pulse write the data you have requested to a CSV file
      data_mgr.SetResultsFilename("./test_results/HowTo_EngineUse.cs.csv");
      // Create a reference to a double[] that will contain the data returned from Pulse
      double[] data_values;
      // data_values[0] is ALWAYS the simulation time in seconds
      // The rest of the data values are in order of the data_requests list provided

      // Instantiate a Pulse engine
      PulseEngine pulse = new PulseEngine();

      // You can ask Pulse to write out a log file if you want
      // By default, no log file is written
      pulse.SetLogFilename("./test_results/HowTo_EngineUse.cs.log");
      // You can also have pulse log to the console (std::cout)
      // By default, console logging is turned off
      //pulse.LogToConsole(true);
      // It is recommended to listen and  manage the log messages yourself
      // With a listener, you can add messages to a log, display, or write them to console
      pulse.SetLogListener(new MyLogListener());
      // Listen for events, this is optional
      // You must provide an event listener to get events
      pulse.SetEventHandler(new MyEventHandler());

      InitializationType initType = InitializationType.StateFileName;
      switch (initType)
      {
        case InitializationType.StateFileName:
          {
            // Load a state file
            if (!pulse.SerializeFromFile("./states/Soldier@0s.json", data_mgr, SerializationFormat.JSON))
            {
              Console.WriteLine("Error Initializing Pulse!");
              return;
            }
            break;
          }
        case InitializationType.StateString:
          {
            string file_content = File.ReadAllText("./states/Soldier@0s.json");
            if (!pulse.SerializeFromString(file_content, data_mgr, SerializationFormat.JSON))
            {
              Console.WriteLine("Error Initializing Pulse!");
              return;
            }
            break;
          }
        // Both of these cases will use the SEPatientConfiguration object
        case InitializationType.PatientFile:
          {
            // If you have a patient file, simply load up the file
            SEPatientConfiguration cfg = new SEPatientConfiguration();
            // If we want to specify a file on disk that the C++ has access to, we can specify that file
            // (Maybe the Pulse engine is on hosted on another machine)
            //cfg.SetPatientFile("./patients/Soldier.json");
            // Or, ou can load up a local file on disk
            cfg.GetPatient().SerializeFromFile("./patients/Soldier.json", SerializationFormat.JSON);
            // Optionally, you can add conditions to the patient
            cfg.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetSeverity().SetValue(0.2);
            cfg.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetLeftLungAffected().SetValue(1.0);
            cfg.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetRightLungAffected().SetValue(1.0);
            // Initialization requires that you have all data files on disk for Pulse to find when creating a patient
            //   - Patient file (if not creating an inline patient definition)
            //   - Substance, Stabilization (config folder), nutrition, environments, ecg files
            // The data_dir field can be changed in the InitializeEngine method
            // If you have those folders in another location
            if (!pulse.InitializeEngine(cfg, data_mgr))
            {
              Console.WriteLine("Error Initializing Pulse!");
              return;
            }
            // This takes a little while for the engine to find the patient homeostatic/stable state
            break;
          }
        case InitializationType.PatientObject:
          {
            SEPatientConfiguration cfg = new SEPatientConfiguration();
            // Grab the patient and fill in some data
            SEPatient patient = cfg.GetPatient();
            patient.SetName("Owen");
            patient.SetSex(SEPatient.eSex.Male);
            patient.GetAge().SetValue(30, TimeUnit.yr);
            patient.GetWeight().SetValue(200, MassUnit.lb);
            patient.GetHeight().SetValue(74, LengthUnit.inch);
            patient.GetBodyFatFraction().SetValue(0.16);
            patient.GetSystolicArterialPressureBaseline().SetValue(120, PressureUnit.mmHg);
            patient.GetDiastolicArterialPressureBaseline().SetValue(72, PressureUnit.mmHg);
            patient.GetHeartRateBaseline().SetValue(67, FrequencyUnit.Per_min);
            patient.GetRespirationRateBaseline().SetValue(12, FrequencyUnit.Per_min);
            // You really only need to set the name
            // All other patient values will be computed based on
            // https://pulse.kitware.com/_patient_methodology.html
            // Optionally, you can add conditions to the patient
            cfg.GetConditions().GetChronicObstructivePulmonaryDisease().GetBronchitisSeverity().SetValue(0.45);
            cfg.GetConditions().GetChronicObstructivePulmonaryDisease().GetEmphysemaSeverity().SetValue(0.20);
            // Initialization requires that you have all data files on disk for Pulse to find when creating a patient
            //   - Patient file (if not creating an inline patient definition)
            //   - Substance, Stabilization (config folder), nutrition, environments, ecg files
            // The data_dir field can be changed in the InitializeEngine method
            // If you have those folders in another location
            if (!pulse.InitializeEngine(cfg, data_mgr))
            {
              Console.WriteLine("Error Initializing Pulse!");
              return;
            }
            // This takes a little while for the engine to find the patient homeostatic/stable state
            break;
          }
      }
      // DOES ANYONE WANT TO BE ABLE TO CHANGE DATA REQUESTS IN THE MIDDLE OF A RUN?
      // NOTE ANY CSV FILE BEING WRITTEN OUT WOULD NOT SUPPORT CHANGING DATA IN THE MIDDLE OF A RUN
      // BUT IF YOU ARE NOT WRITING A CSV OUT, I COULD SEE THIS BEING USEFUL...

      // Now we can start telling the engine what to do
      // All the same concepts apply from the C++ HowTo files, so look there if you want to see more examples

      // Advance time and print out values
      for (int i = 1; i <= 10; i++)
      {
        if (!pulse.AdvanceTime_s(0.65))
          Console.WriteLine("Error Advancing Time!");
        else
        {
          // Pull data from pulse
          data_values = pulse.PullData();
          // And write it out to the console
          data_mgr.ToConsole(data_values);

          // The engine can provide you with all the current
          // active events and how long they have been active for
          // Active events are reevaluated each call to PullActiveEvents
          pulse.PullActiveEvents(active_events);
          foreach (var active_event in active_events)
          {
            Console.WriteLine(active_event.ToString());
          }
        }
      }

      // Let's do something to the patient, you can either send actions over one at a time, or pass in a List<SEAction>
      List<SEAction> actions = new List<SEAction>();

      SEHemorrhage h = new SEHemorrhage();
      h.SetType(SEHemorrhage.eType.External);
      h.SetCompartment("RightLeg");
      h.GetRate().SetValue(50, VolumePerTimeUnit.mL_Per_min);// Change this to 750 if you want to see how engine failures are handled!!
      actions.Add(h);
      if (!pulse.ProcessActions(actions))
      {
        Console.WriteLine("Engine was unable to process requested actions");
        return;
      }
      // Note CDM is not updated after this call, you have to advance some time
      for (int i = 1; i <= 2; i++)
      {
        if (!pulse.AdvanceTime_s(60)) // Simulate one minute
        {
          Console.WriteLine("Engine was unable to stay within modeling parameters with requested actions");
          return;
        }
        // Get the values of the data you requested at this time
        data_values = pulse.PullData();
        // And write it out to the console
        data_mgr.ToConsole(data_values);
      }

      // Stop the hemorrhage
      h.GetRate().SetValue(0, VolumePerTimeUnit.mL_Per_min);
      if (!pulse.ProcessAction(h))
      {
        Console.WriteLine("Engine was unable to process requested actions");
        return;
      }

      for (int i = 1; i <= 1; i++)
      {
        if (!pulse.AdvanceTime_s(60)) // Simulate one minute
        {
          Console.WriteLine("Engine was unable to stay within modeling parameters with requested actions");
          return;
        }
        // Pull data from pulse
        data_values = pulse.PullData();
        // And write it out to the console
        data_mgr.ToConsole(data_values);
      }

      // Administer Drugs
      SESubstanceInfusion paralytic = new SESubstanceInfusion();
      paralytic.SetSubstance("Succinylcholine");
      paralytic.GetConcentration().SetValue(5000, MassPerVolumeUnit.ug_Per_mL);
      paralytic.GetRate().SetValue(100, VolumePerTimeUnit.mL_Per_min);
      if (!pulse.ProcessAction(paralytic))
      {
        Console.WriteLine("Engine was unable to process requested actions");
        return;
      }
      //Let's use the anesthesia machine to simulate an ambu bag for ventilation
      // We could use the mechanical ventilation action, but you have to provide a changing pressure per squeeze
      // And I just want things automatic for this demo
      // But if you want to interact with a haptic device, use that action with data from your device
      // Or program up a sinusoidal or square wave for applying pressure
      SEAnesthesiaMachineConfiguration am = new SEAnesthesiaMachineConfiguration();
      am.GetConfiguration().SetConnection(SEAnesthesiaMachine.Connection.Tube);
      am.GetConfiguration().GetInletFlow().SetValue(5, VolumePerTimeUnit.L_Per_min);
      am.GetConfiguration().GetInspiratoryExpiratoryRatio().SetValue(0.5);
      am.GetConfiguration().GetOxygenFraction().SetValue(0.23);
      am.GetConfiguration().SetOxygenSource(SEAnesthesiaMachine.OxygenSource.Wall);
      am.GetConfiguration().GetPeakInspiratoryPressure().SetValue(10.5, PressureUnit.cmH2O);
      am.GetConfiguration().GetPositiveEndExpiredPressure().SetValue(1, PressureUnit.cmH2O);
      am.GetConfiguration().SetPrimaryGas(SEAnesthesiaMachine.PrimaryGas.Nitrogen);
      am.GetConfiguration().GetRespiratoryRate().SetValue(16, FrequencyUnit.Per_min);
      if (!pulse.ProcessAction(am))
      {
        Console.WriteLine("Engine was unable to process requested actions");
        return;
      }

      // Infuse some fluids
      SESubstanceCompoundInfusion ivFluids = new SESubstanceCompoundInfusion();
      ivFluids.SetSubstanceCompound("Saline");
      ivFluids.GetBagVolume().SetValue(500, VolumeUnit.mL);
      ivFluids.GetRate().SetValue(100, VolumePerTimeUnit.mL_Per_min);
      if (!pulse.ProcessAction(ivFluids))
      {
        Console.WriteLine("Engine was unable to process requested actions");
        return;
      }

      for (int i = 1; i <= 5; i++)
      {
        if (!pulse.AdvanceTime_s(60)) // Simulate one minute
        {
          Console.WriteLine("Engine was unable to stay within modeling parameters with requested actions");
          return;
        }
        // Pull data from pulse
        data_values = pulse.PullData();
        // And write it out to the console
        data_mgr.ToConsole(data_values);
      }
    }
  }
}
