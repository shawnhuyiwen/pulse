/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEIntubation.h"
#include "bind/cdm/PatientActions.pb.h"
#include "bind/cdm/PatientActionEnums.pb.h"

const std::string& eIntubation_Type_Name(eIntubation_Type m)
{
  return cdm::eIntubation_Type_Name((cdm::eIntubation_Type)m);
}

SEIntubation::SEIntubation() : SEPatientAction()
{
  m_Type = eIntubation_Type::Off;
}

SEIntubation::~SEIntubation()
{
  Clear();
}

void SEIntubation::Clear()
{
  SEPatientAction::Clear();
  m_Type = eIntubation_Type::Off;
}

bool SEIntubation::IsValid() const
{
  return true;
}

bool SEIntubation::IsActive() const
{
  return GetType() != eIntubation_Type::Off;
}

void SEIntubation::Load(const cdm::IntubationData& src, SEIntubation& dst)
{
  SEIntubation::Serialize(src, dst);
}
void SEIntubation::Serialize(const cdm::IntubationData& src, SEIntubation& dst)
{
  SEPatientAction::Serialize(src.patientaction(), dst);
  dst.SetType((eIntubation_Type)src.type());
}

cdm::IntubationData* SEIntubation::Unload(const SEIntubation& src)
{
  cdm::IntubationData* dst = new cdm::IntubationData();
  SEIntubation::Serialize(src, *dst);
  return dst;
}
void SEIntubation::Serialize(const SEIntubation& src, cdm::IntubationData& dst)
{
  SEPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_type((cdm::eIntubation_Type)src.m_Type);
}

eIntubation_Type SEIntubation::GetType() const
{
  return m_Type;
}
void SEIntubation::SetType(eIntubation_Type Type)
{
  m_Type = Type;
}

void SEIntubation::ToString(std::ostream &str) const
{
  str << "Patient Action : Intubation";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tType: " << eIntubation_Type_Name(GetType());
  str << std::flush;
}
