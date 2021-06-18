/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "SEProperty.h"
#include "properties/SESegmentConstant.h"
#include "properties/SESegmentLinear.h"
#include "properties/SESegmentParabolic.h"
#include "properties/SESegmentSigmoidal.h"

class CDM_DECL SECurve : public SEProperty
{
public:

  SECurve();
  virtual ~SECurve();

  virtual void Invalidate();
  virtual bool IsValid() const override;

  virtual bool HasSegment() const;

  SESegmentConstant& AddConstantSegment();
  SESegmentLinear& AddLinearSegment();
  SESegmentParabolic& AddParabolicSegment();
  SESegmentSigmoidal& AddSigmoidalSegment();

  virtual const std::vector<SESegment*>& GetSegments();
  virtual const std::vector<const SESegment*>& GetSegments() const;

  virtual std::string ToString() const;
  virtual void ToString(std::ostream& str) const;

protected:

  std::vector<SESegment*> m_Segments;
};
