/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAction;
class SEPatientAction;
class SEAcuteRespiratoryDistressSyndromeExacerbation;
class SEAcuteStress;
class SEAirwayObstruction;
class SEBrainInjury;
class SEBronchoconstriction;
class SECardiacArrest;
class SEAsthmaAttack;
class SEChestCompression;
class SEChestCompressionForce;
class SEChestCompressionForceScale;
class SEChestOcclusiveDressing;
class SEConsciousRespiration;
class SEConsumeNutrients;
class SEChronicObstructivePulmonaryDiseaseExacerbation;
class SEDyspnea;
class SEExercise;
class SEHemorrhage;
class SEImpairedAlveolarExchangeExacerbation;
class SEIntubation;
class SELobarPneumoniaExacerbation;
class SEMechanicalVentilation;
class SENeedleDecompression;
class SEPericardialEffusion;
class SEPulmonaryShuntExacerbation;
class SERespiratoryFatigue;
class SESubstanceBolus;
class SESubstanceCompoundInfusion;
class SESubstanceInfusion;
class SESupplementalOxygen;
class SETensionPneumothorax;
class SEUrinate;
class SEPatientAssessmentRequest;
class SESubstance;
class SESubstanceCompound;
class SESubstanceManager;

class CDM_DECL SEPatientActionCollection : public Loggable
{
  friend class PBEngine;//friend the serialization class
  friend class SEActionManager;
protected:
  SEPatientActionCollection(SESubstanceManager& subMgr);
public:
  ~SEPatientActionCollection();

  bool HasAcuteRespiratoryDistressSyndromeExacerbation() const;
  SEAcuteRespiratoryDistressSyndromeExacerbation& GetAcuteRespiratoryDistressSyndromeExacerbation();
  const SEAcuteRespiratoryDistressSyndromeExacerbation* GetAcuteRespiratoryDistressSyndromeExacerbation() const;
  void RemoveAcuteRespiratoryDistressSyndromeExacerbation();

  bool HasAcuteStress() const;
  SEAcuteStress& GetAcuteStress();
  const SEAcuteStress* GetAcuteStress() const;
  void RemoveAcuteStress();

  bool HasAirwayObstruction() const;
  SEAirwayObstruction& GetAirwayObstruction();
  const SEAirwayObstruction* GetAirwayObstruction() const;
  void RemoveAirwayObstruction();

  bool HasAsthmaAttack() const;
  SEAsthmaAttack& GetAsthmaAttack();
  const SEAsthmaAttack* GetAsthmaAttack() const;
  void RemoveAsthmaAttack();

  bool HasBrainInjury() const;
  SEBrainInjury& GetBrainInjury();
  const SEBrainInjury* GetBrainInjury() const;
  void RemoveBrainInjury();

  bool HasBronchoconstriction() const;
  SEBronchoconstriction& GetBronchoconstriction();
  const SEBronchoconstriction* GetBronchoconstriction() const;
  void RemoveBronchoconstriction();

  bool HasCardiacArrest() const;
  SECardiacArrest& GetCardiacArrest();
  const SECardiacArrest* GetCardiacArrest() const;
  void RemoveCardiacArrest();

  bool HasChestCompression() const;
  bool HasChestCompressionForce() const;
  SEChestCompressionForce& GetChestCompressionForce();
  const SEChestCompressionForce* GetChestCompressionForce() const;
  void RemoveChestCompressionForce();
  bool HasChestCompressionForceScale() const;
  SEChestCompressionForceScale& GetChestCompressionForceScale();
  const SEChestCompressionForceScale* GetChestCompressionForceScale() const;
  void RemoveChestCompressionForceScale();

  bool HasChestOcclusiveDressing() const;
  bool HasLeftChestOcclusiveDressing() const;
  SEChestOcclusiveDressing& GetLeftChestOcclusiveDressing();
  const SEChestOcclusiveDressing* GetLeftChestOcclusiveDressing() const;
  void RemoveLeftChestOcclusiveDressing();
  bool HasRightChestOcclusiveDressing() const;
  SEChestOcclusiveDressing& GetRightChestOcclusiveDressing();
  const SEChestOcclusiveDressing* GetRightChestOcclusiveDressing() const;
  void RemoveRightChestOcclusiveDressing();

  bool HasChronicObstructivePulmonaryDiseaseExacerbation() const;
  SEChronicObstructivePulmonaryDiseaseExacerbation& GetChronicObstructivePulmonaryDiseaseExacerbation();
  const SEChronicObstructivePulmonaryDiseaseExacerbation* GetChronicObstructivePulmonaryDiseaseExacerbation() const;
  void RemoveChronicObstructivePulmonaryDiseaseExacerbation();

  bool HasConsciousRespiration() const;
  SEConsciousRespiration& GetConsciousRespiration();
  const SEConsciousRespiration* GetConsciousRespiration() const;
  void RemoveConsciousRespiration();

