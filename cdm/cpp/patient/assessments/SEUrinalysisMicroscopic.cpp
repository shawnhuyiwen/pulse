/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/assessments/SEUrinalysisMicroscopic.h"
#include "bind/cdm/PatientAssessments.pb.h"
#include "bind/cdm/PatientAssessmentEnums.pb.h"
#include "properties/SEScalarAmount.h"

const std::string& eUrinalysis_MicroscopicObservationType_Name(eUrinalysis_MicroscopicObservationType m)
{
  return cdm::eUrinalysis_MicroscopicObservationType_Name((cdm::eUrinalysis_MicroscopicObservationType)m);
}

const std::string& eUrinalysis_MicroscopicObservationAmount_Name(eUrinalysis_MicroscopicObservationAmount m)
{
  return cdm::eUrinalysis_MicroscopicObservationAmount_Name((cdm::eUrinalysis_MicroscopicObservationAmount)m);
}

SEUrinalysisMicroscopic::SEUrinalysisMicroscopic(Logger* logger) : Loggable(logger)
{
  m_ObservationType = eUrinalysis_MicroscopicObservationType::NullObservationType;
  m_RedBloodCells = nullptr;
  m_WhiteBloodCells = nullptr;
  m_EpithelialCells = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Casts = nullptr;
  m_Crystals = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Bacteria = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Trichomonads = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Yeast = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

SEUrinalysisMicroscopic::~SEUrinalysisMicroscopic()
{
  Clear();
}

void SEUrinalysisMicroscopic::Clear()
{
  m_ObservationType = eUrinalysis_MicroscopicObservationType::NullObservationType;
  SAFE_DELETE(m_RedBloodCells);
  SAFE_DELETE(m_WhiteBloodCells);
  m_EpithelialCells = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  SAFE_DELETE(m_Casts);
  m_Crystals = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Bacteria = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Trichomonads = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Yeast = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

void SEUrinalysisMicroscopic::Load(const cdm::UrinalysisData_UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst)
{
  SEUrinalysisMicroscopic::Serialize(src, dst);
}
void SEUrinalysisMicroscopic::Serialize(const cdm::UrinalysisData_UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst)
{
  dst.Clear();
  dst.SetObservationType((eUrinalysis_MicroscopicObservationType)src.observationtype());
  dst.SetEpithelialCellsResult((eUrinalysis_MicroscopicObservationAmount)src.epithelialcells());
  dst.SetCrystalsResult((eUrinalysis_MicroscopicObservationAmount)src.crystals());
  dst.SetBacteriaResult((eUrinalysis_MicroscopicObservationAmount)src.bacteria());
  dst.SetTrichomonadsResult((eUrinalysis_MicroscopicObservationAmount)src.trichomonads());
  dst.SetYeastResult((eUrinalysis_MicroscopicObservationAmount)src.yeast());
  if (src.has_redbloodcells())
    SEScalarAmount::Load(src.redbloodcells(), dst.GetRedBloodCellsResult());
  if (src.has_whitebloodcells())
    SEScalarAmount::Load(src.whitebloodcells(), dst.GetWhiteBloodCellsResult());
  if (src.has_casts())
    SEScalarAmount::Load(src.casts(), dst.GetCastsResult());
}

cdm::UrinalysisData_UrinalysisMicroscopicData* SEUrinalysisMicroscopic::Unload(const SEUrinalysisMicroscopic& src)
{
  cdm::UrinalysisData_UrinalysisMicroscopicData* dst = new cdm::UrinalysisData_UrinalysisMicroscopicData();
  SEUrinalysisMicroscopic::Serialize(src, *dst);
  return dst;
}
void SEUrinalysisMicroscopic::Serialize(const SEUrinalysisMicroscopic& src, cdm::UrinalysisData_UrinalysisMicroscopicData& dst)
{
  if (src.HasObservationType())
    dst.set_observationtype((cdm::eUrinalysis_MicroscopicObservationType)src.m_ObservationType);
  if (src.HasEpithelialCellsResult())
    dst.set_epithelialcells((cdm::eUrinalysis_MicroscopicObservationAmount)src.m_EpithelialCells);
  if (src.HasCrystalsResult())
    dst.set_crystals((cdm::eUrinalysis_MicroscopicObservationAmount)src.m_Crystals);
  if (src.HasBacteriaResult())
    dst.set_bacteria((cdm::eUrinalysis_MicroscopicObservationAmount)src.m_Bacteria);
  if (src.HasTrichomonadsResult())
    dst.set_trichomonads((cdm::eUrinalysis_MicroscopicObservationAmount)src.m_Trichomonads);
  if (src.HasYeastResult())
    dst.set_yeast((cdm::eUrinalysis_MicroscopicObservationAmount)src.m_Yeast);
  if (src.HasRedBloodCellsResult())
    dst.set_allocated_redbloodcells(SEScalarAmount::Unload(*src.m_RedBloodCells));
  if (src.HasWhiteBloodCellsResult())
    dst.set_allocated_whitebloodcells(SEScalarAmount::Unload(*src.m_WhiteBloodCells));
  if (src.HasCastsResult())
    dst.set_allocated_casts(SEScalarAmount::Unload(*src.m_Casts));
}

bool SEUrinalysisMicroscopic::HasRedBloodCellsResult() const
{
  return m_RedBloodCells == nullptr ? false : m_RedBloodCells->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetRedBloodCellsResult()
{
  if (m_RedBloodCells == nullptr)
    m_RedBloodCells = new SEScalarAmount();
  return *m_RedBloodCells;
}
double SEUrinalysisMicroscopic::GetRedBloodCellsResult(const AmountUnit& unit) const
{
  if (!HasRedBloodCellsResult())
    return SEScalar::dNaN();
  return m_RedBloodCells->GetValue(unit);
}

bool SEUrinalysisMicroscopic::HasObservationType() const
{
  return m_ObservationType != eUrinalysis_MicroscopicObservationType::NullObservationType;
}
eUrinalysis_MicroscopicObservationType SEUrinalysisMicroscopic::GetObservationType() const
{
  return m_ObservationType;
}
void SEUrinalysisMicroscopic::SetObservationType(eUrinalysis_MicroscopicObservationType c)
{
  m_ObservationType = c;
}
void SEUrinalysisMicroscopic::InvalidateObservationType()
{
  m_ObservationType = eUrinalysis_MicroscopicObservationType::NullObservationType;
}

bool SEUrinalysisMicroscopic::HasWhiteBloodCellsResult() const
{
  return m_WhiteBloodCells == nullptr ? false : m_WhiteBloodCells->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetWhiteBloodCellsResult()
{
  if (m_WhiteBloodCells == nullptr)
    m_WhiteBloodCells = new SEScalarAmount();
  return *m_WhiteBloodCells;
}
double SEUrinalysisMicroscopic::GetWhiteBloodCellsResult(const AmountUnit& unit) const
{
  if (!HasWhiteBloodCellsResult())
    return SEScalar::dNaN();
  return m_WhiteBloodCells->GetValue(unit);
}

bool SEUrinalysisMicroscopic::HasEpithelialCellsResult() const
{
  return m_EpithelialCells != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetEpithelialCellsResult() const
{
  return m_EpithelialCells;
}
void SEUrinalysisMicroscopic::SetEpithelialCellsResult(eUrinalysis_MicroscopicObservationAmount c)
{
  m_EpithelialCells = c;
}
void SEUrinalysisMicroscopic::InvalidateEpithelialCellsResult()
{
  m_EpithelialCells = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

bool SEUrinalysisMicroscopic::HasCastsResult() const
{
  return m_Casts == nullptr ? false : m_Casts->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetCastsResult()
{
  if (m_Casts == nullptr)
    m_Casts = new SEScalarAmount();
  return *m_Casts;
}
double SEUrinalysisMicroscopic::GetCastsResult(const AmountUnit& unit) const
{
  if (!HasCastsResult())
    return SEScalar::dNaN();
  return m_Casts->GetValue(unit);
}

bool SEUrinalysisMicroscopic::HasCrystalsResult() const
{
  return m_Crystals != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetCrystalsResult() const
{
  return m_Crystals;
}
void SEUrinalysisMicroscopic::SetCrystalsResult(eUrinalysis_MicroscopicObservationAmount c)
{
  m_Crystals = c;
}
void SEUrinalysisMicroscopic::InvalidateCrystalsResult()
{
  m_Crystals = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

bool SEUrinalysisMicroscopic::HasBacteriaResult() const
{
  return m_Bacteria != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetBacteriaResult() const
{
  return m_Bacteria;
}
void SEUrinalysisMicroscopic::SetBacteriaResult(eUrinalysis_MicroscopicObservationAmount c)
{
  m_Bacteria = c;
}
void SEUrinalysisMicroscopic::InvalidateBacteriaResult()
{
  m_Bacteria = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

bool SEUrinalysisMicroscopic::HasTrichomonadsResult() const
{
  return m_Trichomonads != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetTrichomonadsResult() const
{
  return m_Trichomonads;
}
void SEUrinalysisMicroscopic::SetTrichomonadsResult(eUrinalysis_MicroscopicObservationAmount c)
{
  m_Trichomonads = c;
}
void SEUrinalysisMicroscopic::InvalidateTrichomonadsResult()
{
  m_Trichomonads = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

bool SEUrinalysisMicroscopic::HasYeastResult() const
{
  return m_Yeast != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetYeastResult() const
{
  return m_Yeast;
}
void SEUrinalysisMicroscopic::SetYeastResult(eUrinalysis_MicroscopicObservationAmount c)
{
  m_Yeast = c;
}
void SEUrinalysisMicroscopic::InvalidateYeastResult()
{
  m_Yeast = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}