/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.engine;

import com.kitware.pulse.cdm.bind.Engine.ValidationTargetData.eTrend;
import com.kitware.pulse.cdm.bind.Engine.ValidationTargetData;

public class SEValidationTarget
{
  public enum eComparisonType
  {
    EqualTo, GreaterThan, LessThan, Increase, Decrease, Range
  }
  protected String          m_Header;
  protected String          m_Reference;
  protected String          m_Notes;
  protected double          m_Target;
  protected double          m_TargetMaximum;
  protected double          m_TargetMinimum;
  protected eComparisonType m_ComparisonType;
  
  public SEValidationTarget()
  {
    clear();
  }
  
  public void clear()
  {
    m_Header = "";
    m_Reference = "";
    m_Notes = "";
    m_Target         = Double.NaN;
    m_TargetMaximum  = Double.NaN;
    m_TargetMinimum  = Double.NaN;
    m_ComparisonType = eComparisonType.EqualTo;
  }
  
  public static void load(ValidationTargetData src, SEValidationTarget dst)
  {
    dst.m_Header = src.getHeader();
    dst.m_Reference = src.getReference();
    dst.m_Notes = src.getNotes();
  }
  public static ValidationTargetData unload(SEValidationTarget src)
  {
    ValidationTargetData.Builder dst = ValidationTargetData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEValidationTarget src, ValidationTargetData.Builder dst)
  {
    dst.setHeader(src.m_Header);
    dst.setReference(src.m_Reference);
    dst.setNotes(src.m_Notes);
    
    switch(src.m_ComparisonType)
    {
      case EqualTo:
        dst.setEqualTo(src.m_Target);
        break;
      case GreaterThan:
        dst.setGreaterThan(src.m_Target);
        break;
      case LessThan:
        dst.setLessThan(src.m_Target);
        break;
      case Increase:
        dst.setTrend(eTrend.Increase);
        break;
      case Decrease:
        dst.setTrend(eTrend.Decrease);
        break;
      case Range:
        dst.getRangeBuilder().setMinimum(src.m_TargetMinimum);
        dst.getRangeBuilder().setMaximum(src.m_TargetMaximum);
        dst.getRangeBuilder().build();
        break;
    }
  }
  
  public String getHeader() { return m_Header; }
  public void setHeader(String h) { m_Header = h; }

  public String getReference() { return m_Reference; }
  public void setReference(String c) { m_Reference = c; }

  public String getNotes() { return m_Notes; }
  public void setNotes(String n) { m_Notes = n; }

  public eComparisonType getComparisonType() { return m_ComparisonType; }
  public double getTargetMaximum() { return m_TargetMaximum; }
  public double getTargetMinimum() { return m_TargetMinimum; }
  public double getTarget() { return m_Target; }
}
