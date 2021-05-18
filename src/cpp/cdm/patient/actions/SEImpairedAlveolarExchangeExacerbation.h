/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEImpairedAlveolarExchangeExacerbation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEImpairedAlveolarExchangeExacerbation(Logger* logger=nullptr);
  virtual ~SEImpairedAlveolarExchangeExacerbation();
  
  virtual void Clear();
  virtual void Copy(const SEImpairedAlveolarExchangeExacerbation& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

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

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  
  SEScalarArea*      m_ImpairedSurfaceArea;
  SEScalar0To1*      m_ImpairedFraction;
  SEScalar0To1*      m_Severity;
};
