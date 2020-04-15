/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/equipment/SEEquipment.h"
class SEEventHandler;
class SEMechanicalVentilatorConfiguration;
class SESubstance;
class SESubstanceFraction;
class SESubstanceConcentration;

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
enum class eMechanicalVentilator_Connection { NullConnection = 0, Off, Mask, Tube };
extern const std::string& eMechanicalVentilator_Connection_Name(eMechanicalVentilator_Connection m);

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
enum class eMechanicalVentilator_Control { NullControl = 0, PC_CMV };
extern const std::string& eMechanicalVentilator_Control_Name(eMechanicalVentilator_Control m);

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
enum class eMechanicalVentilator_DriverWaveform { NullDriverWaveform = 0, Square };
extern const std::string& eMechanicalVentilator_DriverWaveform_Name(eMechanicalVentilator_DriverWaveform m);

class CDM_DECL SEMechanicalVentilator : public SEEquipment
{
  friend class PBMechanicalVentilator;//friend the serialization class
protected:
  friend SEMechanicalVentilatorConfiguration;
public:

  SEMechanicalVentilator(SESubstanceManager& substances);
  virtual ~SEMechanicalVentilator();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m);

protected:

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange(){};
  virtual void Merge(const SEMechanicalVentilator& from);
  virtual void ProcessConfiguration(SEMechanicalVentilatorConfiguration& config);

public:

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasBreathPeriod() const;
  virtual SEScalarTime& GetBreathPeriod();
  virtual double GetBreathPeriod(const TimeUnit& unit) const;

  virtual eMechanicalVentilator_Connection GetConnection() const;
  virtual void SetConnection(eMechanicalVentilator_Connection c);

  virtual eMechanicalVentilator_Control GetControl() const;
  virtual void SetControl(eMechanicalVentilator_Control c);

  virtual eMechanicalVentilator_DriverWaveform GetDriverWaveform() const;
  virtual void SetDriverWaveform(eMechanicalVentilator_DriverWaveform w);

  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;

  virtual bool HasInspiratoryPeriod() const;
  virtual SEScalarTime& GetInspiratoryPeriod();
  virtual double GetInspiratoryPeriod(const TimeUnit& unit) const;

  virtual bool HasExpiratoryPeriod() const;
  virtual SEScalarTime& GetExpiratoryPeriod();
  virtual double GetExpiratoryPeriod(const TimeUnit& unit) const;

  virtual bool HasPeakInspiratoryPressure() const;
  virtual SEScalarPressure& GetPeakInspiratoryPressure();
  virtual double GetPeakInspiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;

  virtual bool HasRespiratoryRate() const;
  virtual SEScalarFrequency& GetRespiratoryRate();
  virtual double GetRespiratoryRate(const FrequencyUnit& unit) const;

  bool HasFractionInspiredGas() const;
  bool HasFractionInspiredGas(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetFractionInspiredGases();
  const std::vector<const SESubstanceFraction*>& GetFractionInspiredGases() const;
  SESubstanceFraction& GetFractionInspiredGas(SESubstance& substance);
  const SESubstanceFraction* GetFractionInspiredGas(const SESubstance& substance) const;
  void RemoveFractionInspiredGas(const SESubstance& substance);
  void RemoveFractionInspiredGases();

  bool HasConcentrationInspiredAerosol() const;
  bool HasConcentrationInspiredAerosol(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetConcentrationInspiredAerosols();
  const std::vector<const SESubstanceConcentration*>& GetConcentrationInspiredAerosols() const;
  SESubstanceConcentration& GetConcentrationInspiredAerosol(SESubstance& substance);
  const SESubstanceConcentration* GetConcentrationInspiredAerosol(const SESubstance& substance) const;
  void RemoveConcentrationInspiredAerosol(const SESubstance& substance);
  void RemoveConcentrationInspiredAerosols();

protected:
  
  SEScalarTime*                                          m_BreathPeriod;
  eMechanicalVentilator_Connection                       m_Connection;
  eMechanicalVentilator_Control                          m_Control;
  eMechanicalVentilator_DriverWaveform                   m_DriverWaveform;
  SEScalar*                                              m_InspiratoryExpiratoryRatio;
  SEScalarTime*                                          m_InspiratoryPeriod;
  SEScalarTime*                                          m_ExpiratoryPeriod;
  SEScalarPressure*                                      m_PeakInspiratoryPressure;
  SEScalarPressure*                                      m_PositiveEndExpiredPressure;
  SEScalarFrequency*                                     m_RespiratoryRate;

  std::vector<SESubstanceFraction*>                      m_FractionInspiredGases;
  std::vector<const SESubstanceFraction*>                m_cFractionInspiredGases;

  std::vector<SESubstanceConcentration*>                 m_ConcentrationInspiredAerosols;
  std::vector<const SESubstanceConcentration*>           m_cConcentrationInspiredAerosols;

  SESubstanceManager&                                    m_Substances;
};
