/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public abstract class SEMechanicalVentilatorAction : SEEquipmentAction
  {
    public SEMechanicalVentilatorAction()
    {

    }

    public void Copy(SEMechanicalVentilatorAction other)
    {
      base.Copy(other);
    }

    public override void Clear()
    {
      base.Clear();
    }
  }
}