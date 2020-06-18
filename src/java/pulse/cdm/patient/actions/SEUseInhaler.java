/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.UseInhalerData;

public class SEUseInhaler extends SEConsciousRespirationCommand
{  
  public SEUseInhaler()
  {
  }
  
  public void copy(SEUseInhaler other)
  {
    if(this==other)
      return;
  }
  
  @Override
  public SEUseInhaler getCopy()
  {
    SEUseInhaler command = new SEUseInhaler();
    command.copy(this);
    
    return command;
  }
  
  @Override
  public void reset()
  {
    super.reset();
  }
  
  @Override
  public boolean isValid()
  {
    return true;
  }
  
  public static void load(UseInhalerData src, SEUseInhaler dst)
  {
    //SEConsciousRespirationCommand.load(src.getConsciousRespirationCommand(), dst);
  }
  
  public static UseInhalerData unload(SEUseInhaler src)
  {
    UseInhalerData.Builder dst = UseInhalerData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEUseInhaler src, UseInhalerData.Builder dst)
  {
    //SEConsciousRespirationCommand.unload(src, dst.getConsciousRespirationCommand());
  }
  
  @Override
  public String toString()
  {
    return "Use Inhaler";
  }
}
