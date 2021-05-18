/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public abstract class SEInhalerAction : SEEquipmentAction
  {
    public SEInhalerAction()
    {

    }

    public void Copy(SEInhalerAction other)
    {
      base.Copy(other);
    }

    public override void Clear()
    {
      base.Clear();
    }
  }
}