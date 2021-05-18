/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.IO;

namespace Pulse.CDM
{
  public class Logger
  {
    private StreamWriter log;

    public Logger(string filename)
    {
      log = new StreamWriter(filename, false);
    }

    public void WriteLine(string line)
    {
      log.WriteLine(line);
      log.Flush();
      Console.WriteLine(line);
    }
  }
}
