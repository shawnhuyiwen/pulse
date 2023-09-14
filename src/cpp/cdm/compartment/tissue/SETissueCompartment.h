/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/compartment/SECompartment.h"
class SESubstance;
class SESubstanceManager;

class CDM_DECL SETissueCompartment : public SECompartment
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SETissueCompartment(const std::string& name,
                      SELiquidCompartment& extracellular,
                      SELiquidCompartment& intracellular, Logger* logger);
public:
  virtual ~SETissueCompartment();

  void Clear() override;

  const SEScalar* GetScalar(const std::string& name) override;

  // Children on tissue is not supported at this time
  bool HasChildren() const override { return false; }
  virtual const std::vector<SETissueCompartment*>& GetChildren() = delete;
  virtual const std::vector<SETissueCompartment*>& GetLeaves() = delete;

  virtual SELiquidCompartment& GetExtracellular() { return m_Extracellular; }
  virtual const SELiquidCompartment& GetExtracellular() const { return m_Extracellular; }
  virtual SELiquidCompartment& GetIntracellular() { return m_Intracellular; }
  virtual const SELiquidCompartment& GetIntracellular() const { return m_Intracellular; }

  virtual void StateChange() override;

  virtual bool HasAcidicPhospohlipidConcentration() const;
  virtual SEScalarMassPerMass& GetAcidicPhospohlipidConcentration();
  virtual double GetAcidicPhospohlipidConcentration(const MassPerMassUnit& unit) const;

  virtual bool HasMatrixVolume() const;
  virtual SEScalarVolume& GetMatrixVolume();
  virtual double GetMatrixVolume(const VolumeUnit& unit) const;

  virtual bool HasNeutralLipidsVolumeFraction() const;
  virtual SEScalar0To1& GetNeutralLipidsVolumeFraction();
  virtual double GetNeutralLipidsVolumeFraction() const;

  virtual bool HasNeutralPhospholipidsVolumeFraction() const;
  virtual SEScalar0To1& GetNeutralPhospholipidsVolumeFraction();
  virtual double GetNeutralPhospholipidsVolumeFraction() const;

  virtual bool HasTissueToPlasmaAlbuminRatio() const;
  virtual SEScalar& GetTissueToPlasmaAlbuminRatio();
  virtual double GetTissueToPlasmaAlbuminRatio() const;

  virtual bool HasTissueToPlasmaAlphaAcidGlycoproteinRatio() const;
  virtual SEScalar& GetTissueToPlasmaAlphaAcidGlycoproteinRatio();
  virtual double GetTissueToPlasmaAlphaAcidGlycoproteinRatio() const;

  virtual bool HasTissueToPlasmaLipoproteinRatio() const;
  virtual SEScalar& GetTissueToPlasmaLipoproteinRatio();
  virtual double GetTissueToPlasmaLipoproteinRatio() const;

  virtual bool HasTotalMass() const;
  virtual SEScalarMass& GetTotalMass();
  virtual double GetTotalMass(const MassUnit& unit) const;

  virtual const SEScalarVolume& GetTotalVolume();
  virtual double GetTotalVolume(const VolumeUnit& unit) const;

protected:

  SEScalarMassPerMass*   m_AcidicPhospohlipidConcentration;
  SEScalarVolume*        m_MatrixVolume;
  SEScalar0To1*          m_NeutralLipidsVolumeFraction;
  SEScalar0To1*          m_NeutralPhospholipidsVolumeFraction;
  SEScalar*              m_TissueToPlasmaAlbuminRatio;
  SEScalar*              m_TissueToPlasmaAlphaAcidGlycoproteinRatio;
  SEScalar*              m_TissueToPlasmaLipoproteinRatio;
  SEScalarMass*          m_TotalMass;

  SEScalarVolume*        m_TotalVolume;
  SELiquidCompartment&   m_Extracellular;
  SELiquidCompartment&   m_Intracellular;
};
