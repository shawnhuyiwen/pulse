/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SESubstanceAdministration.h"
class SESubstance;

class CDM_DECL SESubstanceBolus : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESubstanceBolus(const SESubstance& substance, Logger* logger=nullptr);
  virtual ~SESubstanceBolus();

  virtual void Clear(); //clear memory
  virtual void Copy(const SESubstanceBolus& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Activate();
  virtual void Deactivate();

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

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

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
