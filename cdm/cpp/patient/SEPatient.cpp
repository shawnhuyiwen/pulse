/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"

#include "patient/SEPatient.h"
#include "patient/SENutrition.h"

#include "engine/SEEventHandler.h"

#include "properties/SEScalarTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarFlowElastance.h"
#include "io/protobuf/PBPatient.h"


SEPatient::SEPatient(Logger* logger) : Loggable(logger)
{
  m_EventHandler = nullptr;

  m_Name="";
  m_Sex = ePatient_Sex::Male;
  m_Age=nullptr;
  m_Weight=nullptr;
  m_Height=nullptr;

  m_AlveoliSurfaceArea = nullptr;
  m_BasalMetabolicRate = nullptr;
  m_BloodVolumeBaseline = nullptr;
  m_BodyDensity = nullptr;
  m_BodyFatFraction = nullptr;
  m_DiastolicArterialPressureBaseline = nullptr;
  m_ExpiratoryReserveVolume = nullptr;
  m_FunctionalResidualCapacity = nullptr;
  m_HeartRateBaseline = nullptr;
  m_HeartRateMaximum = nullptr;
  m_HeartRateMinimum = nullptr;
  m_InspiratoryCapacity = nullptr;
  m_InspiratoryReserveVolume = nullptr;
  m_LeanBodyMass = nullptr;
  m_MeanArterialPressureBaseline = nullptr;
  m_ResidualVolume = nullptr;
  m_RespirationRateBaseline = nullptr;
  m_RightLungRatio = nullptr;
  m_SkinSurfaceArea = nullptr;
  m_SystolicArterialPressureBaseline = nullptr;
  m_TidalVolumeBaseline = nullptr;
  m_TotalLungCapacity = nullptr;
  m_VitalCapacity = nullptr;
}

SEPatient::~SEPatient()
{
  Clear();  
}

void SEPatient::Clear()
{
  m_EventHandler=nullptr;
  m_EventState.clear();
  m_EventDuration_s.clear();
  m_Name="";
  m_Sex = ePatient_Sex::Male;
  SAFE_DELETE(m_Age);
  SAFE_DELETE(m_Weight);
  SAFE_DELETE(m_Height);

  SAFE_DELETE(m_AlveoliSurfaceArea);
  SAFE_DELETE(m_BasalMetabolicRate);
  SAFE_DELETE(m_BloodVolumeBaseline);
  SAFE_DELETE(m_BodyDensity);
  SAFE_DELETE(m_BodyFatFraction);
  SAFE_DELETE(m_DiastolicArterialPressureBaseline);
  SAFE_DELETE(m_ExpiratoryReserveVolume);
  SAFE_DELETE(m_FunctionalResidualCapacity);
  SAFE_DELETE(m_HeartRateBaseline);
  SAFE_DELETE(m_HeartRateMaximum);
  SAFE_DELETE(m_HeartRateMinimum);
  SAFE_DELETE(m_InspiratoryCapacity);
  SAFE_DELETE(m_InspiratoryReserveVolume);
  SAFE_DELETE(m_LeanBodyMass);
  SAFE_DELETE(m_MeanArterialPressureBaseline);
  SAFE_DELETE(m_ResidualVolume);
  SAFE_DELETE(m_RespirationRateBaseline);
  SAFE_DELETE(m_RightLungRatio);
  SAFE_DELETE(m_SkinSurfaceArea);
  SAFE_DELETE(m_SystolicArterialPressureBaseline);
  SAFE_DELETE(m_TidalVolumeBaseline);
  SAFE_DELETE(m_TotalLungCapacity);
  SAFE_DELETE(m_VitalCapacity);
}

void SEPatient::Copy(const SEPatient& src)
{
  PBPatient::Copy(src, *this);
}

bool SEPatient::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBPatient::SerializeToString(*this, output, m);
}
bool SEPatient::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBPatient::SerializeToFile(*this, filename, m);
}
bool SEPatient::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBPatient::SerializeFromString(src, *this, m);
}
bool SEPatient::SerializeFromFile(const std::string& filename, SerializationFormat m)
{
  return PBPatient::SerializeFromFile(filename, *this, m);
}

