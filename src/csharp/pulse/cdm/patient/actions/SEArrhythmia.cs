/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eHeartRhythm : int
  {
    NormalSinus = 0,
    SinusBradycardia,
    SinusTachycardia,
    SinusPulselessElectricalActivity,
    Asystole,
    CoarseVentricularFibrillation,
    FineVentricularFibrillation,
    PulselessVentricularTachycardia,
    StableVentricularTachycardia,
    UnstableVentricularTachycardia
  }

   public class SEArrhythmia : SEPatientAction
  {
    protected eHeartRhythm rhythm;

    public SEArrhythmia()
    {
      rhythm = eHeartRhythm.NormalSinus;
    }

    public override void Clear()
    {
      base.Clear();
      rhythm = eHeartRhythm.NormalSinus;
    }

    public override bool IsValid()
    {
      return true;
    }

    public eHeartRhythm GetRhythm()
    {
      return rhythm;
    }
    public void SetRhythm(eHeartRhythm r)
    {
      rhythm = r;
    }
  }
}
