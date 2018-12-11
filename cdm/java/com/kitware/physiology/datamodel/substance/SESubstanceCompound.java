/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.substance;

import java.util.ArrayList;
import java.util.List;

import com.google.protobuf.*;
import com.google.protobuf.util.*;
import com.kitware.physiology.cdm.Patient.PatientData;
import com.kitware.physiology.cdm.Substance.SubstanceCompoundData;
import com.kitware.physiology.cdm.Substance.SubstanceConcentrationData;
import com.kitware.physiology.cdm.Substance.SubstanceData;

import com.kitware.physiology.datamodel.patient.SEPatient;
import com.kitware.physiology.utilities.FileUtils;
import com.kitware.physiology.utilities.StringUtils;

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
  
  public void readFile(String fileName, SESubstanceManager mgr) throws InvalidProtocolBufferException
  {
    SubstanceCompoundData.Builder builder = SubstanceCompoundData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SESubstanceCompound.load(builder.build(), this, mgr);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SESubstanceCompound.unload(this)));
  }
  
  public static void load(SubstanceCompoundData src, SESubstanceCompound dst, SESubstanceManager mgr)
  {
    dst.reset();
    if(src.getName()!=null)
      dst.setName(src.getName());

    if(src.getComponentList()!=null)
    {      
      for(SubstanceConcentrationData cData : src.getComponentList())
      {
        SESubstanceConcentration.load(cData,dst.createComponent(mgr.getSubstance(cData.getName())));
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
  
  public SESubstanceConcentration createComponent(SESubstance s) 
  { 
    return getComponent(s);
  }  
  public List<SESubstanceConcentration> getComponents() 
  { 
    if(this.components==null)
      this.components=new ArrayList<SESubstanceConcentration>();
    return this.components;
  }  
  public boolean hasComponents() {return this.components==null?false:this.components.size()>0;}
  public boolean hasComponent(SESubstance s) 
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
  public SESubstanceConcentration getComponent(SESubstance s) 
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
