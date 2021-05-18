/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBInhaler
  {
    #region SEInhaler
    public static void Load(pulse.cdm.bind.InhalerData src, SEInhaler dst/*, SESubstanceManager subMgr*/)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.InhalerData src, SEInhaler dst/*, SESubstanceManager subMgr*/)
    {
      dst.Clear();
      dst.setState((eSwitch)src.State);
      if (src.MeteredDose != null)
        PBProperty.Load(src.MeteredDose, dst.GetMeteredDose());
      if (src.NozzleLoss != null)
        PBProperty.Load(src.NozzleLoss, dst.GetNozzleLoss());
      if (src.SpacerVolume != null)
        PBProperty.Load(src.SpacerVolume, dst.GetSpacerVolume());
      dst.SetSubstance(src.Substance);
    }

    public static pulse.cdm.bind.InhalerData Unload(SEInhaler src)
    {
      pulse.cdm.bind.InhalerData dst = new pulse.cdm.bind.InhalerData();
      Unload(src, dst);
      return dst;
    }
    protected static void Unload(SEInhaler src, pulse.cdm.bind.InhalerData dst)
    {
      dst.State = (pulse.cdm.bind.eSwitch)(int)src.GetState();
      if (src.HasMeteredDose())
        dst.MeteredDose = PBProperty.Unload(src.GetMeteredDose());
      if (src.HasNozzleLoss())
        dst.NozzleLoss = PBProperty.Unload(src.GetNozzleLoss());
      if (src.HasSpacerVolume())
        dst.SpacerVolume = PBProperty.Unload(src.GetSpacerVolume());
      dst.Substance = src.GetSubstance();
    }
    #endregion
  }
}
