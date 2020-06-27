/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEPupillaryResponse;

class CDM_DECL SESubstancePharmacodynamics : Loggable
{
  friend class PBSubstance;//friend the serialization class
public:

  SESubstancePharmacodynamics(Logger* logger);
  virtual ~SESubstancePharmacodynamics();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasBronchodilation() const;
  virtual SEScalarNegative1To1& GetBronchodilation();
  virtual double GetBronchodilation() const;

  virtual bool HasDiastolicPressureModifier() const;
  virtual SEScalarNegative1To1& GetDiastolicPressureModifier();
  virtual double GetDiastolicPressureModifier() const;

  virtual bool HasEC50() const;
  virtual SEScalarMassPerVolume& GetEC50();
  virtual double GetEC50(const MassPerVolumeUnit& unit) const;

  virtual bool HasEMaxShapeParameter() const;
  virtual SEScalar& GetEMaxShapeParameter();
  virtual double GetEMaxShapeParameter() const;

  virtual bool HasHeartRateModifier() const;
  virtual SEScalarNegative1To1& GetHeartRateModifier();
  virtual double GetHeartRateModifier() const;

  virtual bool HasNeuromuscularBlock() const;
  virtual SEScalarNegative1To1& GetNeuromuscularBlock();
  virtual double GetNeuromuscularBlock() const;

  virtual bool HasPupillaryResponse() const;
  virtual SEPupillaryResponse& GetPupillaryResponse();
  virtual const SEPupillaryResponse* GetPupillaryResponse() const;
  virtual void RemovePupillaryResponse();

  virtual bool HasRespirationRateModifier() const;
  virtual SEScalarNegative1To1& GetRespirationRateModifier();
  virtual double GetRespirationRateModifier() const;

  virtual bool HasSedation() const;
  virtual SEScalarNegative1To1& GetSedation();
  virtual double GetSedation() const;

  virtual bool HasSystolicPressureModifier() const;
  virtual SEScalarNegative1To1& GetSystolicPressureModifier();
  virtual double GetSystolicPressureModifier() const;

  virtual bool HasTidalVolumeModifier() const;
  virtual SEScalarNegative1To1& GetTidalVolumeModifier();
  virtual double GetTidalVolumeModifier() const;

  virtual bool HasTubularPermeabilityModifier() const;
  virtual SEScalarNegative1To1& GetTubularPermeabilityModifier();
  virtual double GetTubularPermeabilityModifier() const;

protected:
  virtual void CalculateDerived();

  SEScalarNegative1To1*          m_Bronchodilation;
  SEScalarNegative1To1*          m_DiastolicPressureModifier;
  SEScalarMassPerVolume*         m_EC50;
  SEScalar*                      m_EMaxShapeParameter;
  SEScalarNegative1To1*          m_HeartRateModifier;
  SEScalarNegative1To1*          m_NeuromuscularBlock;
  SEPupillaryResponse*           m_PupillaryResponse;
  SEScalarNegative1To1*          m_RespirationRateModifier;
  SEScalarNegative1To1*          m_Sedation;
  SEScalarNegative1To1*          m_SystolicPressureModifier;
  SEScalarNegative1To1*          m_TidalVolumeModifier;
  SEScalarNegative1To1*          m_TubularPermeabilityModifier;
};