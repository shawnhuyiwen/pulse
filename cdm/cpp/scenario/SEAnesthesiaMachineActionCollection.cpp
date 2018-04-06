/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEAnesthesiaMachineActionCollection.h"
#include "substance/SESubstanceManager.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesiamachine/actions/SEOxygenTankPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEOxygenWallPortPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEExpiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEExpiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEInspiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEInspiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEMaskLeak.h"
#include "system/equipment/anesthesiamachine/actions/SESodaLimeFailure.h"
#include "system/equipment/anesthesiamachine/actions/SETubeCuffLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEVaporizerFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEVentilatorPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEYPieceDisconnect.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineChamber.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineOxygenBottle.h"
PROTO_PUSH
#include "bind/cdm/Scenario.pb.h"
PROTO_POP
#include "properties/SEScalarVolume.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolumePerTime.h"

SEAnesthesiaMachineActionCollection::SEAnesthesiaMachineActionCollection(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
{
  m_Configuration = nullptr;

  m_OxygenTankPressureLoss = nullptr;
  m_OxygenWallPortPressureLoss = nullptr;

  m_ExpiratoryValveLeak = nullptr;
  m_ExpiratoryValveObstruction = nullptr;
  m_InspiratoryValveLeak = nullptr;
  m_InspiratoryValveObstruction = nullptr;
  m_MaskLeak = nullptr;
  m_SodaLimeFailure = nullptr;
  m_TubeCuffLeak = nullptr;
  m_VaporizerFailure = nullptr;
  m_VentilatorPressureLoss = nullptr;
  m_YPieceDisconnect = nullptr;
}

SEAnesthesiaMachineActionCollection::~SEAnesthesiaMachineActionCollection()
{
  Clear();
}

void SEAnesthesiaMachineActionCollection::Clear()
{
  // State
  RemoveConfiguration();
  // Incidents
  RemoveOxygenTankPressureLoss();
  RemoveOxygenWallPortPressureLoss();
  // Failures
  RemoveExpiratoryValveLeak();
  RemoveExpiratoryValveObstruction();
  RemoveInspiratoryValveLeak();
  RemoveInspiratoryValveObstruction();
  RemoveMaskLeak();
  RemoveSodaLimeFailure();
  RemoveTubeCuffLeak();
  RemoveVaporizerFailure();
  RemoveVentilatorPressureLoss();
  RemoveYPieceDisconnect();
}

void SEAnesthesiaMachineActionCollection::Serialize(const SEAnesthesiaMachineActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasConfiguration())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_Configuration));

  if (src.HasOxygenTankPressureLoss())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_OxygenTankPressureLoss));
  if (src.HasOxygenWallPortPressureLoss())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_OxygenWallPortPressureLoss));

  if (src.HasExpiratoryValveLeak())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_ExpiratoryValveLeak));
  if (src.HasExpiratoryValveObstruction())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_ExpiratoryValveObstruction));
  if (src.HasInspiratoryValveLeak())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_InspiratoryValveLeak));
  if (src.HasInspiratoryValveObstruction())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_InspiratoryValveObstruction));
  if (src.HasMaskLeak())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_MaskLeak));
  if (src.HasSodaLimeFailure())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_SodaLimeFailure));
  if (src.HasTubeCuffLeak())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_TubeCuffLeak));
  if (src.HasVaporizerFailure())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_VaporizerFailure));
  if (src.HasVentilatorPressureLoss())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_VentilatorPressureLoss));
  if (src.HasYPieceDisconnect())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_YPieceDisconnect));
}


