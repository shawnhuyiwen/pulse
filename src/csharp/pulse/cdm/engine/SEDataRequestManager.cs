/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  using System;
  using System.Collections.Generic;

  public class SEDataRequestManager
  {
    protected string results_filename = null;
    protected double samples_per_second = 0;// Sample every time step
    protected List<SEDataRequest> data_requests = null;

    public SEDataRequestManager()
    {

    }

    public SEDataRequestManager(List<SEDataRequest> data_requests)
    {
      SetDataRequests(data_requests);
    }

    public bool HasResultsFilename() { return results_filename != null; }
    public string GetResultFilename() { return results_filename; }
    public void SetResultsFilename(string filename) { results_filename = filename; }

    public double GetSamplesPerSecond() { return samples_per_second; }
    public void SetSamplesPerSecond(double num) { samples_per_second = num; }

    public bool HasDataRequests() { return data_requests != null && data_requests.Count > 0; }
    public void SetDataRequests(List<SEDataRequest> data_requests) { this.data_requests = data_requests; }
    public List<SEDataRequest> GetDataRequests() { return data_requests; }

    public void ToConsole(double[] data_values)
    {
      Console.WriteLine("SimTime(s)=" + data_values[0]);
      for (int d = 1; d < data_values.Length; d++)
        Console.WriteLine(data_requests[d - 1].ToString() + "=" + data_values[d]);
    }
  }
}
