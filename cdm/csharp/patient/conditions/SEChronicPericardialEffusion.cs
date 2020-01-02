/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEChronicPericardialEffusion : SEPatientCondition
{
  protected SEScalarVolume accumulated_volume;

  public SEChronicPericardialEffusion()
  {
    accumulated_volume = null;
  }

  public override void Clear()
  {
    base.Clear();
    if (accumulated_volume != null)
      accumulated_volume.Invalidate();
  }

  public override bool IsValid()
  {
    return HasAccumulatedVolume();
  }

  public bool HasAccumulatedVolume()
  {
    return accumulated_volume == null ? false : accumulated_volume.IsValid();
  }
  public SEScalarVolume GetAccumulatedVolume()
  {
    if (accumulated_volume == null)
      accumulated_volume = new SEScalarVolume();
    return accumulated_volume;
  }
}
