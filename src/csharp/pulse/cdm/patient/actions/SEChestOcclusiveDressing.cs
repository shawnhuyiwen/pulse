/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEChestOcclusiveDressing : SEPatientAction
  {
    protected eSwitch state;
    protected eSide   side;

    public SEChestOcclusiveDressing()
    {
      state = eSwitch.NullSwitch;
      side = eSide.NullSide;
    }

    public override void Clear()
    {
      base.Clear();
      state = eSwitch.NullSwitch;
      side = eSide.NullSide;
    }

    public override bool IsValid()
    {
      return HasState() && HasSide();
    }

    public eSwitch GetState()
    {
      return state;
    }
    public void SetState(eSwitch type)
    {
      this.state = type;
    }
    public bool HasState()
    {
      return state == eSwitch.NullSwitch ? false : true;
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
  }
}
