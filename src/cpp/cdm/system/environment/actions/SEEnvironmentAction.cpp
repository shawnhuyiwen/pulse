/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/environment/actions/SEEnvironmentAction.h"

SEEnvironmentAction::SEEnvironmentAction(Logger* logger) : SEAction(logger)
{

}

SEEnvironmentAction::~SEEnvironmentAction()
{

}

void SEEnvironmentAction::Clear()
{
  SEAction::Clear();
}

bool SEEnvironmentAction::IsValid() const
{
  return SEAction::IsValid();
}