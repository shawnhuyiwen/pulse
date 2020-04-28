/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.equipment;


import org.jfree.util.Log;

import pulse.cdm.bind.EquipmentActions.AnyEquipmentActionData;
import pulse.cdm.bind.Actions.EquipmentActionData;

import pulse.cdm.actions.SEAction;
import pulse.cdm.substance.SESubstanceManager;
import pulse.cdm.system.equipment.anesthesia_machine.actions.*;
import pulse.cdm.system.equipment.inhaler.actions.*;
import pulse.cdm.system.equipment.mechanical_ventilator.actions.*;

public abstract class SEEquipmentAction extends SEAction
{
  public SEEquipmentAction()
  {
    
  }
  
  public void copy(SEEquipmentAction other)
  {
    if (this == other)
      return;
    super.copy(other);
  }
  
  public void reset()
  {
    super.reset();
  }
  
  public static void load(EquipmentActionData src, SEEquipmentAction dst) 
  {
    SEAction.load(src.getAction(), dst);
  }
  protected static void unload(SEEquipmentAction src, EquipmentActionData.Builder dst)
  {
    SEAction.unload(src, dst.getActionBuilder());
  }
  
  public static SEEquipmentAction ANY2CDM(AnyEquipmentActionData c, SESubstanceManager subMgr) 
  {
    switch(c.getActionCase())
    {
    case ANESTHESIAMACHINECONFIGURATION:
    {
      SEAnesthesiaMachineConfiguration dst = new SEAnesthesiaMachineConfiguration();
      SEAnesthesiaMachineConfiguration.load(c.getAnesthesiaMachineConfiguration(), dst, subMgr);
      return dst;
    }
    case ANESTHESIAMACHINEEXPIRATORYVALVELEAK:
    {
      SEAnesthesiaMachineExpiratoryValveLeak dst = new SEAnesthesiaMachineExpiratoryValveLeak();
      SEAnesthesiaMachineExpiratoryValveLeak.load(c.getAnesthesiaMachineExpiratoryValveLeak(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEEXPIRATORYVALVEOBSTRUCTION:
    {
      SEAnesthesiaMachineExpiratoryValveObstruction dst = new SEAnesthesiaMachineExpiratoryValveObstruction();
      SEAnesthesiaMachineExpiratoryValveObstruction.load(c.getAnesthesiaMachineExpiratoryValveObstruction(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEINSPIRATORYVALVELEAK:
    {
      SEAnesthesiaMachineInspiratoryValveLeak dst = new SEAnesthesiaMachineInspiratoryValveLeak();
      SEAnesthesiaMachineInspiratoryValveLeak.load(c.getAnesthesiaMachineInspiratoryValveLeak(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEINSPIRATORYVALVEOBSTRUCTION:
    {
      SEAnesthesiaMachineInspiratoryValveObstruction dst = new SEAnesthesiaMachineInspiratoryValveObstruction();
      SEAnesthesiaMachineInspiratoryValveObstruction.load(c.getAnesthesiaMachineInspiratoryValveObstruction(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEMASKLEAK:
    {
      SEAnesthesiaMachineMaskLeak dst = new SEAnesthesiaMachineMaskLeak();
      SEAnesthesiaMachineMaskLeak.load(c.getAnesthesiaMachineMaskLeak(), dst);
      return dst;
    }
    case ANESTHESIAMACHINESODALIMEFAILURE:
    {
      SEAnesthesiaMachineSodaLimeFailure dst = new SEAnesthesiaMachineSodaLimeFailure();
      SEAnesthesiaMachineSodaLimeFailure.load(c.getAnesthesiaMachineSodaLimeFailure(), dst);
      return dst;
    }
    case ANESTHESIAMACHINETUBECUFFLEAK:
    {
      SEAnesthesiaMachineTubeCuffLeak dst = new SEAnesthesiaMachineTubeCuffLeak();
      SEAnesthesiaMachineTubeCuffLeak.load(c.getAnesthesiaMachineTubeCuffLeak(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEVAPORIZERFAILURE:
    {
      SEAnesthesiaMachineVaporizerFailure dst = new SEAnesthesiaMachineVaporizerFailure();
      SEAnesthesiaMachineVaporizerFailure.load(c.getAnesthesiaMachineVaporizerFailure(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEVENTILATORPRESSURELOSS:
    {
      SEAnesthesiaMachineVentilatorPressureLoss dst = new SEAnesthesiaMachineVentilatorPressureLoss();
      SEAnesthesiaMachineVentilatorPressureLoss.load(c.getAnesthesiaMachineVentilatorPressureLoss(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEYPIECEDISCONNECT:
    {
      SEAnesthesiaMachineYPieceDisconnect dst = new SEAnesthesiaMachineYPieceDisconnect();
      SEAnesthesiaMachineYPieceDisconnect.load(c.getAnesthesiaMachineYPieceDisconnect(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEOXYGENTANKPRESSURELOSS:
    {
      SEAnesthesiaMachineOxygenTankPressureLoss dst = new SEAnesthesiaMachineOxygenTankPressureLoss();
      SEAnesthesiaMachineOxygenTankPressureLoss.load(c.getAnesthesiaMachineOxygenTankPressureLoss(), dst);
      return dst;
    }
    case ANESTHESIAMACHINEOXYGENWALLPORTPRESSURELOSS:
    {
      SEAnesthesiaMachineOxygenWallPortPressureLoss dst = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
      SEAnesthesiaMachineOxygenWallPortPressureLoss.load(c.getAnesthesiaMachineOxygenWallPortPressureLoss(), dst);
      return dst;
    }
    case INHALERCONFIGURATION:
    {
      SEInhalerConfiguration dst = new SEInhalerConfiguration();
      SEInhalerConfiguration.load(c.getInhalerConfiguration(), dst, subMgr);
      return dst;
    }
    case MECHANICALVENTILATORCONFIGURATION:
    {
      SEMechanicalVentilatorConfiguration dst = new SEMechanicalVentilatorConfiguration();
      SEMechanicalVentilatorConfiguration.load(c.getMechanicalVentilatorConfiguration(), dst, subMgr);
      return dst;
    }
    case ACTION_NOT_SET:
      Log.warn("AnyEquipmentActionData was empty...was that intended?");
      return null;
    }
    Log.error("Unsupported Equipment Action type "+c.getActionCase());
    return null;
  }
  public static AnyEquipmentActionData CDM2ANY(SEEquipmentAction c)
  {
    AnyEquipmentActionData.Builder dst = AnyEquipmentActionData.newBuilder();
    if(c instanceof SEAnesthesiaMachineConfiguration)
    {
      dst.setAnesthesiaMachineConfiguration(SEAnesthesiaMachineConfiguration.unload((SEAnesthesiaMachineConfiguration)c));
      return dst.build();
    }
    if(c instanceof SEInhalerConfiguration)
    {
      dst.setInhalerConfiguration(SEInhalerConfiguration.unload((SEInhalerConfiguration)c));
      return dst.build();
    }
    if(c instanceof SEMechanicalVentilatorConfiguration)
    {
      dst.setMechanicalVentilatorConfiguration(SEMechanicalVentilatorConfiguration.unload((SEMechanicalVentilatorConfiguration)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineExpiratoryValveLeak)
    {
      dst.setAnesthesiaMachineExpiratoryValveLeak(SEAnesthesiaMachineExpiratoryValveLeak.unload((SEAnesthesiaMachineExpiratoryValveLeak)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineExpiratoryValveObstruction)
    {
      dst.setAnesthesiaMachineExpiratoryValveObstruction(SEAnesthesiaMachineExpiratoryValveObstruction.unload((SEAnesthesiaMachineExpiratoryValveObstruction)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineInspiratoryValveLeak)
    {
      dst.setAnesthesiaMachineInspiratoryValveLeak(SEAnesthesiaMachineInspiratoryValveLeak.unload((SEAnesthesiaMachineInspiratoryValveLeak)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineInspiratoryValveObstruction)
    {
      dst.setAnesthesiaMachineInspiratoryValveObstruction(SEAnesthesiaMachineInspiratoryValveObstruction.unload((SEAnesthesiaMachineInspiratoryValveObstruction)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineMaskLeak)
    {
      dst.setAnesthesiaMachineMaskLeak(SEAnesthesiaMachineMaskLeak.unload((SEAnesthesiaMachineMaskLeak)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineSodaLimeFailure)
    {
      dst.setAnesthesiaMachineSodaLimeFailure(SEAnesthesiaMachineSodaLimeFailure.unload((SEAnesthesiaMachineSodaLimeFailure)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineTubeCuffLeak)
    {
      dst.setAnesthesiaMachineTubeCuffLeak(SEAnesthesiaMachineTubeCuffLeak.unload((SEAnesthesiaMachineTubeCuffLeak)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineVaporizerFailure)
    {
      dst.setAnesthesiaMachineVaporizerFailure(SEAnesthesiaMachineVaporizerFailure.unload((SEAnesthesiaMachineVaporizerFailure)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineVentilatorPressureLoss)
    {
      dst.setAnesthesiaMachineVentilatorPressureLoss(SEAnesthesiaMachineVentilatorPressureLoss.unload((SEAnesthesiaMachineVentilatorPressureLoss)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineYPieceDisconnect)
    {
      dst.setAnesthesiaMachineYPieceDisconnect(SEAnesthesiaMachineYPieceDisconnect.unload((SEAnesthesiaMachineYPieceDisconnect)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineOxygenTankPressureLoss)
    {
      dst.setAnesthesiaMachineOxygenTankPressureLoss(SEAnesthesiaMachineOxygenTankPressureLoss.unload((SEAnesthesiaMachineOxygenTankPressureLoss)c));
      return dst.build();
    }
    if(c instanceof SEAnesthesiaMachineOxygenWallPortPressureLoss)
    {
      dst.setAnesthesiaMachineOxygenWallPortPressureLoss(SEAnesthesiaMachineOxygenWallPortPressureLoss.unload((SEAnesthesiaMachineOxygenWallPortPressureLoss)c));
      return dst.build();
    }
    Log.error("Unsupported AnesthesiaMachine Action type "+c);
    return dst.build();
  }
  
  public abstract String toString();
}
