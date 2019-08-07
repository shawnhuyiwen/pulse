/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(PatientAction)
CDM_BIND_DECL2(AcuteRespiratoryDistressSyndromeExacerbation)
CDM_BIND_DECL2(AcuteStress)
CDM_BIND_DECL2(AirwayObstruction)
CDM_BIND_DECL2(AsthmaAttack)
CDM_BIND_DECL2(BrainInjury)
CDM_BIND_DECL2(BreathHold)
CDM_BIND_DECL2(Bronchoconstriction)
CDM_BIND_DECL2(CardiacArrest)
CDM_BIND_DECL2(ChestCompressionForce)
CDM_BIND_DECL2(ChestCompressionForceScale)
CDM_BIND_DECL2(ChestOcclusiveDressing)
CDM_BIND_DECL2(ChronicObstructivePulmonaryDiseaseExacerbation)
CDM_BIND_DECL2(ConsciousRespiration)
CDM_BIND_DECL2(ConsumeNutrients)
CDM_BIND_DECL2(Dyspnea)
CDM_BIND_DECL2(Exercise)
CDM_BIND_DECL2(ForcedExhale)
CDM_BIND_DECL2(ForcedInhale)
CDM_BIND_DECL2(Hemorrhage)
CDM_BIND_DECL2(Intubation)
CDM_BIND_DECL2(LobarPneumoniaExacerbation)
CDM_BIND_DECL2(MechanicalVentilation)
CDM_BIND_DECL2(NeedleDecompression)
CDM_BIND_DECL2(PatientAssessmentRequest)
CDM_BIND_DECL2(PericardialEffusion)
CDM_BIND_DECL2(SubstanceBolus)
CDM_BIND_DECL2(SubstanceBolusState)
CDM_BIND_DECL2(SubstanceCompoundInfusion)
CDM_BIND_DECL2(SubstanceInfusion)
CDM_BIND_DECL2(SupplementalOxygen)
CDM_BIND_DECL2(TensionPneumothorax)
CDM_BIND_DECL2(Urinate)
CDM_BIND_DECL2(UseInhaler)
CDM_BIND_DECL(AnyPatientActionData)
class SESubstanceManager;

