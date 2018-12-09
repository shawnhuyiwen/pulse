/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PBAnesthesiaMachineAction
{
  #region Any Anesthesia Machine Action

  public static SEAnesthesiaMachineAction Load(Cdm.AnyAnesthesiaMachineActionData any)
  {
    if (any.Configuration != null)
    {
      SEAnesthesiaMachineConfiguration dst = new SEAnesthesiaMachineConfiguration();
      PBAnesthesiaMachineAction.Load(any.Configuration, dst/*, subMgr*/);
      return dst;
    }
    if (any.ExpiratoryValveLeak != null)
    {
      SEExpiratoryValveLeak dst = new SEExpiratoryValveLeak();
      PBAnesthesiaMachineAction.Load(any.ExpiratoryValveLeak, dst);
      return dst;
    }
    if (any.ExpiratoryValveObstruction != null)
    {
      SEExpiratoryValveObstruction dst = new SEExpiratoryValveObstruction();
      PBAnesthesiaMachineAction.Load(any.ExpiratoryValveObstruction, dst);
      return dst;
    }
    if (any.InspiratoryValveLeak != null)
    {
      SEInspiratoryValveLeak dst = new SEInspiratoryValveLeak();
      PBAnesthesiaMachineAction.Load(any.InspiratoryValveLeak, dst);
      return dst;
    }
    if (any.InspiratoryValveObstruction != null)
    {
      SEInspiratoryValveObstruction dst = new SEInspiratoryValveObstruction();
      PBAnesthesiaMachineAction.Load(any.InspiratoryValveObstruction, dst);
      return dst;
    }
    if (any.MaskLeak != null)
    {
      SEMaskLeak dst = new SEMaskLeak();
      PBAnesthesiaMachineAction.load(any.MaskLeak, dst);
      return dst;
    }
    if (any.SodaLimeFailure != null)
    {
      SESodaLimeFailure dst = new SESodaLimeFailure();
      PBAnesthesiaMachineAction.Load(any.SodaLimeFailure, dst);
      return dst;
    }
    if (any.TubeCuffLeak != null)
    {
      SETubeCuffLeak dst = new SETubeCuffLeak();
      PBAnesthesiaMachineAction.Load(any.TubeCuffLeak, dst);
      return dst;
    }
    if (any.VaporizerFailure != null)
    {
      SEVaporizerFailure dst = new SEVaporizerFailure();
      PBAnesthesiaMachineAction.Load(any.VaporizerFailure, dst);
      return dst;
    }
    if (any.VentilatorPressureLoss != null)
    {
      SEVentilatorPressureLoss dst = new SEVentilatorPressureLoss();
      PBAnesthesiaMachineAction.Load(any.VentilatorPressureLoss, dst);
      return dst;
    }
    if (any.YPieceDisconnect != null)
    {
      SEYPieceDisconnect dst = new SEYPieceDisconnect();
      PBAnesthesiaMachineAction.Load(any.YPieceDisconnect, dst);
      return dst;
    }
    if (any.OxygenTankPressureLoss != null)
    {
      SEOxygenTankPressureLoss dst = new SEOxygenTankPressureLoss();
      PBAnesthesiaMachineAction.Load(any.OxygenTankPressureLoss, dst);
      return dst;
    }
    if (any.OxygenWallPortPressureLoss != null)
    {
      SEOxygenWallPortPressureLoss dst = new SEOxygenWallPortPressureLoss();
      PBAnesthesiaMachineAction.Load(any.OxygenWallPortPressureLoss, dst);
      return dst;
    }

    //Log.error("Unknown AnesthesiaMachine Action");
    return null;
  }

  public static AnyAnesthesiaMachineActionData CDM2ANY(SEAnesthesiaMachineAction c)
  {
    AnyAnesthesiaMachineActionData.Builder dst = AnyAnesthesiaMachineActionData.newBuilder();
    if (c instanceof SEAnesthesiaMachineConfiguration)
    {
      dst.setConfiguration(SEAnesthesiaMachineConfiguration.unload((SEAnesthesiaMachineConfiguration)c));
      return dst.build();
    }
    if (c instanceof SEExpiratoryValveLeak)
    {
      dst.setExpiratoryValveLeak(SEExpiratoryValveLeak.unload((SEExpiratoryValveLeak)c));
      return dst.build();
    }
    if (c instanceof SEExpiratoryValveObstruction)
    {
      dst.setExpiratoryValveObstruction(SEExpiratoryValveObstruction.unload((SEExpiratoryValveObstruction)c));
      return dst.build();
    }
    if (c instanceof SEInspiratoryValveLeak)
    {
      dst.setInspiratoryValveLeak(SEInspiratoryValveLeak.unload((SEInspiratoryValveLeak)c));
      return dst.build();
    }
    if (c instanceof SEInspiratoryValveObstruction)
    {
      dst.setInspiratoryValveObstruction(SEInspiratoryValveObstruction.unload((SEInspiratoryValveObstruction)c));
      return dst.build();
    }
    if (c instanceof SEMaskLeak)
    {
      dst.setMaskLeak(SEMaskLeak.unload((SEMaskLeak)c));
      return dst.build();
    }
    if (c instanceof SESodaLimeFailure)
    {
      dst.setSodaLimeFailure(SESodaLimeFailure.unload((SESodaLimeFailure)c));
      return dst.build();
    }
    if (c instanceof SETubeCuffLeak)
    {
      dst.setTubeCuffLeak(SETubeCuffLeak.unload((SETubeCuffLeak)c));
      return dst.build();
    }
    if (c instanceof SEVaporizerFailure)
    {
      dst.setVaporizerFailure(SEVaporizerFailure.unload((SEVaporizerFailure)c));
      return dst.build();
    }
    if (c instanceof SEVentilatorPressureLoss)
    {
      dst.setVentilatorPressureLoss(SEVentilatorPressureLoss.unload((SEVentilatorPressureLoss)c));
      return dst.build();
    }
    if (c instanceof SEYPieceDisconnect)
    {
      dst.setYPieceDisconnect(SEYPieceDisconnect.unload((SEYPieceDisconnect)c));
      return dst.build();
    }
    if (c instanceof SEOxygenTankPressureLoss)
    {
      dst.setOxygenTankPressureLoss(SEOxygenTankPressureLoss.unload((SEOxygenTankPressureLoss)c));
      return dst.build();
    }
    if (c instanceof SEOxygenWallPortPressureLoss)
    {
      dst.setOxygenWallPortPressureLoss(SEOxygenWallPortPressureLoss.unload((SEOxygenWallPortPressureLoss)c));
      return dst.build();
    }
    Log.error("Unsupported AnesthesiaMachine Action type " + c);
    return dst.build();
  }

  #endregion

  #region SEAnesthesiaMachineAction
  public static void Serialize(Cdm.AnesthesiaMachineActionData src, SEAnesthesiaMachineAction dst)
  {
    PBAction.Serialize(src.Action, dst);
  }
  protected static void Serialize(SEAnesthesiaMachineAction src, Cdm.AnesthesiaMachineActionData dst)
  {
    PBAction.Serialize(src, dst.Action);
  }
  #endregion

  #region SEAnesthesiaMachineConfiguration
  public static void Load(Cdm.AnesthesiaMachineConfigurationData src, SEAnesthesiaMachineConfiguration dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.AnesthesiaMachineConfigurationData src, SEAnesthesiaMachineConfiguration dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Configuration != null)
      PBAnesthesiaMachine.Load(src.Configuration, dst.GetConfiguration());
    else if (src.Configuration != null)
      dst.SetConfigurationFile(src.ConfigurationFile);
  }
  public static Cdm.AnesthesiaMachineConfigurationData Unload(SEAnesthesiaMachineConfiguration src)
  {
    Cdm.AnesthesiaMachineConfigurationData dst = new Cdm.AnesthesiaMachineConfigurationData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEAnesthesiaMachineConfiguration src, Cdm.AnesthesiaMachineConfigurationData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasConfiguration())
      dst.Configuration = PBAnesthesiaMachine.Unload(src.GetConfiguration());
    else if (src.HasConfigurationFile())
      dst.ConfigurationFile = src.GetConfigurationFile();
  }
  #endregion

  #region
  public static void Load(Cdm.ExpiratoryValveLeakData src, SEExpiratoryValveLeak dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ExpiratoryValveLeakData src, SEExpiratoryValveLeak dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.ExpiratoryValveLeakData Unload(SEExpiratoryValveLeak src)
  {
    Cdm.ExpiratoryValveLeakData dst = new Cdm.ExpiratoryValveLeakData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEExpiratoryValveLeak src, Cdm.ExpiratoryValveLeakData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion
}
