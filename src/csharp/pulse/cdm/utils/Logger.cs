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
    public enum ePrettyPrintType : int
    {
      Action = 0,
      Condition = 1,
    }
    public static string PrettyPrint(string msg, ePrettyPrintType ppt)
    {
      string typeTag = "";
      switch(ppt)
      {
        case ePrettyPrintType.Action:
          {
            typeTag = "Action";
            break;
          }
        case ePrettyPrintType.Condition:
          {
            typeTag = "Condition";
            break;
          }
        default:
          Console.WriteLine("ERROR: Unknown pretty print type");
          return "";
      }
      string ret = "";
      string buffer = msg;
      buffer = buffer.Replace("\"", "");
      buffer = buffer.Replace("{", "");
      buffer = buffer.Replace("}", "");
      buffer = buffer.Replace(",", "");

      string[] lines = buffer.Split(
        new string[] { "\n" },
        StringSplitOptions.RemoveEmptyEntries);

      string line, peek;
      for (int i = 0; i < lines.Length; i++)
      {
        line = lines[i].TrimEnd();
        if (line.Length == 0)
          continue;
        if (line.Contains(typeTag))
          continue;
        if (line.Contains("ReadOnly"))
          continue;
        if (line.EndsWith("Comment:"))
          continue;

        if (i + 1 < lines.Length)
        {
          peek = lines[i + 1];
          if (peek.Contains("Scalar"))
          {
            peek = lines[i + 2];
            line += peek.Substring(peek.IndexOf("Value") + 5).TrimEnd();
            if (i + 3 < lines.Length && lines[i + 3].Contains("Unit:"))
            {
              i += 3;
              peek = lines[i];
              line += peek.Substring(peek.IndexOf("Unit:") + 5);
            }
            else
              i += 2;
          }
        }
        ret += line + "\n";
      }

      ret = ret.Replace("::", ":");
      return ret;
    }
  }
}
