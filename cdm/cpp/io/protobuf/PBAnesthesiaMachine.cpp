/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBAnesthesiaMachine.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cdm/AnesthesiaMachine.pb.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineChamber.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineOxygenBottle.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"


void PBAnesthesiaMachine::Load(const cdm::AnesthesiaMachineData& src, SEAnesthesiaMachine& dst)
{
  PBAnesthesiaMachine::Serialize(src, dst);
}
void PBAnesthesiaMachine::Serialize(const cdm::AnesthesiaMachineData& src, SEAnesthesiaMachine& dst)
{
  dst.Clear();
  dst.SetConnection((eAnesthesiaMachine_Connection)src.connection());
  if (src.has_inletflow())
    PBProperty::Load(src.inletflow(), dst.GetInletFlow());
  if (src.has_inspiratoryexpiratoryratio())
    PBProperty::Load(src.inspiratoryexpiratoryratio(), dst.GetInspiratoryExpiratoryRatio());
  if (src.has_oxygenfraction())
    PBProperty::Load(src.oxygenfraction(), dst.GetOxygenFraction());

  dst.SetOxygenSource((eAnesthesiaMachine_OxygenSource)src.oxygensource());
  if (src.has_positiveendexpiredpressure())
    PBProperty::Load(src.positiveendexpiredpressure(), dst.GetPositiveEndExpiredPressure());
  dst.SetPrimaryGas((eAnesthesiaMachine_PrimaryGas)src.primarygas());

  if (src.has_respiratoryrate())
    PBProperty::Load(src.respiratoryrate(), dst.GetRespiratoryRate());
  if (src.has_reliefvalvepressure())
    PBProperty::Load(src.reliefvalvepressure(), dst.GetReliefValvePressure());
  if (src.has_ventilatorpressure())
    PBProperty::Load(src.ventilatorpressure(), dst.GetVentilatorPressure());
  if (src.has_leftchamber())
    PBAnesthesiaMachine::Load(src.leftchamber(), dst.GetLeftChamber());
  if (src.has_rightchamber())
    PBAnesthesiaMachine::Load(src.rightchamber(), dst.GetRightChamber());
  if (src.has_oxygenbottleone())
    PBAnesthesiaMachine::Load(src.oxygenbottleone(), dst.GetOxygenBottleOne());
  if (src.has_oxygenbottletwo())
    PBAnesthesiaMachine::Load(src.oxygenbottletwo(), dst.GetOxygenBottleTwo());

  SEScalarTime time;
  for (int i = 0; i < src.activeevent_size(); i++)
  {
    const cdm::AnesthesiaMachineData::ActiveEventData& e = src.activeevent(i);
    if (e.has_duration())
      PBProperty::Load(e.duration(), time);
    {
      dst.m_ss << "Active AnesthesiaMachine event " << e.event() << " does not have time associated with it";
      dst.Warning(dst.m_ss);
      time.SetValue(0, TimeUnit::s);
    }
    dst.m_EventState[(eAnesthesiaMachine_Event)e.event()] = true;
    dst.m_EventDuration_s[(eAnesthesiaMachine_Event)e.event()] = time.GetValue(TimeUnit::s);
  }

  dst.StateChange();
}

