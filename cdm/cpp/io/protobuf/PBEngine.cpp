/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBEngine.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBConditions.h"
#include "io/protobuf/PBPatient.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cpp/cdm/Engine.pb.h"
#include "engine/SEDataRequest.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEDecimalFormat.h"
#include "engine/SEAutoSerialization.h"
#include "engine/SEDynamicStabilization.h"
#include "engine/SEDynamicStabilizationEngineConvergence.h"
#include "engine/SEDynamicStabilizationPropertyConvergence.h"
#include "engine/SETimedStabilization.h"
#include "engine/SEDataRequest.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SECondition.h"
#include "engine/SEConditionManager.h"
#include "system/environment/conditions/SEInitialEnvironmentConditions.h"
#include "patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
#include "patient/conditions/SEChronicAnemia.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SEChronicPericardialEffusion.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "patient/conditions/SEConsumeMeal.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "patient/conditions/SESepsis.h"
#include "engine/SEAction.h"
#include "engine/SEActionManager.h"
#include "engine/SEAnesthesiaMachineActionCollection.h"
#include "engine/SEEnvironmentActionCollection.h"
#include "engine/SEInhalerActionCollection.h"
#include "engine/SEPatientActionCollection.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineExpiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineExpiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineInspiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineInspiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineMaskLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineSodaLimeFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineTubeCuffLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineVaporizerFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineVentilatorPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineYPieceDisconnect.h"
#include "system/environment/actions/SEChangeEnvironmentConditions.h"
#include "system/environment/actions/SEThermalApplication.h"
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "patient/actions/SEPatientAssessmentRequest.h"
#include "patient/actions/SEAcuteRespiratoryDistressSyndromeExacerbation.h"
#include "patient/actions/SEAcuteStress.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "patient/actions/SEBrainInjury.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "patient/actions/SECardiacArrest.h"
#include "patient/actions/SEChestCompressionForce.h"
#include "patient/actions/SEChestCompressionForceScale.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "patient/actions/SEChronicObstructivePulmonaryDiseaseExacerbation.h"
#include "patient/actions/SEConsciousRespiration.h"
/**/#include "patient/actions/SEBreathHold.h"
/**/#include "patient/actions/SEForcedExhale.h"
/**/#include "patient/actions/SEForcedInhale.h"
/**/#include "patient/actions/SEUseInhaler.h"
#include "patient/actions/SEConsumeNutrients.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEExercise.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SELobarPneumoniaExacerbation.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SEPericardialEffusion.h"
#include "patient/actions/SERespiratoryFatigue.h"
#include "patient/actions/SESubstanceBolus.h"
#include "patient/actions/SESubstanceInfusion.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "patient/actions/SETensionPneumothorax.h"
#include "patient/actions/SEUrinate.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarTime.h"
#include "utils/unitconversion/UCCommon.h"
#include "utils/FileUtils.h"


void PBEngine::Load(const cdm::LogMessagesData& src, LogMessages& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::LogMessagesData& src, LogMessages& dst)
{
  dst.Clear();
  for (int i = 0; i < src.debugmessages_size(); i++)
    dst.debug_msgs.push_back(src.debugmessages()[i]);
  for (int i = 0; i < src.infogmessages_size(); i++)
    dst.info_msgs.push_back(src.infogmessages()[i]);
  for (int i = 0; i < src.warningmessages_size(); i++)
    dst.warning_msgs.push_back(src.warningmessages()[i]);
  for (int i = 0; i < src.errormessages_size(); i++)
    dst.error_msgs.push_back(src.errormessages()[i]);
  for (int i = 0; i < src.fatalmessages_size(); i++)
    dst.fatal_msgs.push_back(src.fatalmessages()[i]);
}
bool PBEngine::SerializeFromString(const std::string& src, LogMessages& dst, SerializationFormat m, Logger* logger)
{
  cdm::LogMessagesData data;
  if (!PBUtils::SerializeFromString(src, data, m, logger))
    return false;
  PBEngine::Load(data, dst);
  return true;
}

