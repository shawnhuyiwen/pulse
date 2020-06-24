/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "patient/conditions/SEPatientCondition.h"

class CDM_DECL SEImpairedAlveolarExchange : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEImpairedAlveolarExchange(Logger* logger=nullptr);
  virtual ~SEImpairedAlveolarExchange();
  
  virtual void Clear();
  virtual void Copy(const SEImpairedAlveolarExchange& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const { return "ImpairedAlveolarExchange"; }

  virtual bool HasImpairedSurfaceArea() const;
  virtual SEScalarArea& GetImpairedSurfaceArea();
  virtual double GetImpairedSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasImpairedFraction() const;
  virtual SEScalar0To1& GetImpairedFraction();
  virtual double GetImpairedFraction() const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream &str) const;

protected:
  
  SEScalarArea*      m_ImpairedSurfaceArea;
  SEScalar0To1*      m_ImpairedFraction;
  SEScalar0To1*      m_Severity;
};
