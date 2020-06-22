/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.substance;

import pulse.cdm.bind.Substance.SubstanceConcentrationData;
import pulse.cdm.properties.SEScalarMassPerVolume;

public class SESubstanceConcentration
{
  protected SEScalarMassPerVolume  concentration;
  protected String                 substance;
  
  public SESubstanceConcentration(String s)
  {
    if(s==null)
      throw new RuntimeException("Must provide a valid substance");
    this.substance=s;
  }
  
  public void reset()
  {
    if(this.concentration!=null)
      this.concentration.invalidate();
  }
  
  public static void load(SubstanceConcentrationData src, SESubstanceConcentration dst)
  {
    dst.reset();
    if(src.hasConcentration())
      SEScalarMassPerVolume.load(src.getConcentration(), dst.getConcentration());
  }
  public static SubstanceConcentrationData unload(SESubstanceConcentration src)
  {
    SubstanceConcentrationData.Builder dst = SubstanceConcentrationData.newBuilder();
    SESubstanceConcentration.unload(src,dst);
    return dst.build();
  }
  protected static void unload(SESubstanceConcentration src, SubstanceConcentrationData.Builder dst)
  {
    dst.setName(src.substance);
    if(src.hasConcentration())
      dst.setConcentration(SEScalarMassPerVolume.unload(src.concentration)); 
  }
  
  public SEScalarMassPerVolume getConcentration() 
  { 
    if(this.concentration==null)
      this.concentration=new SEScalarMassPerVolume();
    return this.concentration;
  }
  public boolean hasConcentration() {return this.concentration==null?false:this.concentration.isValid();}
  
  public boolean hasSubstance() { return substance != null; }
  public String getSubstance() 
  { 
    return this.substance;
  }
  
  @Override
  public String toString()
  {
    if(!hasSubstance())
      return "";
    String str = "Concentration of "+this.substance+": "+(hasConcentration()?getConcentration():"None");      
    return str;
  }
}