cdm::AnesthesiaMachineData* PBAnesthesiaMachine::Unload(const SEAnesthesiaMachine& src)
{
  cdm::AnesthesiaMachineData* dst = new cdm::AnesthesiaMachineData();
  PBAnesthesiaMachine::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachine::Serialize(const SEAnesthesiaMachine& src, cdm::AnesthesiaMachineData& dst)
{
  dst.set_connection((cdm::eAnesthesiaMachine_Connection)src.m_Connection);
  if (src.HasInletFlow())
    dst.set_allocated_inletflow(PBProperty::Unload(*src.m_InletFlow));
  if (src.HasInspiratoryExpiratoryRatio())
    dst.set_allocated_inspiratoryexpiratoryratio(PBProperty::Unload(*src.m_InspiratoryExpiratoryRatio));
  if (src.HasOxygenFraction())
    dst.set_allocated_oxygenfraction(PBProperty::Unload(*src.m_OxygenFraction));

  dst.set_oxygensource((cdm::eAnesthesiaMachine_OxygenSource)src.m_OxygenSource);
  if (src.HasPositiveEndExpiredPressure())
    dst.set_allocated_positiveendexpiredpressure(PBProperty::Unload(*src.m_PositiveEndExpiredPressure));
  dst.set_primarygas((cdm::eAnesthesiaMachine_PrimaryGas)src.m_PrimaryGas);

  if (src.HasRespiratoryRate())
    dst.set_allocated_respiratoryrate(PBProperty::Unload(*src.m_RespiratoryRate));
  if (src.HasReliefValvePressure())
    dst.set_allocated_reliefvalvepressure(PBProperty::Unload(*src.m_ReliefValvePressure));
  if (src.HasVentilatorPressure())
    dst.set_allocated_ventilatorpressure(PBProperty::Unload(*src.m_VentilatorPressure));
  if (src.HasLeftChamber())
    dst.set_allocated_leftchamber(PBAnesthesiaMachine::Unload(*src.m_LeftChamber));
  if (src.HasRightChamber())
    dst.set_allocated_rightchamber(PBAnesthesiaMachine::Unload(*src.m_RightChamber));
  if (src.HasOxygenBottleOne())
    dst.set_allocated_oxygenbottleone(PBAnesthesiaMachine::Unload(*src.m_OxygenBottleOne));
  if (src.HasOxygenBottleTwo())
    dst.set_allocated_oxygenbottletwo(PBAnesthesiaMachine::Unload(*src.m_OxygenBottleTwo));

  SEScalarTime time;
  for (auto itr : src.m_EventState)
  {
    if (!itr.second)
      continue;

    auto it2 = src.m_EventDuration_s.find(itr.first);
    if (it2 == src.m_EventDuration_s.end())// This should not happen... 
      time.SetValue(0, TimeUnit::s);
    else
      time.SetValue(it2->second, TimeUnit::s);

    cdm::AnesthesiaMachineData_ActiveEventData* eData = dst.add_activeevent();

    eData->set_event((cdm::eAnesthesiaMachine_Event)itr.first);
    eData->set_allocated_duration(PBProperty::Unload(time));
  }
}

void PBAnesthesiaMachine::Load(const cdm::AnesthesiaMachineChamberData& src, SEAnesthesiaMachineChamber& dst)
{
  PBAnesthesiaMachine::Serialize(src, dst);
}
void PBAnesthesiaMachine::Serialize(const cdm::AnesthesiaMachineChamberData& src, SEAnesthesiaMachineChamber& dst)
{
  dst.Clear();
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
  if (src.has_substancefraction())
    PBProperty::Load(src.substancefraction(), dst.GetSubstanceFraction());

  if (!src.substance().empty())
  {
    dst.m_Substance = dst.m_Substances.GetSubstance(src.substance());
    if (dst.m_Substance == nullptr)
    {
      dst.Error("Do not have substance : " + src.substance(), "SEAnesthesiaMachineChamber::Serialize");
    }
  }
}

cdm::AnesthesiaMachineChamberData* PBAnesthesiaMachine::Unload(const SEAnesthesiaMachineChamber& src)
{
  cdm::AnesthesiaMachineChamberData* dst = new cdm::AnesthesiaMachineChamberData();
  PBAnesthesiaMachine::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachine::Serialize(const SEAnesthesiaMachineChamber& src, cdm::AnesthesiaMachineChamberData& dst)
{
  dst.set_state((cdm::eSwitch)src.m_State);
  if (src.HasSubstanceFraction())
    dst.set_allocated_substancefraction(PBProperty::Unload(*src.m_SubstanceFraction));
  dst.set_substance(src.m_Substance->GetName());
}

void PBAnesthesiaMachine::Load(const cdm::AnesthesiaMachineOxygenBottleData& src, SEAnesthesiaMachineOxygenBottle& dst)
{
  PBAnesthesiaMachine::Serialize(src, dst);
}
void PBAnesthesiaMachine::Serialize(const cdm::AnesthesiaMachineOxygenBottleData& src, SEAnesthesiaMachineOxygenBottle& dst)
{
  dst.Clear();
  if (src.has_volume())
    PBProperty::Load(src.volume(), dst.GetVolume());
}

cdm::AnesthesiaMachineOxygenBottleData* PBAnesthesiaMachine::Unload(const SEAnesthesiaMachineOxygenBottle& src)
{
  cdm::AnesthesiaMachineOxygenBottleData* dst = new cdm::AnesthesiaMachineOxygenBottleData();
  PBAnesthesiaMachine::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachine::Serialize(const SEAnesthesiaMachineOxygenBottle& src, cdm::AnesthesiaMachineOxygenBottleData& dst)
{
  if (src.HasVolume())
    dst.set_allocated_volume(PBProperty::Unload(*src.m_Volume));
}

bool PBAnesthesiaMachine::SerializeToString(const SEAnesthesiaMachine& src, std::string& output, SerializationMode m)
{
  cdm::AnesthesiaMachineData data;
  PBAnesthesiaMachine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBAnesthesiaMachine::SerializeToFile(const SEAnesthesiaMachine& src, const std::string& filename, SerializationMode m)
{
  cdm::AnesthesiaMachineData data;
  PBAnesthesiaMachine::Serialize(src, data);
  std::string content;
  PBAnesthesiaMachine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}

bool PBAnesthesiaMachine::SerializeFromString(const std::string& src, SEAnesthesiaMachine& dst, SerializationMode m)
{
  cdm::AnesthesiaMachineData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBAnesthesiaMachine::Load(data, dst);
  return true;
}
bool PBAnesthesiaMachine::SerializeFromFile(const std::string& filename, SEAnesthesiaMachine& dst, SerializationMode m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBAnesthesiaMachine::SerializeFromString(content, dst, m);
}