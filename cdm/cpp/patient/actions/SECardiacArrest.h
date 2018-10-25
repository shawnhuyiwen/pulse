/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SECardiacArrest : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SECardiacArrest();
  virtual ~SECardiacArrest();

  virtual void Clear(); //clear memory
  virtual void Copy(const SECardiacArrest& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual eSwitch GetState() const { return m_State; }
  virtual void SetState(eSwitch s) { m_State = (s == eSwitch::NullSwitch) ? eSwitch::Off : s; }

  virtual void ToString(std::ostream &str) const;

protected:
  eSwitch m_State;

};