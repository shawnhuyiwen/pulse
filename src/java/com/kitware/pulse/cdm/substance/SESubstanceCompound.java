/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.substance;

import java.util.ArrayList;
import java.util.List;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.cdm.bind.Substance.SubstanceCompoundData;
import com.kitware.pulse.cdm.bind.Substance.SubstanceConcentrationData;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.StringUtils;

public class SESubstanceCompound
{
  protected String                         name;
  protected List<SESubstanceConcentration> components;
  
  public SESubstanceCompound()
  {
    
  }
  
  public void reset()
  {
    this.name=null;
    
    if(this.components!=null)
      this.components.clear();
  }
  
  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    SubstanceCompoundData.Builder builder = SubstanceCompoundData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SESubstanceCompound.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SESubstanceCompound.unload(this)));
  }
  
  public static void load(SubstanceCompoundData src, SESubstanceCompound dst)
  {
    dst.reset();
    if(src.getName()!=null)
      dst.setName(src.getName());

    if(src.getComponentList()!=null)
    {      
      for(SubstanceConcentrationData cData : src.getComponentList())
      {
        SESubstanceConcentration.load(cData,dst.createComponent(cData.getName()));
      }
    }
  } 
  public static SubstanceCompoundData unload(SESubstanceCompound src)
  {
    SubstanceCompoundData.Builder dst = SubstanceCompoundData.newBuilder();
    SESubstanceCompound.unload(src,dst);
    return dst.build();
  }
  protected static void unload(SESubstanceCompound src, SubstanceCompoundData.Builder dst)
  {
    if(src.hasName())
      dst.setName(src.name);
    
    if(src.hasComponents())
    {
      for(SESubstanceConcentration c : src.getComponents())
        dst.addComponent(SESubstanceConcentration.unload(c));
    }
  }
  
  public String  getName() { return this.name;}
  public void    setName(String name){this.name=name;}
  public boolean hasName(){return StringUtils.exists(this.name);}
  
  public SESubstanceConcentration createComponent(String s) 
  { 
    return getComponent(s);
  }  
  public List<SESubstanceConcentration> getComponents() 
  { 
    if(this.components==null)
      this.components=new ArrayList<>();
    return this.components;
  }  
  public boolean hasComponents() {return this.components==null?false:this.components.size()>0;}
  public boolean hasComponent(String s) 
  {
    if(this.components==null)
      return false;
    for(SESubstanceConcentration c : this.components)
    {
      if(c.getSubstance()==s)
        return true;
    }
    return false;
  }
  public SESubstanceConcentration getComponent(String s) 
  {    
    for(SESubstanceConcentration c : getComponents())
    {
      if(c.getSubstance()==s)
        return c;
    }
    SESubstanceConcentration c=new SESubstanceConcentration(s);
    this.getComponents().add(c);
    return c;
  }
}
