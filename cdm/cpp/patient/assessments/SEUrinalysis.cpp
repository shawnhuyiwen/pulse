/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/assessments/SEUrinalysis.h"
#include "patient/assessments/SEUrinalysisMicroscopic.h"
#include "bind/cdm/PatientAssessments.pb.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarMassPerTime.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalarOsmolarity.h"
#include "properties/SEScalarOsmolality.h"
#include <google/protobuf/text_format.h>

SEUrinalysis::SEUrinalysis(Logger* logger) : SEPatientAssessment(logger)
{
  m_Color = eUrinalysis_UrineColor::NullColor;
  m_Appearance = eUrinalysis_ClarityIndicator::NullClarity;
  m_Glucose = eUrinalysis_PresenceIndicator::NullPresence;
  m_Ketone = eUrinalysis_PresenceIndicator::NullPresence;
  m_Bilirubin = nullptr;
  m_SpecificGravity = nullptr;
  m_Blood = eUrinalysis_PresenceIndicator::NullPresence;
  m_pH = nullptr;
  m_Protein = eUrinalysis_PresenceIndicator::NullPresence;
  m_Urobilinogen = nullptr;
  m_Nitrite = eUrinalysis_PresenceIndicator::NullPresence;
  m_LeukocyteEsterase = eUrinalysis_PresenceIndicator::NullPresence;

  m_Microscopic = nullptr;
}

SEUrinalysis::~SEUrinalysis()
{
  Clear();
}

void SEUrinalysis::Clear()
{
  SEPatientAssessment::Clear();
  m_Color = eUrinalysis_UrineColor::NullColor;
  m_Appearance = eUrinalysis_ClarityIndicator::NullClarity;
  m_Glucose = eUrinalysis_PresenceIndicator::NullPresence;
  m_Ketone = eUrinalysis_PresenceIndicator::NullPresence;
  SAFE_DELETE(m_Bilirubin);
  SAFE_DELETE(m_SpecificGravity);
  m_Blood = eUrinalysis_PresenceIndicator::NullPresence;
  SAFE_DELETE(m_pH);
  m_Protein = eUrinalysis_PresenceIndicator::NullPresence;
  SAFE_DELETE(m_Urobilinogen);
  m_Nitrite = eUrinalysis_PresenceIndicator::NullPresence;
  m_LeukocyteEsterase = eUrinalysis_PresenceIndicator::NullPresence;

  SAFE_DELETE(m_Microscopic);
}

std::string SEUrinalysis::Save() const
{
  std::string content;
  cdm::UrinalysisData* src = SEUrinalysis::Unload(*this);
  google::protobuf::TextFormat::PrintToString(*src, &content);
  return content;
}
void SEUrinalysis::SaveFile(const std::string& filename) const
{
  std::string content;
  cdm::UrinalysisData* src = SEUrinalysis::Unload(*this);
  google::protobuf::TextFormat::PrintToString(*src, &content);
  std::ofstream ascii_ostream(filename, std::ios::out | std::ios::trunc);
  ascii_ostream << content;
  ascii_ostream.flush();
  ascii_ostream.close();
  delete src;
}

void SEUrinalysis::Load(const cdm::UrinalysisData& src, SEUrinalysis& dst)
{
  SEUrinalysis::Serialize(src, dst);
}
void SEUrinalysis::Serialize(const cdm::UrinalysisData& src, SEUrinalysis& dst)
{
  SEPatientAssessment::Serialize(src.patientassessment(), dst);
  dst.SetColorResult((eUrinalysis_UrineColor)src.color());
  dst.SetAppearanceResult((eUrinalysis_ClarityIndicator)src.appearance());
  dst.SetGlucoseResult((eUrinalysis_PresenceIndicator)src.glucose());
  dst.SetKetoneResult((eUrinalysis_PresenceIndicator)src.ketone());
  dst.SetBloodResult((eUrinalysis_PresenceIndicator)src.blood());
  dst.SetProteinResult((eUrinalysis_PresenceIndicator)src.protein());
  dst.SetNitriteResult((eUrinalysis_PresenceIndicator)src.nitrite());
  dst.SetLeukocyteEsteraseResult((eUrinalysis_PresenceIndicator)src.leukocyteesterase());
  if (src.has_bilirubin())
    SEScalar::Load(src.bilirubin(), dst.GetBilirubinResult());
  if (src.has_specificgravity())
    SEScalar::Load(src.specificgravity(), dst.GetSpecificGravityResult());
  if (src.has_ph())
    SEScalar::Load(src.ph(), dst.GetPHResult());
  if (src.has_urobilinogen())
    SEScalarMassPerVolume::Load(src.urobilinogen(), dst.GetUrobilinogenResult());
  if (src.has_microscopic())
    SEUrinalysisMicroscopic::Load(src.microscopic(), dst.GetMicroscopicResult());
}

