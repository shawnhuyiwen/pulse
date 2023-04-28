/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using Pulse;
using Pulse.CDM;

namespace HowTo_Hemothorax
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
        // Vitals Monitor Data
        SEDataRequest.CreatePhysiologyDataRequest("HeartRate", FrequencyUnit.Per_min),
        SEDataRequest.CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit.mmHg),
        SEDataRequest.CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit.mmHg),
        SEDataRequest.CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit.mmHg),
        SEDataRequest.CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit.Per_min),
        SEDataRequest.CreatePhysiologyDataRequest("TidalVolume", VolumeUnit.mL),
        SEDataRequest.CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit.mL),
        SEDataRequest.CreatePhysiologyDataRequest("OxygenSaturation"),
        SEDataRequest.CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit.mL_Per_min),
        SEDataRequest.CreatePhysiologyDataRequest("BloodVolume", VolumeUnit.mL),
        SEDataRequest.CreateActionDataRequest("LeftHemothorax", "FlowRate", VolumePerTimeUnit.mL_Per_min),
        SEDataRequest.CreateActionDataRequest("LeftHemothorax", "TotalBloodVolume", VolumeUnit.mL)
      };
      SEDataRequestManager data_mgr = new SEDataRequestManager(data_requests);
      data_mgr.SetResultsFilename("./test_results/howto/HowToHemothorax.cs.csv");
      // Create a reference to a double[] that will contain the data returned from Pulse
      double[] data_values;
      // data_values[0] is ALWAYS the simulation time in seconds
      // The rest of the data values are in order of the data_requests list provided

      if (!pulse.SerializeFromFile("./states/StandardMale@0s.json", data_mgr))
      {
        Console.WriteLine("Error Initializing Pulse!");
        return;
      }
      pulse.AdvanceTime_s(50);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);

      // Create a Hemothorax
      // Set the severity (a fraction between 0 and 1)
      SEHemothorax hemo = new SEHemothorax();
      hemo.GetSeverity().SetValue(0.75);
      // Optionally, You can set the flow rate of the hemothorax.
      // This is implemented as a flow source, this rate will be constant, and will not be affected by dropping blood pressures
      // It is intended to interact with sensors or with something continuously monitoring physiology and updating the flow
      //hemo.GetFlowRate().SetValue(20, VolumePerTimeUnit.mL_Per_min); //the flow rate of hemothorax
      // When using using a flow rate, you can also optionally set a target volume for the hemothorax
      //hemo.GetTargetVolume().SetValue(600, VolumeUnit.mL); //the target volume of the hemothorax

      // It can be on the left or right side
      hemo.SetSide(eSide.Left);
      //hemo.SetSide(eSide.Right);
      pulse.ProcessAction(hemo);

      pulse.AdvanceTime_s(120);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);

      // Tube thoracostomy should help the patient out
      SETubeThoracostomy tubeThoracostomy = new SETubeThoracostomy();
      // Optionally, you can set the flow rate of the tube thoracostomy.
      //tubeThoracostomy.GetFlowRate().SetValue(15, VolumePerTimeUnit.mL_Per_min);

      // You can set the flow rate to 0 when you would like to remove the intervention
      tubeThoracostomy.GetFlowRate().SetValue(0, VolumePerTimeUnit.mL_Per_min);

      // It can be on the left or right side (it's a good idea to do it on the side of the hemothorax ;)
      tubeThoracostomy.SetSide(eSide.Left);
      //tubeThoracostomy.SetSide(eSide.Right);

      tubeThoracostomy.GetFlowRate().SetValue(15, VolumePerTimeUnit.mL_Per_min);
      pulse.ProcessAction(tubeThoracostomy);
      Console.WriteLine("Giving the patient a tube thoracostomy");

      pulse.AdvanceTime_s(400);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
    }
  }
}
