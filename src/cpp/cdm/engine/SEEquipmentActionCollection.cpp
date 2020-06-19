/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEEquipmentActionCollection.h"
#include "substance/SESubstanceManager.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachineChamber.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveObstruction.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveObstruction.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineMaskLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineSodaLimeFailure.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineTubeCuffLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVaporizerFailure.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVentilatorPressureLoss.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineYPieceDisconnect.h"
#include "system/equipment/inhaler/SEInhaler.h"
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolumePerTime.h"

SEEquipmentActionCollection::SEEquipmentActionCollection(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
{
  m_AnesthesiaMachineConfiguration = nullptr;
  m_InhalerConfiguration = nullptr;
  m_MechanicalVentilatorConfiguration = nullptr;

  m_AnesthesiaMachineOxygenTankPressureLoss = nullptr;
  m_AnesthesiaMachineOxygenWallPortPressureLoss = nullptr;

  m_AnesthesiaMachineExpiratoryValveLeak = nullptr;
  m_AnesthesiaMachineExpiratoryValveObstruction = nullptr;
  m_AnesthesiaMachineInspiratoryValveLeak = nullptr;
  m_AnesthesiaMachineInspiratoryValveObstruction = nullptr;
  m_AnesthesiaMachineMaskLeak = nullptr;
  m_AnesthesiaMachineSodaLimeFailure = nullptr;
  m_AnesthesiaMachineTubeCuffLeak = nullptr;
  m_AnesthesiaMachineVaporizerFailure = nullptr;
  m_AnesthesiaMachineVentilatorPressureLoss = nullptr;
  m_AnesthesiaMachineYPieceDisconnect = nullptr;
}

SEEquipmentActionCollection::~SEEquipmentActionCollection()
{
  Clear();
}

void SEEquipmentActionCollection::Clear()
{
  // State
  RemoveAnesthesiaMachineConfiguration();
  RemoveInhalerConfiguration();
  RemoveMechanicalVentilatorConfiguration();
  // Incidents
  RemoveAnesthesiaMachineOxygenTankPressureLoss();
  RemoveAnesthesiaMachineOxygenWallPortPressureLoss();
  // Failures
  RemoveAnesthesiaMachineExpiratoryValveLeak();
  RemoveAnesthesiaMachineExpiratoryValveObstruction();
  RemoveAnesthesiaMachineInspiratoryValveLeak();
  RemoveAnesthesiaMachineInspiratoryValveObstruction();
  RemoveAnesthesiaMachineMaskLeak();
  RemoveAnesthesiaMachineSodaLimeFailure();
  RemoveAnesthesiaMachineTubeCuffLeak();
  RemoveAnesthesiaMachineVaporizerFailure();
  RemoveAnesthesiaMachineVentilatorPressureLoss();
  RemoveAnesthesiaMachineYPieceDisconnect();
}

bool SEEquipmentActionCollection::ProcessAction(const SEEquipmentAction& action)
{
  const SEMechanicalVentilatorAction* mva = dynamic_cast<const SEMechanicalVentilatorAction*>(&action);
  if (mva != nullptr)
  {
    const SEMechanicalVentilatorConfiguration* config = dynamic_cast<const SEMechanicalVentilatorConfiguration*>(&action);
    if (config != nullptr)
    {
      if (m_MechanicalVentilatorConfiguration == nullptr)
        m_MechanicalVentilatorConfiguration = new SEMechanicalVentilatorConfiguration(GetLogger());
      m_MechanicalVentilatorConfiguration->Copy(*config, m_Substances);
      if (!m_MechanicalVentilatorConfiguration->IsActive())
        RemoveMechanicalVentilatorConfiguration();
      return true;
    }
  }

  const SEAnesthesiaMachineAction* ama = dynamic_cast<const SEAnesthesiaMachineAction*>(&action);
  if (ama != nullptr)
  {
    const SEAnesthesiaMachineConfiguration* config = dynamic_cast<const SEAnesthesiaMachineConfiguration*>(&action);
    if (config != nullptr)
    {
      if (m_AnesthesiaMachineConfiguration == nullptr)
        m_AnesthesiaMachineConfiguration = new SEAnesthesiaMachineConfiguration(GetLogger());
      m_AnesthesiaMachineConfiguration->Copy(*config, m_Substances);
      if (!m_AnesthesiaMachineConfiguration->IsActive())
        RemoveAnesthesiaMachineConfiguration();
      return true;
    }

    const SEAnesthesiaMachineOxygenTankPressureLoss* O2Tank = dynamic_cast<const SEAnesthesiaMachineOxygenTankPressureLoss*>(&action);
    if (O2Tank != nullptr)
    {
      if (m_AnesthesiaMachineOxygenTankPressureLoss == nullptr)
        m_AnesthesiaMachineOxygenTankPressureLoss = new SEAnesthesiaMachineOxygenTankPressureLoss();
      m_AnesthesiaMachineOxygenTankPressureLoss->Copy(*O2Tank);
      if (!m_AnesthesiaMachineOxygenTankPressureLoss->IsActive())
        RemoveAnesthesiaMachineOxygenTankPressureLoss();
      return true;
    }

    const SEAnesthesiaMachineOxygenWallPortPressureLoss* O2Wall = dynamic_cast<const SEAnesthesiaMachineOxygenWallPortPressureLoss*>(&action);
    if (O2Wall != nullptr)
    {
      if (m_AnesthesiaMachineOxygenWallPortPressureLoss == nullptr)
        m_AnesthesiaMachineOxygenWallPortPressureLoss = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
      m_AnesthesiaMachineOxygenWallPortPressureLoss->Copy(*O2Wall);
      if (!m_AnesthesiaMachineOxygenWallPortPressureLoss->IsActive())
        RemoveAnesthesiaMachineOxygenWallPortPressureLoss();
      return true;
    }

    const SEAnesthesiaMachineExpiratoryValveLeak* eLeak = dynamic_cast<const SEAnesthesiaMachineExpiratoryValveLeak*>(&action);
    if (eLeak != nullptr)
    {
      if (m_AnesthesiaMachineExpiratoryValveLeak == nullptr)
        m_AnesthesiaMachineExpiratoryValveLeak = new SEAnesthesiaMachineExpiratoryValveLeak();
      m_AnesthesiaMachineExpiratoryValveLeak->Copy(*eLeak);
      if (!m_AnesthesiaMachineExpiratoryValveLeak->IsActive())
        RemoveAnesthesiaMachineExpiratoryValveLeak();
      return true;
    }

    const SEAnesthesiaMachineExpiratoryValveObstruction* eOb = dynamic_cast<const SEAnesthesiaMachineExpiratoryValveObstruction*>(&action);
    if (eOb != nullptr)
    {
      if (m_AnesthesiaMachineExpiratoryValveObstruction == nullptr)
        m_AnesthesiaMachineExpiratoryValveObstruction = new SEAnesthesiaMachineExpiratoryValveObstruction();
      m_AnesthesiaMachineExpiratoryValveObstruction->Copy(*eOb);
      if (!m_AnesthesiaMachineExpiratoryValveObstruction->IsActive())
        RemoveAnesthesiaMachineExpiratoryValveObstruction();
      return true;
    }

    const SEAnesthesiaMachineInspiratoryValveLeak* iLeak = dynamic_cast<const SEAnesthesiaMachineInspiratoryValveLeak*>(&action);
    if (iLeak != nullptr)
    {
      if (m_AnesthesiaMachineInspiratoryValveLeak == nullptr)
        m_AnesthesiaMachineInspiratoryValveLeak = new SEAnesthesiaMachineInspiratoryValveLeak();
      m_AnesthesiaMachineInspiratoryValveLeak->Copy(*iLeak);
      if (!m_AnesthesiaMachineInspiratoryValveLeak->IsActive())
        RemoveAnesthesiaMachineInspiratoryValveLeak();
      return true;
    }

    const SEAnesthesiaMachineInspiratoryValveObstruction* iOb = dynamic_cast<const SEAnesthesiaMachineInspiratoryValveObstruction*>(&action);
    if (iOb != nullptr)
    {
      if (m_AnesthesiaMachineInspiratoryValveObstruction == nullptr)
        m_AnesthesiaMachineInspiratoryValveObstruction = new SEAnesthesiaMachineInspiratoryValveObstruction();
      m_AnesthesiaMachineInspiratoryValveObstruction->Copy(*iOb);
      if (!m_AnesthesiaMachineInspiratoryValveObstruction->IsActive())
        RemoveAnesthesiaMachineInspiratoryValveObstruction();
      return true;
    }

    const SEAnesthesiaMachineMaskLeak* mask = dynamic_cast<const SEAnesthesiaMachineMaskLeak*>(&action);
    if (mask != nullptr)
    {
      if (m_AnesthesiaMachineMaskLeak == nullptr)
        m_AnesthesiaMachineMaskLeak = new SEAnesthesiaMachineMaskLeak();
      m_AnesthesiaMachineMaskLeak->Copy(*mask);
      if (!m_AnesthesiaMachineMaskLeak->IsActive())
        RemoveAnesthesiaMachineMaskLeak();
      return true;
    }

    const SEAnesthesiaMachineSodaLimeFailure* soda = dynamic_cast<const SEAnesthesiaMachineSodaLimeFailure*>(&action);
    if (soda != nullptr)
    {
      if (m_AnesthesiaMachineSodaLimeFailure == nullptr)
        m_AnesthesiaMachineSodaLimeFailure = new SEAnesthesiaMachineSodaLimeFailure();
      m_AnesthesiaMachineSodaLimeFailure->Copy(*soda);
      if (!m_AnesthesiaMachineSodaLimeFailure->IsActive())
        RemoveAnesthesiaMachineSodaLimeFailure();
      return true;
    }

    const SEAnesthesiaMachineTubeCuffLeak* tube = dynamic_cast<const SEAnesthesiaMachineTubeCuffLeak*>(&action);
    if (tube != nullptr)
    {
      if (m_AnesthesiaMachineTubeCuffLeak == nullptr)
        m_AnesthesiaMachineTubeCuffLeak = new SEAnesthesiaMachineTubeCuffLeak();
      m_AnesthesiaMachineTubeCuffLeak->Copy(*tube);
      if (!m_AnesthesiaMachineTubeCuffLeak->IsActive())
        RemoveAnesthesiaMachineTubeCuffLeak();
      return true;
    }

    const SEAnesthesiaMachineVaporizerFailure* vFail = dynamic_cast<const SEAnesthesiaMachineVaporizerFailure*>(&action);
    if (vFail != nullptr)
    {
      if (m_AnesthesiaMachineVaporizerFailure == nullptr)
        m_AnesthesiaMachineVaporizerFailure = new SEAnesthesiaMachineVaporizerFailure();
      m_AnesthesiaMachineVaporizerFailure->Copy(*vFail);
      if (!m_AnesthesiaMachineVaporizerFailure->IsActive())
        RemoveAnesthesiaMachineVaporizerFailure();
      return true;
    }

    const SEAnesthesiaMachineVentilatorPressureLoss* vLoss = dynamic_cast<const SEAnesthesiaMachineVentilatorPressureLoss*>(&action);
    if (vLoss != nullptr)
    {
      if (m_AnesthesiaMachineVentilatorPressureLoss == nullptr)
        m_AnesthesiaMachineVentilatorPressureLoss = new SEAnesthesiaMachineVentilatorPressureLoss();
      m_AnesthesiaMachineVentilatorPressureLoss->Copy(*vLoss);
      if (!m_AnesthesiaMachineVentilatorPressureLoss->IsActive())
        RemoveAnesthesiaMachineVentilatorPressureLoss();
      return true;
    }

    const SEAnesthesiaMachineYPieceDisconnect* Y = dynamic_cast<const SEAnesthesiaMachineYPieceDisconnect*>(&action);
    if (Y != nullptr)
    {
      if (m_AnesthesiaMachineYPieceDisconnect == nullptr)
        m_AnesthesiaMachineYPieceDisconnect = new SEAnesthesiaMachineYPieceDisconnect();
      m_AnesthesiaMachineYPieceDisconnect->Copy(*Y);
      if (!m_AnesthesiaMachineYPieceDisconnect->IsActive())
        RemoveAnesthesiaMachineYPieceDisconnect();
      return true;
    }
  }

  const SEInhalerAction* ia = dynamic_cast<const SEInhalerAction*>(&action);
  if (ia != nullptr)
  {
    const SEInhalerConfiguration* config = dynamic_cast<const SEInhalerConfiguration*>(&action);
    if (config != nullptr)
    {
      if (m_InhalerConfiguration == nullptr)
        m_InhalerConfiguration = new SEInhalerConfiguration(GetLogger());
      m_InhalerConfiguration->Copy(*config, m_Substances);
      if (!m_InhalerConfiguration->IsActive())
        RemoveInhalerConfiguration();
      return true;
    }
  }

  /// \error Unsupported Action
  Error("Unsupported Action");
  return false;
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineConfiguration() const
{
  return m_AnesthesiaMachineConfiguration == nullptr ? false : true;
}
SEAnesthesiaMachineConfiguration* SEEquipmentActionCollection::GetAnesthesiaMachineConfiguration()
{
  return m_AnesthesiaMachineConfiguration;
}
const SEAnesthesiaMachineConfiguration* SEEquipmentActionCollection::GetAnesthesiaMachineConfiguration() const
{
  return m_AnesthesiaMachineConfiguration;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineConfiguration()
{
  SAFE_DELETE(m_AnesthesiaMachineConfiguration);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineOxygenTankPressureLoss() const
{
  return m_AnesthesiaMachineOxygenTankPressureLoss == nullptr ? false : true;
}
SEAnesthesiaMachineOxygenTankPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineOxygenTankPressureLoss()
{
  return m_AnesthesiaMachineOxygenTankPressureLoss;
}
const SEAnesthesiaMachineOxygenTankPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineOxygenTankPressureLoss() const
{
  return m_AnesthesiaMachineOxygenTankPressureLoss;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineOxygenTankPressureLoss()
{
  SAFE_DELETE(m_AnesthesiaMachineOxygenTankPressureLoss);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineOxygenWallPortPressureLoss() const
{
  return m_AnesthesiaMachineOxygenWallPortPressureLoss == nullptr ? false : true;
}
SEAnesthesiaMachineOxygenWallPortPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineOxygenWallPortPressureLoss()
{
  return m_AnesthesiaMachineOxygenWallPortPressureLoss;
}
const SEAnesthesiaMachineOxygenWallPortPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineOxygenWallPortPressureLoss() const
{
  return m_AnesthesiaMachineOxygenWallPortPressureLoss;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineOxygenWallPortPressureLoss()
{
  SAFE_DELETE(m_AnesthesiaMachineOxygenWallPortPressureLoss);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineExpiratoryValveLeak() const
{
  return m_AnesthesiaMachineExpiratoryValveLeak == nullptr ? false : true;
}
SEAnesthesiaMachineExpiratoryValveLeak* SEEquipmentActionCollection::GetAnesthesiaMachineExpiratoryValveLeak()
{
  return m_AnesthesiaMachineExpiratoryValveLeak;
}
const SEAnesthesiaMachineExpiratoryValveLeak* SEEquipmentActionCollection::GetAnesthesiaMachineExpiratoryValveLeak() const
{
  return m_AnesthesiaMachineExpiratoryValveLeak;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineExpiratoryValveLeak()
{
  SAFE_DELETE(m_AnesthesiaMachineExpiratoryValveLeak);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineExpiratoryValveObstruction() const
{
  return m_AnesthesiaMachineExpiratoryValveObstruction == nullptr ? false : true;
}
SEAnesthesiaMachineExpiratoryValveObstruction* SEEquipmentActionCollection::GetAnesthesiaMachineExpiratoryValveObstruction()
{
  return m_AnesthesiaMachineExpiratoryValveObstruction;
}
const SEAnesthesiaMachineExpiratoryValveObstruction* SEEquipmentActionCollection::GetAnesthesiaMachineExpiratoryValveObstruction() const
{
  return m_AnesthesiaMachineExpiratoryValveObstruction;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineExpiratoryValveObstruction()
{
  SAFE_DELETE(m_AnesthesiaMachineExpiratoryValveObstruction);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineInspiratoryValveLeak() const
{
  return m_AnesthesiaMachineInspiratoryValveLeak == nullptr ? false : true;
}
SEAnesthesiaMachineInspiratoryValveLeak* SEEquipmentActionCollection::GetAnesthesiaMachineInspiratoryValveLeak()
{
  return m_AnesthesiaMachineInspiratoryValveLeak;
}
const SEAnesthesiaMachineInspiratoryValveLeak* SEEquipmentActionCollection::GetAnesthesiaMachineInspiratoryValveLeak() const
{
  return m_AnesthesiaMachineInspiratoryValveLeak;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineInspiratoryValveLeak()
{
  SAFE_DELETE(m_AnesthesiaMachineInspiratoryValveLeak);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineInspiratoryValveObstruction() const
{
  return m_AnesthesiaMachineInspiratoryValveObstruction == nullptr ? false : true;
}
SEAnesthesiaMachineInspiratoryValveObstruction* SEEquipmentActionCollection::GetAnesthesiaMachineInspiratoryValveObstruction()
{
  return m_AnesthesiaMachineInspiratoryValveObstruction;
}
const SEAnesthesiaMachineInspiratoryValveObstruction* SEEquipmentActionCollection::GetAnesthesiaMachineInspiratoryValveObstruction() const
{
  return m_AnesthesiaMachineInspiratoryValveObstruction;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineInspiratoryValveObstruction()
{
  SAFE_DELETE(m_AnesthesiaMachineInspiratoryValveObstruction);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineMaskLeak() const
{
  return m_AnesthesiaMachineMaskLeak == nullptr ? false : true;
}
SEAnesthesiaMachineMaskLeak* SEEquipmentActionCollection::GetAnesthesiaMachineMaskLeak()
{
  return m_AnesthesiaMachineMaskLeak;
}
const SEAnesthesiaMachineMaskLeak* SEEquipmentActionCollection::GetAnesthesiaMachineMaskLeak() const
{
  return m_AnesthesiaMachineMaskLeak;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineMaskLeak()
{
  SAFE_DELETE(m_AnesthesiaMachineMaskLeak);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineSodaLimeFailure() const
{
  return m_AnesthesiaMachineSodaLimeFailure == nullptr ? false : true;
}
SEAnesthesiaMachineSodaLimeFailure* SEEquipmentActionCollection::GetAnesthesiaMachineSodaLimeFailure()
{
  return m_AnesthesiaMachineSodaLimeFailure;
}
const SEAnesthesiaMachineSodaLimeFailure* SEEquipmentActionCollection::GetAnesthesiaMachineSodaLimeFailure() const
{
  return m_AnesthesiaMachineSodaLimeFailure;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineSodaLimeFailure()
{
  SAFE_DELETE(m_AnesthesiaMachineSodaLimeFailure);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineTubeCuffLeak() const
{
  return m_AnesthesiaMachineTubeCuffLeak == nullptr ? false : true;
}
SEAnesthesiaMachineTubeCuffLeak* SEEquipmentActionCollection::GetAnesthesiaMachineTubeCuffLeak()
{
  return m_AnesthesiaMachineTubeCuffLeak;
}
const SEAnesthesiaMachineTubeCuffLeak* SEEquipmentActionCollection::GetAnesthesiaMachineTubeCuffLeak() const
{
  return m_AnesthesiaMachineTubeCuffLeak;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineTubeCuffLeak()
{
  SAFE_DELETE(m_AnesthesiaMachineTubeCuffLeak);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineVaporizerFailure() const
{
  return m_AnesthesiaMachineVaporizerFailure == nullptr ? false : true;
}
SEAnesthesiaMachineVaporizerFailure* SEEquipmentActionCollection::GetAnesthesiaMachineVaporizerFailure()
{
  return m_AnesthesiaMachineVaporizerFailure;
}
const SEAnesthesiaMachineVaporizerFailure* SEEquipmentActionCollection::GetAnesthesiaMachineVaporizerFailure() const
{
  return m_AnesthesiaMachineVaporizerFailure;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineVaporizerFailure()
{
  SAFE_DELETE(m_AnesthesiaMachineVaporizerFailure);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineVentilatorPressureLoss() const
{
  return m_AnesthesiaMachineVentilatorPressureLoss == nullptr ? false : true;
}
SEAnesthesiaMachineVentilatorPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineVentilatorPressureLoss()
{
  return m_AnesthesiaMachineVentilatorPressureLoss;
}
const SEAnesthesiaMachineVentilatorPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineVentilatorPressureLoss() const
{
  return m_AnesthesiaMachineVentilatorPressureLoss;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineVentilatorPressureLoss()
{
  SAFE_DELETE(m_AnesthesiaMachineVentilatorPressureLoss);
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineYPieceDisconnect() const
{
  return m_AnesthesiaMachineYPieceDisconnect == nullptr ? false : true;
}
SEAnesthesiaMachineYPieceDisconnect* SEEquipmentActionCollection::GetAnesthesiaMachineYPieceDisconnect()
{
  return m_AnesthesiaMachineYPieceDisconnect;
}
const SEAnesthesiaMachineYPieceDisconnect* SEEquipmentActionCollection::GetAnesthesiaMachineYPieceDisconnect() const
{
  return m_AnesthesiaMachineYPieceDisconnect;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineYPieceDisconnect()
{
  SAFE_DELETE(m_AnesthesiaMachineYPieceDisconnect);
}

bool SEEquipmentActionCollection::HasInhalerConfiguration() const
{
  return m_InhalerConfiguration == nullptr ? false : true;
}
SEInhalerConfiguration* SEEquipmentActionCollection::GetInhalerConfiguration()
{
  return m_InhalerConfiguration;
}
const SEInhalerConfiguration* SEEquipmentActionCollection::GetInhalerConfiguration() const
{
  return m_InhalerConfiguration;
}
void SEEquipmentActionCollection::RemoveInhalerConfiguration()
{
  SAFE_DELETE(m_InhalerConfiguration);
}

bool SEEquipmentActionCollection::HasMechanicalVentilatorConfiguration() const
{
  return m_MechanicalVentilatorConfiguration == nullptr ? false : true;
}
SEMechanicalVentilatorConfiguration* SEEquipmentActionCollection::GetMechanicalVentilatorConfiguration()
{
  return m_MechanicalVentilatorConfiguration;
}
const SEMechanicalVentilatorConfiguration* SEEquipmentActionCollection::GetMechanicalVentilatorConfiguration() const
{
  return m_MechanicalVentilatorConfiguration;
}
void SEEquipmentActionCollection::RemoveMechanicalVentilatorConfiguration()
{
  SAFE_DELETE(m_MechanicalVentilatorConfiguration);
}

void SEEquipmentActionCollection::GetAllActions(std::vector<const SEAction*>& actions) const
{
  if (HasAnesthesiaMachineConfiguration())
    actions.push_back(GetAnesthesiaMachineConfiguration());
  if (HasAnesthesiaMachineOxygenTankPressureLoss())
    actions.push_back(GetAnesthesiaMachineOxygenTankPressureLoss());
  if (HasAnesthesiaMachineOxygenWallPortPressureLoss())
    actions.push_back(GetAnesthesiaMachineOxygenWallPortPressureLoss());
  if (HasAnesthesiaMachineExpiratoryValveLeak())
    actions.push_back(GetAnesthesiaMachineExpiratoryValveLeak());
  if (HasAnesthesiaMachineExpiratoryValveObstruction())
    actions.push_back(GetAnesthesiaMachineExpiratoryValveObstruction());
  if (HasAnesthesiaMachineInspiratoryValveLeak())
    actions.push_back(GetAnesthesiaMachineInspiratoryValveLeak());
  if (HasAnesthesiaMachineInspiratoryValveObstruction())
    actions.push_back(GetAnesthesiaMachineInspiratoryValveObstruction());
  if (HasAnesthesiaMachineMaskLeak())
    actions.push_back(GetAnesthesiaMachineMaskLeak());
  if (HasAnesthesiaMachineSodaLimeFailure())
    actions.push_back(GetAnesthesiaMachineSodaLimeFailure());
  if (HasAnesthesiaMachineTubeCuffLeak())
    actions.push_back(GetAnesthesiaMachineTubeCuffLeak());
  if (HasAnesthesiaMachineVaporizerFailure())
    actions.push_back(GetAnesthesiaMachineVaporizerFailure());
  if (HasAnesthesiaMachineVentilatorPressureLoss())
    actions.push_back(GetAnesthesiaMachineVentilatorPressureLoss());
  if (HasAnesthesiaMachineYPieceDisconnect())
    actions.push_back(GetAnesthesiaMachineYPieceDisconnect());

  if(HasInhalerConfiguration())
    actions.push_back(GetInhalerConfiguration());

  if(HasMechanicalVentilatorConfiguration())
    actions.push_back(GetMechanicalVentilatorConfiguration());
}