/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.UrinateData;

public class SEUrinate extends SEPatientAction
{

  private static final long serialVersionUID = -4138221836818938148L;


  public SEUrinate()
  {

  }
  
  public void copy(SEUrinate other)
  {
    if(this==other)
      return;
    super.copy(other);    
  }
  
  public void reset()
  {
    super.reset();
  }
  
  public boolean isValid()
  {
    return true;
  }
  
  public static void load(UrinateData src, SEUrinate dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
  }
  
  public static UrinateData unload(SEUrinate src) 
  {
    UrinateData.Builder dst = UrinateData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEUrinate src, UrinateData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
  }
  
  
  public String toString() 
  {
      return "Urinate";
  }
}
