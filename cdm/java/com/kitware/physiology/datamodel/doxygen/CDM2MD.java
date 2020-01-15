/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.doxygen;

import java.io.PrintWriter;
import java.lang.reflect.Method;
import java.util.*;
import java.util.stream.Collectors;

import com.kitware.physiology.cdm.AnesthesiaMachine.AnesthesiaMachineData;
import com.kitware.physiology.cdm.Enums.*;
import com.kitware.physiology.cdm.Environment.EnvironmentalConditionsData;
import com.kitware.physiology.cdm.Patient.PatientData;
import com.kitware.physiology.cdm.Patient.PatientData.eSex;
import com.kitware.physiology.cdm.Events.eEvent;
import com.kitware.physiology.cdm.PatientActions.BrainInjuryData;
import com.kitware.physiology.cdm.PatientActions.HemorrhageData;
import com.kitware.physiology.cdm.PatientActions.IntubationData;
import com.kitware.physiology.cdm.PatientActions.SubstanceBolusData;
import com.kitware.physiology.cdm.PatientActions.SupplementalOxygenData;
import com.kitware.physiology.cdm.PatientAssessments.PatientAssessmentData;
import com.kitware.physiology.cdm.PatientAssessments.UrinalysisData.eClarityIndicator;
import com.kitware.physiology.cdm.PatientAssessments.UrinalysisData.ePresenceIndicator;
import com.kitware.physiology.cdm.PatientAssessments.UrinalysisData.eUrineColor;
import com.kitware.physiology.cdm.PatientAssessments.UrinalysisMicroscopicData.eObservationAmount;
import com.kitware.physiology.cdm.PatientAssessments.UrinalysisMicroscopicData.eObservationType;
import com.kitware.physiology.cdm.Physiology.eHeartRhythm;
import com.kitware.physiology.cdm.Substance.SubstanceData;
import com.kitware.physiology.cdm.Substance.SubstancePhysicochemicalsData;
import com.kitware.physiology.cdm.PatientAssessments.ePatientAssessmentType;
import com.kitware.physiology.datamodel.compartment.SECompartment;
import com.kitware.physiology.datamodel.patient.SEPatient;
import com.kitware.physiology.datamodel.patient.actions.SEConsciousRespirationCommand;
import com.kitware.physiology.datamodel.patient.actions.SEPatientAction;
import com.kitware.physiology.datamodel.patient.assessments.SEPatientAssessment;
import com.kitware.physiology.datamodel.patient.conditions.SEPatientCondition;
import com.kitware.physiology.datamodel.properties.SEFunction;
import com.kitware.physiology.datamodel.properties.SEScalar;
import com.kitware.physiology.datamodel.system.environment.actions.SEEnvironmentAction;
import com.kitware.physiology.datamodel.system.environment.conditions.SEEnvironmentCondition;
import com.kitware.physiology.datamodel.system.equipment.anesthesia.actions.SEAnesthesiaMachineAction;
import com.kitware.physiology.datamodel.system.equipment.inhaler.actions.SEInhalerAction;
import com.kitware.physiology.utilities.FileUtils;
import com.kitware.physiology.utilities.FindObjects;
import com.kitware.physiology.utilities.FindObjects.BagMethod;
import com.kitware.physiology.utilities.jniBridge;
import com.kitware.physiology.utilities.Log;
import com.kitware.physiology.utilities.StringUtils;

/**
 * This class will generate markdown tables
 * for some important cdm data structures
 * for use in our documentation
 */
public class CDM2MD
{

  public static void main(String[] args)
  {
    jniBridge.initialize();
    // Table names that link to classes (and we don't want them to)
    tableNameLinks.add("Environment");
    tableNameLinks.add("Inhaler");
    tableNameLinks.add("AnesthesiaMachine");
    convert(args.length> 0 ? args[0] : "./docs/markdown");
    jniBridge.deinitialize();
  }
  
  protected static Set<String> tableNameLinks = new HashSet<String>();
  
