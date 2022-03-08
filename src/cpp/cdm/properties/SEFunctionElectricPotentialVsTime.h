/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEFunction.h"

class CDM_DECL SEFunctionElectricPotentialVsTime : public SEFunction
{
  friend class PBProperty;//friend the serialization class
public:

  SEFunctionElectricPotentialVsTime();
  virtual ~SEFunctionElectricPotentialVsTime();

  void Clear() override;
  void Copy(const SEFunction& src) = delete;
  void Copy(const SEFunctionElectricPotentialVsTime& src);

  double                               GetIndependentValue(size_t index) = delete;
  const CCompoundUnit*                 GetIndependentUnit() const override {return (CCompoundUnit*)m_TimeUnit;}
  double                               GetTimeValue(size_t index, const TimeUnit& unit) const;
  std::vector<double>&                 GetTime();
  const TimeUnit*                      GetTimeUnit() const;
  void                                 SetTimeUnit(const TimeUnit& unit);

  double                               GetDependentValue(size_t index) = delete;
  const CCompoundUnit*                 GetDependentUnit() const override {return (CCompoundUnit*)m_ElectricPotentialUnit;}
  double                               GetElectricPotentialValue(size_t index, const ElectricPotentialUnit& unit) const;
  std::vector<double>&                 GetElectricPotential();
  const ElectricPotentialUnit*         GetElectricPotentialUnit() const;
  void                                 SetElectricPotentialUnit(const ElectricPotentialUnit& unit);

protected:

  const TimeUnit*              m_TimeUnit;
  const ElectricPotentialUnit* m_ElectricPotentialUnit;
};
