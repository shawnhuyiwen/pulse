/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
CDM_BIND_DECL(IntubationData)
PROTO_PUSH
#include "bind/cdm/PatientActionEnums.pb.h"
PROTO_POP

class CDM_DECL SEIntubation : public SEPatientAction
{
public:

  SEIntubation();
  virtual ~SEIntubation();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::IntubationData& src, SEIntubation& dst);
  static cdm::IntubationData* Unload(const SEIntubation& src);
protected:
  static void Serialize(const cdm::IntubationData& src, SEIntubation& dst);
  static void Serialize(const SEIntubation& src, cdm::IntubationData& dst);

public:
    
  virtual cdm::eIntubation_Type GetType() const;
  virtual void SetType(cdm::eIntubation_Type t);

  virtual void ToString(std::ostream &str) const;
protected:
  cdm::eIntubation_Type m_Type;
};  