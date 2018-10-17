/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
CDM_BIND_DECL(IntubationData)

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eIntubation_Type { Off = 0, Esophageal, LeftMainstem, RightMainstem, Tracheal };
extern const std::string& eIntubation_Type_Name(eIntubation_Type m);

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
    
  virtual eIntubation_Type GetType() const;
  virtual void SetType(eIntubation_Type t);

  virtual void ToString(std::ostream &str) const;
protected:
  eIntubation_Type m_Type;
};  