cdm::LogMessagesData* PBEngine::Unload(const LogMessages& src)
{
  cdm::LogMessagesData* dst = new cdm::LogMessagesData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const LogMessages& src, cdm::LogMessagesData& dst)
{
  for (std::string str : src.debug_msgs)
    dst.add_debugmessages(str);
  for (std::string str : src.info_msgs)
    dst.add_infogmessages(str);
  for (std::string str : src.warning_msgs)
    dst.add_warningmessages(str);
  for (std::string str : src.error_msgs)
    dst.add_errormessages(str);
  for (std::string str : src.fatal_msgs)
    dst.add_fatalmessages(str);
}
bool PBEngine::SerializeToString(const LogMessages& src, std::string& output, SerializationFormat m, Logger* logger)
{
  cdm::LogMessagesData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, logger);
}

void PBEngine::Load(const cdm::ConditionListData& src, SEConditionManager& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::ConditionListData& src, SEConditionManager& dst)
{
  for (int i = 0; i < src.anycondition_size(); i++)
  {
    SECondition* c = PBCondition::Load(src.anycondition()[i], dst.m_Substances);
    dst.ProcessCondition(*c);
    delete c;
  }
}
cdm::ConditionListData* PBEngine::Unload(const SEConditionManager& src)
{
  cdm::ConditionListData* dst = new cdm::ConditionListData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEConditionManager& src, cdm::ConditionListData& dst)
{
  if (src.HasAcuteRespiratoryDistressSyndrome())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_ARDS));
  if (src.HasChronicAnemia())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_Anemia));
  if (src.HasConsumeMeal())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_ConsumeMeal));
  if (src.HasChronicObstructivePulmonaryDisease())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_COPD));
  if (src.HasChronicVentricularSystolicDysfunction())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_ChronicVentricularSystolicDysfunction));
  if (src.HasImpairedAlveolarExchange())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_ImpairedAlveolarExchange));
  if (src.HasChronicPericardialEffusion())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_PericardialEffusion));
  if (src.HasLobarPneumonia())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_LobarPneumonia));
  if (src.HasChronicRenalStenosis())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_RenalStenosis));
  if (src.HasSepsis())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_Sepsis));
  if (src.HasInitialEnvironmentConditions())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_InitialEnvironmentConditions));
}

void PBEngine::Load(const cdm::ActionListData& src, SEActionManager& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::ActionListData& src, SEActionManager& dst)
{
  for (int i = 0; i < src.anyaction_size(); i++)
  {
    SEAction* a = PBAction::Load(src.anyaction()[i], dst.m_Substances);
    dst.ProcessAction(*a);
    delete a;
  }
}
cdm::ActionListData* PBEngine::Unload(const SEActionManager& src)
{
  cdm::ActionListData* dst = new cdm::ActionListData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEActionManager& src, cdm::ActionListData& dst)
{
  PBEngine::Serialize(*src.m_PatientActions, dst);
  PBEngine::Serialize(*src.m_EnvironmentActions, dst);
  PBEngine::Serialize(*src.m_AnesthesiaMachineActions, dst);
  PBEngine::Serialize(*src.m_InhalerActions, dst);
}

void PBEngine::Serialize(const SEAnesthesiaMachineActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasConfiguration())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Configuration));

  if (src.HasOxygenTankPressureLoss())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_OxygenTankPressureLoss));
  if (src.HasOxygenWallPortPressureLoss())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_OxygenWallPortPressureLoss));

  if (src.HasExpiratoryValveLeak())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ExpiratoryValveLeak));
  if (src.HasExpiratoryValveObstruction())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ExpiratoryValveObstruction));
  if (src.HasInspiratoryValveLeak())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_InspiratoryValveLeak));
  if (src.HasInspiratoryValveObstruction())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_InspiratoryValveObstruction));
  if (src.HasMaskLeak())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_MaskLeak));
  if (src.HasSodaLimeFailure())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_SodaLimeFailure));
  if (src.HasTubeCuffLeak())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_TubeCuffLeak));
  if (src.HasVaporizerFailure())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_VaporizerFailure));
  if (src.HasVentilatorPressureLoss())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_VentilatorPressureLoss));
  if (src.HasYPieceDisconnect())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_YPieceDisconnect));
}
void PBEngine::Serialize(const SEEnvironmentActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasChange())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Change));
  if (src.HasThermalApplication())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ThermalApplication));
}
void PBEngine::Serialize(const SEInhalerActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasConfiguration())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Configuration));
}

