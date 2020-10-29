/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEUseInhaler.h"
#include "substance/SESubstance.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarVolume.h"
#include "io/protobuf/PBPatientActions.h"

SEUseInhaler::SEUseInhaler(Logger* logger) : SEConsciousRespirationCommand(logger)
{
  
}

SEUseInhaler::~SEUseInhaler()
{

}

void SEUseInhaler::Clear()
{
  SEConsciousRespirationCommand::Clear();
}

void SEUseInhaler::Copy(const SEUseInhaler& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEUseInhaler::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid();
}

bool SEUseInhaler::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

void SEUseInhaler::ToString(std::ostream &str) const
{
  str << "Use Inhaler";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;  
  str << std::flush;
}