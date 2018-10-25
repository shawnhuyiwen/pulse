/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/assessments/SEPatientAssessment.h"

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_MicroscopicObservationType {
  NullObservationType = 0,
  LowPowerField,
  HighPowerField
};
extern const std::string& eUrinalysis_MicroscopicObservationType_Name(eUrinalysis_MicroscopicObservationType m);

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_MicroscopicObservationAmount {
  NullObservationAmount = 0,
  Few,
  Moderate,
  Many
};
extern const std::string& eUrinalysis_MicroscopicObservationAmount_Name(eUrinalysis_MicroscopicObservationAmount m);


class CDM_DECL SEUrinalysisMicroscopic : public Loggable
{
  friend class PBPatientAssessment;//friend the serialization class
  friend class SEUrinalysis;
protected:
  SEUrinalysisMicroscopic(Logger* logger);
public:
  virtual ~SEUrinalysisMicroscopic();

  virtual void Clear();

  virtual bool HasObservationType() const;  
  virtual eUrinalysis_MicroscopicObservationType GetObservationType() const;
  virtual void SetObservationType(eUrinalysis_MicroscopicObservationType p);
  virtual void InvalidateObservationType();

  virtual bool HasRedBloodCellsResult() const;
  virtual SEScalarAmount& GetRedBloodCellsResult();
  virtual double GetRedBloodCellsResult(const AmountUnit& unit) const;

  virtual bool HasWhiteBloodCellsResult() const;
  virtual SEScalarAmount& GetWhiteBloodCellsResult();
  virtual double GetWhiteBloodCellsResult(const AmountUnit& unit) const;

  virtual bool HasEpithelialCellsResult() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetEpithelialCellsResult() const;
  virtual void SetEpithelialCellsResult(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateEpithelialCellsResult();

  virtual bool HasCastsResult() const;
  virtual SEScalarAmount& GetCastsResult();
  virtual double GetCastsResult(const AmountUnit& unit) const;

  virtual bool HasCrystalsResult() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetCrystalsResult() const;
  virtual void SetCrystalsResult(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateCrystalsResult();

  virtual bool HasBacteriaResult() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetBacteriaResult() const;
  virtual void SetBacteriaResult(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateBacteriaResult();

  virtual bool HasTrichomonadsResult() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetTrichomonadsResult() const;
  virtual void SetTrichomonadsResult(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateTrichomonadsResult();

  virtual bool HasYeastResult() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetYeastResult() const;
  virtual void SetYeastResult(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateYeastResult();

protected:

  eUrinalysis_MicroscopicObservationType   m_ObservationType;
  SEScalarAmount*                          m_RedBloodCells;
  SEScalarAmount*                          m_WhiteBloodCells;
  eUrinalysis_MicroscopicObservationAmount m_EpithelialCells;
  SEScalarAmount*                          m_Casts;
  eUrinalysis_MicroscopicObservationAmount m_Crystals;
  eUrinalysis_MicroscopicObservationAmount m_Bacteria;
  eUrinalysis_MicroscopicObservationAmount m_Trichomonads;
  eUrinalysis_MicroscopicObservationAmount m_Yeast;
};  