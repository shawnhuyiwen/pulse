/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using System.IO;
using Pulse;
using Pulse.CDM;

namespace HowTo_DeathState
{
  // Identifying patient death is highly contextual
  // Depending on what has happened to the patient, you will identify death differently
  // In this example, we demonstrate several possible
  // death checks for the 3 main causes of death on the battle field
  // Hemorrhage, Pneumothorax, Airway Obstruction
  // - Always check for irreversable state
  // - Cardiovascular Collapse
  //    Note, Cardiovascular Collapse is recoverable in Cardiac Arrest, but it not when hemorrhaging
  // - Patient HR reaches the patient maximum
  // - Patient has brain oxygen deficit for an extended amount of time
  // - Patient has myocardium oxygen deficit for an extended amount of time
  // - Systolic Blood Pressure is under 60mmHg
  // - SpO2 is under 85 for 3 min
  // Note this is an example, times/events may vary depending on scenario and your SME

  class DeathCheck : IEventHandler
  {
    public DeathCheck(PulseEngine pulse, Logger log =null)
    {
      this.log = log;
      SEPatient initial_patient = new SEPatient();
      pulse.GetInitialPatient(initial_patient);
      max_hr_bpm = initial_patient.GetHeartRateMaximum().GetValue(FrequencyUnit.Per_min);
    }
    public void HandleEvent(SEEventChange change)
    {
      if(change.Event != eEvent.StartOfCardiacCycle && change.Event != eEvent.StartOfExhale && change.Event != eEvent.StartOfInhale)
        log.WriteLine(change.Event + " is " + change.Active + " at time " + change.SimTime.GetValue(TimeUnit.s) + "s\n");

      if (change.Event == eEvent.IrreversibleState)
      {
        irreversable_state = change.Active;
      }

      if (change.Event == eEvent.CardiovascularCollapse)
      {
        cardiovascular_collapse = change.Active;
      }

      if (change.Event == eEvent.BrainOxygenDeficit)
      {
        if (change.Active)
        {
          if (!brain_o2_deficit)
          {
            brain_o2_deficit = true;
            start_brain_o2_deficit_s = change.SimTime.GetValue(TimeUnit.s);
          }
        }
        else
        {
          brain_o2_deficit = false;
          start_brain_o2_deficit_s = 0;
        }
      }

      if (change.Event == eEvent.MyocardiumOxygenDeficit)
      {
        if (change.Active)
        {
          if (!myocardium_o2_deficit)
          {
            myocardium_o2_deficit = true;
            start_myocardium_o2_deficit_s = change.SimTime.GetValue(TimeUnit.s);
          }
        }
        else
        {
          myocardium_o2_deficit = false;
          start_myocardium_o2_deficit_s = 0;
        }
      }
    }
    private Logger log = null;
    private bool irreversable_state = false;
    private bool cardiovascular_collapse = false;
    private double max_hr_bpm;

    private bool brain_o2_deficit = false;
    private double start_brain_o2_deficit_s = 0;

    private bool myocardium_o2_deficit = false;
    private double start_myocardium_o2_deficit_s = 0;

    private bool spO2_deficit = false;
    private double start_spO2_deficit_s = 0;
    

