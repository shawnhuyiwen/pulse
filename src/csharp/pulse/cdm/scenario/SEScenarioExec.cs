/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEScenarioExec
  {
    protected eSwitch log_to_console;
    protected string log_filename;
    protected string data_request_csv_filename;
    protected string data_root_directory;

    protected string serialization_directory;
    protected string auto_serialize_filename;
    protected eSwitch auto_serialize_after_actions;
    protected double auto_serialize_period_s;
    protected eSwitch time_stamp_serialized_states;
    protected eSwitch reloa_serialized_state;

    protected string engine_configuration_content;
    protected string engine_configuration_filename;

    protected string scenario_content;
    protected string scenario_filename;

    protected eSerializationFormat contentFormat;

    public SEScenarioExec()
    {
      Clear();
    }

    public void Clear()
    {
      log_to_console = eSwitch.On;
      log_filename = "";
      data_request_csv_filename = "";
      data_root_directory = "./";

      serialization_directory = "./";
      auto_serialize_filename = "";
      auto_serialize_after_actions = eSwitch.Off;
      auto_serialize_period_s = 0;
      time_stamp_serialized_states = eSwitch.On;
      reloa_serialized_state = eSwitch.On;

      engine_configuration_content = "";
      engine_configuration_filename = "";

      scenario_content = "";
      scenario_filename = "";

      contentFormat = eSerializationFormat.JSON;
    }

    public eSwitch GetLogToConsole() { return log_to_console; }
    public void SetLogToConsole(eSwitch s) { log_to_console = s; }

    public string GetLogFilename() { return log_filename; }
    public void SetLogFilename(string s) { log_filename = s; }

    public string GetDataRequestCSVFilename() { return data_request_csv_filename; }
    public void SetDataRequestCSVFilename(string s) { data_request_csv_filename = s; }

    public string GetDataRootDirectory() { return data_root_directory; }
    public void SetDataRootDirectory(string s) { data_root_directory = s; }

    public string GetSerializationDirectory() { return serialization_directory; }
    public void SetSerializationDirectory(string s) { serialization_directory = s; }

    public string GetAutoSerializeFilename() { return auto_serialize_filename; }
    public void SetAutoSerializeFilename(string s) { auto_serialize_filename = s; }

    public eSwitch GetAutoSerializeAfterActions() { return auto_serialize_after_actions; }
    public void SetAutoSerializeAfterActions(eSwitch s) { auto_serialize_after_actions = s; }

    public double GetAutoSerializePeriod_s() { return auto_serialize_period_s; }
    public void SetAutoSerializePeriod_s(double d) { auto_serialize_period_s = d; }

    public eSwitch GetTimeStampSerializedStates() { return time_stamp_serialized_states; }
    public void SetTimeStampSerializedStates(eSwitch s) { time_stamp_serialized_states = s; }

    public eSwitch GetReloadSerializedState() { return reloa_serialized_state; }
    public void SetReloadSerializedState(eSwitch s) { reloa_serialized_state = s; }

    public string GetEngineConfigurationContent() { return engine_configuration_content; }
    public void SetEngineConfigurationContent(string s) { engine_configuration_content = s; }

    public string GetEngineConfigurationFilename() { return engine_configuration_filename; }
    public void SetEngineConfigurationFilename(string s) { engine_configuration_filename = s; }

    public string GetScenarioContent() { return scenario_content; }
    public void SetScenarioContent(string s) { scenario_content = s; }

    public string GetScenarioFilename() { return scenario_filename; }
    public void SetScenarioFilename(string s) { scenario_filename = s; }

    public eSerializationFormat GetContentFormat() { return contentFormat; }
    public void SetContentFormat(eSerializationFormat s) { contentFormat = s; }
  }
}
