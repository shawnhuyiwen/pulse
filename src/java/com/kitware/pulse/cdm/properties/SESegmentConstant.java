/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.SegmentConstantData;

public class SESegmentConstant extends SESegment
{
  protected SEScalarVolumePerPressure compliance;

  public SESegmentConstant()
  {
    super();
    compliance = null;
  }

  public SESegmentConstant(SESegment other)
  {
    copy(other);
  }

  public void copy(SESegmentConstant other)
  {
    super.copy(other);
    if (other.hasCompliance())
      getCompliance().set(other.getCompliance());
  }

  public void clear()
  {
    super.clear();
    if (compliance != null)
      compliance.invalidate();
  }

  public static void load(SegmentConstantData src, SESegmentConstant dst)
  {
    dst.clear();
    if (src.hasBeginVolume())
      SEScalarVolume.load(src.getBeginVolume(), dst.getBeginVolume());
    if (src.hasEndVolume())
      SEScalarVolume.load(src.getEndVolume(), dst.getEndVolume());
    if (src.hasCompliance())
      SEScalarVolumePerPressure.load(src.getCompliance(), dst.getCompliance());
  }

  public static SegmentConstantData unload(SESegmentConstant src)
  {
    if (!src.isValid())
      return null;
    SegmentConstantData.Builder dst = SegmentConstantData.newBuilder();
    unload(src, dst);
    return dst.build();
  }

  protected static void unload(SESegmentConstant src, SegmentConstantData.Builder dst)
  {
    if (src.hasBeginVolume())
      dst.setBeginVolume(SEScalarVolume.unload(src.beginVolume));
    if (src.hasEndVolume())
      dst.setEndVolume(SEScalarVolume.unload(src.endVolume));
    if (src.hasCompliance())
      dst.setCompliance(SEScalarVolumePerPressure.unload(src.compliance));
  }

  public boolean

      isValid()
  {
    if (!super.isValid())
      return false;
    return hasCompliance();
  }

  public boolean hasCompliance()
  {
    return compliance == null ? false : compliance.isValid();
  }

  public SEScalarVolumePerPressure getCompliance()
  {
    if (compliance == null)
      compliance = new SEScalarVolumePerPressure();
    return compliance;
  }

  public String toString()
  {
    String str = super.toString();
    str += "\n\tCompliance: " + (hasCompliance() ? getCompliance().toString() : "Not Provided");

    return str;
  }
}
