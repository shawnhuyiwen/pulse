/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SESubstanceAdministration.h"
PROTO_PUSH
#include "bind/cdm/PatientActionEnums.pb.h"
PROTO_POP
CDM_BIND_DECL(SubstanceBolusData)
CDM_BIND_DECL(SubstanceBolusData_StateData)
class SESubstance;

class CDM_DECL SESubstanceBolusState
{
public:
  SESubstanceBolusState(const SESubstance& sub);
  ~SESubstanceBolusState();

  void Clear();

  static void Load(const cdm::SubstanceBolusData_StateData& src, SESubstanceBolusState& dst);
  static cdm::SubstanceBolusData_StateData* Unload(const SESubstanceBolusState& src);
protected:
  static void Serialize(const cdm::SubstanceBolusData_StateData& src, SESubstanceBolusState& dst);
  static void Serialize(const SESubstanceBolusState& src, cdm::SubstanceBolusData_StateData& dst);

public:
  SEScalarTime& GetElapsedTime() { return *m_ElapsedTime; }
  double GetElapsedTime(const TimeUnit& unit) const;

  SEScalarVolume& GetAdministeredDose() { return *m_AdministeredDose; }
  double GetAdministeredDose(const VolumeUnit& unit) const;

protected:
  const SESubstance&   m_Substance;
  SEScalarTime*        m_ElapsedTime;
  SEScalarVolume*      m_AdministeredDose;
};

class CDM_DECL SESubstanceBolus : public SESubstanceAdministration
{
public:

  SESubstanceBolus(const SESubstance& substance);
  virtual ~SESubstanceBolus();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::SubstanceBolusData& src, SESubstanceBolus& dst);
  static cdm::SubstanceBolusData* Unload(const SESubstanceBolus& src);
protected:
  static void Serialize(const cdm::SubstanceBolusData& src, SESubstanceBolus& dst);
  static void Serialize(const SESubstanceBolus& src, cdm::SubstanceBolusData& dst);

public:

  virtual cdm::eSubstanceAdministration_Route GetAdminRoute() const;
  virtual void SetAdminRoute(cdm::eSubstanceAdministration_Route name);

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
  cdm::eSubstanceAdministration_Route m_AdminRoute;
  SEScalarMassPerVolume*                       m_Concentration;
  SEScalarVolume*                              m_Dose;
  const SESubstance&                           m_Substance;
  SESubstanceBolusState                        m_State;
  
};        

