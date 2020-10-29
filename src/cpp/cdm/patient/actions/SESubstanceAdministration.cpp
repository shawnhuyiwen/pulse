/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SESubstanceAdministration.h"
#include "io/protobuf/PBPatientActions.h"

SESubstanceAdministration::SESubstanceAdministration(Logger* logger) : SEPatientAction(logger)
{
}

SESubstanceAdministration::~SESubstanceAdministration()
{

}

void SESubstanceAdministration::Clear()
{
  SEPatientAction::Clear();
}

bool SESubstanceAdministration::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SESubstanceAdministration::IsActive() const
{
  return IsValid();
}