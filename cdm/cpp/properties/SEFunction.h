/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEProperty.h"

class CDM_DECL SEFunction : public SEProperty
{
  friend class PBProperty;//friend the serialization class
public:

  SEFunction();
  virtual ~SEFunction();

  virtual void Clear(); //clear memory
  void Copy(const SEFunction& s);

  virtual bool                          IsValid() const;
  virtual void                          Invalidate();

  virtual size_t                        Length();
  
  double                                GetDependentValue(size_t index);
  std::vector<double>&                  GetDependent();      

  double                                GetIndependentValue(size_t index);
  std::vector<double>&                  GetIndependent();
  
protected:

  std::vector<double> m_Dependent;
  std::vector<double> m_Independent;
};