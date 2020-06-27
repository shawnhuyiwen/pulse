/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/assessments/SEPatientAssessment.h"

class CDM_DECL SECompleteBloodCount : public SEPatientAssessment
{
  friend class PBPatientAssessment;//friend the serialization class
public:

  SECompleteBloodCount(Logger* logger);
  virtual ~SECompleteBloodCount();

  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;

  virtual bool HasHematocrit() const;
  virtual SEScalar0To1& GetHematocrit(); 
  virtual double GetHematocrit() const;

  virtual bool HasHemoglobin() const;
  virtual SEScalarMassPerVolume& GetHemoglobin();
  virtual double GetHemoglobin(const MassPerVolumeUnit& unit) const;

  virtual bool HasPlateletCount() const;
  virtual SEScalarAmountPerVolume& GetPlateletCount();
  virtual double GetPlateletCount(const AmountPerVolumeUnit& unit) const;

  virtual bool HasMeanCorpuscularHemoglobin() const;
  virtual SEScalarMassPerAmount& GetMeanCorpuscularHemoglobin();
  virtual double GetMeanCorpuscularHemoglobin(const MassPerAmountUnit& unit) const;

  virtual bool HasMeanCorpuscularHemoglobinConcentration() const;
  virtual SEScalarMassPerVolume& GetMeanCorpuscularHemoglobinConcentration();
  virtual double GetMeanCorpuscularHemoglobinConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasMeanCorpuscularVolume() const;
  virtual SEScalarVolume& GetMeanCorpuscularVolume();
  virtual double GetMeanCorpuscularVolume(const VolumeUnit& unit) const;
  
  virtual bool HasRedBloodCellCount() const;
  virtual SEScalarAmountPerVolume& GetRedBloodCellCount();
  virtual double GetRedBloodCellCount(const AmountPerVolumeUnit& unit) const;

  virtual bool HasWhiteBloodCellCount() const;
  virtual SEScalarAmountPerVolume& GetWhiteBloodCellCount();
  virtual double GetWhiteBloodCellCount(const AmountPerVolumeUnit& unit) const;

protected:

  SEScalar0To1*               m_Hematocrit;
  SEScalarMassPerVolume*      m_Hemoglobin;
  SEScalarAmountPerVolume*    m_PlateletCount;
  SEScalarMassPerAmount*      m_MeanCorpuscularHemoglobin;
  SEScalarMassPerVolume*      m_MeanCorpuscularHemoglobinConcentration;
  SEScalarVolume*             m_MeanCorpuscularVolume;
  SEScalarAmountPerVolume*    m_RedBloodCellCount;
  SEScalarAmountPerVolume*    m_WhiteBloodCellCount;

};  