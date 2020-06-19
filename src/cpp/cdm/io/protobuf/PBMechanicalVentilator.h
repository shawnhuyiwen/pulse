/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
CDM_BIND_DECL2(MechanicalVentilator)

class CDM_DECL PBMechanicalVentilator
{
public:

  static void Load(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::MechanicalVentilatorData* Unload(const SEMechanicalVentilator& src);
  static void Serialize(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEMechanicalVentilator& src, CDM_BIND::MechanicalVentilatorData& dst);

  static bool SerializeToString(const SEMechanicalVentilator& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEMechanicalVentilator& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEMechanicalVentilator& dst, SerializationFormat m, const SESubstanceManager& subMgr);
  static bool SerializeFromFile(const std::string& filename, SEMechanicalVentilator& dst, const SESubstanceManager& subMgr);

};