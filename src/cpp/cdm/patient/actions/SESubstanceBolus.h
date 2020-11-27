/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SESubstanceAdministration.h"
class SESubstance;

class CDM_DECL SESubstanceBolusState : public Loggable
{
  friend class PBPatientAction;//friend the serialization class
public:
  SESubstanceBolusState(const SESubstance& sub, Logger* logger=nullptr);
  ~SESubstanceBolusState();

  virtual void Clear();

  SEScalarTime& GetElapsedTime() { return *m_ElapsedTime; }
  double GetElapsedTime(const TimeUnit& unit) const;

  virtual SEScalarVolume& GetAdministeredDose() { return *m_AdministeredDose; }
  virtual double GetAdministeredDose(const VolumeUnit& unit) const;

protected:
  const SESubstance&   m_Substance;
  SEScalarTime*        m_ElapsedTime;
  SEScalarVolume*      m_AdministeredDose;
};

class CDM_DECL SESubstanceBolus : public SESubstanceAdministration
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESubstanceBolus(const SESubstance& substance, Logger* logger=nullptr);
  virtual ~SESubstanceBolus();

  virtual void Clear(); //clear memory
  virtual void Copy(const SESubstanceBolus& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

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

  virtual SESubstanceBolusState& GetState() { return m_State; }
  virtual const SESubstanceBolusState& GetState() const { return m_State; }

  virtual void ToString(std::ostream &str) const;

protected:
  const SESubstance&             m_Substance;
  eSubstanceAdministration_Route m_AdminRoute;
  SEScalarTime*                  m_AdminDuration;
  SEScalarMassPerVolume*         m_Concentration;
  SEScalarVolume*                m_Dose;
  SESubstanceBolusState          m_State;
};