class CDM_DECL PBPatientAction
{
public:
  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SEPatientAction* Load(const cdm::AnyPatientActionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyPatientActionData* Unload(const SEPatientAction& action);
  static void Serialize(const cdm::PatientActionData& src, SEPatientAction& dst);
  static void Serialize(const SEPatientAction& src, cdm::PatientActionData& dst);

  static void Load(const cdm::AcuteRespiratoryDistressSyndromeExacerbationData& src, SEAcuteRespiratoryDistressSyndromeExacerbation& dst);
  static cdm::AcuteRespiratoryDistressSyndromeExacerbationData* Unload(const SEAcuteRespiratoryDistressSyndromeExacerbation& src);
  static void Serialize(const cdm::AcuteRespiratoryDistressSyndromeExacerbationData& src, SEAcuteRespiratoryDistressSyndromeExacerbation& dst);
  static void Serialize(const SEAcuteRespiratoryDistressSyndromeExacerbation& src, cdm::AcuteRespiratoryDistressSyndromeExacerbationData& dst);
  static void Copy(const SEAcuteRespiratoryDistressSyndromeExacerbation& src, SEAcuteRespiratoryDistressSyndromeExacerbation& dst);

  static void Load(const cdm::AcuteStressData& src, SEAcuteStress& dst);
  static cdm::AcuteStressData* Unload(const SEAcuteStress& src);
  static void Serialize(const cdm::AcuteStressData& src, SEAcuteStress& dst);
  static void Serialize(const SEAcuteStress& src, cdm::AcuteStressData& dst);
  static void Copy(const SEAcuteStress& src, SEAcuteStress& dst);

  static void Load(const cdm::AirwayObstructionData& src, SEAirwayObstruction& dst);
  static cdm::AirwayObstructionData* Unload(const SEAirwayObstruction& src);
  static void Serialize(const cdm::AirwayObstructionData& src, SEAirwayObstruction& dst);
  static void Serialize(const SEAirwayObstruction& src, cdm::AirwayObstructionData& dst);
  static void Copy(const SEAirwayObstruction& src, SEAirwayObstruction& dst);

  static void Load(const cdm::AsthmaAttackData& src, SEAsthmaAttack& dst);
  static cdm::AsthmaAttackData* Unload(const SEAsthmaAttack& src);
  static void Serialize(const cdm::AsthmaAttackData& src, SEAsthmaAttack& dst);
  static void Serialize(const SEAsthmaAttack& src, cdm::AsthmaAttackData& dst);
  static void Copy(const SEAsthmaAttack& src, SEAsthmaAttack& dst);

  static void Load(const cdm::BrainInjuryData& src, SEBrainInjury& dst);
  static cdm::BrainInjuryData* Unload(const SEBrainInjury& src);
  static void Serialize(const cdm::BrainInjuryData& src, SEBrainInjury& dst);
  static void Serialize(const SEBrainInjury& src, cdm::BrainInjuryData& dst);
  static void Copy(const SEBrainInjury& src, SEBrainInjury& dst);

  static void Load(const cdm::BreathHoldData& src, SEBreathHold& dst);
  static cdm::BreathHoldData* Unload(const SEBreathHold& src);
  static void Serialize(const cdm::BreathHoldData& src, SEBreathHold& dst);
  static void Serialize(const SEBreathHold& src, cdm::BreathHoldData& dst);
  static void Copy(const SEBreathHold& src, SEBreathHold& dst);

  static void Load(const cdm::BronchoconstrictionData& src, SEBronchoconstriction& dst);
  static cdm::BronchoconstrictionData* Unload(const SEBronchoconstriction& src);
  static void Serialize(const cdm::BronchoconstrictionData& src, SEBronchoconstriction& dst);
  static void Serialize(const SEBronchoconstriction& src, cdm::BronchoconstrictionData& dst);
  static void Copy(const SEBronchoconstriction& src, SEBronchoconstriction& dst);

  static void Load(const cdm::CardiacArrestData& src, SECardiacArrest& dst);
  static cdm::CardiacArrestData* Unload(const SECardiacArrest& src);
  static void Serialize(const cdm::CardiacArrestData& src, SECardiacArrest& dst);
  static void Serialize(const SECardiacArrest& src, cdm::CardiacArrestData& dst);
  static void Copy(const SECardiacArrest& src, SECardiacArrest& dst);

  static void Load(const cdm::ChestCompressionForceData& src, SEChestCompressionForce& dst);
  static cdm::ChestCompressionForceData* Unload(const SEChestCompressionForce& src);
  static void Serialize(const cdm::ChestCompressionForceData& src, SEChestCompressionForce& dst);
  static void Serialize(const SEChestCompressionForce& src, cdm::ChestCompressionForceData& dst);
  static void Copy(const SEChestCompressionForce& src, SEChestCompressionForce& dst);

  static void Load(const cdm::ChestCompressionForceScaleData& src, SEChestCompressionForceScale& dst);
  static cdm::ChestCompressionForceScaleData* Unload(const SEChestCompressionForceScale& src);
  static void Serialize(const cdm::ChestCompressionForceScaleData& src, SEChestCompressionForceScale& dst);
  static void Serialize(const SEChestCompressionForceScale& src, cdm::ChestCompressionForceScaleData& dst);
  static void Copy(const SEChestCompressionForceScale& src, SEChestCompressionForceScale& dst);

  static void Load(const cdm::ChestOcclusiveDressingData& src, SEChestOcclusiveDressing& dst);
  static cdm::ChestOcclusiveDressingData* Unload(const SEChestOcclusiveDressing& src);
  static void Serialize(const cdm::ChestOcclusiveDressingData& src, SEChestOcclusiveDressing& dst);
  static void Serialize(const SEChestOcclusiveDressing& src, cdm::ChestOcclusiveDressingData& dst);
  static void Copy(const SEChestOcclusiveDressing& src, SEChestOcclusiveDressing& dst);

  static void Load(const cdm::ChronicObstructivePulmonaryDiseaseExacerbationData& src, SEChronicObstructivePulmonaryDiseaseExacerbation& dst);
  static cdm::ChronicObstructivePulmonaryDiseaseExacerbationData* Unload(const SEChronicObstructivePulmonaryDiseaseExacerbation& src);
  static void Serialize(const cdm::ChronicObstructivePulmonaryDiseaseExacerbationData& src, SEChronicObstructivePulmonaryDiseaseExacerbation& dst);
  static void Serialize(const SEChronicObstructivePulmonaryDiseaseExacerbation& src, cdm::ChronicObstructivePulmonaryDiseaseExacerbationData& dst);
  static void Copy(const SEChronicObstructivePulmonaryDiseaseExacerbation& src, SEChronicObstructivePulmonaryDiseaseExacerbation& dst);

  static void Load(const cdm::ConsciousRespirationData& src, SEConsciousRespiration& dst);
  static cdm::ConsciousRespirationData* Unload(const SEConsciousRespiration& src);
  static void Serialize(const cdm::ConsciousRespirationData& src, SEConsciousRespiration& dst);
  static void Serialize(const SEConsciousRespiration& src, cdm::ConsciousRespirationData& dst);
  static void Copy(const SEConsciousRespiration& src, SEConsciousRespiration& dst);

  static void Load(const cdm::ConsumeNutrientsData& src, SEConsumeNutrients& dst);
  static cdm::ConsumeNutrientsData* Unload(const SEConsumeNutrients& src);
  static void Serialize(const cdm::ConsumeNutrientsData& src, SEConsumeNutrients& dst);
  static void Serialize(const SEConsumeNutrients& src, cdm::ConsumeNutrientsData& dst);
  static void Copy(const SEConsumeNutrients& src, SEConsumeNutrients& dst);

  static void Load(const cdm::DyspneaData& src, SEDyspnea& dst);
  static cdm::DyspneaData* Unload(const SEDyspnea& src);
  static void Serialize(const cdm::DyspneaData& src, SEDyspnea& dst);
  static void Serialize(const SEDyspnea& src, cdm::DyspneaData& dst);
  static void Copy(const SEDyspnea& src, SEDyspnea& dst);

  static void Load(const cdm::ExerciseData& src, SEExercise& dst);
  static cdm::ExerciseData* Unload(const SEExercise& src);
  static void Serialize(const cdm::ExerciseData& src, SEExercise& dst);
  static void Serialize(const SEExercise& src, cdm::ExerciseData& dst);
  static void Copy(const SEExercise& src, SEExercise& dst);

  static void Load(const cdm::ForcedExhaleData& src, SEForcedExhale& dst);
  static cdm::ForcedExhaleData* Unload(const SEForcedExhale& src);
  static void Serialize(const cdm::ForcedExhaleData& src, SEForcedExhale& dst);
  static void Serialize(const SEForcedExhale& src, cdm::ForcedExhaleData& dst);
  static void Copy(const SEForcedExhale& src, SEForcedExhale& dst);

  static void Load(const cdm::ForcedInhaleData& src, SEForcedInhale& dst);
  static cdm::ForcedInhaleData* Unload(const SEForcedInhale& src);
  static void Serialize(const cdm::ForcedInhaleData& src, SEForcedInhale& dst);
  static void Serialize(const SEForcedInhale& src, cdm::ForcedInhaleData& dst);
  static void Copy(const SEForcedInhale& src, SEForcedInhale& dst);

  static void Load(const cdm::HemorrhageData& src, SEHemorrhage& dst);
  static cdm::HemorrhageData* Unload(const SEHemorrhage& src);
  static void Serialize(const cdm::HemorrhageData& src, SEHemorrhage& dst);
  static void Serialize(const SEHemorrhage& src, cdm::HemorrhageData& dst);
  static void Copy(const SEHemorrhage& src, SEHemorrhage& dst);

  static void Load(const cdm::IntubationData& src, SEIntubation& dst);
  static cdm::IntubationData* Unload(const SEIntubation& src);
  static void Serialize(const cdm::IntubationData& src, SEIntubation& dst);
  static void Serialize(const SEIntubation& src, cdm::IntubationData& dst);
  static void Copy(const SEIntubation& src, SEIntubation& dst);

  static void Load(const cdm::LobarPneumoniaExacerbationData& src, SELobarPneumoniaExacerbation& dst);
  static cdm::LobarPneumoniaExacerbationData* Unload(const SELobarPneumoniaExacerbation& src);
  static void Serialize(const cdm::LobarPneumoniaExacerbationData& src, SELobarPneumoniaExacerbation& dst);
  static void Serialize(const SELobarPneumoniaExacerbation& src, cdm::LobarPneumoniaExacerbationData& dst);
  static void Copy(const SELobarPneumoniaExacerbation& src, SELobarPneumoniaExacerbation& dst);

  static void Load(const cdm::MechanicalVentilationData& src, SEMechanicalVentilation& dst, const SESubstanceManager& subMgr);
  static cdm::MechanicalVentilationData* Unload(const SEMechanicalVentilation& src);
  static void Serialize(const cdm::MechanicalVentilationData& src, SEMechanicalVentilation& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEMechanicalVentilation& src, cdm::MechanicalVentilationData& dst);
  static void Copy(const SEMechanicalVentilation& src, SEMechanicalVentilation& dst, const SESubstanceManager& subMgr);

  static void Load(const cdm::NeedleDecompressionData& src, SENeedleDecompression& dst);
  static cdm::NeedleDecompressionData* Unload(const SENeedleDecompression& src);
  static void Serialize(const cdm::NeedleDecompressionData& src, SENeedleDecompression& dst);
  static void Serialize(const SENeedleDecompression& src, cdm::NeedleDecompressionData& dst);
  static void Copy(const SENeedleDecompression& src, SENeedleDecompression& dst);

  static void Load(const cdm::PatientAssessmentRequestData& src, SEPatientAssessmentRequest& dst);
  static cdm::PatientAssessmentRequestData* Unload(const SEPatientAssessmentRequest& src);
  static void Serialize(const cdm::PatientAssessmentRequestData& src, SEPatientAssessmentRequest& dst);
  static void Serialize(const SEPatientAssessmentRequest& src, cdm::PatientAssessmentRequestData& dst);
  static void Copy(const SEPatientAssessmentRequest& src, SEPatientAssessmentRequest& dst);

  static void Load(const cdm::PericardialEffusionData& src, SEPericardialEffusion& dst);
  static cdm::PericardialEffusionData* Unload(const SEPericardialEffusion& src);
  static void Serialize(const cdm::PericardialEffusionData& src, SEPericardialEffusion& dst);
  static void Serialize(const SEPericardialEffusion& src, cdm::PericardialEffusionData& dst);
  static void Copy(const SEPericardialEffusion& src, SEPericardialEffusion& dst);

  static void Load(const cdm::SubstanceBolusStateData& src, SESubstanceBolusState& dst);
  static cdm::SubstanceBolusStateData* Unload(const SESubstanceBolusState& src);
  static void Serialize(const cdm::SubstanceBolusStateData& src, SESubstanceBolusState& dst);
  static void Serialize(const SESubstanceBolusState& src, cdm::SubstanceBolusStateData& dst);
  static void Copy(const SESubstanceBolusState& src, SESubstanceBolusState& dst);

  static void Load(const cdm::SubstanceBolusData& src, SESubstanceBolus& dst);
  static cdm::SubstanceBolusData* Unload(const SESubstanceBolus& src);
  static void Serialize(const cdm::SubstanceBolusData& src, SESubstanceBolus& dst);
  static void Serialize(const SESubstanceBolus& src, cdm::SubstanceBolusData& dst);
  static void Copy(const SESubstanceBolus& src, SESubstanceBolus& dst);

  static void Load(const cdm::SubstanceCompoundInfusionData& src, SESubstanceCompoundInfusion& dst);
  static cdm::SubstanceCompoundInfusionData* Unload(const SESubstanceCompoundInfusion& src);
  static void Serialize(const cdm::SubstanceCompoundInfusionData& src, SESubstanceCompoundInfusion& dst);
  static void Serialize(const SESubstanceCompoundInfusion& src, cdm::SubstanceCompoundInfusionData& dst);
  static void Copy(const SESubstanceCompoundInfusion& src, SESubstanceCompoundInfusion& dst);

  static void Load(const cdm::SubstanceInfusionData& src, SESubstanceInfusion& dst);
  static cdm::SubstanceInfusionData* Unload(const SESubstanceInfusion& src);
  static void Serialize(const cdm::SubstanceInfusionData& src, SESubstanceInfusion& dst);
  static void Serialize(const SESubstanceInfusion& src, cdm::SubstanceInfusionData& dst);
  static void Copy(const SESubstanceInfusion& src, SESubstanceInfusion& dst);

  static void Load(const cdm::SupplementalOxygenData& src, SESupplementalOxygen& dst);
  static cdm::SupplementalOxygenData* Unload(const SESupplementalOxygen& src);
  static void Serialize(const cdm::SupplementalOxygenData& src, SESupplementalOxygen& dst);
  static void Serialize(const SESupplementalOxygen& src, cdm::SupplementalOxygenData& dst);
  static void Copy(const SESupplementalOxygen& src, SESupplementalOxygen& dst);

  static void Load(const cdm::TensionPneumothoraxData& src, SETensionPneumothorax& dst);
  static cdm::TensionPneumothoraxData* Unload(const SETensionPneumothorax& src);
  static void Serialize(const cdm::TensionPneumothoraxData& src, SETensionPneumothorax& dst);
  static void Serialize(const SETensionPneumothorax& src, cdm::TensionPneumothoraxData& dst);
  static void Copy(const SETensionPneumothorax& src, SETensionPneumothorax& dst);

  static void Load(const cdm::UrinateData& src, SEUrinate& dst);
  static cdm::UrinateData* Unload(const SEUrinate& src);
  static void Serialize(const cdm::UrinateData& src, SEUrinate& dst);
  static void Serialize(const SEUrinate& src, cdm::UrinateData& dst);
  static void Copy(const SEUrinate& src, SEUrinate& dst);

  static void Load(const cdm::UseInhalerData& src, SEUseInhaler& dst);
  static cdm::UseInhalerData* Unload(const SEUseInhaler& src);
  static void Serialize(const cdm::UseInhalerData& src, SEUseInhaler& dst);
  static void Serialize(const SEUseInhaler& src, cdm::UseInhalerData& dst);
  static void Copy(const SEUseInhaler& src, SEUseInhaler& dst);

};