bool SEAnesthesiaMachineActionCollection::ProcessAction(const SEAnesthesiaMachineAction& action, cdm::AnyAnesthesiaMachineActionData* any)
{
  const SEAnesthesiaMachineConfiguration* config = dynamic_cast<const SEAnesthesiaMachineConfiguration*>(&action);
  if (config != nullptr)
  {
    if (m_Configuration == nullptr)
      m_Configuration = new SEAnesthesiaMachineConfiguration(m_Substances);
    auto* copy = SEAnesthesiaMachineConfiguration::Unload(*config);
    SEAnesthesiaMachineConfiguration::Load(*copy, *m_Configuration);
    (any != nullptr) ? any->set_allocated_configuration(copy) : delete copy;
    if (!m_Configuration->IsActive())
      RemoveConfiguration();
    return true;
  }

  const SEOxygenTankPressureLoss* O2Tank = dynamic_cast<const SEOxygenTankPressureLoss*>(&action);
  if (O2Tank != nullptr)
  {
    if (m_OxygenTankPressureLoss == nullptr)
      m_OxygenTankPressureLoss = new SEOxygenTankPressureLoss();
    auto* copy = SEOxygenTankPressureLoss::Unload(*O2Tank);
    SEOxygenTankPressureLoss::Load(*copy, *m_OxygenTankPressureLoss);
    (any != nullptr) ? any->set_allocated_oxygentankpressureloss(copy) : delete copy;
    if (!m_OxygenTankPressureLoss->IsActive())
      RemoveOxygenTankPressureLoss();
    return true;
  }

  const SEOxygenWallPortPressureLoss* O2Wall = dynamic_cast<const SEOxygenWallPortPressureLoss*>(&action);
  if (O2Wall != nullptr)
  {
    if (m_OxygenWallPortPressureLoss == nullptr)
      m_OxygenWallPortPressureLoss = new SEOxygenWallPortPressureLoss();
    auto* copy = SEOxygenWallPortPressureLoss::Unload(*O2Wall);
    SEOxygenWallPortPressureLoss::Load(*copy, *m_OxygenWallPortPressureLoss);
    (any != nullptr) ? any->set_allocated_oxygenwallportpressureloss(copy) : delete copy;
    if (!m_OxygenWallPortPressureLoss->IsActive())
      RemoveOxygenWallPortPressureLoss();
    return true;
  }

  const SEExpiratoryValveLeak* eLeak = dynamic_cast<const SEExpiratoryValveLeak*>(&action);
  if (eLeak != nullptr)
  {
    if (m_ExpiratoryValveLeak == nullptr)
      m_ExpiratoryValveLeak = new SEExpiratoryValveLeak();
    auto* copy = SEExpiratoryValveLeak::Unload(*eLeak);
    SEExpiratoryValveLeak::Load(*copy, *m_ExpiratoryValveLeak);
    (any != nullptr) ? any->set_allocated_expiratoryvalveleak(copy) : delete copy;
    if (!m_ExpiratoryValveLeak->IsActive())
      RemoveExpiratoryValveLeak();
    return true;
  }

  const SEExpiratoryValveObstruction* eOb = dynamic_cast<const SEExpiratoryValveObstruction*>(&action);
  if (eOb != nullptr)
  {
    if (m_ExpiratoryValveObstruction == nullptr)
      m_ExpiratoryValveObstruction = new SEExpiratoryValveObstruction();
    auto* copy = SEExpiratoryValveObstruction::Unload(*eOb);
    SEExpiratoryValveObstruction::Load(*copy, *m_ExpiratoryValveObstruction);
    (any != nullptr) ? any->set_allocated_expiratoryvalveobstruction(copy) : delete copy;
    if (!m_ExpiratoryValveObstruction->IsActive())
      RemoveExpiratoryValveObstruction();
    return true;
  }

  const SEInspiratoryValveLeak* iLeak = dynamic_cast<const SEInspiratoryValveLeak*>(&action);
  if (iLeak != nullptr)
  {
    if (m_InspiratoryValveLeak == nullptr)
      m_InspiratoryValveLeak = new SEInspiratoryValveLeak();
    auto* copy = SEInspiratoryValveLeak::Unload(*iLeak);
    SEInspiratoryValveLeak::Load(*copy, *m_InspiratoryValveLeak);
    (any != nullptr) ? any->set_allocated_inspiratoryvalveleak(copy) : delete copy;
    if (!m_InspiratoryValveLeak->IsActive())
      RemoveInspiratoryValveLeak();
    return true;
  }

  const SEInspiratoryValveObstruction* iOb = dynamic_cast<const SEInspiratoryValveObstruction*>(&action);
  if (iOb != nullptr)
  {
    if (m_InspiratoryValveObstruction == nullptr)
      m_InspiratoryValveObstruction = new SEInspiratoryValveObstruction();
    auto* copy = SEInspiratoryValveObstruction::Unload(*iOb);
    SEInspiratoryValveObstruction::Load(*copy, *m_InspiratoryValveObstruction);
    (any != nullptr) ? any->set_allocated_inspiratoryvalveobstruction(copy) : delete copy;
    if (!m_InspiratoryValveObstruction->IsActive())
      RemoveInspiratoryValveObstruction();
    return true;
  }

  const SEMaskLeak* mask = dynamic_cast<const SEMaskLeak*>(&action);
  if (mask != nullptr)
  {
    if (m_MaskLeak == nullptr)
      m_MaskLeak = new SEMaskLeak();
    auto* copy = SEMaskLeak::Unload(*mask);
    SEMaskLeak::Load(*copy, *m_MaskLeak);
    (any != nullptr) ? any->set_allocated_maskleak(copy) : delete copy;
    if (!m_MaskLeak->IsActive())
      RemoveMaskLeak();
    return true;
  }

  const SESodaLimeFailure* soda = dynamic_cast<const SESodaLimeFailure*>(&action);
  if (soda != nullptr)
  {
    if (m_SodaLimeFailure == nullptr)
      m_SodaLimeFailure = new SESodaLimeFailure();
    auto* copy = SESodaLimeFailure::Unload(*soda);
    SESodaLimeFailure::Load(*copy, *m_SodaLimeFailure);
    (any != nullptr) ? any->set_allocated_sodalimefailure(copy) : delete copy;
    if (!m_SodaLimeFailure->IsActive())
      RemoveSodaLimeFailure();
    return true;
  }

  const SETubeCuffLeak* tube = dynamic_cast<const SETubeCuffLeak*>(&action);
  if (tube != nullptr)
  {
    if (m_TubeCuffLeak == nullptr)
      m_TubeCuffLeak = new SETubeCuffLeak();
    auto* copy = SETubeCuffLeak::Unload(*tube);
    SETubeCuffLeak::Load(*copy, *m_TubeCuffLeak);
    (any != nullptr) ? any->set_allocated_tubecuffleak(copy) : delete copy;
    if (!m_TubeCuffLeak->IsActive())
      RemoveTubeCuffLeak();
    return true;
  }

  const SEVaporizerFailure* vFail = dynamic_cast<const SEVaporizerFailure*>(&action);
  if (vFail != nullptr)
  {
    if (m_VaporizerFailure == nullptr)
      m_VaporizerFailure = new SEVaporizerFailure();
    auto* copy = SEVaporizerFailure::Unload(*vFail);
    SEVaporizerFailure::Load(*copy, *m_VaporizerFailure);
    (any != nullptr) ? any->set_allocated_vaporizerfailure(copy) : delete copy;
    if (!m_VaporizerFailure->IsActive())
      RemoveVaporizerFailure();
    return true;
  }

  const SEVentilatorPressureLoss* vLoss = dynamic_cast<const SEVentilatorPressureLoss*>(&action);
  if (vLoss != nullptr)
  {
    if (m_VentilatorPressureLoss == nullptr)
      m_VentilatorPressureLoss = new SEVentilatorPressureLoss();
    auto* copy = SEVentilatorPressureLoss::Unload(*vLoss);
    SEVentilatorPressureLoss::Load(*copy, *m_VentilatorPressureLoss);
    (any != nullptr) ? any->set_allocated_ventilatorpressureloss(copy) : delete copy;
    if (!m_VentilatorPressureLoss->IsActive())
      RemoveVentilatorPressureLoss();
    return true;
  }

  const SEYPieceDisconnect* Y = dynamic_cast<const SEYPieceDisconnect*>(&action);
  if (Y != nullptr)
  {
    if (m_YPieceDisconnect == nullptr)
      m_YPieceDisconnect = new SEYPieceDisconnect();
    auto* copy = SEYPieceDisconnect::Unload(*Y);
    SEYPieceDisconnect::Load(*copy, *m_YPieceDisconnect);
    (any != nullptr) ? any->set_allocated_ypiecedisconnect(copy) : delete copy;
    if (!m_YPieceDisconnect->IsActive())
      RemoveYPieceDisconnect();
    return true;
  }
  /// \error Unsupported Action
  Error("Unsupported Action");
  return false;
}

