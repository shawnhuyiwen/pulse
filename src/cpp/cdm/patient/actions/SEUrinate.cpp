/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEUrinate.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEUrinate::SEUrinate(Logger* logger) : SEPatientAction(logger)
{

}

SEUrinate::~SEUrinate()
{

}

void SEUrinate::Clear()
{
  SEPatientAction::Clear();
}

DISABLE_UNREFERENCED_FORMAL_PARAMETER
void SEUrinate::Copy(const SEUrinate& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}
DISABLE_WARNING_POP

bool SEUrinate::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SEUrinate::IsActive() const
{
  return SEPatientAction::IsActive();
}
void SEUrinate::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

DISABLE_UNREFERENCED_FORMAL_PARAMETER
const SEScalar* SEUrinate::GetScalar(const std::string& name)
{
  return nullptr;
}
DISABLE_WARNING_POP

void SEUrinate::ToString(std::ostream &str) const
{
  str << "Patient Action : Urinate"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << std::flush;
}
