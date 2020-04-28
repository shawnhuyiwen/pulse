/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public abstract class SEAnesthesiaMachineAction : SEEquipmentAction
  {
    public SEAnesthesiaMachineAction()
    {

    }

    public void Copy(SEAnesthesiaMachineAction other)
    {
      base.Copy(other);
    }

    public override void Clear()
    {
      base.Clear();
    }
  }
}