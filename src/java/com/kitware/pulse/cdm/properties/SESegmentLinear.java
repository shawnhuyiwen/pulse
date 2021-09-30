/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.SegmentLinearData;

public class SESegmentLinear extends SESegment
{
  protected SEScalarPressurePerVolume slope;
  protected SEScalarPressure          yIntercept;

  public SESegmentLinear()
  {
    super();
    slope = null;
    yIntercept = null;
  }

  public SESegmentLinear(SESegment other)
  {
    copy(other);
  }

  public void copy(SESegmentLinear other)
  {
    super.copy(other);
    if (other.hasSlope())
      getSlope().set(other.getSlope());
    if (other.hasYIntercept())
      getYIntercept().set(other.getYIntercept());
  }

  public static void load(SegmentLinearData src, SESegmentLinear dst)
  {
    dst.clear();
    if (src.hasBeginVolume())
      SEScalarVolume.load(src.getBeginVolume(), dst.getBeginVolume());
    if (src.hasEndVolume())
      SEScalarVolume.load(src.getEndVolume(), dst.getEndVolume());
    if (src.hasSlope())
      SEScalarPressurePerVolume.load(src.getSlope(), dst.getSlope());
    if (src.hasYIntercept())
      SEScalarPressure.load(src.getYIntercept(), dst.getYIntercept());
  }

  public static SegmentLinearData unload(SESegmentLinear src)
  {
    if (!src.isValid())
      return null;
    SegmentLinearData.Builder dst = SegmentLinearData.newBuilder();
    unload(src, dst);
    return dst.build();
  }

  protected static void unload(SESegmentLinear src, SegmentLinearData.Builder dst)
  {
    if (src.hasBeginVolume())
      dst.setBeginVolume(SEScalarVolume.unload(src.beginVolume));
    if (src.hasEndVolume())
      dst.setEndVolume(SEScalarVolume.unload(src.endVolume));
    if (src.hasSlope())
      dst.setSlope(SEScalarPressurePerVolume.unload(src.slope));
    if (src.hasYIntercept())
      dst.setYIntercept(SEScalarPressure.unload(src.yIntercept));
  }

  public void clear()
  {
    super.clear();
    if (slope != null)
      slope.invalidate();
    if (yIntercept != null)
      yIntercept.invalidate();
  }

  public boolean isValid()
  {
    if (!super.isValid())
      return false;
    return hasSlope() && hasYIntercept();
  }

  public boolean hasSlope()
  {
    return slope == null ? false : slope.isValid();
  }

  public SEScalarPressurePerVolume getSlope()
  {
    if (slope == null)
      slope = new SEScalarPressurePerVolume();
    return slope;
  }

  public boolean hasYIntercept()
  {
    return yIntercept == null ? false : yIntercept.isValid();
  }

  public SEScalarPressure getYIntercept()
  {
    if (yIntercept == null)
      yIntercept = new SEScalarPressure();
    return yIntercept;
  }

  public String toString()
  {
    String str = super.toString();
    str += "\n\tSlope: " + (hasSlope() ? getSlope().toString() : "Not Provided");
    str += "\n\tYIntercepte: " + (hasYIntercept() ? getYIntercept().toString() : "Not Provided");

    return str;
  }
}
