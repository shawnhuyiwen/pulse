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

  static void Load(const PULSE_BIND::BloodChemistryData& src, BloodChemistry& dst);
  static PULSE_BIND::BloodChemistryData* Unload(const BloodChemistry& src);
  static void Serialize(const PULSE_BIND::BloodChemistryData& src, BloodChemistry& dst);
  static void Serialize(const BloodChemistry& src, PULSE_BIND::BloodChemistryData& dst);

  static void Load(const PULSE_BIND::CardiovascularData& src, Cardiovascular& dst);
  static PULSE_BIND::CardiovascularData* Unload(const Cardiovascular& src);
  static void Serialize(const PULSE_BIND::CardiovascularData& src, Cardiovascular& dst);
  static void Serialize(const Cardiovascular& src, PULSE_BIND::CardiovascularData& dst);

  static void Load(const PULSE_BIND::DrugData& src, Drugs& dst);
  static PULSE_BIND::DrugData* Unload(const Drugs& src);
  static void Serialize(const PULSE_BIND::DrugData& src, Drugs& dst);
  static void Serialize(const Drugs& src, PULSE_BIND::DrugData& dst);

  static void Load(const PULSE_BIND::EndocrineData& src, Endocrine& dst);
  static PULSE_BIND::EndocrineData* Unload(const Endocrine& src);
  static void Serialize(const PULSE_BIND::EndocrineData& src, Endocrine& dst);
  static void Serialize(const Endocrine& src, PULSE_BIND::EndocrineData& dst);

  static void Load(const PULSE_BIND::EnergyData& src, Energy& dst);
  static PULSE_BIND::EnergyData* Unload(const Energy& src);
  static void Serialize(const PULSE_BIND::EnergyData& src, Energy& dst);
  static void Serialize(const Energy& src, PULSE_BIND::EnergyData& dst);

  static void Load(const PULSE_BIND::GastrointestinalData& src, Gastrointestinal& dst);
  static PULSE_BIND::GastrointestinalData* Unload(const Gastrointestinal& src);
  static void Serialize(const PULSE_BIND::GastrointestinalData& src, Gastrointestinal& dst);
  static void Serialize(const Gastrointestinal& src, PULSE_BIND::GastrointestinalData& dst);

  static void Load(const PULSE_BIND::HepaticData& src, Hepatic& dst);
  static PULSE_BIND::HepaticData* Unload(const Hepatic& src);
  static void Serialize(const PULSE_BIND::HepaticData& src, Hepatic& dst);
  static void Serialize(const Hepatic& src, PULSE_BIND::HepaticData& dst);

  static void Load(const PULSE_BIND::NervousData& src, Nervous& dst);
  static PULSE_BIND::NervousData* Unload(const Nervous& src);
  static void Serialize(const PULSE_BIND::NervousData& src, Nervous& dst);
  static void Serialize(const Nervous& src, PULSE_BIND::NervousData& dst);

  static void Load(const PULSE_BIND::RenalData& src, Renal& dst);
  static PULSE_BIND::RenalData* Unload(const Renal& src);
  static void Serialize(const PULSE_BIND::RenalData& src, Renal& dst);
  static void Serialize(const Renal& src, PULSE_BIND::RenalData& dst);

  static void Load(const PULSE_BIND::RespiratoryData& src, Respiratory& dst);
  static PULSE_BIND::RespiratoryData* Unload(const Respiratory& src);
  static void Serialize(const PULSE_BIND::RespiratoryData& src, Respiratory& dst);
  static void Serialize(const Respiratory& src, PULSE_BIND::RespiratoryData& dst);

  static void Load(const PULSE_BIND::TissueData& src, Tissue& dst);
  static PULSE_BIND::TissueData* Unload(const Tissue& src);
  static void Serialize(const PULSE_BIND::TissueData& src, Tissue& dst);
  static void Serialize(const Tissue& src, PULSE_BIND::TissueData& dst);
};