package com.kitware.pulse.engine;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;
import com.kitware.pulse.engine.bind.Scenario.ScenarioData;
import com.kitware.pulse.cdm.scenario.SEScenario;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;


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
  public void writeFile(String fileName)
  {
    FileUtils.writeFile(fileName, toJSON());
  }
  
  public String toJSON() 
  {
    String json;
    try
    {
      json = JsonFormat.printer().print(PulseScenario.unload(this));
    }
    catch (InvalidProtocolBufferException e)
    {
      Log.error("Unable to generate json from SEScenario");
      json = "";
    }
    return json;
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
