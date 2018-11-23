/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SETensionPneumothorax : SEPatientAction
{
    protected eGate type;
    protected eSide side;
    protected SEScalar0To1 severity;

    public SETensionPneumothorax()
    {
        type = eGate.NullGate;
        side = eSide.NullSide;
        severity = null;
    }

    public override void Clear()
    {
        base.Clear();
        type = eGate.NullGate;
        side = eSide.NullSide;
        if (severity != null)
            severity.Invalidate();
    }

    public override bool IsValid()
    {
        return HasType() && HasSide() && HasSeverity();
    }

    public new eGate GetType()
    {
        return type;
    }
    public void SetType(eGate type)
    {
        this.type = type;
    }
    public bool HasType()
    {
        return type == eGate.NullGate ? false : true;
    }

    public eSide GetSide()
    {
        return side;
    }
    public void SetSide(eSide side)
    {
        this.side = side;
    }
    public bool HasSide()
    {
        return side == eSide.NullSide ? false : true;
    }

    public bool HasSeverity()
    {
        return severity == null ? false : severity.IsValid();
    }
    public SEScalar0To1 GetSeverity()
    {
        if (severity == null)
            severity = new SEScalar0To1();
        return severity;
    }

}
