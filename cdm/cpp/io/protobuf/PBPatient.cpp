/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBPatient.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cdm/Patient.pb.h"
#include "bind/cdm/PatientNutrition.pb.h"
#include "patient/SEPatient.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"

void PBPatient::Load(const cdm::PatientData& src, SEPatient& dst)
{
  PBPatient::Serialize(src, dst);
}
void PBPatient::Serialize(const cdm::PatientData& src, SEPatient& dst)
{
  dst.Clear();
  dst.SetName(src.name());
  dst.SetSex((ePatient_Sex)src.sex());
  if (src.has_age())
    PBProperty::Load(src.age(), dst.GetAge());
  if (src.has_weight())
    PBProperty::Load(src.weight(), dst.GetWeight());
  if (src.has_height())
    PBProperty::Load(src.height(), dst.GetHeight());
  if (src.has_bodydensity())
    PBProperty::Load(src.bodydensity(), dst.GetBodyDensity());
  if (src.has_bodyfatfraction())
    PBProperty::Load(src.bodyfatfraction(), dst.GetBodyFatFraction());
  if (src.has_leanbodymass())
    PBProperty::Load(src.leanbodymass(), dst.GetLeanBodyMass());

  if (src.has_alveolisurfacearea())
    PBProperty::Load(src.alveolisurfacearea(), dst.GetAlveoliSurfaceArea());
  if (src.has_rightlungratio())
    PBProperty::Load(src.rightlungratio(), dst.GetRightLungRatio());
  if (src.has_skinsurfacearea())
    PBProperty::Load(src.skinsurfacearea(), dst.GetSkinSurfaceArea());

  if (src.has_basalmetabolicrate())
    PBProperty::Load(src.basalmetabolicrate(), dst.GetBasalMetabolicRate());
  if (src.has_bloodvolumebaseline())
    PBProperty::Load(src.bloodvolumebaseline(), dst.GetBloodVolumeBaseline());
  if (src.has_diastolicarterialpressurebaseline())
    PBProperty::Load(src.diastolicarterialpressurebaseline(), dst.GetDiastolicArterialPressureBaseline());
  if (src.has_heartratebaseline())
    PBProperty::Load(src.heartratebaseline(), dst.GetHeartRateBaseline());
  if (src.has_meanarterialpressurebaseline())
    PBProperty::Load(src.meanarterialpressurebaseline(), dst.GetMeanArterialPressureBaseline());
  if (src.has_respirationratebaseline())
    PBProperty::Load(src.respirationratebaseline(), dst.GetRespirationRateBaseline());
  if (src.has_systolicarterialpressurebaseline())
    PBProperty::Load(src.systolicarterialpressurebaseline(), dst.GetSystolicArterialPressureBaseline());
  if (src.has_tidalvolumebaseline())
    PBProperty::Load(src.tidalvolumebaseline(), dst.GetTidalVolumeBaseline());

  if (src.has_heartratemaximum())
    PBProperty::Load(src.heartratemaximum(), dst.GetHeartRateMaximum());
  if (src.has_heartrateminimum())
    PBProperty::Load(src.heartrateminimum(), dst.GetHeartRateMinimum());
  if (src.has_expiratoryreservevolume())
    PBProperty::Load(src.expiratoryreservevolume(), dst.GetExpiratoryReserveVolume());
  if (src.has_functionalresidualcapacity())
    PBProperty::Load(src.functionalresidualcapacity(), dst.GetFunctionalResidualCapacity());
  if (src.has_inspiratorycapacity())
    PBProperty::Load(src.inspiratorycapacity(), dst.GetInspiratoryCapacity());
  if (src.has_inspiratoryreservevolume())
    PBProperty::Load(src.inspiratoryreservevolume(), dst.GetInspiratoryReserveVolume());
  if (src.has_residualvolume())
    PBProperty::Load(src.residualvolume(), dst.GetResidualVolume());
  if (src.has_totallungcapacity())
    PBProperty::Load(src.totallungcapacity(), dst.GetTotalLungCapacity());
  if (src.has_vitalcapacity())
    PBProperty::Load(src.vitalcapacity(), dst.GetVitalCapacity());

  SEScalarTime time;
  for (int i = 0; i < src.activeevent_size(); i++)
  {
    const cdm::PatientData::ActiveEventData& e = src.activeevent(i);
    if (e.has_duration())
      PBProperty::Load(e.duration(), time);
    {
      dst.m_ss << "Active Patient event " << cdm::ePatient_Event_Name(e.event()) << " does not have time associated with it";
      dst.Warning(dst.m_ss);
      time.SetValue(0, TimeUnit::s);
    }
    dst.m_EventState[(ePatient_Event)e.event()] = true;
    dst.m_EventDuration_s[(ePatient_Event)e.event()] = time.GetValue(TimeUnit::s);
  }
}

