/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.pulse.engine;

import java.util.*;

import com.google.protobuf.TextFormat.ParseException;
import com.kitware.physiology.cdm.AnesthesiaMachine.AnesthesiaMachineData;
import com.kitware.physiology.cdm.AnesthesiaMachineEnums.eAnesthesiaMachine;
import com.kitware.physiology.cdm.Patient.PatientData;
import com.kitware.physiology.cdm.PatientEnums.ePatient;
import com.kitware.physiology.cdm.ScenarioEnums.eDataRequest.*;

import com.kitware.physiology.datamodel.conditions.SECondition;
import com.kitware.physiology.datamodel.datarequests.SEDataRequest;
import com.kitware.physiology.datamodel.datarequests.SEDataRequestManager;
import com.kitware.physiology.datamodel.patient.SEPatient;
import com.kitware.physiology.datamodel.patient.actions.SEHemorrhage;
import com.kitware.physiology.datamodel.patient.actions.SESubstanceCompoundInfusion;
import com.kitware.physiology.datamodel.patient.actions.SESubstanceInfusion;
import com.kitware.physiology.datamodel.patient.assessments.SECompleteBloodCount;
import com.kitware.physiology.datamodel.patient.conditions.SEChronicAnemia;
import com.kitware.physiology.datamodel.properties.CommonUnits.FrequencyUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.LengthUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.MassUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.PressureUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.TimeUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.VolumePerTimeUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.VolumeUnit;
import com.kitware.physiology.datamodel.properties.SEScalarTime;
import com.kitware.physiology.datamodel.substance.SESubstanceCompound;
import com.kitware.physiology.datamodel.utilities.SEEventHandler;
import com.kitware.physiology.utilities.Log;
import com.kitware.physiology.utilities.LogListener;
import com.kitware.physiology.utilities.jniBridge;

/**
 * How to use the Java interface to the Pulse Physiology Engine
 */
public class HowTo_EngineUse
{
//Create a listener that will catch any messages logged in C++
 // This class will take the messages and add them to the log created in Java
 protected static class MyListener extends LogListener
 {
   protected MyListener()
   {
     super();
     // Since we are just pushing this data into the log
     // We set listen to false, when you create a LogListener
     // is will automatically register itself with the static Log class,
     // and when ever a Log even class is called, this will be called
     // and since this just calls the Log to log you will get into a recursive infinite loop.
     // This is just because I use the LogListener interface to listen to any log messages coming
     // from C++. Technically it is a LogListener as it is 'listening' to log events in C++.
     // At any rate the Java side will receive log events from C++, and you create your own 
     // LogListener so you can do as you see fit with those messages. BUT if you do want to 
     // push those messages to the Java Log, you will need this class to not listen so you
     // don't get into an infinite recursive loop
     listen(false);
   }    
   public void handleDebug(String msg) { Log.debug(msg); }
   public void handleInfo(String msg)  { Log.info(msg); }
   public void handleWarn(String msg)  { Log.warn(msg); }
   public void handleError(String msg) { Log.error(msg); }
   public void handleFatal(String msg) { Log.fatal(msg); }
 }
 
 protected static class MyEventHandler implements SEEventHandler
 {
  public void handlePatientEvent(ePatient.Event type, boolean active, SEScalarTime time)
  {
    // Here is how you test against an event of interest and do something
    //if(type == ePatient.Event.StartOfCardiacCycle && active)
      //Log.info("Patient started a new heart beat at time "+time);    
  }

  public void handleAnesthesiaMachineEvent(eAnesthesiaMachine.Event type, boolean active, SEScalarTime time)
  {
    
  } 
 }
 
 public enum InitializationType { PatientObject, PatientFile, StateFile };
 
