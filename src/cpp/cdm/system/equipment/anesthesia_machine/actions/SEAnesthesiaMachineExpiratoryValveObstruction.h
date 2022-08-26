/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"

class CDM_DECL SEAnesthesiaMachineExpiratoryValveObstruction : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineExpiratoryValveObstruction(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineExpiratoryValveObstruction();

  static constexpr char const* Name = "Expiratory Valve Obstruction";
  virtual std::string GetName() const { return Name; }

  virtual void Clear();
  virtual void Copy(const SEAnesthesiaMachineExpiratoryValveObstruction& src, bool /*preserveState*/=false);

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