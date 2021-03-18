/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Substance.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBSubstance.h"
#include "io/protobuf/PBPhysiology.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
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
#include "utils/FileUtils.h"


void PBSubstance::Load(const CDM_BIND::SubstanceData& src, SESubstance& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);

  if (dst.HasClearance() && dst.HasPK() && dst.GetPK().HasPhysicochemicals() &&
    dst.GetClearance().HasFractionUnboundInPlasma() &&
    !dst.GetClearance().GetFractionUnboundInPlasma().Equals(dst.GetPK().GetPhysicochemicals().GetFractionUnboundInPlasma()))
  {
    dst.Fatal("Multiple FractionUnboundInPlasma values specified, but not the same. These must match at this time.");
  }
}
void PBSubstance::Serialize(const CDM_BIND::SubstanceData& src, SESubstance& dst)
{
  if(dst.GetName() != src.name())
    dst.Error("Substance "+src.name()+" is loading into a substance named "+dst.GetName());
  dst.SetState((eSubstance_State)src.state());
  if (src.has_density())
    PBProperty::Load(src.density(), dst.GetDensity());
  if (src.has_molarmass())
    PBProperty::Load(src.molarmass(), dst.GetMolarMass());

  if (src.has_maximumdiffusionflux())
    PBProperty::Load(src.maximumdiffusionflux(), dst.GetMaximumDiffusionFlux());
  if (src.has_michaeliscoefficient())
    PBProperty::Load(src.michaeliscoefficient(), dst.GetMichaelisCoefficient());

  if (src.has_aerosolization())
    PBSubstance::Load(src.aerosolization(), dst.GetAerosolization());
  if (src.has_bloodconcentration())
    PBProperty::Load(src.bloodconcentration(), dst.GetBloodConcentration());
  if (src.has_massinbody())
    PBProperty::Load(src.massinbody(), dst.GetMassInBody());
  if (src.has_massinblood())
    PBProperty::Load(src.massinblood(), dst.GetMassInBlood());
  if (src.has_massintissue())
    PBProperty::Load(src.massintissue(), dst.GetMassInTissue());
  if (src.has_plasmaconcentration())
    PBProperty::Load(src.plasmaconcentration(), dst.GetPlasmaConcentration());
  if (src.has_systemicmasscleared())
    PBProperty::Load(src.systemicmasscleared(), dst.GetSystemicMassCleared());
  if (src.has_tissueconcentration())
    PBProperty::Load(src.tissueconcentration(), dst.GetTissueConcentration());

  if (src.has_alveolartransfer())
    PBProperty::Load(src.alveolartransfer(), dst.GetAlveolarTransfer());
  if (src.has_diffusingcapacity())
    PBProperty::Load(src.diffusingcapacity(), dst.GetDiffusingCapacity());
  if (src.has_endtidalfraction())
    PBProperty::Load(src.endtidalfraction(), dst.GetEndTidalFraction());
  if (src.has_endtidalpressure())
    PBProperty::Load(src.endtidalpressure(), dst.GetEndTidalPressure());
  if (src.has_relativediffusioncoefficient())
    PBProperty::Load(src.relativediffusioncoefficient(), dst.GetRelativeDiffusionCoefficient());
  if (src.has_solubilitycoefficient())
    PBProperty::Load(src.solubilitycoefficient(), dst.GetSolubilityCoefficient());

  if (src.has_clearance())
    PBSubstance::Load(src.clearance(), dst.GetClearance());
  if (src.has_pharmacokinetics())
    PBSubstance::Load(src.pharmacokinetics(), dst.GetPK());
  if (src.has_pharmacodynamics())
    PBSubstance::Load(src.pharmacodynamics(), dst.GetPD());
}

