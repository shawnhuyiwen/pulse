/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"

class CDM_DECL SEAnesthesiaMachineTubeCuffLeak : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineTubeCuffLeak(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineTubeCuffLeak();

  static constexpr char const* Name = "Tube Cuff Leak";
  virtual std::string GetName() const { return Name; }

  virtual void Clear();
  virtual void Copy(const SEAnesthesiaMachineTubeCuffLeak& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  SEScalar0To1*     m_Severity;
};  