  public static void convert(String destDir)
  {
    try
    {
      //FileUtils.delete(destDir);//Caller should delete old contents
      FileUtils.createDirectory(destDir);

      PrintWriter writer=new PrintWriter(destDir+"/CDMTable.md", "UTF-8");

      writer.append("CDM Tables {#CDMTables}\n");
      writer.append("=======================\n");

      List<String> skipProperties = new ArrayList<String>();
      skipProperties.add("Comment");
      skipProperties.add("ScenarioTime");

      // PATIENT
      writer.append("#### The following tables describe a patient for Pulse to simulate\n<hr>\n");
      WriteDoxyTable(SEPatient.class, "", writer, skipProperties);    
      WriteDoxyTable(PatientData.eSex.class, "PatientData_", writer, skipProperties);  
      
      writer.append("#### The following tables describe the physiologic and equipment states Pulse supports.\n<hr>\n");
      WriteDoxyTable(eEvent.class, "", writer, skipProperties);
      
      // PATIENT CONDITIONS
      writer.append("#### The following tables describe the conditions that can be applied to the patient before starting the simulation\n<hr>\n");
      Set<Class<? extends SEPatientCondition>> pConditions = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.patient.conditions", SEPatientCondition.class);
      List<Class<? extends Object>> pConditionsSorted = pConditions.stream().collect(Collectors.toList());
      Collections.sort(pConditionsSorted, (o1, o2) -> o1.getName().compareTo(o2.getName()));
      for(Class<?> c : pConditionsSorted)
        WriteDoxyTable(c, "", writer, skipProperties);
      
      // PHYSIOLOGY
      writer.append("#### The following tables describe the system data that is calculated each time step\n<hr>\n");
      Set<Class<? extends Object>> phys = FindObjects.findAllClasses("com.kitware.physiology.datamodel.system.physiology");
      List<Class<? extends Object>> physSorted = phys.stream().collect(Collectors.toList());
      Collections.sort(physSorted, (o1, o2) -> o1.getName().compareTo(o2.getName()));
      for(Class<?> c : physSorted)
        WriteDoxyTable(c, "", writer, skipProperties);
      WriteDoxyTable(eHeartRhythm.class, "", writer, skipProperties);
      
      // PATIENT ACTIONS/CONDITIONS/ASSESSMENTS
      writer.append("#### The following tables describe the are actions that may be performed on the patient\n<hr>\n");
      Set<Class<? extends SEPatientAction>> pActions = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.patient.actions", SEPatientAction.class);
      List<Class<? extends SEPatientAction>> pActionsSorted = pActions.stream().collect(Collectors.toList());
      Collections.sort(pActionsSorted, (o1, o2) -> o1.getName().compareTo(o2.getName()));
      for(Class<?> c : pActionsSorted)
        WriteDoxyTable(c, "", writer, skipProperties);
      Set<Class<? extends SEConsciousRespirationCommand>> cmds = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.patient.actions", SEConsciousRespirationCommand.class);
      for(Class<?> c : cmds)
        WriteDoxyTable(c, "", writer, skipProperties);
      WriteDoxyTable(BrainInjuryData.eType.class, "BrainInjuryData_", writer, skipProperties);
      WriteDoxyTable(HemorrhageData.eType.class, "HemorrhageData_", writer, skipProperties);
      WriteDoxyTable(IntubationData.eType.class, "IntubationData_", writer, skipProperties);
      WriteDoxyTable(SubstanceBolusData.eRoute.class, "SubstanceBolusData_", writer, skipProperties);
      WriteDoxyTable(SupplementalOxygenData.eDevice.class, "SupplementalOxygenData_", writer, skipProperties);
      WriteDoxyTable(ePatientAssessmentType.class, "", writer, skipProperties);
      Set<Class<? extends Object>> pNutrition = FindObjects.findAllClasses("com.kitware.physiology.datamodel.patient.nutrition");
      for(Class<?> c : pNutrition)
        WriteDoxyTable(c, "", writer, skipProperties);
      
      writer.append("#### The following tables describe the assessments that may be performed on the patient\n<hr>\n");
      Set<Class<? extends SEPatientAssessment>> pAsses = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.patient.assessments", SEPatientAssessment.class);
      for(Class<?> c : pAsses)
        WriteDoxyTable(c, "", writer, skipProperties);
      WriteDoxyTable(ePatientAssessmentType.class, "", writer, skipProperties);
      WriteDoxyTable(ePresenceIndicator.class, "eUrinalysis_", writer, skipProperties);
      WriteDoxyTable(eClarityIndicator.class, "eUrinalysis_", writer, skipProperties);
      WriteDoxyTable(eUrineColor.class, "eUrinalysis_", writer, skipProperties);
      WriteDoxyTable(eObservationType.class, "eUrinalysis_", writer, skipProperties);
      WriteDoxyTable(eObservationAmount.class, "eUrinalysis_", writer, skipProperties);
      
      // ENVIRONMENT
      writer.append("#### The following tables describe the external environment that surrounds the patient\n<hr>\n");
      Set<Class<? extends Object>> env = FindObjects.findAllClasses("com.kitware.physiology.datamodel.system.environment");
      for(Class<?> c : env)
        WriteDoxyTable(c, "", writer, skipProperties);
      Set<Class<? extends SEEnvironmentAction>> eActions = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.system.environment.actions", SEEnvironmentAction.class);
      for(Class<?> c : eActions)
        WriteDoxyTable(c, "", writer, skipProperties);
      Set<Class<? extends SEEnvironmentCondition>> eConditions = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.system.environment.conditions", SEEnvironmentCondition.class);
      for(Class<?> c : eConditions)
        WriteDoxyTable(c, "", writer, skipProperties);
      WriteDoxyTable(EnvironmentalConditionsData.eSurroundingType.class, "EnvironmentalConditionsData_", writer, skipProperties);

      // ANESTHESIA MACHINE
      writer.append("#### The following tables describe the anesthesia machine\n<hr>\n"); 
      Set<Class<? extends Object>> anes = FindObjects.findAllClasses("com.kitware.physiology.datamodel.system.equipment.anesthesia");
      for(Class<?> c : anes)
        WriteDoxyTable(c, "", writer, skipProperties);
      Set<Class<? extends SEAnesthesiaMachineAction>> aActions = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.system.equipment.anesthesia.actions", SEAnesthesiaMachineAction.class);
      for(Class<?> c : aActions)
        WriteDoxyTable(c, "", writer, skipProperties);
      WriteDoxyTable(AnesthesiaMachineData.eConnection.class, "AnesthesiaMachineData_", writer, skipProperties);
      WriteDoxyTable(AnesthesiaMachineData.eOxygenSource.class, "AnesthesiaMachineData_", writer, skipProperties);
      WriteDoxyTable(AnesthesiaMachineData.ePrimaryGas.class, "AnesthesiaMachineData_", writer, skipProperties);

      // ECG
      writer.append("#### The following tables describe the %ECG\n<hr>\n");
      Set<Class<? extends Object>> ecg = FindObjects.findAllClasses("com.kitware.physiology.datamodel.system.equipment.electrocardiogram");
      for(Class<?> c : ecg)
        WriteDoxyTable(c, "", writer, skipProperties);

      // INHALER
      writer.append("#### The following tables describe the inhaler\n<hr>\n");
      Set<Class<? extends Object>> inhaler = FindObjects.findAllClasses("com.kitware.physiology.datamodel.system.equipment.inhaler");
      for(Class<?> c : inhaler)
        WriteDoxyTable(c, "", writer, skipProperties);
      Set<Class<? extends SEInhalerAction>> iActions = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.system.equipment.inhaler.actions", SEInhalerAction.class);
      for(Class<?> c : iActions)
        WriteDoxyTable(c, "", writer, skipProperties);

      // SUBSTSANCE
      writer.append("#### The following tables describe substances used in Pulse\n<hr>\n");
      Set<Class<? extends Object>> subs = FindObjects.findAllClasses("com.kitware.physiology.datamodel.substance");
      for(Class<?> c : subs)
        WriteDoxyTable(c, "", writer, skipProperties);
      Set<Class<? extends Object>> subQs = FindObjects.findAllClasses("com.kitware.physiology.datamodel.substance.quantity");
      for(Class<?> c : subQs)
        WriteDoxyTable(c, "", writer, skipProperties);
      WriteDoxyTable(SubstanceData.eState.class, "SubstanceData_", writer, skipProperties);
      WriteDoxyTable(SubstancePhysicochemicalsData.eIonicState.class, "SubstancePhysicochemicalsData_", writer, skipProperties);
      WriteDoxyTable(SubstancePhysicochemicalsData.eBindingProtein.class, "SubstancePhysicochemicalsData_", writer, skipProperties);

      // COMPARTMENT
      writer.append("#### The following tables describe anatomical compartments\n<hr>\n");
      Set<Class<? extends SECompartment>> cmpts = FindObjects.findClassSubTypes("com.kitware.physiology.datamodel.compartment",SECompartment.class);
      for(Class<?> c : cmpts)
        WriteDoxyTable(c, "", writer, skipProperties);

      // SCENARIO
      writer.append("#### The following tables describe a simulation scenario\n<hr>\n");
      Set<Class<? extends Object>> sce = FindObjects.findAllClasses("com.kitware.physiology.datamodel.scenario");
      for(Class<?> c : sce)
        WriteDoxyTable(c, "", writer, skipProperties);
      
      // General Enums
      writer.append("#### The following tables describe general enumerations\n<hr>\n");
      WriteDoxyTable(eSide.class, "", writer, skipProperties);
      WriteDoxyTable(eGate.class, "", writer, skipProperties);
      WriteDoxyTable(eSwitch.class, "", writer, skipProperties);
      WriteDoxyTable(eCharge.class, "", writer, skipProperties);
      
      writer.close();

    } 
    catch (Exception e)
    {
      Log.error("Could not create directory .markdown",e);
    }
  }

