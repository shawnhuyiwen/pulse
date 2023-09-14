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
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEAnesthesiaMachineTubeCuffLeak& src, bool /*preserveState*/=false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalar0To1*     m_Severity;
};  