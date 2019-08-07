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
class SEIntubation;
class SELobarPneumoniaExacerbation;
class SEMechanicalVentilation;
class SENeedleDecompression;
class SEPericardialEffusion;
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
  SEPatientActionCollection(SESubstanceManager&);
public:
  ~SEPatientActionCollection();

  bool HasAcuteRespiratoryDistressSyndromeExacerbation() const;
  SEAcuteRespiratoryDistressSyndromeExacerbation* GetAcuteRespiratoryDistressSyndromeExacerbation();
  const SEAcuteRespiratoryDistressSyndromeExacerbation* GetAcuteRespiratoryDistressSyndromeExacerbation() const;
  void RemoveAcuteRespiratoryDistressSyndromeExacerbation();

  bool HasAcuteStress() const;
  SEAcuteStress* GetAcuteStress();
  const SEAcuteStress* GetAcuteStress() const;
  void RemoveAcuteStress();

  bool HasAirwayObstruction() const;
  SEAirwayObstruction* GetAirwayObstruction();
  const SEAirwayObstruction* GetAirwayObstruction() const;
  void RemoveAirwayObstruction();

  bool HasAsthmaAttack() const;
  SEAsthmaAttack* GetAsthmaAttack();
  const SEAsthmaAttack* GetAsthmaAttack() const;
  void RemoveAsthmaAttack();

  bool HasBrainInjury() const;
  SEBrainInjury* GetBrainInjury();
  const SEBrainInjury* GetBrainInjury() const;
  void RemoveBrainInjury();

  bool HasBronchoconstriction() const;
  SEBronchoconstriction* GetBronchoconstriction();
  const SEBronchoconstriction* GetBronchoconstriction() const;
  void RemoveBronchoconstriction();

  bool HasCardiacArrest() const;
  SECardiacArrest* GetCardiacArrest();
  const SECardiacArrest* GetCardiacArrest() const;
  void RemoveCardiacArrest();

  bool HasChestCompression() const;
  void RemoveChestCompression();
  bool HasChestCompressionForce() const;
  SEChestCompressionForce* GetChestCompressionForce();
  const SEChestCompressionForce* GetChestCompressionForce() const;
  bool HasChestCompressionForceScale() const;
  SEChestCompressionForceScale* GetChestCompressionForceScale();
  const SEChestCompressionForceScale* GetChestCompressionForceScale() const;

  bool HasChestOcclusiveDressing() const;
  bool HasLeftChestOcclusiveDressing() const;
  SEChestOcclusiveDressing* GetLeftChestOcclusiveDressing();
  const SEChestOcclusiveDressing* GetLeftChestOcclusiveDressing() const;
  void RemoveLeftChestOcclusiveDressing();
  bool HasRightChestOcclusiveDressing() const;
  SEChestOcclusiveDressing* GetRightChestOcclusiveDressing();
  const SEChestOcclusiveDressing* GetRightChestOcclusiveDressing() const;
  void RemoveRightChestOcclusiveDressing();

  bool HasChronicObstructivePulmonaryDiseaseExacerbation() const;
  SEChronicObstructivePulmonaryDiseaseExacerbation* GetChronicObstructivePulmonaryDiseaseExacerbation();
  const SEChronicObstructivePulmonaryDiseaseExacerbation* GetChronicObstructivePulmonaryDiseaseExacerbation() const;
  void RemoveChronicObstructivePulmonaryDiseaseExacerbation();

  bool HasConsciousRespiration() const;
  SEConsciousRespiration* GetConsciousRespiration();
  const SEConsciousRespiration* GetConsciousRespiration() const;
  void RemoveConsciousRespiration();

  bool HasConsumeNutrients() const;
  SEConsumeNutrients* GetConsumeNutrients();
  const SEConsumeNutrients* GetConsumeNutrients() const;
  void RemoveConsumeNutrients();

  bool HasDyspnea() const;
  SEDyspnea* GetDyspnea();
  const SEDyspnea* GetDyspnea() const;
  void RemoveDyspnea();

  bool HasExercise() const;
  SEExercise* GetExercise();
  const SEExercise* GetExercise() const;
  void RemoveExercise();

  bool HasHemorrhage() const;
  const std::map<std::string, SEHemorrhage*>& GetHemorrhages() const;
  void RemoveHemorrhage(const std::string& cmpt);

  bool HasIntubation() const;
  SEIntubation* GetIntubation();
  const SEIntubation* GetIntubation() const;
  void RemoveIntubation();

  bool HasLobarPneumoniaExacerbation() const;
  SELobarPneumoniaExacerbation* GetLobarPneumoniaExacerbation();
  const SELobarPneumoniaExacerbation* GetLobarPneumoniaExacerbation() const;
  void RemoveLobarPneumoniaExacerbation();

  bool HasMechanicalVentilation() const;
  SEMechanicalVentilation* GetMechanicalVentilation();
  const SEMechanicalVentilation* GetMechanicalVentilation() const;
  void RemoveMechanicalVentilation();

  bool HasNeedleDecompression() const;
  bool HasLeftNeedleDecompression() const;
  SENeedleDecompression* GetLeftNeedleDecompression();
  const SENeedleDecompression* GetLeftNeedleDecompression() const;
  void RemoveLeftNeedleDecompression();

  bool HasRightNeedleDecompression() const;
  SENeedleDecompression* GetRightNeedleDecompression();
  const SENeedleDecompression* GetRightNeedleDecompression() const;
  void RemoveRightNeedleDecompression();

  bool HasPericardialEffusion() const;
  SEPericardialEffusion* GetPericardialEffusion();
  const SEPericardialEffusion* GetPericardialEffusion() const;
  void RemovePericardialEffusion();

  const std::map<const SESubstance*, SESubstanceBolus*>&  GetSubstanceBoluses() const;
  void RemoveSubstanceBolus(const SESubstance& sub);

  const std::map<const SESubstance*, SESubstanceInfusion*>& GetSubstanceInfusions() const;
  void RemoveSubstanceInfusion(const SESubstance& sub);

  const std::map<const SESubstanceCompound*, SESubstanceCompoundInfusion*>&  GetSubstanceCompoundInfusions() const;
  void RemoveSubstanceCompoundInfusion(const SESubstanceCompound& sub);

  bool HasSupplementalOxygen() const;
  SESupplementalOxygen* GetSupplementalOxygen();
  const SESupplementalOxygen* GetSupplementalOxygen() const;
  void RemoveSupplementalOxygen();

  bool HasTensionPneumothorax() const;

  bool HasLeftClosedTensionPneumothorax() const;
  SETensionPneumothorax* GetLeftClosedTensionPneumothorax();
  const SETensionPneumothorax* GetLeftClosedTensionPneumothorax() const;
  void RemoveLeftClosedTensionPneumothorax();

  bool HasLeftOpenTensionPneumothorax() const;
  SETensionPneumothorax* GetLeftOpenTensionPneumothorax();
  const SETensionPneumothorax* GetLeftOpenTensionPneumothorax() const;
  void RemoveLeftOpenTensionPneumothorax();

  bool HasRightClosedTensionPneumothorax() const;
  SETensionPneumothorax* GetRightClosedTensionPneumothorax();
  const SETensionPneumothorax* GetRightClosedTensionPneumothorax() const;
  void RemoveRightClosedTensionPneumothorax();

  bool HasRightOpenTensionPneumothorax() const;
  SETensionPneumothorax* GetRightOpenTensionPneumothorax();
  const SETensionPneumothorax* GetRightOpenTensionPneumothorax() const;
  void RemoveRightOpenTensionPneumothorax();

  bool HasUrinate() const;
  SEUrinate* GetUrinate();
  const SEUrinate* GetUrinate() const;
  void RemoveUrinate();

  void GetAllActions(std::vector<const SEAction*>& v) const;

