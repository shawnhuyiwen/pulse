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
  virtual std::string GetName() const { return Name; }

  virtual void Clear();
  virtual void Copy(const SEArrhythmia& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eHeartRhythm GetRhythm() const;
  virtual void SetRhythm(eHeartRhythm t);

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eHeartRhythm           m_Rhythm;
};
