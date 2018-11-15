/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SETensionPneumothorax : SEPatientAction
{
    protected eGate type;
    protected eSide side;
    protected SEScalar0To1 severity;

    public SETensionPneumothorax()
    {
        type = null;
        side = null;
        severity = null;
    }

    public override void Reset()
    {
        base.Reset();
        type = null;
        side = null;
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
    public void setType(eGate type)
    {
        this.type = type;
    }
    public bool HasType()
    {
        return type == null ? false : true;
    }

    public eSide GetSide()
    {
        return side;
    }
    public void setSide(eSide side)
    {
        this.side = side;
    }
    public bool HasSide()
    {
        return side == null ? false : true;
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