protected:
  void Clear();
  bool ProcessAction(const SEPatientAction& action);

  SEAcuteRespiratoryDistressSyndromeExacerbation*   m_ARDSExacerbation;
  SEAcuteStress*                                    m_AcuteStress;
  SEAirwayObstruction*                              m_AirwayObstruction;
  SEAsthmaAttack*                                   m_AsthmaAttack;
  SEBrainInjury*                                    m_BrainInjury;
  SEBronchoconstriction*                            m_Bronchoconstriction;
  SECardiacArrest*                                  m_CardiacArrest;
  SEChestCompression*                               m_ChestCompression;
  SEChestOcclusiveDressing*                         m_LeftChestOcclusiveDressing;
  SEChestOcclusiveDressing*                         m_RightChestOcclusiveDressing;
  SEConsciousRespiration*                           m_ConsciousRespiration;
  SEConsumeNutrients*                               m_ConsumeNutrients;
  SEChronicObstructivePulmonaryDiseaseExacerbation* m_COPDExacerbation;
  SEDyspnea*                                        m_Dyspnea;
  SEExercise*                                       m_Exercise;
  SEIntubation*                                     m_Intubation;
  SELobarPneumoniaExacerbation*                     m_LobarPneumoniaExacerbation;
  SEMechanicalVentilation*                          m_MechanicalVentilation;
  SENeedleDecompression*                            m_LeftNeedleDecompression;
  SENeedleDecompression*                            m_RightNeedleDecompression;
  SEPericardialEffusion*                            m_PericardialEffusion;
  SESupplementalOxygen*                             m_SupplementalOxygen;
  SETensionPneumothorax*                            m_LeftClosedTensionPneumothorax;
  SETensionPneumothorax*                            m_LeftOpenTensionPneumothorax;
  SETensionPneumothorax*                            m_RightClosedTensionPneumothorax;
  SETensionPneumothorax*                            m_RightOpenTensionPneumothorax;
  SEUrinate*                                        m_Urinate;

  std::map<std::string, SEHemorrhage*>                              m_Hemorrhages;
  std::map<const SESubstance*,SESubstanceBolus*>                    m_SubstanceBolus;
  std::map<const SESubstance*,SESubstanceInfusion*>                 m_SubstanceInfusions;
  std::map<const SESubstanceCompound*,SESubstanceCompoundInfusion*> m_SubstanceCompoundInfusions;

  SESubstanceManager& m_Substances;
};
