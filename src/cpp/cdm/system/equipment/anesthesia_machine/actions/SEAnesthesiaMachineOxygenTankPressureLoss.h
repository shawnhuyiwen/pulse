/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"
#include "cdm/properties/SEScalar.h"

class CDM_DECL SEAnesthesiaMachineOxygenTankPressureLoss : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:
  SEAnesthesiaMachineOxygenTankPressureLoss(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineOxygenTankPressureLoss();

  static constexpr char const* Name = "Oxygen Tank Pressure Loss";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src, bool /*preserveState*/=false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch state);

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalarEnum<eSwitch> m_State;
};
