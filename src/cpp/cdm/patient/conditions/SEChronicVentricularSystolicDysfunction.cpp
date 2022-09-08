/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SEChronicVentricularSystolicDysfunction::SEChronicVentricularSystolicDysfunction(Logger* logger) : SEPatientCondition(logger)
{
}

SEChronicVentricularSystolicDysfunction::~SEChronicVentricularSystolicDysfunction()
{
  Clear();
}

void SEChronicVentricularSystolicDysfunction::Clear()
{
  SEPatientCondition::Clear();
}

void SEChronicVentricularSystolicDysfunction::Copy(const SEChronicVentricularSystolicDysfunction& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEChronicVentricularSystolicDysfunction::IsValid() const
{
  return true;
}

bool SEChronicVentricularSystolicDysfunction::IsActive() const
{
  return IsValid();
}
