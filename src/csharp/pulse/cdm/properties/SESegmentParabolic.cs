/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESegmentParabolic : SESegment
  {
    protected SEScalar coefficient1;
    protected SEScalar coefficient2;
    protected SEScalar coefficient3;
    protected SEScalar coefficient4;

    public SESegmentParabolic() : base()
    {
      coefficient1 = null;
      coefficient2 = null;
      coefficient3 = null;
      coefficient4 = null;
    }

    public SESegmentParabolic(SESegment other)
    {
      Copy(other);
    }

    public void Copy(SESegmentParabolic other)
    {
      base.Copy(other);
      if (other.HasCoefficient1())
        GetCoefficient1().Set(other.GetCoefficient1());
      if (other.HasCoefficient2())
        GetCoefficient2().Set(other.GetCoefficient2());
      if (other.HasCoefficient3())
        GetCoefficient3().Set(other.GetCoefficient3());
      if (other.HasCoefficient4())
        GetCoefficient4().Set(other.GetCoefficient4());
    }

    public override void Clear()
    {
      base.Clear();
      if (coefficient1 != null)
        coefficient1.Invalidate();
      if (coefficient2 != null)
        coefficient2.Invalidate();
      if (coefficient3 != null)
        coefficient3.Invalidate();
      if (coefficient4 != null)
        coefficient4.Invalidate();
    }

    public override bool IsValid()
    {
      if (!base.IsValid())
        return false;
      return HasCoefficient1() && HasCoefficient2() &&
        HasCoefficient3() && HasCoefficient4();
    }

    public bool HasCoefficient1()
    {
      return coefficient1 == null ? false : coefficient1.IsValid();
    }
    public SEScalar GetCoefficient1()
    {
      if (coefficient1 == null)
        coefficient1 = new SEScalar();
      return coefficient1;
    }

    public bool HasCoefficient2()
    {
      return coefficient2 == null ? false : coefficient2.IsValid();
    }
    public SEScalar GetCoefficient2()
    {
      if (coefficient2 == null)
        coefficient2 = new SEScalar();
      return coefficient2;
    }

    public bool HasCoefficient3()
    {
      return coefficient3 == null ? false : coefficient3.IsValid();
    }
    public SEScalar GetCoefficient3()
    {
      if (coefficient3 == null)
        coefficient3 = new SEScalar();
      return coefficient3;
    }

    public bool HasCoefficient4()
    {
      return coefficient4 == null ? false : coefficient4.IsValid();
    }
    public SEScalar GetCoefficient4()
    {
      if (coefficient4 == null)
        coefficient4 = new SEScalar();
      return coefficient4;
    }

    public override string ToString()
    {
      string str = base.ToString();
      str += "\n\tCoefficient1: " + (HasCoefficient1() ? GetCoefficient1().ToString() : "Not Provided");
      str += "\n\tCoefficient2: " + (HasCoefficient2() ? GetCoefficient2().ToString() : "Not Provided");
      str += "\n\tCoefficient3: " + (HasCoefficient3() ? GetCoefficient3().ToString() : "Not Provided");
      str += "\n\tCoefficient4: " + (HasCoefficient4() ? GetCoefficient4().ToString() : "Not Provided");

      return str;
    }
  }
}
