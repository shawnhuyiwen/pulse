/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.conditions;

import java.io.Serializable;

import com.kitware.pulse.cdm.bind.Conditions.ConditionData;
import com.kitware.pulse.cdm.bind.Engine.AnyConditionData;
import com.kitware.pulse.cdm.patient.conditions.SEPatientCondition;
import com.kitware.pulse.cdm.system.environment.conditions.SEEnvironmentCondition;
import com.kitware.pulse.utilities.Log;

public abstract class SECondition implements Serializable
{
  private static final long serialVersionUID = -1566830492504081551L;
  
  protected String comment;
  
  public SECondition() 
  {
    comment = null;
  }
  
  public void copy(SECondition other) 
  {
    reset();
    comment = other.comment;
  }
  
  public void reset() 
  {
    this.comment = null;
  }
  
  public static void load(ConditionData src, SECondition dst) 
  {
    dst.reset();
    if(!src.getComment().isEmpty())
      dst.setComment(src.getComment());
  }
  protected static void unload(SECondition src, ConditionData.Builder dst)
  {
    if (src.hasComment())
      dst.setComment(src.comment);
  }
  
  public boolean hasComment()
  {
    return this.comment != null && !this.comment.isEmpty();
  }  
  public String getComment()
  {
    return this.comment;
  }
  public void setComment(String comment)
  {
    this.comment = comment;
  }
  public void invalidateComment()
  {
    this.comment = null;
  }
  
  public static SECondition ANY2CDM(AnyConditionData any)
  {
    switch(any.getConditionCase())
    {
      case PATIENTCONDITION:
        return SEPatientCondition.ANY2CDM(any.getPatientCondition());
      case ENVIRONMENTCONDITION:
        return SEEnvironmentCondition.ANY2CDM(any.getEnvironmentCondition());
      case CONDITION_NOT_SET:
        Log.warn("AnyConditionData is empty...was that intended?");
        return null;
      default:
        Log.error("Unsupported AnyConditionData Type "+any.getConditionCase());
    }
    return null;
  }
  
  public static AnyConditionData CDM2ANY(SECondition c)
  {
    AnyConditionData.Builder dst = AnyConditionData.newBuilder();
    if(c instanceof SEEnvironmentCondition)
    {
      dst.setEnvironmentCondition(SEEnvironmentCondition.CDM2ANY((SEEnvironmentCondition)c));
      return dst.build();
    }
    if(c instanceof SEPatientCondition)
    {
      dst.setPatientCondition(SEPatientCondition.CDM2ANY((SEPatientCondition)c));
      return dst.build();
    }
    Log.error("Unsupported AnyConditionData Type "+c.toString());
    return dst.build();
  }

  @Override
  public abstract String toString();
}
