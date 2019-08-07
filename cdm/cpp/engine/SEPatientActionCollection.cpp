/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEPatientActionCollection.h"
#include "patient/SEPatient.h"
#include "patient/actions/SEAcuteRespiratoryDistressSyndromeExacerbation.h"
#include "patient/actions/SEAcuteStress.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "patient/actions/SEBrainInjury.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "patient/actions/SECardiacArrest.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "patient/actions/SEChestCompressionForce.h"
#include "patient/actions/SEChestCompressionForceScale.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "patient/actions/SEConsciousRespiration.h"
#include "patient/actions/SEConsumeNutrients.h"
#include "patient/actions/SEChronicObstructivePulmonaryDiseaseExacerbation.h"
#include "patient/actions/SEExercise.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SELobarPneumoniaExacerbation.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SEPericardialEffusion.h"
#include "patient/actions/SESubstanceBolus.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "patient/actions/SESubstanceInfusion.h"
#include "patient/actions/SESupplementalOxygen.h"
#include "patient/actions/SETensionPneumothorax.h"
#include "patient/actions/SEUrinate.h"
#include "patient/actions/SEPatientAssessmentRequest.h"

#include "substance/SESubstanceManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceCompound.h"
#include "substance/SESubstanceConcentration.h"

#include "properties/SEScalarVolumePerTime.h"

