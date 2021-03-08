/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachineChamber.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineConfiguration::SEAnesthesiaMachineConfiguration(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_ConfigurationFile = "";
  m_Configuration = nullptr;
}

SEAnesthesiaMachineConfiguration::~SEAnesthesiaMachineConfiguration()
{
  m_ConfigurationFile = "";
  SAFE_DELETE(m_Configuration);
}

void SEAnesthesiaMachineConfiguration::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_ConfigurationFile = "";
  if (m_Configuration)
    m_Configuration->Clear();
}

void SEAnesthesiaMachineConfiguration::Copy(const SEAnesthesiaMachineConfiguration& src, const SESubstanceManager& subMgr, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEAnesthesiaMachineConfiguration::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}

bool SEAnesthesiaMachineConfiguration::IsActive() const
{
  return SEAnesthesiaMachineAction::IsActive();
}
void SEAnesthesiaMachineConfiguration::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineConfiguration::GetScalar(const std::string& name)
{
  return GetConfiguration().GetScalar(name);
}

bool SEAnesthesiaMachineConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SEAnesthesiaMachine& SEAnesthesiaMachineConfiguration::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new SEAnesthesiaMachine(GetLogger());
  return *m_Configuration;
}
const SEAnesthesiaMachine* SEAnesthesiaMachineConfiguration::GetConfiguration() const
{
  return m_Configuration;
}

std::string SEAnesthesiaMachineConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
void SEAnesthesiaMachineConfiguration::SetConfigurationFile(const std::string& fileName)
{
  m_ConfigurationFile = fileName;
}
bool SEAnesthesiaMachineConfiguration::HasConfigurationFile() const
{
  return !m_ConfigurationFile.empty();
}

void SEAnesthesiaMachineConfiguration::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasConfigurationFile())
  {
    str << "\n\tConfiguration File: "; str << m_ConfigurationFile;
  }
  else if (HasConfiguration())
  {
    str << "\n\tConnection: " << eAnesthesiaMachine_Connection_Name(m_Configuration->GetConnection());
    str << "\n\tInlet Flow Rate: "; m_Configuration->HasInletFlow() ? str << m_Configuration->GetInletFlow() : str << "NaN";
    str << "\n\tInspiratoryExpiratory Ratio: "; m_Configuration->HasInspiratoryExpiratoryRatio() ? str << m_Configuration->GetInspiratoryExpiratoryRatio() : str << "NaN";
    str << "\n\tOxygen Fraction: "; m_Configuration->HasOxygenFraction() ? str << m_Configuration->GetOxygenFraction() : str << "NaN";
    str << "\n\tOxygen Source: " << eAnesthesiaMachine_OxygenSource_Name(m_Configuration->GetOxygenSource());
    str << "\n\tPositive End Expired Pressure: "; m_Configuration->HasPositiveEndExpiredPressure() ? str << m_Configuration->GetPositiveEndExpiredPressure() : str << "NaN";
    str << "\n\tPrimary Gas: " << eAnesthesiaMachine_PrimaryGas_Name(m_Configuration->GetPrimaryGas());
    str << "\n\tRelief Valve Pressure: "; m_Configuration->HasReliefValvePressure() ? str << m_Configuration->GetReliefValvePressure() : str << "NaN";
    str << "\n\tRespiratory Rate: "; m_Configuration->HasRespiratoryRate() ? str << m_Configuration->GetRespiratoryRate() : str << "NaN";
    str << "\n\tPeak Inspiratory Pressure: "; m_Configuration->HasPeakInspiratoryPressure() ? str << m_Configuration->GetPeakInspiratoryPressure() : str << "NaN";
    if (m_Configuration->HasLeftChamber())
    {
      str << "\n\tLeftChamberState: " << eSwitch_Name(m_Configuration->GetLeftChamber().GetState());
      str << "\n\tLeftChamberSubstance: "; m_Configuration->GetLeftChamber().HasSubstance() ? str << m_Configuration->GetLeftChamber().GetSubstance()->GetName() : str << "Not Set";
      str << "\n\tLeftChamberSubstanceAmount: "; m_Configuration->GetLeftChamber().HasSubstanceFraction() ? str << m_Configuration->GetLeftChamber().GetSubstanceFraction() : str << "Not Set";
    }
    if (m_Configuration->HasRightChamber())
    {
      str << "\n\tRightChamberState: " << eSwitch_Name(m_Configuration->GetRightChamber().GetState());
      str << "\n\tRightChamberSubstance: "; m_Configuration->GetRightChamber().HasSubstance() ? str << m_Configuration->GetRightChamber().GetSubstance()->GetName() : str << "Not Set";
      str << "\n\tRightChamberSubstanceAmount: "; m_Configuration->GetRightChamber().HasSubstanceFraction() ? str << m_Configuration->GetRightChamber().GetSubstanceFraction() : str << "Not Set";
    }
    if (m_Configuration->HasOxygenBottleOne())
    {
      str << "\n\tOxygenBottleOneVolume: "; m_Configuration->GetOxygenBottleOne().HasVolume() ? str << m_Configuration->GetOxygenBottleOne().GetVolume() : str << "Not Set";
    }
    if (m_Configuration->HasOxygenBottleTwo())
    {
      str << "\n\tOxygenBottleTwoVolume: "; m_Configuration->GetOxygenBottleTwo().HasVolume() ? str << m_Configuration->GetOxygenBottleTwo().GetVolume() : str << "Not Set";
    }
  }
  str << std::flush;
}
