/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/engine/SEAction.h"

class CDM_DECL SEOverrides : public SEAction
{
  friend class PBAction;//friend the serialization class
public:

  SEOverrides(Logger* logger=nullptr);
  virtual ~SEOverrides();

  static constexpr char const* Name = "Overrides";
  std::string GetName() const override{ return Name; }

  void Clear() override; //clear memory
  virtual void Copy(const SEOverrides& src);

  bool IsValid() const override;

  virtual bool HasScalarProperties() const;
  virtual SEScalarProperties& GetScalarProperties();
  virtual const SEScalarProperties& GetScalarProperties() const;

  const SEScalar* GetScalar(const std::string& name) override;

protected:
  SEScalarProperties m_ScalarProperties;
};