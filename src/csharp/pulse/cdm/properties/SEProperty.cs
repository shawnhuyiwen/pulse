/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEProperty
  {
    public virtual void Invalidate() { }

    public virtual void Copy(SEProperty from) { }

    public virtual bool IsValid() { return true; }
  }

  public class Unit
  {
    public Unit(string v) { Value = v; }
    public new string ToString() { return Value; }
    protected string Value;
  }
}
