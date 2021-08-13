package com.kitware.pulse.cdm.testing;

import com.google.protobuf.TextFormat;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.cdm.substance.SESubstanceManager;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.JNIBridge;

public class BindingConverter 
{

  public static void main(String[] args) 
  {
    JNIBridge.initialize();

    SESubstanceManager mgr = new SESubstanceManager();
    mgr.loadSubstanceDirectory();

    // Convert pba to json
    for(String pba_filename : FileUtils.findFiles("verification\\scenarios", ".pba", true))
    {
      com.kitware.pulse.cdm.bind.Scenario.ScenarioData.Builder builder = com.kitware.pulse.cdm.bind.Scenario.ScenarioData.newBuilder();
       com.kitware.pulse.cdm.bind.PatientAssessments.PulmonaryFunctionTestData.Builder pftData = com.kitware.pulse.cdm.bind.PatientAssessments.PulmonaryFunctionTestData.newBuilder();
      com.kitware.pulse.cdm.bind.PatientAssessments.ComprehensiveMetabolicPanelData.Builder cmpData = com.kitware.pulse.cdm.bind.PatientAssessments.ComprehensiveMetabolicPanelData.newBuilder();
      com.kitware.pulse.cdm.bind.PatientAssessments.CompleteBloodCountData.Builder cbcData = com.kitware.pulse.cdm.bind.PatientAssessments.CompleteBloodCountData.newBuilder();
      com.kitware.pulse.cdm.bind.PatientAssessments.UrinalysisData.Builder uData = com.kitware.pulse.cdm.bind.PatientAssessments.UrinalysisData.newBuilder();
      com.kitware.pulse.engine.bind.Scenario.ScenarioData.Builder pbuilder = com.kitware.pulse.engine.bind.Scenario.ScenarioData.newBuilder();

      Log.info("Converting file "+pba_filename);
      String json_filename = pba_filename.replace(".pba", ".json");
      try
      {
        TextFormat.getParser().merge(FileUtils.readFile(pba_filename), builder);
        FileUtils.writeFile(json_filename, JsonFormat.printer().print(builder));
        FileUtils.delete(pba_filename);
        continue;
      }catch(Exception ex){}

      try
      {
        TextFormat.getParser().merge(FileUtils.readFile(pba_filename), pbuilder);
        FileUtils.writeFile(json_filename, JsonFormat.printer().print(pbuilder));
        FileUtils.delete(pba_filename);
        continue;
      }
      catch(Exception ex){}
      
      try
      {
        TextFormat.getParser().merge(FileUtils.readFile(pba_filename), pftData);
        FileUtils.writeFile(json_filename, JsonFormat.printer().print(pftData));
        FileUtils.delete(pba_filename);
        continue;
      }
      catch(Exception ex){}
      
      try
      {
        TextFormat.getParser().merge(FileUtils.readFile(pba_filename), cmpData);
        FileUtils.writeFile(json_filename, JsonFormat.printer().print(cmpData));
        FileUtils.delete(pba_filename);
        continue;
      }
      catch(Exception ex){}
      
      try
      {
        TextFormat.getParser().merge(FileUtils.readFile(pba_filename), cbcData);
        FileUtils.writeFile(json_filename, JsonFormat.printer().print(cbcData));
        FileUtils.delete(pba_filename);
        continue;
      }
      catch(Exception ex){}
      
      try
      {
        TextFormat.getParser().merge(FileUtils.readFile(pba_filename), uData);
        FileUtils.writeFile(json_filename, JsonFormat.printer().print(uData));
        FileUtils.delete(pba_filename);
        continue;
      }
      catch(Exception ex){}

    }

  }

}
