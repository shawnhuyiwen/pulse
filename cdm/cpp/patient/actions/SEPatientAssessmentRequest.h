/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
CDM_BIND_DECL(PatientAssessmentRequestData)
#include "bind/cdm/PatientAssessmentEnums.pb.h"

class CDM_DECL SEPatientAssessmentRequest : public SEPatientAction
{
public:

  SEPatientAssessmentRequest();
  virtual ~SEPatientAssessmentRequest();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::PatientAssessmentRequestData& src, SEPatientAssessmentRequest& dst);
  static cdm::PatientAssessmentRequestData* Unload(const SEPatientAssessmentRequest& src);
protected:
  static void Serialize(const cdm::PatientAssessmentRequestData& src, SEPatientAssessmentRequest& dst);
  static void Serialize(const SEPatientAssessmentRequest& src, cdm::PatientAssessmentRequestData& dst);

public:

  virtual cdm::ePatientAssessment_Type GetType() const;
  virtual void SetType(cdm::ePatientAssessment_Type type);

  virtual void ToString(std::ostream &str) const;
  
protected:

  cdm::ePatientAssessment_Type m_Type;
}; 