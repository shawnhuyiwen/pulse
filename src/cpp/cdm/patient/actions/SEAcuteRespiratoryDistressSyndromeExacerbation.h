/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"
#include "cdm/system/physiology/SERespiratorySystem.h"

class CDM_DECL SEAcuteRespiratoryDistressSyndromeExacerbation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEAcuteRespiratoryDistressSyndromeExacerbation(Logger* logger=nullptr);
  virtual ~SEAcuteRespiratoryDistressSyndromeExacerbation();

  static constexpr char const* Name = "ARDS Exacerbation";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEAcuteRespiratoryDistressSyndromeExacerbation& src, bool preserveState=false);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasSeverity() const;
  virtual LungImpairmentMap& GetSeverities();
  virtual const LungImpairmentMap& GetSeverities() const;
  virtual bool HasSeverity(eLungCompartment cmpt) const;
  virtual SEScalar0To1& GetSeverity(eLungCompartment cmpt);
  virtual const SEScalar0To1* GetSeverity(eLungCompartment cmpt) const;

protected:
  LungImpairmentMap m_Severities;
};