/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PROTO_PUSH
#include "bind/cdm/SubstanceEnums.pb.h"
PROTO_POP
CDM_BIND_DECL(SubstanceData_PhysicochemicalData)

class CDM_DECL SESubstancePhysicochemicals : public Loggable
{
public:

  SESubstancePhysicochemicals(Logger* logger);
  virtual ~SESubstancePhysicochemicals();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const std::string& name);

  static void Load(const cdm::SubstanceData_PhysicochemicalData& src, SESubstancePhysicochemicals& dst);
  static cdm::SubstanceData_PhysicochemicalData* Unload(const SESubstancePhysicochemicals& src);
protected:
  static void Serialize(const cdm::SubstanceData_PhysicochemicalData& src, SESubstancePhysicochemicals& dst);
  static void Serialize(const SESubstancePhysicochemicals& src, cdm::SubstanceData_PhysicochemicalData& dst);

public:
  virtual bool HasAcidDissociationConstant() const;
  virtual SEScalar& GetAcidDissociationConstant();
  virtual double GetAcidDissociationConstant() const;

  virtual cdm::eSubstance_BindingProtein GetBindingProtein() const;
  virtual void SetBindingProtein(cdm::eSubstance_BindingProtein state);
  virtual bool HasBindingProtein() const;
  virtual void InvalidateBindingProtein();

  virtual bool HasBloodPlasmaRatio() const;
  virtual SEScalar& GetBloodPlasmaRatio();
  virtual double GetBloodPlasmaRatio() const;

  virtual bool HasFractionUnboundInPlasma() const;
  virtual SEScalar0To1& GetFractionUnboundInPlasma();
  virtual double GetFractionUnboundInPlasma() const;

  virtual cdm::eSubstance_IonicState GetIonicState() const;
  virtual void SetIonicState(cdm::eSubstance_IonicState state);
  virtual bool HasIonicState() const;
  virtual void InvalidateIonicState();

  virtual bool HasLogP() const;
  virtual SEScalar& GetLogP();
  virtual double GetLogP() const;

  virtual bool HasOralAbsorptionRateConstant() const;
  virtual SEScalar& GetOralAbsorptionRateConstant();
  virtual double GetOralAbsorptionRateConstant() const;

protected: 
  SEScalar*                                m_AcidDissociationConstant;
  cdm::eSubstance_BindingProtein       m_BindingProtein;
  SEScalar*                                m_BloodPlasmaRatio;
  SEScalar0To1*                            m_FractionUnboundInPlasma;
  cdm::eSubstance_IonicState           m_IonicState;
  SEScalar*                                m_LogP;
  SEScalar*                                m_OralAbsorptionRateConstant;
};