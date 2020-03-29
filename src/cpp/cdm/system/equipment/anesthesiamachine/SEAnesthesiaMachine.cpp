/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineChamber.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineOxygenBottle.h"
#include "substance/SESubstanceManager.h"

// State Actions
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineConfiguration.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBAnesthesiaMachine.h"

SEAnesthesiaMachine::SEAnesthesiaMachine(SESubstanceManager& substances) : SESystem(substances.GetLogger()), m_Substances(substances)
{
  m_Connection = eAnesthesiaMachine_Connection::NullConnection;
  m_InletFlow = nullptr;
  m_InspiratoryExpiratoryRatio = nullptr;
  m_OxygenFraction = nullptr;
  m_OxygenSource = eAnesthesiaMachine_OxygenSource::NullSource;
  m_PositiveEndExpiredPressure = nullptr;
  m_PrimaryGas = eAnesthesiaMachine_PrimaryGas::NullGas;  
  m_RespiratoryRate = nullptr;
  m_ReliefValvePressure = nullptr;
  m_VentilatorPressure = nullptr;
  m_LeftChamber = nullptr;
  m_RightChamber = nullptr;
  m_OxygenBottleOne = nullptr;
  m_OxygenBottleTwo = nullptr;
}

SEAnesthesiaMachine::~SEAnesthesiaMachine()
{
  Clear();
}

void SEAnesthesiaMachine::Clear()
{
  SESystem::Clear();

  m_Connection = eAnesthesiaMachine_Connection::NullConnection;
  SAFE_DELETE(m_InletFlow);
  SAFE_DELETE(m_InspiratoryExpiratoryRatio);
  SAFE_DELETE(m_OxygenFraction);
  m_OxygenSource = eAnesthesiaMachine_OxygenSource::NullSource;
  SAFE_DELETE(m_PositiveEndExpiredPressure);
  m_PrimaryGas = eAnesthesiaMachine_PrimaryGas::NullGas;
  SAFE_DELETE(m_RespiratoryRate);
  SAFE_DELETE(m_ReliefValvePressure);
  SAFE_DELETE(m_VentilatorPressure);
  SAFE_DELETE(m_LeftChamber);
  SAFE_DELETE(m_RightChamber);
  SAFE_DELETE(m_OxygenBottleOne);
  SAFE_DELETE(m_OxygenBottleTwo);
}

void SEAnesthesiaMachine::Merge(const SEAnesthesiaMachine& from)
{
  if(from.m_Connection!=eAnesthesiaMachine_Connection::NullConnection)
    SetConnection(from.m_Connection);
  COPY_PROPERTY(InletFlow);
  COPY_PROPERTY(InspiratoryExpiratoryRatio);
  COPY_PROPERTY(OxygenFraction);
  if (from.m_OxygenSource != eAnesthesiaMachine_OxygenSource::NullSource)
    SetOxygenSource(from.m_OxygenSource);
  COPY_PROPERTY(PositiveEndExpiredPressure);
  if (from.m_PrimaryGas != eAnesthesiaMachine_PrimaryGas::NullGas)
    SetPrimaryGas(from.m_PrimaryGas);
  COPY_PROPERTY(RespiratoryRate);
  COPY_PROPERTY(ReliefValvePressure);
  COPY_PROPERTY(VentilatorPressure);

  MERGE_CHILD(LeftChamber);
  MERGE_CHILD(RightChamber);
 
  MERGE_CHILD(OxygenBottleOne);
  MERGE_CHILD(OxygenBottleTwo);
}

bool SEAnesthesiaMachine::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBAnesthesiaMachine::SerializeToString(*this, output, m);
}
bool SEAnesthesiaMachine::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBAnesthesiaMachine::SerializeToFile(*this, filename, m);
}
bool SEAnesthesiaMachine::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBAnesthesiaMachine::SerializeFromString(src, *this, m);
}
bool SEAnesthesiaMachine::SerializeFromFile(const std::string& filename, SerializationFormat m)
{
  return PBAnesthesiaMachine::SerializeFromFile(filename, *this, m);
}

void SEAnesthesiaMachine::ProcessConfiguration(SEAnesthesiaMachineConfiguration& config)
{
  if (config.HasConfiguration())
    Merge(config.GetConfiguration());
  else if (config.HasConfigurationFile())
  {
    // Update the action with the file contents
    std::string cfg_file = config.GetConfigurationFile();
    if (!config.GetConfiguration().SerializeFromFile(cfg_file, JSON))
      Error("Unable to load configuration file", "SEAnesthesiaMachine::ProcessConfiguration");
    Merge(config.GetConfiguration());
  }
  StateChange();
}

