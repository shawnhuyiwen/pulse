/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstance;
class SESubstanceManager;
class SEAnesthesiaMachine;

class CDM_DECL SEAnesthesiaMachineChamber : Loggable
{
  friend class PBAnesthesiaMachine;//friend the serialization class
  friend SEAnesthesiaMachine;
public:

  SEAnesthesiaMachineChamber(Logger* logger);
  virtual ~SEAnesthesiaMachineChamber();

  virtual void Clear();

  virtual void Merge(const SEAnesthesiaMachineChamber& from, SESubstanceManager& subMgr);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch s);

  virtual bool HasSubstanceFraction() const;
  virtual SEScalar0To1& GetSubstanceFraction();
  virtual double GetSubstanceFraction() const;

  virtual bool HasSubstance() const;
  virtual SESubstance* GetSubstance() const;
  virtual void SetSubstance(const SESubstance& substance);
  virtual void RemoveSubstance();

  virtual void ToString(std::ostream &str) const;

protected:

  eSwitch               m_State;
  const SESubstance*    m_Substance;
  SEScalar0To1*         m_SubstanceFraction;
};
