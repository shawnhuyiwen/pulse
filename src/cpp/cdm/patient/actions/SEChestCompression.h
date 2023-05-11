/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEChestCompression : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEChestCompression(Logger* logger=nullptr);
  virtual ~SEChestCompression();

  static constexpr char const* Name = "Chest Compression";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEChestCompression& src, bool /*preserveState*/=false);

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

  virtual bool HasCompressionPeriod() const;
  virtual SEScalarTime& GetCompressionPeriod();
  virtual double GetCompressionPeriod(const TimeUnit& unit) const;

protected:
  SEScalarForce*           m_Force;
  SEScalarLength*          m_Depth;
  SEScalarTime*            m_CompressionPeriod;
};