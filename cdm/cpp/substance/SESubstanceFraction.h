/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once
class SESubstance;
class SESubstanceManager;
class SEEnvironmentalConditions;
CDM_BIND_DECL(SubstanceData_FractionAmountData)

class CDM_DECL SESubstanceFraction : public Loggable
{
  friend class PBSubstance;//friend the serialization class
  friend SEEnvironmentalConditions;// So it can add substances to the manager
public:

  SESubstanceFraction(SESubstance&);
  virtual ~SESubstanceFraction();

  virtual void Clear();

  virtual bool HasFractionAmount() const;
  virtual SEScalar0To1& GetFractionAmount();
  virtual double GetFractionAmount() const;

  virtual SESubstance& GetSubstance() const;

protected:

  SESubstance&       m_Substance;
  SEScalar0To1*  m_FractionAmount;
};  