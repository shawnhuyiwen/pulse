package com.kitware.physiology.testing;

import com.google.protobuf.TextFormat;
import com.google.protobuf.util.JsonFormat;
import com.kitware.physiology.datamodel.substance.SESubstanceManager;
import com.kitware.physiology.utilities.FileUtils;
import com.kitware.physiology.utilities.Log;
import com.kitware.physiology.utilities.jniBridge;

public class BindingConverter 
{

  public static void main(String[] args) 
  {
    jniBridge.initialize();

    SESubstanceManager mgr = new SESubstanceManager();
    mgr.loadSubstanceDirectory();

    // Convert pba to json
    for(String pba_filename : FileUtils.findFiles("verification\\scenarios", ".pba", true))
    {
      com.kitware.physiology.cdm.Scenario.ScenarioData.Builder builder = com.kitware.physiology.cdm.Scenario.ScenarioData.newBuilder();
      com.kitware.physiology.pulse.Pulse.ScenarioData.Builder pbuilder = com.kitware.physiology.pulse.Pulse.ScenarioData.newBuilder();
      com.kitware.physiology.cdm.PatientAssessments.PulmonaryFunctionTestData.Builder pftData = com.kitware.physiology.cdm.PatientAssessments.PulmonaryFunctionTestData.newBuilder();
      com.kitware.physiology.cdm.PatientAssessments.ComprehensiveMetabolicPanelData.Builder cmpData = com.kitware.physiology.cdm.PatientAssessments.ComprehensiveMetabolicPanelData.newBuilder();
      com.kitware.physiology.cdm.PatientAssessments.CompleteBloodCountData.Builder cbcData = com.kitware.physiology.cdm.PatientAssessments.CompleteBloodCountData.newBuilder();
      com.kitware.physiology.cdm.PatientAssessments.UrinalysisData.Builder uData = com.kitware.physiology.cdm.PatientAssessments.UrinalysisData.newBuilder();

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
