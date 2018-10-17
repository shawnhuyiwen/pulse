/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
CDM_BIND_DECL(ChestOcclusiveDressingData)

class CDM_DECL SEChestOcclusiveDressing : public SEPatientAction
{
public:

  SEChestOcclusiveDressing();
  virtual ~SEChestOcclusiveDressing();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::ChestOcclusiveDressingData& src, SEChestOcclusiveDressing& dst);
  static cdm::ChestOcclusiveDressingData* Unload(const SEChestOcclusiveDressing& src);
protected:
  static void Serialize(const cdm::ChestOcclusiveDressingData& src, SEChestOcclusiveDressing& dst);
  static void Serialize(const SEChestOcclusiveDressing& src, cdm::ChestOcclusiveDressingData& dst);

public:

  virtual eSwitch GetState() const { return m_State; }
  virtual void SetState(eSwitch s) { m_State = (s == eSwitch::NullSwitch) ? eSwitch::Off : s; }

  virtual eSide GetSide() const;
  virtual void SetSide(eSide LeftOrRight);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

  virtual void ToString(std::ostream &str) const;

protected:
  eSide m_Side;
  eSwitch m_State;
 
};   