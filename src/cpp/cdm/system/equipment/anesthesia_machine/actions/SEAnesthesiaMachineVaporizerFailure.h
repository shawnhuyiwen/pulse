/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"

class CDM_DECL SEAnesthesiaMachineVaporizerFailure : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineVaporizerFailure(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineVaporizerFailure();

  virtual void Clear();
  virtual void Copy(const SEAnesthesiaMachineVaporizerFailure& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  SEScalar0To1*     m_Severity;
};  