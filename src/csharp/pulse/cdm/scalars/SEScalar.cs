/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;

namespace Pulse.CDM
{
  public class SEScalar : SEProperty
  {
    protected double value;

    public SEScalar() : base()
    {
      Invalidate();
    }

    public SEScalar(double val) : base()
    {
      Invalidate();
      SetValue(val);
    }

    public void Set(SEScalar from)
    {
      if (from == null)
      {
        Invalidate();
        return;
      }
      SetValue(from.value);
    }

    public virtual bool IsValid()
    {
      if (Double.NaN == this.value)
        return false;
      return true;
    }

    public virtual void Invalidate()
    {
      value = Double.NaN;
    }

    public virtual double GetValue()
    {
      return value;
    }

    public virtual void SetValue(double val)
    {
      value = val;
    }

    public static bool IsValue(double a, double b, double epsilon = 1e-10)
    {
      const double MinNormal = 2.2250738585072014E-308d;
      double absA = Math.Abs(a);
      double absB = Math.Abs(b);
      double diff = Math.Abs(a - b);

      if (a.Equals(b))
      { // shortcut, handles infinities
        return true;
      }
      else if (a == 0 || b == 0 || absA + absB < MinNormal)
      {
        // a or b is zero or both are extremely close to it
        // relative error is less meaningful here
        return diff < (epsilon * MinNormal);
      }
      else
      { // use relative error
        return diff / (absA + absB) < epsilon;
      }
    }
  }
}
