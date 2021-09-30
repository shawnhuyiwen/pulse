/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEMechanicalVentilatorMode : SEMechanicalVentilatorAction
  {
    protected eSwitch connection;

    public SEMechanicalVentilatorMode()
    {
      Clear();
    }

    public SEMechanicalVentilatorMode(SEMechanicalVentilatorMode other)
    {
      Copy(other);
    }

    public void copy(SEMechanicalVentilatorMode other)
    {
      base.Copy(other);
      connection = other.connection;
    }

    public override void Clear()
    {
      base.Clear();
      connection = eSwitch.Off;
    }

    public override bool IsValid()
    {
      return true;
    }

    public eSwitch GetConnection()
    {
      return connection;
    }
    public void SetConnection(eSwitch s)
    {
      connection = s;
    }

    public override string ToString()
    {
      string str = "Mechanical Ventilator Mode";
      str += "\n\tConnection: " + this.connection;

      return str;
    }
  }
}