CDM_BIND::SubstanceData* PBSubstance::Unload(const SESubstance& src)
{
  CDM_BIND::SubstanceData* dst = new CDM_BIND::SubstanceData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstance& src, CDM_BIND::SubstanceData& dst)
{
  dst.set_name(src.m_Name);
  if (src.HasState())
    dst.set_state((CDM_BIND::SubstanceData::eState)src.m_State);
  if (src.HasDensity())
    dst.set_allocated_density(PBProperty::Unload(*src.m_Density));
  if (src.HasMolarMass())
    dst.set_allocated_molarmass(PBProperty::Unload(*src.m_MolarMass));

  if (src.HasMaximumDiffusionFlux())
    dst.set_allocated_maximumdiffusionflux(PBProperty::Unload(*src.m_MaximumDiffusionFlux));
  if (src.HasMichaelisCoefficient())
    dst.set_allocated_michaeliscoefficient(PBProperty::Unload(*src.m_MichaelisCoefficient));

  if (src.HasAerosolization())
    dst.set_allocated_aerosolization(PBSubstance::Unload(*src.m_Aerosolization));
  if (src.HasBloodConcentration())
    dst.set_allocated_bloodconcentration(PBProperty::Unload(*src.m_BloodConcentration));
  if (src.HasMassInBody())
    dst.set_allocated_massinbody(PBProperty::Unload(*src.m_MassInBody));
  if (src.HasMassInBlood())
    dst.set_allocated_massinblood(PBProperty::Unload(*src.m_MassInBlood));
  if (src.HasMassInTissue())
    dst.set_allocated_massintissue(PBProperty::Unload(*src.m_MassInTissue));
  if (src.HasPlasmaConcentration())
    dst.set_allocated_plasmaconcentration(PBProperty::Unload(*src.m_PlasmaConcentration));
  if (src.HasSystemicMassCleared())
    dst.set_allocated_systemicmasscleared(PBProperty::Unload(*src.m_SystemicMassCleared));
  if (src.HasTissueConcentration())
    dst.set_allocated_tissueconcentration(PBProperty::Unload(*src.m_TissueConcentration));

  if (src.HasAlveolarTransfer())
    dst.set_allocated_alveolartransfer(PBProperty::Unload(*src.m_AlveolarTransfer));
  if (src.HasDiffusingCapacity())
    dst.set_allocated_diffusingcapacity(PBProperty::Unload(*src.m_DiffusingCapacity));
  if (src.HasEndTidalFraction())
    dst.set_allocated_endtidalfraction(PBProperty::Unload(*src.m_EndTidalFraction));
  if (src.HasEndTidalPressure())
    dst.set_allocated_endtidalpressure(PBProperty::Unload(*src.m_EndTidalPressure));
  if (src.HasRelativeDiffusionCoefficient())
    dst.set_allocated_relativediffusioncoefficient(PBProperty::Unload(*src.m_RelativeDiffusionCoefficient));
  if (src.HasSolubilityCoefficient())
    dst.set_allocated_solubilitycoefficient(PBProperty::Unload(*src.m_SolubilityCoefficient));

  if (src.HasClearance())
    dst.set_allocated_clearance(PBSubstance::Unload(*src.m_Clearance));
  if (src.HasPK())
    dst.set_allocated_pharmacokinetics(PBSubstance::Unload(*src.m_PK));
  if (src.HasPD())
    dst.set_allocated_pharmacodynamics(PBSubstance::Unload(*src.m_PD));
}


void PBSubstance::Load(const CDM_BIND::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const CDM_BIND::SubstanceAerosolizationData& src, SESubstanceAerosolization& dst)
{
  if (src.has_bronchiolemodifier())
    PBProperty::Load(src.bronchiolemodifier(), dst.GetBronchioleModifier());
  if (src.has_inflammationcoefficient())
    PBProperty::Load(src.inflammationcoefficient(), dst.GetInflammationCoefficient());
  if (src.has_particulatesizedistribution())
    PBProperty::Load(src.particulatesizedistribution(), dst.GetParticulateSizeDistribution());
}

CDM_BIND::SubstanceAerosolizationData* PBSubstance::Unload(const SESubstanceAerosolization& src)
{
  CDM_BIND::SubstanceAerosolizationData* dst = new CDM_BIND::SubstanceAerosolizationData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceAerosolization& src, CDM_BIND::SubstanceAerosolizationData& dst)
{
  if (src.HasBronchioleModifier())
    dst.set_allocated_bronchiolemodifier(PBProperty::Unload(*src.m_BronchioleModifier));
  if (src.HasInflammationCoefficient())
    dst.set_allocated_inflammationcoefficient(PBProperty::Unload(*src.m_InflammationCoefficient));
  if (src.HasParticulateSizeDistribution())
    dst.set_allocated_particulatesizedistribution(PBProperty::Unload(*src.m_ParticulateSizeDistribution));
}