bool SEAnesthesiaMachineActionCollection::HasConfiguration() const
{
  return m_Configuration == nullptr ? false : true;
}
SEAnesthesiaMachineConfiguration* SEAnesthesiaMachineActionCollection::GetConfiguration()
{
  return m_Configuration;
}
const SEAnesthesiaMachineConfiguration* SEAnesthesiaMachineActionCollection::GetConfiguration() const
{
  return m_Configuration;
}
void SEAnesthesiaMachineActionCollection::RemoveConfiguration()
{
  SAFE_DELETE(m_Configuration);
}

bool SEAnesthesiaMachineActionCollection::HasOxygenTankPressureLoss() const
{
  return m_OxygenTankPressureLoss == nullptr ? false : true;
}
SEOxygenTankPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenTankPressureLoss()
{
  return m_OxygenTankPressureLoss;
}
const SEOxygenTankPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenTankPressureLoss() const
{
  return m_OxygenTankPressureLoss;
}
void SEAnesthesiaMachineActionCollection::RemoveOxygenTankPressureLoss()
{
  SAFE_DELETE(m_OxygenTankPressureLoss);
}

bool SEAnesthesiaMachineActionCollection::HasOxygenWallPortPressureLoss() const
{
  return m_OxygenWallPortPressureLoss == nullptr ? false : true;
}
SEOxygenWallPortPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenWallPortPressureLoss()
{
  return m_OxygenWallPortPressureLoss;
}
const SEOxygenWallPortPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenWallPortPressureLoss() const
{
  return m_OxygenWallPortPressureLoss;
}
void SEAnesthesiaMachineActionCollection::RemoveOxygenWallPortPressureLoss()
{
  SAFE_DELETE(m_OxygenWallPortPressureLoss);
}

