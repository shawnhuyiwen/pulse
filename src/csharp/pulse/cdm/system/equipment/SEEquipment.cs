/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public abstract class SEEquipment : SESystem
  {
    public SEEquipment()
    {
    }

    public override void Clear() 
    {
      base.Clear();
    }

    public void Copy(SEEquipment from)
    {
      base.Copy(from);
    }


    public abstract override string ToString();
  }
}