void PBSubstance::Load(const CDM_BIND::SubstanceClearanceData& src, SESubstanceClearance& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const CDM_BIND::SubstanceClearanceData& src, SESubstanceClearance& dst)
{
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
    PBProperty::Load(src.systemicclearance().fractionexcretedinfeces(), dst.GetFractionExcretedInFeces());
    PBProperty::Load(src.systemicclearance().fractionexcretedinurine(), dst.GetFractionExcretedInUrine());
    PBProperty::Load(src.systemicclearance().fractionmetabolizedingut(), dst.GetFractionMetabolizedInGut());
    PBProperty::Load(src.systemicclearance().fractionunboundinplasma(), dst.GetFractionUnboundInPlasma());
    PBProperty::Load(src.systemicclearance().intrinsicclearance(), dst.GetIntrinsicClearance());
    PBProperty::Load(src.systemicclearance().renalclearance(), dst.GetRenalClearance());
    PBProperty::Load(src.systemicclearance().systemicclearance(), dst.GetSystemicClearance());
  }

  if (src.has_renalclearance())
  {
    if (src.renalclearance().has_clearance())
    {
      dst.m_RenalDynamic = RenalDynamic::Clearance;
      PBProperty::Load(src.renalclearance().clearance(), dst.GetRenalClearance());
    }
    // This if order matters!!!
    if (src.renalclearance().has_regulation())
    {// If we have a regulation object, we are regulating, the clearance is bookkeeping only
      dst.m_RenalDynamic = RenalDynamic::Regulation;
      dst.SetChargeInBlood((eCharge)src.renalclearance().regulation().chargeinblood());
      PBProperty::Load(src.renalclearance().regulation().fractionunboundinplasma(), dst.GetFractionUnboundInPlasma());
      PBProperty::Load(src.renalclearance().regulation().reabsorptionratio(), dst.GetRenalReabsorptionRatio());
      PBProperty::Load(src.renalclearance().regulation().transportmaximum(), dst.GetRenalTransportMaximum());
    }

    if (src.renalclearance().has_filtrationrate())
      PBProperty::Load(src.renalclearance().filtrationrate(), dst.GetRenalFiltrationRate());
    if (src.renalclearance().has_glomerularfilterability())
      PBProperty::Load(src.renalclearance().glomerularfilterability(), dst.GetGlomerularFilterability());
    if (src.renalclearance().has_reabsorptionrate())
      PBProperty::Load(src.renalclearance().reabsorptionrate(), dst.GetRenalReabsorptionRate());
    if (src.renalclearance().has_excretionrate())
      PBProperty::Load(src.renalclearance().excretionrate(), dst.GetRenalExcretionRate());
  }
}

