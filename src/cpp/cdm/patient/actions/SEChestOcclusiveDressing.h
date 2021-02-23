/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEChestOcclusiveDressing : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEChestOcclusiveDressing(Logger* logger=nullptr);
  virtual ~SEChestOcclusiveDressing();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChestOcclusiveDressing& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eSwitch GetState() const { return m_State; }
  virtual void SetState(eSwitch s) { m_State = (s == eSwitch::NullSwitch) ? eSwitch::Off : s; }

  virtual eSide GetSide() const;
  virtual void SetSide(eSide LeftOrRight);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eSide m_Side;
  eSwitch m_State;
 
};   