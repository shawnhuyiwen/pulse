/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "properties/SESegment.h"

class CDM_DECL SESegmentConstant : public SESegment
{
protected:
  friend class SECurve;
  SESegmentConstant();
public:
  virtual ~SESegmentConstant();

  virtual void Clear() override;
  virtual bool IsValid() const override;

  virtual bool HasCompliance() const;
  virtual SEScalarPressureTimePerVolume& GetCompliance();
  virtual double GetCompliance(const PressureTimePerVolumeUnit& unit) const;

  virtual std::string ToString() const override;
  virtual void ToString(std::ostream& str) const override;

protected:
  virtual void Abstract() override { };

  SEScalarPressureTimePerVolume* m_Compliance;
};
