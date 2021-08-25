/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace Pulse.CDM
{
  public class SECurve : SEProperty
  {

    protected List<SESegment> segments = new List<SESegment>();

    public SECurve() : base()
    {
    }

    public SECurve(SECurve other)
    {
      Copy(other);
    }

    public void Copy(SECurve other)
    {
      base.Copy(other);
      foreach (SESegment s in segments)
      {
        if(s is SESegmentConstant)
          AddConstantSegment().Copy((SESegmentConstant)s);
        else if (s is SESegmentLinear)
          AddConstantSegment().Copy((SESegmentLinear)s);
        else if (s is SESegmentParabolic)
          AddConstantSegment().Copy((SESegmentParabolic)s);
        else if (s is SESegmentSigmoidal)
          AddConstantSegment().Copy((SESegmentSigmoidal)s);
      }
    }

    public override void Invalidate()
    {
      base.Invalidate();
      segments.Clear();
    }

    public override bool IsValid()
    {
      if (!HasSegment())
        return false;
      foreach (SESegment s in segments)
        if (!s.IsValid())
          return false;
      return true;
    }

    public bool HasSegment()
    {
      return segments.Count > 0;
    }

    public SESegmentConstant AddConstantSegment()
    {
      SESegmentConstant s = new SESegmentConstant();
      segments.Add(s);
      return s;
    }

    public SESegmentLinear AddLinearSegment()
    {
      SESegmentLinear s = new SESegmentLinear();
      segments.Add(s);
      return s;
    }

    public SESegmentParabolic AddParabolicSegment()
    {
      SESegmentParabolic s = new SESegmentParabolic();
      segments.Add(s);
      return s;
    }

    public SESegmentSigmoidal AddSigmoidalSegment()
    {
      SESegmentSigmoidal s = new SESegmentSigmoidal();
      segments.Add(s);
      return s;
    }

    public ReadOnlyCollection<SESegment> GetSegments()
    {
      return segments.AsReadOnly();
    }

    public override string ToString()
    {
      string str = "Curve: ";
      foreach (SESegment s in segments)
        str += "\n\t" + s.ToString();
      return str;
    }
  }
}