const SEScalar* SEPatient::GetScalar(const std::string& name)
{
  if (name.compare("Age") == 0)
    return &GetAge();
  if (name.compare("Weight") == 0)
    return &GetWeight();
  if (name.compare("Height") == 0)
    return &GetHeight();

  if (name.compare("AlveoliSurfaceArea") == 0)
    return &GetAlveoliSurfaceArea();
  if (name.compare("BasalMetabolicRate") == 0)
    return &GetBasalMetabolicRate();
  if (name.compare("BloodVolumeBaseline") == 0)
    return &GetBloodVolumeBaseline();
  if (name.compare("BodyDensity") == 0)
    return &GetBodyDensity();
  if (name.compare("BodyFatFraction") == 0)
    return &GetBodyFatFraction();
  if (name.compare("DiastolicArterialPressureBaseline") == 0)
    return &GetDiastolicArterialPressureBaseline();
  if (name.compare("ExpiratoryReserveVolume") == 0)
    return &GetExpiratoryReserveVolume();
  if (name.compare("FunctionalResidualCapacity") == 0)
    return &GetFunctionalResidualCapacity();
  if (name.compare("HeartRateBaseline") == 0)
    return &GetHeartRateBaseline();
  if (name.compare("HeartRateMaximum") == 0)
    return &GetHeartRateMaximum();
  if (name.compare("HeartRateMinimum") == 0)
    return &GetHeartRateMinimum();
  if (name.compare("InspiratoryCapacity") == 0)
    return &GetInspiratoryCapacity();
  if (name.compare("InspiratoryReserveVolume") == 0)
    return &GetInspiratoryReserveVolume();
  if (name.compare("LeanBodyMass") == 0)
    return &GetLeanBodyMass();
  if (name.compare("MeanArterialPressureBaseline") == 0)
    return &GetMeanArterialPressureBaseline();
  if (name.compare("ResidualVolume") == 0)
    return &GetResidualVolume();
  if (name.compare("RespirationRateBaseline") == 0)
    return &GetRespirationRateBaseline();
  if (name.compare("RightLungRatio") == 0)
    return &GetRightLungRatio();
  if (name.compare("SkinSurfaceArea") == 0)
    return &GetSkinSurfaceArea();
  if (name.compare("SystolicArterialPressureBaseline") == 0)
    return &GetSystolicArterialPressureBaseline();
  if (name.compare("TidalVolumeBaseline") == 0)
    return &GetTidalVolumeBaseline();
  if (name.compare("TotalLungCapacity") == 0)
    return &GetTotalLungCapacity();
  if (name.compare("VitalCapacity") == 0)
    return &GetVitalCapacity();

  return nullptr;
}

