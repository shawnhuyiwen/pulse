/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/equipment/SEEquipment.h"
class Serializer; 
class SESubstance;
class SESubstanceManager;
class SEInhalerConfiguration;

class CDM_DECL SEInhaler : public SEEquipment
{
  friend class PBInhaler;//friend the serialization class
public:

  SEInhaler(Logger* logger);
  virtual ~SEInhaler();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr);

  /** @name GetScalar
  *   @brief - A reflextion type call that will return the Scalar associated
  *            with the string. ex. GetScalar("Hematocrit") will return the
  *            SEScalarPercent object associated with Hematocrit
  *   @details - Note this is an expensive call as it will string compare many times
  *              This is best used, and intended for, you to dynamically prepopulate
  *              a mapping data structure that will help access what you need
  */
  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange(){};
  virtual void Merge(const SEInhaler& from, SESubstanceManager& subMgr);
  virtual void ProcessConfiguration(SEInhalerConfiguration& config, SESubstanceManager& subMgr);

public:

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch name);

  virtual bool HasMeteredDose() const;
  virtual SEScalarMass& GetMeteredDose();
  virtual double GetMeteredDose(const MassUnit& unit) const;

  virtual bool HasNozzleLoss() const;
  virtual SEScalar0To1& GetNozzleLoss();
  virtual double GetNozzleLoss() const;

  virtual bool HasSpacerVolume() const;
  virtual SEScalarVolume& GetSpacerVolume();
  virtual double GetSpacerVolume(const VolumeUnit& unit) const;

  virtual bool HasSubstance() const;
  virtual void SetSubstance(const SESubstance* sub);
  virtual SESubstance* GetSubstance() const;

protected:

  eSwitch               m_State;
  SEScalarMass*         m_MeteredDose;
  SEScalar0To1*         m_NozzleLoss;
  SEScalarVolume*       m_SpacerVolume;
  const SESubstance*    m_Substance;
};
