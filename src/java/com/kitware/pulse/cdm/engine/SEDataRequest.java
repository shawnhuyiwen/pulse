/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.engine;

import java.io.Serializable;

import com.kitware.pulse.cdm.bind.Engine.DataRequestData;
import com.kitware.pulse.cdm.bind.Engine.DataRequestData.eCategory;
import com.kitware.pulse.cdm.bind.Engine.DecimalFormatData.eType;
import com.kitware.pulse.cdm.properties.CommonUnits;
import com.kitware.pulse.cdm.properties.CommonUnits.Unit;

public class SEDataRequest implements Serializable
{
  private static final long serialVersionUID = -6340908837323396480L;

  protected String                  propertyName;
  protected Unit                    unit;
  protected eCategory               category;
  protected String                  actionName;
  protected String                  compartmentName;
  protected String                  substanceName;
  protected Integer                 precision;
  protected eType                   format;

  public SEDataRequest(eCategory c)
  {
    clear();
    category = c;
  }

  public void clear()
  {
    propertyName    = "";
    unit            = null;
    actionName      = "";
    compartmentName = "";
    substanceName   = "";
    precision       = null;
    format          = null;
  }

  public static void load(DataRequestData src, SEDataRequest dst)
  {
    dst.clear();
    dst.propertyName = src.getPropertyName();
    dst.unit = CommonUnits.getUnit(src.getUnit());
    if(src.getCategory()!=eCategory.UNRECOGNIZED)
    	dst.category = src.getCategory();
    dst.actionName = src.getActionName();
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
      dst.setUnit(src.unit.toString());
    if(src.hasCategory())
      dst.setCategory(src.category);
    if(src.hasActionName())
      dst.setActionName(src.actionName);
    if(src.hasCompartmentName())
      dst.setCompartmentName(src.compartmentName);
    if(src.hasSubstanceName())
      dst.setSubstanceName(src.substanceName);
    if(src.hasFormat())
      dst.getDecimalFormatBuilder().setType(src.format);
    if(src.hasPrecision())
      dst.getDecimalFormatBuilder().setPrecision(src.precision);
  }

  @Override
  public int hashCode()
  {
    int c = 17;
    if(propertyName!=null)
    c = 31 * c + propertyName.hashCode();
    if(unit!=null)
      c = 31 * c + unit.hashCode();
    if(category!=null)
      c = 31 * c + category.hashCode();
    if(actionName!=null)
      c = 31 * c + actionName.hashCode();
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

  public Unit getUnit(){ return unit; }
  public void setUnit(Unit unit){ this.unit = unit; }
  public boolean hasUnit(){ return unit==null ? false : true; }

  public eCategory getCategory(){ return category; }
  //public void setCategory(eCategory c){ this.category = c; }
  public boolean hasCategory(){ return category==null||category==eCategory.UNRECOGNIZED ? false : true; }

  public String getActionName(){ return actionName; }
  public void setActionName(String a){ this.actionName = a; }
  public boolean hasActionName(){ return actionName==null||actionName.isEmpty() ? false : true; }

  public String getCompartmentName(){ return compartmentName; }
  public void setCompartmentName(String c){ this.compartmentName = c; }
  public void setCompartmentName(Enum<?> c){ this.compartmentName = c.name(); }
  public boolean hasCompartmentName(){ return compartmentName==null||compartmentName.isEmpty() ? false : true; }

  public String getSubstanceName(){ return substanceName; }
  public void setSubstanceName(String s){ this.substanceName = s; }
  public boolean hasSubstanceName(){ return substanceName==null||substanceName.isEmpty() ? false : true; }

  public eType getFormat(){ return format; }
  public void setFormat(eType f){ this.format = f; }
  public boolean hasFormat(){ return format==null||format==eType.UNRECOGNIZED ? false : true; }

  public Integer getPrecision(){ return precision; }
  public void setPrecision(Integer p){ this.precision = p; }
  public boolean hasPrecision(){ return precision==null ? false : true; }

  public String toString()
  {
    // This needs to match C++ SEDataRequest::ToString
    // We use this string for csv headers and we need to be consistent across all languages
    String str="";
    switch (this.category)
    {
    case Action:
      str = getActionName()+"-";
      if (hasCompartmentName())
        str += getCompartmentName()+"-";
      else if (hasSubstanceName())
        str += getSubstanceName()+"-";
      break;
    case Patient:
      str = "Patient-";
      break;
    case AnesthesiaMachine:
      str = "AnesthesiaMachine-";
      break;
    case BagValveMask:
      str = "BagValveMask-";
      break;
    case ECG:
      str = "ECG-";
      break;
    case ECMO:
      str = "ECMO-";
      break;
    case Inhaler:
      str = "Inhaler-";
      break;
    case MechanicalVentilator:
      str = "MechanicalVentilator-";
      break;
    case GasCompartment:
    case LiquidCompartment:
    case ThermalCompartment:
    case TissueCompartment:
      str += getCompartmentName()+"-";
      if (hasSubstanceName())
        str += getSubstanceName()+"-";
      break;
    case Substance:
      str += getSubstanceName()+"-";
      if (hasCompartmentName())
        str += getCompartmentName()+"-";
      break;
      default:
        break;
    }
    str += this.propertyName;
    if (unit != null)
     str += "(" + unit.toString() + ")";
    str=str.replaceAll(" ", "_");
    return str;
  }

}
