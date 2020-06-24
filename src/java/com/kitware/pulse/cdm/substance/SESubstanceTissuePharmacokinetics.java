/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.substance;

import com.kitware.pulse.cdm.bind.Substance.SubstanceTissuePharmacokineticsData;
import com.kitware.pulse.cdm.properties.SEScalar;

public class SESubstanceTissuePharmacokinetics 
{
  protected String        name;
  protected SEScalar      partitionCoefficient;
  
  public SESubstanceTissuePharmacokinetics()
  {
    
  }
  
  public void reset()
  {
    this.name=null;
    if(this.partitionCoefficient!=null)
      this.partitionCoefficient.invalidate();  
  }
  
  public static void load(SubstanceTissuePharmacokineticsData src, SESubstanceTissuePharmacokinetics dst)
  {
    if(src.getName()!=null)
      dst.setName(src.getName());
    if(src.hasPartitionCoefficient())
      SEScalar.load(src.getPartitionCoefficient(),dst.getPartitionCoefficient());    
  }
  
  public static SubstanceTissuePharmacokineticsData unload(SESubstanceTissuePharmacokinetics src)
  {
    SubstanceTissuePharmacokineticsData.Builder dst = SubstanceTissuePharmacokineticsData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESubstanceTissuePharmacokinetics src, SubstanceTissuePharmacokineticsData.Builder dst)
  {
    if(src.hasName())
      dst.setName(src.name);    
    if(src.hasPartitionCoefficient())
      dst.setPartitionCoefficient(SEScalar.unload(src.partitionCoefficient));    
  }
  
  public String  getName() { return this.name;}
  public void    setName(String name){this.name=name;}
  public boolean hasName(){return this.name==null?false:!this.name.isEmpty();}
  
  public SEScalar getPartitionCoefficient() 
  { 
    if(this.partitionCoefficient==null)
      this.partitionCoefficient=new SEScalar();
    return this.partitionCoefficient;
  }
  public boolean      hasPartitionCoefficient() {return this.partitionCoefficient==null?false:this.partitionCoefficient.isValid();}
}
