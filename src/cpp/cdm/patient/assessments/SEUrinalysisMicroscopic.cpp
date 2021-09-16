/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/assessments/SEUrinalysisMicroscopic.h"
#include "cdm/properties/SEScalarAmount.h"

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

bool SEUrinalysisMicroscopic::HasRedBloodCells() const
{
  return m_RedBloodCells == nullptr ? false : m_RedBloodCells->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetRedBloodCells()
{
  if (m_RedBloodCells == nullptr)
    m_RedBloodCells = new SEScalarAmount();
  return *m_RedBloodCells;
}
double SEUrinalysisMicroscopic::GetRedBloodCells(const AmountUnit& unit) const
{
  if (!HasRedBloodCells())
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

bool SEUrinalysisMicroscopic::HasWhiteBloodCells() const
{
  return m_WhiteBloodCells == nullptr ? false : m_WhiteBloodCells->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetWhiteBloodCells()
{
  if (m_WhiteBloodCells == nullptr)
    m_WhiteBloodCells = new SEScalarAmount();
  return *m_WhiteBloodCells;
}
double SEUrinalysisMicroscopic::GetWhiteBloodCells(const AmountUnit& unit) const
{
  if (!HasWhiteBloodCells())
    return SEScalar::dNaN();
  return m_WhiteBloodCells->GetValue(unit);
}

bool SEUrinalysisMicroscopic::HasEpithelialCells() const
{
  return m_EpithelialCells != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetEpithelialCells() const
{
  return m_EpithelialCells;
}
void SEUrinalysisMicroscopic::SetEpithelialCells(eUrinalysis_MicroscopicObservationAmount c)
{
  m_EpithelialCells = c;
}
void SEUrinalysisMicroscopic::InvalidateEpithelialCells()
{
  m_EpithelialCells = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

bool SEUrinalysisMicroscopic::HasCasts() const
{
  return m_Casts == nullptr ? false : m_Casts->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetCasts()
{
  if (m_Casts == nullptr)
    m_Casts = new SEScalarAmount();
  return *m_Casts;
}
double SEUrinalysisMicroscopic::GetCasts(const AmountUnit& unit) const
{
  if (!HasCasts())
    return SEScalar::dNaN();
  return m_Casts->GetValue(unit);
}

bool SEUrinalysisMicroscopic::HasCrystals() const
{
  return m_Crystals != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetCrystals() const
{
  return m_Crystals;
}
void SEUrinalysisMicroscopic::SetCrystals(eUrinalysis_MicroscopicObservationAmount c)
{
  m_Crystals = c;
}
void SEUrinalysisMicroscopic::InvalidateCrystals()
{
  m_Crystals = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

bool SEUrinalysisMicroscopic::HasBacteria() const
{
  return m_Bacteria != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetBacteria() const
{
  return m_Bacteria;
}
void SEUrinalysisMicroscopic::SetBacteria(eUrinalysis_MicroscopicObservationAmount c)
{
  m_Bacteria = c;
}
void SEUrinalysisMicroscopic::InvalidateBacteria()
{
  m_Bacteria = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

bool SEUrinalysisMicroscopic::HasTrichomonads() const
{
  return m_Trichomonads != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetTrichomonads() const
{
  return m_Trichomonads;
}
void SEUrinalysisMicroscopic::SetTrichomonads(eUrinalysis_MicroscopicObservationAmount c)
{
  m_Trichomonads = c;
}
void SEUrinalysisMicroscopic::InvalidateTrichomonads()
{
  m_Trichomonads = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}

bool SEUrinalysisMicroscopic::HasYeast() const
{
  return m_Yeast != eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}
eUrinalysis_MicroscopicObservationAmount SEUrinalysisMicroscopic::GetYeast() const
{
  return m_Yeast;
}
void SEUrinalysisMicroscopic::SetYeast(eUrinalysis_MicroscopicObservationAmount c)
{
  m_Yeast = c;
}
void SEUrinalysisMicroscopic::InvalidateYeast()
{
  m_Yeast = eUrinalysis_MicroscopicObservationAmount::NullObservationAmount;
}