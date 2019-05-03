/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEProperty.h"

class CDM_DECL SEHistogram : public SEProperty
{
  friend class PBProperty;//friend the serialization class
public:

  SEHistogram();
  virtual ~SEHistogram();

  virtual void Clear(); //clear memory
  void Copy(const SEHistogram& s);

  virtual bool                          IsValid() const;
  virtual void                          Invalidate();

  virtual size_t                        NumberOfBins() const;
  virtual size_t                        NumberOfBoundaries() const;
  
  double                                GetDependentValue(size_t index) const;
  std::vector<double>&                  GetDependent();      
  const std::vector<double>&            GetDependent() const;

  double                                GetIndependentValue(size_t index) const;
  std::vector<double>&                  GetIndependent();
  const std::vector<double>&            GetIndependent() const;
  
protected:

  std::vector<double> m_Dependent;
  std::vector<double> m_Independent;
};