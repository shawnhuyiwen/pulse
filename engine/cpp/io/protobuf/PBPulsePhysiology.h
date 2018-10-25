/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PULSE_BIND_DECL(BloodChemistry)
PULSE_BIND_DECL(Cardiovascular)
PULSE_BIND_DECL(Drug)
PULSE_BIND_DECL(Endocrine)
PULSE_BIND_DECL(Energy)
PULSE_BIND_DECL(Gastrointestinal)
PULSE_BIND_DECL(Hepatic)
PULSE_BIND_DECL(Nervous)
PULSE_BIND_DECL(Renal)
PULSE_BIND_DECL(Respiratory)
PULSE_BIND_DECL(Tissue)


class PULSE_DECL PBPulsePhysiology
{
public:

  static void Load(const pulse::proto::BloodChemistryData& src, BloodChemistry& dst);
  static pulse::proto::BloodChemistryData* Unload(const BloodChemistry& src);
  static void Serialize(const pulse::proto::BloodChemistryData& src, BloodChemistry& dst);
  static void Serialize(const BloodChemistry& src, pulse::proto::BloodChemistryData& dst);

  static void Load(const pulse::proto::CardiovascularData& src, Cardiovascular& dst);
  static pulse::proto::CardiovascularData* Unload(const Cardiovascular& src);
  static void Serialize(const pulse::proto::CardiovascularData& src, Cardiovascular& dst);
  static void Serialize(const Cardiovascular& src, pulse::proto::CardiovascularData& dst);

  static void Load(const pulse::proto::DrugData& src, Drugs& dst);
  static pulse::proto::DrugData* Unload(const Drugs& src);
  static void Serialize(const pulse::proto::DrugData& src, Drugs& dst);
  static void Serialize(const Drugs& src, pulse::proto::DrugData& dst);

  static void Load(const pulse::proto::EndocrineData& src, Endocrine& dst);
  static pulse::proto::EndocrineData* Unload(const Endocrine& src);
  static void Serialize(const pulse::proto::EndocrineData& src, Endocrine& dst);
  static void Serialize(const Endocrine& src, pulse::proto::EndocrineData& dst);

  static void Load(const pulse::proto::EnergyData& src, Energy& dst);
  static pulse::proto::EnergyData* Unload(const Energy& src);
  static void Serialize(const pulse::proto::EnergyData& src, Energy& dst);
  static void Serialize(const Energy& src, pulse::proto::EnergyData& dst);

  static void Load(const pulse::proto::GastrointestinalData& src, Gastrointestinal& dst);
  static pulse::proto::GastrointestinalData* Unload(const Gastrointestinal& src);
  static void Serialize(const pulse::proto::GastrointestinalData& src, Gastrointestinal& dst);
  static void Serialize(const Gastrointestinal& src, pulse::proto::GastrointestinalData& dst);

  static void Load(const pulse::proto::HepaticData& src, Hepatic& dst);
  static pulse::proto::HepaticData* Unload(const Hepatic& src);
  static void Serialize(const pulse::proto::HepaticData& src, Hepatic& dst);
  static void Serialize(const Hepatic& src, pulse::proto::HepaticData& dst);

  static void Load(const pulse::proto::NervousData& src, Nervous& dst);
  static pulse::proto::NervousData* Unload(const Nervous& src);
  static void Serialize(const pulse::proto::NervousData& src, Nervous& dst);
  static void Serialize(const Nervous& src, pulse::proto::NervousData& dst);

  static void Load(const pulse::proto::RenalData& src, Renal& dst);
  static pulse::proto::RenalData* Unload(const Renal& src);
  static void Serialize(const pulse::proto::RenalData& src, Renal& dst);
  static void Serialize(const Renal& src, pulse::proto::RenalData& dst);

  static void Load(const pulse::proto::RespiratoryData& src, Respiratory& dst);
  static pulse::proto::RespiratoryData* Unload(const Respiratory& src);
  static void Serialize(const pulse::proto::RespiratoryData& src, Respiratory& dst);
  static void Serialize(const Respiratory& src, pulse::proto::RespiratoryData& dst);

  static void Load(const pulse::proto::TissueData& src, Tissue& dst);
  static pulse::proto::TissueData* Unload(const Tissue& src);
  static void Serialize(const pulse::proto::TissueData& src, Tissue& dst);
  static void Serialize(const Tissue& src, pulse::proto::TissueData& dst);
};