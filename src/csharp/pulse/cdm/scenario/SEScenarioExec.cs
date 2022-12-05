/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEScenarioExec
  {
    protected eSwitch log_to_console;
    protected string data_root_directory;
    protected string output_root_directory;
    protected eSwitch organize_output_directory;

    protected eSwitch auto_serialize_after_actions;
    protected double auto_serialize_period_s;
    protected eSwitch time_stamp_serialized_states;

    protected string engine_configuration_content;
    protected string engine_configuration_filename;

    protected string scenario_content;
    protected string scenario_filename;
    protected string scenario_directory;

    protected string data_request_files_search;

    protected eSerializationFormat contentFormat;
    protected int thread_count;

    public SEScenarioExec()
    {
      Clear();
    }

    public virtual void Clear()
    {
      log_to_console = eSwitch.On;
      data_root_directory = "./";
      output_root_directory = "";
      organize_output_directory = eSwitch.Off;

      auto_serialize_after_actions = eSwitch.Off;
      auto_serialize_period_s = 0;
      time_stamp_serialized_states = eSwitch.On;

      engine_configuration_content = "";
      engine_configuration_filename = "";

      scenario_content = "";
      scenario_filename = "";
      scenario_directory = "";

      data_request_files_search = "";

      contentFormat = eSerializationFormat.JSON;
      thread_count = -1;
    }

    public eSwitch GetLogToConsole() { return log_to_console; }
    public void SetLogToConsole(eSwitch s) { log_to_console = s; }

    public string GetDataRootDirectory() { return data_root_directory; }
    public void SetDataRootDirectory(string s) { data_root_directory = s; }

    public string GetOutputRootDirectory() { return output_root_directory; }
    public void SetOutputRootDirectory(string s) { output_root_directory = s; }
    public eSwitch GetOrganizeOutputDirectory() { return organize_output_directory; }
    public void SetOrganizeOutputDirectory(eSwitch s) { organize_output_directory = s; }

    public eSwitch GetAutoSerializeAfterActions() { return auto_serialize_after_actions; }
    public void SetAutoSerializeAfterActions(eSwitch s) { auto_serialize_after_actions = s; }

    public double GetAutoSerializePeriod_s() { return auto_serialize_period_s; }
    public void SetAutoSerializePeriod_s(double d) { auto_serialize_period_s = d; }

    public eSwitch GetTimeStampSerializedStates() { return time_stamp_serialized_states; }
    public void SetTimeStampSerializedStates(eSwitch s) { time_stamp_serialized_states = s; }

    public string GetEngineConfigurationContent() { return engine_configuration_content; }
    public void SetEngineConfigurationContent(string s) { engine_configuration_content = s; }

    public string GetEngineConfigurationFilename() { return engine_configuration_filename; }
    public void SetEngineConfigurationFilename(string s) { engine_configuration_filename = s; }

    public string GetScenarioContent() { return scenario_content; }
    public void SetScenarioContent(string s)
    {
      scenario_content = s;
      scenario_filename = "";
      scenario_directory = "";
    }

    public string GetScenarioFilename() { return scenario_filename; }
    public void SetScenarioFilename(string s)
    {
      scenario_content = "";
      scenario_filename = s;
      scenario_directory = "";
    }

    public string GetScenarioDirectory() { return scenario_directory; }
    public void SetScenarioDirectory(string s)
    {
      scenario_content = "";
      scenario_filename = "";
      scenario_directory = s;
    }

    public string GetDataRequestFilesSearch() { return data_request_files_search; }
    public void SetDataRequestFilesSearch(string s) { data_request_files_search = s; }

    public eSerializationFormat GetContentFormat() { return contentFormat; }
    public void SetContentFormat(eSerializationFormat s) { contentFormat = s; }

    public int GetThreadCount() { return thread_count; }
    public void SetThreadCount(int c) { thread_count = c; }
  }
}
