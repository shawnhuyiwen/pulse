/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
CDM_BIND_DECL2(BagValveMask)

class CDM_DECL PBBagValveMask
{
public:
  static void Load(const CDM_BIND::BagValveMaskData& src, SEBagValveMask& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::BagValveMaskData* Unload(const SEBagValveMask& src);
  static void Serialize(const CDM_BIND::BagValveMaskData& src, SEBagValveMask& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEBagValveMask& src, CDM_BIND::BagValveMaskData& dst);

  static bool SerializeToString(const SEBagValveMask& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEBagValveMask& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEBagValveMask& dst, SerializationFormat m, const SESubstanceManager& subMgr);
  static bool SerializeFromFile(const std::string& filename, SEBagValveMask& dst, const SESubstanceManager& subMgr);
};
