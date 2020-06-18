/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarPressureTimePerVolumeData;
import pulse.cdm.properties.CommonUnits.PressureTimePerVolumeUnit;

/**
 * @author abray
 * PressureTimePerVolume Scalar Class, will enforce that units are proper to PressureTimePerVolumes
 */

public class SEScalarPressureTimePerVolume extends SEScalar
{
  public SEScalarPressureTimePerVolume()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarPressureTimePerVolume(double value, String unit)
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
  public SEScalarPressureTimePerVolume(double value, PressureTimePerVolumeUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarPressureTimePerVolumeData src, SEScalarPressureTimePerVolume dst)
  {
    SEScalar.load(src.getScalarPressureTimePerVolume(),dst);
  }
  public static ScalarPressureTimePerVolumeData unload(SEScalarPressureTimePerVolume src)
  {
    if(!src.isValid())
      return null;
    ScalarPressureTimePerVolumeData.Builder dst = ScalarPressureTimePerVolumeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarPressureTimePerVolume src, ScalarPressureTimePerVolumeData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarPressureTimePerVolumeBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, PressureTimePerVolumeUnit unit)
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
  public double getValue(PressureTimePerVolumeUnit unit)
  {
    return this.getValue(unit.toString());
  }

  public boolean validUnit(String unit)
  {
    if(PressureTimePerVolumeUnit.validUnit(unit))
      return true;
    return false;
  }
}
