/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cpp/cdm/Patient.pb.h"
#include "patient/SEPatient.h"

const std::string& ePatient_Sex_Name(ePatient_Sex m)
{
  return cdm::PatientData::eSex_Name((cdm::PatientData::eSex)m);
}

