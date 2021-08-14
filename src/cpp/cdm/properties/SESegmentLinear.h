/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/properties/SESegment.h"

class CDM_DECL SESegmentLinear : public SESegment
{
protected:
  friend class PBProperty;//friend the serialization class
  friend class SECurve;
  SESegmentLinear();
public:
  virtual ~SESegmentLinear();

  virtual void Clear() override;
  virtual bool IsValid() const override;

  virtual bool HasSlope() const;
  virtual SEScalarPressurePerVolume& GetSlope();
  virtual double GetSlope(const PressurePerVolumeUnit& unit) const;

  virtual bool HasYIntercept() const;
  virtual SEScalarPressure& GetYIntercept();
  virtual double GetYIntercept(const PressureUnit& unit) const;

  virtual std::string ToString() const override;
  virtual void ToString(std::ostream& str) const override;

protected:
  virtual void Abstract() override { };

  SEScalarPressurePerVolume*     m_Slope;
  SEScalarPressure*              m_YIntercept;
};
