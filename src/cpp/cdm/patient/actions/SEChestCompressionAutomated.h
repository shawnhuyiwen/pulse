/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEChestCompressionAutomated : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEChestCompressionAutomated(Logger* logger=nullptr);
  virtual ~SEChestCompressionAutomated();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChestCompressionAutomated& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasCompressionFrequency() const;
  virtual SEScalarFrequency& GetCompressionFrequency();
  virtual double GetCompressionFrequency(const FrequencyUnit& unit) const;

  virtual bool HasForce() const;
  virtual SEScalarForce& GetForce();
  virtual double GetForce(const ForceUnit& unit) const;

  virtual bool HasForceScale() const;
  virtual SEScalar0To1& GetForceScale();
  virtual double GetForceScale() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalarFrequency*       m_CompressionFrequency;
  SEScalarForce*           m_Force;
  SEScalar0To1*            m_ForceScale;
};