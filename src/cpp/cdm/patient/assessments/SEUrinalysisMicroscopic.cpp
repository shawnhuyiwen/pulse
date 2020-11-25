/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/assessments/SEUrinalysisMicroscopic.h"
#include "properties/SEScalarAmount.h"

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

void SEUrinalysisMicroscopic::Clear()
{
  m_ObservationType = eUrinalysis_MicroscopicObservationType::NullObservationType;
  INVALIDATE_PROPERTY(m_RedBloodCells);
  INVALIDATE_PROPERTY(m_WhiteBloodCells);
  m_EpithelialCells = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  INVALIDATE_PROPERTY(m_Casts);
  m_Crystals = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Bacteria = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Trichomonads = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
  m_Yeast = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
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