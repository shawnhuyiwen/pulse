/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
CDM_BIND_DECL2(Inhaler)

class CDM_DECL PBInhaler
{
public:

  static void Load(const CDM_BIND::InhalerData& src, SEInhaler& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::InhalerData* Unload(const SEInhaler& src);
  static void Serialize(const CDM_BIND::InhalerData& src, SEInhaler& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEInhaler& src, CDM_BIND::InhalerData& dst);

  static bool SerializeToString(const SEInhaler& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEInhaler& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEInhaler& dst, SerializationFormat m, const SESubstanceManager& subMgr);
  static bool SerializeFromFile(const std::string& filename, SEInhaler& dst, const SESubstanceManager& subMgr);
};