/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorAction.h"
class SEMechanicalVentilator;
class SESubstanceManager;

class CDM_DECL SEMechanicalVentilatorHold : public SEMechanicalVentilatorAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorHold(Logger* logger=nullptr);
  virtual ~SEMechanicalVentilatorHold();

  static constexpr char const* Name = "Hold";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEMechanicalVentilatorHold& src, bool /*preserveState*/ = false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch name);

  virtual eAppliedRespiratoryCycle GetAppliedRespiratoryCycle() const;
  virtual void SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle c);

  const SEScalar* GetScalar(const std::string& name) override;

protected:
  eAppliedRespiratoryCycle m_AppliedRespiratoryCycle;
  eSwitch m_State;
};