  protected static void WriteDoxyTable(Class<?> c, String prefix, PrintWriter writer, List<String> skipProperties)
  {    
    String tableName = c.getSimpleName();
    if(tableName.startsWith("SE"))
      tableName = tableName.substring(2);
    String descPrepend;
    if(c.isEnum())
    {
      tableName = prefix+tableName;
      descPrepend = "@copybrief "+tableName;
    }
    else
      descPrepend = "@copybrief "+prefix+tableName+"Data";

    String columnHeaders[] = new String[3];
    int maxColumnLength[] = new int[columnHeaders.length];
    columnHeaders[0] = "Property Name";
    columnHeaders[1] = "Type";
    columnHeaders[2] = "Definition";
    for(int i=0; i<maxColumnLength.length; i++)
      maxColumnLength[i] = columnHeaders[i].length();

    List<BagMethod> bagMethods = FindObjects.getBagMethods(c,skipProperties);
    if(c.isEnum())
    {
      for(Object o : c.getEnumConstants())
      {
        Method m;
        //Enum<?> e = (Enum<?>)o;
        try
        { 
          if(o.toString().equals("UNRECOGNIZED"))
            continue;
          BagMethod bag = new BagMethod();
          bag.propertyName = o.toString();
          bag.returnType = c;
          bagMethods.add(bag);
        } 
        catch (Exception ex)
        {
          Log.info("Enum is not happy",ex);
        }
      }
    }

    // Compute all of our header values and sizes
    for(BagMethod bag : bagMethods)
    {
      // Header (was built when we had to pull data out of the results file)
      if(bag.propertyName.length()>maxColumnLength[0])
        maxColumnLength[0] = bag.propertyName.length();
      if(bag.returnType.getSimpleName().length()>maxColumnLength[1])
        maxColumnLength[1] = bag.returnType.getSimpleName().length();
    }
    maxColumnLength[2] = descPrepend.length()+maxColumnLength[0];

    try
    {
      // Create file and start the table      
      writer.println("");
      writer.println("@anchor "+StringUtils.removeSpaces(tableName)+"Table");
      if(c.isEnum())
        writer.println("##### "+tableName);
      else
      {
        if (tableNameLinks.contains(tableName))
          writer.println("##### %"+tableName);
        else
          writer.println("##### "+tableName);
      }
      writer.println(descPrepend+"");

      if(!bagMethods.isEmpty())
      {
        for(int i=0; i<columnHeaders.length; i++)
          writer.print("|"+pad(columnHeaders[i],maxColumnLength[i]));
        writer.println("|");
        for(int i=0; i<columnHeaders.length; i++)
          writer.print("|"+pad("---",maxColumnLength[i]));
        writer.println("|");
        // Now loop the vData and write out table rows
        for(BagMethod bag : bagMethods)
        {
          writer.print("|"+pad(bag.propertyName,maxColumnLength[0]));
          if(bag.returnType.getSimpleName().equals("List"))
          {
            // If it is a list, I am doing this special code, not generic smart yet
            if(bag.propertyName.equals("AmbientSubstance"))
            {
              writer.print("|"+"List of SubstanceFractions");
              writer.print("|"+"@ref SubstanceFractionTable");
            }
            else if(bag.propertyName.equals("Commands"))
            {
              writer.print("|"+"List of Commands");
              String tables = "@ref ForcedPauseTable <br> @ref ForcedExhaleTable <br> @ref ForcedInhaleTable <br> @ref UseInhalerTable";
              writer.print("|"+tables);
            }
            else if(bag.propertyName.equals("SubstanceQuantities") && tableName.startsWith("Gas"))
            {
              writer.print("|"+"List of SEGasSubstanceQuantity");
              writer.print("|"+"@ref GasSubstanceQuantityTable");
            }
            else if(bag.propertyName.equals("SubstanceQuantities") && tableName.startsWith("Liquid"))
            {
              writer.print("|"+"List of SELiquidSubstanceQuantity");
              writer.print("|"+"@ref LiquidSubstanceQuantityTable");
            }
            else if(bag.propertyName.equals("Components"))
            {
              writer.print("|"+"List of SESubstanceCompoundComponent");
              writer.print("|"+"@ref SubstanceCompoundComponentTable");
            }
            else if(bag.propertyName.equals("GasFraction"))
            {
              writer.print("|"+"List of SESubstanceFraction");
              writer.print("|"+"@ref SubstanceFractionTable");
            }
            else if(bag.propertyName.equals("Aerosol"
                + ""))
            {
              writer.print("|"+"List of SESubstanceConcentration");
              writer.print("|"+"@ref SubstanceConcentrationTable");
            }
            else if(bag.propertyName.equals("AmbientGas"))
            {
              writer.print("|"+"List of SESubstanceFraction");
              writer.print("|"+"@ref SubstanceFractionTable");
            }
            else if(bag.propertyName.equals("AmbientAerosol"))
            {
              writer.print("|"+"List of SESubstanceConcentration");
              writer.print("|"+"@ref SubstanceConcentrationTable");
            }
            else
              Log.error("Unsupported List type for :"+bag.propertyName+" on table "+tableName);

          }
          else
          {
            if(Enum.class.isAssignableFrom(bag.returnType))
               writer.print("|"+pad("Enum",maxColumnLength[1]));
            else
              writer.print("|"+pad(bag.returnType.getSimpleName(),maxColumnLength[1]));
            if(SEScalar.class.isAssignableFrom(bag.returnType) || 
                Enum.class.isAssignableFrom(bag.returnType) ||
                String.class.isAssignableFrom(bag.returnType) ||
                SEFunction.class.isAssignableFrom(bag.returnType))
              writer.print("|"+pad(descPrepend+"_"+bag.propertyName,maxColumnLength[2]));
            else
            {
              String refTable = bag.returnType.getSimpleName();
              if(refTable.startsWith("SE"))
                refTable = refTable.substring(2);
              writer.print("|"+pad("@ref "+refTable+"Table",maxColumnLength[2]));
            }
          }
          writer.println("|");
        }   
      }
      writer.println("");
    }
    catch(Exception ex)
    {
      Log.error("Error writing cdm table for "+tableName,ex);
    }
    writer.print("\n<hr>\n");
  }
  protected static String pad(String s, int max)
  {
    if(s==null)
      return new String(new char[(max+5)]).replace('\0',' ');
    // Using a pad of 5 between columns
    try
    {
      String ret =  s + new String(new char[(max-s.length())+5]).replace('\0',' ');
      return ret;
    }
    catch(Exception ex)
    {
      Log.error("Could not pad "+s+" with a max of "+max,ex);
      return "";
    }
  }

}
