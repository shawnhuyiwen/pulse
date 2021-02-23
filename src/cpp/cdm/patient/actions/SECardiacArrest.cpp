/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SECardiacArrest.h"
#include "io/protobuf/PBPatientActions.h"

SECardiacArrest::SECardiacArrest(Logger* logger) : SEPatientAction(logger)
{
  m_State = eSwitch::Off;
}

SECardiacArrest::~SECardiacArrest()
{
  m_State = eSwitch::Off;
}

void SECardiacArrest::Clear()
{
  SEPatientAction::Clear();
  m_State = eSwitch::Off;
}

void SECardiacArrest::Copy(const SECardiacArrest& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SECardiacArrest::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SECardiacArrest::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return m_State == eSwitch::On;
}
void SECardiacArrest::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SECardiacArrest::GetScalar(const std::string& name)
{
  return nullptr;
}

void SECardiacArrest::ToString(std::ostream &str) const
{
  str << "Patient Action : Cardiac Arrest";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State);
  str << std::flush;
}