cdm::UrinalysisData* SEUrinalysis::Unload(const SEUrinalysis& src)
{
  cdm::UrinalysisData* dst = new cdm::UrinalysisData();
  SEUrinalysis::Serialize(src, *dst);
  return dst;
}
void SEUrinalysis::Serialize(const SEUrinalysis& src, cdm::UrinalysisData& dst)
{
  SEPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasColorResult())
    dst.set_color((cdm::eUrinalysis_UrineColor)src.m_Color);
  if (src.HasAppearanceResult())
    dst.set_appearance((cdm::eUrinalysis_ClarityIndicator)src.m_Appearance);
  if (src.HasGlucoseResult())
    dst.set_glucose((cdm::eUrinalysis_PresenceIndicator)src.m_Glucose);
  if (src.HasKetoneResult())
    dst.set_ketone((cdm::eUrinalysis_PresenceIndicator)src.m_Ketone);
  if (src.HasBloodResult())
    dst.set_blood((cdm::eUrinalysis_PresenceIndicator)src.m_Blood);
  if (src.HasProteinResult())
    dst.set_protein((cdm::eUrinalysis_PresenceIndicator)src.m_Protein);
  if (src.HasNitriteResult())
    dst.set_nitrite((cdm::eUrinalysis_PresenceIndicator)src.m_Nitrite);
  if (src.HasBilirubinResult())
    dst.set_allocated_bilirubin(SEScalar::Unload(*src.m_Bilirubin));
  if (src.HasSpecificGravityResult())
    dst.set_allocated_specificgravity(SEScalar::Unload(*src.m_SpecificGravity));
  if (src.HasPHResult())
    dst.set_allocated_ph(SEScalar::Unload(*src.m_pH));
  if (src.HasUrobilinogenResult())
    dst.set_allocated_urobilinogen(SEScalarMassPerVolume::Unload(*src.m_Urobilinogen));
  if (src.HasMicroscopicResult())
    dst.set_allocated_microscopic(SEUrinalysisMicroscopic::Unload(*src.m_Microscopic));
}

bool SEUrinalysis::HasColorResult() const
{
  return m_Color != eUrinalysis_UrineColor::NullColor;
}
eUrinalysis_UrineColor SEUrinalysis::GetColorResult() const
{
  return m_Color;
}
void SEUrinalysis::SetColorResult(eUrinalysis_UrineColor color)
{
  m_Color = color;
}
void SEUrinalysis::InvalidateColorResult()
{
  m_Color = eUrinalysis_UrineColor::NullColor;
}

bool SEUrinalysis::HasAppearanceResult() const
{
  return m_Appearance != eUrinalysis_ClarityIndicator::NullClarity;
}
eUrinalysis_ClarityIndicator SEUrinalysis::GetAppearanceResult() const
{
  return m_Appearance;
}
void SEUrinalysis::SetAppearanceResult(eUrinalysis_ClarityIndicator c)
{
  m_Appearance = c;
}
void SEUrinalysis::InvalidateAppearanceResult()
{
  m_Appearance = eUrinalysis_ClarityIndicator::NullClarity;
}

