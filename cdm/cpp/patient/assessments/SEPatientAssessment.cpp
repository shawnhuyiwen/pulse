/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/assessments/SEPatientAssessment.h"
#include "bind/cdm/PatientAssessments.pb.h"

SEPatientAssessment::SEPatientAssessment(Logger* logger) : Loggable(logger)
{
  
}

SEPatientAssessment::~SEPatientAssessment()
{
  Clear();
}

void SEPatientAssessment::Clear()
{
  
}
