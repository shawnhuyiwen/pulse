/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBAnesthesiaMachineAction
  {
    #region Any Anesthesia Machine Action

    public static SEAnesthesiaMachineAction Load(pulse.cdm.bind.AnyAnesthesiaMachineActionData any)
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

    public static pulse.cdm.bind.AnyAnesthesiaMachineActionData Unload(SEAnesthesiaMachineAction action)
    {
      pulse.cdm.bind.AnyAnesthesiaMachineActionData any = new pulse.cdm.bind.AnyAnesthesiaMachineActionData();

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
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineActionData src, SEAnesthesiaMachineAction dst)
    {
      PBAction.Serialize(src.Action, dst);
    }
    protected static void Serialize(SEAnesthesiaMachineAction src, pulse.cdm.bind.AnesthesiaMachineActionData dst)
    {
      PBAction.Serialize(src, dst.Action);
    }
    #endregion

    #region SEAnesthesiaMachineConfiguration
    public static void Load(pulse.cdm.bind.AnesthesiaMachineConfigurationData src, SEAnesthesiaMachineConfiguration dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineConfigurationData src, SEAnesthesiaMachineConfiguration dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Configuration != null)
        PBAnesthesiaMachine.Load(src.Configuration, dst.GetConfiguration());
      else if (src.Configuration != null)
        dst.SetConfigurationFile(src.ConfigurationFile);
    }
    public static pulse.cdm.bind.AnesthesiaMachineConfigurationData Unload(SEAnesthesiaMachineConfiguration src)
    {
      pulse.cdm.bind.AnesthesiaMachineConfigurationData dst = new pulse.cdm.bind.AnesthesiaMachineConfigurationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEAnesthesiaMachineConfiguration src, pulse.cdm.bind.AnesthesiaMachineConfigurationData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasConfiguration())
        dst.Configuration = PBAnesthesiaMachine.Unload(src.GetConfiguration());
      else if (src.HasConfigurationFile())
        dst.ConfigurationFile = src.GetConfigurationFile();
    }
    #endregion

    #region SEExpiratoryValveLeak
    public static void Load(pulse.cdm.bind.AnesthesiaMachineExpiratoryValveLeakData src, SEAnesthesiaMachineExpiratoryValveLeak dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineExpiratoryValveLeakData src, SEAnesthesiaMachineExpiratoryValveLeak dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineExpiratoryValveLeakData Unload(SEAnesthesiaMachineExpiratoryValveLeak src)
    {
      pulse.cdm.bind.AnesthesiaMachineExpiratoryValveLeakData dst = new pulse.cdm.bind.AnesthesiaMachineExpiratoryValveLeakData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineExpiratoryValveLeak src, pulse.cdm.bind.AnesthesiaMachineExpiratoryValveLeakData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEExpiratoryValveObstruction
    public static void Load(pulse.cdm.bind.AnesthesiaMachineExpiratoryValveObstructionData src, SEAnesthesiaMachineExpiratoryValveObstruction dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineExpiratoryValveObstructionData src, SEAnesthesiaMachineExpiratoryValveObstruction dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineExpiratoryValveObstructionData Unload(SEAnesthesiaMachineExpiratoryValveObstruction src)
    {
      pulse.cdm.bind.AnesthesiaMachineExpiratoryValveObstructionData dst = new pulse.cdm.bind.AnesthesiaMachineExpiratoryValveObstructionData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineExpiratoryValveObstruction src, pulse.cdm.bind.AnesthesiaMachineExpiratoryValveObstructionData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEInspiratoryValveLeak
    public static void Load(pulse.cdm.bind.AnesthesiaMachineInspiratoryValveLeakData src, SEAnesthesiaMachineInspiratoryValveLeak dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineInspiratoryValveLeakData src, SEAnesthesiaMachineInspiratoryValveLeak dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineInspiratoryValveLeakData Unload(SEAnesthesiaMachineInspiratoryValveLeak src)
    {
      pulse.cdm.bind.AnesthesiaMachineInspiratoryValveLeakData dst = new pulse.cdm.bind.AnesthesiaMachineInspiratoryValveLeakData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineInspiratoryValveLeak src, pulse.cdm.bind.AnesthesiaMachineInspiratoryValveLeakData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEInspiratoryValveObstruction
    public static void Load(pulse.cdm.bind.AnesthesiaMachineInspiratoryValveObstructionData src, SEAnesthesiaMachineInspiratoryValveObstruction dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineInspiratoryValveObstructionData src, SEAnesthesiaMachineInspiratoryValveObstruction dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineInspiratoryValveObstructionData Unload(SEAnesthesiaMachineInspiratoryValveObstruction src)
    {
      pulse.cdm.bind.AnesthesiaMachineInspiratoryValveObstructionData dst = new pulse.cdm.bind.AnesthesiaMachineInspiratoryValveObstructionData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineInspiratoryValveObstruction src, pulse.cdm.bind.AnesthesiaMachineInspiratoryValveObstructionData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEMaskLeak
    public static void Load(pulse.cdm.bind.AnesthesiaMachineMaskLeakData src, SEAnesthesiaMachineMaskLeak dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineMaskLeakData src, SEAnesthesiaMachineMaskLeak dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineMaskLeakData Unload(SEAnesthesiaMachineMaskLeak src)
    {
      pulse.cdm.bind.AnesthesiaMachineMaskLeakData dst = new pulse.cdm.bind.AnesthesiaMachineMaskLeakData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineMaskLeak src, pulse.cdm.bind.AnesthesiaMachineMaskLeakData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEOxygenTankPressureLoss
    public static void Load(pulse.cdm.bind.AnesthesiaMachineOxygenTankPressureLossData src, SEAnesthesiaMachineOxygenTankPressureLoss dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineOxygenTankPressureLossData src, SEAnesthesiaMachineOxygenTankPressureLoss dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      dst.SetState((eSwitch)(int)src.State);
    }

    public static pulse.cdm.bind.AnesthesiaMachineOxygenTankPressureLossData Unload(SEAnesthesiaMachineOxygenTankPressureLoss src)
    {
      pulse.cdm.bind.AnesthesiaMachineOxygenTankPressureLossData dst = new pulse.cdm.bind.AnesthesiaMachineOxygenTankPressureLossData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineOxygenTankPressureLoss src, pulse.cdm.bind.AnesthesiaMachineOxygenTankPressureLossData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      dst.State = (pulse.cdm.bind.eSwitch)(int)src.GetState();
    }
    #endregion

    #region SEOxygenWallPortPressureLoss
    public static void Load(pulse.cdm.bind.AnesthesiaMachineOxygenWallPortPressureLossData src, SEAnesthesiaMachineOxygenWallPortPressureLoss dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineOxygenWallPortPressureLossData src, SEAnesthesiaMachineOxygenWallPortPressureLoss dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      dst.SetState((eSwitch)(int)src.State);
    }

    public static pulse.cdm.bind.AnesthesiaMachineOxygenWallPortPressureLossData Unload(SEAnesthesiaMachineOxygenWallPortPressureLoss src)
    {
      pulse.cdm.bind.AnesthesiaMachineOxygenWallPortPressureLossData dst = new pulse.cdm.bind.AnesthesiaMachineOxygenWallPortPressureLossData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineOxygenWallPortPressureLoss src, pulse.cdm.bind.AnesthesiaMachineOxygenWallPortPressureLossData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      dst.State = (pulse.cdm.bind.eSwitch)(int)src.GetState();
    }
    #endregion

    #region SESodaLimeFailure
    public static void Load(pulse.cdm.bind.AnesthesiaMachineSodaLimeFailureData src, SEAnesthesiaMachineSodaLimeFailure dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineSodaLimeFailureData src, SEAnesthesiaMachineSodaLimeFailure dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineSodaLimeFailureData Unload(SEAnesthesiaMachineSodaLimeFailure src)
    {
      pulse.cdm.bind.AnesthesiaMachineSodaLimeFailureData dst = new pulse.cdm.bind.AnesthesiaMachineSodaLimeFailureData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineSodaLimeFailure src, pulse.cdm.bind.AnesthesiaMachineSodaLimeFailureData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SETubeCuffLeak
    public static void Load(pulse.cdm.bind.AnesthesiaMachineTubeCuffLeakData src, SEAnesthesiaMachineTubeCuffLeak dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineTubeCuffLeakData src, SEAnesthesiaMachineTubeCuffLeak dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineTubeCuffLeakData Unload(SEAnesthesiaMachineTubeCuffLeak src)
    {
      pulse.cdm.bind.AnesthesiaMachineTubeCuffLeakData dst = new pulse.cdm.bind.AnesthesiaMachineTubeCuffLeakData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineTubeCuffLeak src, pulse.cdm.bind.AnesthesiaMachineTubeCuffLeakData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEVaporizerFailure
    public static void Load(pulse.cdm.bind.AnesthesiaMachineVaporizerFailureData src, SEAnesthesiaMachineVaporizerFailure dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineVaporizerFailureData src, SEAnesthesiaMachineVaporizerFailure dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineVaporizerFailureData Unload(SEAnesthesiaMachineVaporizerFailure src)
    {
      pulse.cdm.bind.AnesthesiaMachineVaporizerFailureData dst = new pulse.cdm.bind.AnesthesiaMachineVaporizerFailureData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineVaporizerFailure src, pulse.cdm.bind.AnesthesiaMachineVaporizerFailureData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEVentilatorPressureLoss
    public static void Load(pulse.cdm.bind.AnesthesiaMachineVentilatorPressureLossData src, SEAnesthesiaMachineVentilatorPressureLoss dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineVentilatorPressureLossData src, SEAnesthesiaMachineVentilatorPressureLoss dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineVentilatorPressureLossData Unload(SEAnesthesiaMachineVentilatorPressureLoss src)
    {
      pulse.cdm.bind.AnesthesiaMachineVentilatorPressureLossData dst = new pulse.cdm.bind.AnesthesiaMachineVentilatorPressureLossData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineVentilatorPressureLoss src, pulse.cdm.bind.AnesthesiaMachineVentilatorPressureLossData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEYPieceDisconnect
    public static void Load(pulse.cdm.bind.AnesthesiaMachineYPieceDisconnectData src, SEAnesthesiaMachineYPieceDisconnect dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineYPieceDisconnectData src, SEAnesthesiaMachineYPieceDisconnect dst)
    {
      Serialize(src.AnesthesiaMachineAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }

    public static pulse.cdm.bind.AnesthesiaMachineYPieceDisconnectData Unload(SEAnesthesiaMachineYPieceDisconnect src)
    {
      pulse.cdm.bind.AnesthesiaMachineYPieceDisconnectData dst = new pulse.cdm.bind.AnesthesiaMachineYPieceDisconnectData();
      Serialize(src, dst);
      return dst;
    }
    protected static void Serialize(SEAnesthesiaMachineYPieceDisconnect src, pulse.cdm.bind.AnesthesiaMachineYPieceDisconnectData dst)
    {
      dst.AnesthesiaMachineAction = new pulse.cdm.bind.AnesthesiaMachineActionData();
      Serialize(src, dst.AnesthesiaMachineAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion
  }
}