bool SEAnesthesiaMachineActionCollection::HasExpiratoryValveLeak() const
{
  return m_ExpiratoryValveLeak == nullptr ? false : true;
}
SEExpiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetExpiratoryValveLeak()
{
  return m_ExpiratoryValveLeak;
}
const SEExpiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetExpiratoryValveLeak() const
{
  return m_ExpiratoryValveLeak;
}
void SEAnesthesiaMachineActionCollection::RemoveExpiratoryValveLeak()
{
  SAFE_DELETE(m_ExpiratoryValveLeak);
}

bool SEAnesthesiaMachineActionCollection::HasExpiratoryValveObstruction() const
{
  return m_ExpiratoryValveObstruction == nullptr ? false : true;
}
SEExpiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetExpiratoryValveObstruction()
{
  return m_ExpiratoryValveObstruction;
}
const SEExpiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetExpiratoryValveObstruction() const
{
  return m_ExpiratoryValveObstruction;
}
void SEAnesthesiaMachineActionCollection::RemoveExpiratoryValveObstruction()
{
  SAFE_DELETE(m_ExpiratoryValveObstruction);
}

bool SEAnesthesiaMachineActionCollection::HasInspiratoryValveLeak() const
{
  return m_InspiratoryValveLeak == nullptr ? false : true;
}
SEInspiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetInspiratoryValveLeak()
{
  return m_InspiratoryValveLeak;
}
const SEInspiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetInspiratoryValveLeak() const
{
  return m_InspiratoryValveLeak;
}
void SEAnesthesiaMachineActionCollection::RemoveInspiratoryValveLeak()
{
  SAFE_DELETE(m_InspiratoryValveLeak);
}

