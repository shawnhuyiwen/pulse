/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
// Forward Declaire SE and Protobuf Substance Types
CDM_DECL_BIND(Substance)
CDM_DECL_BIND(SubstanceAerosolization)
CDM_DECL_BIND(SubstanceClearance)
CDM_DECL_BIND(SubstanceCompound)
CDM_DECL_BIND(SubstanceConcentration)
CDM_DECL_BIND(SubstanceFraction)
CDM_DECL_BIND(SubstancePharmacodynamics)
CDM_DECL_BIND(SubstancePharmacokinetics)
CDM_DECL_BIND(SubstancePhysicochemicals)
CDM_DECL_BIND(SubstanceTissuePharmacokinetics)

class CDM_DECL PBSubstance
{
public:
  static bool LoadSubstanceDirectory(SESubstanceManager& mgr);

public:
  static void Load(const cdm::SubstanceData& src, SESubstance& dst);
  static cdm::SubstanceData* Unload(const SESubstance& src);
protected:
  static void Serialize(const cdm::SubstanceData& src, SESubstance& dst);
  static void Serialize(const SESubstance& src, cdm::SubstanceData& dst);

public:
  static void Load(const cdm::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst);
  static cdm::SubstanceAerosolizationData* Unload(const SESubstanceAerosolization& src);
protected:
  static void Serialize(const cdm::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst);
  static void Serialize(const SESubstanceAerosolization& src, cdm::SubstanceAerosolizationData& dst);

public:
  static void Load(const cdm::SubstanceClearanceData& src, SESubstanceClearance& dst);
  static cdm::SubstanceClearanceData* Unload(const SESubstanceClearance& src);
protected:
  static void Serialize(const cdm::SubstanceClearanceData& src, SESubstanceClearance& dst);
  static void Serialize(const SESubstanceClearance& src, cdm::SubstanceClearanceData& dst);

public:
  static void Load(const cdm::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr);
  static cdm::SubstanceCompoundData* Unload(const SESubstanceCompound& src);
protected:
  static void Serialize(const cdm::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SESubstanceCompound& src, cdm::SubstanceCompoundData& dst);

public:
  static void Load(const cdm::SubstanceConcentrationData& src, SESubstanceConcentration& dst);
  static cdm::SubstanceConcentrationData* Unload(const SESubstanceConcentration& src);
protected:
  static void Serialize(const cdm::SubstanceConcentrationData& src, SESubstanceConcentration& dst);
  static void Serialize(const SESubstanceConcentration& src, cdm::SubstanceConcentrationData& dst);

public:
  static void Load(const cdm::SubstanceFractionData& src, SESubstanceFraction& dst);
  static cdm::SubstanceFractionData* Unload(const SESubstanceFraction& src);
protected:
  static void Serialize(const cdm::SubstanceFractionData& src, SESubstanceFraction& dst);
  static void Serialize(const SESubstanceFraction& src, cdm::SubstanceFractionData& dst);

public:
  static void Load(const cdm::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst);
  static cdm::SubstancePharmacodynamicsData* Unload(const SESubstancePharmacodynamics& src);
protected:
  static void Serialize(const cdm::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst);
  static void Serialize(const SESubstancePharmacodynamics& src, cdm::SubstancePharmacodynamicsData& dst);

public:
  static void Load(const cdm::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst);
  static cdm::SubstancePharmacokineticsData* Unload(const SESubstancePharmacokinetics& src);
protected:
  static void Serialize(const cdm::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst);
  static void Serialize(const SESubstancePharmacokinetics& src, cdm::SubstancePharmacokineticsData& dst);

public:
  static void Load(const cdm::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst);
  static cdm::SubstancePhysicochemicalsData* Unload(const SESubstancePhysicochemicals& src);
protected:
  static void Serialize(const cdm::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst);
  static void Serialize(const SESubstancePhysicochemicals& src, cdm::SubstancePhysicochemicalsData& dst);

public:
  static void Load(const cdm::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst);
  static cdm::SubstanceTissuePharmacokineticsData* Unload(const SESubstanceTissuePharmacokinetics& src);
protected:
  static void Serialize(const cdm::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst);
  static void Serialize(const SESubstanceTissuePharmacokinetics& src, cdm::SubstanceTissuePharmacokineticsData& dst);

};
