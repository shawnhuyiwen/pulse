/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEImpairedAlveolarExchangeExacerbation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEImpairedAlveolarExchangeExacerbation(Logger* logger=nullptr);
  virtual ~SEImpairedAlveolarExchangeExacerbation();

  static constexpr char const* Name = "Impaired Alveolar Exchange Exacerbation";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEImpairedAlveolarExchangeExacerbation& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasImpairedSurfaceArea() const;
  virtual SEScalarArea& GetImpairedSurfaceArea();
  virtual double GetImpairedSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasImpairedFraction() const;
  virtual SEScalar0To1& GetImpairedFraction();
  virtual double GetImpairedFraction() const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

protected:
  
  SEScalarArea*      m_ImpairedSurfaceArea;
  SEScalar0To1*      m_ImpairedFraction;
  SEScalar0To1*      m_Severity;
};
