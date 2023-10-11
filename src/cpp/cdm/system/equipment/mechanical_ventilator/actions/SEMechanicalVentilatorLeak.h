/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorAction.h"
class SEMechanicalVentilator;
class SESubstanceManager;

class CDM_DECL SEMechanicalVentilatorLeak : public SEMechanicalVentilatorAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorLeak(Logger* logger = nullptr);
  virtual ~SEMechanicalVentilatorLeak();

  static constexpr char const* Name = "Leak";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEMechanicalVentilatorLeak& src, bool /*preserveState*/ = false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalar0To1* m_Severity;
};