  bool HasConsumeNutrients() const;
  SEConsumeNutrients& GetConsumeNutrients();
  const SEConsumeNutrients* GetConsumeNutrients() const;
  void RemoveConsumeNutrients();

  bool HasDyspnea() const;
  SEDyspnea& GetDyspnea();
  const SEDyspnea* GetDyspnea() const;
  void RemoveDyspnea();

  bool HasExercise() const;
  SEExercise& GetExercise();
  const SEExercise* GetExercise() const;
  void RemoveExercise();

  bool HasHemorrhage() const;
  bool HasHemorrhage(const std::string& cmptName) const;
  SEHemorrhage& GetHemorrhage(const std::string& cmptName);
  const SEHemorrhage* GetHemorrhage(const std::string& cmptName) const;
  const std::vector<SEHemorrhage*>& GetHemorrhages();
  const std::vector<SEHemorrhage const*>& GetHemorrhages() const;
  void RemoveHemorrhage(const std::string& cmptName);

  bool HasImpairedAlveolarExchangeExacerbation() const;
  SEImpairedAlveolarExchangeExacerbation& GetImpairedAlveolarExchangeExacerbation();
  const SEImpairedAlveolarExchangeExacerbation* GetImpairedAlveolarExchangeExacerbation() const;
  void RemoveImpairedAlveolarExchangeExacerbation();

  bool HasIntubation() const;
  SEIntubation& GetIntubation();
  const SEIntubation* GetIntubation() const;
  void RemoveIntubation();

  bool HasLobarPneumoniaExacerbation() const;
  SELobarPneumoniaExacerbation& GetLobarPneumoniaExacerbation();
  const SELobarPneumoniaExacerbation* GetLobarPneumoniaExacerbation() const;
  void RemoveLobarPneumoniaExacerbation();

  bool HasMechanicalVentilation() const;
  SEMechanicalVentilation& GetMechanicalVentilation();
  const SEMechanicalVentilation* GetMechanicalVentilation() const;
  void RemoveMechanicalVentilation();

  bool HasNeedleDecompression() const;
  bool HasLeftNeedleDecompression() const;
  SENeedleDecompression& GetLeftNeedleDecompression();
  const SENeedleDecompression* GetLeftNeedleDecompression() const;
  void RemoveLeftNeedleDecompression();

  bool HasRightNeedleDecompression() const;
  SENeedleDecompression& GetRightNeedleDecompression();
  const SENeedleDecompression* GetRightNeedleDecompression() const;
  void RemoveRightNeedleDecompression();

  bool HasPericardialEffusion() const;
  SEPericardialEffusion& GetPericardialEffusion();
  const SEPericardialEffusion* GetPericardialEffusion() const;
  void RemovePericardialEffusion();

  bool HasPulmonaryShuntExacerbation() const;
  SEPulmonaryShuntExacerbation& GetPulmonaryShuntExacerbation();
  const SEPulmonaryShuntExacerbation* GetPulmonaryShuntExacerbation() const;
  void RemovePulmonaryShuntExacerbation();

  bool HasRespiratoryFatigue() const;
  SERespiratoryFatigue& GetRespiratoryFatigue();
  const SERespiratoryFatigue* GetRespiratoryFatigue() const;
  void RemoveRespiratoryFatigue();

  bool HasSubstanceBolus() const;
  bool HasSubstanceBolus(const SESubstance& sub) const;
  SESubstanceBolus& GetSubstanceBolus(const SESubstance& sub);
  const SESubstanceBolus* GetSubstanceBolus(const SESubstance& sub) const;
  const std::vector<SESubstanceBolus*>& GetSubstanceBoluses();
  const std::vector<SESubstanceBolus const*>& GetSubstanceBoluses() const;
  void RemoveSubstanceBolus(const SESubstance& sub);

  bool HasSubstanceInfusion() const;
  bool HasSubstanceInfusion(const SESubstance& sub) const;
  SESubstanceInfusion& GetSubstanceInfusion(const SESubstance& sub);
  const SESubstanceInfusion* GetSubstanceInfusion(const SESubstance& sub) const;
  const std::vector<SESubstanceInfusion*>& GetSubstanceInfusions();
  const std::vector<SESubstanceInfusion const*>& GetSubstanceInfusions() const;
  void RemoveSubstanceInfusion(const SESubstance& sub);

  bool HasSubstanceCompoundInfusion() const;
  bool HasSubstanceCompoundInfusion(const SESubstanceCompound& sub) const;
  SESubstanceCompoundInfusion& GetSubstanceCompoundInfusion(const SESubstanceCompound& sub);
  const SESubstanceCompoundInfusion* GetSubstanceCompoundInfusion(const SESubstanceCompound& sub) const;
  const std::vector<SESubstanceCompoundInfusion*>& GetSubstanceCompoundInfusions();
  const std::vector<SESubstanceCompoundInfusion const*>&  GetSubstanceCompoundInfusions() const;
  void RemoveSubstanceCompoundInfusion(const SESubstanceCompound& sub);

