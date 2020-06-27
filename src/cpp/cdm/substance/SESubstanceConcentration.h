/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstance;
class SESubstanceManager;
class SEEnvironmentalConditions;
CDM_BIND_DECL(SubstanceData_ConcentrationData)

class CDM_DECL SESubstanceConcentration : public Loggable
{
  friend class PBSubstance;//friend the serialization class
  friend SEEnvironmentalConditions;// So it can add substances to the manager
public:

  SESubstanceConcentration(const SESubstance& substance);
  virtual ~SESubstanceConcentration();

  virtual void Clear();

  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();
  virtual double GetConcentration(const MassPerVolumeUnit& unit) const;

  virtual const SESubstance& GetSubstance() const;
  
protected: 

  const SESubstance&     m_Substance;
  SEScalarMassPerVolume* m_Concentration;
  
};