void PBEngine::Serialize(const SEPatientActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasAcuteRespiratoryDistressSyndromeExacerbation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ARDSExacerbation));
  if (src.HasAcuteStress())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AcuteStress));
  if (src.HasAirwayObstruction())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AirwayObstruction));
  if (src.HasAsthmaAttack())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AsthmaAttack));
  if (src.HasBrainInjury())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_BrainInjury));
  if (src.HasBronchoconstriction())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Bronchoconstriction));
  if (src.HasCardiacArrest())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_CardiacArrest));
  if (src.HasChestCompression())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ChestCompression));
  if (src.HasLeftChestOcclusiveDressing())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_LeftChestOcclusiveDressing));
  if (src.HasRightChestOcclusiveDressing())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_RightChestOcclusiveDressing));
  if (src.HasChronicObstructivePulmonaryDiseaseExacerbation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_COPDExacerbation));
  if (src.HasConsciousRespiration())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ConsciousRespiration));
  if (src.HasConsumeNutrients())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ConsumeNutrients));
  if (src.HasDyspnea())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Dyspnea));
  if (src.HasExercise())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Exercise));
  if (src.HasHemorrhage())
  {
    for (auto itr : src.m_Hemorrhages)
      dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*itr.second));
  }
  if (src.HasIntubation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Intubation));
  if (src.HasLobarPneumoniaExacerbation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_LobarPneumoniaExacerbation));
  if (src.HasMechanicalVentilation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_MechanicalVentilation));
  if (src.HasLeftNeedleDecompression())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_LeftNeedleDecompression));
  if (src.HasRightNeedleDecompression())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_RightNeedleDecompression));
  if (src.HasPericardialEffusion())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_PericardialEffusion));
  if (src.HasRespiratoryFatigue())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_RespiratoryFatigue));
  if (src.HasLeftClosedTensionPneumothorax())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_LeftClosedTensionPneumothorax));
  if (src.HasLeftOpenTensionPneumothorax())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_LeftOpenTensionPneumothorax));
  if (src.HasRightClosedTensionPneumothorax())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_RightClosedTensionPneumothorax));
  if (src.GetRightOpenTensionPneumothorax())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_RightOpenTensionPneumothorax));
  for (auto itr : src.m_SubstanceBolus)
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*itr.second));
  for (auto itr : src.m_SubstanceInfusions)
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*itr.second));
  for (auto itr : src.m_SubstanceCompoundInfusions)
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*itr.second));
  if (src.HasUrinate())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Urinate));
}


void PBEngine::Load(const cdm::PatientConfigurationData& src, SEPatientConfiguration& dst, SESubstanceManager& subMgr)
{
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const cdm::PatientConfigurationData& src, SEPatientConfiguration& dst, SESubstanceManager& subMgr)
{
  dst.Clear();

  if (src.has_patient())
    PBPatient::Load(src.patient(), dst.GetPatient());
  else
    dst.SetPatientFile(src.patientfile());
  
  if (src.has_conditions())
    PBEngine::Load(src.conditions(), dst.GetConditions());
}
cdm::PatientConfigurationData* PBEngine::Unload(const SEPatientConfiguration& src)
{
  cdm::PatientConfigurationData* dst = new cdm::PatientConfigurationData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEPatientConfiguration& src, cdm::PatientConfigurationData& dst)
{
  if (src.HasPatientFile())
    dst.set_patientfile(src.m_PatientFile);
  else if (src.HasPatient())
    dst.set_allocated_patient(PBPatient::Unload(*src.m_Patient));

  dst.set_allocated_conditions(PBEngine::Unload(*src.GetConditions()));
}


