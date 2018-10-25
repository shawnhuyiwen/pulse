/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEChestCompression.h"

class CDM_DECL SEChestCompressionForceScale : public SEChestCompression
{
  friend class PBPatientAction;//friend the serialization class
public:
  SEChestCompressionForceScale();
  virtual ~SEChestCompressionForceScale();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChestCompressionForceScale& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasForceScale() const;
  virtual SEScalar0To1& GetForceScale();
  virtual double GetForceScale() const;

  virtual bool HasForcePeriod() const;
  virtual SEScalarTime& GetForcePeriod();
  virtual double GetForcePeriod(const TimeUnit& unit) const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1*           m_ForceScale;
  SEScalarTime*               m_ForcePeriod;
};