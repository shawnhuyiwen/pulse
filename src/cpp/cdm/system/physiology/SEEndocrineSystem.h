/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/system/SESystem.h"

class CDM_DECL SEEndocrineSystem : public SESystem
{
  friend class PBPhysiology;//friend the serialization class
public:

  SEEndocrineSystem(Logger* logger);
  virtual ~SEEndocrineSystem();

  void Clear() override;// Deletes all members
  
  const SEScalar* GetScalar(const std::string& name) override;
  
public:

  virtual bool HasInsulinSynthesisRate() const;
  virtual SEScalarAmountPerTime& GetInsulinSynthesisRate();
  virtual double GetInsulinSynthesisRate(const AmountPerTimeUnit& unit) const;

protected:

  SEScalarAmountPerTime* m_InsulinSynthesisRate;

};
