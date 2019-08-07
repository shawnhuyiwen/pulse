/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/PatientActions.pb.h"
#include "io/protobuf/PBUtils.h"
#include "patient/actions/SEBrainInjury.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SESubstanceAdministration.h"
#include "patient/actions/SESupplementalOxygen.h"

const std::string& eBrainInjury_Type_Name(eBrainInjury_Type m)
{
  return cdm::BrainInjuryData::eType_Name((cdm::BrainInjuryData::eType)m);
}

const std::string& eHemorrhage_Type_Name(eHemorrhage_Type m)
{
  return cdm::HemorrhageData::eType_Name((cdm::HemorrhageData::eType)m);
}

const std::string& eIntubation_Type_Name(eIntubation_Type m)
{
  return cdm::IntubationData::eType_Name((cdm::IntubationData::eType)m);
}

const std::string& eSubstanceAdministration_Route_Name(eSubstanceAdministration_Route m)
{
  return cdm::SubstanceBolusData::eRoute_Name((cdm::SubstanceBolusData::eRoute)m);
}

const std::string& eSupplementalOxygen_Device_Name(eSupplementalOxygen_Device m)
{
  return cdm::SupplementalOxygenData::eDevice_Name((cdm::SupplementalOxygenData::eDevice)m);
}