void SEPatient::SetEvent(ePatient_Event type, bool active, const SEScalarTime& time)
{
  bool b=false;// Default is off
  if(m_EventState.find(type)!=m_EventState.end())
    b = m_EventState[type];
  if(b==active)
    return;//No Change
  if(active!=b)
  {
    m_ss.str("");
    m_ss<<"[Event] "<<time<<", ";
    if(active)
    {
      switch(type)
      {
      case ePatient_Event::Antidiuresis:
        m_ss << " Patient has Antidiuresis";
        break;
      case ePatient_Event::Asystole:
        m_ss << " Patient has Asystole";
        break;
      case ePatient_Event::Bradycardia:
        m_ss << " Patient has Bradycardia";
        break;
      case ePatient_Event::Bradypnea:
        m_ss << " Patient has Bradypnea";
        break;
      case ePatient_Event::BrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain is dangerously low";
        break;
      case ePatient_Event::CardiacArrest:
        m_ss << " Patient has Cardiac Arrest";
        break;
      case ePatient_Event::CardiogenicShock:
        m_ss << " Patient has Cardiogenic Shock";
        break;
      case ePatient_Event::CriticalBrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain is critically low";
        break;
      case ePatient_Event::Dehydration:
        m_ss << " Patient has entered state of Dehydration";
        break;
      case ePatient_Event::Diuresis:
        m_ss << " Patient has entered Diuresis";
        break;
      case ePatient_Event::Fasciculation:
        m_ss << "Patient has Fasciculation";
        break;
      case ePatient_Event::FunctionalIncontinence:
        m_ss << " Patient has involuntarily emptied their bladder";
        break;
      case ePatient_Event::Hypercapnia:
        m_ss << " Patient has Hypercapnia";
        break;
      case ePatient_Event::Hyperglycemia:
        m_ss << " Patient has Hyperglycemia";
        break;
      case ePatient_Event::Hyperthermia:
        m_ss << " Patient is Hyperthermic";
        break;
      case ePatient_Event::Hypoglycemia:
        m_ss << " Patient has Hypoglycemia";
        break;
      case ePatient_Event::Hypothermia:
        m_ss << " Patient is Hypothermic";
        break;
      case ePatient_Event::Hypoxia:
        m_ss << " Patient has Hypoxia";
        break;
      case ePatient_Event::IntracranialHypertension:
        m_ss << " Patient has Intracranial Hypertension";
        break;
      case ePatient_Event::IntracranialHypotension:
        m_ss << " Patient has Intracranial Hypotension";
        break;
      case ePatient_Event::HypovolemicShock:
        m_ss << " Patient is in Hypovolemic Shock";
        break;
      case ePatient_Event::IrreversibleState:
        m_ss<<" Patient has entered irreversible state";
        break;
      case ePatient_Event::Ketoacidosis:
        m_ss << " Patient has Ketoacidosis";
        break;
      case ePatient_Event::LacticAcidosis:
        m_ss << " Patient has LacticAcidosis";
        break;
      case ePatient_Event::MaximumPulmonaryVentilationRate:
        m_ss << " Patient's Respiratory Driver has exceeded the maximum target pulmonary ventilation rate, setting value to the maximum allowable rate";
        break;
      case ePatient_Event::MetabolicAcidosis:
        m_ss << " The patient is in a state of metabolic acidosis";
        break;
      case ePatient_Event::MetabolicAlkalosis:
        m_ss << " The patient is in a state of metabolic alkalosis";
        break;
      case ePatient_Event::MildAcuteRespiratoryDistress:
        m_ss << " The patient has Mild Acute Respiratory Distress";
        break;
      case ePatient_Event::ModerateAcuteRespiratoryDistress:
        m_ss << " The patient has Moderate Acute Respiratory Distress";
        break;
      case ePatient_Event::MyocardiumOxygenDeficit:
        m_ss << " The patient's heart is not receiving enough oxygen";
        break;
      case ePatient_Event::Natriuresis:
        m_ss << " Patient has Natriuresis";
        break;
      case ePatient_Event::NutritionDepleted:
        m_ss << " Patient has depleted all nutrition in body";
        break;
      case ePatient_Event::PulselessRhythm:
        m_ss << " Patient has a Pulseless Rhythm";
        break;
      case ePatient_Event::RenalHypoperfusion:
        m_ss << " Patient has Renal Hypoperfusion";
        break;
      case ePatient_Event::SevereAcuteRespiratoryDistress:
        m_ss << " The patient has Severe Acute Respiratory Distress";
        break;
      case ePatient_Event::Tachycardia:
        m_ss<<" Patient has Tachycardia";
        break;
      case ePatient_Event::Tachypnea:
        m_ss << " Patient has Tachypnea";
        break;
      case ePatient_Event::Fatigue:
        m_ss << "Patient has fatigue";
        break;
      case ePatient_Event::StartOfCardiacCycle:
      case ePatient_Event::StartOfExhale:
      case ePatient_Event::StartOfInhale:
        m_ss.str("");// make m_ss empty and nothing will be logged, this event does not need to get logged each activation
        break;
      default:
        m_ss<<" Patient has entered state : "<< ePatient_Event_Name(type);
      }
    }
    else
    {
      switch(type)
      {
      case ePatient_Event::Antidiuresis:
        m_ss << " Patient no longer is in Antidiuresis";
        break;
      case ePatient_Event::Asystole:
        m_ss << " Patient no longer is in Asystole";
        break; 
      case ePatient_Event::Bradycardia:
        m_ss << " Patient no longer has Bradycardia";
        break;
      case ePatient_Event::Bradypnea:
        m_ss << " Patient no longer has Bradypnea";
        break;
      case ePatient_Event::BrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain has increased above the danger threshold";
        break;
      case ePatient_Event::CardiacArrest:
        m_ss << " Patient no longer has Cardiac Arrest";
        break;
      case ePatient_Event::CardiogenicShock:
        m_ss << " Patient no longer has Cardiogenic Shock";
        break;
      case ePatient_Event::CriticalBrainOxygenDeficit:
        m_ss << " Oxygen tension in the brain has increased above the critical threshold";
        break;
      case ePatient_Event::Dehydration:
        m_ss << " Patient no longer is in Dehydration state";
        break;
      case ePatient_Event::Diuresis:
        m_ss << " Patient no longer has Diuresis";
        break;
      case ePatient_Event::Fasciculation:
        m_ss << "Patient no longer has fasciculations";
        break;
      case ePatient_Event::FunctionalIncontinence:
        m_ss << " Patient has an empty bladder";
        break;
      case ePatient_Event::Hypercapnia:
        m_ss << " Patient no longer has Hypercapnia";
        break;
      case ePatient_Event::Hyperglycemia:
        m_ss << " Patient no longer has Hyperglycemia";
        break;
      case ePatient_Event::Hyperthermia:
        m_ss << " Patient is no longer has Hyperthermic";
        break;
      case ePatient_Event::Hypoglycemia:
        m_ss << " Patient no longer has Hypoglycemia";
        break;
      case ePatient_Event::Hypothermia:
        m_ss << " Patient is no longer has Hypothermic";
        break;
      case ePatient_Event::Hypoxia:
        m_ss << " Patient no longer has Hypoxia";
        break;
      case ePatient_Event::HypovolemicShock:
        m_ss << " Patient is no longer in Hypovolemic Shock";
        break;
      case ePatient_Event::IntracranialHypertension:
        m_ss << " Patient no longer has Intracranial Hypertension";
        break;
      case ePatient_Event::IntracranialHypotension:
        m_ss << " Patient no longer has Intracranial Hypotension";
        break;
      case ePatient_Event::IrreversibleState:
        m_ss<<" Patient no longer is in irreversible state?!";
        break;
      case ePatient_Event::Ketoacidosis:
        m_ss << " Patient no longer has Ketoacidosis";
        break;
      case ePatient_Event::LacticAcidosis:
        m_ss << " Patient no longer has LacticAcidosis";
        break;
      case ePatient_Event::MaximumPulmonaryVentilationRate:
        m_ss << " Patient's Respiratory Driver is no longer exceeding the maximum target pulmonary ventilation rate";
        break;
      case ePatient_Event::MetabolicAcidosis:
        m_ss << " The patient is no longer in a state of metabolic acidosis";
        break;
      case ePatient_Event::MetabolicAlkalosis:
        m_ss << " The patient is no longer in a state of metabolic alkalosis";
        break;
      case ePatient_Event::MildAcuteRespiratoryDistress:
        m_ss << " Patient no longer has a Mild Acute Respiratory Distress";
        break;
      case ePatient_Event::ModerateAcuteRespiratoryDistress:
        m_ss << " Patient no longer has a Moderate Acute Respiratory Distress";
        break;
      case ePatient_Event::MyocardiumOxygenDeficit:
        m_ss << " Patient no longer has a Myocardium Oxygen Deficit";
        break;
      case ePatient_Event::Natriuresis:
        m_ss << " Patient no longer has Natriuresis";
        break;
      case ePatient_Event::NutritionDepleted:
        m_ss << " Patient has nutrition in body";
        break;
      case ePatient_Event::PulselessRhythm:
        m_ss << " Patient no longer has a Pulseless Rhythm";
        break;
      case ePatient_Event::RenalHypoperfusion:
        m_ss << " Patient no longer has Renal Hypoperfusion";
        break;
      case ePatient_Event::SevereAcuteRespiratoryDistress:
        m_ss << " Patient no longer has a Severe Acute Respiratory Distress";
        break;
      case ePatient_Event::Tachycardia:
        m_ss<<" Patient no longer has Tachycardia";
        break;
      case ePatient_Event::Tachypnea:
        m_ss << " Patient no longer has Tachypnea";
        break;
      case ePatient_Event::Fatigue:
        m_ss << "Patient is no longer fatigued";
        break;
      case ePatient_Event::StartOfCardiacCycle:
      case ePatient_Event::StartOfExhale:
      case ePatient_Event::StartOfInhale:
        m_ss.str("");// make m_ss empty and nothing will be logged, this event does not need to get logged each activation
        break;
      default:
        m_ss<<" Patient has exited state : "<< ePatient_Event_Name(type);
      }
    }
    if (!m_ss.str().empty())
      Info(m_ss);
  }
  m_EventState[type] = active;
  m_EventDuration_s[type] = 0;
  if(m_EventHandler!=nullptr)
    m_EventHandler->HandlePatientEvent(type,active,&time);
}

