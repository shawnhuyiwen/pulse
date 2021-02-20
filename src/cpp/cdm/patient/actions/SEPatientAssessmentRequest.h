/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
#include "patient/assessments/SEPatientAssessment.h"

class CDM_DECL SEPatientAssessmentRequest : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEPatientAssessmentRequest(Logger* logger=nullptr);
  virtual ~SEPatientAssessmentRequest();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEPatientAssessmentRequest& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual ePatientAssessment_Type GetType() const;
  virtual void SetType(ePatientAssessment_Type type);

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);
  
protected:

  ePatientAssessment_Type m_Type;
}; 