CDM_BIND::SubstanceClearanceData* PBSubstance::Unload(const SESubstanceClearance& src)
{
  CDM_BIND::SubstanceClearanceData* dst = new CDM_BIND::SubstanceClearanceData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceClearance& src, CDM_BIND::SubstanceClearanceData& dst)
{
  if (src.HasSystemicClearance())
  {
    CDM_BIND::SubstanceSystemicClearanceData* sys = new CDM_BIND::SubstanceSystemicClearanceData();
    dst.set_allocated_systemicclearance(sys);

    if (src.HasFractionExcretedInFeces())
      sys->set_allocated_fractionexcretedinfeces(PBProperty::Unload(*src.m_FractionExcretedInFeces));
    if (src.HasFractionExcretedInUrine())
      sys->set_allocated_fractionexcretedinurine(PBProperty::Unload(*src.m_FractionExcretedInUrine));
    if (src.HasFractionMetabolizedInGut())
      sys->set_allocated_fractionmetabolizedingut(PBProperty::Unload(*src.m_FractionMetabolizedInGut));
    if (src.HasFractionUnboundInPlasma())
      sys->set_allocated_fractionunboundinplasma(PBProperty::Unload(*src.m_FractionUnboundInPlasma));
    if (src.HasRenalClearance())
      sys->set_allocated_renalclearance(PBProperty::Unload(*src.m_RenalClearance));
    if (src.HasIntrinsicClearance())
      sys->set_allocated_intrinsicclearance(PBProperty::Unload(*src.m_IntrinsicClearance));
    if (src.HasSystemicClearance())
      sys->set_allocated_systemicclearance(PBProperty::Unload(*src.m_SystemicClearance));
  }

  if (src.HasRenalDynamic())
  {
    CDM_BIND::SubstanceRenalClearanceData* rc = new CDM_BIND::SubstanceRenalClearanceData();
    dst.set_allocated_renalclearance(rc);

    if (src.HasRenalClearance())
      rc->set_allocated_clearance(PBProperty::Unload(*src.m_RenalClearance));
    if (src.m_RenalDynamic == RenalDynamic::Regulation)
    {
      CDM_BIND::SubstanceRenalRegulationData* rr = new CDM_BIND::SubstanceRenalRegulationData();
      rc->set_allocated_regulation(rr);

      if (src.HasChargeInBlood())
        rr->set_chargeinblood((CDM_BIND::eCharge)src.m_ChargeInBlood);
      if (src.HasFractionUnboundInPlasma())
        rr->set_allocated_fractionunboundinplasma(PBProperty::Unload(*src.m_FractionUnboundInPlasma));
      if (src.HasRenalReabsorptionRatio())
        rr->set_allocated_reabsorptionratio(PBProperty::Unload(*src.m_RenalReabsorptionRatio));
      if (src.HasRenalTransportMaximum())
        rr->set_allocated_transportmaximum(PBProperty::Unload(*src.m_RenalTransportMaximum));
    }
    if (src.HasGlomerularFilterability())
      rc->set_allocated_glomerularfilterability(PBProperty::Unload(*src.m_GlomerularFilterability));
    if (src.HasRenalFiltrationRate())
      rc->set_allocated_filtrationrate(PBProperty::Unload(*src.m_RenalFiltrationRate));
    if (src.HasRenalReabsorptionRate())
      rc->set_allocated_reabsorptionrate(PBProperty::Unload(*src.m_RenalReabsorptionRate));
    if (src.HasRenalExcretionRate())
      rc->set_allocated_excretionrate(PBProperty::Unload(*src.m_RenalExcretionRate));
  }
}

void PBSubstance::Load(const CDM_BIND::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst, subMgr);
}
void PBSubstance::Serialize(const CDM_BIND::SubstanceCompoundData& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr)
{
  if (dst.GetName() != src.name())
    dst.Error("Substance Compound " + src.name() + " is loading into a compound named " + dst.GetName());

  std::string err;

  const SESubstance* substance = nullptr;
  for (int i = 0; i < src.component_size(); i++)
  {
    const CDM_BIND::SubstanceConcentrationData& cData = src.component(i);
    substance = subMgr.GetSubstance(cData.name());
    if (substance == nullptr)
    {
      /// \fatal Could not load find substance compound component for specified substance
      dst.Fatal("Could not load find substance compound component : " + cData.name(), "SESubstanceCompound::Load");
      continue;
    }
    SESubstanceConcentration& sc = dst.GetComponent(*substance);
    PBSubstance::Load(cData, sc);
  }
}

CDM_BIND::SubstanceCompoundData* PBSubstance::Unload(const SESubstanceCompound& src)
{
  CDM_BIND::SubstanceCompoundData* dst = new CDM_BIND::SubstanceCompoundData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceCompound& src, CDM_BIND::SubstanceCompoundData& dst)
{
  dst.set_name(src.m_Name);
  for (SESubstanceConcentration* c : src.m_Components)
    dst.mutable_component()->AddAllocated(PBSubstance::Unload(*c));
}