bool PBEngine::SerializeToString(const SEPatientConfiguration& src, std::string& output, SerializationFormat m)
{
  cdm::PatientConfigurationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEPatientConfiguration& src, const std::string& filename, SerializationFormat m)
{
  cdm::PatientConfigurationData data;
  PBEngine::Serialize(src, data);
  std::string content;
  PBEngine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBEngine::SerializeFromString(const std::string& src, SEPatientConfiguration& dst, SerializationFormat m, SESubstanceManager& subMgr)
{
  cdm::PatientConfigurationData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEPatientConfiguration& dst, SerializationFormat m, SESubstanceManager& subMgr)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEngine::SerializeFromString(content, dst, m, subMgr);
}

void PBEngine::Load(const cdm::DataRequestData& src, SEDataRequest& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::DataRequestData& src, SEDataRequest& dst)
{
  PBEngine::Serialize(src.decimalformat(), dst);
  dst.m_CompartmentName = src.compartmentname();
  dst.m_SubstanceName = src.substancename();
  dst.m_PropertyName = src.propertyname();
  dst.m_RequestedUnit = src.unit();
}
cdm::DataRequestData* PBEngine::Unload(const SEDataRequest& src)
{
  cdm::DataRequestData* dst = new cdm::DataRequestData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDataRequest& src, cdm::DataRequestData& dst)
{
  PBEngine::Serialize(src, *dst.mutable_decimalformat());
  dst.set_category((cdm::DataRequestData::eCategory)src.m_Category);
  if (src.HasCompartmentName())
    dst.set_compartmentname(src.m_CompartmentName);
  if (src.HasSubstanceName())
    dst.set_substancename(src.m_SubstanceName);

  dst.set_propertyname(src.m_PropertyName);
  if (src.HasUnit())
    dst.set_unit(src.m_Unit->GetString());
  else if (src.HasRequestedUnit())
    dst.set_unit(src.m_RequestedUnit);
}
void PBEngine::Copy(const SEDataRequest& src, SEDataRequest& dst)
{
  cdm::DataRequestData data;
  PBEngine::Serialize(src, data);
  PBEngine::Serialize(data, dst);
}

void PBEngine::Load(const cdm::DataRequestManagerData& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const cdm::DataRequestManagerData& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  dst.m_ResultsFilename = src.resultsfilename();
  dst.m_SamplesPerSecond = src.samplespersecond();
  if (src.has_defaultdecimalformatting())
    PBEngine::Load(src.defaultdecimalformatting(), dst.GetDefaultDecimalFormatting());
  if (src.has_overridedecimalformatting())
    PBEngine::Load(src.overridedecimalformatting(), dst.GetOverrideDecimalFormatting());

  for (int i = 0; i < src.datarequest_size(); i++)
  {
    const cdm::DataRequestData& drData = src.datarequest(i);
    SEDataRequest* dr = new SEDataRequest((eDataRequest_Category)drData.category(), dst.HasOverrideDecimalFormatting() ? dst.m_OverrideDecimalFormatting : dst.m_DefaultDecimalFormatting);
    PBEngine::Load(drData, *dr);
    if (!dr->IsValid())
      dst.Error("Ignoring invalid DataRequest for property " + dr->m_PropertyName);
    else
      dst.m_Requests.push_back(dr);
  }
}
cdm::DataRequestManagerData* PBEngine::Unload(const SEDataRequestManager& src)
{
  cdm::DataRequestManagerData* dst = new cdm::DataRequestManagerData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDataRequestManager& src, cdm::DataRequestManagerData& dst)
{
  dst.set_resultsfilename(src.m_ResultsFilename);
  dst.set_samplespersecond(src.m_SamplesPerSecond);
  if (src.HasDefaultDecimalFormatting())
    dst.set_allocated_defaultdecimalformatting(PBEngine::Unload(*src.m_DefaultDecimalFormatting));
  if (src.HasOverrideDecimalFormatting())
    dst.set_allocated_overridedecimalformatting(PBEngine::Unload(*src.m_OverrideDecimalFormatting));
  for (SEDataRequest* dr : src.m_Requests)
    dst.mutable_datarequest()->AddAllocated(PBEngine::Unload(*dr));
}
void PBEngine::Copy(const SEDataRequestManager& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  cdm::DataRequestManagerData data;
  PBEngine::Serialize(src, data);
  PBEngine::Serialize(data, dst, subMgr);
}

