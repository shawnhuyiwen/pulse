/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eIntubation_Type { Off = 0, Esophageal, LeftMainstem, RightMainstem, Tracheal };
extern const std::string& eIntubation_Type_Name(eIntubation_Type m);

class CDM_DECL SEIntubation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEIntubation(Logger* logger=nullptr);
  virtual ~SEIntubation();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEIntubation& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eIntubation_Type GetType() const;
  virtual void SetType(eIntubation_Type t);

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eIntubation_Type m_Type;
};  