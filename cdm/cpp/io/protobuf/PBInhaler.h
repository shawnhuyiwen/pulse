/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Inhaler)

class CDM_DECL PBInhaler
{
public:

  static void Load(const cdm::InhalerData& src, SEInhaler& dst);
  static cdm::InhalerData* Unload(const SEInhaler& src);
  static void Serialize(const cdm::InhalerData& src, SEInhaler& dst);
  static void Serialize(const SEInhaler& src, cdm::InhalerData& dst);

  static bool SerializeToString(const SEInhaler& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEInhaler& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEInhaler& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEInhaler& dst, SerializationMode m);
};