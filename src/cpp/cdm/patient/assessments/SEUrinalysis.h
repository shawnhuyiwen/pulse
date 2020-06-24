/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/assessments/SEPatientAssessment.h"
class SEUrinalysisMicroscopic;

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_PresenceIndicator {
  NullPresence = 0,
  Positive,
  Negative
};
extern const std::string& eUrinalysis_PresenceIndicator_Name(eUrinalysis_PresenceIndicator m);

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_ClarityIndicator {
  NullClarity = 0,
  Translucent,
  SlightlyCloudy,
  Cloudy,
  Turbid
};
extern const std::string& eUrinalysis_ClarityIndicator_Name(eUrinalysis_ClarityIndicator m);

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_UrineColor {
  NullColor = 0,
  PaleYellow,
  Yellow,
  DarkYellow
};
extern const std::string& eUrinalysis_UrineColor_Name(eUrinalysis_UrineColor m);


class CDM_DECL SEUrinalysis : public SEPatientAssessment
{
  friend class PBPatientAssessment;//friend the serialization class
public:

  SEUrinalysis(Logger* logger);
  virtual ~SEUrinalysis();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;

  virtual bool HasColorResult() const;
  virtual eUrinalysis_UrineColor GetColorResult() const;
  virtual void SetColorResult(eUrinalysis_UrineColor color);
  virtual void InvalidateColorResult();

  virtual bool HasAppearanceResult() const;
  virtual eUrinalysis_ClarityIndicator GetAppearanceResult() const;
  virtual void SetAppearanceResult(eUrinalysis_ClarityIndicator c);
  virtual void InvalidateAppearanceResult();

  virtual bool HasGlucoseResult() const;
  virtual eUrinalysis_PresenceIndicator GetGlucoseResult() const;
  virtual void SetGlucoseResult(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateGlucoseResult();

  virtual bool HasKetoneResult() const;
  virtual eUrinalysis_PresenceIndicator GetKetoneResult() const;
  virtual void SetKetoneResult(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateKetoneResult();
  
  virtual bool HasBilirubinResult() const;
  virtual SEScalar& GetBilirubinResult();
  virtual double GetBilirubinResult() const;

  virtual bool HasSpecificGravityResult() const;
  virtual SEScalar& GetSpecificGravityResult();
  virtual double GetSpecificGravityResult() const;

  virtual bool HasBloodResult() const;
  virtual eUrinalysis_PresenceIndicator GetBloodResult() const;
  virtual void SetBloodResult(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateBloodResult();

  virtual bool HasPHResult() const;
  virtual SEScalar& GetPHResult();
  virtual double GetPHResult() const;

  virtual bool HasProteinResult() const;
  virtual eUrinalysis_PresenceIndicator GetProteinResult() const;
  virtual void SetProteinResult(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateProteinResult();

  virtual bool HasUrobilinogenResult() const;
  virtual SEScalarMassPerVolume& GetUrobilinogenResult();
  virtual double GetUrobilinogenResult(const MassPerVolumeUnit& unit) const;

  virtual bool HasNitriteResult() const;
  virtual eUrinalysis_PresenceIndicator GetNitriteResult() const;
  virtual void SetNitriteResult(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateNitriteResult();

  virtual bool HasLeukocyteEsteraseResult() const;
  virtual eUrinalysis_PresenceIndicator GetLeukocyteEsteraseResult() const;
  virtual void SetLeukocyteEsteraseResult(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateLeukocyteEsteraseResult();

  virtual bool HasMicroscopicResult() const;
  virtual SEUrinalysisMicroscopic& GetMicroscopicResult();
  virtual const SEUrinalysisMicroscopic* GetMicroscopicResult() const;
  virtual void RemoveMicroscopicResult();
  
protected:

  eUrinalysis_UrineColor        m_Color;
  eUrinalysis_ClarityIndicator  m_Appearance;
  eUrinalysis_PresenceIndicator m_Glucose;
  eUrinalysis_PresenceIndicator m_Ketone;
  SEScalar*                     m_Bilirubin;
  SEScalar*                     m_SpecificGravity;
  eUrinalysis_PresenceIndicator m_Blood;
  SEScalar*                     m_pH;
  eUrinalysis_PresenceIndicator m_Protein;
  SEScalarMassPerVolume*        m_Urobilinogen;
  eUrinalysis_PresenceIndicator m_Nitrite;
  eUrinalysis_PresenceIndicator m_LeukocyteEsterase;

  SEUrinalysisMicroscopic*      m_Microscopic;
};  