/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/ElectroCardioGram.pb.h"
POP_PROTO_WARNINGS
#include "cdm/io/protobuf/PBElectroCardioGram.h"
#include "cdm/io/protobuf/PBProperties.h"
#include "cdm/io/protobuf/PBUtils.h"
#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"
#include "cdm/utils/FileUtils.h"


void PBElectroCardioGram::Load(const CDM_BIND::ElectroCardioGramData& src, SEElectroCardioGram& dst)
{
  dst.Clear();
  PBElectroCardioGram::Serialize(src, dst);
}
void PBElectroCardioGram::Serialize(const CDM_BIND::ElectroCardioGramData& src, SEElectroCardioGram& dst)
{
  if (src.has_lead1electricpotential())
    PBProperty::Load(src.lead1electricpotential(), dst.GetLead1ElectricPotential());
  if (src.has_lead2electricpotential())
    PBProperty::Load(src.lead2electricpotential(), dst.GetLead2ElectricPotential());
  if (src.has_lead3electricpotential())
    PBProperty::Load(src.lead3electricpotential(), dst.GetLead3ElectricPotential());
  if (src.has_lead4electricpotential())
    PBProperty::Load(src.lead4electricpotential(), dst.GetLead4ElectricPotential());
  if (src.has_lead5electricpotential())
    PBProperty::Load(src.lead5electricpotential(), dst.GetLead5ElectricPotential());
  if (src.has_lead6electricpotential())
    PBProperty::Load(src.lead6electricpotential(), dst.GetLead6ElectricPotential());
  if (src.has_lead7electricpotential())
    PBProperty::Load(src.lead7electricpotential(), dst.GetLead7ElectricPotential());
  if (src.has_lead8electricpotential())
    PBProperty::Load(src.lead8electricpotential(), dst.GetLead8ElectricPotential());
  if (src.has_lead9electricpotential())
    PBProperty::Load(src.lead9electricpotential(), dst.GetLead9ElectricPotential());
  if (src.has_lead10electricpotential())
    PBProperty::Load(src.lead10electricpotential(), dst.GetLead10ElectricPotential());
  if (src.has_lead11electricpotential())
    PBProperty::Load(src.lead11electricpotential(), dst.GetLead11ElectricPotential());
  if (src.has_lead12electricpotential())
    PBProperty::Load(src.lead12electricpotential(), dst.GetLead12ElectricPotential());


  for (int i = 0; i < src.waveforms_size(); i++)
  {
    SEElectroCardioGramWaveform* waveform = new SEElectroCardioGramWaveform(dst.GetLogger());
    PBElectroCardioGram::Load(src.waveforms()[i], *waveform);
    dst.GetWaveforms().push_back(waveform);
  }
}
CDM_BIND::ElectroCardioGramData* PBElectroCardioGram::Unload(const SEElectroCardioGram& src)
{
  CDM_BIND::ElectroCardioGramData* dst = new CDM_BIND::ElectroCardioGramData();
  PBElectroCardioGram::Serialize(src, *dst);
  return dst;
}
void PBElectroCardioGram::Serialize(const SEElectroCardioGram& src, CDM_BIND::ElectroCardioGramData& dst)
{
  if (src.HasLead1ElectricPotential())
    dst.set_allocated_lead1electricpotential(PBProperty::Unload(*src.m_Lead1ElectricPotential));
  if (src.HasLead2ElectricPotential())
    dst.set_allocated_lead2electricpotential(PBProperty::Unload(*src.m_Lead2ElectricPotential));
  if (src.HasLead3ElectricPotential())
    dst.set_allocated_lead3electricpotential(PBProperty::Unload(*src.m_Lead3ElectricPotential));
  if (src.HasLead4ElectricPotential())
    dst.set_allocated_lead4electricpotential(PBProperty::Unload(*src.m_Lead4ElectricPotential));
  if (src.HasLead5ElectricPotential())
    dst.set_allocated_lead5electricpotential(PBProperty::Unload(*src.m_Lead5ElectricPotential));
  if (src.HasLead6ElectricPotential())
    dst.set_allocated_lead6electricpotential(PBProperty::Unload(*src.m_Lead6ElectricPotential));
  if (src.HasLead7ElectricPotential())
    dst.set_allocated_lead7electricpotential(PBProperty::Unload(*src.m_Lead7ElectricPotential));
  if (src.HasLead8ElectricPotential())
    dst.set_allocated_lead8electricpotential(PBProperty::Unload(*src.m_Lead8ElectricPotential));
  if (src.HasLead9ElectricPotential())
    dst.set_allocated_lead9electricpotential(PBProperty::Unload(*src.m_Lead9ElectricPotential));
  if (src.HasLead10ElectricPotential())
    dst.set_allocated_lead10electricpotential(PBProperty::Unload(*src.m_Lead10ElectricPotential));
  if (src.HasLead11ElectricPotential())
    dst.set_allocated_lead11electricpotential(PBProperty::Unload(*src.m_Lead11ElectricPotential));
  if (src.HasLead12ElectricPotential())
    dst.set_allocated_lead12electricpotential(PBProperty::Unload(*src.m_Lead12ElectricPotential));

  for (SEElectroCardioGramWaveform* waveform : src.m_Waveforms)
    dst.mutable_waveforms()->AddAllocated(PBElectroCardioGram::Unload(*waveform));
}

