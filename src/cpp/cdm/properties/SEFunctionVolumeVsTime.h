/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEFunction.h"

class CDM_DECL SEFunctionVolumeVsTime : public SEFunction
{
  friend class PBProperty;//friend the serialization class
public:

  SEFunctionVolumeVsTime();
  virtual ~SEFunctionVolumeVsTime();

  virtual void Clear();
  void Copy(const SEFunction& src) = delete;
  void Copy(const SEFunctionVolumeVsTime& src);

  double                       GetIndependentValue(size_t index) = delete;
  const CCompoundUnit*         GetIndependentUnit() const override {return (CCompoundUnit*)m_TimeUnit;}
  double                       GetTimeValue(size_t index, const TimeUnit& unit) const;
  std::vector<double>&         GetTime();
  const TimeUnit*              GetTimeUnit() const;
  void                         SetTimeUnit(const TimeUnit& unit);

  double                       GetDependentValue(size_t index) = delete;
  const CCompoundUnit*         GetDependentUnit() const override {return (CCompoundUnit*)m_VolumeUnit;}
  double                       GetVolumeValue(size_t index, const VolumeUnit& unit) const;
  std::vector<double>&         GetVolume();
  const VolumeUnit*            GetVolumeUnit() const;
  void                         SetVolumeUnit(const VolumeUnit& unit);

protected:

  const TimeUnit*   m_TimeUnit;
  const VolumeUnit* m_VolumeUnit;
};
