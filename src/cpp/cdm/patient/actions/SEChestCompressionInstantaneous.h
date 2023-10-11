/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEChestCompressionInstantaneous : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEChestCompressionInstantaneous(Logger* logger=nullptr);
  virtual ~SEChestCompressionInstantaneous();

  static constexpr char const* Name = "Chest Compression Instantaneous";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEChestCompressionInstantaneous& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasForce() const;
  virtual SEScalarForce& GetForce();
  virtual double GetForce(const ForceUnit& unit) const;
  
  virtual bool HasDepth() const;
  virtual SEScalarLength& GetDepth();
  virtual double GetDepth(const LengthUnit& unit) const;

protected:
  SEScalarForce*           m_Force;
  SEScalarLength*          m_Depth;
};