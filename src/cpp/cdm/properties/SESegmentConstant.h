/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "properties/SESegment.h"

class CDM_DECL SESegmentConstant : public SESegment
{
protected:
  friend class PBProperty;//friend the serialization class
  friend class SECurve;
  SESegmentConstant();
public:
  virtual ~SESegmentConstant();

  virtual void Clear() override;
  virtual bool IsValid() const override;

  virtual bool HasCompliance() const;
  virtual SEScalarVolumePerPressure& GetCompliance();
  virtual double GetCompliance(const VolumePerPressureUnit& unit) const;

  virtual std::string ToString() const override;
  virtual void ToString(std::ostream& str) const override;

protected:
  virtual void Abstract() override { };

  SEScalarVolumePerPressure* m_Compliance;
};