    public bool IsDead(double[] data_requests)
    {
      if (irreversable_state)
      {
        if (log != null)
          log.WriteLine("Patient died from irreversable state");
        return true;
      }
      if (cardiovascular_collapse)
      {
        if (log != null)
          log.WriteLine("Patient died from cardiovascular collapse");
        return true;
      }
      if (data_requests[1] >= max_hr_bpm)// Match your data request order!
      {
        if (log != null)
          log.WriteLine("Patient died from reaching max hr of "+ max_hr_bpm);
        return true;
      }
      if (brain_o2_deficit && (data_requests[0] - start_brain_o2_deficit_s) > 180)
      {
        if (log != null)
          log.WriteLine("Patient died from brain O2 deficit of 180s");
        return true;
      }
      if (myocardium_o2_deficit && (data_requests[0] - start_myocardium_o2_deficit_s) > 180)
      {
        if (log != null)
          log.WriteLine("Patient died from myocardium O2 deficit of 180s");
        return true;
      }

      if (data_requests[6] <= 0.85) // SpO2 Check
      {
        if (!spO2_deficit)
        {
          spO2_deficit = true;
          start_spO2_deficit_s = data_requests[0];
        }
        if ((data_requests[0] - start_spO2_deficit_s) > 140)
        {
          if (log != null)
            log.WriteLine("Patient died from SpO2 < 85 for 140s");
          return true;
        }
      }
      else
        spO2_deficit = false;
      
      if (data_requests[4] < 60)// Systolic Blood Pressure (mmHg) Check
      {
        if (log != null)
          log.WriteLine("Patient died from a SBP < 60");
        return true;
      }

      return false;
    }
  }

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
        // Hemorrhage Related Data Requests
        SEDataRequest.CreatePhysiologyRequest("TotalHemorrhageRate", "mL/s"),
        SEDataRequest.CreatePhysiologyRequest("TotalHemorrhagedVolume", "mL"),
        SEDataRequest.CreateActionCompartmentDataRequest("Hemorrhage", "VenaCava","FlowRate", "mL/s"),
        SEDataRequest.CreateActionCompartmentDataRequest("Hemorrhage", "VenaCava","TotalBloodLost", "mL"),
        SEDataRequest.CreateActionCompartmentDataRequest("Hemorrhage", "RightArmVasculature","FlowRate", "mL/s"),
        SEDataRequest.CreateActionCompartmentDataRequest("Hemorrhage", "RightArmVasculature","TotalBloodLost", "mL"),
        SEDataRequest.CreateActionCompartmentDataRequest("Hemorrhage", "RightLegVasculature","FlowRate", "mL/s"),
        SEDataRequest.CreateActionCompartmentDataRequest("Hemorrhage", "RightLegVasculature","TotalBloodLost", "mL"),
      };
      SEDataRequestManager data_mgr = new SEDataRequestManager(data_requests);
      // Create a reference to a double[] that will contain the data returned from Pulse
      double[] data_values;
      // data_values[0] is ALWAYS the simulation time in seconds
      // The rest of the data values are in order of the data_requests list provided

      // NOTE: No data requests are being provided, so Pulse will return the default vitals data
      if (!pulse.SerializeFromFile("./states/Soldier@0s.pbb", data_mgr))
      {
        Console.WriteLine("Error Initializing Pulse!");
        return;
      }
      DeathCheck death_check = new DeathCheck(pulse);
      pulse.SetEventHandler(death_check);

      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);

      // Let's do something to the patient, you can either send actions over one at a time, or pass in a List<SEAction>
      SEHemorrhage rightLeg = new SEHemorrhage();
      // Setting up a realistic hemorrhage can be difficult
      // Here is an example of how the engine will act if you create an unrealistic hemorrhage
      rightLeg.SetType(SEHemorrhage.eType.External);
      rightLeg.SetCompartment("RightLegVasculature");
      rightLeg.GetSeverity().SetValue(0.8);
      pulse.ProcessAction(rightLeg);

      SEHemorrhage rightArm = new SEHemorrhage();
      rightArm.SetType(SEHemorrhage.eType.External);
      rightArm.SetCompartment("RightArmVasculature");
      rightArm.GetSeverity().SetValue(0.9);
      pulse.ProcessAction(rightArm);

      SEHemorrhage venaCava = new SEHemorrhage();
      venaCava.SetType(SEHemorrhage.eType.External);
      venaCava.SetCompartment("VenaCava");
      venaCava.GetSeverity().SetValue(0.1);
      pulse.ProcessAction(venaCava);

      int time_to_run = 1200 * 50;
      int status = 60 * 50;
      // seconds * Hz = total seconds to run, status time for print

      // Run pulse in a loop, and test for death after each time step
      for(int i=0; i< time_to_run; i++)
      {
        pulse.AdvanceTimeStep();
        // Get the values of the data you requested at this time
        data_values = pulse.PullData();
        if(death_check.IsDead(data_values))
        {
          System.Console.Out.WriteLine("Patient has died");
          break;
        }
        if( (i % status) == 0)
          data_mgr.WriteData(data_values);
      }
    }
  }
}
