/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Inhaler)

class CDM_DECL PBInhaler
{
public:

  static void Load(const CDM_BIND::InhalerData& src, SEInhaler& dst);
  static CDM_BIND::InhalerData* Unload(const SEInhaler& src);
  static void Serialize(const CDM_BIND::InhalerData& src, SEInhaler& dst);
  static void Serialize(const SEInhaler& src, CDM_BIND::InhalerData& dst);

  static bool SerializeToString(const SEInhaler& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEInhaler& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEInhaler& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEInhaler& dst, SerializationFormat m);
};