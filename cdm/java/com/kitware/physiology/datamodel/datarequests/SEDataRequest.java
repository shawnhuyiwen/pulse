/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.datarequests;

import java.io.Serializable;

import com.kitware.physiology.cdm.Engine.DataRequestData;
import com.kitware.physiology.cdm.EngineEnums.eDataRequest;
import com.kitware.physiology.cdm.EngineEnums.eDecimalFormat;

public class SEDataRequest implements Serializable
{
  protected String                  propertyName;
  protected String                  unit;
  protected eDataRequest.Category   category;
  protected String                  compartmentName;
  protected String                  substanceName;
  protected Integer                 precision;
  protected eDecimalFormat.Type     format;
  
  public SEDataRequest() 
  {
    reset();
  }
  
  public void reset()
  {
    propertyName    = "";
    unit            = "";
    category        = null;
    compartmentName = "";
    substanceName   = "";
    precision       = null;
    format          = null;
  }
  
  public static void load(DataRequestData src, SEDataRequest dst)
  {
    dst.reset();    
    dst.propertyName = src.getPropertyName();
    dst.unit = src.getUnit();
    if(src.getCategory()!=eDataRequest.Category.UNRECOGNIZED)
    	dst.category = src.getCategory();
    dst.compartmentName = src.getCompartmentName();
    dst.substanceName = src.getSubstanceName();
    if(src.hasDecimalFormat())
    {
      dst.format = src.getDecimalFormat().getType();      
      dst.precision = src.getDecimalFormat().getPrecision();
    }
  }
  public static DataRequestData unload(SEDataRequest src)
  {
    DataRequestData.Builder dst = DataRequestData.newBuilder();
    unload(src,dst);
    return dst.build();    
  }
  protected static void unload(SEDataRequest src, DataRequestData.Builder dst)
  {
    if(src.hasPropertyName())
      dst.setPropertyName(src.propertyName);
    if(src.hasUnit())
      dst.setUnit(src.unit);
    if(src.hasCategory())
      dst.setCategory(src.category);
    if(src.hasCompartmentName())
      dst.setCompartmentName(src.compartmentName);
    if(src.hasSubstanceName())
      dst.setSubstanceName(src.substanceName);
    if(src.hasFormat())
      dst.getDecimalFormatBuilder().setType(src.format);
    if(src.hasPrecision())
      dst.getDecimalFormatBuilder().setPrecision(src.precision);    
  }
  
  public int hashCode()
  {
    int c = 17;
    if(propertyName!=null)
    c = 31 * c + propertyName.hashCode();
    if(unit!=null)
      c = 31 * c + unit.hashCode();
    if(category!=null)
      c = 31 * c + category.hashCode();
    if(compartmentName!=null)
      c = 31 * c + compartmentName.hashCode();
    if(substanceName!=null)
      c = 31 * c + substanceName.hashCode();
    if(precision!=null)
      c = 31 * c + (int)(precision ^ (precision >>> 32));
    if(format!=null)
      c = 31 * c + format.hashCode();    
    return c;
  }
  
  public String getPropertyName(){ return propertyName; }
  public void setPropertyName(String name){ this.propertyName = name; }
  public boolean hasPropertyName() { return propertyName==null||propertyName.isEmpty() ? false : true; }
  
  public String getUnit(){ return unit; }
  public void setUnit(String unit){ this.unit = unit; }
  public boolean hasUnit(){ return unit==null||unit.isEmpty() ? false : true; }
  
  public eDataRequest.Category getCategory(){ return category; }
  public void setCategory(eDataRequest.Category c){ this.category = c; }
  public boolean hasCategory(){ return category==null||category==eDataRequest.Category.UNRECOGNIZED ? false : true; }
  
  public String getCompartmentName(){ return compartmentName; }
  public void setCompartmentName(String c){ this.compartmentName = c; }
  public boolean hasCompartmentName(){ return compartmentName==null||compartmentName.isEmpty() ? false : true; }
  
  public String getSubstanceName(){ return substanceName; }
  public void setSubstanceName(String s){ this.substanceName = s; }
  public boolean hasSubstanceName(){ return substanceName==null||substanceName.isEmpty() ? false : true; }
  
  public eDecimalFormat.Type getFormat(){ return format; }
  public void setFormat(eDecimalFormat.Type f){ this.format = f; }
  public boolean hasFormat(){ return format==null||format==eDecimalFormat.Type.UNRECOGNIZED ? false : true; }
  
  public Integer getPrecision(){ return precision; }
  public void setPrecision(Integer p){ this.precision = p; }
  public boolean hasPrecision(){ return precision==null ? false : true; }
  
}
