/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using Pulse;
using Pulse.CDM;

namespace HowTo_DeathState
{
  // Identifying patient death is highly contextual
  // Depending on what has happened to the patient, you will identify death differently
  // In this example we are looking to identify death due to hemorrhage
  // The contextual aspect here, is Cardiovascular Collapse due to bleed out
  // We cannot generically test for Cardiovascular Collapse, as this event will be
  // thrown by an engine where a patient is in Cardiac Arrest.
  // Cardiovascular Collapse is recoverable in Cardiac Arrest, but it not when hemorrhaging

  class HemorrhageDeath : IEventHandler
  {
    public void HandleEvent(SEEventChange change)
    {
      if (change.Event == eEvent.CardiovascularCollapse && change.Active)
      {
        isDead = true;
        Console.WriteLine("Patient has died due to Cardiovascular Collapse");
      }
    }
    private bool isDead;
    public bool IsDead { get { return this.isDead; } }
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
      pulse.SetEventHandler(new HemorrhageDeath());

      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.ToConsole(data_values);

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

      pulse.AdvanceTime_s(30);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.ToConsole(data_values);

      // If the hemorrhage is very bad, a Hypovolemic Shock event will be thrown
      // Eventually Cardiovascular Collapse will be triggered, then you need to shut the engine down.
      // This collapse should be interpreted as death for a bleeding patient.
      // If you let it bleed after that, you will eventually get an irreversible state
      // advance time will then return false, and the engine will no longer do anything on subsequent advance time calls and only return false
      // If the provided hemorrhage is very bad, you may skip cardiovascular collapse and go straight to irreversable state...

      // Advance lots of time unitl the patient dies
      pulse.AdvanceTime_s(300);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.ToConsole(data_values);
    }
  }
}
