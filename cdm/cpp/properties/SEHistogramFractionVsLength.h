/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEHistogram.h"

class CDM_DECL SEHistogramFractionVsLength : public SEHistogram
{
  friend class PBProperty;//friend the serialization class
public:

  SEHistogramFractionVsLength();
  virtual ~SEHistogramFractionVsLength();

  virtual void Clear();

  virtual bool IsVaild() const;

  virtual double                     GetFractionValue(size_t index) const;
  virtual std::vector<double>&       GetFraction();
  virtual const std::vector<double>& GetFraction() const;
  
  virtual double                     GetLengthValue(size_t index, const LengthUnit& unit)  const;
  virtual std::vector<double>&       GetLength();
  virtual const std::vector<double>& GetLength() const;
  virtual const LengthUnit*          GetLengthUnit()  const;
  virtual void                       SetLengthUnit(const LengthUnit& unit);

protected:
  const LengthUnit* m_LengthUnit;
};