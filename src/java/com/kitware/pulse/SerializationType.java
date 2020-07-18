/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse;

public enum SerializationType 
{ JSON(1), BINARY(0);
  private final int value;
  private SerializationType(int value) 
  {
    this.value = value;
  }
  public int value() 
  {
    return value;
  }
}
