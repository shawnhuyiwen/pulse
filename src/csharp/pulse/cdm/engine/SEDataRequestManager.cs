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
    protected List<SEDataRequest> data_requests = new List<SEDataRequest>();

    public SEDataRequestManager()
    {

    }

    public SEDataRequestManager(List<SEDataRequest> data_requests)
    {
      SetDataRequests(data_requests);
    }

    public void Clear()
    {
      results_filename = null;
      samples_per_second = 0;
      data_requests.Clear();
    }

    public bool HasResultsFilename() { return results_filename != null; }
    public string GetResultFilename() { return results_filename; }
    public void SetResultsFilename(string filename) { results_filename = filename; }

    public double GetSamplesPerSecond() { return samples_per_second; }
    public void SetSamplesPerSecond(double num) { samples_per_second = num; }

    public bool HasDataRequests() { return data_requests != null && data_requests.Count > 0; }
    public void SetDataRequests(List<SEDataRequest> data_requests) { this.data_requests = data_requests; }
    public List<SEDataRequest> GetDataRequests() { return data_requests; }

    public void WriteData(double[] data_values, Logger log=null)
    {
      if (log != null)
        log.WriteLine("SimTime(s)=" + data_values[0]);
      else
        Console.WriteLine("SimTime(s)=" + data_values[0]);

      for (int d = 1; d < data_values.Length; d++)
      {
        if (log != null)
          log.WriteLine(data_requests[d - 1].ToString() + "=" + data_values[d]);
        else
          Console.WriteLine(data_requests[d - 1].ToString() + "=" + data_values[d]);
      }

      if (log != null)
        log.WriteLine("");
      else
        Console.WriteLine("");
    }
  }
}