void PBSubstance::Load(const CDM_BIND::SubstanceConcentrationData& src, SESubstanceConcentration& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const CDM_BIND::SubstanceConcentrationData& src, SESubstanceConcentration& dst)
{

  if (src.has_concentration())
    PBProperty::Load(src.concentration(), dst.GetConcentration());
}

CDM_BIND::SubstanceConcentrationData* PBSubstance::Unload(const SESubstanceConcentration& src)
{
  CDM_BIND::SubstanceConcentrationData* dst = new CDM_BIND::SubstanceConcentrationData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}

void PBSubstance::Serialize(const SESubstanceConcentration& src, CDM_BIND::SubstanceConcentrationData& dst)
{
  dst.set_name(src.m_Substance.GetName());
  if (src.HasConcentration())
    dst.set_allocated_concentration(PBProperty::Unload(*src.m_Concentration));
}

void PBSubstance::Load(const CDM_BIND::SubstanceFractionData& src, SESubstanceFraction& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const CDM_BIND::SubstanceFractionData& src, SESubstanceFraction& dst)
{
  if (src.has_amount())
    PBProperty::Load(src.amount(), dst.GetFractionAmount());
}

CDM_BIND::SubstanceFractionData* PBSubstance::Unload(const SESubstanceFraction& src)
{
  CDM_BIND::SubstanceFractionData* dst = new CDM_BIND::SubstanceFractionData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceFraction& src, CDM_BIND::SubstanceFractionData& dst)
{
  dst.set_name(src.m_Substance.GetName());
  if (src.HasFractionAmount())
    dst.set_allocated_amount(PBProperty::Unload(*src.m_FractionAmount));
}


void PBSubstance::Load(const CDM_BIND::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const CDM_BIND::SubstancePharmacodynamicsData& src, SESubstancePharmacodynamics& dst)
{
  if (src.has_bronchodilation())
    PBProperty::Load(src.bronchodilation(), dst.GetBronchodilation());
  if (src.has_diastolicpressuremodifier())
    PBProperty::Load(src.diastolicpressuremodifier(), dst.GetDiastolicPressureModifier());
  if (src.has_ec50())
    PBProperty::Load(src.ec50(), dst.GetEC50());
  if (src.has_emaxshapeparameter())
    PBProperty::Load(src.emaxshapeparameter(), dst.GetEMaxShapeParameter());
  if (src.has_heartratemodifier())
    PBProperty::Load(src.heartratemodifier(), dst.GetHeartRateModifier());
  if (src.has_neuromuscularblock())
    PBProperty::Load(src.neuromuscularblock(), dst.GetNeuromuscularBlock());
  if (src.has_pupillaryresponse())
    PBPhysiology::Load(src.pupillaryresponse(), dst.GetPupillaryResponse());
  if (src.has_respirationratemodifier())
    PBProperty::Load(src.respirationratemodifier(), dst.GetRespirationRateModifier());
  if (src.has_sedation())
    PBProperty::Load(src.sedation(), dst.GetSedation());
  if (src.has_systolicpressuremodifier())
    PBProperty::Load(src.systolicpressuremodifier(), dst.GetSystolicPressureModifier());
  if (src.has_tidalvolumemodifier())
    PBProperty::Load(src.tidalvolumemodifier(), dst.GetTidalVolumeModifier());
  if (src.has_tubularpermeabilitymodifier())
    PBProperty::Load(src.tubularpermeabilitymodifier(), dst.GetTubularPermeabilityModifier());
  dst.CalculateDerived();
}

