/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"

class CDM_DECL SEArrhythmia : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEArrhythmia(Logger* logger=nullptr);
  virtual ~SEArrhythmia();

  static constexpr char const* Name = "Arrhythmia";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEArrhythmia& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eHeartRhythm GetRhythm() const;
  virtual void SetRhythm(eHeartRhythm t);

protected:
  eHeartRhythm           m_Rhythm;
};
