/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import java.util.*;

import com.kitware.pulse.cdm.bind.Properties.AnySegmentData;
import com.kitware.pulse.cdm.bind.Properties.CurveData;

public class SECurve extends SEProperty
{
  protected List<SESegment> segments = new ArrayList<SESegment>();

  public SECurve()
  {
    super();
  }

  public SECurve(SECurve other)
  {
    copy(other);
  }

  public void copy(SECurve other)
  {
    for (SESegment s : segments)
    {
      if (s instanceof SESegmentConstant)
        addConstantSegment().copy((SESegmentConstant) s);
      else if (s instanceof SESegmentLinear)
        addConstantSegment().copy((SESegmentLinear) s);
      else if (s instanceof SESegmentParabolic)
        addConstantSegment().copy((SESegmentParabolic) s);
      else if (s instanceof SESegmentSigmoidal)
        addConstantSegment().copy((SESegmentSigmoidal) s);
    }
  }

  public static void load(CurveData src, SECurve dest)
  {
    dest.invalidate();
    for (AnySegmentData seg : src.getSegmentList())
    {
      switch(seg.getActionCase())
      {
        case CONSTANTSEGMENT:
        {
          SESegmentConstant s = new SESegmentConstant();
          SESegmentConstant.load(seg.getConstantSegment(), s);
          continue;
        }
        case LINEARSEGMENT:
        {
          SESegmentLinear s = new SESegmentLinear();
          SESegmentLinear.load(seg.getLinearSegment(), s);
          continue;
        }
        case PARABOLICSEGMENT:
        {
          SESegmentParabolic s = new SESegmentParabolic();
          SESegmentParabolic.load(seg.getParabolicSegment(), s);
          continue;
        }
        case SIGMOIDALSEGMENT:
        {
          SESegmentSigmoidal s = new SESegmentSigmoidal();
          SESegmentSigmoidal.load(seg.getSigmoidalSegment(), s);
          continue;
        }
      }
    }
  }

  public static CurveData unload(SECurve src)
  {
    if (!src.isValid())
      return null;
    CurveData.Builder dst = CurveData.newBuilder();
    unload(src, dst);
    return dst.build();
  }

  protected static void unload(SECurve src, CurveData.Builder dst)
  {
    for (SESegment seg : src.getSegments())
    {
      if (seg instanceof SESegmentConstant)
      {
        dst.addSegmentBuilder()
          .setConstantSegment(SESegmentConstant.unload((SESegmentConstant)seg))
          .build();
        continue;
      }
      if (seg instanceof SESegmentLinear)
      {
        dst.addSegmentBuilder()
          .setLinearSegment(SESegmentLinear.unload((SESegmentLinear)seg))
          .build();
        continue;
      }
      if (seg instanceof SESegmentParabolic)
      {
        dst.addSegmentBuilder()
          .setParabolicSegment(SESegmentParabolic.unload((SESegmentParabolic)seg))
          .build();
        continue;
      }
      if (seg instanceof SESegmentSigmoidal)
      {
        dst.addSegmentBuilder()
          .setSigmoidalSegment(SESegmentSigmoidal.unload((SESegmentSigmoidal)seg))
          .build();
        continue;
      }
    }
  }

  public void invalidate()
  {
    segments.clear();
  }

  public boolean isValid()
  {
    if (!hasSegment())
      return false;
    for (SESegment s : segments)
      if (!s.isValid())
        return false;
    return true;
  }

  public boolean hasSegment()
  {
    return segments.size() > 0;
  }

  public SESegmentConstant addConstantSegment()
  {
    SESegmentConstant s = new SESegmentConstant();
    segments.add(s);
    return s;
  }

  public SESegmentLinear addLinearSegment()
  {
    SESegmentLinear s = new SESegmentLinear();
    segments.add(s);
    return s;
  }

  public SESegmentParabolic addParabolicSegment()
  {
    SESegmentParabolic s = new SESegmentParabolic();
    segments.add(s);
    return s;
  }

  public SESegmentSigmoidal addSigmoidalSegment()
  {
    SESegmentSigmoidal s = new SESegmentSigmoidal();
    segments.add(s);
    return s;
  }

  public List<SESegment> getSegments()
  {
    return Collections.unmodifiableList(segments);
  }

  public String toString()
  {
    String str = "Curve: ";
    for (SESegment s : segments)
      str += "\n\t" + s.toString();
    return str;
  }
}