const SEScalar* SEAnesthesiaMachine::GetScalar(const std::string& name)
{
  if (name == "InletFlow")
    return &GetInletFlow();
  if (name == "InspiratoryExpiratoryRatio")
    return &GetInspiratoryExpiratoryRatio();
  if (name == "OxygenFraction")
    return &GetOxygenFraction();
  if (name == "PositiveEndExpiredPressure")
    return &GetPositiveEndExpiredPressure();
  if (name == "ReliefValvePressure")
    return &GetReliefValvePressure();
  if (name == "RespiratoryRate")
    return &GetRespiratoryRate();
  if (name == "VentilatorPressure")
    return &GetVentilatorPressure();

  size_t split = name.find('-');
  if (split != name.npos)
  {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == "LeftChamber")
      return GetLeftChamber().GetScalar(prop);
    if (child == "RightChamber")
      return GetRightChamber().GetScalar(prop);
    if (child == "OxygenBottleOne")
      return GetOxygenBottleOne().GetScalar(prop);
    if (child == "OxygenBottleTwo")
      return GetOxygenBottleTwo().GetScalar(prop);
  }

  return nullptr;
}

void SEAnesthesiaMachine::SetConnection(eAnesthesiaMachine_Connection c)
{
  m_Connection = c;
}
eAnesthesiaMachine_Connection SEAnesthesiaMachine::GetConnection() const
{
  return m_Connection;
}

bool SEAnesthesiaMachine::HasInletFlow() const
{
  return m_InletFlow == nullptr ? false : m_InletFlow->IsValid();
}
SEScalarVolumePerTime& SEAnesthesiaMachine::GetInletFlow()
{
  if (m_InletFlow == nullptr)
    m_InletFlow = new SEScalarVolumePerTime();
  return *m_InletFlow;
}
double SEAnesthesiaMachine::GetInletFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InletFlow == nullptr)
    return SEScalar::dNaN();
  return m_InletFlow->GetValue(unit);
}

bool SEAnesthesiaMachine::HasInspiratoryExpiratoryRatio() const
{
  return m_InspiratoryExpiratoryRatio == nullptr ? false : m_InspiratoryExpiratoryRatio->IsValid();
}
SEScalar& SEAnesthesiaMachine::GetInspiratoryExpiratoryRatio()
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    m_InspiratoryExpiratoryRatio = new SEScalar();
  return *m_InspiratoryExpiratoryRatio;
}
double SEAnesthesiaMachine::GetInspiratoryExpiratoryRatio() const
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryExpiratoryRatio->GetValue();
}

bool SEAnesthesiaMachine::HasOxygenFraction() const
{
  return m_OxygenFraction == nullptr ? false : m_OxygenFraction->IsValid();
}
SEScalar0To1& SEAnesthesiaMachine::GetOxygenFraction()
{
  if (m_OxygenFraction == nullptr)
    m_OxygenFraction = new SEScalar0To1();
  return *m_OxygenFraction;
}
double SEAnesthesiaMachine::GetOxygenFraction() const
{
  if (m_OxygenFraction == nullptr)
    return SEScalar::dNaN();
  return m_OxygenFraction->GetValue();
}

eAnesthesiaMachine_OxygenSource SEAnesthesiaMachine::GetOxygenSource() const
{
  return m_OxygenSource;
}
void SEAnesthesiaMachine::SetOxygenSource(eAnesthesiaMachine_OxygenSource src)
{
  m_OxygenSource = src;
}

bool SEAnesthesiaMachine::HasPositiveEndExpiredPressure() const
{
  return m_PositiveEndExpiredPressure == nullptr ? false : m_PositiveEndExpiredPressure->IsValid();
}
SEScalarPressure& SEAnesthesiaMachine::GetPositiveEndExpiredPressure()
{
  if (m_PositiveEndExpiredPressure == nullptr)
    m_PositiveEndExpiredPressure = new SEScalarPressure();
  return *m_PositiveEndExpiredPressure;
}
double SEAnesthesiaMachine::GetPositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_PositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_PositiveEndExpiredPressure->GetValue(unit);
}

eAnesthesiaMachine_PrimaryGas SEAnesthesiaMachine::GetPrimaryGas() const
{
  return m_PrimaryGas;
}
void SEAnesthesiaMachine::SetPrimaryGas(eAnesthesiaMachine_PrimaryGas gas)
{
  m_PrimaryGas = gas;
}

