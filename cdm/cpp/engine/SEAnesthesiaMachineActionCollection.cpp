/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEAnesthesiaMachineActionCollection.h"
#include "substance/SESubstanceManager.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineExpiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineExpiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineInspiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineInspiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineMaskLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineSodaLimeFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineTubeCuffLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineVaporizerFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineVentilatorPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineYPieceDisconnect.h"
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

  const SEAnesthesiaMachineOxygenTankPressureLoss* O2Tank = dynamic_cast<const SEAnesthesiaMachineOxygenTankPressureLoss*>(&action);
  if (O2Tank != nullptr)
  {
    if (m_OxygenTankPressureLoss == nullptr)
      m_OxygenTankPressureLoss = new SEAnesthesiaMachineOxygenTankPressureLoss();
    m_OxygenTankPressureLoss->Copy(*O2Tank);
    if (!m_OxygenTankPressureLoss->IsActive())
      RemoveOxygenTankPressureLoss();
    return true;
  }

  const SEAnesthesiaMachineOxygenWallPortPressureLoss* O2Wall = dynamic_cast<const SEAnesthesiaMachineOxygenWallPortPressureLoss*>(&action);
  if (O2Wall != nullptr)
  {
    if (m_OxygenWallPortPressureLoss == nullptr)
      m_OxygenWallPortPressureLoss = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
    m_OxygenWallPortPressureLoss->Copy(*O2Wall);
    if (!m_OxygenWallPortPressureLoss->IsActive())
      RemoveOxygenWallPortPressureLoss();
    return true;
  }

  const SEAnesthesiaMachineExpiratoryValveLeak* eLeak = dynamic_cast<const SEAnesthesiaMachineExpiratoryValveLeak*>(&action);
  if (eLeak != nullptr)
  {
    if (m_ExpiratoryValveLeak == nullptr)
      m_ExpiratoryValveLeak = new SEAnesthesiaMachineExpiratoryValveLeak();
    m_ExpiratoryValveLeak->Copy(*eLeak);
    if (!m_ExpiratoryValveLeak->IsActive())
      RemoveExpiratoryValveLeak();
    return true;
  }

  const SEAnesthesiaMachineExpiratoryValveObstruction* eOb = dynamic_cast<const SEAnesthesiaMachineExpiratoryValveObstruction*>(&action);
  if (eOb != nullptr)
  {
    if (m_ExpiratoryValveObstruction == nullptr)
      m_ExpiratoryValveObstruction = new SEAnesthesiaMachineExpiratoryValveObstruction();
    m_ExpiratoryValveObstruction->Copy(*eOb);
    if (!m_ExpiratoryValveObstruction->IsActive())
      RemoveExpiratoryValveObstruction();
    return true;
  }

  const SEAnesthesiaMachineInspiratoryValveLeak* iLeak = dynamic_cast<const SEAnesthesiaMachineInspiratoryValveLeak*>(&action);
  if (iLeak != nullptr)
  {
    if (m_InspiratoryValveLeak == nullptr)
      m_InspiratoryValveLeak = new SEAnesthesiaMachineInspiratoryValveLeak();
    m_InspiratoryValveLeak->Copy(*iLeak);
    if (!m_InspiratoryValveLeak->IsActive())
      RemoveInspiratoryValveLeak();
    return true;
  }

  const SEAnesthesiaMachineInspiratoryValveObstruction* iOb = dynamic_cast<const SEAnesthesiaMachineInspiratoryValveObstruction*>(&action);
  if (iOb != nullptr)
  {
    if (m_InspiratoryValveObstruction == nullptr)
      m_InspiratoryValveObstruction = new SEAnesthesiaMachineInspiratoryValveObstruction();
    m_InspiratoryValveObstruction->Copy(*iOb);
    if (!m_InspiratoryValveObstruction->IsActive())
      RemoveInspiratoryValveObstruction();
    return true;
  }

  const SEAnesthesiaMachineMaskLeak* mask = dynamic_cast<const SEAnesthesiaMachineMaskLeak*>(&action);
  if (mask != nullptr)
  {
    if (m_MaskLeak == nullptr)
      m_MaskLeak = new SEAnesthesiaMachineMaskLeak();
    m_MaskLeak->Copy(*mask);
    if (!m_MaskLeak->IsActive())
      RemoveMaskLeak();
    return true;
  }

  const SEAnesthesiaMachineSodaLimeFailure* soda = dynamic_cast<const SEAnesthesiaMachineSodaLimeFailure*>(&action);
  if (soda != nullptr)
  {
    if (m_SodaLimeFailure == nullptr)
      m_SodaLimeFailure = new SEAnesthesiaMachineSodaLimeFailure();
    m_SodaLimeFailure->Copy(*soda);
    if (!m_SodaLimeFailure->IsActive())
      RemoveSodaLimeFailure();
    return true;
  }

  const SEAnesthesiaMachineTubeCuffLeak* tube = dynamic_cast<const SEAnesthesiaMachineTubeCuffLeak*>(&action);
  if (tube != nullptr)
  {
    if (m_TubeCuffLeak == nullptr)
      m_TubeCuffLeak = new SEAnesthesiaMachineTubeCuffLeak();
    m_TubeCuffLeak->Copy(*tube);
    if (!m_TubeCuffLeak->IsActive())
      RemoveTubeCuffLeak();
    return true;
  }

  const SEAnesthesiaMachineVaporizerFailure* vFail = dynamic_cast<const SEAnesthesiaMachineVaporizerFailure*>(&action);
  if (vFail != nullptr)
  {
    if (m_VaporizerFailure == nullptr)
      m_VaporizerFailure = new SEAnesthesiaMachineVaporizerFailure();
    m_VaporizerFailure->Copy(*vFail);
    if (!m_VaporizerFailure->IsActive())
      RemoveVaporizerFailure();
    return true;
  }

  const SEAnesthesiaMachineVentilatorPressureLoss* vLoss = dynamic_cast<const SEAnesthesiaMachineVentilatorPressureLoss*>(&action);
  if (vLoss != nullptr)
  {
    if (m_VentilatorPressureLoss == nullptr)
      m_VentilatorPressureLoss = new SEAnesthesiaMachineVentilatorPressureLoss();
    m_VentilatorPressureLoss->Copy(*vLoss);
    if (!m_VentilatorPressureLoss->IsActive())
      RemoveVentilatorPressureLoss();
    return true;
  }

  const SEAnesthesiaMachineYPieceDisconnect* Y = dynamic_cast<const SEAnesthesiaMachineYPieceDisconnect*>(&action);
  if (Y != nullptr)
  {
    if (m_YPieceDisconnect == nullptr)
      m_YPieceDisconnect = new SEAnesthesiaMachineYPieceDisconnect();
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
SEAnesthesiaMachineOxygenTankPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenTankPressureLoss()
{
  return m_OxygenTankPressureLoss;
}
const SEAnesthesiaMachineOxygenTankPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenTankPressureLoss() const
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
SEAnesthesiaMachineOxygenWallPortPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenWallPortPressureLoss()
{
  return m_OxygenWallPortPressureLoss;
}
const SEAnesthesiaMachineOxygenWallPortPressureLoss* SEAnesthesiaMachineActionCollection::GetOxygenWallPortPressureLoss() const
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
SEAnesthesiaMachineExpiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetExpiratoryValveLeak()
{
  return m_ExpiratoryValveLeak;
}
const SEAnesthesiaMachineExpiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetExpiratoryValveLeak() const
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
SEAnesthesiaMachineExpiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetExpiratoryValveObstruction()
{
  return m_ExpiratoryValveObstruction;
}
const SEAnesthesiaMachineExpiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetExpiratoryValveObstruction() const
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
SEAnesthesiaMachineInspiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetInspiratoryValveLeak()
{
  return m_InspiratoryValveLeak;
}
const SEAnesthesiaMachineInspiratoryValveLeak* SEAnesthesiaMachineActionCollection::GetInspiratoryValveLeak() const
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
SEAnesthesiaMachineInspiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetInspiratoryValveObstruction()
{
  return m_InspiratoryValveObstruction;
}
const SEAnesthesiaMachineInspiratoryValveObstruction* SEAnesthesiaMachineActionCollection::GetInspiratoryValveObstruction() const
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
SEAnesthesiaMachineMaskLeak* SEAnesthesiaMachineActionCollection::GetMaskLeak()
{
  return m_MaskLeak;
}
const SEAnesthesiaMachineMaskLeak* SEAnesthesiaMachineActionCollection::GetMaskLeak() const
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
SEAnesthesiaMachineSodaLimeFailure* SEAnesthesiaMachineActionCollection::GetSodaLimeFailure()
{
  return m_SodaLimeFailure;
}
const SEAnesthesiaMachineSodaLimeFailure* SEAnesthesiaMachineActionCollection::GetSodaLimeFailure() const
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
SEAnesthesiaMachineTubeCuffLeak* SEAnesthesiaMachineActionCollection::GetTubeCuffLeak()
{
  return m_TubeCuffLeak;
}
const SEAnesthesiaMachineTubeCuffLeak* SEAnesthesiaMachineActionCollection::GetTubeCuffLeak() const
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
SEAnesthesiaMachineVaporizerFailure* SEAnesthesiaMachineActionCollection::GetVaporizerFailure()
{
  return m_VaporizerFailure;
}
const SEAnesthesiaMachineVaporizerFailure* SEAnesthesiaMachineActionCollection::GetVaporizerFailure() const
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
SEAnesthesiaMachineVentilatorPressureLoss* SEAnesthesiaMachineActionCollection::GetVentilatorPressureLoss()
{
  return m_VentilatorPressureLoss;
}
const SEAnesthesiaMachineVentilatorPressureLoss* SEAnesthesiaMachineActionCollection::GetVentilatorPressureLoss() const
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
SEAnesthesiaMachineYPieceDisconnect* SEAnesthesiaMachineActionCollection::GetYPieceDisconnect()
{
  return m_YPieceDisconnect;
}
const SEAnesthesiaMachineYPieceDisconnect* SEAnesthesiaMachineActionCollection::GetYPieceDisconnect() const
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