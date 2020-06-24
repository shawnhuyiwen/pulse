/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
// Forward Declaire SE and Protobuf Substance Types
CDM_BIND_DECL2(Substance)
CDM_BIND_DECL2(SubstanceAerosolization)
CDM_BIND_DECL2(SubstanceClearance)
CDM_BIND_DECL2(SubstanceCompound)
CDM_BIND_DECL2(SubstanceConcentration)
CDM_BIND_DECL2(SubstanceFraction)
CDM_BIND_DECL2(SubstancePharmacodynamics)
CDM_BIND_DECL2(SubstancePharmacokinetics)
CDM_BIND_DECL2(SubstancePhysicochemicals)
CDM_BIND_DECL2(SubstanceTissuePharmacokinetics)

class CDM_DECL PBSubstance
{
public:

  static void Load(const CDM_BIND::SubstanceData& src, SESubstance& dst);
  static CDM_BIND::SubstanceData* Unload(const SESubstance& src);
  static void Serialize(const CDM_BIND::SubstanceData& src, SESubstance& dst);
  static void Serialize(const SESubstance& src, CDM_BIND::SubstanceData& dst);

  static bool SerializeToString(const SESubstance& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SESubstance& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SESubstance& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SESubstance& dst);

  static void Load(const CDM_BIND::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst);
  static CDM_BIND::SubstanceAerosolizationData* Unload(const SESubstanceAerosolization& src);
  static void Serialize(const CDM_BIND::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst);
  static void Serialize(const SESubstanceAerosolization& src, CDM_BIND::SubstanceAerosolizationData& dst);

  static void Load(const CDM_BIND::SubstanceClearanceData& src, SESubstanceClearance& dst);
  static CDM_BIND::SubstanceClearanceData* Unload(const SESubstanceClearance& src);
  static void Serialize(const CDM_BIND::SubstanceClearanceData& src, SESubstanceClearance& dst);
  static void Serialize(const SESubstanceClearance& src, CDM_BIND::SubstanceClearanceData& dst);

  static void Load(const CDM_BIND::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::SubstanceCompoundData* Unload(const SESubstanceCompound& src);
  static void Serialize(const CDM_BIND::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SESubstanceCompound& src, CDM_BIND::SubstanceCompoundData& dst);

  static bool SerializeToString(const SESubstanceCompound& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SESubstanceCompound& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SESubstanceCompound& dst, const SESubstanceManager& subMgr);

  static void Load(const CDM_BIND::SubstanceConcentrationData& src, SESubstanceConcentration& dst);
  static CDM_BIND::SubstanceConcentrationData* Unload(const SESubstanceConcentration& src);
  static void Serialize(const CDM_BIND::SubstanceConcentrationData& src, SESubstanceConcentration& dst);
  static void Serialize(const SESubstanceConcentration& src, CDM_BIND::SubstanceConcentrationData& dst);

  static void Load(const CDM_BIND::SubstanceFractionData& src, SESubstanceFraction& dst);
  static CDM_BIND::SubstanceFractionData* Unload(const SESubstanceFraction& src);
  static void Serialize(const CDM_BIND::SubstanceFractionData& src, SESubstanceFraction& dst);
  static void Serialize(const SESubstanceFraction& src, CDM_BIND::SubstanceFractionData& dst);

  static void Load(const CDM_BIND::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst);
  static CDM_BIND::SubstancePharmacodynamicsData* Unload(const SESubstancePharmacodynamics& src);
  static void Serialize(const CDM_BIND::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst);
  static void Serialize(const SESubstancePharmacodynamics& src, CDM_BIND::SubstancePharmacodynamicsData& dst);

  static void Load(const CDM_BIND::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst);
  static CDM_BIND::SubstancePharmacokineticsData* Unload(const SESubstancePharmacokinetics& src);
  static void Serialize(const CDM_BIND::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst);
  static void Serialize(const SESubstancePharmacokinetics& src, CDM_BIND::SubstancePharmacokineticsData& dst);

  static void Load(const CDM_BIND::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst);
  static CDM_BIND::SubstancePhysicochemicalsData* Unload(const SESubstancePhysicochemicals& src);
  static void Serialize(const CDM_BIND::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst);
  static void Serialize(const SESubstancePhysicochemicals& src, CDM_BIND::SubstancePhysicochemicalsData& dst);

  static void Load(const CDM_BIND::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst);
  static CDM_BIND::SubstanceTissuePharmacokineticsData* Unload(const SESubstanceTissuePharmacokinetics& src);
  static void Serialize(const CDM_BIND::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst);
  static void Serialize(const SESubstanceTissuePharmacokinetics& src, CDM_BIND::SubstanceTissuePharmacokineticsData& dst);
};
