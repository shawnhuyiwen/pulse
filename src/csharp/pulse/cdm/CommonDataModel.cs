/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eSerializationFormat : int   // State file serialization format
  {
    JSON = 0,
    BINARY
  }

  public enum eSide : int
  {
    NullSide = 0,
    Left,
    Right
  }

  public enum eGate : int
  {
    NullGate = 0,
    Open,
    Closed
  }

  public enum eSwitch : int
  {
    NullSwitch = 0,
    Off,
    On
  }

  public enum eCharge : int
  {
    NullCharge = 0,
    Negative,
    Neutral,
    Positive
  }
}
