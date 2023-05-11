/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SENeedleDecompression : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SENeedleDecompression(Logger* logger=nullptr);
  virtual ~SENeedleDecompression();

  static constexpr char const* Name = "Needle Decompression";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SENeedleDecompression& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch name);

  virtual eSide GetSide() const;
  virtual void SetSide(eSide LeftOrRight);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

protected:
  eSide m_Side;
  eSwitch m_State;
};
