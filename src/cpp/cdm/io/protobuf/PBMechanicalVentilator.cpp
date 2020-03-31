/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/MechanicalVentilator.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBMechanicalVentilator.h"
#include "io/protobuf/PBSubstance.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"

void PBMechanicalVentilator::Load(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst)
{
  PBMechanicalVentilator::Serialize(src, dst);
}
void PBMechanicalVentilator::Serialize(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst)
{
  dst.Clear();
  if (src.has_breathprofile())
  {
    if(src.breathprofile().has_rate())
      PBProperty::Load(src.breathprofile().rate(), dst.GetRespiratoryRate());
    if (src.breathprofile().has_period())
      PBProperty::Load(src.breathprofile().period(), dst.GetBreathPeriod());
    if (src.breathprofile().has_inspiratoryexpiratoryratio())
      PBProperty::Load(src.breathprofile().inspiratoryexpiratoryratio(), dst.GetInspiratoryExpiratoryRatio());
  }
  else if (src.has_inspiratoryexpiratoryperiod())
  {
    if (src.inspiratoryexpiratoryperiod().has_expiratoryperiod())
      PBProperty::Load(src.inspiratoryexpiratoryperiod().expiratoryperiod(), dst.GetExpiratoryPeriod());
    if (src.inspiratoryexpiratoryperiod().has_inspiratoryperiod())
      PBProperty::Load(src.inspiratoryexpiratoryperiod().inspiratoryperiod(), dst.GetInspiratoryPeriod());
  }
  dst.SetConnection((eMechanicalVentilator_Connection)src.connection());
  dst.SetControl((eMechanicalVentilator_Control)src.control());
  dst.SetDriverWaveform((eMechanicalVentilator_DriverWaveform)src.driverwaveform());

  if (src.has_positiveendexpiredpressure())
    PBProperty::Load(src.positiveendexpiredpressure(), dst.GetPositiveEndExpiredPressure());
  if (src.has_peakinspiratorypressure())
    PBProperty::Load(src.peakinspiratorypressure(), dst.GetPeakInspiratoryPressure());

  SESubstance* sub;
  for (int i = 0; i < src.fractioninspiredgas_size(); i++)
  {
    const CDM_BIND::SubstanceFractionData& sfData = src.fractioninspiredgas()[i];
    sub = dst.m_Substances.GetSubstance(sfData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring a mechanical ventilator fraction inspired gas that was not found : " + sfData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Gas)
    {
      dst.Error("Ignoring a mechanical ventilator fraction gas that is not a gas : " + sfData.name());
      continue;
    }
    PBSubstance::Load(sfData, dst.GetFractionInspiredGas(*sub));
  }

  dst.StateChange();
}

CDM_BIND::MechanicalVentilatorData* PBMechanicalVentilator::Unload(const SEMechanicalVentilator& src)
{
  CDM_BIND::MechanicalVentilatorData* dst = new CDM_BIND::MechanicalVentilatorData();
  PBMechanicalVentilator::Serialize(src, *dst);
  return dst;
}
void PBMechanicalVentilator::Serialize(const SEMechanicalVentilator& src, CDM_BIND::MechanicalVentilatorData& dst)
{
  dst.set_connection((CDM_BIND::MechanicalVentilatorData::eConnection)src.m_Connection);
  dst.set_control((CDM_BIND::MechanicalVentilatorData::eControl)src.m_Control);
  dst.set_driverwaveform((CDM_BIND::MechanicalVentilatorData::eDriverWaveform)src.m_DriverWaveform);

  if ((src.HasBreathPeriod() || src.HasRespiratoryRate()) && src.HasInspiratoryExpiratoryRatio())
  {
    if (src.HasBreathPeriod())
      dst.mutable_breathprofile()->set_allocated_period(PBProperty::Unload(*src.m_BreathPeriod));
    else if (src.HasRespiratoryRate())
      dst.mutable_breathprofile()->set_allocated_rate(PBProperty::Unload(*src.m_RespiratoryRate));
    dst.mutable_breathprofile()->set_allocated_inspiratoryexpiratoryratio(PBProperty::Unload(*src.m_InspiratoryExpiratoryRatio));
  }
  else if (src.HasExpiratoryPeriod() && src.HasInspiratoryPeriod())
  {
    dst.mutable_inspiratoryexpiratoryperiod()->set_allocated_expiratoryperiod(PBProperty::Unload(*src.m_ExpiratoryPeriod));
    dst.mutable_inspiratoryexpiratoryperiod()->set_allocated_inspiratoryperiod(PBProperty::Unload(*src.m_InspiratoryPeriod));
  }

  if (src.HasPeakInspiratoryPressure())
    dst.set_allocated_peakinspiratorypressure(PBProperty::Unload(*src.m_PeakInspiratoryPressure));
  if (src.HasPositiveEndExpiredPressure())
    dst.set_allocated_positiveendexpiredpressure(PBProperty::Unload(*src.m_PositiveEndExpiredPressure));

  for (SESubstanceFraction* sf : src.m_FractionInspiredGases)
    dst.mutable_fractioninspiredgas()->AddAllocated(PBSubstance::Unload(*sf));
}

bool PBMechanicalVentilator::SerializeToString(const SEMechanicalVentilator& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::MechanicalVentilatorData data;
  PBMechanicalVentilator::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBMechanicalVentilator::SerializeToFile(const SEMechanicalVentilator& src, const std::string& filename, SerializationFormat m)
{
  CDM_BIND::MechanicalVentilatorData data;
  PBMechanicalVentilator::Serialize(src, data);
  std::string content;
  PBMechanicalVentilator::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}

bool PBMechanicalVentilator::SerializeFromString(const std::string& src, SEMechanicalVentilator& dst, SerializationFormat m)
{
  CDM_BIND::MechanicalVentilatorData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBMechanicalVentilator::Load(data, dst);
  return true;
}
bool PBMechanicalVentilator::SerializeFromFile(const std::string& filename, SEMechanicalVentilator& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBMechanicalVentilator::SerializeFromString(content, dst, m);
}
