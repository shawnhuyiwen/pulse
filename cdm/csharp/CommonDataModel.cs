/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public enum SerializationFormat : int   // State file serialization format
{
    BINARY = 0,
    ASCII
}

public class eSide
{
    private eSide(string v) { Value = v; }
    protected string Value { get; }

    public static eSide NullSide { get { return new eSide("NullSide"); } }
    public static eSide Left { get { return new eSide("Left"); } }
    public static eSide Right { get { return new eSide("Right"); } }
}

public class eGate
{
    private eGate(string v) { Value = v; }
    protected string Value { get; }

    public static eGate NullGate { get { return new eGate("NullGate"); } }
    public static eGate Open { get { return new eGate("Open"); } }
    public static eGate Closed { get { return new eGate("Closed"); } }
}

public class eSwitch
{
    private eSwitch(string v) { Value = v; }
    protected string Value { get; }

    public static eSwitch NullSwitch { get { return new eSwitch("NullSwitch"); } }
    public static eSwitch Off { get { return new eSwitch("Off"); } }
    public static eSwitch On { get { return new eSwitch("On"); } }
}

public class eCharge
{
    private eCharge(string v) { Value = v; }
    protected string Value { get; }

    public static eCharge NullCharge { get { return new eCharge("NullCharge"); } }
    public static eCharge Negative { get { return new eCharge("Negative"); } }
    public static eCharge Neutral { get { return new eCharge("Neutral"); } }
    public static eCharge Positive { get { return new eCharge("Positive"); } }
}
