/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorHold.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorHold::SEMechanicalVentilatorHold(Logger* logger) : SEMechanicalVentilatorAction(logger)
{
  m_AppliedRespiratoryCycle = eAppliedRespiratoryCycle::Expiratory;
  m_State = eSwitch::Off;
}

SEMechanicalVentilatorHold::~SEMechanicalVentilatorHold()
{

}

void SEMechanicalVentilatorHold::Clear()
{
  SEMechanicalVentilatorAction::Clear();
  m_AppliedRespiratoryCycle = eAppliedRespiratoryCycle::Expiratory;
  m_State = eSwitch::Off;
}

void SEMechanicalVentilatorHold::Copy(const SEMechanicalVentilatorHold& src, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEMechanicalVentilatorHold::IsValid() const
{
  return SEMechanicalVentilatorAction::IsValid();
}

bool SEMechanicalVentilatorHold::IsActive() const
{
  if (!SEMechanicalVentilatorAction::IsActive())
    return false;
  return GetState() == eSwitch::On;
}
void SEMechanicalVentilatorHold::Deactivate()
{
  SEMechanicalVentilatorAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEMechanicalVentilatorHold::GetScalar(const std::string& /*name*/)
{
  return nullptr;
}

void SEMechanicalVentilatorHold::SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle c)
{
  m_AppliedRespiratoryCycle = c;
}
eAppliedRespiratoryCycle SEMechanicalVentilatorHold::GetAppliedRespiratoryCycle() const
{
  return m_AppliedRespiratoryCycle;
}

void SEMechanicalVentilatorHold::SetState(eSwitch s)
{
  m_State = s;
}
eSwitch SEMechanicalVentilatorHold::GetState() const
{
  return m_State;
}

void SEMechanicalVentilatorHold::ToString(std::ostream &str) const
{
  SEMechanicalVentilatorAction::ToString(str);
  str << "\n\tAppliedRespiratoryCycle: " << eAppliedRespiratoryCycle_Name(m_AppliedRespiratoryCycle);
  str << "\n\tState: " << eSwitch_Name(m_State);
  str << std::flush;
}
