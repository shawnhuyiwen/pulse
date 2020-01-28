/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBInhaler.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "bind/Inhaler.pb.h"
#include "system/equipment/inhaler/SEInhaler.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "utils/FileUtils.h"

void PBInhaler::Load(const CDM_BIND::InhalerData& src, SEInhaler& dst)
{
  PBInhaler::Serialize(src, dst);
  dst.StateChange();
}
void PBInhaler::Serialize(const CDM_BIND::InhalerData& src, SEInhaler& dst)
{
  if (src.state() != CDM_BIND::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
  if (src.has_metereddose())
    PBProperty::Load(src.metereddose(), dst.GetMeteredDose());
  if (src.has_nozzleloss())
    PBProperty::Load(src.nozzleloss(), dst.GetNozzleLoss());
  if (src.has_spacervolume())
    PBProperty::Load(src.spacervolume(), dst.GetSpacerVolume());
  if (!src.substance().empty())
    dst.SetSubstance(dst.m_Substances.GetSubstance(src.substance()));
}

CDM_BIND::InhalerData* PBInhaler::Unload(const SEInhaler& src)
{
  CDM_BIND::InhalerData* dst = new CDM_BIND::InhalerData();
  PBInhaler::Serialize(src, *dst);
  return dst;
}
void PBInhaler::Serialize(const SEInhaler& src, CDM_BIND::InhalerData& dst)
{
  dst.set_state((CDM_BIND::eSwitch)src.m_State);
  if (src.HasMeteredDose())
    dst.set_allocated_metereddose(PBProperty::Unload(*src.m_MeteredDose));
  if (src.HasNozzleLoss())
    dst.set_allocated_nozzleloss(PBProperty::Unload(*src.m_NozzleLoss));
  if (src.HasSpacerVolume())
    dst.set_allocated_spacervolume(PBProperty::Unload(*src.m_SpacerVolume));
  if (src.HasSubstance())
    dst.set_substance(src.m_Substance->GetName());
}

bool PBInhaler::SerializeToString(const SEInhaler& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::InhalerData data;
  PBInhaler::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBInhaler::SerializeToFile(const SEInhaler& src, const std::string& filename, SerializationFormat m)
{
  CDM_BIND::InhalerData data;
  PBInhaler::Serialize(src, data);
  std::string content;
  PBInhaler::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}

bool PBInhaler::SerializeFromString(const std::string& src, SEInhaler& dst, SerializationFormat m)
{
  CDM_BIND::InhalerData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBInhaler::Load(data, dst);
  return true;
}
bool PBInhaler::SerializeFromFile(const std::string& filename, SEInhaler& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBInhaler::SerializeFromString(content, dst, m);
}
