/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SECardiacArrest.h"
#include "cdm/io/protobuf/PBPatientActions.h"

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

DISABLE_UNREFERENCED_FORMAL_PARAMETER
void SECardiacArrest::Copy(const SECardiacArrest& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}
DISABLE_WARNING_POP

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

DISABLE_UNREFERENCED_FORMAL_PARAMETER
const SEScalar* SECardiacArrest::GetScalar(const std::string& name)
{
  return nullptr;
}
DISABLE_WARNING_POP

void SECardiacArrest::ToString(std::ostream &str) const
{
  str << "Patient Action : Cardiac Arrest";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State);
  str << std::flush;
}