/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eHeartRhythm : int
  {
    NormalSinus = 0,
    Asystole,
    CourseVentricularFibrillation,
    FineVentricularFibrillation,
    PulselessElectricalActivity,
    PulselessVentricularTachycardia,
    StableVentricularTachycardia,
    UnstableVentricularTachycardia
  }

   public class SEArrhythmia : SEPatientAction
  {
    protected eHeartRhythm type;

    public SEArrhythmia()
    {
      type = eHeartRhythm.NormalSinus;
    }

    public override void Clear()
    {
      base.Clear();
      type = eHeartRhythm.NormalSinus;
    }

    public override bool IsValid()
    {
      return true;
    }

    public eHeartRhythm GetType()
    {
      return type;
    }
    public void SetType(eHeartRhythm r)
    {
      type = r;
    }
  }
}
