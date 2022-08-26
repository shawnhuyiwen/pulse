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
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEOverrides& src);

  virtual bool IsValid() const;

  virtual bool HasScalarProperties() const;
  virtual SEScalarProperties& GetScalarProperties();
  virtual const SEScalarProperties& GetScalarProperties() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalarProperties m_ScalarProperties;
};