bool SEAnesthesiaMachineActionCollection::HasInspiratoryValveObstruction() const
{
  return m_InspiratoryValveObstruction == nullptr ? false : true;
}
SEInspiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetInspiratoryValveObstruction()
{
  return m_InspiratoryValveObstruction;
}
const SEInspiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetInspiratoryValveObstruction() const
{
  return m_InspiratoryValveObstruction;
}
void SEAnesthesiaMachineActionCollection::RemoveInspiratoryValveObstruction()
{
  SAFE_DELETE(m_InspiratoryValveObstruction);
}

bool SEAnesthesiaMachineActionCollection::HasMaskLeak() const
{
  return m_MaskLeak == nullptr ? false : true;
}
SEMaskLeak* SEAnesthesiaMachineActionCollection::GetMaskLeak()
{
  return m_MaskLeak;
}
const SEMaskLeak* SEAnesthesiaMachineActionCollection::GetMaskLeak() const
{
  return m_MaskLeak;
}
void SEAnesthesiaMachineActionCollection::RemoveMaskLeak()
{
  SAFE_DELETE(m_MaskLeak);
}

bool SEAnesthesiaMachineActionCollection::HasSodaLimeFailure() const
{
  return m_SodaLimeFailure == nullptr ? false : true;
}
SESodaLimeFailure* SEAnesthesiaMachineActionCollection::GetSodaLimeFailure()
{
  return m_SodaLimeFailure;
}
const SESodaLimeFailure* SEAnesthesiaMachineActionCollection::GetSodaLimeFailure() const
{
  return m_SodaLimeFailure;
}
void SEAnesthesiaMachineActionCollection::RemoveSodaLimeFailure()
{
  SAFE_DELETE(m_SodaLimeFailure);
}

bool SEAnesthesiaMachineActionCollection::HasTubeCuffLeak() const
{
  return m_TubeCuffLeak == nullptr ? false : true;
}
SETubeCuffLeak* SEAnesthesiaMachineActionCollection::GetTubeCuffLeak()
{
  return m_TubeCuffLeak;
}
const SETubeCuffLeak* SEAnesthesiaMachineActionCollection::GetTubeCuffLeak() const
{
  return m_TubeCuffLeak;
}
void SEAnesthesiaMachineActionCollection::RemoveTubeCuffLeak()
{
  SAFE_DELETE(m_TubeCuffLeak);
}

bool SEAnesthesiaMachineActionCollection::HasVaporizerFailure() const
{
  return m_VaporizerFailure == nullptr ? false : true;
}
SEVaporizerFailure* SEAnesthesiaMachineActionCollection::GetVaporizerFailure()
{
  return m_VaporizerFailure;
}
const SEVaporizerFailure* SEAnesthesiaMachineActionCollection::GetVaporizerFailure() const
{
  return m_VaporizerFailure;
}
void SEAnesthesiaMachineActionCollection::RemoveVaporizerFailure()
{
  SAFE_DELETE(m_VaporizerFailure);
}

bool SEAnesthesiaMachineActionCollection::HasVentilatorPressureLoss() const
{
  return m_VentilatorPressureLoss == nullptr ? false : true;
}
SEVentilatorPressureLoss* SEAnesthesiaMachineActionCollection::GetVentilatorPressureLoss()
{
  return m_VentilatorPressureLoss;
}
const SEVentilatorPressureLoss* SEAnesthesiaMachineActionCollection::GetVentilatorPressureLoss() const
{
  return m_VentilatorPressureLoss;
}
void SEAnesthesiaMachineActionCollection::RemoveVentilatorPressureLoss()
{
  SAFE_DELETE(m_VentilatorPressureLoss);
}

bool SEAnesthesiaMachineActionCollection::HasYPieceDisconnect() const
{
  return m_YPieceDisconnect == nullptr ? false : true;
}
SEYPieceDisconnect* SEAnesthesiaMachineActionCollection::GetYPieceDisconnect()
{
  return m_YPieceDisconnect;
}
const SEYPieceDisconnect* SEAnesthesiaMachineActionCollection::GetYPieceDisconnect() const
{
  return m_YPieceDisconnect;
}
void SEAnesthesiaMachineActionCollection::RemoveYPieceDisconnect()
{
  SAFE_DELETE(m_YPieceDisconnect);
}