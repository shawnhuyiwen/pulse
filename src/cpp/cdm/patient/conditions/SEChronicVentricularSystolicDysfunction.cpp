/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "io/protobuf/PBPatientConditions.h"

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

void SEChronicVentricularSystolicDysfunction::ToString(std::ostream &str) const
{
  str << "Patient Condition : Ventricular Systolic Dysfunction Heart Failure"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << std::flush;
}