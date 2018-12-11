/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cpp/cdm/PatientEnums.pb.h"
#include "patient/SEPatient.h"

const std::string& ePatient_Sex_Name(ePatient_Sex m)
{
  return cdm::ePatient_Sex_Name((cdm::ePatient_Sex)m);
}

const std::string& ePatient_Event_Name(ePatient_Event m)
{
  return cdm::ePatient_Event_Name((cdm::ePatient_Event)m);
}
