/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.AnySegmentData;

public class SESegment
{
  protected SEScalarVolume beginVolume;
  protected SEScalarVolume endVolume;

  protected SESegment()
  {
    beginVolume = null;
    endVolume = null;
  }

  public SESegment(SESegment other)
  {
    copy(other);
  }

  public void copy(SESegment other)
  {
    if (other.hasBeginVolume())
      getBeginVolume().set(other.getBeginVolume());
    if (other.hasEndVolume())
      getEndVolume().set(other.getEndVolume());
  }

  public void clear()
  {
    if (beginVolume != null)
      beginVolume.invalidate();
    if (endVolume != null)
      endVolume.invalidate();
  }

  public boolean isValid()
  {
    return hasBeginVolume() && hasEndVolume();
  }

  public boolean hasBeginVolume()
  {
    return beginVolume == null ? false : beginVolume.isValid();
  }

  public SEScalarVolume getBeginVolume()
  {
    if (beginVolume == null)
      beginVolume = new SEScalarVolume();
    return beginVolume;
  }

  public boolean hasEndVolume()
  {
    return endVolume == null ? false : endVolume.isValid();
  }

  public SEScalarVolume getEndVolume()
  {
    if (endVolume == null)
      endVolume = new SEScalarVolume();
    return endVolume;
  }

  public String toString()
  {
    String str = "";
    str += "\n\tBeginVolume: " + (hasBeginVolume() ? getBeginVolume().toString() : "Not Provided");
    str += "\n\tEndVolume: " + (hasEndVolume() ? getEndVolume().toString() : "Not Provided");

    return str;
  }
}
