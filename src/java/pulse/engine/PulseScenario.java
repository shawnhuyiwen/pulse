package pulse.engine;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import pulse.cdm.scenario.SEScenario;
import pulse.cpm.bind.Pulse.ScenarioData;
import pulse.utilities.FileUtils;


public class PulseScenario extends SEScenario 
{
  protected PulseConfiguration config = null;

  public PulseScenario()
  {
    super();
  }

  public void reset() 
  {
    if(this.config != null)
    this.config.reset();
  }

  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    ScenarioData.Builder builder = ScenarioData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    PulseScenario.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(PulseScenario.unload(this)));
  }

  public static void load(ScenarioData src, PulseScenario dst)
  {
    SEScenario.load(src.getScenario(), dst);

    if(src.hasConfiguration())
      PulseConfiguration.load(src.getConfiguration(),dst.getConfiguration());
  }

  public static ScenarioData unload(PulseScenario src)
  {
    ScenarioData.Builder dst = ScenarioData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(PulseScenario src, ScenarioData.Builder dst)
  {
    SEScenario.unload(src, dst.getScenarioBuilder());

    if(src.hasConfiguration())
      dst.setConfiguration(PulseConfiguration.unload(src.config));
  }
  
  public boolean hasConfiguration()
  {
    return config!=null && config.isValid();
  }
  public PulseConfiguration getConfiguration()
  {
    if(this.config==null)
      this.config=new PulseConfiguration();
    return this.config;
  }
  public void invalidatePatientConfiguration()
  {
    this.config = null;
  }
}
