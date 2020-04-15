/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public abstract class SESystem
  {
    public SESystem()
    {
    }

    public virtual void Clear()
    {
     
    }

    public void Copy(SESystem from)
    {
      Clear();
    }


    public abstract override string ToString();
  }
}