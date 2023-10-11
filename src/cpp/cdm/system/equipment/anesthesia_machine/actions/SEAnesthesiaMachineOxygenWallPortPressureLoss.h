/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"
#include "cdm/properties/SEScalar.h"

class CDM_DECL SEAnesthesiaMachineOxygenWallPortPressureLoss : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:
  SEAnesthesiaMachineOxygenWallPortPressureLoss(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineOxygenWallPortPressureLoss();

  static constexpr char const* Name = "Oxygen Wall Port Pressure Loss";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, bool /*preserveState*/=false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch state);

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalarEnum<eSwitch> m_State;
};
