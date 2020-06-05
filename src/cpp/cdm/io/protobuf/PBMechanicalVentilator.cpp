/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/MechanicalVentilator.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBMechanicalVentilator.h"
#include "io/protobuf/PBSubstance.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"

void PBMechanicalVentilator::Load(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst)
{
  dst.Clear();
  PBMechanicalVentilator::Serialize(src, dst);
}
void PBMechanicalVentilator::Serialize(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst)
{
  dst.SetConnection((eMechanicalVentilator_Connection)src.connection());

  if (src.has_endotrachealtuberesistance())
    PBProperty::Load(src.endotrachealtuberesistance(), dst.GetEndotrachealTubeResistance());

  if (src.has_positiveendexpiredpressure())
    PBProperty::Load(src.positiveendexpiredpressure(), dst.GetPositiveEndExpiredPressure());
  if (src.has_functionalresidualcapacity())
    PBProperty::Load(src.functionalresidualcapacity(), dst.GetFunctionalResidualCapacity());

  if (src.has_expirationcycleflow())
    PBProperty::Load(src.expirationcycleflow(), dst.GetExpirationCycleFlow());
  if (src.has_expirationcyclepressure())
    PBProperty::Load(src.expirationcyclepressure(), dst.GetExpirationCyclePressure());
  if (src.has_expirationcycletime())
    PBProperty::Load(src.expirationcycletime(), dst.GetExpirationCycleTime());

  if (src.has_expirationtuberesistance())
    PBProperty::Load(src.expirationtuberesistance(), dst.GetExpirationTubeResistance());
  if (src.has_expirationvalveresistance())
    PBProperty::Load(src.expirationvalveresistance(), dst.GetExpirationValveResistance());
  dst.SetExpirationWaveform((eMechanicalVentilator_DriverWaveform)src.expirationwaveform());

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
  if (src.has_endtidalcarbondioxidepressure())
    PBProperty::Load(src.endtidalcarbondioxidepressure(), dst.GetEndTidalCarbonDioxidePressure());

  if (src.has_inspirationtriggerflow())
    PBProperty::Load(src.inspirationtriggerflow(), dst.GetInspirationTriggerFlow());
  if (src.has_inspirationtriggerpressure())
    PBProperty::Load(src.inspirationtriggerpressure(), dst.GetInspirationTriggerPressure());
  if (src.has_inspirationtriggertime())
    PBProperty::Load(src.inspirationtriggertime(), dst.GetInspirationTriggerTime());

  if (src.has_inspirationtuberesistance())
    PBProperty::Load(src.inspirationtuberesistance(), dst.GetInspirationTubeResistance());
  if (src.has_inspirationvalveresistance())
    PBProperty::Load(src.inspirationvalveresistance(), dst.GetInspirationValveResistance());
  dst.SetInspirationWaveform((eMechanicalVentilator_DriverWaveform)src.inspirationwaveform());

  SESubstance* sub;
  for (int i = 0; i < src.fractioninspiredgas_size(); i++)
  {
    const CDM_BIND::SubstanceFractionData& sfData = src.fractioninspiredgas()[i];
    sub = dst.m_Substances.GetSubstance(sfData.name());
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
    sub = dst.m_Substances.GetSubstance(scData.name());
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

  dst.StateChange();
}

CDM_BIND::MechanicalVentilatorData* PBMechanicalVentilator::Unload(const SEMechanicalVentilator& src)
{
  CDM_BIND::MechanicalVentilatorData* dst = new CDM_BIND::MechanicalVentilatorData();
  PBMechanicalVentilator::Serialize(src, *dst);
  return dst;
}
void PBMechanicalVentilator::Serialize(const SEMechanicalVentilator& src, CDM_BIND::MechanicalVentilatorData& dst)
{
  dst.set_connection((CDM_BIND::MechanicalVentilatorData::eConnection)src.m_Connection);

  if (src.HasEndotrachealTubeResistance())
    dst.set_allocated_endotrachealtuberesistance(PBProperty::Unload(*src.m_EndotrachealTubeResistance));

  if (src.HasPositiveEndExpiredPressure())
    dst.set_allocated_positiveendexpiredpressure(PBProperty::Unload(*src.m_PositiveEndExpiredPressure));
  if (src.HasFunctionalResidualCapacity())
    dst.set_allocated_functionalresidualcapacity(PBProperty::Unload(*src.m_FunctionalResidualCapacity));

  if (src.HasExpirationCycleFlow())
    dst.set_allocated_expirationcycleflow(PBProperty::Unload(*src.m_ExpirationCycleFlow));
  if (src.HasExpirationCyclePressure())
    dst.set_allocated_expirationcyclepressure(PBProperty::Unload(*src.m_ExpirationCyclePressure));
  if (src.HasExpirationCycleTime())
    dst.set_allocated_expirationcycletime(PBProperty::Unload(*src.m_ExpirationCycleTime));

  if (src.HasExpirationTubeResistance())
    dst.set_allocated_expirationtuberesistance(PBProperty::Unload(*src.m_ExpirationTubeResistance));
  if (src.HasExpirationValveResistance())
    dst.set_allocated_expirationvalveresistance(PBProperty::Unload(*src.m_ExpirationValveResistance));
  dst.set_expirationwaveform((CDM_BIND::MechanicalVentilatorData::eDriverWaveform)src.m_ExpirationWaveform);

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
  if (src.HasEndTidalCarbonDioxidePressure())
    dst.set_allocated_endtidalcarbondioxidepressure(PBProperty::Unload(*src.m_EndTidalCarbonDioxidePressure));

  if (src.HasInspirationTriggerFlow())
    dst.set_allocated_inspirationtriggerflow(PBProperty::Unload(*src.m_InspirationTriggerFlow));
  if (src.HasInspirationTriggerPressure())
    dst.set_allocated_inspirationtriggerpressure(PBProperty::Unload(*src.m_InspirationTriggerPressure));
  if (src.HasInspirationTriggerTime())
    dst.set_allocated_inspirationtriggertime(PBProperty::Unload(*src.m_InspirationTriggerTime));

  if (src.HasInspirationTubeResistance())
    dst.set_allocated_inspirationtuberesistance(PBProperty::Unload(*src.m_InspirationTubeResistance));
  if (src.HasInspirationValveResistance())
    dst.set_allocated_inspirationvalveresistance(PBProperty::Unload(*src.m_InspirationValveResistance));
  dst.set_inspirationwaveform((CDM_BIND::MechanicalVentilatorData::eDriverWaveform)src.m_InspirationWaveform);

  for (SESubstanceFraction* sf : src.m_FractionInspiredGases)
    dst.mutable_fractioninspiredgas()->AddAllocated(PBSubstance::Unload(*sf));

  for (SESubstanceConcentration* sc : src.m_ConcentrationInspiredAerosols)
    dst.mutable_concentrationinspiredaerosol()->AddAllocated(PBSubstance::Unload(*sc));
}

bool PBMechanicalVentilator::SerializeToString(const SEMechanicalVentilator& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::MechanicalVentilatorData data;
  PBMechanicalVentilator::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBMechanicalVentilator::SerializeToFile(const SEMechanicalVentilator& src, const std::string& filename, SerializationFormat m)
{
  CDM_BIND::MechanicalVentilatorData data;
  PBMechanicalVentilator::Serialize(src, data);
  std::string content;
  PBMechanicalVentilator::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}

bool PBMechanicalVentilator::SerializeFromString(const std::string& src, SEMechanicalVentilator& dst, SerializationFormat m)
{
  CDM_BIND::MechanicalVentilatorData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBMechanicalVentilator::Load(data, dst);
  return true;
}
bool PBMechanicalVentilator::SerializeFromFile(const std::string& filename, SEMechanicalVentilator& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBMechanicalVentilator::SerializeFromString(content, dst, m);
}
