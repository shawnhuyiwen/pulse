/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.equipment;

import pulse.cdm.actions.SEAction;
import pulse.cdm.bind.Actions.EquipmentActionData;
import pulse.cdm.bind.EquipmentActions.AnyEquipmentActionData;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineConfiguration;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineExpiratoryValveLeak;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineExpiratoryValveObstruction;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineInspiratoryValveLeak;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineInspiratoryValveObstruction;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineMaskLeak;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineOxygenTankPressureLoss;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineOxygenWallPortPressureLoss;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineSodaLimeFailure;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineTubeCuffLeak;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineVaporizerFailure;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineVentilatorPressureLoss;
import pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineYPieceDisconnect;
import pulse.cdm.system.equipment.inhaler.actions.SEInhalerConfiguration;
import pulse.cdm.system.equipment.mechanical_ventilator.actions.SEMechanicalVentilatorConfiguration;
import pulse.utilities.Log;

public abstract class SEEquipmentAction extends SEAction
{

  private static final long serialVersionUID = 2322082255225125060L;

  public SEEquipmentAction()
  {
    
  }
  
  public void copy(SEEquipmentAction other)
  {
    if (this == other)
      return;
    super.copy(other);
  }
  
  @Override
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
  
  public static SEEquipmentAction ANY2CDM(AnyEquipmentActionData c) 
  {
    switch(c.getActionCase())
    {
    case ANESTHESIAMACHINECONFIGURATION:
    {
      SEAnesthesiaMachineConfiguration dst = new SEAnesthesiaMachineConfiguration();
      SEAnesthesiaMachineConfiguration.load(c.getAnesthesiaMachineConfiguration(), dst);
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
      SEInhalerConfiguration.load(c.getInhalerConfiguration(), dst);
      return dst;
    }
    case MECHANICALVENTILATORCONFIGURATION:
    {
      SEMechanicalVentilatorConfiguration dst = new SEMechanicalVentilatorConfiguration();
      SEMechanicalVentilatorConfiguration.load(c.getMechanicalVentilatorConfiguration(), dst);
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
  
  @Override
  public abstract String toString();
}
