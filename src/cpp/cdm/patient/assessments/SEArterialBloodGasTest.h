/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/assessments/SEPatientAssessment.h"

class CDM_DECL SEArterialBloodGasTest : public SEPatientAssessment
{
  friend class PBPatientAssessment;//friend the serialization class
public:

  SEArterialBloodGasTest(Logger* logger);
  virtual ~SEArterialBloodGasTest();

  void Clear() override; //clear memory

  bool SerializeToString(std::string& output, eSerializationFormat m) const override;
  bool SerializeToFile(const std::string& filename) const override;

  virtual bool HasBloodPH() const;
  virtual SEScalar& GetBloodPH();
  virtual double GetBloodPH() const;

  virtual bool HasBicarbonate() const;
  virtual SEScalarEquivalentWeightPerVolume& GetBicarbonate();
  virtual double GetBicarbonate(const EquivalentWeightPerVolumeUnit& unit) const;

  virtual bool HasPartialPressureOfOxygen() const;
  virtual SEScalarPressure& GetPartialPressureOfOxygen();
  virtual double GetPartialPressureOfOxygen(const PressureUnit& unit) const;

  virtual bool HasPartialPressureOfCarbonDioxide() const;
  virtual SEScalarPressure& GetPartialPressureOfCarbonDioxide();
  virtual double GetPartialPressureOfCarbonDioxide(const PressureUnit& unit) const;

  virtual bool HasOxygenSaturation() const;
  virtual SEScalar& GetOxygenSaturation();
  virtual double GetOxygenSaturation() const;

protected:

  SEScalar*                          m_BloodPH;
  SEScalarEquivalentWeightPerVolume* m_Bicarbonate;
  SEScalarPressure*                  m_PartialPressureOfOxygen;
  SEScalarPressure*                  m_PartialPressureOfCarbonDioxide;
  SEScalar*                          m_OxygenSaturation;

};  