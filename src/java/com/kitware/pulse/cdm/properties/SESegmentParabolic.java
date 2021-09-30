/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.SegmentParabolicData;

public class SESegmentParabolic extends SESegment
{
  protected SEScalar coefficient1;
  protected SEScalar coefficient2;
  protected SEScalar coefficient3;
  protected SEScalar coefficient4;

  public SESegmentParabolic()
  {
    super();
    coefficient1 = null;
    coefficient2 = null;
    coefficient3 = null;
    coefficient4 = null;
  }

  public SESegmentParabolic(SESegment other)
  {
    copy(other);
  }

  public void copy(SESegmentParabolic other)
  {
    super.copy(other);
    if (other.hasCoefficient1())
      getCoefficient1().set(other.getCoefficient1());
    if (other.hasCoefficient2())
      getCoefficient2().set(other.getCoefficient2());
    if (other.hasCoefficient3())
      getCoefficient3().set(other.getCoefficient3());
    if (other.hasCoefficient4())
      getCoefficient4().set(other.getCoefficient4());
  }

  public static void load(SegmentParabolicData src, SESegmentParabolic dst)
  {
    dst.clear();
    if (src.hasBeginVolume())
      SEScalarVolume.load(src.getBeginVolume(), dst.getBeginVolume());
    if (src.hasEndVolume())
      SEScalarVolume.load(src.getEndVolume(), dst.getEndVolume());
    if (src.hasCoefficient1())
      SEScalar.load(src.getCoefficient1(), dst.getCoefficient1());
    if (src.hasCoefficient2())
      SEScalar.load(src.getCoefficient2(), dst.getCoefficient2());
    if (src.hasCoefficient3())
      SEScalar.load(src.getCoefficient3(), dst.getCoefficient3());
    if (src.hasCoefficient4())
      SEScalar.load(src.getCoefficient4(), dst.getCoefficient4());
  }

  public static SegmentParabolicData unload(SESegmentParabolic src)
  {
    if (!src.isValid())
      return null;
    SegmentParabolicData.Builder dst = SegmentParabolicData.newBuilder();
    unload(src, dst);
    return dst.build();
  }

  protected static void unload(SESegmentParabolic src, SegmentParabolicData.Builder dst)
  {
    if (src.hasBeginVolume())
      dst.setBeginVolume(SEScalarVolume.unload(src.beginVolume));
    if (src.hasEndVolume())
      dst.setEndVolume(SEScalarVolume.unload(src.endVolume));
    if (src.hasCoefficient1())
      dst.setCoefficient1(SEScalar.unload(src.coefficient1));
    if (src.hasCoefficient2())
      dst.setCoefficient2(SEScalar.unload(src.coefficient2));
    if (src.hasCoefficient3())
      dst.setCoefficient3(SEScalar.unload(src.coefficient3));
    if (src.hasCoefficient4())
      dst.setCoefficient4(SEScalar.unload(src.coefficient4));
  }

  public void clear()
  {
    super.clear();
    if (coefficient1 != null)
      coefficient1.invalidate();
    if (coefficient2 != null)
      coefficient2.invalidate();
    if (coefficient3 != null)
      coefficient3.invalidate();
    if (coefficient4 != null)
      coefficient4.invalidate();
  }

  public boolean isValid()
  {
    if (!super.isValid())
      return false;
    return hasCoefficient1() && hasCoefficient2() && hasCoefficient3() && hasCoefficient4();
  }

  public boolean hasCoefficient1()
  {
    return coefficient1 == null ? false : coefficient1.isValid();
  }

  public SEScalar getCoefficient1()
  {
    if (coefficient1 == null)
      coefficient1 = new SEScalar();
    return coefficient1;
  }

  public boolean hasCoefficient2()
  {
    return coefficient2 == null ? false : coefficient2.isValid();
  }

  public SEScalar getCoefficient2()
  {
    if (coefficient2 == null)
      coefficient2 = new SEScalar();
    return coefficient2;
  }

  public boolean hasCoefficient3()
  {
    return coefficient3 == null ? false : coefficient3.isValid();
  }

  public SEScalar getCoefficient3()
  {
    if (coefficient3 == null)
      coefficient3 = new SEScalar();
    return coefficient3;
  }

  public boolean hasCoefficient4()
  {
    return coefficient4 == null ? false : coefficient4.isValid();
  }

  public SEScalar getCoefficient4()
  {
    if (coefficient4 == null)
      coefficient4 = new SEScalar();
    return coefficient4;
  }

  public String toString()
  {
    String str = super.toString();
    str += "\n\tCoefficient1: " + (hasCoefficient1() ? getCoefficient1().toString() : "Not Provided");
    str += "\n\tCoefficient2: " + (hasCoefficient2() ? getCoefficient2().toString() : "Not Provided");
    str += "\n\tCoefficient3: " + (hasCoefficient3() ? getCoefficient3().toString() : "Not Provided");
    str += "\n\tCoefficient4: " + (hasCoefficient4() ? getCoefficient4().toString() : "Not Provided");

    return str;
  }
}
