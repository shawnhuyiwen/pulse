/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.substance;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import com.kitware.pulse.cdm.bind.Substance.SubstancePharmacokineticsData;
import com.kitware.pulse.cdm.bind.Substance.SubstanceTissuePharmacokineticsData;

public class SESubstancePharmacokinetics
{
  protected SESubstancePhysicochemicals physicochemicals;  
  protected Map<String,SESubstanceTissuePharmacokinetics> TissueKinetics = new HashMap<>();
    
  public SESubstancePharmacokinetics()
  {
    
  }
  
  public void reset()
  {
    if(this.physicochemicals!=null)
      this.physicochemicals.reset();
    
    if(this.TissueKinetics!=null)
      this.TissueKinetics.clear();
  }
  
  public boolean isValid()
  {
    if(hasTissueKinetics())
      return true;
    if(hasPhysicochemicals())       
      return true;
    return false;
  }
  
  public static void load(SubstancePharmacokineticsData src, SESubstancePharmacokinetics dst)
  {
    dst.reset();
    if(src.hasPhysicochemicals())
      SESubstancePhysicochemicals.load(src.getPhysicochemicals(), dst.getPhysicochemicals());
    
    if(src.getTissueKineticsList()!=null)
    {      
      for(SubstanceTissuePharmacokineticsData kData : src.getTissueKineticsList())
      {
        SESubstanceTissuePharmacokinetics.load(kData,dst.getTissueKinetics(kData.getName()));
      }
    }
  }
  
  public static SubstancePharmacokineticsData unload(SESubstancePharmacokinetics src)
  {
    if(!src.isValid())
      return null;
    SubstancePharmacokineticsData.Builder dst = SubstancePharmacokineticsData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESubstancePharmacokinetics src, SubstancePharmacokineticsData.Builder dst)
  {
    if(src.hasPhysicochemicals())
      dst.setPhysicochemicals(SESubstancePhysicochemicals.unload(src.physicochemicals));
    
    if(src.hasTissueKinetics())
    {
      for(SESubstanceTissuePharmacokinetics fx : src.getTissueKinetics())
      {
        dst.addTissueKinetics(SESubstanceTissuePharmacokinetics.unload(fx));
      }
    }
  }
  
  public SESubstancePhysicochemicals getPhysicochemicals() 
  { 
    if(this.physicochemicals == null)
      this.physicochemicals = new SESubstancePhysicochemicals();
    return this.physicochemicals;
  }  
  public boolean hasPhysicochemicals() { return this.physicochemicals==null ? false : this.physicochemicals.isValid(); }
  
  
  public Collection<SESubstanceTissuePharmacokinetics> getTissueKinetics() 
  { 
    return this.TissueKinetics.values();
  }  
  public boolean hasTissueKinetics() {return !this.TissueKinetics.isEmpty();}
  public boolean hasTissueKinetics(String name)
  {
    return this.TissueKinetics.containsKey(name);
  }
  public SESubstanceTissuePharmacokinetics getTissueKinetics(String name)
  {
    
    SESubstanceTissuePharmacokinetics tk = this.TissueKinetics.get(name);
    if(tk==null)
    {
      tk = new SESubstanceTissuePharmacokinetics();
      tk.setName(name);
      this.TissueKinetics.put(name,tk);
    }    
    return tk;
  }
}
