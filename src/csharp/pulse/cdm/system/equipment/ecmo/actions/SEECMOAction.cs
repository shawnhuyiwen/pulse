/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public abstract class SEECMOAction : SEEquipmentAction
  {
    public SEECMOAction()
    {

    }

    public void Copy(SEECMOAction other)
    {
      base.Copy(other);
    }

    public override void Clear()
    {
      base.Clear();
    }
  }
}