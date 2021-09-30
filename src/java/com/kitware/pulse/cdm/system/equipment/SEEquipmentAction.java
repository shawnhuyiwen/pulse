/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment;

import com.kitware.pulse.cdm.actions.SEAction;
import com.kitware.pulse.cdm.bind.Actions.EquipmentActionData;
import com.kitware.pulse.cdm.bind.EquipmentActions.AnyEquipmentActionData;
import com.kitware.pulse.cdm.system.equipment.anesthesia_machine.actions.*;
import com.kitware.pulse.cdm.system.equipment.bag_valve_mask.actions.*;
import com.kitware.pulse.cdm.system.equipment.inhaler.actions.*;
import com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions.*;
import com.kitware.pulse.utilities.Log;

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
  public void clear()
  {
    super.clear();
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
    case BAGVALVEMASKCONFIGURATION:
    {
      SEBagValveMaskConfiguration dst = new SEBagValveMaskConfiguration();
      SEBagValveMaskConfiguration.load(c.getBagValveMaskConfiguration(), dst);
      return dst;
    }
    case BAGVALVEMASKAUTOMATED:
    {
      SEBagValveMaskAutomated dst = new SEBagValveMaskAutomated();
      SEBagValveMaskAutomated.load(c.getBagValveMaskAutomated(), dst);
      return dst;
    }
    case BAGVALVEMASKINSTANTANEOUS:
    {
      SEBagValveMaskInstantaneous dst = new SEBagValveMaskInstantaneous();
      SEBagValveMaskInstantaneous.load(c.getBagValveMaskInstantaneous(), dst);
      return dst;
    }
    case BAGVALVEMASKSQUEEZE:
    {
      SEBagValveMaskSqueeze dst = new SEBagValveMaskSqueeze();
      SEBagValveMaskSqueeze.load(c.getBagValveMaskSqueeze(), dst);
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
    case MECHANICALVENTILATORCONTINUOUSPOSITIVEAIRWAYPRESSURE:
    {
      SEMechanicalVentilatorContinuousPositiveAirwayPressure dst = new SEMechanicalVentilatorContinuousPositiveAirwayPressure();
      SEMechanicalVentilatorContinuousPositiveAirwayPressure.load(c.getMechanicalVentilatorContinuousPositiveAirwayPressure(), dst);
    }
    case MECHANICALVENTILATORHOLD:
    {
      SEMechanicalVentilatorHold dst = new SEMechanicalVentilatorHold();
      SEMechanicalVentilatorHold.load(c.getMechanicalVentilatorHold(), dst);
    }
    case MECHANICALVENTILATORLEAK:
    {
      SEMechanicalVentilatorLeak dst = new SEMechanicalVentilatorLeak();
      SEMechanicalVentilatorLeak.load(c.getMechanicalVentilatorLeak(), dst);
    }
    case MECHANICALVENTILATORPRESSURECONTROL:
    {
      SEMechanicalVentilatorPressureControl dst = new SEMechanicalVentilatorPressureControl();
      SEMechanicalVentilatorPressureControl.load(c.getMechanicalVentilatorPressureControl(), dst);
    }
    case MECHANICALVENTILATORVOLUMECONTROL:
    {
      SEMechanicalVentilatorVolumeControl dst = new SEMechanicalVentilatorVolumeControl();
      SEMechanicalVentilatorVolumeControl.load(c.getMechanicalVentilatorVolumeControl(), dst);
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
    
    if(c instanceof SEAnesthesiaMachineAction)
    {
      if(c instanceof SEAnesthesiaMachineConfiguration)
      {
        dst.setAnesthesiaMachineConfiguration(SEAnesthesiaMachineConfiguration.unload((SEAnesthesiaMachineConfiguration)c));
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
      return null;
    }
    
    if (c instanceof SEBagValveMaskAction)
    {
      if (c instanceof SEBagValveMaskConfiguration)
      {
        dst.setBagValveMaskConfiguration(SEBagValveMaskConfiguration.unload((SEBagValveMaskConfiguration) c));
        return dst.build();
      }
      if (c instanceof SEBagValveMaskAutomated)
      {
        dst.setBagValveMaskAutomated(SEBagValveMaskAutomated.unload((SEBagValveMaskAutomated) c));
        return dst.build();
      }
      if (c instanceof SEBagValveMaskInstantaneous)
      {
        dst.setBagValveMaskInstantaneous(SEBagValveMaskInstantaneous.unload((SEBagValveMaskInstantaneous) c));
        return dst.build();
      }
      if (c instanceof SEBagValveMaskSqueeze)
      {
        dst.setBagValveMaskSqueeze(SEBagValveMaskSqueeze.unload((SEBagValveMaskSqueeze) c));
        return dst.build();
      }
      Log.error("Unsupported Bag Valve Mask Action type " + c);
      return null;
    }

    if (c instanceof SEInhalerAction)
    {
      if (c instanceof SEInhalerConfiguration)
      {
        dst.setInhalerConfiguration(SEInhalerConfiguration.unload((SEInhalerConfiguration) c));
        return dst.build();
      }
      Log.error("Unsupported Inhaler Action type " + c);
      return null;
    }

    if (c instanceof SEMechanicalVentilatorAction)
    {
      if (c instanceof SEMechanicalVentilatorConfiguration)
      {
        dst.setMechanicalVentilatorConfiguration(
            SEMechanicalVentilatorConfiguration.unload((SEMechanicalVentilatorConfiguration) c));
        return dst.build();
      }
      if (c instanceof SEMechanicalVentilatorHold)
      {
        dst.setMechanicalVentilatorHold(
            SEMechanicalVentilatorHold.unload((SEMechanicalVentilatorHold) c));
        return dst.build();
      }
      if (c instanceof SEMechanicalVentilatorLeak)
      {
        dst.setMechanicalVentilatorLeak(
            SEMechanicalVentilatorLeak.unload((SEMechanicalVentilatorLeak) c));
        return dst.build();
      }
      if (c instanceof SEMechanicalVentilatorContinuousPositiveAirwayPressure)
      {
        dst.setMechanicalVentilatorContinuousPositiveAirwayPressure(
            SEMechanicalVentilatorContinuousPositiveAirwayPressure.unload((SEMechanicalVentilatorContinuousPositiveAirwayPressure) c));
        return dst.build();
      }
      if (c instanceof SEMechanicalVentilatorPressureControl)
      {
        dst.setMechanicalVentilatorPressureControl(
            SEMechanicalVentilatorPressureControl.unload((SEMechanicalVentilatorPressureControl) c));
        return dst.build();
      }
      if (c instanceof SEMechanicalVentilatorVolumeControl)
      {
        dst.setMechanicalVentilatorVolumeControl(
            SEMechanicalVentilatorVolumeControl.unload((SEMechanicalVentilatorVolumeControl) c));
        return dst.build();
      }
      Log.error("Unsupported Mechanical Ventilator Action type " + c);
      return null;
    }
    
    return dst.build();
  }
  
  @Override
  public abstract String toString();
}
