/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Patient)

class CDM_DECL PBPatient
{
public:

  static void Load(const cdm::PatientData& src, SEPatient& dst);
  static cdm::PatientData* Unload(const SEPatient& src);
  static void Serialize(const cdm::PatientData& src, SEPatient& dst);
  static void Serialize(const SEPatient& src, cdm::PatientData& dst);
  static void Copy(const SEPatient& src, SEPatient& dst);

  static bool SerializeToString(const SEPatient& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEPatient& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEPatient& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEPatient& dst, SerializationMode m);
};
