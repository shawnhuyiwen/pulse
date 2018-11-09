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

  static void Load(const cdm::SubstanceData& src, SESubstance& dst);
  static cdm::SubstanceData* Unload(const SESubstance& src);
  static void Serialize(const cdm::SubstanceData& src, SESubstance& dst);
  static void Serialize(const SESubstance& src, cdm::SubstanceData& dst);

  static bool SerializeToString(const SESubstance& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SESubstance& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SESubstance& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SESubstance& dst, SerializationFormat m);

  static void Load(const cdm::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst);
  static cdm::SubstanceAerosolizationData* Unload(const SESubstanceAerosolization& src);
  static void Serialize(const cdm::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst);
  static void Serialize(const SESubstanceAerosolization& src, cdm::SubstanceAerosolizationData& dst);

  static void Load(const cdm::SubstanceClearanceData& src, SESubstanceClearance& dst);
  static cdm::SubstanceClearanceData* Unload(const SESubstanceClearance& src);
  static void Serialize(const cdm::SubstanceClearanceData& src, SESubstanceClearance& dst);
  static void Serialize(const SESubstanceClearance& src, cdm::SubstanceClearanceData& dst);

  static void Load(const cdm::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr);
  static cdm::SubstanceCompoundData* Unload(const SESubstanceCompound& src);
  static void Serialize(const cdm::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SESubstanceCompound& src, cdm::SubstanceCompoundData& dst);

  static bool SerializeToString(const SESubstanceCompound& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SESubstanceCompound& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SESubstanceCompound& dst, const SESubstanceManager& subMgr, SerializationFormat m);

  static void Load(const cdm::SubstanceConcentrationData& src, SESubstanceConcentration& dst);
  static cdm::SubstanceConcentrationData* Unload(const SESubstanceConcentration& src);
  static void Serialize(const cdm::SubstanceConcentrationData& src, SESubstanceConcentration& dst);
  static void Serialize(const SESubstanceConcentration& src, cdm::SubstanceConcentrationData& dst);

  static void Load(const cdm::SubstanceFractionData& src, SESubstanceFraction& dst);
  static cdm::SubstanceFractionData* Unload(const SESubstanceFraction& src);
  static void Serialize(const cdm::SubstanceFractionData& src, SESubstanceFraction& dst);
  static void Serialize(const SESubstanceFraction& src, cdm::SubstanceFractionData& dst);

  static void Load(const cdm::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst);
  static cdm::SubstancePharmacodynamicsData* Unload(const SESubstancePharmacodynamics& src);
  static void Serialize(const cdm::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst);
  static void Serialize(const SESubstancePharmacodynamics& src, cdm::SubstancePharmacodynamicsData& dst);

  static void Load(const cdm::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst);
  static cdm::SubstancePharmacokineticsData* Unload(const SESubstancePharmacokinetics& src);
  static void Serialize(const cdm::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst);
  static void Serialize(const SESubstancePharmacokinetics& src, cdm::SubstancePharmacokineticsData& dst);

  static void Load(const cdm::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst);
  static cdm::SubstancePhysicochemicalsData* Unload(const SESubstancePhysicochemicals& src);
  static void Serialize(const cdm::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst);
  static void Serialize(const SESubstancePhysicochemicals& src, cdm::SubstancePhysicochemicalsData& dst);

  static void Load(const cdm::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst);
  static cdm::SubstanceTissuePharmacokineticsData* Unload(const SESubstanceTissuePharmacokinetics& src);
  static void Serialize(const cdm::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst);
  static void Serialize(const SESubstanceTissuePharmacokinetics& src, cdm::SubstanceTissuePharmacokineticsData& dst);
};
