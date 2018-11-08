/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cdm/PatientActionEnums.pb.h"
#include "patient/actions/SEBrainInjury.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SESubstanceAdministration.h"


const std::string& eBrainInjury_Type_Name(eBrainInjury_Type m)
{
  return cdm::eBrainInjury_Type_Name((cdm::eBrainInjury_Type)m);
}

const std::string& eHemorrhage_Type_Name(eHemorrhage_Type m)
{
  return cdm::eHemorrhage_Type_Name((cdm::eHemorrhage_Type)m);
}

const std::string& eIntubation_Type_Name(eIntubation_Type m)
{
  return cdm::eIntubation_Type_Name((cdm::eIntubation_Type)m);
}

const std::string& eSubstanceAdministration_Route_Name(eSubstanceAdministration_Route m)
{
  return cdm::eSubstanceAdministration_Route_Name((cdm::eSubstanceAdministration_Route)m);
}