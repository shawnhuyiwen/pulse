/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.datarequests;

import com.kitware.pulse.cdm.bind.Engine.DataRequestData.eCategory;
import com.kitware.pulse.cdm.bind.Engine.ValidationTargetData;
import com.kitware.pulse.cdm.bind.Engine.ValidationTargetData.eType;

public class SEValidationTarget extends SEDataRequest
{
  public SEValidationTarget(eCategory c)
  {
    super(c);
  }

  public double        rangeMin;
  public double        rangeMax;
  public eType         type;
  
  public void clear()
  {
    super.clear();
    type = eType.Mean;
    rangeMin=Double.NaN;
    rangeMax=Double.NaN;
  }
  
  public static void load(ValidationTargetData src, SEValidationTarget dst)
  {
    SEDataRequest.load(src.getDataRequest(), dst);
    dst.rangeMax = src.getRangeMax();
    dst.rangeMin = src.getRangeMin();
    dst.type = src.getType();
  }
  public static ValidationTargetData unload(SEValidationTarget src)
  {
    ValidationTargetData.Builder dst = ValidationTargetData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEValidationTarget src, ValidationTargetData.Builder dst)
  {
    SEDataRequest.unload(src,dst.getDataRequestBuilder());
    dst.setRangeMax(src.rangeMax);
    dst.setRangeMin(src.rangeMin);
    dst.setType(src.type);
  }

  @Override
  public String toString()
  {
    return "Validation Target"
        + "\n\tDataRequest: " + super.toString()
        + "\n\tType: " + type
        + "\n\tRange : [" + rangeMin + ", " + rangeMax + "]";
  }
}
