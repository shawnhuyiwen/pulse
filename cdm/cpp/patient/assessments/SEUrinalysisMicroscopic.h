/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/assessments/SEPatientAssessment.h"
CDM_BIND_DECL(UrinalysisData_UrinalysisMicroscopicData)
#include "bind/cdm/PatientAssessmentEnums.pb.h"

class CDM_DECL SEUrinalysisMicroscopic : public Loggable
{
  friend class SEUrinalysis;
protected:
  SEUrinalysisMicroscopic(Logger* logger);
public:
  virtual ~SEUrinalysisMicroscopic();

  virtual void Clear();

  static void Load(const cdm::UrinalysisData_UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst);
  static cdm::UrinalysisData_UrinalysisMicroscopicData* Unload(const SEUrinalysisMicroscopic& src);
protected:
  static void Serialize(const cdm::UrinalysisData_UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst);
  static void Serialize(const SEUrinalysisMicroscopic& src, cdm::UrinalysisData_UrinalysisMicroscopicData& dst);

public:

  virtual bool HasObservationType() const;  
  virtual cdm::eUrinalysis_MicroscopicObservationType GetObservationType() const;
  virtual void SetObservationType(cdm::eUrinalysis_MicroscopicObservationType p);
  virtual void InvalidateObservationType();

  virtual bool HasRedBloodCellsResult() const;
  virtual SEScalarAmount& GetRedBloodCellsResult();
  virtual double GetRedBloodCellsResult(const AmountUnit& unit) const;

  virtual bool HasWhiteBloodCellsResult() const;
  virtual SEScalarAmount& GetWhiteBloodCellsResult();
  virtual double GetWhiteBloodCellsResult(const AmountUnit& unit) const;

  virtual bool HasEpithelialCellsResult() const;
  virtual cdm::eUrinalysis_MicroscopicObservationAmount GetEpithelialCellsResult() const;
  virtual void SetEpithelialCellsResult(cdm::eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateEpithelialCellsResult();

  virtual bool HasCastsResult() const;
  virtual SEScalarAmount& GetCastsResult();
  virtual double GetCastsResult(const AmountUnit& unit) const;

  virtual bool HasCrystalsResult() const;
  virtual cdm::eUrinalysis_MicroscopicObservationAmount GetCrystalsResult() const;
  virtual void SetCrystalsResult(cdm::eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateCrystalsResult();

  virtual bool HasBacteriaResult() const;
  virtual cdm::eUrinalysis_MicroscopicObservationAmount GetBacteriaResult() const;
  virtual void SetBacteriaResult(cdm::eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateBacteriaResult();

  virtual bool HasTrichomonadsResult() const;
  virtual cdm::eUrinalysis_MicroscopicObservationAmount GetTrichomonadsResult() const;
  virtual void SetTrichomonadsResult(cdm::eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateTrichomonadsResult();

  virtual bool HasYeastResult() const;
  virtual cdm::eUrinalysis_MicroscopicObservationAmount GetYeastResult() const;
  virtual void SetYeastResult(cdm::eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateYeastResult();

protected:

  cdm::eUrinalysis_MicroscopicObservationType   m_ObservationType;
  SEScalarAmount*                               m_RedBloodCells;
  SEScalarAmount*                               m_WhiteBloodCells;
  cdm::eUrinalysis_MicroscopicObservationAmount m_EpithelialCells;
  SEScalarAmount*                               m_Casts;
  cdm::eUrinalysis_MicroscopicObservationAmount m_Crystals;
  cdm::eUrinalysis_MicroscopicObservationAmount m_Bacteria;
  cdm::eUrinalysis_MicroscopicObservationAmount m_Trichomonads;
  cdm::eUrinalysis_MicroscopicObservationAmount m_Yeast;
};  