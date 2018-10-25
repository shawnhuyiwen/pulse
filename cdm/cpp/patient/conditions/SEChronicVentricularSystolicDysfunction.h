/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/conditions/SEChronicHeartFailure.h"

class CDM_DECL SEChronicVentricularSystolicDysfunction : public SEChronicHeartFailure
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEChronicVentricularSystolicDysfunction();
  virtual ~SEChronicVentricularSystolicDysfunction();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChronicVentricularSystolicDysfunction& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const { return "ChronicVentricularSystolicDysfunction"; }

  virtual void ToString(std::ostream &str) const;

protected:
};    