bool SEUrinalysis::HasGlucoseResult() const
{
  return m_Glucose != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetGlucoseResult() const
{
  return m_Glucose;
}
void SEUrinalysis::SetGlucoseResult(eUrinalysis_PresenceIndicator p)
{
  m_Glucose = p;
}
void SEUrinalysis::InvalidateGlucoseResult()
{
  m_Glucose = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasKetoneResult() const
{
  return m_Ketone != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetKetoneResult() const
{
  return m_Ketone;
}
void SEUrinalysis::SetKetoneResult(eUrinalysis_PresenceIndicator p)
{
  m_Ketone = p;
}
void SEUrinalysis::InvalidateKetoneResult()
{
  m_Ketone = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasBilirubinResult() const
{
  return m_Bilirubin == nullptr ? false : m_Bilirubin->IsValid();
}
SEScalar& SEUrinalysis::GetBilirubinResult()
{
  if (m_Bilirubin == nullptr)
    m_Bilirubin = new SEScalar();
  return *m_Bilirubin;
}
double SEUrinalysis::GetBilirubinResult() const
{
  if (!HasBilirubinResult())
    return SEScalar::dNaN();
  return m_Bilirubin->GetValue();
}

bool SEUrinalysis::HasSpecificGravityResult() const
{
  return m_SpecificGravity == nullptr ? false : m_SpecificGravity->IsValid();
}
SEScalar& SEUrinalysis::GetSpecificGravityResult()
{
  if (m_SpecificGravity == nullptr)
    m_SpecificGravity = new SEScalar();
  return *m_SpecificGravity;
}
double SEUrinalysis::GetSpecificGravityResult() const
{
  if (!HasSpecificGravityResult())
    return SEScalar::dNaN();
  return m_SpecificGravity->GetValue();
}

bool SEUrinalysis::HasBloodResult() const
{
  return m_Blood != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetBloodResult() const
{
  return m_Blood;
}
void SEUrinalysis::SetBloodResult(eUrinalysis_PresenceIndicator p)
{
  m_Blood = p;
}
void SEUrinalysis::InvalidateBloodResult()
{
  m_Blood = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasPHResult() const
{
  return m_pH == nullptr ? false : m_pH->IsValid();
}
SEScalar& SEUrinalysis::GetPHResult()
{
  if (m_pH == nullptr)
    m_pH = new SEScalar();
  return *m_pH;
}
double SEUrinalysis::GetPHResult() const
{
  if (!HasPHResult())
    return SEScalar::dNaN();
  return m_pH->GetValue();
}

bool SEUrinalysis::HasProteinResult() const
{
  return m_Protein != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetProteinResult() const
{
  return m_Protein;
}
void SEUrinalysis::SetProteinResult(eUrinalysis_PresenceIndicator p)
{
  m_Protein = p;
}
void SEUrinalysis::InvalidateProteinResult()
{
  m_Protein = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasUrobilinogenResult() const
{
  return m_Urobilinogen == nullptr ? false : m_Urobilinogen->IsValid();
}
SEScalarMassPerVolume& SEUrinalysis::GetUrobilinogenResult()
{
  if (m_Urobilinogen == nullptr)
    m_Urobilinogen = new SEScalarMassPerVolume();
  return *m_Urobilinogen;
}
double SEUrinalysis::GetUrobilinogenResult(const MassPerVolumeUnit& unit) const
{
  if (!HasUrobilinogenResult())
    return SEScalar::dNaN();
  return m_Urobilinogen->GetValue(unit);
}

bool SEUrinalysis::HasNitriteResult() const
{
  return m_Nitrite != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetNitriteResult() const
{
  return m_Nitrite;
}
void SEUrinalysis::SetNitriteResult(eUrinalysis_PresenceIndicator p)
{
  m_Nitrite = p;
}
void SEUrinalysis::InvalidateNitriteResult()
{
  m_Nitrite = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasLeukocyteEsteraseResult() const
{
  return m_LeukocyteEsterase != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetLeukocyteEsteraseResult() const
{
  return m_LeukocyteEsterase;
}
void SEUrinalysis::SetLeukocyteEsteraseResult(eUrinalysis_PresenceIndicator p)
{
  m_LeukocyteEsterase = p;
}
void SEUrinalysis::InvalidateLeukocyteEsteraseResult()
{
  m_LeukocyteEsterase = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasMicroscopicResult() const
{
  return m_Microscopic != nullptr;
}
SEUrinalysisMicroscopic& SEUrinalysis::GetMicroscopicResult()
{
  if (m_Microscopic == nullptr)
    m_Microscopic = new SEUrinalysisMicroscopic(GetLogger());
  return *m_Microscopic;
}
const SEUrinalysisMicroscopic* SEUrinalysis::GetMicroscopicResult() const
{
  return m_Microscopic;
}
void SEUrinalysis::RemoveMicroscopicResult()
{
  SAFE_DELETE(m_Microscopic);
}