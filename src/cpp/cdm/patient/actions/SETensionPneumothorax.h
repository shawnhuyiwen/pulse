/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SETensionPneumothorax : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SETensionPneumothorax(Logger* logger=nullptr);
  virtual ~SETensionPneumothorax();

  static constexpr char const* Name = "Tension Pneumothorax";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SETensionPneumothorax& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eGate GetType() const;
  virtual void SetType(eGate name);
  virtual bool HasType() const;
  virtual void InvalidateType();
  
  virtual eSide GetSide() const;
  virtual void SetSide(eSide name);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

protected:
  eGate    m_Type;
  eSide    m_Side;
  SEScalar0To1* m_Severity;
  
};