  bool HasSupplementalOxygen() const;
  SESupplementalOxygen& GetSupplementalOxygen();
  const SESupplementalOxygen* GetSupplementalOxygen() const;
  void RemoveSupplementalOxygen();

  bool HasTensionPneumothorax() const;

  bool HasLeftClosedTensionPneumothorax() const;
  SETensionPneumothorax& GetLeftClosedTensionPneumothorax();
  const SETensionPneumothorax* GetLeftClosedTensionPneumothorax() const;
  void RemoveLeftClosedTensionPneumothorax();

  bool HasLeftOpenTensionPneumothorax() const;
  SETensionPneumothorax& GetLeftOpenTensionPneumothorax();
  const SETensionPneumothorax* GetLeftOpenTensionPneumothorax() const;
  void RemoveLeftOpenTensionPneumothorax();

  bool HasRightClosedTensionPneumothorax() const;
  SETensionPneumothorax& GetRightClosedTensionPneumothorax();
  const SETensionPneumothorax* GetRightClosedTensionPneumothorax() const;
  void RemoveRightClosedTensionPneumothorax();

  bool HasRightOpenTensionPneumothorax() const;
  SETensionPneumothorax& GetRightOpenTensionPneumothorax();
  const SETensionPneumothorax* GetRightOpenTensionPneumothorax() const;
  void RemoveRightOpenTensionPneumothorax();

  bool HasUrinate() const;
  SEUrinate& GetUrinate();
  const SEUrinate* GetUrinate() const;
  void RemoveUrinate();

  void GetAllActions(std::vector<const SEAction*>& v) const;
  const SEScalar* GetScalar(const std::string& actionName, const std::string& cmptName, const std::string& substance, const std::string& property);

protected:
  void Clear();
  bool ProcessAction(const SEPatientAction& action);

  SESubstanceManager&                               m_SubMgr;

  SEAcuteRespiratoryDistressSyndromeExacerbation*   m_ARDSExacerbation;
  SEAcuteStress*                                    m_AcuteStress;
  SEAirwayObstruction*                              m_AirwayObstruction;
  SEAsthmaAttack*                                   m_AsthmaAttack;
  SEBrainInjury*                                    m_BrainInjury;
  SEBronchoconstriction*                            m_Bronchoconstriction;
  SECardiacArrest*                                  m_CardiacArrest;
  SEChestCompressionForce*                          m_ChestCompressionForce;
  SEChestCompressionForceScale*                     m_ChestCompressionForceScale;
  SEChestOcclusiveDressing*                         m_LeftChestOcclusiveDressing;
  SEChestOcclusiveDressing*                         m_RightChestOcclusiveDressing;
  SEConsciousRespiration*                           m_ConsciousRespiration;
  SEConsumeNutrients*                               m_ConsumeNutrients;
  SEChronicObstructivePulmonaryDiseaseExacerbation* m_COPDExacerbation;
  SEDyspnea*                                        m_Dyspnea;
  SEExercise*                                       m_Exercise;
  SEIntubation*                                     m_Intubation;
  SEImpairedAlveolarExchangeExacerbation*           m_ImpairedAlveolarExchangeExacerbation;
  SELobarPneumoniaExacerbation*                     m_LobarPneumoniaExacerbation;
  SEMechanicalVentilation*                          m_MechanicalVentilation;
  SENeedleDecompression*                            m_LeftNeedleDecompression;
  SENeedleDecompression*                            m_RightNeedleDecompression;
  SEPericardialEffusion*                            m_PericardialEffusion;
  SEPulmonaryShuntExacerbation*                     m_PulmonaryShuntExacerbation;
  SERespiratoryFatigue*                             m_RespiratoryFatigue;
  SESupplementalOxygen*                             m_SupplementalOxygen;
  SETensionPneumothorax*                            m_LeftClosedTensionPneumothorax;
  SETensionPneumothorax*                            m_LeftOpenTensionPneumothorax;
  SETensionPneumothorax*                            m_RightClosedTensionPneumothorax;
  SETensionPneumothorax*                            m_RightOpenTensionPneumothorax;
  SEUrinate*                                        m_Urinate;

  std::vector<SEHemorrhage*>                        m_Hemorrhages;
  std::vector<SESubstanceBolus*>                    m_SubstanceBoluses;
  std::vector<SESubstanceInfusion*>                 m_SubstanceInfusions;
  std::vector<SESubstanceCompoundInfusion*>         m_SubstanceCompoundInfusions;
};
