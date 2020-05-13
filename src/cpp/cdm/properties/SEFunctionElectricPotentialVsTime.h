/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEFunction.h"

class CDM_DECL SEFunctionElectricPotentialVsTime : public SEFunction
{
  friend class PBProperty;//friend the serialization class
public:

  SEFunctionElectricPotentialVsTime();
  virtual ~SEFunctionElectricPotentialVsTime();

  virtual void Clear();

  double                               GetIndependentValue(size_t index) = delete;
  virtual double                       GetTimeValue(size_t index, const TimeUnit& unit);
  virtual std::vector<double>&         GetTime();
  virtual const TimeUnit*              GetTimeUnit();
  virtual void                         SetTimeUnit(const TimeUnit& unit);

  double                               GetDependentValue(size_t index) = delete;
  virtual double                       GetElectricPotentialValue(size_t index, const ElectricPotentialUnit& unit);
  virtual std::vector<double>&         GetElectricPotential();
  virtual const ElectricPotentialUnit* GetElectricPotentialUnit();
  virtual void                         SetElectricPotentialUnit(const ElectricPotentialUnit& unit);

protected:

  const TimeUnit*              m_TimeUnit;
  const ElectricPotentialUnit* m_ElectricPotentialUnit;
};