/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"
#include "cdm/patient/assessments/SEPatientAssessment.h"

class CDM_DECL SEPatientAssessmentRequest : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEPatientAssessmentRequest(Logger* logger=nullptr);
  virtual ~SEPatientAssessmentRequest();

  static constexpr char const* Name = "Patient Assessment Request";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEPatientAssessmentRequest& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual ePatientAssessment_Type GetType() const;
  virtual void SetType(ePatientAssessment_Type type);

  virtual const SEScalar* GetScalar(const std::string& name);
  
protected:

  ePatientAssessment_Type m_Type;
}; 