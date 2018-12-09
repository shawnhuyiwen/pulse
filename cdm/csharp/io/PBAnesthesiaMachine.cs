/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PBAnesthesiaMachine
{
  public static string Connection_Name(SEAnesthesiaMachine.Connection c)
  {
    return ((Cdm.eAnesthesiaMachine.Types.Connection)(int)c).ToString();
  }
  public static string OxygenSource_Name(SEAnesthesiaMachine.OxygenSource os)
  {
    return ((Cdm.eAnesthesiaMachine.Types.OxygenSource)(int)os).ToString();
  }
  public static string PrimaryGas_Name(SEAnesthesiaMachine.PrimaryGas pg)
  {
    return ((Cdm.eAnesthesiaMachine.Types.PrimaryGas)(int)pg).ToString();
  }

  #region SEAnesthesiaMachine
  public static void Load(Cdm.AnesthesiaMachineData src, SEAnesthesiaMachine dst/*, SESubstanceManager subMgr*/)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.AnesthesiaMachineData src, SEAnesthesiaMachine dst/*, SESubstanceManager subMgr*/)
  {
    dst.Clear();
    dst.SetConnection((SEAnesthesiaMachine.Connection)src.Connection);
    if (src.InletFlow != null)
      PBProperty.Load(src.InletFlow, dst.GetInletFlow());
    if (src.InspiratoryExpiratoryRatio != null)
      PBProperty.Load(src.InspiratoryExpiratoryRatio, dst.GetInspiratoryExpiratoryRatio());
    if (src.OxygenFraction != null)
      PBProperty.Load(src.OxygenFraction, dst.GetOxygenFraction());
    dst.SetOxygenSource((SEAnesthesiaMachine.OxygenSource)src.OxygenSource);
    if (src.PositiveEndExpiredPressure != null)
      PBProperty.Load(src.PositiveEndExpiredPressure, dst.GetPositiveEndExpiredPressure());
    dst.SetPrimaryGas((SEAnesthesiaMachine.PrimaryGas)src.PrimaryGas);
    if (src.RespiratoryRate != null)
      PBProperty.Load(src.RespiratoryRate, dst.GetRespiratoryRate());
    if (src.ReliefValvePressure != null)
      PBProperty.Load(src.ReliefValvePressure, dst.GetReliefValvePressure());
    if (src.VentilatorPressure != null)
      PBProperty.Load(src.VentilatorPressure, dst.GetVentilatorPressure());
    //if (src.LeftChamber!=null)
    //  PBAnesthesiaMachine.Load(src.GetLeftChamber(), dst.GetLeftChamber(), subMgr);
    //if (src.RightChamber!=null)
    //  PBAnesthesiaMachine.Load(src.GetRightChamber(), dst.GetRightChamber(), subMgr);
    //if (src.OxygenBottleOne!=null)
    //  PBAnesthesiaMachine.Load(src.GetOxygenBottleOne(), dst.GetOxygenBottleOne());
    //if (src.OxygenBottleTwo!=null)
    //  PBAnesthesiaMachine.Load(src.GetOxygenBottleTwo(), dst.GetOxygenBottleTwo());
  }

  public static Cdm.AnesthesiaMachineData Unload(SEAnesthesiaMachine src)
  {
    Cdm.AnesthesiaMachineData dst = new Cdm.AnesthesiaMachineData();
    Unload(src, dst);
    return dst;
  }
  protected static void Unload(SEAnesthesiaMachine src, Cdm.AnesthesiaMachineData dst)
  {
    dst.Connection = (Cdm.eAnesthesiaMachine.Types.Connection)(int)src.GetConnection();
    if (src.HasInletFlow())
      dst.InletFlow = PBProperty.Unload(src.GetInletFlow());
    if (src.HasInspiratoryExpiratoryRatio())
      dst.InspiratoryExpiratoryRatio = PBProperty.Unload(src.GetInspiratoryExpiratoryRatio());
    if (src.HasOxygenFraction())
      dst.OxygenFraction = PBProperty.Unload(src.GetOxygenFraction());
    dst.OxygenSource = (Cdm.eAnesthesiaMachine.Types.OxygenSource)(int)src.GetOxygenSource();
    if (src.HasPositiveEndExpiredPressure())
      dst.PositiveEndExpiredPressure = PBProperty.Unload(src.GetPositiveEndExpiredPressure());
    dst.PrimaryGas = (Cdm.eAnesthesiaMachine.Types.PrimaryGas)(int)src.GetPrimaryGas();
    if (src.HasRespiratoryRate())
      dst.RespiratoryRate = PBProperty.Unload(src.GetRespiratoryRate());
    if (src.HasReliefValvePressure())
      dst.ReliefValvePressure = PBProperty.Unload(src.GetReliefValvePressure());
    if (src.HasVentilatorPressure())
      dst.VentilatorPressure = PBProperty.Unload(src.GetVentilatorPressure());

    //if (src.HasLeftChamber())
    //  dst.LeftChamber(SEAnesthesiaMachineChamber.Unload(src.leftChamber));
    //if (src.HasRightChamber())
    //  dst.RightChamber(SEAnesthesiaMachineChamber.Unload(src.rightChamber));
    //if (src.HasOxygenBottleOne())
    //  dst.OxygenBottleOne(SEAnesthesiaMachineOxygenBottle.Unload(src.oxygenBottleOne));
    //if (src.HasOxygenBottleTwo())
    //  dst.OxygenBottleTwo(SEAnesthesiaMachineOxygenBottle.Unload(src.oxygenBottleTwo));
  }
  #endregion
}

//public static void Load(AnesthesiaMachineChamberData src, SEAnesthesiaMachineChamber dst, SESubstanceManager subMgr)
//{
//  dst.reSet();
//  if (src.GetState() != eSwitch.UNRECOGNIZED && src.GetState() != eSwitch.NullSwitch)
//    dst.SetState(src.GetState());
//  if (src.GetSubstance() != null)
//    dst.SetSubstance(subMgr.GetSubstance(src.GetSubstance()));
//  if (src.HasSubstanceFraction())
//    SEScalar0To1.Load(src.GetSubstanceFraction(), dst.GetSubstanceFraction());
//}
//public static AnesthesiaMachineChamberData Unload(SEAnesthesiaMachineChamber src)
//{
//  AnesthesiaMachineChamberData.Builder dst = AnesthesiaMachineChamberData.newBuilder();
//  Unload(src, dst);
//  return dst.build();
//}
//protected static void Unload(SEAnesthesiaMachineChamber src, AnesthesiaMachineChamberData.Builder dst)
//{
//  if (src.HasSubstance())
//    dst.SetSubstance(src.substance.GetName());
//  dst.SetState(src.state);
//  if (src.HasSubstanceFraction())
//    dst.SetSubstanceFraction(SEScalar0To1.Unload(src.substanceFraction));
//}
