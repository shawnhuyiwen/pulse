/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.engine;
import java.util.ArrayList;
import java.util.List;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;
import com.kitware.pulse.cdm.bind.Engine.SegmentValidationTargetData;
import com.kitware.pulse.cdm.bind.Engine.SegmentValidationTargetListData;
import com.kitware.pulse.utilities.FileUtils;

public class SESegmentValidationTarget extends SEValidationTarget
{
  public int m_Segment;
  
  public SESegmentValidationTarget()
  {
    super();
  }

  
  public void clear()
  {
    super.clear();
    m_Segment = 0;
  }
  
  public static List<SESegmentValidationTarget> readFile(String fileName) throws InvalidProtocolBufferException
  {
    SegmentValidationTargetListData.Builder builder = SegmentValidationTargetListData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    List<SESegmentValidationTarget> list = new ArrayList<SESegmentValidationTarget>();
    SESegmentValidationTarget.load(builder.build(), list);
    return list;
  }
  public static void writeFile(List<SESegmentValidationTarget> tgts, String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SESegmentValidationTarget.unload(tgts)));
  }
  public static void load(SegmentValidationTargetListData src, List<SESegmentValidationTarget> dst)
  {
    for(SegmentValidationTargetData data : src.getSegmentValidationTargetList())
    {
      SESegmentValidationTarget vt = new SESegmentValidationTarget();
      SESegmentValidationTarget.load(data, vt);
    }
  }
  public static SegmentValidationTargetListData unload(List<SESegmentValidationTarget> src)
  {
    SegmentValidationTargetListData.Builder dst = SegmentValidationTargetListData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(List<SESegmentValidationTarget> src, SegmentValidationTargetListData.Builder dst)
  {
    for(SESegmentValidationTarget vt : src)
    {
      dst.addSegmentValidationTarget(SESegmentValidationTarget.unload(vt));
    }
  }
  
  
  public static void load(SegmentValidationTargetData src, SESegmentValidationTarget dst)
  {
    SEValidationTarget.load(src.getValidationTarget(), dst);
    switch(src.getValidationTarget().getExpectedCase())
    {
      case EQUALTO:
        dst.setEqualTo(src.getValidationTarget().getEqualTo(), src.getSegment());
        break;
      case GREATERTHAN:
        dst.setGreaterThan(src.getValidationTarget().getGreaterThan(), src.getSegment());
        break;
      case LESSTHAN:
        dst.setLessThan(src.getValidationTarget().getLessThan(), src.getSegment());
        break;
      case TREND:
        switch(src.getValidationTarget().getTrend())
        {
          case Increase:
            dst.setIncrease(src.getSegment());
            break;
          case Decrease:
            dst.setDecrease(src.getSegment());
            break;
          default:
            break;
        }
        break;
      case RANGE:
        dst.setRange(
            src.getValidationTarget().getRange().getMinimum(),
            src.getValidationTarget().getRange().getMaximum(),
            src.getSegment());
        break;
      default:
        break;
    }

    dst.m_Segment = src.getSegment();
  }
  public static SegmentValidationTargetData unload(SESegmentValidationTarget src)
  {
    SegmentValidationTargetData.Builder dst = SegmentValidationTargetData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SESegmentValidationTarget src, SegmentValidationTargetData.Builder dst)
  {
    SEValidationTarget.unload(src,dst.getValidationTargetBuilder());
    dst.setSegment(src.m_Segment);
  }

  public int GetSegment() { return m_Segment; }
  
  public void setEqualTo(double d, int segment)
  {
    m_Target = d;
    m_TargetMaximum = d;
    m_TargetMinimum = d;
    m_Segment = segment;
    m_ComparisonType = eComparisonType.EqualTo;
  }
  public void setGreaterThan(double d, int segment)
  {
    m_Target = d;
    m_TargetMaximum = d;
    m_TargetMinimum = d;
    m_Segment = segment;
    m_ComparisonType = eComparisonType.GreaterThan;
    
  }
  public void setLessThan(double d, int segment)
  {
    m_Target = d;
    m_TargetMaximum = d;
    m_TargetMinimum = d;
    m_Segment = segment;
    m_ComparisonType = eComparisonType.LessThan;
  }
  public void setIncrease(int segment)
  {
    m_Target = Double.NaN;
    m_TargetMaximum = Double.NaN;
    m_TargetMinimum = Double.NaN;
    m_Segment = segment;
    m_ComparisonType = eComparisonType.Increase;
  }
  public void setDecrease(int segment)
  {
    m_Target = Double.NaN;
    m_TargetMaximum = Double.NaN;
    m_TargetMinimum = Double.NaN;
    m_Segment = segment;
    m_ComparisonType = eComparisonType.Decrease;
  }
  public void setRange(double min, double max, int segment)
  {
    m_Target = Double.NaN;
    m_TargetMaximum = max;
    m_TargetMinimum = min;
    m_Segment = segment;
    m_ComparisonType = eComparisonType.Range;
  }
}
