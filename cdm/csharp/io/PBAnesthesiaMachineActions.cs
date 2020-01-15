/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
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
        SEAnesthesiaMachineExpiratoryValveLeak dst = new SEAnesthesiaMachineExpiratoryValveLeak();
        PBAnesthesiaMachineAction.Load(any.ExpiratoryValveLeak, dst);
        return dst;
      }
      if (any.ExpiratoryValveObstruction != null)
      {
        SEAnesthesiaMachineExpiratoryValveObstruction dst = new SEAnesthesiaMachineExpiratoryValveObstruction();
        PBAnesthesiaMachineAction.Load(any.ExpiratoryValveObstruction, dst);
        return dst;
      }
      if (any.InspiratoryValveLeak != null)
      {
        SEAnesthesiaMachineInspiratoryValveLeak dst = new SEAnesthesiaMachineInspiratoryValveLeak();
        PBAnesthesiaMachineAction.Load(any.InspiratoryValveLeak, dst);
        return dst;
      }
      if (any.InspiratoryValveObstruction != null)
      {
        SEAnesthesiaMachineInspiratoryValveObstruction dst = new SEAnesthesiaMachineInspiratoryValveObstruction();
        PBAnesthesiaMachineAction.Load(any.InspiratoryValveObstruction, dst);
        return dst;
      }
      if (any.MaskLeak != null)
      {
        SEAnesthesiaMachineMaskLeak dst = new SEAnesthesiaMachineMaskLeak();
        PBAnesthesiaMachineAction.Load(any.MaskLeak, dst);
        return dst;
      }
      if (any.SodaLimeFailure != null)
      {
        SEAnesthesiaMachineSodaLimeFailure dst = new SEAnesthesiaMachineSodaLimeFailure();
        PBAnesthesiaMachineAction.Load(any.SodaLimeFailure, dst);
        return dst;
      }
      if (any.TubeCuffLeak != null)
      {
        SEAnesthesiaMachineTubeCuffLeak dst = new SEAnesthesiaMachineTubeCuffLeak();
        PBAnesthesiaMachineAction.Load(any.TubeCuffLeak, dst);
        return dst;
      }
      if (any.VaporizerFailure != null)
      {
        SEAnesthesiaMachineVaporizerFailure dst = new SEAnesthesiaMachineVaporizerFailure();
        PBAnesthesiaMachineAction.Load(any.VaporizerFailure, dst);
        return dst;
      }
      if (any.VentilatorPressureLoss != null)
      {
        SEAnesthesiaMachineVentilatorPressureLoss dst = new SEAnesthesiaMachineVentilatorPressureLoss();
        PBAnesthesiaMachineAction.Load(any.VentilatorPressureLoss, dst);
        return dst;
      }
      if (any.YPieceDisconnect != null)
      {
        SEAnesthesiaMachineYPieceDisconnect dst = new SEAnesthesiaMachineYPieceDisconnect();
        PBAnesthesiaMachineAction.Load(any.YPieceDisconnect, dst);
        return dst;
      }
      if (any.OxygenTankPressureLoss != null)
      {
        SEAnesthesiaMachineOxygenTankPressureLoss dst = new SEAnesthesiaMachineOxygenTankPressureLoss();
        PBAnesthesiaMachineAction.Load(any.OxygenTankPressureLoss, dst);
        return dst;
      }
      if (any.OxygenWallPortPressureLoss != null)
      {
        SEAnesthesiaMachineOxygenWallPortPressureLoss dst = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
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
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineExpiratoryValveLeak)))
      {
        any.ExpiratoryValveLeak = Unload((SEAnesthesiaMachineExpiratoryValveLeak)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineExpiratoryValveObstruction)))
      {
        any.ExpiratoryValveObstruction = Unload((SEAnesthesiaMachineExpiratoryValveObstruction)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineInspiratoryValveLeak)))
      {
        any.InspiratoryValveLeak = Unload((SEAnesthesiaMachineInspiratoryValveLeak)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineInspiratoryValveObstruction)))
      {
        any.InspiratoryValveObstruction = Unload((SEAnesthesiaMachineInspiratoryValveObstruction)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineMaskLeak)))
      {
        any.MaskLeak = Unload((SEAnesthesiaMachineMaskLeak)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineOxygenTankPressureLoss)))
      {
        any.OxygenTankPressureLoss = Unload((SEAnesthesiaMachineOxygenTankPressureLoss)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineOxygenWallPortPressureLoss)))
      {
        any.OxygenWallPortPressureLoss = Unload((SEAnesthesiaMachineOxygenWallPortPressureLoss)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineSodaLimeFailure)))
      {
        any.SodaLimeFailure = Unload((SEAnesthesiaMachineSodaLimeFailure)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineTubeCuffLeak)))
      {
        any.TubeCuffLeak = Unload((SEAnesthesiaMachineTubeCuffLeak)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineVaporizerFailure)))
      {
        any.VaporizerFailure = Unload((SEAnesthesiaMachineVaporizerFailure)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineVentilatorPressureLoss)))
      {
        any.VentilatorPressureLoss = Unload((SEAnesthesiaMachineVentilatorPressureLoss)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineYPieceDisconnect)))
      {
        any.YPieceDisconnect = Unload((SEAnesthesiaMachineYPieceDisconnect)action);
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
    public static void Load(Cdm.AnesthesiaMachineExpiratoryValveLeakData src, SEAnesthesiaMachineExpiratoryValveLeak dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineExpiratoryValveLeakData src, SEAnesthesiaMachineExpiratoryValveLeak dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineExpiratoryValveLeakData Unload(SEAnesthesiaMachineExpiratoryValveLeak src)
    {
      Cdm.AnesthesiaMachineExpiratoryValveLeakData dst = new Cdm.AnesthesiaMachineExpiratoryValveLeakData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineExpiratoryValveLeak src, Cdm.AnesthesiaMachineExpiratoryValveLeakData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEExpiratoryValveObstruction
    public static void Load(Cdm.AnesthesiaMachineExpiratoryValveObstructionData src, SEAnesthesiaMachineExpiratoryValveObstruction dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineExpiratoryValveObstructionData src, SEAnesthesiaMachineExpiratoryValveObstruction dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineExpiratoryValveObstructionData Unload(SEAnesthesiaMachineExpiratoryValveObstruction src)
    {
      Cdm.AnesthesiaMachineExpiratoryValveObstructionData dst = new Cdm.AnesthesiaMachineExpiratoryValveObstructionData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineExpiratoryValveObstruction src, Cdm.AnesthesiaMachineExpiratoryValveObstructionData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEInspiratoryValveLeak
    public static void Load(Cdm.AnesthesiaMachineInspiratoryValveLeakData src, SEAnesthesiaMachineInspiratoryValveLeak dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineInspiratoryValveLeakData src, SEAnesthesiaMachineInspiratoryValveLeak dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineInspiratoryValveLeakData Unload(SEAnesthesiaMachineInspiratoryValveLeak src)
    {
      Cdm.AnesthesiaMachineInspiratoryValveLeakData dst = new Cdm.AnesthesiaMachineInspiratoryValveLeakData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineInspiratoryValveLeak src, Cdm.AnesthesiaMachineInspiratoryValveLeakData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEInspiratoryValveObstruction
    public static void Load(Cdm.AnesthesiaMachineInspiratoryValveObstructionData src, SEAnesthesiaMachineInspiratoryValveObstruction dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineInspiratoryValveObstructionData src, SEAnesthesiaMachineInspiratoryValveObstruction dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineInspiratoryValveObstructionData Unload(SEAnesthesiaMachineInspiratoryValveObstruction src)
    {
      Cdm.AnesthesiaMachineInspiratoryValveObstructionData dst = new Cdm.AnesthesiaMachineInspiratoryValveObstructionData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineInspiratoryValveObstruction src, Cdm.AnesthesiaMachineInspiratoryValveObstructionData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEMaskLeak
    public static void Load(Cdm.AnesthesiaMachineMaskLeakData src, SEAnesthesiaMachineMaskLeak dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineMaskLeakData src, SEAnesthesiaMachineMaskLeak dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineMaskLeakData Unload(SEAnesthesiaMachineMaskLeak src)
    {
      Cdm.AnesthesiaMachineMaskLeakData dst = new Cdm.AnesthesiaMachineMaskLeakData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineMaskLeak src, Cdm.AnesthesiaMachineMaskLeakData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEOxygenTankPressureLoss
    public static void Load(Cdm.AnesthesiaMachineOxygenTankPressureLossData src, SEAnesthesiaMachineOxygenTankPressureLoss dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineOxygenTankPressureLossData src, SEAnesthesiaMachineOxygenTankPressureLoss dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      dst.SetState((eSwitch)(int)src.State);
    }

    public static Cdm.AnesthesiaMachineOxygenTankPressureLossData Unload(SEAnesthesiaMachineOxygenTankPressureLoss src)
    {
      Cdm.AnesthesiaMachineOxygenTankPressureLossData dst = new Cdm.AnesthesiaMachineOxygenTankPressureLossData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineOxygenTankPressureLoss src, Cdm.AnesthesiaMachineOxygenTankPressureLossData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      dst.State = (Cdm.eSwitch)(int)src.GetState();
    }
    #endregion

    #region SEOxygenWallPortPressureLoss
    public static void Load(Cdm.AnesthesiaMachineOxygenWallPortPressureLossData src, SEAnesthesiaMachineOxygenWallPortPressureLoss dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineOxygenWallPortPressureLossData src, SEAnesthesiaMachineOxygenWallPortPressureLoss dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      dst.SetState((eSwitch)(int)src.State);
    }

    public static Cdm.AnesthesiaMachineOxygenWallPortPressureLossData Unload(SEAnesthesiaMachineOxygenWallPortPressureLoss src)
    {
      Cdm.AnesthesiaMachineOxygenWallPortPressureLossData dst = new Cdm.AnesthesiaMachineOxygenWallPortPressureLossData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineOxygenWallPortPressureLoss src, Cdm.AnesthesiaMachineOxygenWallPortPressureLossData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      dst.State = (Cdm.eSwitch)(int)src.GetState();
    }
    #endregion

    #region SESodaLimeFailure
    public static void Load(Cdm.AnesthesiaMachineSodaLimeFailureData src, SEAnesthesiaMachineSodaLimeFailure dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineSodaLimeFailureData src, SEAnesthesiaMachineSodaLimeFailure dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineSodaLimeFailureData Unload(SEAnesthesiaMachineSodaLimeFailure src)
    {
      Cdm.AnesthesiaMachineSodaLimeFailureData dst = new Cdm.AnesthesiaMachineSodaLimeFailureData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineSodaLimeFailure src, Cdm.AnesthesiaMachineSodaLimeFailureData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SETubeCuffLeak
    public static void Load(Cdm.AnesthesiaMachineTubeCuffLeakData src, SEAnesthesiaMachineTubeCuffLeak dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineTubeCuffLeakData src, SEAnesthesiaMachineTubeCuffLeak dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineTubeCuffLeakData Unload(SEAnesthesiaMachineTubeCuffLeak src)
    {
      Cdm.AnesthesiaMachineTubeCuffLeakData dst = new Cdm.AnesthesiaMachineTubeCuffLeakData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineTubeCuffLeak src, Cdm.AnesthesiaMachineTubeCuffLeakData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEVaporizerFailure
    public static void Load(Cdm.AnesthesiaMachineVaporizerFailureData src, SEAnesthesiaMachineVaporizerFailure dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineVaporizerFailureData src, SEAnesthesiaMachineVaporizerFailure dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineVaporizerFailureData Unload(SEAnesthesiaMachineVaporizerFailure src)
    {
      Cdm.AnesthesiaMachineVaporizerFailureData dst = new Cdm.AnesthesiaMachineVaporizerFailureData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineVaporizerFailure src, Cdm.AnesthesiaMachineVaporizerFailureData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEVentilatorPressureLoss
    public static void Load(Cdm.AnesthesiaMachineVentilatorPressureLossData src, SEAnesthesiaMachineVentilatorPressureLoss dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineVentilatorPressureLossData src, SEAnesthesiaMachineVentilatorPressureLoss dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineVentilatorPressureLossData Unload(SEAnesthesiaMachineVentilatorPressureLoss src)
    {
      Cdm.AnesthesiaMachineVentilatorPressureLossData dst = new Cdm.AnesthesiaMachineVentilatorPressureLossData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineVentilatorPressureLoss src, Cdm.AnesthesiaMachineVentilatorPressureLossData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEYPieceDisconnect
    public static void Load(Cdm.AnesthesiaMachineYPieceDisconnectData src, SEAnesthesiaMachineYPieceDisconnect dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.AnesthesiaMachineYPieceDisconnectData src, SEAnesthesiaMachineYPieceDisconnect dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static Cdm.AnesthesiaMachineYPieceDisconnectData Unload(SEAnesthesiaMachineYPieceDisconnect src)
    {
      Cdm.AnesthesiaMachineYPieceDisconnectData dst = new Cdm.AnesthesiaMachineYPieceDisconnectData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineYPieceDisconnect src, Cdm.AnesthesiaMachineYPieceDisconnectData dst)
    {
      dst.AnesthesiaMachineAction = new Cdm.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion
  }
}
