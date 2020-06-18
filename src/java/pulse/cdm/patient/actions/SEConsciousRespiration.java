/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import java.util.ArrayList;
import java.util.List;

import pulse.cdm.bind.PatientActions.AnyConsciousRespirationCommandData;
import pulse.cdm.bind.PatientActions.ConsciousRespirationData;

public class SEConsciousRespiration extends SEPatientAction
{
  private static final long serialVersionUID = 846068910422793098L;
  
  protected List<SEConsciousRespirationCommand> commands;
  protected Boolean                             startImmediately;
  
  public SEConsciousRespiration()
  {
    startImmediately = false;
    commands = new ArrayList<SEConsciousRespirationCommand>();
  }
  
  public void copy(SEConsciousRespiration other)
  {
    if (this == other)
      return;
    super.copy(other);
    startImmediately = other.startImmediately;
    if (other.commands != null)
      {
      commands.clear();
        for (SEConsciousRespirationCommand command : other.commands)
        {
            commands.add(command.getCopy());
        }
      }
  }  
  
  @Override
  public void reset()
  {
    super.reset();
    commands.clear();
    startImmediately = false;
  }
  
  @Override
  public boolean isValid()
  {
    if(!hasCommands())
      return false;
    for(SEConsciousRespirationCommand c : this.commands)
      if(!c.isValid())
        return false;
    return true;
  }
  
  public static void load(ConsciousRespirationData src, SEConsciousRespiration dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    dst.startImmediately = src.getStartImmediately();
    for (AnyConsciousRespirationCommandData cmd : src.getCommandList())
    {
      switch(cmd.getCommandCase())
      {
        case FORCEDPAUSE:
        {
          SEForcedPause command = new SEForcedPause();
          SEForcedPause.load(cmd.getForcedPause(),command);
          dst.commands.add(command);
          break;
        }
        case FORCEDEXHALE:
        {
          SEForcedExhale command = new SEForcedExhale();
          SEForcedExhale.load(cmd.getForcedExhale(),command);
          dst.commands.add(command);
          break;
        }
        case FORCEDINHALE:
        {
          SEForcedInhale command = new SEForcedInhale();
          SEForcedInhale.load(cmd.getForcedInhale(),command);
          dst.commands.add(command);
          break;
        }
        case USEINHALER:
        {
          SEUseInhaler command = new SEUseInhaler();
          SEUseInhaler.load(cmd.getUseInhaler(),command);
          dst.commands.add(command);
          break;
        }
        default: // do nothing
      }
    }
  }
  
  public static ConsciousRespirationData unload(SEConsciousRespiration src)
  {
    ConsciousRespirationData.Builder dst = ConsciousRespirationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEConsciousRespiration src, ConsciousRespirationData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    dst.setStartImmediately(src.startImmediately);
    for (SEConsciousRespirationCommand command : src.commands)
    {
      AnyConsciousRespirationCommandData.Builder cmd = dst.addCommandBuilder();
      if (command instanceof SEForcedPause)
      {
        cmd.setForcedPause(SEForcedPause.unload((SEForcedPause)command));
        continue;
      }
      if (command instanceof SEForcedExhale)
      {
        cmd.setForcedExhale(SEForcedExhale.unload((SEForcedExhale)command));
        continue;
      }
      if (command instanceof SEForcedInhale)
      {
        cmd.setForcedInhale(SEForcedInhale.unload((SEForcedInhale)command));
        continue;
      }
      if (command instanceof SEUseInhaler)
      {
        cmd.setUseInhaler(SEUseInhaler.unload((SEUseInhaler)command));
        continue;
      }
    }
  }
    
  public void setStartImmediately(Boolean b) { this.startImmediately=b;}
  public boolean startImmediately() { return startImmediately; }
  
  
  public boolean hasCommands()
  {
    return commands != null && !commands.isEmpty();
  }
  public List<SEConsciousRespirationCommand> getCommands()
  {
    return commands;
  }
  
  @Override
  public String toString()
  {
    String str = "ConsciousRespiration";    
    for (SEConsciousRespirationCommand command : commands)
    {
      str += "\n\t"+command.toString().replaceAll("\n\t", "\n\t\t");
    }
    
    return str;
  }
}
