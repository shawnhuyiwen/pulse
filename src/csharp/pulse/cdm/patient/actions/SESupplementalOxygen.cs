/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eSupplementalOxygen_Device : int
  {
    None = 0,
    NasalCannula,
    SimpleMask,
    NonRebreatherMask
  }

  public class SESupplementalOxygen : SEPatientAction
  {
    protected eSupplementalOxygen_Device device;
    protected SEScalarVolumePerTime flow;
    protected SEScalarVolume volume;

    public SESupplementalOxygen()
    {
      device = eSupplementalOxygen_Device.None;
      flow = null;
      volume = null;
    }

    public override void Clear()
    {
      base.Clear();
      device = eSupplementalOxygen_Device.None;
      if (flow != null)
        flow.Invalidate();
      if (volume != null)
        volume.Invalidate();
    }

    public override bool IsValid()
    {
      return HasFlow() && HasVolume();
    }

    public eSupplementalOxygen_Device GetDevice() { return device; }
    public void SetDevice(eSupplementalOxygen_Device d) { device = d; }

    public bool HasFlow()
    {
      return flow == null ? false : flow.IsValid();
    }
    public SEScalarVolumePerTime GetFlow()
    {
      if (flow == null)
        flow = new SEScalarVolumePerTime();
      return flow;
    }

    public bool HasVolume()
    {
      return volume == null ? false : volume.IsValid();
    }
    public SEScalarVolume GetVolume()
    {
      if (volume == null)
        volume = new SEScalarVolume();
      return volume;
    }
  }
}
