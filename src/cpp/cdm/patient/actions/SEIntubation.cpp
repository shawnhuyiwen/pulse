/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEIntubation.h"
#include "io/protobuf/PBPatientActions.h"

SEIntubation::SEIntubation(Logger* logger) : SEPatientAction(logger)
{
  m_Type = eIntubation_Type::Off;
}

SEIntubation::~SEIntubation()
{
  m_Type = eIntubation_Type::Off;
}

void SEIntubation::Clear()
{
  SEPatientAction::Clear();
  m_Type = eIntubation_Type::Off;
}

void SEIntubation::Copy(const SEIntubation& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}
bool SEIntubation::IsValid() const
{
  return true;
}

bool SEIntubation::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return GetType() != eIntubation_Type::Off;
}
void SEIntubation::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEIntubation::GetScalar(const std::string& name)
{
  return nullptr;
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
