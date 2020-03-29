/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/PatientActions.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBUtils.h"
#include "patient/actions/SEBrainInjury.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SESubstanceAdministration.h"
#include "patient/actions/SESupplementalOxygen.h"

const std::string& eBrainInjury_Type_Name(eBrainInjury_Type m)
{
  return CDM_BIND::BrainInjuryData::eType_Name((CDM_BIND::BrainInjuryData::eType)m);
}

const std::string& eHemorrhage_Type_Name(eHemorrhage_Type m)
{
  return CDM_BIND::HemorrhageData::eType_Name((CDM_BIND::HemorrhageData::eType)m);
}

const std::string& eIntubation_Type_Name(eIntubation_Type m)
{
  return CDM_BIND::IntubationData::eType_Name((CDM_BIND::IntubationData::eType)m);
}

const std::string& eSubstanceAdministration_Route_Name(eSubstanceAdministration_Route m)
{
  return CDM_BIND::SubstanceBolusData::eRoute_Name((CDM_BIND::SubstanceBolusData::eRoute)m);
}

const std::string& eSupplementalOxygen_Device_Name(eSupplementalOxygen_Device m)
{
  return CDM_BIND::SupplementalOxygenData::eDevice_Name((CDM_BIND::SupplementalOxygenData::eDevice)m);
}