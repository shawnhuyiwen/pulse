/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/assessments/SEUrinalysis.h"
#include "cdm/patient/assessments/SEUrinalysisMicroscopic.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarMassPerTime.h"
#include "cdm/properties/SEScalarAmountPerVolume.h"
#include "cdm/properties/SEScalarOsmolarity.h"
#include "cdm/properties/SEScalarOsmolality.h"
#include "cdm/io/protobuf/PBPatientAssessments.h"

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

void SEUrinalysis::Clear()
{
  SEPatientAssessment::Clear();
  m_Color = eUrinalysis_UrineColor::NullColor;
  m_Appearance = eUrinalysis_ClarityIndicator::NullClarity;
  m_Glucose = eUrinalysis_PresenceIndicator::NullPresence;
  m_Ketone = eUrinalysis_PresenceIndicator::NullPresence;
  INVALIDATE_PROPERTY(m_Bilirubin);
  INVALIDATE_PROPERTY(m_SpecificGravity);
  m_Blood = eUrinalysis_PresenceIndicator::NullPresence;
  INVALIDATE_PROPERTY(m_pH);
  m_Protein = eUrinalysis_PresenceIndicator::NullPresence;
  INVALIDATE_PROPERTY(m_Urobilinogen);
  m_Nitrite = eUrinalysis_PresenceIndicator::NullPresence;
  m_LeukocyteEsterase = eUrinalysis_PresenceIndicator::NullPresence;

  if (m_Microscopic)
    m_Microscopic->Clear();
}

bool SEUrinalysis::SerializeToString(std::string& output, eSerializationFormat m) const
{
  return PBPatientAssessment::SerializeToString(*this, output, m);
}
bool SEUrinalysis::SerializeToFile(const std::string& filename) const
{
  return PBPatientAssessment::SerializeToFile(*this, filename);
}

bool SEUrinalysis::HasColor() const
{
  return m_Color != eUrinalysis_UrineColor::NullColor;
}
eUrinalysis_UrineColor SEUrinalysis::GetColor() const
{
  return m_Color;
}
void SEUrinalysis::SetColor(eUrinalysis_UrineColor color)
{
  m_Color = color;
}
void SEUrinalysis::InvalidateColor()
{
  m_Color = eUrinalysis_UrineColor::NullColor;
}

bool SEUrinalysis::HasAppearance() const
{
  return m_Appearance != eUrinalysis_ClarityIndicator::NullClarity;
}
eUrinalysis_ClarityIndicator SEUrinalysis::GetAppearance() const
{
  return m_Appearance;
}
void SEUrinalysis::SetAppearance(eUrinalysis_ClarityIndicator c)
{
  m_Appearance = c;
}
void SEUrinalysis::InvalidateAppearance()
{
  m_Appearance = eUrinalysis_ClarityIndicator::NullClarity;
}

bool SEUrinalysis::HasGlucose() const
{
  return m_Glucose != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetGlucose() const
{
  return m_Glucose;
}
void SEUrinalysis::SetGlucose(eUrinalysis_PresenceIndicator p)
{
  m_Glucose = p;
}
void SEUrinalysis::InvalidateGlucose()
{
  m_Glucose = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasKetone() const
{
  return m_Ketone != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetKetone() const
{
  return m_Ketone;
}
void SEUrinalysis::SetKetone(eUrinalysis_PresenceIndicator p)
{
  m_Ketone = p;
}
void SEUrinalysis::InvalidateKetone()
{
  m_Ketone = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasBilirubin() const
{
  return m_Bilirubin == nullptr ? false : m_Bilirubin->IsValid();
}
SEScalar& SEUrinalysis::GetBilirubin()
{
  if (m_Bilirubin == nullptr)
    m_Bilirubin = new SEScalar();
  return *m_Bilirubin;
}
double SEUrinalysis::GetBilirubin() const
{
  if (!HasBilirubin())
    return SEScalar::dNaN();
  return m_Bilirubin->GetValue();
}

bool SEUrinalysis::HasSpecificGravity() const
{
  return m_SpecificGravity == nullptr ? false : m_SpecificGravity->IsValid();
}
SEScalar& SEUrinalysis::GetSpecificGravity()
{
  if (m_SpecificGravity == nullptr)
    m_SpecificGravity = new SEScalar();
  return *m_SpecificGravity;
}
double SEUrinalysis::GetSpecificGravity() const
{
  if (!HasSpecificGravity())
    return SEScalar::dNaN();
  return m_SpecificGravity->GetValue();
}

bool SEUrinalysis::HasBlood() const
{
  return m_Blood != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetBlood() const
{
  return m_Blood;
}
void SEUrinalysis::SetBlood(eUrinalysis_PresenceIndicator p)
{
  m_Blood = p;
}
void SEUrinalysis::InvalidateBlood()
{
  m_Blood = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasPH() const
{
  return m_pH == nullptr ? false : m_pH->IsValid();
}
SEScalar& SEUrinalysis::GetPH()
{
  if (m_pH == nullptr)
    m_pH = new SEScalar();
  return *m_pH;
}
double SEUrinalysis::GetPH() const
{
  if (!HasPH())
    return SEScalar::dNaN();
  return m_pH->GetValue();
}

bool SEUrinalysis::HasProtein() const
{
  return m_Protein != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetProtein() const
{
  return m_Protein;
}
void SEUrinalysis::SetProtein(eUrinalysis_PresenceIndicator p)
{
  m_Protein = p;
}
void SEUrinalysis::InvalidateProtein()
{
  m_Protein = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasUrobilinogen() const
{
  return m_Urobilinogen == nullptr ? false : m_Urobilinogen->IsValid();
}
SEScalarMassPerVolume& SEUrinalysis::GetUrobilinogen()
{
  if (m_Urobilinogen == nullptr)
    m_Urobilinogen = new SEScalarMassPerVolume();
  return *m_Urobilinogen;
}
double SEUrinalysis::GetUrobilinogen(const MassPerVolumeUnit& unit) const
{
  if (!HasUrobilinogen())
    return SEScalar::dNaN();
  return m_Urobilinogen->GetValue(unit);
}

bool SEUrinalysis::HasNitrite() const
{
  return m_Nitrite != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetNitrite() const
{
  return m_Nitrite;
}
void SEUrinalysis::SetNitrite(eUrinalysis_PresenceIndicator p)
{
  m_Nitrite = p;
}
void SEUrinalysis::InvalidateNitrite()
{
  m_Nitrite = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasLeukocyteEsterase() const
{
  return m_LeukocyteEsterase != eUrinalysis_PresenceIndicator::NullPresence;
}
eUrinalysis_PresenceIndicator SEUrinalysis::GetLeukocyteEsterase() const
{
  return m_LeukocyteEsterase;
}
void SEUrinalysis::SetLeukocyteEsterase(eUrinalysis_PresenceIndicator p)
{
  m_LeukocyteEsterase = p;
}
void SEUrinalysis::InvalidateLeukocyteEsterase()
{
  m_LeukocyteEsterase = eUrinalysis_PresenceIndicator::NullPresence;
}

bool SEUrinalysis::HasMicroscopic() const
{
  return m_Microscopic != nullptr;
}
SEUrinalysisMicroscopic& SEUrinalysis::GetMicroscopic()
{
  if (m_Microscopic == nullptr)
    m_Microscopic = new SEUrinalysisMicroscopic(GetLogger());
  return *m_Microscopic;
}
const SEUrinalysisMicroscopic* SEUrinalysis::GetMicroscopic() const
{
  return m_Microscopic;
}
void SEUrinalysis::RemoveMicroscopic()
{
  SAFE_DELETE(m_Microscopic);
}