/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEAirwayObstruction : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEAirwayObstruction(Logger* logger=nullptr);
  virtual ~SEAirwayObstruction();

  virtual void Clear();
  virtual void Copy(const SEAirwayObstruction& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalar0To1*           m_Severity;
};      