bool SEPatient::IsEventActive(ePatient_Event type) const
{
  auto i = m_EventState.find(type);
  if(i==m_EventState.end())
    return false;
  return i->second;
}

double SEPatient::GetEventDuration(ePatient_Event type, const TimeUnit& unit) const
{
  auto i = m_EventDuration_s.find(type);
  if (i == m_EventDuration_s.end())
    return 0;
  return Convert(i->second, TimeUnit::s, unit);
}

void SEPatient::UpdateEvents(const SEScalarTime& timeStep)
{
  for(auto& itr : m_EventDuration_s)
    itr.second += timeStep.GetValue(TimeUnit::s);
}

void SEPatient::ForwardEvents(SEEventHandler* handler) const
{
  m_EventHandler=handler;
}

std::string SEPatient::GetName() const
{
  return m_Name;
}
void SEPatient::SetName(const std::string& name)
{
  m_Name = name;
}
bool SEPatient::HasName() const
{
  return m_Name.empty()?false:true;
}
void SEPatient::InvalidateName()
{
  m_Name = "";
}

ePatient_Sex SEPatient::GetSex() const
{
  return m_Sex;
}
void SEPatient::SetSex(ePatient_Sex sex)
{
  m_Sex = sex;
}

bool SEPatient::HasAge() const
{
  return m_Age==nullptr?false:m_Age->IsValid();
}
SEScalarTime& SEPatient::GetAge()
{
  if(m_Age==nullptr)
    m_Age=new SEScalarTime();
  return *m_Age;
}
double SEPatient::GetAge(const TimeUnit& unit) const
{
  if (m_Age == nullptr)
    return SEScalar::dNaN();
  return m_Age->GetValue(unit);
}

