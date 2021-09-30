/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.SegmentSigmoidalData;

public class SESegmentSigmoidal extends SESegment
{
  protected SEScalarPressure          lowerCorner;
  protected SEScalarPressure          upperCorner;
  protected SEScalarVolumePerPressure baselineCompliance;

  public SESegmentSigmoidal()
  {
    super();
    lowerCorner = null;
    upperCorner = null;
    baselineCompliance = null;
  }

  public SESegmentSigmoidal(SESegment other)
  {
    copy(other);
  }

  public void copy(SESegmentSigmoidal other)
  {
    super.copy(other);
    if (other.hasLowerCorner())
      getLowerCorner().set(other.getLowerCorner());
    if (other.hasUpperCorner())
      getUpperCorner().set(other.getUpperCorner());
    if (other.hasBaselineCompliance())
      getBaselineCompliance().set(other.getBaselineCompliance());
  }

  public void clear()
  {
    super.clear();
    if (lowerCorner != null)
      lowerCorner.invalidate();
    if (upperCorner != null)
      upperCorner.invalidate();
    if (baselineCompliance != null)
      baselineCompliance.invalidate();
  }

  public static void load(SegmentSigmoidalData src, SESegmentSigmoidal dst)
  {
    dst.clear();
    if (src.hasBeginVolume())
      SEScalarVolume.load(src.getBeginVolume(), dst.getBeginVolume());
    if (src.hasEndVolume())
      SEScalarVolume.load(src.getEndVolume(), dst.getEndVolume());
    if (src.hasLowerCorner())
      SEScalarPressure.load(src.getLowerCorner(), dst.getLowerCorner());
    if (src.hasUpperCorner())
      SEScalarPressure.load(src.getUpperCorner(), dst.getUpperCorner());
    if (src.hasBaselineCompliance())
      SEScalarVolumePerPressure.load(src.getBaselineCompliance(), dst.getBaselineCompliance());
  }

  public static SegmentSigmoidalData unload(SESegmentSigmoidal src)
  {
    if (!src.isValid())
      return null;
    SegmentSigmoidalData.Builder dst = SegmentSigmoidalData.newBuilder();
    unload(src, dst);
    return dst.build();
  }

  protected static void unload(SESegmentSigmoidal src, SegmentSigmoidalData.Builder dst)
  {
    if (src.hasBeginVolume())
      dst.setBeginVolume(SEScalarVolume.unload(src.beginVolume));
    if (src.hasEndVolume())
      dst.setEndVolume(SEScalarVolume.unload(src.endVolume));
    if (src.hasLowerCorner())
      dst.setLowerCorner(SEScalarPressure.unload(src.lowerCorner));
    if (src.hasUpperCorner())
      dst.setUpperCorner(SEScalarPressure.unload(src.upperCorner));
    if (src.hasBaselineCompliance())
      dst.setBaselineCompliance(SEScalarVolumePerPressure.unload(src.baselineCompliance));
  }

  public boolean isValid()
  {
    if (!super.isValid())
      return false;
    return hasLowerCorner() && hasUpperCorner() && hasBaselineCompliance();
  }

  public boolean hasLowerCorner()
  {
    return lowerCorner == null ? false : lowerCorner.isValid();
  }

  public SEScalarPressure getLowerCorner()
  {
    if (lowerCorner == null)
      lowerCorner = new SEScalarPressure();
    return lowerCorner;
  }

  public boolean hasUpperCorner()
  {
    return upperCorner == null ? false : upperCorner.isValid();
  }

  public SEScalarPressure getUpperCorner()
  {
    if (upperCorner == null)
      upperCorner = new SEScalarPressure();
    return upperCorner;
  }

  public boolean hasBaselineCompliance()
  {
    return baselineCompliance == null ? false : baselineCompliance.isValid();
  }

  public SEScalarVolumePerPressure getBaselineCompliance()
  {
    if (baselineCompliance == null)
      baselineCompliance = new SEScalarVolumePerPressure();
    return baselineCompliance;
  }

  public String toString()
  {
    String str = super.toString();
    str += "\n\tLowerCorner: " + (hasLowerCorner() ? getLowerCorner().toString() : "Not Provided");
    str += "\n\tUpperCorner: " + (hasUpperCorner() ? getUpperCorner().toString() : "Not Provided");
    str += "\n\tBaselineCompliance: " + (hasBaselineCompliance() ? getBaselineCompliance().toString() : "Not Provided");

    return str;
  }
}
