/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "properties/SESegment.h"

class CDM_DECL SESegmentSigmoidal : public SESegment
{
protected:
  friend class PBProperty;//friend the serialization class
  friend class SECurve;
  SESegmentSigmoidal();
public:
  virtual ~SESegmentSigmoidal();

  virtual void Clear() override;
  virtual bool IsValid() const override;

  virtual bool HasLowerCorner() const;
  virtual SEScalarPressure& GetLowerCorner();
  virtual double GetLowerCorner(const PressureUnit& unit) const;

  virtual bool HasUpperCorner() const;
  virtual SEScalarPressure& GetUpperCorner();
  virtual double GetUpperCorner(const PressureUnit& unit) const;

  virtual bool HasBaselineCompliance() const;
  virtual SEScalarVolumePerPressure& GetBaselineCompliance();
  virtual double GetBaselineCompliance(const VolumePerPressureUnit& unit) const;

  virtual std::string ToString() const override;
  virtual void ToString(std::ostream& str) const override;

protected:
  virtual void Abstract() override { };

  SEScalarPressure*              m_LowerCorner;
  SEScalarPressure*              m_UpperCorner;
  SEScalarVolumePerPressure*     m_BaselineCompliance;
};
