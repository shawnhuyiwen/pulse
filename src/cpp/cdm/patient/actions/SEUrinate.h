/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEUrinate : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEUrinate(Logger* logger=nullptr);
  virtual ~SEUrinate();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEUrinate& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

};