CDM_BIND::SubstancePharmacodynamicsData* PBSubstance::Unload(const SESubstancePharmacodynamics& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::SubstancePharmacodynamicsData* dst = new CDM_BIND::SubstancePharmacodynamicsData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstancePharmacodynamics& src, CDM_BIND::SubstancePharmacodynamicsData& dst)
{
  if (src.HasBronchodilation())
    dst.set_allocated_bronchodilation(PBProperty::Unload(*src.m_Bronchodilation));
  if (src.HasDiastolicPressureModifier())
    dst.set_allocated_diastolicpressuremodifier(PBProperty::Unload(*src.m_DiastolicPressureModifier));
  if (src.HasEC50())
    dst.set_allocated_ec50(PBProperty::Unload(*src.m_EC50));
  if (src.HasEMaxShapeParameter())
    dst.set_allocated_emaxshapeparameter(PBProperty::Unload(*src.m_EMaxShapeParameter));
  if (src.HasHeartRateModifier())
    dst.set_allocated_heartratemodifier(PBProperty::Unload(*src.m_HeartRateModifier));
  if (src.HasNeuromuscularBlock())
    dst.set_allocated_neuromuscularblock(PBProperty::Unload(*src.m_NeuromuscularBlock));
  if (src.HasPupillaryResponse())
    dst.set_allocated_pupillaryresponse(PBPhysiology::Unload(*src.m_PupillaryResponse));
  if (src.HasRespirationRateModifier())
    dst.set_allocated_respirationratemodifier(PBProperty::Unload(*src.m_RespirationRateModifier));
  if (src.HasSedation())
    dst.set_allocated_sedation(PBProperty::Unload(*src.m_Sedation));
  if (src.HasSystolicPressureModifier())
    dst.set_allocated_systolicpressuremodifier(PBProperty::Unload(*src.m_SystolicPressureModifier));
  if (src.HasTidalVolumeModifier())
    dst.set_allocated_tidalvolumemodifier(PBProperty::Unload(*src.m_TidalVolumeModifier));
  if (src.HasTubularPermeabilityModifier())
    dst.set_allocated_tubularpermeabilitymodifier(PBProperty::Unload(*src.m_TubularPermeabilityModifier));
}


void PBSubstance::Load(const CDM_BIND::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const CDM_BIND::SubstancePharmacokineticsData& src, SESubstancePharmacokinetics& dst)
{
  if (src.has_physicochemicals())
    PBSubstance::Load(src.physicochemicals(), dst.GetPhysicochemicals());

  SESubstanceTissuePharmacokinetics* fx;
  for (int i = 0; i < src.tissuekinetics_size(); i++)
  {
    const CDM_BIND::SubstanceTissuePharmacokineticsData& fxData = src.tissuekinetics(i);
    fx = new SESubstanceTissuePharmacokinetics(fxData.name(), dst.GetLogger());
    PBSubstance::Load(fxData, *fx);
    dst.m_TissueKinetics[fx->GetName()] = (fx);
  }
}

CDM_BIND::SubstancePharmacokineticsData* PBSubstance::Unload(const SESubstancePharmacokinetics& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::SubstancePharmacokineticsData* dst = new CDM_BIND::SubstancePharmacokineticsData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstancePharmacokinetics& src, CDM_BIND::SubstancePharmacokineticsData& dst)
{
  if (src.HasPhysicochemicals())
    dst.set_allocated_physicochemicals(PBSubstance::Unload(*src.m_Physicochemicals));

  for (auto itr : src.m_TissueKinetics)
    dst.mutable_tissuekinetics()->AddAllocated(PBSubstance::Unload(*itr.second));
}


void PBSubstance::Load(const CDM_BIND::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const CDM_BIND::SubstancePhysicochemicalsData& src, SESubstancePhysicochemicals& dst)
{
  if (src.has_aciddissociationconstant())
    PBProperty::Load(src.aciddissociationconstant(), dst.GetAcidDissociationConstant());
  dst.SetBindingProtein((eSubstance_BindingProtein)src.bindingprotein());
  if (src.has_bloodplasmaratio())
    PBProperty::Load(src.bloodplasmaratio(), dst.GetBloodPlasmaRatio());
  if (src.has_fractionunboundinplasma())
    PBProperty::Load(src.fractionunboundinplasma(), dst.GetFractionUnboundInPlasma());
  dst.SetIonicState((eSubstance_IonicState)src.ionicstate());
  if (src.has_logp())
    PBProperty::Load(src.logp(), dst.GetLogP());
  if (src.has_oralabsorptionrateconstant())
    PBProperty::Load(src.oralabsorptionrateconstant(), dst.GetOralAbsorptionRateConstant());
}

