/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.exceptions;

public class InvalidUnitException extends RuntimeException
{
  private static final long serialVersionUID = 2688722483921736083L;

  private String badUnit;
  private String type;
  public InvalidUnitException(String unit, String type)
  {
    this.badUnit=unit;
    this.type=type;
  }
  
  @Override
  public String toString()
  {
    return "InvalidUnitException: Unit '" + badUnit + "' is not compatible with " + type;
  }
}
