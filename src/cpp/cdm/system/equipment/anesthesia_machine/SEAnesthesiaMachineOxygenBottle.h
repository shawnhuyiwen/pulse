/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAnesthesiaMachine;
class SESubstanceManager;

class CDM_DECL SEAnesthesiaMachineOxygenBottle : Loggable
{
  friend class PBAnesthesiaMachine;//friend the serialization class
  friend SEAnesthesiaMachine;
public:

  SEAnesthesiaMachineOxygenBottle(Logger* logger);
  virtual ~SEAnesthesiaMachineOxygenBottle();

  virtual void Clear();

  virtual void Merge(const SEAnesthesiaMachineOxygenBottle& from, SESubstanceManager& subMgr);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual void ToString(std::ostream &str);

protected:

  SEScalarVolume*     m_Volume;
};
