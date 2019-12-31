/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEExercise : SEPatientAction
{
  protected SEScalar0To1 intensity;

  public SEExercise()
  {
    intensity = null;
  }

  public override void Clear()
  {
    base.Clear();
    if (intensity != null)
      intensity.Invalidate();
  }

  public override bool IsValid()
  {
    return HasIntensity();
  }

  public bool HasIntensity()
  {
    return intensity == null ? false : intensity.IsValid();
  }
  public SEScalar0To1 GetIntensity()
  {
    if (intensity == null)
      intensity = new SEScalar0To1();
    return intensity;
  }
}
