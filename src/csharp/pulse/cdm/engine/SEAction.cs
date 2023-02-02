/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Linq;

namespace Pulse.CDM
{
  public enum eMergeType : int
  {
    Append = 0,
    Replace
  }
  
  public enum eAppliedRespiratoryCycle : int
  {
    Expiratory = 0,
    Inspiratory,
    Instantaneous
  }
  public abstract class SEAction
  {
    protected string comment;

    public SEAction()
    {
      comment = null;
    }

    public void Copy(SEAction other)
    {
      Clear();
      comment = other.comment;
    }

    public virtual void Clear()
    {
      comment = null;
    }

    public bool HasComment()
    {
      return !string.IsNullOrEmpty(comment);
    }
    public string GetComment()
    {
      return this.comment;
    }
    public void SetComment(string comment)
    {
      this.comment = comment;
    }
    public void InvalidateComment()
    {
      this.comment = null;
    }

    public abstract bool IsValid();

    public static string PrettyPrint(string msg)
    {
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
      for (int i=0; i< lines.Length; i++)
      {
        line = lines[i].TrimEnd();
        if (line.Length == 0)
          continue;
        if (line.Contains("Action"))
          continue;
        if (line.Contains("ReadOnly"))
          continue;
        if (line.Contains("Comment:") && line.Length < 9)
            continue;

        if (i+1 < lines.Length)
        {
          peek = lines[i+1];
          if (peek.Contains("Scalar"))
          {
            peek = lines[i+2];
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
        ret += line+"\n";
      }

      ret = ret.Replace("::", ":");
      return ret;
    }
  }
}
