/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SESubstanceAdministration.h"
class SESubstance;

class CDM_DECL SESubstanceBolus : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESubstanceBolus(const SESubstance& substance, Logger* logger=nullptr);
  virtual ~SESubstanceBolus();

  static constexpr char const* Name = "Substance Bolus";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SESubstanceBolus& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Activate() override;
  void Deactivate() override;

  virtual eSubstanceAdministration_Route GetAdminRoute() const;
  virtual void SetAdminRoute(eSubstanceAdministration_Route name);

  virtual bool HasAdminDuration() const;
  virtual SEScalarTime& GetAdminDuration();
  virtual double GetAdminDuration(const TimeUnit& unit) const;

  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();
  virtual double GetConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasDose() const;
  virtual SEScalarVolume& GetDose();
  virtual double GetDose(const VolumeUnit& unit) const;

  virtual SESubstance& GetSubstance();
  virtual const SESubstance& GetSubstance() const;

  virtual bool HasTotalInfusedDose() const;
  virtual SEScalarVolume& GetTotalInfusedDose();
  virtual double GetTotalInfusedDose(const VolumeUnit& unit) const;

protected:

  const SESubstance&             m_Substance;
  eSubstanceAdministration_Route m_AdminRoute;
  SEScalarTime*                  m_AdminDuration;
  SEScalarMassPerVolume*         m_Concentration;
  SEScalarVolume*                m_Dose;
  SEScalarVolume*                m_TotalInfusedDose;
};
