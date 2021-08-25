/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eIntubation_Type : int
  {
    Off = 0,
    Esophageal,
    LeftMainstem,
    RightMainstem,
    Tracheal,
    Oropharyngeal,
    Nasopharyngeal
  }

  public class SEIntubation : SEPatientAction
  {
    protected eIntubation_Type type;

    public SEIntubation()
    {
      type = eIntubation_Type.Off;
    }

    public override void Clear()
    {
      base.Clear();
      type = eIntubation_Type.Off;
    }

    public override bool IsValid()
    {
      return type != eIntubation_Type.Off;
    }

    public new eIntubation_Type GetType()
    {
      return type;
    }
    public void SetType(eIntubation_Type t)
    {
      type = t;
    }
  }
}
