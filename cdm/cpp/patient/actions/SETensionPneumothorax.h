/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
CDM_BIND_DECL(TensionPneumothoraxData)

class CDM_DECL SETensionPneumothorax : public SEPatientAction
{
public:

  SETensionPneumothorax();
  virtual ~SETensionPneumothorax();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::TensionPneumothoraxData& src, SETensionPneumothorax& dst);
  static cdm::TensionPneumothoraxData* Unload(const SETensionPneumothorax& src);
protected:
  static void Serialize(const cdm::TensionPneumothoraxData& src, SETensionPneumothorax& dst);
  static void Serialize(const SETensionPneumothorax& src, cdm::TensionPneumothoraxData& dst);

public:

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

  virtual void ToString(std::ostream &str) const;

protected:
  eGate    m_Type;
  eSide    m_Side;
  SEScalar0To1* m_Severity;
  
};    