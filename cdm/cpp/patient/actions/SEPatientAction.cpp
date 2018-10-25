/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEPatientAction.h"

SEPatientAction::SEPatientAction() : SEAction()
{
  
}

SEPatientAction::~SEPatientAction()
{
  Clear();
}

void SEPatientAction::Clear()
{
  SEAction::Clear();
}

bool SEPatientAction::IsValid() const
{
  return SEAction::IsValid();
}
