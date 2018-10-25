/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"

class CDM_DECL SEOxygenWallPortPressureLoss : public SEAnesthesiaMachineAction
{
  friend class PBAnesthesiaMachineAction;//friend the serialization class
public:
  SEOxygenWallPortPressureLoss();
  virtual ~SEOxygenWallPortPressureLoss();

  virtual void Clear();
  virtual void Copy(const SEOxygenWallPortPressureLoss& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void SetActive(bool b);

  virtual void ToString(std::ostream &str) const;

protected:
  
  eSwitch m_State;
};  