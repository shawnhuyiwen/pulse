/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceClearanceAnatomyEffect;

enum class RenalDynamic{ NullDynamic, Clearance, Regulation };
class CDM_DECL SESubstanceClearance : public Loggable
{
  friend class PBSubstance;//friend the serialization class
public:

  SESubstanceClearance(Logger* logger);
  virtual ~SESubstanceClearance();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasSystemic() const { return m_hasSystemic; }
  virtual void SetSystemic(bool b){ m_hasSystemic=b; }

  virtual eCharge GetChargeInBlood() const;
  virtual void SetChargeInBlood(eCharge type);
  virtual bool HasChargeInBlood() const;
  virtual void InvalidateChargeInBlood();

  virtual bool HasFractionExcretedInFeces() const;
  virtual SEScalar0To1& GetFractionExcretedInFeces();
  virtual double GetFractionExcretedInFeces() const;

  virtual bool HasFractionExcretedInUrine() const;
  virtual SEScalar0To1& GetFractionExcretedInUrine();
  virtual double GetFractionExcretedInUrine() const;

  virtual bool HasFractionMetabolizedInGut() const;
  virtual SEScalar0To1& GetFractionMetabolizedInGut();
  virtual double GetFractionMetabolizedInGut() const;

  virtual bool HasFractionUnboundInPlasma() const;
  virtual SEScalar0To1& GetFractionUnboundInPlasma();
  virtual double GetFractionUnboundInPlasma() const;

  virtual bool HasGlomerularFilterability() const;
  virtual SEScalar& GetGlomerularFilterability();
  virtual double GetGlomerularFilterability() const;

  virtual bool HasIntrinsicClearance() const;
  virtual SEScalarVolumePerTimeMass& GetIntrinsicClearance();
  virtual double GetIntrinsicClearance(const VolumePerTimeMassUnit& unit) const;

  virtual RenalDynamic GetRenalDynamic() const;
  virtual void SetRenalDynamic(RenalDynamic d);
  virtual bool HasRenalDynamic() const;
  virtual void InvalidateRenalDynamic();

  virtual bool HasRenalClearance() const;
  virtual SEScalarVolumePerTimeMass& GetRenalClearance();
  virtual double GetRenalClearance(const VolumePerTimeMassUnit& unit) const;

  virtual bool HasRenalReabsorptionRatio() const;
  virtual SEScalar& GetRenalReabsorptionRatio();
  virtual double GetRenalReabsorptionRatio() const;

  virtual bool HasRenalTransportMaximum() const;
  virtual SEScalarMassPerTime& GetRenalTransportMaximum();
  virtual double GetRenalTransportMaximum(const MassPerTimeUnit& unit) const;

  virtual bool HasRenalFiltrationRate() const;
  virtual SEScalarMassPerTime& GetRenalFiltrationRate();
  virtual double GetRenalFiltrationRate(const MassPerTimeUnit& unit) const;

  virtual bool HasRenalReabsorptionRate() const;
  virtual SEScalarMassPerTime& GetRenalReabsorptionRate();
  virtual double GetRenalReabsorptionRate(const MassPerTimeUnit& unit) const;

  virtual bool HasRenalExcretionRate() const;
  virtual SEScalarMassPerTime& GetRenalExcretionRate();
  virtual double GetRenalExcretionRate(const MassPerTimeUnit& unit) const;

  virtual bool HasSystemicClearance() const;
  virtual SEScalarVolumePerTimeMass& GetSystemicClearance();
  virtual double GetSystemicClearance(const VolumePerTimeMassUnit& unit) const;

protected: 

  bool                            m_hasSystemic;
  eCharge          m_ChargeInBlood;
  SEScalar0To1*                m_FractionExcretedInFeces;
  SEScalar0To1*                m_FractionExcretedInUrine;
  SEScalar0To1*                m_FractionMetabolizedInGut;
  SEScalar0To1*                m_FractionUnboundInPlasma;
  SEScalarVolumePerTimeMass*      m_IntrinsicClearance;
  RenalDynamic                    m_RenalDynamic;
  SEScalarVolumePerTimeMass*      m_RenalClearance;
  SEScalar*                       m_RenalReabsorptionRatio;
  SEScalarMassPerTime*            m_RenalTransportMaximum;
  SEScalarMassPerTime*            m_RenalFiltrationRate;
  SEScalarMassPerTime*            m_RenalReabsorptionRate;
  SEScalarMassPerTime*            m_RenalExcretionRate;
  SEScalar*                       m_GlomerularFilterability;
  SEScalarVolumePerTimeMass*      m_SystemicClearance;
};