bool SEAnesthesiaMachine::HasRespiratoryRate() const
{
  return m_RespiratoryRate == nullptr ? false : m_RespiratoryRate->IsValid();
}
SEScalarFrequency& SEAnesthesiaMachine::GetRespiratoryRate()
{
  if (m_RespiratoryRate == nullptr)
    m_RespiratoryRate = new SEScalarFrequency();
  return *m_RespiratoryRate;
}
double SEAnesthesiaMachine::GetRespiratoryRate(const FrequencyUnit& unit) const
{
  if (m_RespiratoryRate == nullptr)
    return SEScalar::dNaN();
  return m_RespiratoryRate->GetValue(unit);
}

bool SEAnesthesiaMachine::HasReliefValvePressure() const
{
  return m_ReliefValvePressure == nullptr ? false : m_ReliefValvePressure->IsValid();
}
SEScalarPressure& SEAnesthesiaMachine::GetReliefValvePressure()
{
  if (m_ReliefValvePressure == nullptr)
    m_ReliefValvePressure = new SEScalarPressure();
  return *m_ReliefValvePressure;
}
double SEAnesthesiaMachine::GetReliefValvePressure(const PressureUnit& unit) const
{
  if (m_ReliefValvePressure == nullptr)
    return SEScalar::dNaN();
  return m_ReliefValvePressure->GetValue(unit);
}

bool SEAnesthesiaMachine::HasVentilatorPressure() const
{
  return m_VentilatorPressure == nullptr ? false : m_VentilatorPressure->IsValid();
}
SEScalarPressure& SEAnesthesiaMachine::GetVentilatorPressure()
{
  if (m_VentilatorPressure == nullptr)
    m_VentilatorPressure = new SEScalarPressure();
  return *m_VentilatorPressure;
}
double SEAnesthesiaMachine::GetVentilatorPressure(const PressureUnit& unit) const
{
  if (m_VentilatorPressure == nullptr)
    return SEScalar::dNaN();
  return m_VentilatorPressure->GetValue(unit);
}

bool SEAnesthesiaMachine::HasLeftChamber() const
{
  return m_LeftChamber == nullptr ? false : true;
}
SEAnesthesiaMachineChamber& SEAnesthesiaMachine::GetLeftChamber()
{
  if (m_LeftChamber == nullptr)
    m_LeftChamber = new SEAnesthesiaMachineChamber(m_Substances);
  return *m_LeftChamber;
}
const SEAnesthesiaMachineChamber* SEAnesthesiaMachine::GetLeftChamber() const
{
  return m_LeftChamber;
}
void SEAnesthesiaMachine::RemoveLeftChamber()
{
  SAFE_DELETE(m_LeftChamber);
}

bool SEAnesthesiaMachine::HasRightChamber() const
{
  return m_RightChamber == nullptr ? false : true;
}
SEAnesthesiaMachineChamber& SEAnesthesiaMachine::GetRightChamber()
{
  if (m_RightChamber == nullptr)
    m_RightChamber = new SEAnesthesiaMachineChamber(m_Substances);
  return *m_RightChamber;
}
const SEAnesthesiaMachineChamber* SEAnesthesiaMachine::GetRightChamber() const
{
  return m_RightChamber;
}
void SEAnesthesiaMachine::RemoveRightChamber()
{
  SAFE_DELETE(m_RightChamber);
}

bool SEAnesthesiaMachine::HasOxygenBottleOne() const
{
  return m_OxygenBottleOne == nullptr ? false : true;
}
SEAnesthesiaMachineOxygenBottle& SEAnesthesiaMachine::GetOxygenBottleOne()
{
  if (m_OxygenBottleOne == nullptr)
    m_OxygenBottleOne = new SEAnesthesiaMachineOxygenBottle(GetLogger());
  return *m_OxygenBottleOne;
}
const SEAnesthesiaMachineOxygenBottle* SEAnesthesiaMachine::GetOxygenBottleOne() const
{
  return m_OxygenBottleOne;
}
void SEAnesthesiaMachine::RemoveOxygenBottleOne()
{
  SAFE_DELETE(m_OxygenBottleOne);
}

bool SEAnesthesiaMachine::HasOxygenBottleTwo() const
{
  return m_OxygenBottleTwo == nullptr ? false : true;
}
SEAnesthesiaMachineOxygenBottle& SEAnesthesiaMachine::GetOxygenBottleTwo()
{
  if (m_OxygenBottleTwo == nullptr)
    m_OxygenBottleTwo = new SEAnesthesiaMachineOxygenBottle(GetLogger());
  return *m_OxygenBottleTwo;
}
const SEAnesthesiaMachineOxygenBottle* SEAnesthesiaMachine::GetOxygenBottleTwo() const
{
  return m_OxygenBottleTwo;
}
void SEAnesthesiaMachine::RemoveOxygenBottleTwo()
{
  SAFE_DELETE(m_OxygenBottleTwo);
}
