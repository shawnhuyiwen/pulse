/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(MechanicalVentilator)

class CDM_DECL PBMechanicalVentilator
{
public:

  static void Load(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst);
  static CDM_BIND::MechanicalVentilatorData* Unload(const SEMechanicalVentilator& src);
  static void Serialize(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst);
  static void Serialize(const SEMechanicalVentilator& src, CDM_BIND::MechanicalVentilatorData& dst);

  static bool SerializeToString(const SEMechanicalVentilator& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEMechanicalVentilator& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEMechanicalVentilator& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEMechanicalVentilator& dst, SerializationFormat m);

};