/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SECardiacArrest.h"
#include "io/protobuf/PBPatientActions.h"

SECardiacArrest::SECardiacArrest() : SEPatientAction()
{
  m_State = eSwitch::Off;
}

SECardiacArrest::~SECardiacArrest()
{
  Clear();
}

void SECardiacArrest::Clear()
{
  SEPatientAction::Clear();
  m_State = eSwitch::Off;
}

void SECardiacArrest::Copy(const SECardiacArrest& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SECardiacArrest::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SECardiacArrest::IsActive() const
{
  return IsValid() && m_State == eSwitch::On;
}

void SECardiacArrest::ToString(std::ostream &str) const
{
  str << "Patient Action : Cardiac Arrest";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State);;
  str << std::flush;
}