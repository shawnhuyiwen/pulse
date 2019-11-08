using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using Pulse;

namespace HowTo_UseC
{
  class Program
  {
    protected enum InitializationType { PatientObject, PatientFile, StateFileName, StateString };

    static void Main(string[] args)
    {
      // We are not providing data requests, so this will default to vitals data
      List<string> headings = new List<string>();
      headings.Add("SimTime(s)");
      headings.Add("Lead3ElectricPotential(mV)");
      headings.Add("HeartRate(1 / min)");
      headings.Add("ArterialPressure(mmHg)");
      headings.Add("MeanArterialPressure(mmHg)");
      headings.Add("SystolicArterialPressure(mmHg)");
      headings.Add("DiastolicArterialPressure(mmHg)");
      headings.Add("OxygenSaturation");
      headings.Add("EndTidalCarbonDioxidePressure(mmHg)");
      headings.Add("RespirationRate(1 / min)");
      headings.Add("SkinTemperature(degC)");
      headings.Add("Carina-CarbonDioxide-PartialPressure(mmHg)");
      headings.Add("BloodVolume(mL)");
      double[] result = new double[headings.Count];

      // Here is an array of Event Changes
      List<SEEventChange> event_changes = new List<SEEventChange>();
      // Here is an array of Active Events
      List<SEActiveEvent> active_events = new List<SEActiveEvent>();

      // Instatiate a Pulse engine
      PulseEngine pulse = new PulseEngine("pulse.log", ".");

      InitializationType initType = InitializationType.StateFileName;
      // INITIALIZE THE ENGINE WITH A PATIENT
      switch (initType)
      {
        case InitializationType.StateFileName:
          {
            // Load a state file
            if (!pulse.SerializeFromFile("./states/Soldier@0s.json", null, SerializationFormat.JSON, 0))
              Console.WriteLine("Error Initializing Pulse!");
            break;
          }
        case InitializationType.StateString:
          {
            string file_content = File.ReadAllText("./states/Soldier@0s.json");
            if (!pulse.SerializeFromString(file_content, null, SerializationFormat.JSON, 0))
              Console.WriteLine("Error Initializing Pulse!");
            break;
          }
      }

      // Now we can start telling the engine what to do
      // All the same concepts apply from the C++ HowTo files, so look there if you want to see more examples

      // Advance time and print out values
      for (int i = 1; i <= 10; i++)
      {
        if (!pulse.AdvanceTime_s(1))
          Console.WriteLine("Error Advancing Time!");
        else
        {
          // Pull data from pulse
          IntPtr data = pulse.PullData();
          Marshal.Copy(data, result, 0, headings.Count);
          for (int d = 0; d < headings.Count; d++)
            Console.WriteLine(headings[d] + " " + result[d]);
          // Did any events occur during from the last time we called PullEvents?
          pulse.PullEvents(event_changes);
          // Iterate through the events to look for things you are interested in
          // Use PullEvents when you are advancing Pulse at/near single time step at a time
          // If you have large time steps, you will essentially get a history of event changes
          // Which may or may not be considered 'stale'
          // You will also get a lot of StartOfCardiacCycle and StartOfInhale/Exhale events
          // The provided event_changes list will be cleared in PullEvents
          // event_changes will be populated with any changes from the last PullEvents,
          // So if you want to have a full event history between advancements
          // You will need to keep that history in your own List.
          foreach (var change in event_changes)
          {
            Console.WriteLine(change.ToString());
          }
          pulse.PullActiveEvents(active_events);
          // The engine will provide you with all the current
          // Active events and how long they have been active for
          // Active events are reevaluated each call to PullActiveEvents
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
        IntPtr data = pulse.PullData();
        Marshal.Copy(data, result, 0, headings.Count);
        for (int d = 0; d < headings.Count; d++)
          Console.WriteLine(headings[d] + " " + result[d]);
        // Check for event activity
        pulse.PullEvents(event_changes);
        foreach (var change in event_changes)
          Console.WriteLine(change.ToString());
        pulse.PullActiveEvents(active_events);
        foreach (var active_event in active_events)
          Console.WriteLine(active_event.ToString());
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
        IntPtr data = pulse.PullData();
        Marshal.Copy(data, result, 0, headings.Count);
        for (int d = 0; d < headings.Count; d++)
          Console.WriteLine(headings[d] + " " + result[d]);
        // Check for event activity
        pulse.PullEvents(event_changes);
        foreach (var change in event_changes)
          Console.WriteLine(change.ToString());
        pulse.PullActiveEvents(active_events);
        foreach (var active_event in active_events)
          Console.WriteLine(active_event.ToString());
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
      am.GetConfiguration().GetPositiveEndExpiredPressure().SetValue(1, PressureUnit.cmH2O);
      am.GetConfiguration().SetPrimaryGas(SEAnesthesiaMachine.PrimaryGas.Nitrogen);
      am.GetConfiguration().GetRespiratoryRate().SetValue(16, FrequencyUnit.Per_min);
      am.GetConfiguration().GetVentilatorPressure().SetValue(10.5, PressureUnit.cmH2O);
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
        IntPtr data = pulse.PullData();
        Marshal.Copy(data, result, 0, headings.Count);
        for (int d = 0; d < headings.Count; d++)
          Console.WriteLine(headings[d] + " " + result[d]);
        // Check for event activity
        pulse.PullEvents(event_changes);
        foreach (var change in event_changes)
          Console.WriteLine(change.ToString());
        pulse.PullActiveEvents(active_events);
        foreach (var active_event in active_events)
          Console.WriteLine(active_event.ToString());
      }
    }
  }
}
