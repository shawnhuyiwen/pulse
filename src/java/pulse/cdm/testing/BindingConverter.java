package pulse.cdm.testing;

import com.google.protobuf.TextFormat;
import com.google.protobuf.util.JsonFormat;
import pulse.cdm.substance.SESubstanceManager;
import pulse.utilities.FileUtils;
import pulse.utilities.Log;
import pulse.utilities.jniBridge;

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
      pulse.cdm.bind.Scenario.ScenarioData.Builder builder = pulse.cdm.bind.Scenario.ScenarioData.newBuilder();
      pulse.cpm.bind.Pulse.ScenarioData.Builder pbuilder = pulse.cpm.bind.Pulse.ScenarioData.newBuilder();
      pulse.cdm.bind.PatientAssessments.PulmonaryFunctionTestData.Builder pftData = pulse.cdm.bind.PatientAssessments.PulmonaryFunctionTestData.newBuilder();
      pulse.cdm.bind.PatientAssessments.ComprehensiveMetabolicPanelData.Builder cmpData = pulse.cdm.bind.PatientAssessments.ComprehensiveMetabolicPanelData.newBuilder();
      pulse.cdm.bind.PatientAssessments.CompleteBloodCountData.Builder cbcData = pulse.cdm.bind.PatientAssessments.CompleteBloodCountData.newBuilder();
      pulse.cdm.bind.PatientAssessments.UrinalysisData.Builder uData = pulse.cdm.bind.PatientAssessments.UrinalysisData.newBuilder();

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
