/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEChestOcclusiveDressing : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEChestOcclusiveDressing(Logger* logger=nullptr);
  virtual ~SEChestOcclusiveDressing();

  static constexpr char const* Name = "Chest Occlusive Dressing";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEChestOcclusiveDressing& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eSwitch GetState() const { return m_State; }
  virtual void SetState(eSwitch s) { m_State = (s == eSwitch::NullSwitch) ? eSwitch::Off : s; }

  virtual eSide GetSide() const;
  virtual void SetSide(eSide LeftOrRight);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

protected:
  eSide m_Side;
  eSwitch m_State;
 
};   