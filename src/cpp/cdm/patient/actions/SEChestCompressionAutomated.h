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

  static constexpr char const* Name = "Chest Compression Automated";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChestCompressionAutomated& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasAppliedForceFraction() const;
  virtual SEScalar0To1& GetAppliedForceFraction();
  virtual double GetAppliedForceFraction() const;

  virtual bool HasCompressionFrequency() const;
  virtual SEScalarFrequency& GetCompressionFrequency();
  virtual double GetCompressionFrequency(const FrequencyUnit& unit) const;

  virtual bool HasForce() const;
  virtual SEScalarForce& GetForce();
  virtual double GetForce(const ForceUnit& unit) const;

  virtual bool HasDepth() const;
  virtual SEScalarLength& GetDepth();
  virtual double GetDepth(const LengthUnit& unit) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalar0To1*            m_AppliedForceFraction;
  SEScalarFrequency*       m_CompressionFrequency;
  SEScalarForce*           m_Force;
  SEScalarLength*          m_Depth;
};