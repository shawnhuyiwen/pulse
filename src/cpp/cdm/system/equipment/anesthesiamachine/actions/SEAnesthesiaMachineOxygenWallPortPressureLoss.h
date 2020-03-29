/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"

class CDM_DECL SEAnesthesiaMachineOxygenWallPortPressureLoss : public SEAnesthesiaMachineAction
{
  friend class PBAnesthesiaMachineAction;//friend the serialization class
public:
  SEAnesthesiaMachineOxygenWallPortPressureLoss();
  virtual ~SEAnesthesiaMachineOxygenWallPortPressureLoss();

  virtual void Clear();
  virtual void Copy(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void SetActive(bool b);

  virtual void ToString(std::ostream &str) const;

protected:
  
  eSwitch m_State;
};  