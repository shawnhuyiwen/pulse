/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.FunctionElectricPotentialVsTimeData;
import com.kitware.pulse.cdm.exceptions.InvalidUnitException;
import com.kitware.pulse.cdm.properties.CommonUnits.ElectricPotentialUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;


/**
 * @author abray
 */
public class SEFunctionElectricPotentialVsTime extends SEFunction
{
  public SEFunctionElectricPotentialVsTime()
  {
    super();
  }
    
  public static void load(FunctionElectricPotentialVsTimeData src, SEFunctionElectricPotentialVsTime dst)
  {
    SEFunction.load(src.getFunctionElectricPotentialVsTime(), dst);
  }
  public static FunctionElectricPotentialVsTimeData unload(SEFunctionElectricPotentialVsTime src)
  {
    if(!src.isValid())
      return null;
    FunctionElectricPotentialVsTimeData.Builder dst = FunctionElectricPotentialVsTimeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEFunctionElectricPotentialVsTime src, FunctionElectricPotentialVsTimeData.Builder dst)
  {
    SEFunction.unload(src,dst.getFunctionElectricPotentialVsTimeBuilder());
  }

  @Override
  public boolean isValidDependentUnit(String unit)
  {
    return ElectricPotentialUnit.validUnit(unit);
  }
  
  @Override
  public boolean isValidIndependentUnit(String unit)
  {
    return TimeUnit.validUnit(unit);
  }
  
  public String getElectricPotentialUnit()
  {
    return this.dependentUnit;
  }
  
  public String getTimeUnit()
  {
    return this.independentUnit;
  }

  public double[] getElectricPotential(String unit)
  {
    if(!isValidDependentUnit(unit))
      throw new InvalidUnitException(unit, "ElectricPotential");
    return getDependent(unit);
  }
  
  public String getElectricPotentialString(String unit)
  {
    if(!isValidDependentUnit(unit))
      throw new InvalidUnitException(unit, "ElectricPotential");
    return getDependentString(unit);
  }
  
  public void setElectricPotential(double[] data, String unit)
  {
    if(!isValidDependentUnit(unit))
      throw new InvalidUnitException(unit, "ElectricPotential");
    setDependent(data, unit);
  }
  
  public void setElectricPotential(String data, String unit)
  {
    if(!isValidDependentUnit(unit))
      throw new InvalidUnitException(unit, "ElectricPotential");
    setDependent(data, unit);
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double[] getElectricPotential(ElectricPotentialUnit unit)
  {
    return getDependent(unit.toString());
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public String getElectricPotentialString(ElectricPotentialUnit unit)
  {
    return getDependentString(unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setElectricPotential(double[] data, ElectricPotentialUnit unit)
  {
    setDependent(data, unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setElectricPotential(String data, ElectricPotentialUnit unit)
  {
    setDependent(data, unit.toString());
  }
  
  public double[] getTime(String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Time");
    return getIndependent(unit);
  }
  
  public String getTimeString(String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Time");
    return getIndependentString(unit);
  }
  
  public void setTime(double[] data, String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Time");
    setIndependent(data, unit);
  }
  
  public void setTime(String data, String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Time");
    setIndependent(data, unit);
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double[] getTime(TimeUnit unit)
  {
    return getIndependent(unit.toString());
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public String getTimeString(TimeUnit unit)
  {
    return getIndependentString(unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setTime(double[] data, TimeUnit unit)
  {
    setIndependent(data, unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setTime(String data, TimeUnit unit)
  {
    setIndependent(data, unit.toString());
  }
}
