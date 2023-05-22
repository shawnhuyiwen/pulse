/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.engine;
import java.util.ArrayList;
import java.util.List;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;
import com.kitware.pulse.cdm.bind.Engine.TimeSeriesValidationTargetData;
import com.kitware.pulse.cdm.bind.Engine.TimeSeriesValidationTargetData.eType;
import com.kitware.pulse.cdm.bind.Engine.TimeSeriesValidationTargetListData;
import com.kitware.pulse.utilities.FileUtils;

public class SETimeSeriesValidationTarget extends SEValidationTarget
{
  protected eType m_TargetType;
  
  public SETimeSeriesValidationTarget()
  {
    super();
  }

  
  public void clear()
  {
    super.clear();
    m_TargetType = eType.Mean;
  }
  
  public static List<SETimeSeriesValidationTarget> readFile(String fileName) throws InvalidProtocolBufferException
  {
    TimeSeriesValidationTargetListData.Builder builder = TimeSeriesValidationTargetListData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    List<SETimeSeriesValidationTarget> list = new ArrayList<SETimeSeriesValidationTarget>();
    SETimeSeriesValidationTarget.load(builder.build(), list);
    return list;
  }
  public static void writeFile(List<SETimeSeriesValidationTarget> tgts, String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SETimeSeriesValidationTarget.unload(tgts)));
  }
  public static void load(TimeSeriesValidationTargetListData src, List<SETimeSeriesValidationTarget> dst)
  {
    for(TimeSeriesValidationTargetData data : src.getTimeSeriesValidationTargetList())
    {
      SETimeSeriesValidationTarget vt = new SETimeSeriesValidationTarget();
      SETimeSeriesValidationTarget.load(data, vt);
    }
  }
  public static TimeSeriesValidationTargetListData unload(List<SETimeSeriesValidationTarget> src)
  {
    TimeSeriesValidationTargetListData.Builder dst = TimeSeriesValidationTargetListData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(List<SETimeSeriesValidationTarget> src, TimeSeriesValidationTargetListData.Builder dst)
  {
    for(SETimeSeriesValidationTarget vt : src)
    {
      dst.addTimeSeriesValidationTarget(SETimeSeriesValidationTarget.unload(vt));
    }
  }
  
  public static void load(TimeSeriesValidationTargetData src, SETimeSeriesValidationTarget dst)
  {
    SEValidationTarget.load(src.getValidationTarget(), dst);
    switch(src.getValidationTarget().getExpectedCase())
    {
      case EQUALTO:
        dst.setEqualTo(src.getValidationTarget().getEqualTo(), src.getType());
        break;
      case GREATERTHAN:
        dst.setGreaterThan(src.getValidationTarget().getGreaterThan(), src.getType());
        break;
      case LESSTHAN:
        dst.setLessThan(src.getValidationTarget().getLessThan(), src.getType());
        break;
      case TREND:
        switch(src.getValidationTarget().getTrend())
        {
          case Increase:
            dst.setIncrease(src.getType());
            break;
          case Decrease:
            dst.setDecrease(src.getType());
            break;
          default:
            break;
        }
        break;
      case RANGE:
        dst.setRange(
            src.getValidationTarget().getRange().getMinimum(),
            src.getValidationTarget().getRange().getMaximum(),
            src.getType());
        break;
      default:
        break;
    }

    dst.m_TargetType = src.getType();
  }
  public static TimeSeriesValidationTargetData unload(SETimeSeriesValidationTarget src)
  {
    TimeSeriesValidationTargetData.Builder dst = TimeSeriesValidationTargetData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SETimeSeriesValidationTarget src, TimeSeriesValidationTargetData.Builder dst)
  {
    SEValidationTarget.unload(src,dst.getValidationTargetBuilder());
    dst.setType(src.m_TargetType);
  }

  public eType getTargetType() { return m_TargetType; }
  
  public void setEqualTo(double d, eType t)
  {
    m_Target = d;
    m_TargetMaximum = d;
    m_TargetMinimum = d;
    m_TargetType = t;
    m_ComparisonType = eComparisonType.EqualTo;
  }
  public void setGreaterThan(double d, eType t)
  {
    m_Target = d;
    m_TargetMaximum = d;
    m_TargetMinimum = d;
    m_TargetType = t;
    m_ComparisonType = eComparisonType.GreaterThan;
    
  }
  public void setLessThan(double d, eType t)
  {
    m_Target = d;
    m_TargetMaximum = d;
    m_TargetMinimum = d;
    m_TargetType = t;
    m_ComparisonType = eComparisonType.LessThan;
  }
  public void setIncrease(eType t)
  {
    m_Target = Double.NaN;
    m_TargetMaximum = Double.NaN;
    m_TargetMinimum = Double.NaN;
    m_TargetType = t;
    m_ComparisonType = eComparisonType.Increase;
  }
  public void setDecrease(eType t)
  {
    m_Target = Double.NaN;
    m_TargetMaximum = Double.NaN;
    m_TargetMinimum = Double.NaN;
    m_TargetType = t;
    m_ComparisonType = eComparisonType.Decrease;
  }
  public void setRange(double min, double max, eType t)
  {
    m_Target = Double.NaN;
    m_TargetMaximum = max;
    m_TargetMinimum = min;
    m_TargetType = t;
    m_ComparisonType = eComparisonType.Range;
  }
}
