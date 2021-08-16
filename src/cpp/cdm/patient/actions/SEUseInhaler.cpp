/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEUseInhaler.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/io/protobuf/PBPatientActions.h"

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