bool SEPatient::HasWeight() const
{
  return m_Weight==nullptr?false:m_Weight->IsValid();
}
SEScalarMass& SEPatient::GetWeight()
{
  if(m_Weight==nullptr)
    m_Weight=new SEScalarMass();
  return *m_Weight;
}
double SEPatient::GetWeight(const MassUnit& unit) const
{
  if (m_Weight == nullptr)
    return SEScalar::dNaN();
  return m_Weight->GetValue(unit);
}

bool SEPatient::HasHeight() const
{
  return m_Height==nullptr?false:m_Height->IsValid();
}
SEScalarLength& SEPatient::GetHeight()
{
  if(m_Height==nullptr)
    m_Height=new SEScalarLength();
  return *m_Height;
}
double SEPatient::GetHeight(const LengthUnit& unit) const
{
  if (m_Height == nullptr)
    return SEScalar::dNaN();
  return m_Height->GetValue(unit);
}

bool SEPatient::HasAlveoliSurfaceArea() const
{
  return m_AlveoliSurfaceArea == nullptr ? false : m_AlveoliSurfaceArea->IsValid();
}
SEScalarArea& SEPatient::GetAlveoliSurfaceArea()
{
  if (m_AlveoliSurfaceArea == nullptr)
    m_AlveoliSurfaceArea = new SEScalarArea();
  return *m_AlveoliSurfaceArea;
}
double SEPatient::GetAlveoliSurfaceArea(const AreaUnit& unit) const
{
  if (m_AlveoliSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_AlveoliSurfaceArea->GetValue(unit);
}

bool SEPatient::HasBasalMetabolicRate() const
{
  return m_BasalMetabolicRate == nullptr ? false : m_BasalMetabolicRate->IsValid();
}
SEScalarPower& SEPatient::GetBasalMetabolicRate()
{
  if (m_BasalMetabolicRate == nullptr)
    m_BasalMetabolicRate = new SEScalarPower();
  return *m_BasalMetabolicRate;
}
double SEPatient::GetBasalMetabolicRate(const PowerUnit& unit) const
{
  if (m_BasalMetabolicRate == nullptr)
    return SEScalar::dNaN();
  return m_BasalMetabolicRate->GetValue(unit);
}

bool SEPatient::HasBloodVolumeBaseline() const
{
  return m_BloodVolumeBaseline == nullptr ? false : m_BloodVolumeBaseline->IsValid();
}
SEScalarVolume& SEPatient::GetBloodVolumeBaseline()
{
  if (m_BloodVolumeBaseline == nullptr)
    m_BloodVolumeBaseline = new SEScalarVolume();
  return *m_BloodVolumeBaseline;
}
double SEPatient::GetBloodVolumeBaseline(const VolumeUnit& unit) const
{
  if (m_BloodVolumeBaseline == nullptr)
    return SEScalar::dNaN();
  return m_BloodVolumeBaseline->GetValue(unit);
}

bool SEPatient::HasBodyDensity() const
{
  return m_BodyDensity == nullptr ? false : m_BodyDensity->IsValid();
}
SEScalarMassPerVolume& SEPatient::GetBodyDensity()
{
  if (m_BodyDensity == nullptr)
    m_BodyDensity = new SEScalarMassPerVolume();
  return *m_BodyDensity;
}
double SEPatient::GetBodyDensity(const MassPerVolumeUnit& unit) const
{
  if (m_BodyDensity == nullptr)
    return SEScalar::dNaN();
  return m_BodyDensity->GetValue(unit);
}

bool SEPatient::HasBodyFatFraction() const
{
  return m_BodyFatFraction == nullptr ? false : m_BodyFatFraction->IsValid();
}
SEScalar0To1& SEPatient::GetBodyFatFraction()
{
  if (m_BodyFatFraction == nullptr)
    m_BodyFatFraction = new SEScalar0To1();
  return *m_BodyFatFraction;
}
double SEPatient::GetBodyFatFraction() const
{
  if (m_BodyFatFraction == nullptr)
    return SEScalar::dNaN();
  return m_BodyFatFraction->GetValue();
}

bool SEPatient::HasDiastolicArterialPressureBaseline() const
{
  return m_DiastolicArterialPressureBaseline == nullptr ? false : m_DiastolicArterialPressureBaseline->IsValid();
}
SEScalarPressure& SEPatient::GetDiastolicArterialPressureBaseline()
{
  if (m_DiastolicArterialPressureBaseline == nullptr)
    m_DiastolicArterialPressureBaseline = new SEScalarPressure();
  return *m_DiastolicArterialPressureBaseline;
}
double SEPatient::GetDiastolicArterialPressureBaseline(const PressureUnit& unit) const
{
  if (m_DiastolicArterialPressureBaseline == nullptr)
    return SEScalar::dNaN();
  return m_DiastolicArterialPressureBaseline->GetValue(unit);
}

bool SEPatient::HasExpiratoryReserveVolume() const
{
  return m_ExpiratoryReserveVolume==nullptr?false:m_ExpiratoryReserveVolume->IsValid();
}
SEScalarVolume& SEPatient::GetExpiratoryReserveVolume()
{
  if(m_ExpiratoryReserveVolume==nullptr)
    m_ExpiratoryReserveVolume=new SEScalarVolume();
  return *m_ExpiratoryReserveVolume;
}
double SEPatient::GetExpiratoryReserveVolume(const VolumeUnit& unit) const
{
  if (m_ExpiratoryReserveVolume == nullptr)
    return SEScalar::dNaN();
  return m_ExpiratoryReserveVolume->GetValue(unit);
}

bool SEPatient::HasFunctionalResidualCapacity() const
{
  return m_FunctionalResidualCapacity==nullptr?false:m_FunctionalResidualCapacity->IsValid();
}
SEScalarVolume& SEPatient::GetFunctionalResidualCapacity()
{
  if(m_FunctionalResidualCapacity==nullptr)
    m_FunctionalResidualCapacity=new SEScalarVolume();
  return *m_FunctionalResidualCapacity;
}
double SEPatient::GetFunctionalResidualCapacity(const VolumeUnit& unit) const
{
  if (m_FunctionalResidualCapacity == nullptr)
    return SEScalar::dNaN();
  return m_FunctionalResidualCapacity->GetValue(unit);
}

bool SEPatient::HasHeartRateBaseline() const
{
  return m_HeartRateBaseline==nullptr?false:m_HeartRateBaseline->IsValid();
}
SEScalarFrequency& SEPatient::GetHeartRateBaseline()
{
  if(m_HeartRateBaseline==nullptr)
    m_HeartRateBaseline=new SEScalarFrequency();
  return *m_HeartRateBaseline;
}
double SEPatient::GetHeartRateBaseline(const FrequencyUnit& unit) const
{
  if (m_HeartRateBaseline == nullptr)
    return SEScalar::dNaN();
  return m_HeartRateBaseline->GetValue(unit);
}

bool SEPatient::HasHeartRateMaximum() const
{
  return m_HeartRateMaximum==nullptr?false:m_HeartRateMaximum->IsValid();
}
SEScalarFrequency& SEPatient::GetHeartRateMaximum()
{
  if(m_HeartRateMaximum==nullptr)
    m_HeartRateMaximum=new SEScalarFrequency();
  return *m_HeartRateMaximum;
}
double SEPatient::GetHeartRateMaximum(const FrequencyUnit& unit) const
{
  if (m_HeartRateMaximum == nullptr)
    return SEScalar::dNaN();
  return m_HeartRateMaximum->GetValue(unit);
}

bool SEPatient::HasHeartRateMinimum() const
{
  return m_HeartRateMinimum==nullptr?false:m_HeartRateMinimum->IsValid();
}
SEScalarFrequency& SEPatient::GetHeartRateMinimum()
{
  if(m_HeartRateMinimum==nullptr)
    m_HeartRateMinimum=new SEScalarFrequency();
  return *m_HeartRateMinimum;
}
double SEPatient::GetHeartRateMinimum(const FrequencyUnit& unit) const
{
  if (m_HeartRateMinimum == nullptr)
    return SEScalar::dNaN();
  return m_HeartRateMinimum->GetValue(unit);
}

bool SEPatient::HasInspiratoryCapacity() const
{
  return m_InspiratoryCapacity == nullptr ? false : m_InspiratoryCapacity->IsValid();
}
SEScalarVolume& SEPatient::GetInspiratoryCapacity()
{
  if (m_InspiratoryCapacity == nullptr)
    m_InspiratoryCapacity = new SEScalarVolume();
  return *m_InspiratoryCapacity;
}
double SEPatient::GetInspiratoryCapacity(const VolumeUnit& unit) const
{
  if (m_InspiratoryCapacity == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryCapacity->GetValue(unit);
}

bool SEPatient::HasInspiratoryReserveVolume() const
{
  return m_InspiratoryReserveVolume == nullptr ? false : m_InspiratoryReserveVolume->IsValid();
}
SEScalarVolume& SEPatient::GetInspiratoryReserveVolume()
{
  if (m_InspiratoryReserveVolume == nullptr)
    m_InspiratoryReserveVolume = new SEScalarVolume();
  return *m_InspiratoryReserveVolume;
}
double SEPatient::GetInspiratoryReserveVolume(const VolumeUnit& unit) const
{
  if (m_InspiratoryReserveVolume == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryReserveVolume->GetValue(unit);
}

bool SEPatient::HasLeanBodyMass() const
{
  return m_LeanBodyMass == nullptr ? false : m_LeanBodyMass->IsValid();
}
SEScalarMass& SEPatient::GetLeanBodyMass()
{
  if (m_LeanBodyMass == nullptr)
    m_LeanBodyMass = new SEScalarMass();
  return *m_LeanBodyMass;
}
double SEPatient::GetLeanBodyMass(const MassUnit& unit) const
{
  if (m_LeanBodyMass == nullptr)
    return SEScalar::dNaN();
  return m_LeanBodyMass->GetValue(unit);
}

bool SEPatient::HasMeanArterialPressureBaseline() const
{
  return m_MeanArterialPressureBaseline==nullptr?false:m_MeanArterialPressureBaseline->IsValid();
}
SEScalarPressure& SEPatient::GetMeanArterialPressureBaseline()
{
  if(m_MeanArterialPressureBaseline==nullptr)
    m_MeanArterialPressureBaseline=new SEScalarPressure();
  return *m_MeanArterialPressureBaseline;
}
double SEPatient::GetMeanArterialPressureBaseline(const PressureUnit& unit) const
{
  if (m_MeanArterialPressureBaseline == nullptr)
    return SEScalar::dNaN();
  return m_MeanArterialPressureBaseline->GetValue(unit);
}

bool SEPatient::HasResidualVolume() const
{
  return m_ResidualVolume == nullptr ? false : m_ResidualVolume->IsValid();
}
SEScalarVolume& SEPatient::GetResidualVolume()
{
  if (m_ResidualVolume == nullptr)
    m_ResidualVolume = new SEScalarVolume();
  return *m_ResidualVolume;
}
double SEPatient::GetResidualVolume(const VolumeUnit& unit) const
{
  if (m_ResidualVolume == nullptr)
    return SEScalar::dNaN();
  return m_ResidualVolume->GetValue(unit);
}

bool SEPatient::HasRespirationRateBaseline() const
{
  return m_RespirationRateBaseline==nullptr?false:m_RespirationRateBaseline->IsValid();
}
SEScalarFrequency& SEPatient::GetRespirationRateBaseline()
{
  if(m_RespirationRateBaseline==nullptr)
    m_RespirationRateBaseline=new SEScalarFrequency();
  return *m_RespirationRateBaseline;
}
double SEPatient::GetRespirationRateBaseline(const FrequencyUnit& unit) const
{
  if (m_RespirationRateBaseline == nullptr)
    return SEScalar::dNaN();
  return m_RespirationRateBaseline->GetValue(unit);
}

bool SEPatient::HasRightLungRatio() const
{
  return m_RightLungRatio==nullptr?false:m_RightLungRatio->IsValid();
}
SEScalar0To1& SEPatient::GetRightLungRatio()
{
  if(m_RightLungRatio==nullptr)
    m_RightLungRatio=new SEScalar0To1();
  return *m_RightLungRatio;
}
double SEPatient::GetRightLungRatio() const
{
  if (m_RightLungRatio == nullptr)
    return SEScalar::dNaN();
  return m_RightLungRatio->GetValue();
}

bool SEPatient::HasSkinSurfaceArea() const
{
  return m_SkinSurfaceArea == nullptr ? false : m_SkinSurfaceArea->IsValid();
}
SEScalarArea& SEPatient::GetSkinSurfaceArea()
{
  if (m_SkinSurfaceArea == nullptr)
    m_SkinSurfaceArea = new SEScalarArea();
  return *m_SkinSurfaceArea;
}
double SEPatient::GetSkinSurfaceArea(const AreaUnit& unit) const
{
  if (m_SkinSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_SkinSurfaceArea->GetValue(unit);
}

bool SEPatient::HasSystolicArterialPressureBaseline() const
{
  return m_SystolicArterialPressureBaseline == nullptr ? false : m_SystolicArterialPressureBaseline->IsValid();
}
SEScalarPressure& SEPatient::GetSystolicArterialPressureBaseline()
{
  if (m_SystolicArterialPressureBaseline == nullptr)
    m_SystolicArterialPressureBaseline = new SEScalarPressure();
  return *m_SystolicArterialPressureBaseline;
}
double SEPatient::GetSystolicArterialPressureBaseline(const PressureUnit& unit) const
{
  if (m_SystolicArterialPressureBaseline == nullptr)
    return SEScalar::dNaN();
  return m_SystolicArterialPressureBaseline->GetValue(unit);
}

bool SEPatient::HasTidalVolumeBaseline() const
{
  return m_TidalVolumeBaseline == nullptr ? false : m_TidalVolumeBaseline->IsValid();
}
SEScalarVolume& SEPatient::GetTidalVolumeBaseline()
{
  if (m_TidalVolumeBaseline == nullptr)
    m_TidalVolumeBaseline = new SEScalarVolume();
  return *m_TidalVolumeBaseline;
}
double SEPatient::GetTidalVolumeBaseline(const VolumeUnit& unit) const
{
  if (m_TidalVolumeBaseline == nullptr)
    return SEScalar::dNaN();
  return m_TidalVolumeBaseline->GetValue(unit);
}

bool SEPatient::HasTotalLungCapacity() const
{
  return m_TotalLungCapacity==nullptr?false:m_TotalLungCapacity->IsValid();
}
SEScalarVolume& SEPatient::GetTotalLungCapacity()
{
  if(m_TotalLungCapacity==nullptr)
    m_TotalLungCapacity=new SEScalarVolume();
  return *m_TotalLungCapacity;
}
double SEPatient::GetTotalLungCapacity(const VolumeUnit& unit) const
{
  if (m_TotalLungCapacity == nullptr)
    return SEScalar::dNaN();
  return m_TotalLungCapacity->GetValue(unit);
}

bool SEPatient::HasVitalCapacity() const
{
  return m_VitalCapacity == nullptr ? false : m_VitalCapacity->IsValid();
}
SEScalarVolume& SEPatient::GetVitalCapacity()
{
  if (m_VitalCapacity == nullptr)
    m_VitalCapacity = new SEScalarVolume();
  return *m_VitalCapacity;
}
double SEPatient::GetVitalCapacity(const VolumeUnit& unit) const
{
  if (m_VitalCapacity == nullptr)
    return SEScalar::dNaN();
  return m_VitalCapacity->GetValue(unit);
}