/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/MechanicalVentilator.pb.h"
POP_PROTO_WARNINGS
#include "cdm/io/protobuf/PBMechanicalVentilator.h"
#include "cdm/io/protobuf/PBSubstance.h"
#include "cdm/io/protobuf/PBProperties.h"
#include "cdm/io/protobuf/PBUtils.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/FileUtils.h"

void PBMechanicalVentilator::Load(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBMechanicalVentilator::Serialize(src, dst, subMgr);
  dst.StateChange();
}
void PBMechanicalVentilator::Serialize(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst, const SESubstanceManager& subMgr)
{

  if (src.has_settings())
    PBMechanicalVentilator::Load(src.settings(), dst.GetSettings(), subMgr);
}
CDM_BIND::MechanicalVentilatorData* PBMechanicalVentilator::Unload(const SEMechanicalVentilator& src)
{
  CDM_BIND::MechanicalVentilatorData* dst = new CDM_BIND::MechanicalVentilatorData();
  PBMechanicalVentilator::Serialize(src, *dst);
  return dst;
}
void PBMechanicalVentilator::Serialize(const SEMechanicalVentilator& src, CDM_BIND::MechanicalVentilatorData& dst)
{

  if (src.HasSettings())
    dst.set_allocated_settings(PBMechanicalVentilator::Unload(*src.m_Settings));
}