SEPatientActionCollection::SEPatientActionCollection(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
{
  m_ARDSExacerbation = nullptr;
  m_AcuteStress = nullptr;
  m_AirwayObstruction = nullptr;
  m_AsthmaAttack = nullptr;
  m_BrainInjury = nullptr;
  m_Bronchoconstriction = nullptr;
  m_CardiacArrest = nullptr;
  m_ChestCompression = nullptr;
  m_ConsciousRespiration = nullptr;
  m_ConsumeNutrients = nullptr;
  m_COPDExacerbation = nullptr;
  m_LeftChestOcclusiveDressing = nullptr;
  m_RightChestOcclusiveDressing = nullptr;
  m_Dyspnea = nullptr;
  m_Exercise = nullptr;
  m_Intubation = nullptr;
  m_LobarPneumoniaExacerbation = nullptr;
  m_MechanicalVentilation = nullptr;
  m_LeftNeedleDecompression = nullptr;
  m_RightNeedleDecompression = nullptr;
  m_PericardialEffusion = nullptr;
  m_SupplementalOxygen = nullptr;
  m_LeftOpenTensionPneumothorax = nullptr;
  m_LeftClosedTensionPneumothorax = nullptr;
  m_RightOpenTensionPneumothorax = nullptr;
  m_RightClosedTensionPneumothorax = nullptr;
  m_Urinate = nullptr;
}

SEPatientActionCollection::~SEPatientActionCollection()
{
  Clear();
}

void SEPatientActionCollection::Clear()
{
  RemoveAcuteRespiratoryDistressSyndromeExacerbation();
  RemoveAcuteStress();
  RemoveAirwayObstruction();
  RemoveAsthmaAttack();
  RemoveBrainInjury();
  RemoveBronchoconstriction();
  RemoveChestCompression();
  RemoveCardiacArrest();
  RemoveChronicObstructivePulmonaryDiseaseExacerbation();
  RemoveConsciousRespiration();
  RemoveConsumeNutrients();
  RemoveLeftChestOcclusiveDressing();
  RemoveRightChestOcclusiveDressing();
  RemoveDyspnea();
  RemoveExercise();
  RemoveIntubation();
  RemoveLobarPneumoniaExacerbation();
  RemoveMechanicalVentilation();
  RemoveLeftNeedleDecompression();
  RemoveRightNeedleDecompression();
  RemovePericardialEffusion();
  RemoveLeftOpenTensionPneumothorax();
  RemoveLeftClosedTensionPneumothorax();
  RemoveRightOpenTensionPneumothorax();
  RemoveRightClosedTensionPneumothorax();
  RemoveUrinate();

  DELETE_MAP_SECOND(m_Hemorrhages);
  DELETE_MAP_SECOND(m_SubstanceBolus);
  DELETE_MAP_SECOND(m_SubstanceInfusions);
  DELETE_MAP_SECOND(m_SubstanceCompoundInfusions);
}

bool SEPatientActionCollection::ProcessAction(const SEPatientAction& action)
{
  const SEPatientAssessmentRequest* patientAss = dynamic_cast<const SEPatientAssessmentRequest*>(&action);
  if (patientAss != nullptr)
  {
    // TODO just add this to a list?
    // Not doing anything with this, assessment actions
    // are currently only being handled by the Engine ScenarioExec methods.
    // Which probably just write out the assessment requested to a file
    // Assessments are really a real time API thing, not really a scenario thing
    // If they are a scenario thing, its just to write out the data.
    return true;//?
  }

  // We are creating our own copy of these actions
  // There can be multiple actions in a Scenario that are associated
  // with a specific function, such as hemorrhage, and we only need
  // to have a single action in our list associated with a hemorrhage.
  // We just overwrite our saved action with the current state of that action
  // So if the 4 hemorrhage actions come in, and lower the rate each time for a 
  // certain compartment, we just update the single hemorrhage action associated 
  // with that compartment. 
  // SO, we make our own copy and manage that copy (i.e. by updating a single action)

  const SEAcuteRespiratoryDistressSyndromeExacerbation* ards = dynamic_cast<const SEAcuteRespiratoryDistressSyndromeExacerbation*>(&action);
  if (ards != nullptr)
  {
    if (m_ARDSExacerbation == nullptr)
      m_ARDSExacerbation = new SEAcuteRespiratoryDistressSyndromeExacerbation();
    m_ARDSExacerbation->Copy(*ards);
    if (!m_ARDSExacerbation->IsActive())
      RemoveAcuteRespiratoryDistressSyndromeExacerbation();
    return true;
  }

  const SEAcuteStress* aStress = dynamic_cast<const SEAcuteStress*>(&action);
  if (aStress != nullptr)
  {
    if (m_AcuteStress == nullptr)
      m_AcuteStress = new SEAcuteStress();
    m_AcuteStress->Copy(*aStress);
    if (!m_AcuteStress->IsActive())
      RemoveAcuteStress();
    return true;
  }

  const SEAirwayObstruction* airwayObst = dynamic_cast<const SEAirwayObstruction*>(&action);
  if (airwayObst != nullptr)
  {
    if (m_AirwayObstruction == nullptr)
      m_AirwayObstruction = new SEAirwayObstruction();
    m_AirwayObstruction->Copy(*airwayObst);
    if (!m_AirwayObstruction->IsActive())
      RemoveAirwayObstruction();
    return true;
  }

  const SEAsthmaAttack* asthmaattack = dynamic_cast<const SEAsthmaAttack*>(&action);
  if (asthmaattack != nullptr)
  {
    if (m_AsthmaAttack == nullptr)
      m_AsthmaAttack = new SEAsthmaAttack();
    m_AsthmaAttack->Copy(*asthmaattack);
    if (!m_AsthmaAttack->IsActive())
      RemoveAsthmaAttack();
    return true;
  }

  const SEBrainInjury* brainInjury = dynamic_cast<const SEBrainInjury*>(&action);
  if (brainInjury != nullptr)
  {
    if (m_BrainInjury == nullptr)
      m_BrainInjury = new SEBrainInjury();
    m_BrainInjury->Copy(*brainInjury);
    if (!m_BrainInjury->IsActive())
      RemoveBrainInjury();
    return true;
  }

  const SEBronchoconstriction* bronchoconstr = dynamic_cast<const SEBronchoconstriction*>(&action);
  if (bronchoconstr != nullptr)
  {
    if (m_Bronchoconstriction == nullptr)
      m_Bronchoconstriction = new SEBronchoconstriction();
    m_Bronchoconstriction->Copy(*bronchoconstr);
    if (!m_Bronchoconstriction->IsActive())
      RemoveBronchoconstriction();
    return true;
  }

  const SECardiacArrest* cardiacarrest = dynamic_cast<const SECardiacArrest*>(&action);
  if (cardiacarrest != nullptr)
  {
    if (m_CardiacArrest == nullptr)
      m_CardiacArrest = new SECardiacArrest();
    m_CardiacArrest->Copy(*cardiacarrest);
    if (!m_CardiacArrest->IsActive())
      RemoveCardiacArrest();
    return true;
  }

  const SEChestCompression* chestcomp = dynamic_cast<const SEChestCompression*>(&action);
  if (chestcomp != nullptr)
  {
    const SEChestCompressionForce* cprForce = dynamic_cast<const SEChestCompressionForce*>(chestcomp);
    if (cprForce != nullptr)
    {
      if (HasChestCompressionForceScale())
        RemoveChestCompression();
      if (m_ChestCompression == nullptr)
        m_ChestCompression = new SEChestCompressionForce();
      ((SEChestCompressionForce*)m_ChestCompression)->Copy(*cprForce);
      if (!m_ChestCompression->IsActive())
        RemoveChestCompression();
      return true;
    }
    const SEChestCompressionForceScale* cprScale = dynamic_cast<const SEChestCompressionForceScale*>(chestcomp);
    if (cprScale != nullptr)
    {
      if (HasChestCompressionForce())
        RemoveChestCompression();
      if (m_ChestCompression == nullptr)
        m_ChestCompression = new SEChestCompressionForceScale();
      ((SEChestCompressionForceScale*)m_ChestCompression)->Copy(*cprScale);
      if (!m_ChestCompression->IsActive())
        RemoveChestCompression();
      return true;
    }
    Error("Unknown Chest Compression Type");
    /// \error Unknown ChestCompression type 
    return false;
  }

  const SEChestOcclusiveDressing* chestOccl = dynamic_cast<const SEChestOcclusiveDressing*>(&action);
  if (chestOccl != nullptr)
  {
    if (chestOccl->GetSide() == eSide::Left)
    {
      if (m_LeftChestOcclusiveDressing == nullptr)
        m_LeftChestOcclusiveDressing = new SEChestOcclusiveDressing();
      m_LeftChestOcclusiveDressing->Copy(*chestOccl);
      if (!m_LeftChestOcclusiveDressing->IsActive())
        RemoveLeftChestOcclusiveDressing();
      return true;
    }
    else if (chestOccl->GetSide() == eSide::Right)
    {
      if (m_RightChestOcclusiveDressing == nullptr)
        m_RightChestOcclusiveDressing = new SEChestOcclusiveDressing();
      m_RightChestOcclusiveDressing->Copy(*chestOccl);
      if (!m_RightChestOcclusiveDressing->IsActive())
        RemoveRightChestOcclusiveDressing();
      return true;
    }
  }

  const SEChronicObstructivePulmonaryDiseaseExacerbation* copd = dynamic_cast<const SEChronicObstructivePulmonaryDiseaseExacerbation*>(&action);
  if (copd != nullptr)
  {
    if (m_COPDExacerbation == nullptr)
      m_COPDExacerbation = new SEChronicObstructivePulmonaryDiseaseExacerbation();
    m_COPDExacerbation->Copy(*copd);
    if (!m_COPDExacerbation->IsActive())
      RemoveChronicObstructivePulmonaryDiseaseExacerbation();
    return true;
  }

  const SEConsciousRespiration* conResp = dynamic_cast<const SEConsciousRespiration*>(&action);
  if (conResp != nullptr)
  {
    if (m_ConsciousRespiration == nullptr)
      m_ConsciousRespiration = new SEConsciousRespiration();
    m_ConsciousRespiration->Copy(*conResp);
    if (!m_ConsciousRespiration->IsActive())
      RemoveConsciousRespiration();
    return true;
  }

  const SEConsumeNutrients* consume = dynamic_cast<const SEConsumeNutrients*>(&action);
  if (consume != nullptr)
  {
    if (m_ConsumeNutrients == nullptr)
      m_ConsumeNutrients = new SEConsumeNutrients();
    m_ConsumeNutrients->Copy(*consume);
    if (!m_ConsumeNutrients->IsActive())
      RemoveConsumeNutrients();
    return true;
  }

  const SEDyspnea* Dyspnea = dynamic_cast<const SEDyspnea*>(&action);
  if (Dyspnea != nullptr)
  {
    if (m_Dyspnea == nullptr)
      m_Dyspnea = new SEDyspnea();
    m_Dyspnea->Copy(*Dyspnea);
    if (!m_Dyspnea->IsActive())
      RemoveDyspnea();
    return true;
  }

  const SEExercise* exercise = dynamic_cast<const SEExercise*>(&action);
  if (exercise != nullptr)
  {
    if (m_Exercise == nullptr)
      m_Exercise = new SEExercise();
    m_Exercise->Copy(*exercise);
    if (!m_Exercise->IsActive())
      RemoveExercise();
    return true;
  }

  const SEHemorrhage* hem = dynamic_cast<const SEHemorrhage*>(&action);
  if (hem != nullptr)
  {
    SEHemorrhage* myHem = m_Hemorrhages[hem->GetCompartment()];
    if (myHem == nullptr)
    {
      myHem = new SEHemorrhage();
      m_Hemorrhages[hem->GetCompartment()] = myHem;
    }
    myHem->Copy(*hem);
    if (!myHem->IsActive())
      RemoveHemorrhage(hem->GetCompartment());
    return true;
  }

  const SEIntubation* intubation = dynamic_cast<const SEIntubation*>(&action);
  if (intubation != nullptr)
  {
    if (m_Intubation == nullptr)
      m_Intubation = new SEIntubation();
    m_Intubation->Copy(*intubation);
    if (!m_Intubation->IsActive())
      RemoveIntubation();
    return true;
  }

  const SELobarPneumoniaExacerbation* lp = dynamic_cast<const SELobarPneumoniaExacerbation*>(&action);
  if (lp != nullptr)
  {
    if (m_LobarPneumoniaExacerbation == nullptr)
      m_LobarPneumoniaExacerbation = new SELobarPneumoniaExacerbation();
    m_LobarPneumoniaExacerbation->Copy(*lp);
    if (!m_LobarPneumoniaExacerbation->IsActive())
      RemoveLobarPneumoniaExacerbation();
    return true;
  }

  const SEMechanicalVentilation* mvData = dynamic_cast<const SEMechanicalVentilation*>(&action);
  if (mvData != nullptr)
  {
    if (m_MechanicalVentilation == nullptr)
      m_MechanicalVentilation = new SEMechanicalVentilation();
    m_MechanicalVentilation->Copy(*mvData,m_Substances);
    if (!m_MechanicalVentilation->IsActive())
      RemoveMechanicalVentilation();
    return true;
  }

  const SENeedleDecompression* needleDecomp = dynamic_cast<const SENeedleDecompression*>(&action);
  if (needleDecomp != nullptr)
  {
    if (needleDecomp->GetSide() == eSide::Left)
    {
      if (m_LeftNeedleDecompression == nullptr)
        m_LeftNeedleDecompression = new SENeedleDecompression();
      m_LeftNeedleDecompression->Copy(*needleDecomp);
      if (!m_LeftNeedleDecompression->IsActive())
        RemoveLeftNeedleDecompression();
      return true;
    }
    else if (needleDecomp->GetSide() == eSide::Right)
    {
      if (m_RightNeedleDecompression == nullptr)
        m_RightNeedleDecompression = new SENeedleDecompression();
      m_RightNeedleDecompression->Copy(*needleDecomp);
      if (!m_RightNeedleDecompression->IsActive())
        RemoveRightNeedleDecompression();
      return true;
    }
    Error("Unknown Needle Decompression Type");
    return false;
  }

  const SEPericardialEffusion* pericardialEff = dynamic_cast<const SEPericardialEffusion*>(&action);
  if (pericardialEff != nullptr)
  {
    if (m_PericardialEffusion == nullptr)
      m_PericardialEffusion = new SEPericardialEffusion();
    m_PericardialEffusion->Copy(*pericardialEff);
    if (!m_PericardialEffusion->IsActive())
      RemovePericardialEffusion();
    return true;
  }

  const SESubstanceBolus* bolus = dynamic_cast<const SESubstanceBolus*>(&action);
  if (bolus != nullptr)
  {
    SESubstance* sub = m_Substances.GetSubstance(bolus->GetSubstance().GetName());
    if (sub == nullptr)//Make sure this substance manager has it
    {
      Error("Ignoring SESubstanceBolus action due to unknown substance in action: " + bolus->GetSubstance().GetName());
      return false;
    }
    SESubstanceBolus* myBolus = m_SubstanceBolus[sub];
    if (myBolus == nullptr)
    {
      myBolus = new SESubstanceBolus(*sub);
      m_SubstanceBolus[sub] = myBolus;
      m_Substances.AddActiveSubstance(*sub);
    }
    myBolus->Copy(*bolus);
    if (!myBolus->IsActive())
      RemoveSubstanceBolus(*sub);
    return true;
  }

  const SESubstanceInfusion* subInfusion = dynamic_cast<const SESubstanceInfusion*>(&action);
  if (subInfusion != nullptr)
  {
    SESubstance* sub = m_Substances.GetSubstance(subInfusion->GetSubstance().GetName());
    if (sub == nullptr)//Make sure this substance manager has it
    {
      Error("Ignoring SESubstanceInfusion action due to unknown substance in action: " + subInfusion->GetSubstance().GetName());
      return false;
    }
    SESubstanceInfusion* mySubInfuse = m_SubstanceInfusions[sub];
    if (mySubInfuse == nullptr)
    {
      mySubInfuse = new SESubstanceInfusion(*sub);
      m_SubstanceInfusions[sub] = mySubInfuse;
      m_Substances.AddActiveSubstance(*sub);
    }
    mySubInfuse->Copy(*subInfusion);
    if (!mySubInfuse->IsActive())
      RemoveSubstanceInfusion(*sub);
    return true;
  }

  const SESubstanceCompoundInfusion* cSubInfusion = dynamic_cast<const SESubstanceCompoundInfusion*>(&action);
  if (cSubInfusion != nullptr)
  {
    SESubstanceCompound* cmpd = m_Substances.GetCompound(cSubInfusion->GetSubstanceCompound().GetName());
    if (cmpd == nullptr)//Make sure this substance manager has it
    {
      Error("Ignoring SESubstanceCompoundInfusion action due to unknown substance in action: " + cSubInfusion->GetSubstanceCompound().GetName());
      return false;
    }
    SESubstanceCompoundInfusion* mySubCompInfuse = m_SubstanceCompoundInfusions[cmpd];
    if (mySubCompInfuse == nullptr)
    {
      mySubCompInfuse = new SESubstanceCompoundInfusion(*cmpd);
      m_SubstanceCompoundInfusions[cmpd] = mySubCompInfuse;
      m_Substances.AddActiveCompound(*cmpd);
      // Activate the compound component substances
      const SESubstanceCompound& sc = mySubCompInfuse->GetSubstanceCompound();
      for (const SESubstanceConcentration* scc : sc.GetComponents())
      {
        m_Substances.AddActiveSubstance(scc->GetSubstance());
      }
    }
    mySubCompInfuse->Copy(*cSubInfusion);
    if (!mySubCompInfuse->IsActive())
      RemoveSubstanceCompoundInfusion(*cmpd);
    return true;
  }

  const SESupplementalOxygen* supplementalO2 = dynamic_cast<const SESupplementalOxygen*>(&action);
  if (supplementalO2 != nullptr)
  {
    if (m_SupplementalOxygen == nullptr)
      m_SupplementalOxygen = new SESupplementalOxygen();
    m_SupplementalOxygen->Copy(*supplementalO2);
    if (!m_SupplementalOxygen->IsActive())
      RemoveSupplementalOxygen();
    return true;
  }

  const SETensionPneumothorax* pneumo = dynamic_cast<const SETensionPneumothorax*>(&action);
  if (pneumo != nullptr)
  {
    if (pneumo->GetSide() == eSide::Left&&pneumo->GetType() == eGate::Open)
    {
      if (m_LeftOpenTensionPneumothorax == nullptr)
        m_LeftOpenTensionPneumothorax = new SETensionPneumothorax();
      m_LeftOpenTensionPneumothorax->Copy(*pneumo);
      if (!m_LeftOpenTensionPneumothorax->IsActive())
        RemoveLeftOpenTensionPneumothorax();
      return true;
    }
    else if (pneumo->GetSide() == eSide::Left&&pneumo->GetType() == eGate::Closed)
    {
      if (m_LeftClosedTensionPneumothorax == nullptr)
        m_LeftClosedTensionPneumothorax = new SETensionPneumothorax();
      m_LeftClosedTensionPneumothorax->Copy(*pneumo);
      if (!m_LeftClosedTensionPneumothorax->IsActive())
        RemoveLeftClosedTensionPneumothorax();
      return true;
    }
    else if (pneumo->GetSide() == eSide::Right&&pneumo->GetType() == eGate::Open)
    {
      if (m_RightOpenTensionPneumothorax == nullptr)
        m_RightOpenTensionPneumothorax = new SETensionPneumothorax();
      m_RightOpenTensionPneumothorax->Copy(*pneumo);
      if (!m_RightOpenTensionPneumothorax->IsActive())
        RemoveRightOpenTensionPneumothorax();
      return true;
    }
    else if (pneumo->GetSide() == eSide::Right&&pneumo->GetType() == eGate::Closed)
    {
      if (m_RightClosedTensionPneumothorax == nullptr)
        m_RightClosedTensionPneumothorax = new SETensionPneumothorax();
      m_RightClosedTensionPneumothorax->Copy(*pneumo);
      if (!m_RightClosedTensionPneumothorax->IsActive())
        RemoveRightClosedTensionPneumothorax();
      return true;
    }
    Error("Unknown Tension Pnumothorax Type");
    return false;// Duno what this is...
  }

  const SEUrinate* urinate = dynamic_cast<const SEUrinate*>(&action);
  if (urinate != nullptr)
  {
    if (m_Urinate == nullptr)
      m_Urinate = new SEUrinate();
    m_Urinate->Copy(*urinate);
    if (!m_Urinate->IsActive())
      RemoveUrinate();
    return true;
  }

  /// \error Unsupported Action
  Error("Unsupported Action");
  return false;
}

bool SEPatientActionCollection::HasAcuteRespiratoryDistressSyndromeExacerbation() const
{
  return m_ARDSExacerbation == nullptr ? false : m_ARDSExacerbation->IsActive();
}
SEAcuteRespiratoryDistressSyndromeExacerbation* SEPatientActionCollection::GetAcuteRespiratoryDistressSyndromeExacerbation()
{
  return m_ARDSExacerbation;
}
const SEAcuteRespiratoryDistressSyndromeExacerbation* SEPatientActionCollection::GetAcuteRespiratoryDistressSyndromeExacerbation() const
{
  return m_ARDSExacerbation;
}
void SEPatientActionCollection::RemoveAcuteRespiratoryDistressSyndromeExacerbation()
{
  SAFE_DELETE(m_ARDSExacerbation);
}

bool SEPatientActionCollection::HasAcuteStress() const
{
  return m_AcuteStress == nullptr ? false : m_AcuteStress->IsActive();
}
SEAcuteStress* SEPatientActionCollection::GetAcuteStress()
{
  return m_AcuteStress;
}
const SEAcuteStress* SEPatientActionCollection::GetAcuteStress() const
{
  return m_AcuteStress;
}
void SEPatientActionCollection::RemoveAcuteStress()
{
  SAFE_DELETE(m_AcuteStress);
}

bool SEPatientActionCollection::HasAirwayObstruction() const
{
  return m_AirwayObstruction == nullptr ? false : m_AirwayObstruction->IsActive();
}
SEAirwayObstruction* SEPatientActionCollection::GetAirwayObstruction()
{
  return m_AirwayObstruction;
}
const SEAirwayObstruction* SEPatientActionCollection::GetAirwayObstruction() const
{
  return m_AirwayObstruction;
}
void SEPatientActionCollection::RemoveAirwayObstruction()
{
  SAFE_DELETE(m_AirwayObstruction);
}

bool SEPatientActionCollection::HasAsthmaAttack() const
{
  return m_AsthmaAttack == nullptr ? false : m_AsthmaAttack->IsActive();
}
SEAsthmaAttack* SEPatientActionCollection::GetAsthmaAttack()
{
  return m_AsthmaAttack;
}
const SEAsthmaAttack* SEPatientActionCollection::GetAsthmaAttack() const
{
  return m_AsthmaAttack;
}
void SEPatientActionCollection::RemoveAsthmaAttack()
{
  SAFE_DELETE(m_AsthmaAttack);
}

bool SEPatientActionCollection::HasBrainInjury() const
{
  return m_BrainInjury == nullptr ? false : m_BrainInjury->IsActive();
}
SEBrainInjury* SEPatientActionCollection::GetBrainInjury()
{
  return m_BrainInjury;
}
const SEBrainInjury* SEPatientActionCollection::GetBrainInjury() const
{
  return m_BrainInjury;
}
void SEPatientActionCollection::RemoveBrainInjury()
{
  SAFE_DELETE(m_BrainInjury);
}

bool SEPatientActionCollection::HasBronchoconstriction() const
{
  return m_Bronchoconstriction == nullptr ? false : m_Bronchoconstriction->IsActive();
}
SEBronchoconstriction* SEPatientActionCollection::GetBronchoconstriction()
{
  return m_Bronchoconstriction;
}
const SEBronchoconstriction* SEPatientActionCollection::GetBronchoconstriction() const
{
  return m_Bronchoconstriction;
}
void SEPatientActionCollection::RemoveBronchoconstriction()
{
  SAFE_DELETE(m_Bronchoconstriction);
}

bool SEPatientActionCollection::HasCardiacArrest() const
{
  return m_CardiacArrest == nullptr ? false : m_CardiacArrest->IsActive();
}
SECardiacArrest* SEPatientActionCollection::GetCardiacArrest()
{
  return m_CardiacArrest;
}
const SECardiacArrest* SEPatientActionCollection::GetCardiacArrest() const
{
  return m_CardiacArrest;
}
void SEPatientActionCollection::RemoveCardiacArrest()
{
  SAFE_DELETE(m_CardiacArrest);
}

bool SEPatientActionCollection::HasChestCompression() const
{
  return m_ChestCompression == nullptr ? false : m_ChestCompression->IsActive();
}
void SEPatientActionCollection::RemoveChestCompression()
{
  SAFE_DELETE(m_ChestCompression);
}
bool SEPatientActionCollection::HasChestCompressionForce() const
{
  if (m_ChestCompression == nullptr)
    return false;
  if (dynamic_cast<SEChestCompressionForce*>(m_ChestCompression) == nullptr)
    return false;
  return m_ChestCompression->IsActive();
}
SEChestCompressionForce* SEPatientActionCollection::GetChestCompressionForce()
{
  return dynamic_cast<SEChestCompressionForce*>(m_ChestCompression);
}
const SEChestCompressionForce* SEPatientActionCollection::GetChestCompressionForce() const
{
  return dynamic_cast<SEChestCompressionForce*>(m_ChestCompression);
}
bool SEPatientActionCollection::HasChestCompressionForceScale() const
{
  if (m_ChestCompression == nullptr)
    return false;
  if (dynamic_cast<SEChestCompressionForceScale*>(m_ChestCompression) == nullptr)
    return false;
  return m_ChestCompression->IsActive();
}
SEChestCompressionForceScale* SEPatientActionCollection::GetChestCompressionForceScale()
{
  return dynamic_cast<SEChestCompressionForceScale*>(m_ChestCompression);
}
const SEChestCompressionForceScale* SEPatientActionCollection::GetChestCompressionForceScale() const
{
  return dynamic_cast<SEChestCompressionForceScale*>(m_ChestCompression);
}

bool SEPatientActionCollection::HasChestOcclusiveDressing() const
{
  return HasLeftChestOcclusiveDressing() || HasRightChestOcclusiveDressing();
}
bool SEPatientActionCollection::HasLeftChestOcclusiveDressing() const
{
  return m_LeftChestOcclusiveDressing == nullptr ? false : m_LeftChestOcclusiveDressing->IsActive();
}
SEChestOcclusiveDressing* SEPatientActionCollection::GetLeftChestOcclusiveDressing()
{
  return m_LeftChestOcclusiveDressing;
}
const SEChestOcclusiveDressing* SEPatientActionCollection::GetLeftChestOcclusiveDressing() const
{
  return m_LeftChestOcclusiveDressing;
}
void SEPatientActionCollection::RemoveLeftChestOcclusiveDressing()
{
  SAFE_DELETE(m_LeftChestOcclusiveDressing);
}
bool SEPatientActionCollection::HasRightChestOcclusiveDressing() const
{
  return m_RightChestOcclusiveDressing == nullptr ? false : m_RightChestOcclusiveDressing->IsActive();
}
SEChestOcclusiveDressing* SEPatientActionCollection::GetRightChestOcclusiveDressing()
{
  return m_RightChestOcclusiveDressing;
}
const SEChestOcclusiveDressing* SEPatientActionCollection::GetRightChestOcclusiveDressing() const
{
  return m_RightChestOcclusiveDressing;
}
void SEPatientActionCollection::RemoveRightChestOcclusiveDressing()
{
  SAFE_DELETE(m_RightChestOcclusiveDressing);
}

bool SEPatientActionCollection::HasChronicObstructivePulmonaryDiseaseExacerbation() const
{
  return m_COPDExacerbation == nullptr ? false : m_COPDExacerbation->IsActive();
}
SEChronicObstructivePulmonaryDiseaseExacerbation* SEPatientActionCollection::GetChronicObstructivePulmonaryDiseaseExacerbation()
{
  return m_COPDExacerbation;
}
const SEChronicObstructivePulmonaryDiseaseExacerbation* SEPatientActionCollection::GetChronicObstructivePulmonaryDiseaseExacerbation() const
{
  return m_COPDExacerbation;
}
void SEPatientActionCollection::RemoveChronicObstructivePulmonaryDiseaseExacerbation()
{
  SAFE_DELETE(m_COPDExacerbation);
}

bool SEPatientActionCollection::HasConsciousRespiration() const
{
  return m_ConsciousRespiration == nullptr ? false : m_ConsciousRespiration->IsActive();
}
SEConsciousRespiration* SEPatientActionCollection::GetConsciousRespiration()
{
  return m_ConsciousRespiration;
}
const SEConsciousRespiration* SEPatientActionCollection::GetConsciousRespiration() const
{
  return m_ConsciousRespiration;
}
void SEPatientActionCollection::RemoveConsciousRespiration()
{
  SAFE_DELETE(m_ConsciousRespiration);
}

bool SEPatientActionCollection::HasConsumeNutrients() const
{
  return m_ConsumeNutrients == nullptr ? false : m_ConsumeNutrients->IsActive();
}
SEConsumeNutrients* SEPatientActionCollection::GetConsumeNutrients()
{
  return m_ConsumeNutrients;
}
const SEConsumeNutrients* SEPatientActionCollection::GetConsumeNutrients() const
{
  return m_ConsumeNutrients;
}
void SEPatientActionCollection::RemoveConsumeNutrients()
{
  SAFE_DELETE(m_ConsumeNutrients);
}

bool SEPatientActionCollection::HasDyspnea() const
{
  return m_Dyspnea == nullptr ? false : m_Dyspnea->IsActive();
}
SEDyspnea* SEPatientActionCollection::GetDyspnea()
{
  return m_Dyspnea;
}
const SEDyspnea* SEPatientActionCollection::GetDyspnea() const
{
  return m_Dyspnea;
}
void SEPatientActionCollection::RemoveDyspnea()
{
  SAFE_DELETE(m_Dyspnea);
}

bool SEPatientActionCollection::HasExercise() const
{
  return m_Exercise == nullptr ? false : m_Exercise->IsActive();
}
SEExercise* SEPatientActionCollection::GetExercise()
{
  return m_Exercise;
}
const SEExercise* SEPatientActionCollection::GetExercise() const
{
  return m_Exercise;
}
void SEPatientActionCollection::RemoveExercise()
{
  SAFE_DELETE(m_Exercise);
}

bool SEPatientActionCollection::HasHemorrhage() const
{
  return m_Hemorrhages.empty() ? false : true;
}
const std::map<std::string, SEHemorrhage*>& SEPatientActionCollection::GetHemorrhages() const
{
  return m_Hemorrhages;
}
void SEPatientActionCollection::RemoveHemorrhage(const std::string& cmpt)
{
  SEHemorrhage* h = m_Hemorrhages[cmpt];
  m_Hemorrhages.erase(cmpt);
  SAFE_DELETE(h);
}

bool SEPatientActionCollection::HasIntubation() const
{
  return m_Intubation == nullptr ? false : m_Intubation->IsActive();
}
SEIntubation* SEPatientActionCollection::GetIntubation()
{
  return m_Intubation;
}
const SEIntubation* SEPatientActionCollection::GetIntubation() const
{
  return m_Intubation;
}
void SEPatientActionCollection::RemoveIntubation()
{
  SAFE_DELETE(m_Intubation);
}

bool SEPatientActionCollection::HasLobarPneumoniaExacerbation() const
{
  return m_LobarPneumoniaExacerbation == nullptr ? false : m_LobarPneumoniaExacerbation->IsActive();
}
SELobarPneumoniaExacerbation* SEPatientActionCollection::GetLobarPneumoniaExacerbation()
{
  return m_LobarPneumoniaExacerbation;
}
const SELobarPneumoniaExacerbation* SEPatientActionCollection::GetLobarPneumoniaExacerbation() const
{
  return m_LobarPneumoniaExacerbation;
}
void SEPatientActionCollection::RemoveLobarPneumoniaExacerbation()
{
  SAFE_DELETE(m_LobarPneumoniaExacerbation);
}

bool SEPatientActionCollection::HasMechanicalVentilation() const
{
  return m_MechanicalVentilation == nullptr ? false : m_MechanicalVentilation->IsActive();
}
SEMechanicalVentilation* SEPatientActionCollection::GetMechanicalVentilation()
{
  return m_MechanicalVentilation;
}
const SEMechanicalVentilation* SEPatientActionCollection::GetMechanicalVentilation() const
{
  return m_MechanicalVentilation;
}
void SEPatientActionCollection::RemoveMechanicalVentilation()
{
  SAFE_DELETE(m_MechanicalVentilation);
}

bool SEPatientActionCollection::HasNeedleDecompression() const
{
  return HasLeftNeedleDecompression() || HasRightNeedleDecompression();
}
bool SEPatientActionCollection::HasLeftNeedleDecompression() const
{
  return m_LeftNeedleDecompression == nullptr ? false : m_LeftNeedleDecompression->IsActive();
}
SENeedleDecompression* SEPatientActionCollection::GetLeftNeedleDecompression()
{
  return m_LeftNeedleDecompression;
}
const SENeedleDecompression* SEPatientActionCollection::GetLeftNeedleDecompression() const
{
  return m_LeftNeedleDecompression;
}
void SEPatientActionCollection::RemoveLeftNeedleDecompression()
{
  SAFE_DELETE(m_LeftNeedleDecompression);
}
bool SEPatientActionCollection::HasRightNeedleDecompression() const
{
  return m_RightNeedleDecompression == nullptr ? false : m_RightNeedleDecompression->IsActive();
}
SENeedleDecompression* SEPatientActionCollection::GetRightNeedleDecompression()
{
  return m_RightNeedleDecompression;
}
const SENeedleDecompression* SEPatientActionCollection::GetRightNeedleDecompression() const
{
  return m_RightNeedleDecompression;
}
void SEPatientActionCollection::RemoveRightNeedleDecompression()
{
  SAFE_DELETE(m_RightNeedleDecompression);
}

bool SEPatientActionCollection::HasPericardialEffusion() const
{
  return m_PericardialEffusion == nullptr ? false : m_PericardialEffusion->IsActive();
}
SEPericardialEffusion* SEPatientActionCollection::GetPericardialEffusion()
{
  return m_PericardialEffusion;
}
const SEPericardialEffusion* SEPatientActionCollection::GetPericardialEffusion() const
{
  return m_PericardialEffusion;
}
void SEPatientActionCollection::RemovePericardialEffusion()
{
  SAFE_DELETE(m_PericardialEffusion);
}

bool SEPatientActionCollection::HasSupplementalOxygen() const
{
  return m_SupplementalOxygen == nullptr ? false : m_SupplementalOxygen->IsActive();
}
SESupplementalOxygen* SEPatientActionCollection::GetSupplementalOxygen()
{
  return m_SupplementalOxygen;
}
const SESupplementalOxygen* SEPatientActionCollection::GetSupplementalOxygen() const
{
  return m_SupplementalOxygen;
}
void SEPatientActionCollection::RemoveSupplementalOxygen()
{
  SAFE_DELETE(m_SupplementalOxygen);
}

const std::map<const SESubstance*, SESubstanceBolus*>& SEPatientActionCollection::GetSubstanceBoluses() const
{
  return m_SubstanceBolus;
}
void SEPatientActionCollection::RemoveSubstanceBolus(const SESubstance& sub)
{
  SESubstanceBolus* b = m_SubstanceBolus[&sub];
  m_SubstanceBolus.erase(&sub);
  SAFE_DELETE(b);
}

const std::map<const SESubstance*, SESubstanceInfusion*>& SEPatientActionCollection::GetSubstanceInfusions() const
{
  return m_SubstanceInfusions;
}
void SEPatientActionCollection::RemoveSubstanceInfusion(const SESubstance& sub)
{
  SESubstanceInfusion* si = m_SubstanceInfusions[&sub];
  m_SubstanceInfusions.erase(&sub);
  SAFE_DELETE(si);
}

const std::map<const SESubstanceCompound*, SESubstanceCompoundInfusion*>& SEPatientActionCollection::GetSubstanceCompoundInfusions() const
{
  return m_SubstanceCompoundInfusions;
}
void SEPatientActionCollection::RemoveSubstanceCompoundInfusion(const SESubstanceCompound& cSub)
{
  SESubstanceCompoundInfusion* sci = m_SubstanceCompoundInfusions[&cSub];
  m_SubstanceCompoundInfusions.erase(&cSub);
  SAFE_DELETE(sci);
}

bool SEPatientActionCollection::HasTensionPneumothorax() const
{
  if (m_LeftOpenTensionPneumothorax != nullptr&&m_LeftOpenTensionPneumothorax->IsActive())
    return true;
  if (m_LeftClosedTensionPneumothorax != nullptr&&m_LeftClosedTensionPneumothorax->IsActive())
    return true;
  if (m_RightOpenTensionPneumothorax != nullptr&&m_RightOpenTensionPneumothorax->IsActive())
    return true;
  if (m_RightClosedTensionPneumothorax != nullptr&&m_RightClosedTensionPneumothorax->IsActive())
    return true;
  return false;
}
bool SEPatientActionCollection::HasLeftOpenTensionPneumothorax() const
{
  if (m_LeftOpenTensionPneumothorax != nullptr&&m_LeftOpenTensionPneumothorax->IsActive())
    return true;
  return false;
}
SETensionPneumothorax* SEPatientActionCollection::GetLeftOpenTensionPneumothorax()
{
  return m_LeftOpenTensionPneumothorax;
}
const SETensionPneumothorax* SEPatientActionCollection::GetLeftOpenTensionPneumothorax() const
{
  return m_LeftOpenTensionPneumothorax;
}
void SEPatientActionCollection::RemoveLeftOpenTensionPneumothorax()
{
  SAFE_DELETE(m_LeftOpenTensionPneumothorax);
}
bool SEPatientActionCollection::HasLeftClosedTensionPneumothorax() const
{
  if (m_LeftClosedTensionPneumothorax != nullptr&&m_LeftClosedTensionPneumothorax->IsActive())
    return true;
  return false;
}
SETensionPneumothorax* SEPatientActionCollection::GetLeftClosedTensionPneumothorax()
{
  return m_LeftClosedTensionPneumothorax;
}
const SETensionPneumothorax* SEPatientActionCollection::GetLeftClosedTensionPneumothorax() const
{
  return m_LeftClosedTensionPneumothorax;
}
void SEPatientActionCollection::RemoveLeftClosedTensionPneumothorax()
{
  SAFE_DELETE(m_LeftClosedTensionPneumothorax);
}
bool SEPatientActionCollection::HasRightOpenTensionPneumothorax() const
{
  if (m_RightOpenTensionPneumothorax != nullptr&&m_RightOpenTensionPneumothorax->IsActive())
    return true;
  return false;
}
SETensionPneumothorax* SEPatientActionCollection::GetRightOpenTensionPneumothorax()
{
  return m_RightOpenTensionPneumothorax;
}
const SETensionPneumothorax* SEPatientActionCollection::GetRightOpenTensionPneumothorax() const
{
  return m_RightOpenTensionPneumothorax;
}
void SEPatientActionCollection::RemoveRightOpenTensionPneumothorax()
{
  SAFE_DELETE(m_RightOpenTensionPneumothorax);
}
bool SEPatientActionCollection::HasRightClosedTensionPneumothorax() const
{
  if (m_RightClosedTensionPneumothorax != nullptr&&m_RightClosedTensionPneumothorax->IsActive())
    return true;
  return false;
}
SETensionPneumothorax* SEPatientActionCollection::GetRightClosedTensionPneumothorax()
{
  return m_RightClosedTensionPneumothorax;
}
const SETensionPneumothorax* SEPatientActionCollection::GetRightClosedTensionPneumothorax() const
{
  return m_RightClosedTensionPneumothorax;
}
void SEPatientActionCollection::RemoveRightClosedTensionPneumothorax()
{
  SAFE_DELETE(m_RightClosedTensionPneumothorax);
}

bool SEPatientActionCollection::HasUrinate() const
{
  return m_Urinate == nullptr ? false : m_Urinate->IsActive();
}
SEUrinate* SEPatientActionCollection::GetUrinate()
{
  return m_Urinate;
}
const SEUrinate* SEPatientActionCollection::GetUrinate() const
{
  return m_Urinate;
}
void SEPatientActionCollection::RemoveUrinate()
{
  SAFE_DELETE(m_Urinate);
}

void SEPatientActionCollection::GetAllActions(std::vector<const SEAction*>& actions) const
{
  if (HasAcuteRespiratoryDistressSyndromeExacerbation())
    actions.push_back(GetAcuteRespiratoryDistressSyndromeExacerbation());
  if (HasAcuteStress())
    actions.push_back(GetAcuteStress());
  if (HasAirwayObstruction())
    actions.push_back(GetAirwayObstruction());
  if (HasAsthmaAttack())
    actions.push_back(GetAsthmaAttack());
  if (HasBrainInjury())
    actions.push_back(GetBrainInjury());
  if (HasBronchoconstriction())
    actions.push_back(GetBronchoconstriction());
  if (HasCardiacArrest())
    actions.push_back(GetCardiacArrest());
  if (HasChestCompressionForce())
    actions.push_back(GetChestCompressionForce());
  if (HasChestCompressionForceScale())
    actions.push_back(GetChestCompressionForceScale());
  if (HasLeftChestOcclusiveDressing())
    actions.push_back(GetLeftChestOcclusiveDressing());
  if (HasRightChestOcclusiveDressing())
    actions.push_back(GetRightChestOcclusiveDressing());
  if (HasChronicObstructivePulmonaryDiseaseExacerbation())
    actions.push_back(GetChronicObstructivePulmonaryDiseaseExacerbation());
  if (HasConsciousRespiration())
    actions.push_back(GetConsciousRespiration());
  if (HasConsumeNutrients())
    actions.push_back(GetConsumeNutrients());
  if (HasDyspnea())
    actions.push_back(GetDyspnea());
  if (HasExercise())
    actions.push_back(GetExercise());
  for (auto itr : m_Hemorrhages)
    actions.push_back(itr.second);
  if (HasIntubation())
    actions.push_back(GetIntubation());
  if (HasLobarPneumoniaExacerbation())
    actions.push_back(GetLobarPneumoniaExacerbation());
  if (HasMechanicalVentilation())
    actions.push_back(GetMechanicalVentilation());
  if (HasLeftNeedleDecompression())
    actions.push_back(GetLeftNeedleDecompression());
  if (HasRightNeedleDecompression())
    actions.push_back(GetRightNeedleDecompression());
  if (HasPericardialEffusion())
    actions.push_back(GetPericardialEffusion());
  if (HasLeftClosedTensionPneumothorax())
    actions.push_back(GetLeftClosedTensionPneumothorax());
  if (HasLeftOpenTensionPneumothorax())
    actions.push_back(GetLeftOpenTensionPneumothorax());
  if (HasRightClosedTensionPneumothorax())
    actions.push_back(GetRightClosedTensionPneumothorax());
  if (HasRightOpenTensionPneumothorax())
    actions.push_back(GetRightOpenTensionPneumothorax());
  for (auto itr : m_SubstanceBolus)
    actions.push_back(itr.second);
  for (auto itr : m_SubstanceInfusions)
    actions.push_back(itr.second);
  for (auto itr : m_SubstanceCompoundInfusions)
    actions.push_back(itr.second);
  if (HasUrinate())
    actions.push_back(GetUrinate());
}