/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"

class CDM_DECL SEEndocrineSystem : public SESystem
{
  friend class PBPhysiology;//friend the serialization class
public:

  SEEndocrineSystem(Logger* logger);
  virtual ~SEEndocrineSystem();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);
  
public:

  virtual bool HasInsulinSynthesisRate() const;
  virtual SEScalarAmountPerTime& GetInsulinSynthesisRate();
  virtual double GetInsulinSynthesisRate(const AmountPerTimeUnit& unit) const;

protected:

  SEScalarAmountPerTime* m_InsulinSynthesisRate;

};
