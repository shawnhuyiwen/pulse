/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eSerialization_Type : int
  {
    Save = 0,
    Load
  }
  public class SESerializeState : SEAction
  {
    protected string filename;
    protected eSerialization_Type type;

    public SESerializeState()
    {
      Clear();
    }

    public override void Clear()
    {
      base.Clear();
      filename = null;
      type = eSerialization_Type.Save;
    }

    public override bool IsValid()
    {
      return HasFilename();
    }

    public eSerialization_Type GetType() { return type; }
    public void SetType(eSerialization_Type t) { type = t; }

    public bool HasFilename()
    {
      return !string.IsNullOrEmpty(filename);
    }
    public string GetFilename()
    {
      return filename;
    }
    public void SetFilename(string f)
    {
      filename = f;
    }
  }
}
