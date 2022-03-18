/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
CDM_BIND_DECL2(ECMO)
CDM_BIND_DECL2(ECMOSettings)

class CDM_DECL PBECMO
{
public:

  static void Load(const CDM_BIND::ECMOData& src, SEECMO& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::ECMOData* Unload(const SEECMO& src);
  static void Serialize(const CDM_BIND::ECMOData& src, SEECMO& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEECMO& src, CDM_BIND::ECMOData& dst);

  static void Load(const CDM_BIND::ECMOSettingsData& src, SEECMOSettings& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::ECMOSettingsData* Unload(const SEECMOSettings& src);
  static void Serialize(const CDM_BIND::ECMOSettingsData& src, SEECMOSettings& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEECMOSettings& src, CDM_BIND::ECMOSettingsData& dst);

  static bool SerializeToString(const SEECMOSettings& src, std::string& output, eSerializationFormat m);
  static bool SerializeToFile(const SEECMOSettings& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEECMOSettings& dst, eSerializationFormat m, const SESubstanceManager& subMgr);
  static bool SerializeFromFile(const std::string& filename, SEECMOSettings& dst, const SESubstanceManager& subMgr);

};