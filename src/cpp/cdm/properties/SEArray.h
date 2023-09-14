/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEProperty.h"

class CDM_DECL SEArray : public SEProperty
{
  friend class PBProperty;//friend the serialization class
public:
  SEArray();
  ~SEArray();

  virtual void Clear();
  void Copy(const SEArray& src);

  bool                          IsValid() const override;
  void                          Invalidate() override;

  std::vector<double>&                  GetData();

protected:
  std::vector<double> m_Data;
};
