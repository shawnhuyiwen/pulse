/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eBrainInjury_Type { Diffuse = 0, LeftFocal, RightFocal };
extern const std::string& eBrainInjury_Type_Name(eBrainInjury_Type m);

class CDM_DECL SEBrainInjury : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEBrainInjury(Logger* logger=nullptr);
  virtual ~SEBrainInjury();

  virtual void Clear();
  virtual void Copy(const SEBrainInjury& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual eBrainInjury_Type GetType() const;
  virtual void SetType(eBrainInjury_Type t);

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1*          m_Severity;
  eBrainInjury_Type      m_Type;
};