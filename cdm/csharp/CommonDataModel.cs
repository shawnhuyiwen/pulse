/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public enum SerializationFormat : int   // State file serialization format
{
    BINARY = 0,
    JSON
}

public enum eSide : int
{
    NullSide = 0,
    Left,
    Right
}

public enum eGate : int
{
    NullGate= 0,
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
