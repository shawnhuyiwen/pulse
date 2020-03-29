/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Patient.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBUtils.h"
#include "patient/SEPatient.h"

const std::string& ePatient_Sex_Name(ePatient_Sex m)
{
  return CDM_BIND::PatientData::eSex_Name((CDM_BIND::PatientData::eSex)m);
}

