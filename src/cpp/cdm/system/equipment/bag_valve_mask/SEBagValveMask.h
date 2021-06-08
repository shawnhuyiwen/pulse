/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/equipment/SEEquipment.h"
class SEEventHandler;
class SEBagValveMaskConfiguration;
class SESubstance;
class SESubstanceFraction;
class SESubstanceConcentration;

// Keep enums in sync with appropriate schema/cdm/BagValveMask.proto file !!
enum class eBagValveMask_Connection { NullConnection = 0, Off, Mask, Tube };
extern const std::string& eBagValveMask_Connection_Name(eBagValveMask_Connection m);

// Keep enums in sync with appropriate schema/cdm/BagValveMask.proto file !!
enum class eBagValveMask_DriverWaveform { NullDriverWaveform = 0, Square };
extern const std::string& eBagValveMask_DriverWaveform_Name(eBagValveMask_DriverWaveform m);

class CDM_DECL SEBagValveMask : public SEEquipment
{
  friend class PBBagValveMask;//friend the serialization class
protected:
  friend SEBagValveMaskConfiguration;
public:

  SEBagValveMask(Logger* logger);
  virtual ~SEBagValveMask();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr);

protected:

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange(){};
  virtual void Merge(const SEBagValveMask& from, SESubstanceManager& subMgr);
  virtual void ProcessConfiguration(SEBagValveMaskConfiguration& config, SESubstanceManager& subMgr);

public:

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual eBagValveMask_Connection GetConnection() const;
  virtual void SetConnection(eBagValveMask_Connection c);

  virtual bool HasBagResistance() const;
  virtual SEScalarPressureTimePerVolume& GetBagResistance();
  virtual double GetBagResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasFilterResistance() const;
  virtual SEScalarPressureTimePerVolume& GetFilterResistance();
  virtual double GetFilterResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasFilterVolume() const;
  virtual SEScalarVolume& GetFilterVolume();
  virtual double GetFilterVolume(const VolumeUnit& unit) const;

  virtual bool HasValveResistance() const;
  virtual SEScalarPressureTimePerVolume& GetValveResistance();
  virtual double GetValveResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasValvePositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetValvePositiveEndExpiredPressure();
  virtual double GetValvePositiveEndExpiredPressure(const PressureUnit& unit) const;

  bool HasFractionInspiredGas() const;
  bool HasFractionInspiredGas(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetFractionInspiredGases();
  const std::vector<const SESubstanceFraction*>& GetFractionInspiredGases() const;
  SESubstanceFraction& GetFractionInspiredGas(const SESubstance& substance);
  const SESubstanceFraction* GetFractionInspiredGas(const SESubstance& substance) const;
  void RemoveFractionInspiredGas(const SESubstance& substance);
  void RemoveFractionInspiredGases();

  bool HasConcentrationInspiredAerosol() const;
  bool HasConcentrationInspiredAerosol(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetConcentrationInspiredAerosols();
  const std::vector<const SESubstanceConcentration*>& GetConcentrationInspiredAerosols() const;
  SESubstanceConcentration& GetConcentrationInspiredAerosol(const SESubstance& substance);
  const SESubstanceConcentration* GetConcentrationInspiredAerosol(const SESubstance& substance) const;
  void RemoveConcentrationInspiredAerosol(const SESubstance& substance);
  void RemoveConcentrationInspiredAerosols();

protected:
  eBagValveMask_Connection                     m_Connection;

  SEScalarPressureTimePerVolume*               m_BagResistance;
  SEScalarPressureTimePerVolume*               m_FilterResistance;
  SEScalarVolume*                              m_FilterVolume;
  SEScalarPressure*                            m_ValvePositiveEndExpiredPressure;
  SEScalarPressureTimePerVolume*               m_ValveResistance;

  std::vector<SESubstanceFraction*>            m_FractionInspiredGases;
  std::vector<const SESubstanceFraction*>      m_cFractionInspiredGases;

  std::vector<SESubstanceConcentration*>       m_ConcentrationInspiredAerosols;
  std::vector<const SESubstanceConcentration*> m_cConcentrationInspiredAerosols;
};