CDM_BIND::SubstancePhysicochemicalsData* PBSubstance::Unload(const SESubstancePhysicochemicals& src)
{
  if (!src.IsValid())
    return nullptr;
  CDM_BIND::SubstancePhysicochemicalsData* dst = new CDM_BIND::SubstancePhysicochemicalsData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstancePhysicochemicals& src, CDM_BIND::SubstancePhysicochemicalsData& dst)
{
  if (src.HasAcidDissociationConstant())
    dst.set_allocated_aciddissociationconstant(PBProperty::Unload(*src.m_AcidDissociationConstant));
  if (src.HasBindingProtein())
    dst.set_bindingprotein((CDM_BIND::SubstancePhysicochemicalsData::eBindingProtein)src.m_BindingProtein);
  if (src.HasBloodPlasmaRatio())
    dst.set_allocated_bloodplasmaratio(PBProperty::Unload(*src.m_BloodPlasmaRatio));
  if (src.HasFractionUnboundInPlasma())
    dst.set_allocated_fractionunboundinplasma(PBProperty::Unload(*src.m_FractionUnboundInPlasma));
  if (src.HasIonicState())
    dst.set_ionicstate((CDM_BIND::SubstancePhysicochemicalsData::eIonicState)src.m_IonicState);
  if (src.HasLogP())
    dst.set_allocated_logp(PBProperty::Unload(*src.m_LogP));
  if (src.HasOralAbsorptionRateConstant())
    dst.set_allocated_oralabsorptionrateconstant(PBProperty::Unload(*src.m_OralAbsorptionRateConstant));
}


void PBSubstance::Load(const CDM_BIND::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst)
{
  dst.Clear();
  PBSubstance::Serialize(src, dst);
}
void PBSubstance::Serialize(const CDM_BIND::SubstanceTissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst)
{
  if (src.has_partitioncoefficient())
    PBProperty::Load(src.partitioncoefficient(), dst.GetPartitionCoefficient());
}

CDM_BIND::SubstanceTissuePharmacokineticsData* PBSubstance::Unload(const SESubstanceTissuePharmacokinetics& src)
{
  CDM_BIND::SubstanceTissuePharmacokineticsData* dst = new CDM_BIND::SubstanceTissuePharmacokineticsData();
  PBSubstance::Serialize(src, *dst);
  return dst;
}
void PBSubstance::Serialize(const SESubstanceTissuePharmacokinetics& src, CDM_BIND::SubstanceTissuePharmacokineticsData& dst)
{
  dst.set_name(src.m_Name);
  if (src.HasPartitionCoefficient())
    dst.set_allocated_partitioncoefficient(PBProperty::Unload(*src.m_PartitionCoefficient));
}

bool PBSubstance::SerializeToString(const SESubstance& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::SubstanceData data;
  PBSubstance::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBSubstance::SerializeToFile(const SESubstance& src, const std::string& filename)
{
  CDM_BIND::SubstanceData data;
  PBSubstance::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBSubstance::SerializeFromString(const std::string& src, SESubstance& dst, SerializationFormat m)
{
  CDM_BIND::SubstanceData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBSubstance::Load(data, dst);
  return true;
}
bool PBSubstance::SerializeFromFile(const std::string& filename, SESubstance& dst)
{
  CDM_BIND::SubstanceData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBSubstance::Load(data, dst);
  return true;
}

bool PBSubstance::SerializeToString(const SESubstanceCompound& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::SubstanceCompoundData data;
  PBSubstance::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBSubstance::SerializeToFile(const SESubstanceCompound& src, const std::string& filename)
{
  CDM_BIND::SubstanceCompoundData data;
  PBSubstance::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBSubstance::SerializeFromString(const std::string& src, SESubstanceCompound& dst, const SESubstanceManager& subMgr, SerializationFormat m)
{
  CDM_BIND::SubstanceCompoundData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBSubstance::Load(data, dst, subMgr);
  return true;
}
bool PBSubstance::SerializeFromFile(const std::string& filename, SESubstanceCompound& dst, const SESubstanceManager& subMgr)
{
  CDM_BIND::SubstanceCompoundData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBSubstance::Load(data, dst, subMgr);
  return true;
}