/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
PROTO_PUSH
#include "bind/cdm/PatientActionEnums.pb.h"
PROTO_POP
CDM_BIND_DECL(BrainInjuryData)

class CDM_DECL SEBrainInjury : public SEPatientAction
{
public:

  SEBrainInjury();
  virtual ~SEBrainInjury();

  virtual void Clear();

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::BrainInjuryData& src, SEBrainInjury& dst);
  static cdm::BrainInjuryData* Unload(const SEBrainInjury& src);
protected:
  static void Serialize(const cdm::BrainInjuryData& src, SEBrainInjury& dst);
  static void Serialize(const SEBrainInjury& src, cdm::BrainInjuryData& dst);

public:

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();  

  virtual cdm::eBrainInjury_Type GetType() const;
  virtual void SetType(cdm::eBrainInjury_Type t);

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1*               m_Severity;
  cdm::eBrainInjury_Type      m_Type;
};