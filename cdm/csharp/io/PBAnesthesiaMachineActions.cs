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
      PBAnesthesiaMachineAction.Load(any.MaskLeak, dst);
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

  public static Cdm.AnyAnesthesiaMachineActionData Unload(SEAnesthesiaMachineAction action)
  {
    Cdm.AnyAnesthesiaMachineActionData any = new Cdm.AnyAnesthesiaMachineActionData();

    if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineConfiguration)))
    {
      any.Configuration = Unload((SEAnesthesiaMachineConfiguration)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEExpiratoryValveLeak)))
    {
      any.ExpiratoryValveLeak = Unload((SEExpiratoryValveLeak)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEExpiratoryValveObstruction)))
    {
      any.ExpiratoryValveObstruction = Unload((SEExpiratoryValveObstruction)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEInspiratoryValveLeak)))
    {
      any.InspiratoryValveLeak = Unload((SEInspiratoryValveLeak)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEInspiratoryValveObstruction)))
    {
      any.InspiratoryValveObstruction = Unload((SEInspiratoryValveObstruction)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEMaskLeak)))
    {
      any.MaskLeak = Unload((SEMaskLeak)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEOxygenTankPressureLoss)))
    {
      any.OxygenTankPressureLoss = Unload((SEOxygenTankPressureLoss)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEOxygenWallPortPressureLoss)))
    {
      any.OxygenWallPortPressureLoss = Unload((SEOxygenWallPortPressureLoss)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SESodaLimeFailure)))
    {
      any.SodaLimeFailure = Unload((SESodaLimeFailure)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SETubeCuffLeak)))
    {
      any.TubeCuffLeak = Unload((SETubeCuffLeak)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEVaporizerFailure)))
    {
      any.VaporizerFailure = Unload((SEVaporizerFailure)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEVentilatorPressureLoss)))
    {
      any.VentilatorPressureLoss = Unload((SEVentilatorPressureLoss)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEYPieceDisconnect)))
    {
      any.YPieceDisconnect = Unload((SEYPieceDisconnect)action);
      return any;
    }
    //Log.error("Unsupported AnesthesiaMachine Action type " + c);
    return null;
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

  #region SEExpiratoryValveLeak
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

  #region SEExpiratoryValveObstruction
  public static void Load(Cdm.ExpiratoryValveObstructionData src, SEExpiratoryValveObstruction dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ExpiratoryValveObstructionData src, SEExpiratoryValveObstruction dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.ExpiratoryValveObstructionData Unload(SEExpiratoryValveObstruction src)
  {
    Cdm.ExpiratoryValveObstructionData dst = new Cdm.ExpiratoryValveObstructionData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEExpiratoryValveObstruction src, Cdm.ExpiratoryValveObstructionData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEInspiratoryValveLeak
  public static void Load(Cdm.InspiratoryValveLeakData src, SEInspiratoryValveLeak dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.InspiratoryValveLeakData src, SEInspiratoryValveLeak dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.InspiratoryValveLeakData Unload(SEInspiratoryValveLeak src)
  {
    Cdm.InspiratoryValveLeakData dst = new Cdm.InspiratoryValveLeakData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEInspiratoryValveLeak src, Cdm.InspiratoryValveLeakData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEInspiratoryValveObstruction
  public static void Load(Cdm.InspiratoryValveObstructionData src, SEInspiratoryValveObstruction dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.InspiratoryValveObstructionData src, SEInspiratoryValveObstruction dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.InspiratoryValveObstructionData Unload(SEInspiratoryValveObstruction src)
  {
    Cdm.InspiratoryValveObstructionData dst = new Cdm.InspiratoryValveObstructionData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEInspiratoryValveObstruction src, Cdm.InspiratoryValveObstructionData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEMaskLeak
  public static void Load(Cdm.MaskLeakData src, SEMaskLeak dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.MaskLeakData src, SEMaskLeak dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.MaskLeakData Unload(SEMaskLeak src)
  {
    Cdm.MaskLeakData dst = new Cdm.MaskLeakData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEMaskLeak src, Cdm.MaskLeakData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEOxygenTankPressureLoss
  public static void Load(Cdm.OxygenTankPressureLossData src, SEOxygenTankPressureLoss dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.OxygenTankPressureLossData src, SEOxygenTankPressureLoss dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    dst.SetState((eSwitch)(int)src.State);
  }

  public static Cdm.OxygenTankPressureLossData Unload(SEOxygenTankPressureLoss src)
  {
    Cdm.OxygenTankPressureLossData dst = new Cdm.OxygenTankPressureLossData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEOxygenTankPressureLoss src, Cdm.OxygenTankPressureLossData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    dst.State = (Cdm.eSwitch)(int)src.GetState();
  }
  #endregion

  #region SEOxygenWallPortPressureLoss
  public static void Load(Cdm.OxygenWallPortPressureLossData src, SEOxygenWallPortPressureLoss dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.OxygenWallPortPressureLossData src, SEOxygenWallPortPressureLoss dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    dst.SetState((eSwitch)(int)src.State);
  }

  public static Cdm.OxygenWallPortPressureLossData Unload(SEOxygenWallPortPressureLoss src)
  {
    Cdm.OxygenWallPortPressureLossData dst = new Cdm.OxygenWallPortPressureLossData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEOxygenWallPortPressureLoss src, Cdm.OxygenWallPortPressureLossData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    dst.State = (Cdm.eSwitch)(int)src.GetState();
  }
  #endregion

  #region SESodaLimeFailure
  public static void Load(Cdm.SodaLimeFailureData src, SESodaLimeFailure dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.SodaLimeFailureData src, SESodaLimeFailure dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.SodaLimeFailureData Unload(SESodaLimeFailure src)
  {
    Cdm.SodaLimeFailureData dst = new Cdm.SodaLimeFailureData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SESodaLimeFailure src, Cdm.SodaLimeFailureData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SETubeCuffLeak
  public static void Load(Cdm.TubeCuffLeakData src, SETubeCuffLeak dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.TubeCuffLeakData src, SETubeCuffLeak dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.TubeCuffLeakData Unload(SETubeCuffLeak src)
  {
    Cdm.TubeCuffLeakData dst = new Cdm.TubeCuffLeakData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SETubeCuffLeak src, Cdm.TubeCuffLeakData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEVaporizerFailure
  public static void Load(Cdm.VaporizerFailureData src, SEVaporizerFailure dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.VaporizerFailureData src, SEVaporizerFailure dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.VaporizerFailureData Unload(SEVaporizerFailure src)
  {
    Cdm.VaporizerFailureData dst = new Cdm.VaporizerFailureData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEVaporizerFailure src, Cdm.VaporizerFailureData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEVentilatorPressureLoss
  public static void Load(Cdm.VentilatorPressureLossData src, SEVentilatorPressureLoss dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.VentilatorPressureLossData src, SEVentilatorPressureLoss dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.VentilatorPressureLossData Unload(SEVentilatorPressureLoss src)
  {
    Cdm.VentilatorPressureLossData dst = new Cdm.VentilatorPressureLossData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEVentilatorPressureLoss src, Cdm.VentilatorPressureLossData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEYPieceDisconnect
  public static void Load(Cdm.YPieceDisconnectData src, SEYPieceDisconnect dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.YPieceDisconnectData src, SEYPieceDisconnect dst)
  {
    Serialize(src.AnesthesiaMachineAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }

  public static Cdm.YPieceDisconnectData Unload(SEYPieceDisconnect src)
  {
    Cdm.YPieceDisconnectData dst = new Cdm.YPieceDisconnectData();
    Serialize(src, dst);
    return dst;
  }
  protected static void Serialize(SEYPieceDisconnect src, Cdm.YPieceDisconnectData dst)
  {
    dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
    Serialize(src, dst.AnesthesiaMachineAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion
}
