/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEHemothorax : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEHemothorax(Logger* logger=nullptr);
  virtual ~SEHemothorax();

  static constexpr char const* Name = "Hemothorax";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEHemothorax& src, bool /*preserveState*/=false);
  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eSide GetSide() const;
  virtual void SetSide(eSide name);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual bool HasFlowRate() const;
  virtual SEScalarVolumePerTime& GetFlowRate();
  virtual double GetFlowRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalBloodVolume() const;
  virtual SEScalarVolume& GetTotalBloodVolume();
  virtual double GetTotalBloodVolume(const VolumeUnit& unit) const;

protected:
  eSide                   m_Side;
  SEScalar0To1*           m_Severity;
  SEScalarVolumePerTime*  m_FlowRate;
  SEScalarVolume*         m_TotalBloodVolume;
};
