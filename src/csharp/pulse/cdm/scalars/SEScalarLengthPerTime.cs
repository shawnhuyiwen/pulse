/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class LengthPerTimeUnit
{

    private LengthPerTimeUnit(string v) { Value = v; }
    protected string Value { get; }

    public new string ToString() { return Value; }
    public static LengthPerTimeUnit m_Per_s = new LengthPerTimeUnit("m/s");
    public static LengthPerTimeUnit ft_Per_s = new LengthPerTimeUnit("ft/s");

}


public class SEScalarLengthPerTime : SEScalar
{
    protected LengthPerTimeUnit unit;

    public SEScalarLengthPerTime() : base()
    {

    }

    public SEScalarLengthPerTime(SEScalarLengthPerTime from)
    {
        Set(from);
    }

    public SEScalarLengthPerTime(double value, LengthPerTimeUnit unit)
    {
        SetValue(value, unit);
    }


    //public static void load(ScalarLengthPerTimeData src, SEScalarLengthPerTime dst)
    //{
    //    SEScalar.load(src.getScalarLengthPerTime(), dst);
    //}
    //public static ScalarLengthPerTimeData unload(SEScalarLengthPerTime src)
    //{
    //    if (!src.isValid())
    //        return null;
    //    ScalarLengthPerTimeData.Builder dst = ScalarLengthPerTimeData.newBuilder();
    //    unload(src, dst);
    //    return dst.build();
    //}
    //protected static void unload(SEScalarLengthPerTime src, ScalarLengthPerTimeData.Builder dst)
    //{
    //    SEScalar.unload(src, dst.getScalarLengthPerTimeBuilder());
    //}

    public void Set(SEScalarLengthPerTime from)
    {
        if (from == null)
        {
            Invalidate();
            return;
        }
        SetValue(from.value, from.unit);
    }


    public new void SetValue(double value)
    {
        throw new System.NotSupportedException("You must provide a unit");
    }

    public void SetValue(double value, LengthPerTimeUnit unit)
    {
        this.value = value;
        this.unit = unit;
    }


    public double GetValue(LengthPerTimeUnit unit)
    {
        throw new System.NotImplementedException();
    }

    public LengthPerTimeUnit GetUnit()
    {
        return unit;
    }

    public new string ToString()
    {
        return this.value + "(" + this.unit.ToString() + ")";
    }
}

