/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/assessments/SEPatientAssessment.h"
class SEUrinalysisMicroscopic;
CDM_BIND_DECL(UrinalysisData)
#include "bind/cdm/PatientAssessmentEnums.pb.h"

class CDM_DECL SEUrinalysis : public SEPatientAssessment
{
public:

  SEUrinalysis(Logger* logger);
  virtual ~SEUrinalysis();

  virtual void Clear();

  virtual std::string Save() const;
  virtual void SaveFile(const std::string& filename) const;

  static void Load(const cdm::UrinalysisData& src, SEUrinalysis& dst);
  static cdm::UrinalysisData* Unload(const SEUrinalysis& src);
protected:
  static void Serialize(const cdm::UrinalysisData& src, SEUrinalysis& dst);
  static void Serialize(const SEUrinalysis& src, cdm::UrinalysisData& dst);

public:

  virtual bool HasColorResult() const;
  virtual cdm::eUrinalysis_UrineColor GetColorResult() const;
  virtual void SetColorResult(cdm::eUrinalysis_UrineColor color);
  virtual void InvalidateColorResult();

  virtual bool HasAppearanceResult() const;
  virtual cdm::eUrinalysis_ClarityIndicator GetAppearanceResult() const;
  virtual void SetAppearanceResult(cdm::eUrinalysis_ClarityIndicator c);
  virtual void InvalidateAppearanceResult();

  virtual bool HasGlucoseResult() const;
  virtual cdm::eUrinalysis_PresenceIndicator GetGlucoseResult() const;
  virtual void SetGlucoseResult(cdm::eUrinalysis_PresenceIndicator p);
  virtual void InvalidateGlucoseResult();

  virtual bool HasKetoneResult() const;
  virtual cdm::eUrinalysis_PresenceIndicator GetKetoneResult() const;
  virtual void SetKetoneResult(cdm::eUrinalysis_PresenceIndicator p);
  virtual void InvalidateKetoneResult();
  
  virtual bool HasBilirubinResult() const;
  virtual SEScalar& GetBilirubinResult();
  virtual double GetBilirubinResult() const;

  virtual bool HasSpecificGravityResult() const;
  virtual SEScalar& GetSpecificGravityResult();
  virtual double GetSpecificGravityResult() const;

  virtual bool HasBloodResult() const;
  virtual cdm::eUrinalysis_PresenceIndicator GetBloodResult() const;
  virtual void SetBloodResult(cdm::eUrinalysis_PresenceIndicator p);
  virtual void InvalidateBloodResult();

  virtual bool HasPHResult() const;
  virtual SEScalar& GetPHResult();
  virtual double GetPHResult() const;

  virtual bool HasProteinResult() const;
  virtual cdm::eUrinalysis_PresenceIndicator GetProteinResult() const;
  virtual void SetProteinResult(cdm::eUrinalysis_PresenceIndicator p);
  virtual void InvalidateProteinResult();

  virtual bool HasUrobilinogenResult() const;
  virtual SEScalarMassPerVolume& GetUrobilinogenResult();
  virtual double GetUrobilinogenResult(const MassPerVolumeUnit& unit) const;

  virtual bool HasNitriteResult() const;
  virtual cdm::eUrinalysis_PresenceIndicator GetNitriteResult() const;
  virtual void SetNitriteResult(cdm::eUrinalysis_PresenceIndicator p);
  virtual void InvalidateNitriteResult();

  virtual bool HasLeukocyteEsteraseResult() const;
  virtual cdm::eUrinalysis_PresenceIndicator GetLeukocyteEsteraseResult() const;
  virtual void SetLeukocyteEsteraseResult(cdm::eUrinalysis_PresenceIndicator p);
  virtual void InvalidateLeukocyteEsteraseResult();

  virtual bool HasMicroscopicResult() const;
  virtual SEUrinalysisMicroscopic& GetMicroscopicResult();
  virtual const SEUrinalysisMicroscopic* GetMicroscopicResult() const;
  virtual void RemoveMicroscopicResult();
  
protected:

  cdm::eUrinalysis_UrineColor        m_Color;
  cdm::eUrinalysis_ClarityIndicator  m_Appearance;
  cdm::eUrinalysis_PresenceIndicator m_Glucose;
  cdm::eUrinalysis_PresenceIndicator m_Ketone;
  SEScalar*                              m_Bilirubin;
  SEScalar*                              m_SpecificGravity;
  cdm::eUrinalysis_PresenceIndicator m_Blood;
  SEScalar*                              m_pH;
  cdm::eUrinalysis_PresenceIndicator m_Protein;
  SEScalarMassPerVolume*                 m_Urobilinogen;
  cdm::eUrinalysis_PresenceIndicator m_Nitrite;
  cdm::eUrinalysis_PresenceIndicator m_LeukocyteEsterase;

  SEUrinalysisMicroscopic*               m_Microscopic;
};  