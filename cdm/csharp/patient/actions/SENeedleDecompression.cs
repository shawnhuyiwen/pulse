/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SENeedleDecompression : SEPatientAction
{
    protected eSwitch state;
    protected eSide side;

    public SENeedleDecompression()
    {
        state = eSwitch.Off;
        side = eSide.NullSide;
    }

    public override void Clear()
    {
        base.Clear();
        state = eSwitch.Off;
        side = eSide.NullSide;
    }

    public override bool IsValid()
    {
        return HasSide();
    }

    public eSwitch GetState()
    {
        return state;
    }
    public void SetState(eSwitch s)
    {
        this.state = (s == eSwitch.NullSwitch) ? eSwitch.Off : s;
    }

    public eSide GetSide()
    {
        return side;
    }
    public void SetSide(eSide rightOrLeft)
    {
        side = rightOrLeft;
    }
    public bool HasSide()
    {
        return side == null ? false : true;
    }
}
