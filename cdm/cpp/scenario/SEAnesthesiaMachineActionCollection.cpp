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
#include "properties/SEScalarVolume.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolumePerTime.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

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

bool SEAnesthesiaMachineActionCollection::ProcessAction(const SEAnesthesiaMachineAction& action)
{
  const SEAnesthesiaMachineConfiguration* config = dynamic_cast<const SEAnesthesiaMachineConfiguration*>(&action);
  if (config != nullptr)
  {
    if (m_Configuration == nullptr)
      m_Configuration = new SEAnesthesiaMachineConfiguration(m_Substances);
    m_Configuration->Copy(*config);
    if (!m_Configuration->IsActive())
      RemoveConfiguration();
    return true;
  }

  const SEOxygenTankPressureLoss* O2Tank = dynamic_cast<const SEOxygenTankPressureLoss*>(&action);
  if (O2Tank != nullptr)
  {
    if (m_OxygenTankPressureLoss == nullptr)
      m_OxygenTankPressureLoss = new SEOxygenTankPressureLoss();
    m_OxygenTankPressureLoss->Copy(*O2Tank);
    if (!m_OxygenTankPressureLoss->IsActive())
      RemoveOxygenTankPressureLoss();
    return true;
  }

  const SEOxygenWallPortPressureLoss* O2Wall = dynamic_cast<const SEOxygenWallPortPressureLoss*>(&action);
  if (O2Wall != nullptr)
  {
    if (m_OxygenWallPortPressureLoss == nullptr)
      m_OxygenWallPortPressureLoss = new SEOxygenWallPortPressureLoss();
    m_OxygenWallPortPressureLoss->Copy(*O2Wall);
    if (!m_OxygenWallPortPressureLoss->IsActive())
      RemoveOxygenWallPortPressureLoss();
    return true;
  }

  const SEExpiratoryValveLeak* eLeak = dynamic_cast<const SEExpiratoryValveLeak*>(&action);
  if (eLeak != nullptr)
  {
    if (m_ExpiratoryValveLeak == nullptr)
      m_ExpiratoryValveLeak = new SEExpiratoryValveLeak();
    m_ExpiratoryValveLeak->Copy(*eLeak);
    if (!m_ExpiratoryValveLeak->IsActive())
      RemoveExpiratoryValveLeak();
    return true;
  }

  const SEExpiratoryValveObstruction* eOb = dynamic_cast<const SEExpiratoryValveObstruction*>(&action);
  if (eOb != nullptr)
  {
    if (m_ExpiratoryValveObstruction == nullptr)
      m_ExpiratoryValveObstruction = new SEExpiratoryValveObstruction();
    m_ExpiratoryValveObstruction->Copy(*eOb);
    if (!m_ExpiratoryValveObstruction->IsActive())
      RemoveExpiratoryValveObstruction();
    return true;
  }

  const SEInspiratoryValveLeak* iLeak = dynamic_cast<const SEInspiratoryValveLeak*>(&action);
  if (iLeak != nullptr)
  {
    if (m_InspiratoryValveLeak == nullptr)
      m_InspiratoryValveLeak = new SEInspiratoryValveLeak();
    m_InspiratoryValveLeak->Copy(*iLeak);
    if (!m_InspiratoryValveLeak->IsActive())
      RemoveInspiratoryValveLeak();
    return true;
  }

  const SEInspiratoryValveObstruction* iOb = dynamic_cast<const SEInspiratoryValveObstruction*>(&action);
  if (iOb != nullptr)
  {
    if (m_InspiratoryValveObstruction == nullptr)
      m_InspiratoryValveObstruction = new SEInspiratoryValveObstruction();
    m_InspiratoryValveObstruction->Copy(*iOb);
    if (!m_InspiratoryValveObstruction->IsActive())
      RemoveInspiratoryValveObstruction();
    return true;
  }

  const SEMaskLeak* mask = dynamic_cast<const SEMaskLeak*>(&action);
  if (mask != nullptr)
  {
    if (m_MaskLeak == nullptr)
      m_MaskLeak = new SEMaskLeak();
    m_MaskLeak->Copy(*mask);
    if (!m_MaskLeak->IsActive())
      RemoveMaskLeak();
    return true;
  }

  const SESodaLimeFailure* soda = dynamic_cast<const SESodaLimeFailure*>(&action);
  if (soda != nullptr)
  {
    if (m_SodaLimeFailure == nullptr)
      m_SodaLimeFailure = new SESodaLimeFailure();
    m_SodaLimeFailure->Copy(*soda);
    if (!m_SodaLimeFailure->IsActive())
      RemoveSodaLimeFailure();
    return true;
  }

  const SETubeCuffLeak* tube = dynamic_cast<const SETubeCuffLeak*>(&action);
  if (tube != nullptr)
  {
    if (m_TubeCuffLeak == nullptr)
      m_TubeCuffLeak = new SETubeCuffLeak();
    m_TubeCuffLeak->Copy(*tube);
    if (!m_TubeCuffLeak->IsActive())
      RemoveTubeCuffLeak();
    return true;
  }

  const SEVaporizerFailure* vFail = dynamic_cast<const SEVaporizerFailure*>(&action);
  if (vFail != nullptr)
  {
    if (m_VaporizerFailure == nullptr)
      m_VaporizerFailure = new SEVaporizerFailure();
    m_VaporizerFailure->Copy(*vFail);
    if (!m_VaporizerFailure->IsActive())
      RemoveVaporizerFailure();
    return true;
  }

  const SEVentilatorPressureLoss* vLoss = dynamic_cast<const SEVentilatorPressureLoss*>(&action);
  if (vLoss != nullptr)
  {
    if (m_VentilatorPressureLoss == nullptr)
      m_VentilatorPressureLoss = new SEVentilatorPressureLoss();
    m_VentilatorPressureLoss->Copy(*vLoss);
    if (!m_VentilatorPressureLoss->IsActive())
      RemoveVentilatorPressureLoss();
    return true;
  }

  const SEYPieceDisconnect* Y = dynamic_cast<const SEYPieceDisconnect*>(&action);
  if (Y != nullptr)
  {
    if (m_YPieceDisconnect == nullptr)
      m_YPieceDisconnect = new SEYPieceDisconnect();
    m_YPieceDisconnect->Copy(*Y);
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

void SEAnesthesiaMachineActionCollection::GetAllActions(std::vector<const SEAction*>& actions) const
{
  if (HasConfiguration())
    actions.push_back(GetConfiguration());
  if (HasOxygenTankPressureLoss())
    actions.push_back(GetOxygenTankPressureLoss());
  if (HasOxygenWallPortPressureLoss())
    actions.push_back(GetOxygenWallPortPressureLoss());
  if (HasExpiratoryValveLeak())
    actions.push_back(GetExpiratoryValveLeak());
  if (HasExpiratoryValveObstruction())
    actions.push_back(GetExpiratoryValveObstruction());
  if (HasInspiratoryValveLeak())
    actions.push_back(GetInspiratoryValveLeak());
  if (HasInspiratoryValveObstruction())
    actions.push_back(GetInspiratoryValveObstruction());
  if (HasMaskLeak())
    actions.push_back(GetMaskLeak());
  if (HasSodaLimeFailure())
    actions.push_back(GetSodaLimeFailure());
  if (HasTubeCuffLeak())
    actions.push_back(GetTubeCuffLeak());
  if (HasVaporizerFailure())
    actions.push_back(GetVaporizerFailure());
  if (HasVentilatorPressureLoss())
    actions.push_back(GetVentilatorPressureLoss());
  if (HasYPieceDisconnect())
    actions.push_back(GetYPieceDisconnect());
}