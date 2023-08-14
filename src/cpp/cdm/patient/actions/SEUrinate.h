/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEUrinate : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEUrinate(Logger* logger=nullptr);
  virtual ~SEUrinate();

  static constexpr char const* Name = "Urinate";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEUrinate& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

protected:

};