void PBElectroCardioGram::Copy(const SEElectroCardioGram& src, SEElectroCardioGram& dst)
{
  dst.Clear();
  CDM_BIND::ElectroCardioGramData data;
  PBElectroCardioGram::Serialize(src, data);
  PBElectroCardioGram::Serialize(data, dst);
}

bool PBElectroCardioGram::SerializeToString(const SEElectroCardioGram& src, std::string& output, eSerializationFormat m)
{
  CDM_BIND::ElectroCardioGramData data;
  PBElectroCardioGram::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, nullptr);
}
bool PBElectroCardioGram::SerializeToFile(const SEElectroCardioGram& src, const std::string& filename)
{
  CDM_BIND::ElectroCardioGramData data;
  PBElectroCardioGram::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, nullptr);
}
bool PBElectroCardioGram::SerializeFromString(const std::string& src, SEElectroCardioGram& dst, eSerializationFormat m)
{
  CDM_BIND::ElectroCardioGramData data;
  if (!PBUtils::SerializeFromString(src, data, m, nullptr))
    return false;
  PBElectroCardioGram::Load(data, dst);
  return true;
}
bool PBElectroCardioGram::SerializeFromFile(const std::string& filename, SEElectroCardioGram& dst)
{
  CDM_BIND::ElectroCardioGramData data;
  if (!PBUtils::SerializeFromFile(filename, data, nullptr))
    return false;
  PBElectroCardioGram::Load(data, dst);
  return true;
}

void PBElectroCardioGram::Load(const CDM_BIND::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst)
{
  dst.Clear();
  PBElectroCardioGram::Serialize(src, dst);
}
void PBElectroCardioGram::Serialize(const CDM_BIND::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst)
{
  dst.m_Type = (eElectroCardioGram_WaveformType)src.type();
  dst.m_LeadNumber = (eElectroCardioGram_WaveformLead)src.lead();
  if (src.has_originaldata())
    PBProperty::Load(src.originaldata(), dst.GetOriginalData());
  if (src.has_activecycle())
    PBProperty::Load(src.activecycle(), dst.GetActiveCycle());
  dst.SetActiveIndex(src.activecycleindex());
  dst.SetContinuous(src.continuous());
}
CDM_BIND::ElectroCardioGramWaveformData* PBElectroCardioGram::Unload(const SEElectroCardioGramWaveform& src)
{
  CDM_BIND::ElectroCardioGramWaveformData* dst = new CDM_BIND::ElectroCardioGramWaveformData();
  PBElectroCardioGram::Serialize(src, *dst);
  return dst;
}
void PBElectroCardioGram::Serialize(const SEElectroCardioGramWaveform& src, CDM_BIND::ElectroCardioGramWaveformData& dst)
{
  dst.set_type((CDM_BIND::ElectroCardioGramWaveformData::eWaveformType)src.m_Type);
  dst.set_lead((CDM_BIND::ElectroCardioGramWaveformData::eWaveformLead)src.m_LeadNumber);
  if (src.HasOriginalData())
    dst.set_allocated_originaldata(PBProperty::Unload(*src.m_OriginalData));
  if (src.HasActiveCycle())
    dst.set_allocated_activecycle(PBProperty::Unload(*src.m_ActiveCycle));
  dst.set_activecycleindex(src.m_ActiveIndex);
  dst.set_continuous(src.IsContinuous());
}
