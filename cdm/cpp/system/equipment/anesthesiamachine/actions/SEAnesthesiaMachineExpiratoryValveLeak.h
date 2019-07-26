/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"

class CDM_DECL SEAnesthesiaMachineExpiratoryValveLeak : public SEAnesthesiaMachineAction
{
  friend class PBAnesthesiaMachineAction;//friend the serialization class
public:

  SEAnesthesiaMachineExpiratoryValveLeak();
  virtual ~SEAnesthesiaMachineExpiratoryValveLeak();

  virtual void Clear();
  virtual void Copy(const SEAnesthesiaMachineExpiratoryValveLeak& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream &str) const;

protected:

  SEScalar0To1*     m_Severity;
};  