cdm::PatientData* PBPatient::Unload(const SEPatient& src)
{
  cdm::PatientData* dst = new cdm::PatientData();
  PBPatient::Serialize(src, *dst);
  return dst;
}
void PBPatient::Serialize(const SEPatient& src, cdm::PatientData& dst)
{
  if (src.HasName())
    dst.set_name(src.m_Name);
  dst.set_sex((cdm::ePatient_Sex)src.m_Sex);
  if (src.HasAge())
    dst.set_allocated_age(PBProperty::Unload(*src.m_Age));
  if (src.HasWeight())
    dst.set_allocated_weight(PBProperty::Unload(*src.m_Weight));
  if (src.HasHeight())
    dst.set_allocated_height(PBProperty::Unload(*src.m_Height));
  if (src.HasBodyDensity())
    dst.set_allocated_bodydensity(PBProperty::Unload(*src.m_BodyDensity));
  if (src.HasBodyFatFraction())
    dst.set_allocated_bodyfatfraction(PBProperty::Unload(*src.m_BodyFatFraction));
  if (src.HasLeanBodyMass())
    dst.set_allocated_leanbodymass(PBProperty::Unload(*src.m_LeanBodyMass));

  if (src.HasAlveoliSurfaceArea())
    dst.set_allocated_alveolisurfacearea(PBProperty::Unload(*src.m_AlveoliSurfaceArea));
  if (src.HasRightLungRatio())
    dst.set_allocated_rightlungratio(PBProperty::Unload(*src.m_RightLungRatio));
  if (src.HasSkinSurfaceArea())
    dst.set_allocated_skinsurfacearea(PBProperty::Unload(*src.m_SkinSurfaceArea));

  if (src.HasBasalMetabolicRate())
    dst.set_allocated_basalmetabolicrate(PBProperty::Unload(*src.m_BasalMetabolicRate));
  if (src.HasBloodVolumeBaseline())
    dst.set_allocated_bloodvolumebaseline(PBProperty::Unload(*src.m_BloodVolumeBaseline));
  if (src.HasDiastolicArterialPressureBaseline())
    dst.set_allocated_diastolicarterialpressurebaseline(PBProperty::Unload(*src.m_DiastolicArterialPressureBaseline));
  if (src.HasHeartRateBaseline())
    dst.set_allocated_heartratebaseline(PBProperty::Unload(*src.m_HeartRateBaseline));
  if (src.HasMeanArterialPressureBaseline())
    dst.set_allocated_meanarterialpressurebaseline(PBProperty::Unload(*src.m_MeanArterialPressureBaseline));
  if (src.HasRespirationRateBaseline())
    dst.set_allocated_respirationratebaseline(PBProperty::Unload(*src.m_RespirationRateBaseline));
  if (src.HasSystolicArterialPressureBaseline())
    dst.set_allocated_systolicarterialpressurebaseline(PBProperty::Unload(*src.m_SystolicArterialPressureBaseline));
  if (src.HasTidalVolumeBaseline())
    dst.set_allocated_tidalvolumebaseline(PBProperty::Unload(*src.m_TidalVolumeBaseline));

  if (src.HasHeartRateMaximum())
    dst.set_allocated_heartratemaximum(PBProperty::Unload(*src.m_HeartRateMaximum));
  if (src.HasHeartRateMinimum())
    dst.set_allocated_heartrateminimum(PBProperty::Unload(*src.m_HeartRateMinimum));

  if (src.HasExpiratoryReserveVolume())
    dst.set_allocated_expiratoryreservevolume(PBProperty::Unload(*src.m_ExpiratoryReserveVolume));
  if (src.HasFunctionalResidualCapacity())
    dst.set_allocated_functionalresidualcapacity(PBProperty::Unload(*src.m_FunctionalResidualCapacity));
  if (src.HasInspiratoryCapacity())
    dst.set_allocated_inspiratorycapacity(PBProperty::Unload(*src.m_InspiratoryCapacity));
  if (src.HasInspiratoryReserveVolume())
    dst.set_allocated_inspiratoryreservevolume(PBProperty::Unload(*src.m_InspiratoryReserveVolume));
  if (src.HasResidualVolume())
    dst.set_allocated_residualvolume(PBProperty::Unload(*src.m_ResidualVolume));
  if (src.HasTotalLungCapacity())
    dst.set_allocated_totallungcapacity(PBProperty::Unload(*src.m_TotalLungCapacity));
  if (src.HasVitalCapacity())
    dst.set_allocated_vitalcapacity(PBProperty::Unload(*src.m_VitalCapacity));

  SEScalarTime time;
  for (auto itr : src.m_EventState)
  {
    if (!itr.second)
      continue;

    auto it2 = src.m_EventDuration_s.find(itr.first);
    if (it2 == src.m_EventDuration_s.end())// This should not happen... 
      time.SetValue(0, TimeUnit::s);
    else
      time.SetValue(it2->second, TimeUnit::s);

    cdm::PatientData_ActiveEventData* eData = dst.add_activeevent();

    eData->set_event((cdm::ePatient_Event)itr.first);
    eData->set_allocated_duration(PBProperty::Unload(time));
  }
}
void PBPatient::Copy(const SEPatient& src, SEPatient& dst)
{
  cdm::PatientData data;
  PBPatient::Serialize(src, data);
  PBPatient::Serialize(data, dst);
}

bool PBPatient::SerializeToString(const SEPatient& src, std::string& output, SerializationMode m)
{
  cdm::PatientData data;
  PBPatient::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBPatient::SerializeToFile(const SEPatient& src, const std::string& filename, SerializationMode m)
{
  cdm::PatientData data;
  PBPatient::Serialize(src, data);
  std::string content;
  PBPatient::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}

bool PBPatient::SerializeFromString(const std::string& src, SEPatient& dst, SerializationMode m)
{
  cdm::PatientData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBPatient::Load(data, dst);
  return true;
}
bool PBPatient::SerializeFromFile(const std::string& filename, SEPatient& dst, SerializationMode m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBPatient::SerializeFromString(content, dst, m);
}