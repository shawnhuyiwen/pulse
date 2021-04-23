/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/BagValveMask.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBBagValveMask.h"
#include "io/protobuf/PBSubstance.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"

void PBBagValveMask::Load(const CDM_BIND::BagValveMaskData& src, SEBagValveMask& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBBagValveMask::Serialize(src, dst, subMgr);
  dst.StateChange();
}
void PBBagValveMask::Serialize(const CDM_BIND::BagValveMaskData& src, SEBagValveMask& dst, const SESubstanceManager& subMgr)
{
  dst.m_Connection = (eBagValveMask_Connection)src.connection();

  if (src.has_bagresistance())
    PBProperty::Load(src.bagresistance(), dst.GetBagResistance());
  if (src.has_filterresistance())
    PBProperty::Load(src.filterresistance(), dst.GetFilterResistance());
  if (src.has_filtervolume())
    PBProperty::Load(src.filtervolume(), dst.GetFilterVolume());
  if (src.has_masksealresistance())
    PBProperty::Load(src.masksealresistance(), dst.GetMaskSealResistance());
  if (src.has_valveresistance())
    PBProperty::Load(src.valveresistance(), dst.GetValveResistance());

  if (src.has_breathfrequency())
    PBProperty::Load(src.breathfrequency(), dst.GetBreathFrequency());
  if (src.has_inspiratoryexpiratoryratio())
    PBProperty::Load(src.inspiratoryexpiratoryratio(), dst.GetInspiratoryExpiratoryRatio());
  if (src.has_positiveinspiratorypressure())
    PBProperty::Load(src.positiveinspiratorypressure(), dst.GetPositiveInspiratoryPressure());
  if (src.has_tidalvolume())
    PBProperty::Load(src.tidalvolume(), dst.GetTidalVolume());
  if (src.has_valvepositiveendexpiredpressure())
    PBProperty::Load(src.valvepositiveendexpiredpressure(), dst.GetValvePositiveEndExpiredPressure());

  const SESubstance* sub;
  for (int i = 0; i < src.fractioninspiredgas_size(); i++)
  {
    const CDM_BIND::SubstanceFractionData& sfData = src.fractioninspiredgas()[i];
    sub = subMgr.GetSubstance(sfData.name());
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

  for (int i = 0; i < src.concentrationinspiredaerosol_size(); i++)
  {
    const CDM_BIND::SubstanceConcentrationData& scData = src.concentrationinspiredaerosol()[i];
    sub = subMgr.GetSubstance(scData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring an mechanical ventilator aerosol concentration that was not found : " + scData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Liquid && sub->GetState() != eSubstance_State::Solid)
    {
      dst.Error("Ignoring an mechanical ventilator aerosol concentration that is not a gas : " + scData.name());
      continue;
    }
    PBSubstance::Load(scData, dst.GetConcentrationInspiredAerosol(*sub));
  }
}

CDM_BIND::BagValveMaskData* PBBagValveMask::Unload(const SEBagValveMask& src)
{
  CDM_BIND::BagValveMaskData* dst = new CDM_BIND::BagValveMaskData();
  PBBagValveMask::Serialize(src, *dst);
  return dst;
}
void PBBagValveMask::Serialize(const SEBagValveMask& src, CDM_BIND::BagValveMaskData& dst)
{
  dst.set_connection((CDM_BIND::BagValveMaskData::eConnection)src.m_Connection);

  if (src.HasBagResistance())
    dst.set_allocated_bagresistance(PBProperty::Unload(*src.m_BagResistance));
  if (src.HasFilterResistance())
    dst.set_allocated_filterresistance(PBProperty::Unload(*src.m_FilterResistance));
  if (src.HasFilterVolume())
    dst.set_allocated_filtervolume(PBProperty::Unload(*src.m_FilterVolume));
  if (src.HasMaskSealResistance())
    dst.set_allocated_masksealresistance(PBProperty::Unload(*src.m_MaskSealResistance));
  if (src.HasValveResistance())
    dst.set_allocated_valveresistance(PBProperty::Unload(*src.m_ValveResistance));

  if (src.HasBreathFrequency())
    dst.set_allocated_breathfrequency(PBProperty::Unload(*src.m_BreathFrequency));
  if (src.HasInspiratoryExpiratoryRatio())
    dst.set_allocated_inspiratoryexpiratoryratio(PBProperty::Unload(*src.m_InspiratoryExpiratoryRatio));
  if (src.HasPositiveInspiratoryPressure())
    dst.set_allocated_positiveinspiratorypressure(PBProperty::Unload(*src.m_PositiveInspiratoryPressure));
  if (src.HasTidalVolume())
    dst.set_allocated_tidalvolume(PBProperty::Unload(*src.m_TidalVolume));
  if (src.HasValvePositiveEndExpiredPressure())
    dst.set_allocated_valvepositiveendexpiredpressure(PBProperty::Unload(*src.m_ValvePositiveEndExpiredPressure));

  for (SESubstanceFraction* sf : src.m_FractionInspiredGases)
    dst.mutable_fractioninspiredgas()->AddAllocated(PBSubstance::Unload(*sf));

  for (SESubstanceConcentration* sc : src.m_ConcentrationInspiredAerosols)
    dst.mutable_concentrationinspiredaerosol()->AddAllocated(PBSubstance::Unload(*sc));
}

bool PBBagValveMask::SerializeToString(const SEBagValveMask& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::BagValveMaskData data;
  PBBagValveMask::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBBagValveMask::SerializeToFile(const SEBagValveMask& src, const std::string& filename)
{
  CDM_BIND::BagValveMaskData data;
  PBBagValveMask::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBBagValveMask::SerializeFromString(const std::string& src, SEBagValveMask& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::BagValveMaskData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBBagValveMask::Load(data, dst, subMgr);
  return true;
}
bool PBBagValveMask::SerializeFromFile(const std::string& filename, SEBagValveMask& dst, const SESubstanceManager& subMgr)
{
  CDM_BIND::BagValveMaskData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBBagValveMask::Load(data, dst, subMgr);
  return true;
}
