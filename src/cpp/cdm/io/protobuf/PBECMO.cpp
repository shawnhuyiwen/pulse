/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/ECMO.pb.h"
POP_PROTO_WARNINGS
#include "cdm/io/protobuf/PBECMO.h"
#include "cdm/io/protobuf/PBSubstance.h"
#include "cdm/io/protobuf/PBProperties.h"
#include "cdm/io/protobuf/PBUtils.h"
#include "cdm/system/equipment/ecmo/SEECMO.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceCompound.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/FileUtils.h"

const std::string& eECMO_CannulationLocation_Name(eECMO_CannulationLocation m)
{
  return CDM_BIND::eECMOCannulationLocation_Name((CDM_BIND::eECMOCannulationLocation)m);
}


void PBECMO::Load(const CDM_BIND::ECMOData& src, SEECMO& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBECMO::Serialize(src, dst, subMgr);
  dst.StateChange();
}
void PBECMO::Serialize(const CDM_BIND::ECMOData& src, SEECMO& dst, const SESubstanceManager& subMgr)
{
  if (src.has_settings())
    PBECMO::Load(src.settings(), dst.GetSettings(), subMgr);
}
CDM_BIND::ECMOData* PBECMO::Unload(const SEECMO& src)
{
  CDM_BIND::ECMOData* dst = new CDM_BIND::ECMOData();
  PBECMO::Serialize(src, *dst);
  return dst;
}
void PBECMO::Serialize(const SEECMO& src, CDM_BIND::ECMOData& dst)
{
  if (src.HasSettings())
    dst.set_allocated_settings(PBECMO::Unload(*src.m_Settings));
}

void PBECMO::Load(const CDM_BIND::ECMOSettingsData& src, SEECMOSettings& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBECMO::Serialize(src, dst, subMgr);
}
void PBECMO::Serialize(const CDM_BIND::ECMOSettingsData& src, SEECMOSettings& dst, const SESubstanceManager& subMgr)
{
  if (src.inflowlocation() != CDM_BIND::eECMOCannulationLocation::NullCannulationLocation)
    dst.SetInflowLocation((eECMO_CannulationLocation)src.inflowlocation());
  if (src.outflowlocation() != CDM_BIND::eECMOCannulationLocation::NullCannulationLocation)
    dst.SetOutflowLocation((eECMO_CannulationLocation)src.outflowlocation());

  if (src.has_oxygenatorvolume())
    PBProperty::Load(src.oxygenatorvolume(), dst.GetOxygenatorVolume());
  if (src.has_transfusionflow())
    PBProperty::Load(src.transfusionflow(), dst.GetTransfusionFlow());

  if (!src.substancecompound().empty())
  {
    const SESubstanceCompound* c = subMgr.GetCompound(src.substancecompound());
    if (c != nullptr)
      dst.SetSubstanceCompound(*c);
    else
      dst.Error("Unknown compound requsted for ECMO " + src.substancecompound());
  }

  const SESubstance* substance;
  for (int i = 0; i < src.substanceconcentrations_size(); i++)
  {
    const CDM_BIND::SubstanceConcentrationData& cData = src.substanceconcentrations(i);
    substance = subMgr.GetSubstance(cData.name());
    if (substance == nullptr)
    {
      /// \fatal Could not load find substance compound component for specified substance
      dst.Fatal("Could not load find substance component : " + cData.name());
      continue;
    }
    SESubstanceConcentration& sc = dst.GetSubstanceConcentration(*substance);
    PBSubstance::Load(cData, sc);
  }
}

CDM_BIND::ECMOSettingsData* PBECMO::Unload(const SEECMOSettings& src)
{
  CDM_BIND::ECMOSettingsData* dst = new CDM_BIND::ECMOSettingsData();
  PBECMO::Serialize(src, *dst);
  return dst;
}
void PBECMO::Serialize(const SEECMOSettings& src, CDM_BIND::ECMOSettingsData& dst)
{
  dst.set_inflowlocation((CDM_BIND::eECMOCannulationLocation)src.GetInflowLocation());
  dst.set_outflowlocation((CDM_BIND::eECMOCannulationLocation)src.GetOutflowLocation());

  if (src.HasOxygenatorVolume())
    dst.set_allocated_oxygenatorvolume(PBProperty::Unload(*src.m_OxygenatorVolume));
  if (src.HasTransfusionFlow())
    dst.set_allocated_transfusionflow(PBProperty::Unload(*src.m_TransfusionFlow));

  if(src.HasSubstanceCompound())
    dst.set_substancecompound(src.GetSubstanceCompound()->GetName());

  for (SESubstanceConcentration* sc : src.m_SubstanceConcentrations)
  {
    CDM_BIND::SubstanceConcentrationData* scData =
      dst.add_substanceconcentrations();
    PBSubstance::Serialize(*sc, *scData);
  }
}

bool PBECMO::SerializeToString(const SEECMOSettings& src, std::string& output, eSerializationFormat m)
{
  CDM_BIND::ECMOSettingsData data;
  PBECMO::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBECMO::SerializeToFile(const SEECMOSettings& src, const std::string& filename)
{
  CDM_BIND::ECMOSettingsData data;
  PBECMO::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBECMO::SerializeFromString(const std::string& src, SEECMOSettings& dst, eSerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::ECMOSettingsData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBECMO::Load(data, dst, subMgr);
  return true;
}
bool PBECMO::SerializeFromFile(const std::string& filename, SEECMOSettings& dst, const SESubstanceManager& subMgr)
{
  CDM_BIND::ECMOSettingsData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBECMO::Load(data, dst, subMgr);
  return true;
}
