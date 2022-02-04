/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEProperty.h"

class CDM_DECL SEFunction : public SEProperty
{
  friend class PBProperty;//friend the serialization class
public:

  SEFunction();
  virtual ~SEFunction();

  virtual void Clear(); //clear memory
  void Copy(const SEFunction& src);

  bool WriteCSVFile(const std::string& filename) const;

  virtual bool                          IsValid() const;
  virtual void                          Invalidate();

  virtual size_t                        Length() const;
  
  double                                GetDependentValue(size_t index) const;
  std::vector<double>&                  GetDependent();
  virtual const CCompoundUnit*          GetDependentUnit() const {return nullptr;}

  double                                GetIndependentValue(size_t index) const;
  std::vector<double>&                  GetIndependent();
  virtual const CCompoundUnit*          GetIndependentUnit() const {return nullptr;}

protected:

  std::vector<double> m_Dependent;
  std::vector<double> m_Independent;
};