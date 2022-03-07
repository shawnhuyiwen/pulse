/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEAdvanceTime : SEAction
  {
    protected SEScalarTime time;

    public SEAdvanceTime()
    {
      time = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (time != null)
        time.Invalidate();
    }

    public override bool IsValid()
    {
      return HasTime();
    }

    public bool HasTime()
    {
      return time == null ? false : time.IsValid();
    }
    public SEScalarTime GetTime()
    {
      if (time == null)
        time = new SEScalarTime();
      return time;
    }
  }
}
