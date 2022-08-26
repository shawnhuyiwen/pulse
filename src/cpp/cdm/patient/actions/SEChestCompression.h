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
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChestCompression& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasForce() const;
  virtual SEScalarForce& GetForce();
  virtual double GetForce(const ForceUnit& unit) const;
  
  virtual bool HasForceScale() const;
  virtual SEScalar0To1& GetForceScale();
  virtual double GetForceScale() const;

  virtual bool HasCompressionPeriod() const;
  virtual SEScalarTime& GetCompressionPeriod();
  virtual double GetCompressionPeriod(const TimeUnit& unit) const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalarForce*           m_Force;
  SEScalar0To1*            m_ForceScale;
  SEScalarTime*            m_CompressionPeriod;
};