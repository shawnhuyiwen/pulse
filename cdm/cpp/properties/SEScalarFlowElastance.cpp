/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarFlowElastance.h"

const FlowElastanceUnit FlowElastanceUnit::cmH2O_Per_L("cmH2O/L");
const FlowElastanceUnit FlowElastanceUnit::mmHg_Per_mL("mmHg/mL");
const FlowElastanceUnit FlowElastanceUnit::Pa_Per_m3("Pa/m^3");

bool FlowElastanceUnit::IsValidUnit(const std::string& unit)
{
  if (cmH2O_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mmHg_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (Pa_Per_m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const FlowElastanceUnit& FlowElastanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (cmH2O_Per_L.GetString().compare(unit) == 0)
    return cmH2O_Per_L;
  if (mmHg_Per_mL.GetString().compare(unit) == 0)
    return mmHg_Per_mL;
  if (Pa_Per_m3.GetString().compare(unit) == 0)
    return Pa_Per_m3;
  std::stringstream err;
  err << unit << " is not a valid FlowElastance unit";
  throw CommonDataModelException(err.str());
}