void PBEngine::Load(const cdm::DecimalFormatData& src, SEDecimalFormat& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::DecimalFormatData& src, SEDecimalFormat& dst)
{
  dst.Clear();
  dst.SetNotation((eDecimalFormat_Type)src.type());
  dst.SetPrecision(src.precision());
}
cdm::DecimalFormatData* PBEngine::Unload(const SEDecimalFormat& src)
{
  cdm::DecimalFormatData* dst = new cdm::DecimalFormatData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDecimalFormat& src, cdm::DecimalFormatData& dst)
{
  dst.set_type((cdm::DecimalFormatData::eType)src.m_Notation);
  dst.set_precision((google::protobuf::uint32)src.m_Precision);
}

bool PBEngine::SerializeToString(const SEConditionManager& src, std::string& output, SerializationFormat m)
{
  cdm::ConditionListData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEConditionManager& src, const std::string& filename, SerializationFormat m)
{
  cdm::ConditionListData data;
  PBEngine::Serialize(src, data);
  std::string content;
  PBEngine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBEngine::SerializeFromString(const std::string& src, SEConditionManager& dst, SerializationFormat m)
{
  cdm::ConditionListData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEConditionManager& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEngine::SerializeFromString(content, dst, m);
}

bool PBEngine::SerializeToString(const SEActionManager& src, std::string& output, SerializationFormat m)
{
  cdm::ActionListData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEActionManager& src, const std::string& filename, SerializationFormat m)
{
  cdm::ActionListData data;
  PBEngine::Serialize(src, data);
  std::string content;
  PBEngine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBEngine::SerializeFromString(const std::string& src, SEActionManager& dst, SerializationFormat m)
{
  cdm::ActionListData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEActionManager& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEngine::SerializeFromString(content, dst, m);
}

bool PBEngine::SerializeFromString(const std::string& src, std::vector<SEAction*>& dst, SerializationFormat m, SESubstanceManager& subMgr)
{
  cdm::ActionListData data;
  if (!PBUtils::SerializeFromString(src, data, m, subMgr.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
void PBEngine::Load(const cdm::ActionListData& src, std::vector<SEAction*>& dst, SESubstanceManager& subMgr)
{
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const cdm::ActionListData& src, std::vector<SEAction*>& dst, SESubstanceManager& subMgr)
{
  for (int i = 0; i < src.anyaction_size(); i++)
  {
    SEAction* a = PBAction::Load(src.anyaction()[i], subMgr);
    dst.push_back(a);
  }
}

bool PBEngine::SerializeToString(const SEDataRequestManager& src, std::string& output, SerializationFormat m)
{
  cdm::DataRequestManagerData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEDataRequestManager& src, const std::string& filename, SerializationFormat m)
{
  cdm::DataRequestManagerData data;
  PBEngine::Serialize(src, data);
  std::string content;
  PBEngine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBEngine::SerializeFromString(const std::string& src, SEDataRequestManager& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  cdm::DataRequestManagerData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEDataRequestManager& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEngine::SerializeFromString(content, dst, m, subMgr);
}

void PBEngine::Load(const cdm::AutoSerializationData& src, SEAutoSerialization& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::AutoSerializationData& src, SEAutoSerialization& dst)
{
  dst.Clear();
  if (src.has_period())
    PBProperty::Load(src.period(), dst.GetPeriod());
  if (src.periodtimestamps() != cdm::eSwitch::NullSwitch)
    dst.SetPeriodTimeStamps((eSwitch)src.periodtimestamps());
  if (src.afteractions() != cdm::eSwitch::NullSwitch)
    dst.SetAfterActions((eSwitch)src.afteractions());
  if (src.reloadstate() != cdm::eSwitch::NullSwitch)
    dst.SetReloadState((eSwitch)src.reloadstate());
  dst.SetDirectory(src.directory());
  dst.SetFileName(src.filename());
}
cdm::AutoSerializationData* PBEngine::Unload(const SEAutoSerialization& src)
{
  cdm::AutoSerializationData *dst = new cdm::AutoSerializationData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEAutoSerialization& src, cdm::AutoSerializationData& dst)
{
  if (src.HasPeriod())
    dst.set_allocated_period(PBProperty::Unload(*src.m_Period));
  dst.set_periodtimestamps((cdm::eSwitch)src.m_PeriodTimeStamps);
  dst.set_afteractions((cdm::eSwitch)src.m_AfterActions);
  dst.set_reloadstate((cdm::eSwitch)src.m_ReloadState);
  if (src.HasDirectory())
    dst.set_directory(src.m_Directory);
  if (src.HasFileName())
    dst.set_filename(src.m_FileName);
}

void PBEngine::Load(const cdm::DynamicStabilizationData& src, SEDynamicStabilization& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::DynamicStabilizationData& src, SEDynamicStabilization& dst)
{
  dst.Clear();
  if (src.trackingstabilization() != cdm::eSwitch::NullSwitch)
    dst.TrackStabilization((eSwitch)src.trackingstabilization());
  if (src.has_restingconvergence())
    PBEngine::Load(src.restingconvergence(), dst.GetRestingConvergence());
  if (src.has_feedbackconvergence())
    PBEngine::Load(src.feedbackconvergence(), dst.GetFeedbackConvergence());

  for (auto itr : src.conditionconvergence())
  {
    SEDynamicStabilizationEngineConvergence* c = new SEDynamicStabilizationEngineConvergence(dst.GetLogger());
    PBEngine::Load(itr.second, *c);
    dst.m_ConditionConvergence[itr.first] = c;
  }
}
cdm::DynamicStabilizationData* PBEngine::Unload(const SEDynamicStabilization& src)
{
  cdm::DynamicStabilizationData* dst(new cdm::DynamicStabilizationData());
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDynamicStabilization& src, cdm::DynamicStabilizationData& dst)
{
  dst.set_trackingstabilization((cdm::eSwitch)src.m_TrackingStabilization);
  dst.set_allocated_restingconvergence(PBEngine::Unload(*src.m_RestingConvergence));
  if (src.HasFeedbackConvergence())
    dst.set_allocated_feedbackconvergence(PBEngine::Unload(*src.m_FeedbackConvergence));
  for (auto &c : src.m_ConditionConvergence)
  {
    cdm::DynamicStabilizationEngineConvergenceData* cData = PBEngine::Unload(*c.second);
    (*dst.mutable_conditionconvergence())[c.first] = *cData;
    delete cData;
  }
}


void PBEngine::Load(const cdm::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst)
{
  dst.Clear();
  // TODO Warn if these are not provided
  if (src.has_convergencetime())
    PBProperty::Load(src.convergencetime(), dst.GetConvergenceTime());
  if (src.has_minimumreactiontime())
    PBProperty::Load(src.minimumreactiontime(), dst.GetMinimumReactionTime());
  if (src.has_maximumallowedstabilizationtime())
    PBProperty::Load(src.maximumallowedstabilizationtime(), dst.GetMaximumAllowedStabilizationTime());
  for (int i = 0; i < src.propertyconvergence_size(); i++)
  {
    const cdm::DynamicStabilizationPropertyConvergenceData& pcData = src.propertyconvergence(i);
    if (pcData.has_datarequest())
    {
      SEDataRequest& dr = dst.m_DataRequestMgr->CreateDataRequest((eDataRequest_Category)pcData.datarequest().category());
      PBEngine::Load(pcData.datarequest(), dr);
      dst.CreatePropertyConvergence(dr, pcData.percentdifference());
    }
    else
    {
      dst.Error("Ignoring property convergence that has no data request");
    }
  }
}
cdm::DynamicStabilizationEngineConvergenceData* PBEngine::Unload(const SEDynamicStabilizationEngineConvergence& src)
{
  cdm::DynamicStabilizationEngineConvergenceData* dst = new cdm::DynamicStabilizationEngineConvergenceData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDynamicStabilizationEngineConvergence& src, cdm::DynamicStabilizationEngineConvergenceData& dst)
{
  dst.set_allocated_convergencetime(PBProperty::Unload(*src.m_ConvergenceTime));
  dst.set_allocated_minimumreactiontime(PBProperty::Unload(*src.m_MinimumReactionTime));
  dst.set_allocated_maximumallowedstabilizationtime(PBProperty::Unload(*src.m_MaximumAllowedStabilizationTime));
  for (auto pc : src.m_PropertyConvergence)
  {
    cdm::DynamicStabilizationPropertyConvergenceData* pcData = dst.mutable_propertyconvergence()->Add();
    pcData->set_percentdifference(pc->m_Error);
    pcData->set_allocated_datarequest(PBEngine::Unload(pc->m_DataRequest));
  }
}

void PBEngine::Load(const cdm::TimedStabilizationData& src, SETimedStabilization& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::TimedStabilizationData& src, SETimedStabilization& dst)
{
  dst.Clear();
  if (src.trackingstabilization() != cdm::eSwitch::NullSwitch)
    dst.TrackStabilization((eSwitch)src.trackingstabilization());
  if (src.has_restingstabilizationtime())
    PBProperty::Load(src.restingstabilizationtime(), dst.GetRestingStabilizationTime());
  if (src.has_feedbackstabilizationtime())
    PBProperty::Load(src.feedbackstabilizationtime(), dst.GetFeedbackStabilizationTime());
  for (auto itr : src.conditionstabilization())
  {
    SEScalarTime* time = new SEScalarTime();
    PBProperty::Load(itr.second, *time);
    dst.m_ConditionTimes[itr.first] = time;
  }
}
cdm::TimedStabilizationData* PBEngine::Unload(const SETimedStabilization& src)
{
  cdm::TimedStabilizationData* dst(new cdm::TimedStabilizationData());
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SETimedStabilization& src, cdm::TimedStabilizationData& dst)
{
  dst.set_trackingstabilization((cdm::eSwitch)src.m_TrackingStabilization);
  dst.set_allocated_restingstabilizationtime(PBProperty::Unload(*src.m_RestingStabilizationTime));
  if (src.HasFeedbackStabilizationTime())
    dst.set_allocated_feedbackstabilizationtime(PBProperty::Unload(*src.m_FeedbackStabilizationTime));
  for (auto cc : src.m_ConditionTimes)
  {
    if (cc.second == nullptr)
      continue;
    cdm::ScalarTimeData* time = PBProperty::Unload(*cc.second);
    (*dst.mutable_conditionstabilization())[cc.first] = *time;
    delete time;
  }
}

bool PBEngine::SerializeToString(const SEDynamicStabilization& src, std::string& output, SerializationFormat m)
{
  cdm::DynamicStabilizationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEDynamicStabilization& src, const std::string& filename, SerializationFormat m)
{
  cdm::DynamicStabilizationData data;
  PBEngine::Serialize(src, data);
  std::string content;
  PBEngine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBEngine::SerializeFromString(const std::string& src, SEDynamicStabilization& dst, SerializationFormat m)
{
  cdm::DynamicStabilizationData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEDynamicStabilization& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEngine::SerializeFromString(content, dst, m);
}

bool PBEngine::SerializeToString(const SETimedStabilization& src, std::string& output, SerializationFormat m)
{
  cdm::TimedStabilizationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SETimedStabilization& src, const std::string& filename, SerializationFormat m)
{
  cdm::TimedStabilizationData data;
  PBEngine::Serialize(src, data);
  std::string content;
  PBEngine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBEngine::SerializeFromString(const std::string& src, SETimedStabilization& dst, SerializationFormat m)
{
  cdm::TimedStabilizationData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SETimedStabilization& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEngine::SerializeFromString(content, dst, m);
}

