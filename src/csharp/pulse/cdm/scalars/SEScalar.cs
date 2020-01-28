/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;


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
}