void PBMechanicalVentilator::Load(const CDM_BIND::MechanicalVentilatorSettingsData& src, SEMechanicalVentilatorSettings& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBMechanicalVentilator::Serialize(src, dst, subMgr);
}
void PBMechanicalVentilator::Serialize(const CDM_BIND::MechanicalVentilatorSettingsData& src, SEMechanicalVentilatorSettings& dst, const SESubstanceManager& subMgr)
{
  dst.m_Connection = (eMechanicalVentilator_Connection)src.connection();

  if (src.has_positiveendexpiredpressure())
    PBProperty::Load(src.positiveendexpiredpressure(), dst.GetPositiveEndExpiredPressure());
  if (src.has_functionalresidualcapacity())
    PBProperty::Load(src.functionalresidualcapacity(), dst.GetFunctionalResidualCapacity());

  if (src.has_expirationcycleflow())
    PBProperty::Load(src.expirationcycleflow(), dst.GetExpirationCycleFlow());
  if (src.has_expirationcyclepressure())
    PBProperty::Load(src.expirationcyclepressure(), dst.GetExpirationCyclePressure());
  if (src.has_expirationcyclevolume())
    PBProperty::Load(src.expirationcyclevolume(), dst.GetExpirationCycleVolume());
  if (src.has_expirationcycletime())
    PBProperty::Load(src.expirationcycletime(), dst.GetExpirationCycleTime());

  if (src.has_expirationlimbvolume())
    PBProperty::Load(src.expirationlimbvolume(), dst.GetExpirationLimbVolume());
  if (src.has_expirationtuberesistance())
    PBProperty::Load(src.expirationtuberesistance(), dst.GetExpirationTubeResistance());
  if (src.has_expirationvalveresistance())
    PBProperty::Load(src.expirationvalveresistance(), dst.GetExpirationValveResistance());
  if (src.has_expirationvalvevolume())
    PBProperty::Load(src.expirationvalvevolume(), dst.GetExpirationValveVolume());
  dst.SetExpirationWaveform((eMechanicalVentilator_DriverWaveform)src.expirationwaveform());
  if (src.has_expirationwaveformperiod())
    PBProperty::Load(src.expirationwaveformperiod(), dst.GetExpirationWaveformPeriod());

  if (src.has_inspirationlimitflow())
    PBProperty::Load(src.inspirationlimitflow(), dst.GetInspirationLimitFlow());
  if (src.has_inspirationlimitpressure())
    PBProperty::Load(src.inspirationlimitpressure(), dst.GetInspirationLimitPressure());
  if (src.has_inspirationlimitvolume())
    PBProperty::Load(src.inspirationlimitvolume(), dst.GetInspirationLimitVolume());

  if (src.has_inspirationpausetime())
    PBProperty::Load(src.inspirationpausetime(), dst.GetInspirationPauseTime());

  if (src.has_peakinspiratorypressure())
    PBProperty::Load(src.peakinspiratorypressure(), dst.GetPeakInspiratoryPressure());
  if (src.has_inspirationtargetflow())
    PBProperty::Load(src.inspirationtargetflow(), dst.GetInspirationTargetFlow());

  if (src.has_inspirationmachinetriggertime())
    PBProperty::Load(src.inspirationmachinetriggertime(), dst.GetInspirationMachineTriggerTime());

  if (src.has_inspirationpatienttriggerflow())
    PBProperty::Load(src.inspirationpatienttriggerflow(), dst.GetInspirationPatientTriggerFlow());
  if (src.has_inspirationpatienttriggerpressure())
    PBProperty::Load(src.inspirationpatienttriggerpressure(), dst.GetInspirationPatientTriggerPressure());  

  if (src.has_inspirationlimbvolume())
    PBProperty::Load(src.inspirationlimbvolume(), dst.GetInspirationLimbVolume());
  if (src.has_inspirationtuberesistance())
    PBProperty::Load(src.inspirationtuberesistance(), dst.GetInspirationTubeResistance());
  if (src.has_inspirationvalveresistance())
    PBProperty::Load(src.inspirationvalveresistance(), dst.GetInspirationValveResistance());
  if (src.has_inspirationvalvevolume())
    PBProperty::Load(src.inspirationvalvevolume(), dst.GetInspirationValveVolume());
  dst.SetInspirationWaveform((eMechanicalVentilator_DriverWaveform)src.inspirationwaveform());
  if (src.has_inspirationwaveformperiod())
    PBProperty::Load(src.inspirationwaveformperiod(), dst.GetInspirationWaveformPeriod());

  if (src.has_ypiecevolume())
    PBProperty::Load(src.ypiecevolume(), dst.GetYPieceVolume());
  if (src.has_connectionvolume())
    PBProperty::Load(src.connectionvolume(), dst.GetConnectionVolume());

  const SESubstance* sub;
  for (int i = 0; i < src.fractioninspiredgas_size(); i++)
  {
    const CDM_BIND::SubstanceFractionData& sfData = src.fractioninspiredgas()[i];
    sub = subMgr.GetSubstance(sfData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring a mechanical ventilator fraction inspired gas that was not found : " + sfData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Gas)
    {
      dst.Error("Ignoring a mechanical ventilator fraction gas that is not a gas : " + sfData.name());
      continue;
    }
    PBSubstance::Load(sfData, dst.GetFractionInspiredGas(*sub));
  }

  for (int i = 0; i < src.concentrationinspiredaerosol_size(); i++)
  {
    const CDM_BIND::SubstanceConcentrationData& scData = src.concentrationinspiredaerosol()[i];
    sub = subMgr.GetSubstance(scData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring an mechanical ventilator aerosol concentration that was not found : " + scData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Liquid && sub->GetState() != eSubstance_State::Solid)
    {
      dst.Error("Ignoring an mechanical ventilator aerosol concentration that is not a gas : " + scData.name());
      continue;
    }
    PBSubstance::Load(scData, dst.GetConcentrationInspiredAerosol(*sub));
  }
}

CDM_BIND::MechanicalVentilatorSettingsData* PBMechanicalVentilator::Unload(const SEMechanicalVentilatorSettings& src)
{
  CDM_BIND::MechanicalVentilatorSettingsData* dst = new CDM_BIND::MechanicalVentilatorSettingsData();
  PBMechanicalVentilator::Serialize(src, *dst);
  return dst;
}
void PBMechanicalVentilator::Serialize(const SEMechanicalVentilatorSettings& src, CDM_BIND::MechanicalVentilatorSettingsData& dst)
{
  dst.set_connection((CDM_BIND::MechanicalVentilatorSettingsData::eConnection)src.m_Connection);

  if (src.HasPositiveEndExpiredPressure())
    dst.set_allocated_positiveendexpiredpressure(PBProperty::Unload(*src.m_PositiveEndExpiredPressure));
  if (src.HasFunctionalResidualCapacity())
    dst.set_allocated_functionalresidualcapacity(PBProperty::Unload(*src.m_FunctionalResidualCapacity));

  if (src.HasExpirationCycleFlow())
    dst.set_allocated_expirationcycleflow(PBProperty::Unload(*src.m_ExpirationCycleFlow));
  if (src.HasExpirationCyclePressure())
    dst.set_allocated_expirationcyclepressure(PBProperty::Unload(*src.m_ExpirationCyclePressure));
  if (src.HasExpirationCycleVolume())
    dst.set_allocated_expirationcyclevolume(PBProperty::Unload(*src.m_ExpirationCycleVolume));
  if (src.HasExpirationCycleTime())
    dst.set_allocated_expirationcycletime(PBProperty::Unload(*src.m_ExpirationCycleTime));

  if (src.HasExpirationLimbVolume())
    dst.set_allocated_expirationlimbvolume(PBProperty::Unload(*src.m_ExpirationLimbVolume));
  if (src.HasExpirationTubeResistance())
    dst.set_allocated_expirationtuberesistance(PBProperty::Unload(*src.m_ExpirationTubeResistance));
  if (src.HasExpirationValveVolume())
    dst.set_allocated_expirationvalvevolume(PBProperty::Unload(*src.m_ExpirationValveVolume));
  if (src.HasExpirationValveResistance())
    dst.set_allocated_expirationvalveresistance(PBProperty::Unload(*src.m_ExpirationValveResistance));
  dst.set_expirationwaveform((CDM_BIND::MechanicalVentilatorSettingsData::eDriverWaveform)src.m_ExpirationWaveform);
  if (src.HasExpirationWaveformPeriod())
    dst.set_allocated_expirationwaveformperiod(PBProperty::Unload(*src.m_ExpirationWaveformPeriod));

  if (src.HasInspirationLimitFlow())
    dst.set_allocated_inspirationlimitflow(PBProperty::Unload(*src.m_InspirationLimitFlow));
  if (src.HasInspirationLimitPressure())
    dst.set_allocated_inspirationlimitpressure(PBProperty::Unload(*src.m_InspirationLimitPressure));
  if (src.HasInspirationLimitVolume())
    dst.set_allocated_inspirationlimitvolume(PBProperty::Unload(*src.m_InspirationLimitVolume));

  if (src.HasInspirationPauseTime())
    dst.set_allocated_inspirationpausetime(PBProperty::Unload(*src.m_InspirationPauseTime));

  if (src.HasPeakInspiratoryPressure())
    dst.set_allocated_peakinspiratorypressure(PBProperty::Unload(*src.m_PeakInspiratoryPressure));
  if (src.HasInspirationTargetFlow())
    dst.set_allocated_inspirationtargetflow(PBProperty::Unload(*src.m_InspirationTargetFlow));

  if (src.HasInspirationMachineTriggerTime())
    dst.set_allocated_inspirationmachinetriggertime(PBProperty::Unload(*src.m_InspirationMachineTriggerTime));

  if (src.HasInspirationPatientTriggerFlow())
    dst.set_allocated_inspirationpatienttriggerflow(PBProperty::Unload(*src.m_InspirationPatientTriggerFlow));
  if (src.HasInspirationPatientTriggerPressure())
    dst.set_allocated_inspirationpatienttriggerpressure(PBProperty::Unload(*src.m_InspirationPatientTriggerPressure));  

  if (src.HasInspirationTubeResistance())
    dst.set_allocated_inspirationtuberesistance(PBProperty::Unload(*src.m_InspirationTubeResistance));
  if (src.HasInspirationValveResistance())
    dst.set_allocated_inspirationvalveresistance(PBProperty::Unload(*src.m_InspirationValveResistance));
  if (src.HasInspirationLimbVolume())
    dst.set_allocated_inspirationlimbvolume(PBProperty::Unload(*src.m_InspirationLimbVolume));
  if (src.HasInspirationValveVolume())
    dst.set_allocated_inspirationvalvevolume(PBProperty::Unload(*src.m_InspirationValveVolume));
  dst.set_inspirationwaveform((CDM_BIND::MechanicalVentilatorSettingsData::eDriverWaveform)src.m_InspirationWaveform);
  if (src.HasInspirationWaveformPeriod())
    dst.set_allocated_inspirationwaveformperiod(PBProperty::Unload(*src.m_InspirationWaveformPeriod));

  if (src.HasYPieceVolume())
    dst.set_allocated_ypiecevolume(PBProperty::Unload(*src.m_YPieceVolume));
  if (src.HasConnectionVolume())
    dst.set_allocated_connectionvolume(PBProperty::Unload(*src.m_ConnectionVolume));

  for (SESubstanceFraction* sf : src.m_FractionInspiredGases)
    dst.mutable_fractioninspiredgas()->AddAllocated(PBSubstance::Unload(*sf));

  for (SESubstanceConcentration* sc : src.m_ConcentrationInspiredAerosols)
    dst.mutable_concentrationinspiredaerosol()->AddAllocated(PBSubstance::Unload(*sc));
}

bool PBMechanicalVentilator::SerializeToString(const SEMechanicalVentilatorSettings& src, std::string& output, eSerializationFormat m)
{
  CDM_BIND::MechanicalVentilatorSettingsData data;
  PBMechanicalVentilator::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBMechanicalVentilator::SerializeToFile(const SEMechanicalVentilatorSettings& src, const std::string& filename)
{
  CDM_BIND::MechanicalVentilatorSettingsData data;
  PBMechanicalVentilator::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBMechanicalVentilator::SerializeFromString(const std::string& src, SEMechanicalVentilatorSettings& dst, eSerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::MechanicalVentilatorSettingsData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBMechanicalVentilator::Load(data, dst, subMgr);
  return true;
}
bool PBMechanicalVentilator::SerializeFromFile(const std::string& filename, SEMechanicalVentilatorSettings& dst, const SESubstanceManager& subMgr)
{
  CDM_BIND::MechanicalVentilatorSettingsData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBMechanicalVentilator::Load(data, dst, subMgr);
  return true;
}
