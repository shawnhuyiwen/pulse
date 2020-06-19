/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Patient)

class CDM_DECL PBPatient
{
public:

  static void Load(const CDM_BIND::PatientData& src, SEPatient& dst);
  static CDM_BIND::PatientData* Unload(const SEPatient& src);
  static void Serialize(const CDM_BIND::PatientData& src, SEPatient& dst);
  static void Serialize(const SEPatient& src, CDM_BIND::PatientData& dst);
  static void Copy(const SEPatient& src, SEPatient& dst);

  static bool SerializeToString(const SEPatient& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEPatient& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEPatient& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEPatient& dst);
};
