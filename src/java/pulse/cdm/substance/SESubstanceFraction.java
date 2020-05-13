/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.substance;


import pulse.cdm.bind.Substance.SubstanceData;
import pulse.cdm.bind.Substance.SubstanceFractionData;

import pulse.cdm.properties.*;

public class SESubstanceFraction
{
  protected SEScalar0To1  amount;
  protected SESubstance   substance;
  
  public SESubstanceFraction(SESubstance s)
  {
    if(s==null)
      throw new RuntimeException("Must provide a valid substance");
    this.substance=s;
  }
  
  public void reset()
  {
    if(this.amount!=null)
      this.amount.invalidate();
  }
  
  public void copy(SESubstanceFraction from)
  {
    this.substance=from.substance;
    if(from.hasAmount())
      this.getAmount().set(from.getAmount());
  }
  
  public static void load(SubstanceFractionData src, SESubstanceFraction dst)
  {
    dst.reset();
    if(src.hasAmount())
      SEScalar0To1.load(src.getAmount(), dst.getAmount());
  }
  public static SubstanceFractionData unload(SESubstanceFraction src)
  {
    SubstanceFractionData.Builder dst = SubstanceFractionData.newBuilder();
    SESubstanceFraction.unload(src,dst);
    return dst.build();
  }
  protected static void unload(SESubstanceFraction src, SubstanceFractionData.Builder dst)
  {
    dst.setName(src.substance.getName());
    if(src.hasAmount())
      dst.setAmount(SEScalar0To1.unload(src.amount)); 
  }
  
  public SEScalar0To1 getAmount() 
  { 
    if(this.amount==null)
      this.amount=new SEScalar0To1();
    return this.amount;
  }
  public boolean hasAmount() {return this.amount==null?false:this.amount.isValid();}
  
  public boolean hasSubstance() { return substance != null; }
  public SESubstance getSubstance() 
  { 
    return this.substance;
  }
  
  public String toString()
  {
    if(!hasSubstance())
      return "";
    String str = "Amount of "+this.substance.getName()+": "+(hasAmount()?getAmount():"None");      
    return str;
  }
}
