/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/cdm/PBSubstance.h"
#include "io/protobuf/cdm/PBPhysiology.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceAerosolization.h"
#include "substance/SESubstanceClearance.h"
#include "substance/SESubstanceCompound.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstancePharmacodynamics.h"
#include "substance/SESubstancePharmacokinetics.h"
#include "substance/SESubstancePhysicochemicals.h"
#include "substance/SESubstanceTissuePharmacokinetics.h"
#include "substance/SESubstanceManager.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarNegative1To1.h"
#include "properties/SEScalarInversePressure.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarMassPerAreaTime.h"
#include "properties/SEScalarMassPerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolumePerTimeMass.h"
#include "properties/SEScalarVolumePerTimePressure.h"

#include "properties/SEHistogramFractionVsLength.h"

#include "dirent.h"
#include "utils/FileUtils.h"
#include "utils/unitconversion/UnitConversionEngine.h"

#include "bind/cdm/Substance.pb.h"
#include <google/protobuf/text_format.h>

bool PBSubstance::LoadSubstanceDirectory(SESubstanceManager& mgr)
{
  bool succeed = true;
  mgr.Clear();
  std::stringstream ss;
  DIR *sdir;
  DIR *cdir;
  struct dirent *ent;

  std::string workingDirectory = GetCurrentWorkingDirectory();

#if defined(_WIN32)
  sdir = opendir("./substances/");
  cdir = opendir("./substances/compounds");
#else
  sdir = opendir(std::string(workingDirectory + std::string("/substances/")).c_str());
  cdir = opendir(std::string(workingDirectory + std::string("/substances/compounds/")).c_str());
#endif

  cdm::SubstanceData* subData;
  cdm::SubstanceCompoundData* compoundData;

  if (sdir != nullptr)
  {
    while ((ent = readdir(sdir)) != nullptr)
    {
      ss.str("");
      ss << workingDirectory << "/substances/" << ent->d_name;
      if (!IsDirectory(ent) && strlen(ent->d_name) > 2)
      {
        try
        {
          //Info("Reading substance file : " + ss.str());
          std::ifstream input(ss.str());
          std::string fmsg((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

          subData = new cdm::SubstanceData();
          if (!google::protobuf::TextFormat::ParseFromString(fmsg, subData))
          {
            succeed = false;
            mgr.GetLogger()->Error("Unable to read substance " + ss.str());
            delete subData;
            continue;
          }
        }
        catch (...)
        {
          mgr.GetLogger()->Info("I caught something in " + ss.str());
          delete subData;
          continue;
        }
        SESubstance* sub = new SESubstance(mgr.GetLogger());
        PBSubstance::Load(*subData, *sub);
        //PBSubstance::Unload(*sub); // This is here to debug serialization if I have to
        mgr.AddSubstance(*sub);
      }
    }
  }
  closedir(sdir);

  if (cdir != nullptr)
  {
    while ((ent = readdir(cdir)) != nullptr)
    {
      ss.str("");
      ss << workingDirectory << "/substances/compounds/" << ent->d_name;
      if (!IsDirectory(ent) && strlen(ent->d_name) > 2)
      {
        std::ifstream input(ss.str());
        std::string fmsg((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

        //Info("Reading compound file : ./substances/compounds/" + ent->d_name);
        compoundData = new cdm::SubstanceCompoundData();
        if (!google::protobuf::TextFormat::ParseFromString(fmsg, compoundData))
        {
          succeed = false;
          mgr.GetLogger()->Error("Unable to read compound " + ss.str());
          delete compoundData;
          continue;
        }
        SESubstanceCompound* compound = new SESubstanceCompound(mgr.GetLogger());
        PBSubstance::Load(*compoundData, *compound, mgr);
        mgr.AddCompound(*compound);
      }
    }
  }
  closedir(cdir);
  return succeed;
}

void PBSubstance::Load(const cdm::SubstanceData& src, SESubstance& dst)
{
  PBSubstance::Serialize(src, dst);

  if (dst.HasClearance() && dst.HasPK() && dst.GetPK().HasPhysicochemicals() &&
    dst.GetClearance().HasFractionUnboundInPlasma() &&
    !dst.GetClearance().GetFractionUnboundInPlasma().Equals(dst.GetPK().GetPhysicochemicals().GetFractionUnboundInPlasma()))
  {
    dst.Fatal("Multiple FractionUnboundInPlasma values specified, but not the same. These must match at this time.");
  }
}
void PBSubstance::Serialize(const cdm::SubstanceData& src, SESubstance& dst)
{
  dst.Clear();
  dst.SetName(src.name());
  dst.SetState((eSubstance_State)src.state());
  if (src.has_density())
    SEScalarMassPerVolume::Load(src.density(), dst.GetDensity());
  if (src.has_molarmass())
    SEScalarMassPerAmount::Load(src.molarmass(), dst.GetMolarMass());

  if (src.has_maximumdiffusionflux())
    SEScalarMassPerAreaTime::Load(src.maximumdiffusionflux(), dst.GetMaximumDiffusionFlux());
  if (src.has_michaeliscoefficient())
    SEScalar::Load(src.michaeliscoefficient(), dst.GetMichaelisCoefficient());

  if (src.has_aerosolization())
    PBSubstance::Load(src.aerosolization(), dst.GetAerosolization());
  if (src.has_bloodconcentration())
    SEScalarMassPerVolume::Load(src.bloodconcentration(), dst.GetBloodConcentration());
  if (src.has_massinbody())
    SEScalarMass::Load(src.massinbody(), dst.GetMassInBody());
  if (src.has_massinblood())
    SEScalarMass::Load(src.massinblood(), dst.GetMassInBlood());
  if (src.has_massintissue())
    SEScalarMass::Load(src.massintissue(), dst.GetMassInTissue());
  if (src.has_plasmaconcentration())
    SEScalarMassPerVolume::Load(src.plasmaconcentration(), dst.GetPlasmaConcentration());
  if (src.has_systemicmasscleared())
    SEScalarMass::Load(src.systemicmasscleared(), dst.GetSystemicMassCleared());
  if (src.has_tissueconcentration())
    SEScalarMassPerVolume::Load(src.tissueconcentration(), dst.GetTissueConcentration());

  if (src.has_alveolartransfer())
    SEScalarVolumePerTime::Load(src.alveolartransfer(), dst.GetAlveolarTransfer());
  if (src.has_diffusingcapacity())
    SEScalarVolumePerTimePressure::Load(src.diffusingcapacity(), dst.GetDiffusingCapacity());
  if (src.has_endtidalfraction())
    SEScalar0To1::Load(src.endtidalfraction(), dst.GetEndTidalFraction());
  if (src.has_endtidalpressure())
    SEScalarPressure::Load(src.endtidalpressure(), dst.GetEndTidalPressure());
  if (src.has_relativediffusioncoefficient())
    SEScalar::Load(src.relativediffusioncoefficient(), dst.GetRelativeDiffusionCoefficient());
  if (src.has_solubilitycoefficient())
    SEScalarInversePressure::Load(src.solubilitycoefficient(), dst.GetSolubilityCoefficient());

  if (src.has_clearance())
    PBSubstance::Load(src.clearance(), dst.GetClearance());
  if (src.has_pharmacokinetics())
    PBSubstance::Load(src.pharmacokinetics(), dst.GetPK());
  if (src.has_pharmacodynamics())
    PBSubstance::Load(src.pharmacodynamics(), dst.GetPD());
}

cdm::SubstanceData* PBSubstance::Unload(const SESubstance& src)
{
  cdm::SubstanceData* dst = new cdm::SubstanceData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstance& src, cdm::SubstanceData& dst)
{
  if (src.HasName())
    dst.set_name(src.m_Name);
  if (src.HasState())
    dst.set_state((cdm::eSubstance_State)src.m_State);
  if (src.HasDensity())
    dst.set_allocated_density(SEScalarMassPerVolume::Unload(*src.m_Density));
  if (src.HasMolarMass())
    dst.set_allocated_molarmass(SEScalarMassPerAmount::Unload(*src.m_MolarMass));

  if (src.HasMaximumDiffusionFlux())
    dst.set_allocated_maximumdiffusionflux(SEScalarMassPerAreaTime::Unload(*src.m_MaximumDiffusionFlux));
  if (src.HasMichaelisCoefficient())
    dst.set_allocated_michaeliscoefficient(SEScalar::Unload(*src.m_MichaelisCoefficient));

  if (src.HasAerosolization())
    dst.set_allocated_aerosolization(PBSubstance::Unload(*src.m_Aerosolization));
  if (src.HasBloodConcentration())
    dst.set_allocated_bloodconcentration(SEScalarMassPerVolume::Unload(*src.m_BloodConcentration));
  if (src.HasMassInBody())
    dst.set_allocated_massinbody(SEScalarMass::Unload(*src.m_MassInBody));
  if (src.HasMassInBlood())
    dst.set_allocated_massinblood(SEScalarMass::Unload(*src.m_MassInBlood));
  if (src.HasMassInTissue())
    dst.set_allocated_massintissue(SEScalarMass::Unload(*src.m_MassInTissue));
  if (src.HasPlasmaConcentration())
    dst.set_allocated_plasmaconcentration(SEScalarMassPerVolume::Unload(*src.m_PlasmaConcentration));
  if (src.HasSystemicMassCleared())
    dst.set_allocated_systemicmasscleared(SEScalarMass::Unload(*src.m_SystemicMassCleared));
  if (src.HasTissueConcentration())
    dst.set_allocated_tissueconcentration(SEScalarMassPerVolume::Unload(*src.m_TissueConcentration));

  if (src.HasAlveolarTransfer())
    dst.set_allocated_alveolartransfer(SEScalarVolumePerTime::Unload(*src.m_AlveolarTransfer));
  if (src.HasDiffusingCapacity())
    dst.set_allocated_diffusingcapacity(SEScalarVolumePerTimePressure::Unload(*src.m_DiffusingCapacity));
  if (src.HasEndTidalFraction())
    dst.set_allocated_endtidalfraction(SEScalar0To1::Unload(*src.m_EndTidalFraction));
  if (src.HasEndTidalPressure())
    dst.set_allocated_endtidalpressure(SEScalarPressure::Unload(*src.m_EndTidalPressure));
  if (src.HasRelativeDiffusionCoefficient())
    dst.set_allocated_relativediffusioncoefficient(SEScalar::Unload(*src.m_RelativeDiffusionCoefficient));
  if (src.HasSolubilityCoefficient())
    dst.set_allocated_solubilitycoefficient(SEScalarInversePressure::Unload(*src.m_SolubilityCoefficient));

  if (src.HasClearance())
    dst.set_allocated_clearance(PBSubstance::Unload(*src.m_Clearance));
  if (src.HasPK())
    dst.set_allocated_pharmacokinetics(PBSubstance::Unload(*src.m_PK));
  if (src.HasPD())
    dst.set_allocated_pharmacodynamics(PBSubstance::Unload(*src.m_PD));
}


void PBSubstance::Load(const cdm::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst)
{
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const cdm::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst)
{
  dst.Clear();
  if (src.has_bronchiolemodifier())
    SEScalarNegative1To1::Load(src.bronchiolemodifier(), dst.GetBronchioleModifier());
  if (src.has_inflammationcoefficient())
    SEScalar0To1::Load(src.inflammationcoefficient(), dst.GetInflammationCoefficient());
  if (src.has_particulatesizedistribution())
    SEHistogramFractionVsLength::Load(src.particulatesizedistribution(), dst.GetParticulateSizeDistribution());
}

cdm::SubstanceAerosolizationData* PBSubstance::Unload(const SESubstanceAerosolization& src)
{
  cdm::SubstanceAerosolizationData* dst = new cdm::SubstanceAerosolizationData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceAerosolization& src, cdm::SubstanceAerosolizationData& dst)
{
  if (src.HasBronchioleModifier())
    dst.set_allocated_bronchiolemodifier(SEScalarNegative1To1::Unload(*src.m_BronchioleModifier));
  if (src.HasInflammationCoefficient())
    dst.set_allocated_inflammationcoefficient(SEScalar0To1::Unload(*src.m_InflammationCoefficient));
  if (src.HasParticulateSizeDistribution())
    dst.set_allocated_particulatesizedistribution(SEHistogramFractionVsLength::Unload(*src.m_ParticulateSizeDistribution));
}


void PBSubstance::Load(const cdm::SubstanceClearanceData& src, SESubstanceClearance& dst)
{
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const cdm::SubstanceClearanceData& src, SESubstanceClearance& dst)
{
  dst.Clear();

  // Make sure dups match
  if (src.has_systemicclearance() && src.systemicclearance().has_renalclearance() && src.systemicclearance().renalclearance().has_scalarvolumepertimemass() &&
    src.has_renalclearance() && src.renalclearance().has_clearance() && src.renalclearance().clearance().has_scalarvolumepertimemass() &&
    src.systemicclearance().renalclearance().scalarvolumepertimemass().value() != src.renalclearance().clearance().scalarvolumepertimemass().value())
  {// This is assuming the same unit, so make sure that is not your problem - lazy developer
    dst.Fatal("Multiple Renal Clearances specified, but not the same. These must match at this time.");
  }
  if (src.has_systemicclearance() && src.systemicclearance().has_fractionunboundinplasma() &&
    src.has_renalclearance() && src.renalclearance().has_regulation() && src.renalclearance().regulation().has_fractionunboundinplasma() &&
    src.systemicclearance().fractionunboundinplasma().scalar0to1().value() != src.renalclearance().regulation().fractionunboundinplasma().scalar0to1().value())
  {
    dst.Fatal("Multiple FractionUnboundInPlasma values specified, but not the same. These must match at this time.");
  }

  if (src.has_systemicclearance())
  {
    dst.SetSystemic(true);
    SEScalar0To1::Load(src.systemicclearance().fractionexcretedinfeces(), dst.GetFractionExcretedInFeces());
    SEScalar0To1::Load(src.systemicclearance().fractionexcretedinurine(), dst.GetFractionExcretedInUrine());
    SEScalar0To1::Load(src.systemicclearance().fractionmetabolizedingut(), dst.GetFractionMetabolizedInGut());
    SEScalar0To1::Load(src.systemicclearance().fractionunboundinplasma(), dst.GetFractionUnboundInPlasma());
    SEScalarVolumePerTimeMass::Load(src.systemicclearance().intrinsicclearance(), dst.GetIntrinsicClearance());
    SEScalarVolumePerTimeMass::Load(src.systemicclearance().renalclearance(), dst.GetRenalClearance());
    SEScalarVolumePerTimeMass::Load(src.systemicclearance().systemicclearance(), dst.GetSystemicClearance());
  }

  if (src.has_renalclearance())
  {
    if (src.renalclearance().has_regulation())
    {
      dst.m_RenalDynamic = RenalDynamic::Regulation;
      dst.SetChargeInBlood((eCharge)src.renalclearance().regulation().chargeinblood());
      SEScalar0To1::Load(src.renalclearance().regulation().fractionunboundinplasma(), dst.GetFractionUnboundInPlasma());
      SEScalar::Load(src.renalclearance().regulation().reabsorptionratio(), dst.GetRenalReabsorptionRatio());
      SEScalarMassPerTime::Load(src.renalclearance().regulation().transportmaximum(), dst.GetRenalTransportMaximum());
    }
    else if (src.renalclearance().has_clearance())
    {
      dst.m_RenalDynamic = RenalDynamic::Clearance;
      SEScalarVolumePerTimeMass::Load(src.renalclearance().clearance(), dst.GetRenalClearance());
    }

    if (src.renalclearance().has_filtrationrate())
      SEScalarMassPerTime::Load(src.renalclearance().filtrationrate(), dst.GetRenalFiltrationRate());
    if (src.renalclearance().has_glomerularfilterability())
      SEScalar::Load(src.renalclearance().glomerularfilterability(), dst.GetGlomerularFilterability());
    if (src.renalclearance().has_reabsorptionrate())
      SEScalarMassPerTime::Load(src.renalclearance().reabsorptionrate(), dst.GetRenalReabsorptionRate());
    if (src.renalclearance().has_excretionrate())
      SEScalarMassPerTime::Load(src.renalclearance().excretionrate(), dst.GetRenalExcretionRate());
  }
}

cdm::SubstanceClearanceData* PBSubstance::Unload(const SESubstanceClearance& src)
{
  cdm::SubstanceClearanceData* dst = new cdm::SubstanceClearanceData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceClearance& src, cdm::SubstanceClearanceData& dst)
{
  if (src.HasSystemicClearance())
  {
    cdm::SubstanceSystemicClearanceData* sys = new cdm::SubstanceSystemicClearanceData();
    dst.set_allocated_systemicclearance(sys);

    if (src.HasFractionExcretedInFeces())
      sys->set_allocated_fractionexcretedinfeces(SEScalar0To1::Unload(*src.m_FractionExcretedInFeces));
    if (src.HasFractionExcretedInUrine())
      sys->set_allocated_fractionexcretedinurine(SEScalar0To1::Unload(*src.m_FractionExcretedInUrine));
    if (src.HasFractionMetabolizedInGut())
      sys->set_allocated_fractionmetabolizedingut(SEScalar0To1::Unload(*src.m_FractionMetabolizedInGut));
    if (src.HasFractionUnboundInPlasma())
      sys->set_allocated_fractionunboundinplasma(SEScalar0To1::Unload(*src.m_FractionUnboundInPlasma));
    if (src.HasRenalClearance())
      sys->set_allocated_renalclearance(SEScalarVolumePerTimeMass::Unload(*src.m_RenalClearance));
    if (src.HasIntrinsicClearance())
      sys->set_allocated_intrinsicclearance(SEScalarVolumePerTimeMass::Unload(*src.m_IntrinsicClearance));
    if (src.HasSystemicClearance())
      sys->set_allocated_systemicclearance(SEScalarVolumePerTimeMass::Unload(*src.m_SystemicClearance));
  }

  if (src.HasRenalDynamic())
  {
    cdm::SubstanceRenalClearanceData* rc = new cdm::SubstanceRenalClearanceData();
    dst.set_allocated_renalclearance(rc);

    if (src.m_RenalDynamic == RenalDynamic::Clearance && src.HasRenalClearance())
      rc->set_allocated_clearance(SEScalarVolumePerTimeMass::Unload(*src.m_RenalClearance));
    else if (src.m_RenalDynamic == RenalDynamic::Regulation)
    {
      cdm::SubstanceRenalRegulationData* rr = new cdm::SubstanceRenalRegulationData();
      rc->set_allocated_regulation(rr);

      if (src.HasChargeInBlood())
        rr->set_chargeinblood((cdm::eCharge)src.m_ChargeInBlood);
      if (src.HasFractionUnboundInPlasma())
        rr->set_allocated_fractionunboundinplasma(SEScalar0To1::Unload(*src.m_FractionUnboundInPlasma));
      if (src.HasRenalReabsorptionRatio())
        rr->set_allocated_reabsorptionratio(SEScalar::Unload(*src.m_RenalReabsorptionRatio));
      if (src.HasRenalTransportMaximum())
        rr->set_allocated_transportmaximum(SEScalarMassPerTime::Unload(*src.m_RenalTransportMaximum));
    }
    if (src.HasGlomerularFilterability())
      rc->set_allocated_glomerularfilterability(SEScalar::Unload(*src.m_GlomerularFilterability));
    if (src.HasRenalFiltrationRate())
      rc->set_allocated_filtrationrate(SEScalarMassPerTime::Unload(*src.m_RenalFiltrationRate));
    if (src.HasRenalReabsorptionRate())
      rc->set_allocated_reabsorptionrate(SEScalarMassPerTime::Unload(*src.m_RenalReabsorptionRate));
    if (src.HasRenalExcretionRate())
      rc->set_allocated_excretionrate(SEScalarMassPerTime::Unload(*src.m_RenalExcretionRate));
  }
}

void PBSubstance::Load(const cdm::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr)
{
  PBSubstance::Serialize(src, dst, subMgr);
}
void PBSubstance::Serialize(const cdm::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  dst.m_Name = src.name();

  std::string err;

  SESubstance* substance = nullptr;
  SESubstanceConcentration* cc;
  for (int i = 0; i < src.component_size(); i++)
  {
    const cdm::SubstanceConcentrationData& cData = src.component(i);
    substance = subMgr.GetSubstance(cData.name());
    if (substance == nullptr)
    {
      /// \fatal Could not load find substance compound component for specified substance
      dst.Fatal("Could not load find substance compound component : " + cData.name(), "SESubstanceCompound::Load");
      continue;
    }
    cc = new SESubstanceConcentration(*substance);
    PBSubstance::Load(cData, *cc);
    dst.m_Components.push_back(cc);
    dst.m_cComponents.push_back(cc);
  }
}

cdm::SubstanceCompoundData* PBSubstance::Unload(const SESubstanceCompound& src)
{
  cdm::SubstanceCompoundData* dst = new cdm::SubstanceCompoundData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceCompound& src, cdm::SubstanceCompoundData& dst)
{
  if (src.HasName())
    dst.set_name(src.m_Name);

  for (SESubstanceConcentration* c : src.m_Components)
    dst.mutable_component()->AddAllocated(PBSubstance::Unload(*c));
}

void PBSubstance::Load(const cdm::SubstanceConcentrationData& src, SESubstanceConcentration& dst)
{
  dst.Clear();
  if (src.has_concentration())
    SEScalarMassPerVolume::Load(src.concentration(), dst.GetConcentration());
}
void PBSubstance::Serialize(const cdm::SubstanceConcentrationData& src, SESubstanceConcentration& dst)
{

}

cdm::SubstanceConcentrationData* PBSubstance::Unload(const SESubstanceConcentration& src)
{
  cdm::SubstanceConcentrationData* dst = new cdm::SubstanceConcentrationData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}

void PBSubstance::Serialize(const SESubstanceConcentration& src, cdm::SubstanceConcentrationData& dst)
{
  dst.set_name(src.m_Substance.GetName());
  if (src.HasConcentration())
    dst.set_allocated_concentration(SEScalarMassPerVolume::Unload(*src.m_Concentration));
}


void PBSubstance::Load(const cdm::SubstanceFractionData& src, SESubstanceFraction& dst)
{
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const cdm::SubstanceFractionData& src, SESubstanceFraction& dst)
{
  dst.Clear();
  if (src.has_amount())
    SEScalar0To1::Load(src.amount(), dst.GetFractionAmount());
}

cdm::SubstanceFractionData* PBSubstance::Unload(const SESubstanceFraction& src)
{
  cdm::SubstanceFractionData* dst = new cdm::SubstanceFractionData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceFraction& src, cdm::SubstanceFractionData& dst)
{
  dst.set_name(src.m_Substance.GetName());
  if (src.HasFractionAmount())
    dst.set_allocated_amount(SEScalar0To1::Unload(*src.m_FractionAmount));
}


void PBSubstance::Load(const cdm::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst)
{
  PBSubstance::Serialize(src, dst);
  dst.CalculateDerived();
}
void PBSubstance::Serialize(const cdm::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst)
{
  dst.Clear();

  if (src.has_bronchodilation())
    SEScalarNegative1To1::Load(src.bronchodilation(), dst.GetBronchodilation());
  if (src.has_diastolicpressuremodifier())
    SEScalarNegative1To1::Load(src.diastolicpressuremodifier(), dst.GetDiastolicPressureModifier());
  if (src.has_ec50())
    SEScalarMassPerVolume::Load(src.ec50(), dst.GetEC50());
  if (src.has_emaxshapeparameter())
    SEScalar::Load(src.emaxshapeparameter(), dst.GetEMaxShapeParameter());
  if (src.has_heartratemodifier())
    SEScalarNegative1To1::Load(src.heartratemodifier(), dst.GetHeartRateModifier());
  if (src.has_neuromuscularblock())
    SEScalarNegative1To1::Load(src.neuromuscularblock(), dst.GetNeuromuscularBlock());
  if (src.has_pupillaryresponse())
    PBPhysiology::Load(src.pupillaryresponse(), dst.GetPupillaryResponse());
  if (src.has_respirationratemodifier())
    SEScalarNegative1To1::Load(src.respirationratemodifier(), dst.GetRespirationRateModifier());
  if (src.has_sedation())
    SEScalarNegative1To1::Load(src.sedation(), dst.GetSedation());
  if (src.has_systolicpressuremodifier())
    SEScalarNegative1To1::Load(src.systolicpressuremodifier(), dst.GetSystolicPressureModifier());
  if (src.has_tidalvolumemodifier())
    SEScalarNegative1To1::Load(src.tidalvolumemodifier(), dst.GetTidalVolumeModifier());
  if (src.has_tubularpermeabilitymodifier())
    SEScalarNegative1To1::Load(src.tubularpermeabilitymodifier(), dst.GetTubularPermeabilityModifier());
}

cdm::SubstancePharmacodynamicsData* PBSubstance::Unload(const SESubstancePharmacodynamics& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::SubstancePharmacodynamicsData* dst = new cdm::SubstancePharmacodynamicsData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstancePharmacodynamics& src, cdm::SubstancePharmacodynamicsData& dst)
{
  if (src.HasBronchodilation())
    dst.set_allocated_bronchodilation(SEScalarNegative1To1::Unload(*src.m_Bronchodilation));
  if (src.HasDiastolicPressureModifier())
    dst.set_allocated_diastolicpressuremodifier(SEScalarNegative1To1::Unload(*src.m_DiastolicPressureModifier));
  if (src.HasEC50())
    dst.set_allocated_ec50(SEScalarMassPerVolume::Unload(*src.m_EC50));
  if (src.HasEMaxShapeParameter())
    dst.set_allocated_emaxshapeparameter(SEScalar::Unload(*src.m_EMaxShapeParameter));
  if (src.HasHeartRateModifier())
    dst.set_allocated_heartratemodifier(SEScalarNegative1To1::Unload(*src.m_HeartRateModifier));
  if (src.HasNeuromuscularBlock())
    dst.set_allocated_neuromuscularblock(SEScalarNegative1To1::Unload(*src.m_NeuromuscularBlock));
  if (src.HasPupillaryResponse())
    dst.set_allocated_pupillaryresponse(PBPhysiology::Unload(*src.m_PupillaryResponse));
  if (src.HasRespirationRateModifier())
    dst.set_allocated_respirationratemodifier(SEScalarNegative1To1::Unload(*src.m_RespirationRateModifier));
  if (src.HasSedation())
    dst.set_allocated_sedation(SEScalarNegative1To1::Unload(*src.m_Sedation));
  if (src.HasSystolicPressureModifier())
    dst.set_allocated_systolicpressuremodifier(SEScalarNegative1To1::Unload(*src.m_SystolicPressureModifier));
  if (src.HasTidalVolumeModifier())
    dst.set_allocated_tidalvolumemodifier(SEScalarNegative1To1::Unload(*src.m_TidalVolumeModifier));
  if (src.HasTubularPermeabilityModifier())
    dst.set_allocated_tubularpermeabilitymodifier(SEScalarNegative1To1::Unload(*src.m_TubularPermeabilityModifier));
}


void PBSubstance::Load(const cdm::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst)
{
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const cdm::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst)
{
  dst.Clear();

  if (src.has_physicochemicals())
    PBSubstance::Load(src.physicochemicals(), dst.GetPhysicochemicals());

  SESubstanceTissuePharmacokinetics* fx;
  for (int i = 0; i < src.tissuekinetics_size(); i++)
  {
    const cdm::SubstanceTissuePharmacokineticsData& fxData = src.tissuekinetics(i);
    fx = new SESubstanceTissuePharmacokinetics(fxData.name(), dst.GetLogger());
    PBSubstance::Load(fxData, *fx);
    dst.m_TissueKinetics[fx->GetName()] = (fx);
  }
}

cdm::SubstancePharmacokineticsData* PBSubstance::Unload(const SESubstancePharmacokinetics& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::SubstancePharmacokineticsData* dst = new cdm::SubstancePharmacokineticsData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstancePharmacokinetics& src, cdm::SubstancePharmacokineticsData& dst)
{
  if (src.HasPhysicochemicals())
    dst.set_allocated_physicochemicals(PBSubstance::Unload(*src.m_Physicochemicals));

  for (auto itr : src.m_TissueKinetics)
    dst.mutable_tissuekinetics()->AddAllocated(PBSubstance::Unload(*itr.second));
}


void PBSubstance::Load(const cdm::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst)
{
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const cdm::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst)
{
  dst.Clear();
  if (src.has_aciddissociationconstant())
    SEScalar::Load(src.aciddissociationconstant(), dst.GetAcidDissociationConstant());
  dst.SetBindingProtein((eSubstance_BindingProtein)src.bindingprotein());
  if (src.has_bloodplasmaratio())
    SEScalar::Load(src.bloodplasmaratio(), dst.GetBloodPlasmaRatio());
  if (src.has_fractionunboundinplasma())
    SEScalar0To1::Load(src.fractionunboundinplasma(), dst.GetFractionUnboundInPlasma());
  dst.SetIonicState((eSubstance_IonicState)src.ionicstate());
  if (src.has_logp())
    SEScalar::Load(src.logp(), dst.GetLogP());
  if (src.has_oralabsorptionrateconstant())
    SEScalar::Load(src.oralabsorptionrateconstant(), dst.GetOralAbsorptionRateConstant());
}

cdm::SubstancePhysicochemicalsData* PBSubstance::Unload(const SESubstancePhysicochemicals& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::SubstancePhysicochemicalsData* dst = new cdm::SubstancePhysicochemicalsData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstancePhysicochemicals& src, cdm::SubstancePhysicochemicalsData& dst)
{
  if (src.HasAcidDissociationConstant())
    dst.set_allocated_aciddissociationconstant(SEScalar::Unload(*src.m_AcidDissociationConstant));
  if (src.HasBindingProtein())
    dst.set_bindingprotein((cdm::eSubstance_BindingProtein)src.m_BindingProtein);
  if (src.HasBloodPlasmaRatio())
    dst.set_allocated_bloodplasmaratio(SEScalar::Unload(*src.m_BloodPlasmaRatio));
  if (src.HasFractionUnboundInPlasma())
    dst.set_allocated_fractionunboundinplasma(SEScalar0To1::Unload(*src.m_FractionUnboundInPlasma));
  if (src.HasIonicState())
    dst.set_ionicstate((cdm::eSubstance_IonicState)src.m_IonicState);
  if (src.HasLogP())
    dst.set_allocated_logp(SEScalar::Unload(*src.m_LogP));
  if (src.HasOralAbsorptionRateConstant())
    dst.set_allocated_oralabsorptionrateconstant(SEScalar::Unload(*src.m_OralAbsorptionRateConstant));
}


void PBSubstance::Load(const cdm::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst)
{
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const cdm::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst)
{
  dst.Clear();
  if (src.has_partitioncoefficient())
    SEScalar::Load(src.partitioncoefficient(), dst.GetPartitionCoefficient());
}

cdm::SubstanceTissuePharmacokineticsData* PBSubstance::Unload(const SESubstanceTissuePharmacokinetics& src)
{
  cdm::SubstanceTissuePharmacokineticsData* dst = new cdm::SubstanceTissuePharmacokineticsData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceTissuePharmacokinetics& src, cdm::SubstanceTissuePharmacokineticsData& dst)
{
  dst.set_name(src.m_Name);
  if (src.HasPartitionCoefficient())
    dst.set_allocated_partitioncoefficient(SEScalar::Unload(*src.m_PartitionCoefficient));
}
