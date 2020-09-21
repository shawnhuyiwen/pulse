/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEUrinate.h"
#include "io/protobuf/PBPatientActions.h"

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

void SEUrinate::Copy(const SEUrinate& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEUrinate::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SEUrinate::IsActive() const
{
  return IsValid();
}

void SEUrinate::ToString(std::ostream &str) const
{
  str << "Patient Action : Urinate"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << std::flush;
}
