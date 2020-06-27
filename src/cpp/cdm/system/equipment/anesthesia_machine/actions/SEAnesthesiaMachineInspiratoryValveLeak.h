/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"

class CDM_DECL SEAnesthesiaMachineInspiratoryValveLeak : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineInspiratoryValveLeak(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineInspiratoryValveLeak();

  virtual void Clear();
  virtual void Copy(const SEAnesthesiaMachineInspiratoryValveLeak& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream &str) const;

protected:

  SEScalar0To1*     m_Severity;
};  