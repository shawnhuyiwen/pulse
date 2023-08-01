/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESerializeRequested : SEAction
  {
    protected string filename;

    public SESerializeRequested()
    {
      Clear();
    }

    public override void Clear()
    {
      base.Clear();
      filename = null;
    }

    public override bool IsValid()
    {
      return HasFilename();
    }

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
