/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/SEAction.h"
#include "properties/SEScalar.h"

class CDM_DECL SEOverrides : public SEAction
{
  friend class PBAction;//friend the serialization class
public:

  SEOverrides(Logger* logger=nullptr);
  virtual ~SEOverrides();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEOverrides& src);

  virtual bool IsValid() const;

  virtual bool HasProperty() const;
  virtual void AddScalarProperty(const std::string& name, double value);
  virtual void AddScalarProperty(const std::string& name, double value, std::string unit);
  virtual void AddScalarProperty(const std::string& name, double value, const CCompoundUnit& unit);
  virtual std::vector<SEScalarProperty>& GetScalarProperties();
  virtual const std::vector<SEScalarProperty>& GetScalarProperties() const;
  virtual void RemoveProperties();

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  std::vector<SEScalarProperty> m_ScalarProperties;
};