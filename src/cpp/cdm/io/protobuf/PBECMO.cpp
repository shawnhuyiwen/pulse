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
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/FileUtils.h"

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
}

CDM_BIND::ECMOSettingsData* PBECMO::Unload(const SEECMOSettings& src)
{
  CDM_BIND::ECMOSettingsData* dst = new CDM_BIND::ECMOSettingsData();
  PBECMO::Serialize(src, *dst);
  return dst;
}
void PBECMO::Serialize(const SEECMOSettings& src, CDM_BIND::ECMOSettingsData& dst)
{
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
