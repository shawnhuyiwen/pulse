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

  virtual void Clear();
  virtual void Copy(const SEMechanicalVentilatorHold& src, bool preserveState = false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch name);

  virtual eAppliedRespiratoryCycle GetAppliedRespiratoryCycle() const;
  virtual void SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle c);

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eAppliedRespiratoryCycle m_AppliedRespiratoryCycle;
  eSwitch m_State;
};
