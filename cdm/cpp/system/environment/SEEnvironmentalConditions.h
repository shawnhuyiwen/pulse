/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"
class SESubstance;
class SESubstanceManager;
class SESubstanceFraction;
class SESubstanceConcentration;

// Keep enums in sync with appropriate schema/cdm/EnvironmentEnums.proto file !!
enum class eSurroundingType { NullSurrounding = 0, Air, Water };
extern const std::string& eSurroundingType_Name(eSurroundingType m);

class CDM_DECL SEEnvironmentalConditions : public Loggable
{
  friend class PBEnvironment;//friend the serialization class
  friend class SEEnvironment;
  friend class SEChangeEnvironmentConditions;
  friend class SEInitialEnvironmentConditions;
public:

  SEEnvironmentalConditions(SESubstanceManager& substances);
  virtual ~SEEnvironmentalConditions();

  virtual void Clear();
  virtual void Copy(const SEEnvironmentalConditions&);

  bool SerializeToString(std::string& output, SerializationMode m) const;
  bool SerializeToFile(const std::string& filename, SerializationMode m) const;
  bool SerializeFromString(const std::string& src, SerializationMode m);
  bool SerializeFromFile(const std::string& filename, SerializationMode m);

   virtual void Merge(const SEEnvironmentalConditions& from);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual eSurroundingType GetSurroundingType() const;
  virtual void SetSurroundingType(eSurroundingType name);

  virtual bool HasAirDensity() const;
  virtual SEScalarMassPerVolume& GetAirDensity();
  virtual double GetAirDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasAirVelocity() const;
  virtual SEScalarLengthPerTime& GetAirVelocity();
  virtual double GetAirVelocity(const LengthPerTimeUnit& unit) const;

  virtual bool HasAmbientTemperature() const;
  virtual SEScalarTemperature& GetAmbientTemperature();
  virtual double GetAmbientTemperature(const TemperatureUnit& unit) const;

  virtual bool HasAtmosphericPressure() const;
  virtual SEScalarPressure& GetAtmosphericPressure();
  virtual double GetAtmosphericPressure(const PressureUnit& unit) const;

  virtual bool HasClothingResistance() const;
  virtual SEScalarHeatResistanceArea& GetClothingResistance();
  virtual double GetClothingResistance(const HeatResistanceAreaUnit& unit) const;

  virtual bool HasEmissivity() const;
  virtual SEScalar0To1& GetEmissivity();
  virtual double GetEmissivity() const;

  virtual bool HasMeanRadiantTemperature() const;
  virtual SEScalarTemperature& GetMeanRadiantTemperature();
  virtual double GetMeanRadiantTemperature(const TemperatureUnit& unit) const;

  virtual bool HasRelativeHumidity() const;
  virtual SEScalar0To1& GetRelativeHumidity();
  virtual double GetRelativeHumidity() const;

  virtual bool HasRespirationAmbientTemperature() const;
  virtual SEScalarTemperature& GetRespirationAmbientTemperature();
  virtual double GetRespirationAmbientTemperature(const TemperatureUnit& unit) const;

  bool HasAmbientGas() const;
  bool HasAmbientGas(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetAmbientGases();
  const std::vector<const SESubstanceFraction*>& GetAmbientGases() const;
  SESubstanceFraction& GetAmbientGas(SESubstance& substance);
  const SESubstanceFraction* GetAmbientGas(const SESubstance& substance) const;
  void RemoveAmbientGas(const SESubstance& substance);
  void RemoveAmbientGases();

  bool HasAmbientAerosol() const;
  bool HasAmbientAerosol(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetAmbientAerosols();
  const std::vector<const SESubstanceConcentration*>& GetAmbientAerosols() const;
  SESubstanceConcentration& GetAmbientAerosol(SESubstance& substance);
  const SESubstanceConcentration* GetAmbientAerosol(const SESubstance& substance) const;
  void RemoveAmbientAerosol(const SESubstance& substance);
  void RemoveAmbientAerosols();

protected:

  eSurroundingType                             m_SurroundingType;
  
  SEScalarMassPerVolume*                       m_AirDensity;
  SEScalarLengthPerTime*                       m_AirVelocity;
  SEScalarTemperature*                         m_AmbientTemperature;
  SEScalarPressure*                            m_AtmosphericPressure;
  SEScalarHeatResistanceArea*                  m_ClothingResistance;
  SEScalar0To1*                                m_Emissivity;
  SEScalarTemperature*                         m_MeanRadiantTemperature;
  SEScalar0To1*                                m_RelativeHumidity;
  SEScalarTemperature*                         m_RespirationAmbientTemperature;

  std::vector<SESubstanceFraction*>            m_AmbientGases;
  std::vector<const SESubstanceFraction*>      m_cAmbientGases;

  std::vector<SESubstanceConcentration*>       m_AmbientAerosols;
  std::vector<const SESubstanceConcentration*> m_cAmbientAerosols;

  SESubstanceManager&               m_Substances;
};                  
