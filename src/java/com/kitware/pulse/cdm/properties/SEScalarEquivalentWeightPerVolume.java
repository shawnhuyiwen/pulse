/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.ScalarEquivalentWeightPerVolumeData;
import com.kitware.pulse.cdm.properties.CommonUnits.EquivalentWeightPerVolumeUnit;

public class SEScalarEquivalentWeightPerVolume extends SEScalar
{
  public SEScalarEquivalentWeightPerVolume()
  {
    super();
  }
  

  public SEScalarEquivalentWeightPerVolume(double value, String unit)
  {
    this();
    this.setValue(value, unit);
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public SEScalarEquivalentWeightPerVolume(double value, EquivalentWeightPerVolumeUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarEquivalentWeightPerVolumeData src, SEScalarEquivalentWeightPerVolume dst)
  {
    SEScalar.load(src.getScalarEquivalentWeightPerVolume(),dst);
  }
  public static ScalarEquivalentWeightPerVolumeData unload(SEScalarEquivalentWeightPerVolume src)
  {
    if(!src.isValid())
      return null;
    ScalarEquivalentWeightPerVolumeData.Builder dst = ScalarEquivalentWeightPerVolumeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarEquivalentWeightPerVolume src, ScalarEquivalentWeightPerVolumeData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarEquivalentWeightPerVolumeBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, EquivalentWeightPerVolumeUnit unit)
  {
    this.setValue(value,unit.toString());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double getValue(EquivalentWeightPerVolumeUnit unit)
  {
    return this.getValue(unit.toString());
  }

  @Override
  public boolean validUnit(String unit)
  {
    if(EquivalentWeightPerVolumeUnit.validUnit(unit))
      return true;
    return false;
  }
}