 public static void main(String[] args)
 {
   // Initialize the JNI bridge between Java and C++
   // While not required (yet), it's recommended for program completeness
   jniBridge.initialize();
   example();
   // Shutdown all C++ classes
   // Since Pulse supports running multiple engines within the same process
   // (Each could be on its own thread as well)
   // deinitialize the C++ so it can properly clean up resources
   // Generally this is not needed, but I did find it necessary on windows using mingw
   // If not, the JVM would find itself in a threadlock condition due to threadpool deconstruction
   // So if your Java app using Pulse has shutdown issues, use this method at your apps end of life
   jniBridge.deinitialize();
 }
 
 public static void example()
 {
   SEScalarTime time = new SEScalarTime(0, TimeUnit.s);
   
   // Create a log file for this example (by default, a Pulse.log will be made)
   // NOTE the engine will have its own log, so there is a Java log and an Engine log!!
   // You don't have to have a Java log, but if you want to, this is how you can do it.
   Log.setFileName("HowTo_DynamicEngineDriver.log");
   // Note there is a single singleton logger in Java, but a log per engine in C++
   // i.e. in Java, multiple engines will write to the same log, where as in C++, each engine will write to its own log
   // The listener and callback objects are unique to this engine
   
   // Create a Pulse Engine
   PulseEngine pe = new PulseEngine();// The engine log will be named based the provided string + ".log" 
   
   // I am going to create a listener that will get any log messages (Info, Warnings, Errors, Fatal Errors)
   // that come from the engine. The default listener will just put them into the log file
   // If you want to do custom logic that occurs when the engine throws an error (or any other message type), just create a class just like this one
   pe.setListener(new MyListener());
   
   // I want to know when ever the patient and anesthesia machine(if used) enters and exits a particular state
   pe.setEventHandler(new MyEventHandler());
   
   // Here are the data I want back from the engine
   // The CDM objects on the pe object will be updated 
   // at the end of AdvanceTime calls (which are blocking)
   // No other data values will have data in Java classes
   SEDataRequestManager dataRequests = new SEDataRequestManager();
   SEDataRequest hr = new SEDataRequest();
   hr.setCategory(Category.Physiology);
   hr.setPropertyName("HeartRate");
   hr.setUnit(FrequencyUnit.Per_min.toString());
   dataRequests.getRequestedData().add(hr);
   SEDataRequest rr = new SEDataRequest();
   rr.setCategory(Category.Physiology);
   rr.setPropertyName("RespirationRate");
   rr.setUnit(FrequencyUnit.Per_min.toString());
   dataRequests.getRequestedData().add(rr);
   SEDataRequest tlv = new SEDataRequest(); 
   tlv.setCategory(Category.Physiology);   
   tlv.setPropertyName("TotalLungVolume");
   tlv.setUnit(VolumeUnit.mL.toString());
   dataRequests.getRequestedData().add(tlv);
   SEDataRequest bv = new SEDataRequest();  
   bv.setCategory(Category.Physiology);  
   bv.setPropertyName("BloodVolume");
   bv.setUnit(VolumeUnit.mL.toString());
   dataRequests.getRequestedData().add(bv);
   
   ////////////////////////////////////////////////////
   // NOTE All serialization is assumed to be ASCII  //
   // Binary is available, but the JAVA API does not //
   // currently support it.                          //
   ////////////////////////////////////////////////////
   
   InitializationType initType = InitializationType.StateFile;
   // INITIALIZE THE ENGINE WITH A PATIENT
   switch(initType)
   {
   case PatientObject:
     {
      // Set up your patient
      // This is the minimum and recommended data to set for a patient
      // But you can get more complicated, consult our Patient Variability Documentation for more details
      SEPatient patient = new SEPatient();
      patient.setName("Standard");
      patient.setSex(ePatient.Sex.Male);
      patient.getAge().setValue(44,TimeUnit.yr);
      patient.getWeight().setValue(170,MassUnit.lb);
      patient.getHeight().setValue(71,LengthUnit.in);
      patient.getBodyFatFraction().setValue(0.21);
      patient.getHeartRateBaseline().setValue(72,FrequencyUnit.Per_min);
      patient.getDiastolicArterialPressureBaseline().setValue(72.,PressureUnit.mmHg);
      patient.getSystolicArterialPressureBaseline().setValue(114.,PressureUnit.mmHg);
      patient.getRespirationRateBaseline().setValue(16,FrequencyUnit.Per_min);
      
      // If I wanted to make set a condition on the patient
      // Note that while you can have multiple conditions on a patient
      // It is more than likely not tested and the engine may or may not converge
      List<SECondition> conditions = new ArrayList<SECondition>();
      SEChronicAnemia anemia = new SEChronicAnemia();// One of these days I will prefix these with SE...
      anemia.getReductionFactor().setValue(0.3);
      conditions.add(anemia);
      
      // Allocate an engine
      pe.initializeEngine("./Scenarios/HowToDynamicEngine.log", patient, null/*optionally, pass in our conditions list*/, dataRequests);
       // This method will block while the engine stabilizes to meet the defined patient parameters
       break;
     }
   case PatientFile:
     {
       // Allocate an engine
       pe.initializeEngine("./Scenarios/HowToDynamicEngine.log", "./patient/StandardMale.pba", null/*optionally, pass in a conditions list*/, dataRequests);       
       // This method will block while the engine stabilizes to meet the defined patient parameters
       break;
     }
   case StateFile:
     {
       pe.serializeFromFile("./Scenarios/HowToDynamicEngine.log", "./states/StandardMale@0s.pba", dataRequests);
       // This method method sets the engine to the provided state instantaneously and you are ready to process actions/advance time
       // You can alternatively specify the starting simTime of the engine       
       //pe.loadState("./Scenarios/HowToDynamicEngine.log", "./states/StandardMale@0s.pba", time, dataRequests);
       break;
     }
   }
      
   // Now we can start telling the engine what to do
   // All the same concepts apply from the C++ HowTo files, so look there if you want to see more examples
   
   if(!pe.advanceTime()) // Advance one engine time step
   {
     Log.info("Something bad happened");
     return;
   }
   // The CDM objects contained in pe will be automatically updated after this method returns
   // NOTE ONLY THE CDM PROPERTIES ASSOCIATED WITH THE DATA REQUESTS PROVIDED IN initializeEngine WILL BE UPDATED
   // it would be pretty slow to push EVERYTHING from the engine into Java if you are not using it all.
   // This is just a limitation of the Java interface, let me know if you think we can push everything or have some other idea for pushing..
   // SO this does mean that you have to know up front everything you are going to use from the engine, there may be ways to make it more
   // dynamic it getting data back from the engine, but at this time, I am not going to support that, not that it cannot be done, again let's talk if you want it...
   
   Log.info("Heart Rate " + pe.cardiovascular.getHeartRate());
   Log.info("Respiration Rate " + pe.respiratory.getRespirationRate());
   Log.info("Total Lung Volume " + pe.respiratory.getTotalLungVolume());
   Log.info("Blood Volume " + pe.cardiovascular.getBloodVolume());    
   
   // Let's get an assessment from the engine
   // Assessments can involve extra calculation to generate the data necessary for the specified assessment
   SECompleteBloodCount cbc = new SECompleteBloodCount();
   try
   {
  	 pe.getPatientAssessment(cbc);
   }
   catch(ParseException ex)
   {
  	 Log.error("Failed to get patient assessment",ex);
   }
   Log.info("Red Blood Count "+cbc.getRedBloodCellCount());
   Log.info("White Blood Count "+cbc.getWhiteBloodCellCount());
   
   // You can check if the patient is in a specific state/event
   if(pe.patient.isEventActive(ePatient.Event.CardiacArrest))
     Log.info("CODE BLUE!");
   
   time.setValue(1, TimeUnit.s);
   if(!pe.advanceTime(time)) // Simulate one second
   {
     Log.info("Something bad happened");
     return;
   }
   // Again, the CDM is updated after this call
   
   Log.info("Heart Rate " + pe.cardiovascular.getHeartRate());
   Log.info("Respiration Rate " + pe.respiratory.getRespirationRate());
   Log.info("Total Lung Volume " + pe.respiratory.getTotalLungVolume());   
   Log.info("Blood Volume " + pe.cardiovascular.getBloodVolume());      
   
   // Let's do something to the patient, you can either send actions over one at a time, or pass in a List<SEAction>
   SEHemorrhage h = new SEHemorrhage();
   h.setCompartment(PulseCompartments.Vascular.RightLeg);
   h.getRate().setValue(200,VolumePerTimeUnit.mL_Per_min);// Change this to 750 if you want to see how engine failures are handled!!
   if(!pe.processAction(h))
   {
     Log.error("Engine was unable to process requested actions");
     return;
   }
   // Note CDM is not updated after this call, you have to advance some time

   for(int i=1; i<=2; i++)
   {
  	 time.setValue(i,TimeUnit.min);
	   if(!pe.advanceTime(time)) // Simulate one second
	   {
	     Log.error("Engine was unable to stay within modeling parameters with requested actions");
	     return;
	   }
	   // Again, the CDM is updated after this call
	   Log.info("Heart Rate " + pe.cardiovascular.getHeartRate());
	   Log.info("Respiration Rate " + pe.respiratory.getRespirationRate());
	   Log.info("Total Lung Volume " + pe.respiratory.getTotalLungVolume());     
	   Log.info("Blood Volume " + pe.cardiovascular.getBloodVolume());  
   }
   
   // Stop the hemorrhage
   h.getRate().setValue(0,VolumePerTimeUnit.mL_Per_min);
   if(!pe.processAction(h))
   {
     Log.error("Engine was unable to process requested actions");
     return;
   }
   
   for(int i=1; i<=1; i++)
   {
     time.setValue(i,TimeUnit.min);
     if(!pe.advanceTime(time)) // Simulate one second
     {
       Log.error("Engine was unable to stay within modeling parameters with requested actions");
       return;
     }
     // Again, the CDM is updated after this call
     Log.info("Heart Rate " + pe.cardiovascular.getHeartRate());
     Log.info("Respiration Rate " + pe.respiratory.getRespirationRate());
     Log.info("Total Lung Volume " + pe.respiratory.getTotalLungVolume());     
     Log.info("Blood Volume " + pe.cardiovascular.getBloodVolume());  
   }
   
   // Infuse some fluids
   SESubstanceCompound saline = pe.substanceManager.getCompound("Saline");
   SESubstanceCompoundInfusion ivFluids= new SESubstanceCompoundInfusion(saline);
   ivFluids.getBagVolume().setValue(500,VolumeUnit.mL);
   ivFluids.getRate().setValue(100,VolumePerTimeUnit.mL_Per_min);
   if(!pe.processAction(ivFluids))
   {
     Log.error("Engine was unable to process requested actions");
     return;
   }
   
   for(int i=1; i<=5; i++)
   {
     time.setValue(i,TimeUnit.min);
     if(!pe.advanceTime(time)) // Simulate one second
     {
       Log.error("Engine was unable to stay within modeling parameters with requested actions");
       return;
     }
     // Again, the CDM is updated after this call
     Log.info("Heart Rate " + pe.cardiovascular.getHeartRate());
     Log.info("Respiration Rate " + pe.respiratory.getRespirationRate());
     Log.info("Total Lung Volume " + pe.respiratory.getTotalLungVolume());     
     Log.info("Blood Volume " + pe.cardiovascular.getBloodVolume());  
   }
     
   // Be nice to your memory and deallocate the native memory associated with this engine if you are done with it
   pe.cleanUp();
   // Note you can now run a static (scenario) or another dynamic engine with the pe object, it will allocate and manage a new C++ engine 
 }
}
