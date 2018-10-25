/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
   // Forward Declaire SE and Protobuf Physiology Types
CDM_BIND_DECL2(BloodChemistrySystem)
CDM_BIND_DECL2(CardiovascularSystem)
CDM_BIND_DECL2(EndocrineSystem)
CDM_BIND_DECL2(DrugSystem)
CDM_BIND_DECL2(EnergySystem)
CDM_BIND_DECL2(GastrointestinalSystem)
CDM_BIND_DECL2(HepaticSystem)
CDM_BIND_DECL2(NervousSystem)
CDM_BIND_DECL2(PupillaryResponse)
CDM_BIND_DECL2(RenalSystem)
CDM_BIND_DECL2(RespiratorySystem)
CDM_BIND_DECL2(TissueSystem)


class CDM_DECL PBPhysiology
{
public:

  // Blood Chemistry                                                                           
  static void Load(const cdm::BloodChemistrySystemData& src, SEBloodChemistrySystem& dst);     
  static cdm::BloodChemistrySystemData* Unload(const SEBloodChemistrySystem& src);             
  static void Serialize(const cdm::BloodChemistrySystemData& src, SEBloodChemistrySystem& dst);
  static void Serialize(const SEBloodChemistrySystem& src, cdm::BloodChemistrySystemData& dst);

  // Cardiovascular
  static void Load(const cdm::CardiovascularSystemData& src, SECardiovascularSystem& dst);
  static cdm::CardiovascularSystemData* Unload(const SECardiovascularSystem& src);
  static void Serialize(const cdm::CardiovascularSystemData& src, SECardiovascularSystem& dst);
  static void Serialize(const SECardiovascularSystem& src, cdm::CardiovascularSystemData& dst);

  // Drug
  static void Load(const cdm::DrugSystemData& src, SEDrugSystem& dst);
  static cdm::DrugSystemData* Unload(const SEDrugSystem& src);
  static void Serialize(const cdm::DrugSystemData& src, SEDrugSystem& dst);
  static void Serialize(const SEDrugSystem& src, cdm::DrugSystemData& dst);

  // Endocrine
  static void Load(const cdm::EndocrineSystemData& src, SEEndocrineSystem& dst);
  static cdm::EndocrineSystemData* Unload(const SEEndocrineSystem& src);
  static void Serialize(const cdm::EndocrineSystemData& src, SEEndocrineSystem& dst);
  static void Serialize(const SEEndocrineSystem& src, cdm::EndocrineSystemData& dst);

  // Energy
  static void Load(const cdm::EnergySystemData& src, SEEnergySystem& dst);
  static cdm::EnergySystemData* Unload(const SEEnergySystem& src);
  static void Serialize(const cdm::EnergySystemData& src, SEEnergySystem& dst);
  static void Serialize(const SEEnergySystem& src, cdm::EnergySystemData& dst);

  // Gastrointestinal
  static void Load(const cdm::GastrointestinalSystemData& src, SEGastrointestinalSystem& dst);
  static cdm::GastrointestinalSystemData* Unload(const SEGastrointestinalSystem& src);
  static void Serialize(const cdm::GastrointestinalSystemData& src, SEGastrointestinalSystem& dst);
  static void Serialize(const SEGastrointestinalSystem& src, cdm::GastrointestinalSystemData& dst);

  // Hepatic
  static void Load(const cdm::HepaticSystemData& src, SEHepaticSystem& dst);
  static cdm::HepaticSystemData* Unload(const SEHepaticSystem& src);
  static void Serialize(const cdm::HepaticSystemData& src, SEHepaticSystem& dst);
  static void Serialize(const SEHepaticSystem& src, cdm::HepaticSystemData& dst);

  /// Nervous
  static void Load(const cdm::NervousSystemData& src, SENervousSystem& dst);
  static cdm::NervousSystemData* Unload(const SENervousSystem& src);
  static void Serialize(const cdm::NervousSystemData& src, SENervousSystem& dst);
  static void Serialize(const SENervousSystem& src, cdm::NervousSystemData& dst);

  // Pupillary Response
  static void Load(const cdm::PupillaryResponseData& src, SEPupillaryResponse& dst);
  static cdm::PupillaryResponseData* Unload(const SEPupillaryResponse& src);
  static void Serialize(const cdm::PupillaryResponseData& src, SEPupillaryResponse& dst);
  static void Serialize(const SEPupillaryResponse& src, cdm::PupillaryResponseData& dst);

  // Renal
  static void Load(const cdm::RenalSystemData& src, SERenalSystem& dst);
  static cdm::RenalSystemData* Unload(const SERenalSystem& src);
  static void Serialize(const cdm::RenalSystemData& src, SERenalSystem& dst);
  static void Serialize(const SERenalSystem& src, cdm::RenalSystemData& dst);

  // Respiratory
  static void Load(const cdm::RespiratorySystemData& src, SERespiratorySystem& dst);
  static cdm::RespiratorySystemData* Unload(const SERespiratorySystem& src);
  static void Serialize(const cdm::RespiratorySystemData& src, SERespiratorySystem& dst);
  static void Serialize(const SERespiratorySystem& src, cdm::RespiratorySystemData& dst);

  // Tissue
  static void Load(const cdm::TissueSystemData& src, SETissueSystem& dst);
  static cdm::TissueSystemData* Unload(const SETissueSystem& src);
  static void Serialize(const cdm::TissueSystemData& src, SETissueSystem& dst);
  static void Serialize(const SETissueSystem& src, cdm::TissueSystemData& dst);

};