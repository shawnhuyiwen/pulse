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
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEPatientAssessmentRequest& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual ePatientAssessment_Type GetType() const;
  virtual void SetType(ePatientAssessment_Type type);
  
protected:

  ePatientAssessment_Type m_Type;
}; 