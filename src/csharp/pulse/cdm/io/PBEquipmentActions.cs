/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBEquipmentAction
  {
    #region Any Equipment Action

    public static SEEquipmentAction Load(pulse.cdm.bind.AnyEquipmentActionData any)
    {
      if (any.AnesthesiaMachineConfiguration != null)
      {
        SEAnesthesiaMachineConfiguration dst = new SEAnesthesiaMachineConfiguration();
        PBEquipmentAction.Load(any.AnesthesiaMachineConfiguration, dst/*, subMgr*/);
        return dst;
      }
      if (any.MechanicalVentilatorConfiguration != null)
      {
        SEMechanicalVentilatorConfiguration dst = new SEMechanicalVentilatorConfiguration();
        PBEquipmentAction.Load(any.MechanicalVentilatorConfiguration, dst/*, subMgr*/);
        return dst;
      }

      if (any.AnesthesiaMachineExpiratoryValveLeak != null)
      {
        SEAnesthesiaMachineExpiratoryValveLeak dst = new SEAnesthesiaMachineExpiratoryValveLeak();
        PBEquipmentAction.Load(any.AnesthesiaMachineExpiratoryValveLeak, dst);
        return dst;
      }
      if (any.AnesthesiaMachineExpiratoryValveObstruction != null)
      {
        SEAnesthesiaMachineExpiratoryValveObstruction dst = new SEAnesthesiaMachineExpiratoryValveObstruction();
        PBEquipmentAction.Load(any.AnesthesiaMachineExpiratoryValveObstruction, dst);
        return dst;
      }
      if (any.AnesthesiaMachineInspiratoryValveLeak != null)
      {
        SEAnesthesiaMachineInspiratoryValveLeak dst = new SEAnesthesiaMachineInspiratoryValveLeak();
        PBEquipmentAction.Load(any.AnesthesiaMachineInspiratoryValveLeak, dst);
        return dst;
      }
      if (any.AnesthesiaMachineInspiratoryValveObstruction != null)
      {
        SEAnesthesiaMachineInspiratoryValveObstruction dst = new SEAnesthesiaMachineInspiratoryValveObstruction();
        PBEquipmentAction.Load(any.AnesthesiaMachineInspiratoryValveObstruction, dst);
        return dst;
      }
      if (any.AnesthesiaMachineMaskLeak != null)
      {
        SEAnesthesiaMachineMaskLeak dst = new SEAnesthesiaMachineMaskLeak();
        PBEquipmentAction.Load(any.AnesthesiaMachineMaskLeak, dst);
        return dst;
      }
      if (any.AnesthesiaMachineSodaLimeFailure != null)
      {
        SEAnesthesiaMachineSodaLimeFailure dst = new SEAnesthesiaMachineSodaLimeFailure();
        PBEquipmentAction.Load(any.AnesthesiaMachineSodaLimeFailure, dst);
        return dst;
      }
      if (any.AnesthesiaMachineTubeCuffLeak != null)
      {
        SEAnesthesiaMachineTubeCuffLeak dst = new SEAnesthesiaMachineTubeCuffLeak();
        PBEquipmentAction.Load(any.AnesthesiaMachineTubeCuffLeak, dst);
        return dst;
      }
      if (any.AnesthesiaMachineVaporizerFailure != null)
      {
        SEAnesthesiaMachineVaporizerFailure dst = new SEAnesthesiaMachineVaporizerFailure();
        PBEquipmentAction.Load(any.AnesthesiaMachineVaporizerFailure, dst);
        return dst;
      }
      if (any.AnesthesiaMachineVentilatorPressureLoss != null)
      {
        SEAnesthesiaMachineVentilatorPressureLoss dst = new SEAnesthesiaMachineVentilatorPressureLoss();
        PBEquipmentAction.Load(any.AnesthesiaMachineVentilatorPressureLoss, dst);
        return dst;
      }
      if (any.AnesthesiaMachineYPieceDisconnect != null)
      {
        SEAnesthesiaMachineYPieceDisconnect dst = new SEAnesthesiaMachineYPieceDisconnect();
        PBEquipmentAction.Load(any.AnesthesiaMachineYPieceDisconnect, dst);
        return dst;
      }
      if (any.AnesthesiaMachineOxygenTankPressureLoss != null)
      {
        SEAnesthesiaMachineOxygenTankPressureLoss dst = new SEAnesthesiaMachineOxygenTankPressureLoss();
        PBEquipmentAction.Load(any.AnesthesiaMachineOxygenTankPressureLoss, dst);
        return dst;
      }
      if (any.AnesthesiaMachineOxygenWallPortPressureLoss != null)
      {
        SEAnesthesiaMachineOxygenWallPortPressureLoss dst = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
        PBEquipmentAction.Load(any.AnesthesiaMachineOxygenWallPortPressureLoss, dst);
        return dst;
      }

      //Log.error("Unknown AnesthesiaMachine Action");
      return null;
    }

    public static pulse.cdm.bind.AnyEquipmentActionData Unload(SEEquipmentAction action)
    {
      pulse.cdm.bind.AnyEquipmentActionData any = new pulse.cdm.bind.AnyEquipmentActionData();

      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineConfiguration)))
      {
        any.AnesthesiaMachineConfiguration = Unload((SEAnesthesiaMachineConfiguration)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEMechanicalVentilatorConfiguration)))
      {
        any.MechanicalVentilatorConfiguration = Unload((SEMechanicalVentilatorConfiguration)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineExpiratoryValveLeak)))
      {
        any.AnesthesiaMachineExpiratoryValveLeak = Unload((SEAnesthesiaMachineExpiratoryValveLeak)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineExpiratoryValveObstruction)))
      {
        any.AnesthesiaMachineExpiratoryValveObstruction = Unload((SEAnesthesiaMachineExpiratoryValveObstruction)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineInspiratoryValveLeak)))
      {
        any.AnesthesiaMachineInspiratoryValveLeak = Unload((SEAnesthesiaMachineInspiratoryValveLeak)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineInspiratoryValveObstruction)))
      {
        any.AnesthesiaMachineInspiratoryValveObstruction = Unload((SEAnesthesiaMachineInspiratoryValveObstruction)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineMaskLeak)))
      {
        any.AnesthesiaMachineMaskLeak = Unload((SEAnesthesiaMachineMaskLeak)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineOxygenTankPressureLoss)))
      {
        any.AnesthesiaMachineOxygenTankPressureLoss = Unload((SEAnesthesiaMachineOxygenTankPressureLoss)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineOxygenWallPortPressureLoss)))
      {
        any.AnesthesiaMachineOxygenWallPortPressureLoss = Unload((SEAnesthesiaMachineOxygenWallPortPressureLoss)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineSodaLimeFailure)))
      {
        any.AnesthesiaMachineSodaLimeFailure = Unload((SEAnesthesiaMachineSodaLimeFailure)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineTubeCuffLeak)))
      {
        any.AnesthesiaMachineTubeCuffLeak = Unload((SEAnesthesiaMachineTubeCuffLeak)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineVaporizerFailure)))
      {
        any.AnesthesiaMachineVaporizerFailure = Unload((SEAnesthesiaMachineVaporizerFailure)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineVentilatorPressureLoss)))
      {
        any.AnesthesiaMachineVentilatorPressureLoss = Unload((SEAnesthesiaMachineVentilatorPressureLoss)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAnesthesiaMachineYPieceDisconnect)))
      {
        any.AnesthesiaMachineYPieceDisconnect = Unload((SEAnesthesiaMachineYPieceDisconnect)action);
        return any;
      }
      //Log.error("Unsupported AnesthesiaMachine Action type " + c);
      return null;
    }

    #endregion

    #region SEEquipmentAction
    public static void Serialize(pulse.cdm.bind.EquipmentActionData src, SEEquipmentAction dst)
    {
      PBAction.Serialize(src.Action, dst);
    }
    protected static void Serialize(SEEquipmentAction src, pulse.cdm.bind.EquipmentActionData dst)
    {
      dst.Action = new pulse.cdm.bind.ActionData();
      PBAction.Serialize(src, dst.Action);
    }
    #endregion

    #region SEAnesthesiaMachineAction
    public static void Serialize(pulse.cdm.bind.AnesthesiaMachineActionData src, SEAnesthesiaMachineAction dst)
    {
      PBEquipmentAction.Serialize(src.EquipmentAction, dst);
    }
    protected static void Serialize(SEAnesthesiaMachineAction src, pulse.cdm.bind.AnesthesiaMachineActionData dst)
    {
      dst.EquipmentAction = new pulse.cdm.bind.EquipmentActionData();
      PBEquipmentAction.Serialize(src, dst.EquipmentAction);
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
      if (src.ConfigurationFile != null)
        dst.SetConfigurationFile(src.ConfigurationFile);
      else if (src.Configuration != null)
        PBAnesthesiaMachine.Load(src.Configuration, dst.GetConfiguration());
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

    #region SEAnesthesiaMachineExpiratoryValveLeak
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

    #region SEAnesthesiaMachineExpiratoryValveObstruction
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

    #region SEAnesthesiaMachineInspiratoryValveLeak
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

    #region SEAnesthesiaMachineInspiratoryValveObstruction
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

    #region SEAnesthesiaMachineMaskLeak
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

    #region SEAnesthesiaMachineOxygenTankPressureLoss
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

    #region SEAnesthesiaMachineOxygenWallPortPressureLoss
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

    #region SEAnesthesiaMachineSodaLimeFailure
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

    #region SEAnesthesiaMachineTubeCuffLeak
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

    #region SEAnesthesiaMachineVaporizerFailure
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

    #region SEAnesthesiaMachineVentilatorPressureLoss
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

    #region SEAnesthesiaMachineYPieceDisconnect
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

    #region SEInhalerAction
    public static void Serialize(pulse.cdm.bind.InhalerActionData src, SEInhalerAction dst)
    {
      PBEquipmentAction.Serialize(src.EquipmentAction, dst);
    }
    protected static void Serialize(SEInhalerAction src, pulse.cdm.bind.InhalerActionData dst)
    {
      dst.EquipmentAction = new pulse.cdm.bind.EquipmentActionData();
      PBEquipmentAction.Serialize(src, dst.EquipmentAction);
    }
    #endregion

    #region SEInhalerConfiguration
    public static void Load(pulse.cdm.bind.InhalerConfigurationData src, SEInhalerConfiguration dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.InhalerConfigurationData src, SEInhalerConfiguration dst)
    {
      Serialize(src.InhalerAction, dst);
      if (src.ConfigurationFile != null)
        dst.SetConfigurationFile(src.ConfigurationFile);
      else if (src.Configuration != null)
        PBInhaler.Load(src.Configuration, dst.GetConfiguration());
    }
    public static pulse.cdm.bind.InhalerConfigurationData Unload(SEInhalerConfiguration src)
    {
      pulse.cdm.bind.InhalerConfigurationData dst = new pulse.cdm.bind.InhalerConfigurationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEInhalerConfiguration src, pulse.cdm.bind.InhalerConfigurationData dst)
    {
      dst.InhalerAction = new pulse.cdm.bind.InhalerActionData();
      Serialize(src, dst.InhalerAction);
      if (src.HasConfigurationFile())
        dst.ConfigurationFile = src.GetConfigurationFile();
      else if (src.HasConfiguration())
        dst.Configuration = PBInhaler.Unload(src.GetConfiguration());
    }
    #endregion

    #region SEMechanicalVentilatorAction
    public static void Serialize(pulse.cdm.bind.MechanicalVentilatorActionData src, SEMechanicalVentilatorAction dst)
    {
      PBEquipmentAction.Serialize(src.EquipmentAction, dst);
    }
    protected static void Serialize(SEMechanicalVentilatorAction src, pulse.cdm.bind.MechanicalVentilatorActionData dst)
    {
      dst.EquipmentAction = new pulse.cdm.bind.EquipmentActionData();
      PBEquipmentAction.Serialize(src, dst.EquipmentAction);
    }
    #endregion

    #region SEMechanicalVentilatorConfiguration
    public static void Load(pulse.cdm.bind.MechanicalVentilatorConfigurationData src, SEMechanicalVentilatorConfiguration dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.MechanicalVentilatorConfigurationData src, SEMechanicalVentilatorConfiguration dst)
    {
      Serialize(src.MechanicalVentilatorAction, dst);
      if (src.ConfigurationFile != null)
        dst.SetConfigurationFile(src.ConfigurationFile);
      else if (src.Configuration != null)
        PBMechanicalVentilator.Load(src.Configuration, dst.GetConfiguration());
    }
    public static pulse.cdm.bind.MechanicalVentilatorConfigurationData Unload(SEMechanicalVentilatorConfiguration src)
    {
      pulse.cdm.bind.MechanicalVentilatorConfigurationData dst = new pulse.cdm.bind.MechanicalVentilatorConfigurationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEMechanicalVentilatorConfiguration src, pulse.cdm.bind.MechanicalVentilatorConfigurationData dst)
    {
      dst.MechanicalVentilatorAction = new pulse.cdm.bind.MechanicalVentilatorActionData();
      Serialize(src, dst.MechanicalVentilatorAction);
      if (src.HasConfigurationFile())
        dst.ConfigurationFile = src.GetConfigurationFile();
      else if (src.HasConfiguration())
        dst.Configuration = PBMechanicalVentilator.Unload(src.GetConfiguration());
    }
    #endregion
  }
}
