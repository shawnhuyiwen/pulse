/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"
#include "properties/SEScalar.h"

class CDM_DECL SEAnesthesiaMachineOxygenTankPressureLoss : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:
  SEAnesthesiaMachineOxygenTankPressureLoss(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineOxygenTankPressureLoss();

  virtual void Clear();
  virtual void Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch state);

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  SEScalarEnum<eSwitch> m_State;
};
