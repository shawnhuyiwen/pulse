/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.system.equipment.anesthesia.actions;


import org.jfree.util.Log;

import com.kitware.physiology.cdm.AnesthesiaMachineActions.AnesthesiaMachineActionData;
import com.kitware.physiology.cdm.AnesthesiaMachineActions.AnyAnesthesiaMachineActionData;

import com.kitware.physiology.datamodel.actions.SEAction;
import com.kitware.physiology.datamodel.substance.SESubstanceManager;
import com.kitware.physiology.datamodel.system.environment.actions.SEThermalApplication;

public abstract class SEAnesthesiaMachineAction extends SEAction
{
  public SEAnesthesiaMachineAction()
  {
    
  }
  
  public void copy(SEAnesthesiaMachineAction other)
  {
    if (this == other)
      return;
    super.copy(other);
  }
  
  public void reset()
  {
    super.reset();
  }
  
  public static void load(AnesthesiaMachineActionData src, SEAnesthesiaMachineAction dst) 
  {
    SEAction.load(src.getAction(), dst);
  }
  protected static void unload(SEAnesthesiaMachineAction src, AnesthesiaMachineActionData.Builder dst)
  {
    SEAction.unload(src, dst.getActionBuilder());
  }
  
  public static SEAnesthesiaMachineAction ANY2CDM(AnyAnesthesiaMachineActionData c, SESubstanceManager subMgr) 
  {
    switch(c.getActionCase())
    {
    case CONFIGURATION:
    {
      SEAnesthesiaMachineConfiguration dst = new SEAnesthesiaMachineConfiguration();
      SEAnesthesiaMachineConfiguration.load(c.getConfiguration(), dst, subMgr);
      return dst;
    }
    case EXPIRATORYVALVELEAK:
    {
      SEAnesthesiaMachineExpiratoryValveLeak dst = new SEAnesthesiaMachineExpiratoryValveLeak();
      SEAnesthesiaMachineExpiratoryValveLeak.load(c.getExpiratoryValveLeak(), dst);
      return dst;
    }
    case EXPIRATORYVALVEOBSTRUCTION:
    {
      SEAnesthesiaMachineExpiratoryValveObstruction dst = new SEAnesthesiaMachineExpiratoryValveObstruction();
      SEAnesthesiaMachineExpiratoryValveObstruction.load(c.getExpiratoryValveObstruction(), dst);
      return dst;
    }
    case INSPIRATORYVALVELEAK:
    {
      SEAnesthesiaMachineInspiratoryValveLeak dst = new SEAnesthesiaMachineInspiratoryValveLeak();
      SEAnesthesiaMachineInspiratoryValveLeak.load(c.getInspiratoryValveLeak(), dst);
      return dst;
    }
    case INSPIRATORYVALVEOBSTRUCTION:
    {
      SEAnesthesiaMachineInspiratoryValveObstruction dst = new SEAnesthesiaMachineInspiratoryValveObstruction();
      SEAnesthesiaMachineInspiratoryValveObstruction.load(c.getInspiratoryValveObstruction(), dst);
      return dst;
    }
    case MASKLEAK:
    {
      SEAnesthesiaMachineMaskLeak dst = new SEAnesthesiaMachineMaskLeak();
      SEAnesthesiaMachineMaskLeak.load(c.getMaskLeak(), dst);
      return dst;
    }
    case SODALIMEFAILURE:
    {
      SEAnesthesiaMachineSodaLimeFailure dst = new SEAnesthesiaMachineSodaLimeFailure();
      SEAnesthesiaMachineSodaLimeFailure.load(c.getSodaLimeFailure(), dst);
      return dst;
    }
    case TUBECUFFLEAK:
    {
      SEAnesthesiaMachineTubeCuffLeak dst = new SEAnesthesiaMachineTubeCuffLeak();
      SEAnesthesiaMachineTubeCuffLeak.load(c.getTubeCuffLeak(), dst);
      return dst;
    }
    case VAPORIZERFAILURE:
    {
      SEAnesthesiaMachineVaporizerFailure dst = new SEAnesthesiaMachineVaporizerFailure();
      SEAnesthesiaMachineVaporizerFailure.load(c.getVaporizerFailure(), dst);
      return dst;
    }
    case VENTILATORPRESSURELOSS:
    {
      SEAnesthesiaMachineVentilatorPressureLoss dst = new SEAnesthesiaMachineVentilatorPressureLoss();
      SEAnesthesiaMachineVentilatorPressureLoss.load(c.getVentilatorPressureLoss(), dst);
      return dst;
    }
    case YPIECEDISCONNECT:
    {
      SEAnesthesiaMachineYPieceDisconnect dst = new SEAnesthesiaMachineYPieceDisconnect();
      SEAnesthesiaMachineYPieceDisconnect.load(c.getYPieceDisconnect(), dst);
      return dst;
    }
    case OXYGENTANKPRESSURELOSS:
    {
      SEAnesthesiaMachineOxygenTankPressureLoss dst = new SEAnesthesiaMachineOxygenTankPressureLoss();
      SEAnesthesiaMachineOxygenTankPressureLoss.load(c.getOxygenTankPressureLoss(), dst);
      return dst;
    }
    case OXYGENWALLPORTPRESSURELOSS:
    {
      SEAnesthesiaMachineOxygenWallPortPressureLoss dst = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
      SEAnesthesiaMachineOxygenWallPortPressureLoss.load(c.getOxygenWallPortPressureLoss(), dst);
      return dst;
    }
    case ACTION_NOT_SET:
      Log.warn("AnyAnesthesiaMachineActionData was empty...was that intended?");
      return null;
    }
    Log.error("Unsupported AnesthesiaMachine Action type "+c.getActionCase());
    return null;
  }
  public static AnyAnesthesiaMachineActionData CDM2ANY(SEAnesthesiaMachineAction c)
  {
    AnyAnesthesiaMachineActionData.Builder dst = AnyAnesthesiaMachineActionData.newBuilder();
    if(c instanceof SEAnesthesiaMachineConfiguration)
    {
      dst.setConfiguration(SEAnesthesiaMachineConfiguration.unload((SEAnesthesiaMachineConfiguration)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineExpiratoryValveLeak)
    {
      dst.setExpiratoryValveLeak(SEAnesthesiaMachineExpiratoryValveLeak.unload((SEAnesthesiaMachineExpiratoryValveLeak)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineExpiratoryValveObstruction)
    {
      dst.setExpiratoryValveObstruction(SEAnesthesiaMachineExpiratoryValveObstruction.unload((SEAnesthesiaMachineExpiratoryValveObstruction)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineInspiratoryValveLeak)
    {
      dst.setInspiratoryValveLeak(SEAnesthesiaMachineInspiratoryValveLeak.unload((SEAnesthesiaMachineInspiratoryValveLeak)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineInspiratoryValveObstruction)
    {
      dst.setInspiratoryValveObstruction(SEAnesthesiaMachineInspiratoryValveObstruction.unload((SEAnesthesiaMachineInspiratoryValveObstruction)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineMaskLeak)
    {
      dst.setMaskLeak(SEAnesthesiaMachineMaskLeak.unload((SEAnesthesiaMachineMaskLeak)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineSodaLimeFailure)
    {
      dst.setSodaLimeFailure(SEAnesthesiaMachineSodaLimeFailure.unload((SEAnesthesiaMachineSodaLimeFailure)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineTubeCuffLeak)
    {
      dst.setTubeCuffLeak(SEAnesthesiaMachineTubeCuffLeak.unload((SEAnesthesiaMachineTubeCuffLeak)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineVaporizerFailure)
    {
      dst.setVaporizerFailure(SEAnesthesiaMachineVaporizerFailure.unload((SEAnesthesiaMachineVaporizerFailure)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineVentilatorPressureLoss)
    {
      dst.setVentilatorPressureLoss(SEAnesthesiaMachineVentilatorPressureLoss.unload((SEAnesthesiaMachineVentilatorPressureLoss)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineYPieceDisconnect)
    {
      dst.setYPieceDisconnect(SEAnesthesiaMachineYPieceDisconnect.unload((SEAnesthesiaMachineYPieceDisconnect)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineOxygenTankPressureLoss)
    {
      dst.setOxygenTankPressureLoss(SEAnesthesiaMachineOxygenTankPressureLoss.unload((SEAnesthesiaMachineOxygenTankPressureLoss)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineOxygenWallPortPressureLoss)
    {
      dst.setOxygenWallPortPressureLoss(SEAnesthesiaMachineOxygenWallPortPressureLoss.unload((SEAnesthesiaMachineOxygenWallPortPressureLoss)c));
      return dst.build();
    }
    Log.error("Unsupported AnesthesiaMachine Action type "+c);
    return dst.build();
  }
  
  public abstract String toString();
}
