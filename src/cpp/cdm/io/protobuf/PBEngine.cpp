/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Engine.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBEngine.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBConditions.h"
#include "io/protobuf/PBPatient.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "engine/SECondition.h"
#include "engine/SEConditionManager.h"
#include "engine/SEDataRequest.h"
#include "engine/SEDataRequested.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEDecimalFormat.h"
#include "engine/SEDynamicStabilization.h"
#include "engine/SEDynamicStabilizationEngineConvergence.h"
#include "engine/SEDynamicStabilizationPropertyConvergence.h"
#include "engine/SETimedStabilization.h"
#include "engine/SEEngineInitialization.h"
#include "engine/SEPatientConfiguration.h"
#include "system/environment/conditions/SEInitialEnvironmentalConditions.h"
#include "patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
#include "patient/conditions/SEChronicAnemia.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SEChronicPericardialEffusion.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "patient/conditions/SEConsumeMeal.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
#include "patient/conditions/SEPulmonaryShunt.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "patient/conditions/SEPulmonaryFibrosis.h"
#include "patient/conditions/SESepsis.h"
#include "engine/SEAction.h"
#include "engine/SEActionManager.h"
#include "engine/SEEnvironmentActionCollection.h"
#include "engine/SEEquipmentActionCollection.h"
#include "engine/SEPatientActionCollection.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveObstruction.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveObstruction.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineMaskLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineSodaLimeFailure.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineTubeCuffLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVaporizerFailure.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVentilatorPressureLoss.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineYPieceDisconnect.h"
#include "system/environment/actions/SEChangeEnvironmentalConditions.h"
#include "system/environment/actions/SEThermalApplication.h"
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
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
/**/#include "patient/actions/SEForcedExhale.h"
/**/#include "patient/actions/SEForcedInhale.h"
/**/#include "patient/actions/SEForcedPause.h"
/**/#include "patient/actions/SEUseInhaler.h"
#include "patient/actions/SEConsumeNutrients.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEExercise.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEImpairedAlveolarExchangeExacerbation.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SELobarPneumoniaExacerbation.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SEPericardialEffusion.h"
#include "patient/actions/SEPulmonaryShuntExacerbation.h"
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


void PBEngine::Load(const CDM_BIND::LogMessagesData& src, LogMessages& dst)
{
  dst.Clear();
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const CDM_BIND::LogMessagesData& src, LogMessages& dst)
{
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
  CDM_BIND::LogMessagesData data;
  if (!PBUtils::SerializeFromString(src, data, m, logger))
    return false;
  PBEngine::Load(data, dst);
  return true;
}

CDM_BIND::LogMessagesData* PBEngine::Unload(const LogMessages& src)
{
  CDM_BIND::LogMessagesData* dst = new CDM_BIND::LogMessagesData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const LogMessages& src, CDM_BIND::LogMessagesData& dst)
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
  CDM_BIND::LogMessagesData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, logger);
}

void PBEngine::Load(const CDM_BIND::ConditionListData& src, SEConditionManager& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const CDM_BIND::ConditionListData& src, SEConditionManager& dst, const SESubstanceManager& subMgr)
{
  for (int i = 0; i < src.anycondition_size(); i++)
  {
    SECondition* c = PBCondition::Load(src.anycondition()[i], subMgr);
    dst.Copy(*c, subMgr);
    delete c;
  }
}
CDM_BIND::ConditionListData* PBEngine::Unload(const SEConditionManager& src)
{
  CDM_BIND::ConditionListData* dst = new CDM_BIND::ConditionListData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEConditionManager& src, CDM_BIND::ConditionListData& dst)
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
  if (src.HasPulmonaryFibrosis())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_PulmonaryFibrosis));
  if (src.HasPulmonaryShunt())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_PulmonaryShunt));
  if (src.HasLobarPneumonia())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_LobarPneumonia));
  if (src.HasChronicRenalStenosis())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_RenalStenosis));
  if (src.HasSepsis())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_Sepsis));
  if (src.HasInitialEnvironmentalConditions())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_InitialEnvironmentalConditions));
}

void PBEngine::Load(const CDM_BIND::ActionListData& src, SEActionManager& dst)
{
  dst.Clear();
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const CDM_BIND::ActionListData& src, SEActionManager& dst)
{
  for (int i = 0; i < src.anyaction_size(); i++)
  {
    SEAction* a = PBAction::Load(src.anyaction()[i], dst.m_SubMgr);
    dst.ProcessAction(*a);
    delete a;
  }
}
CDM_BIND::ActionListData* PBEngine::Unload(const SEActionManager& src)
{
  CDM_BIND::ActionListData* dst = new CDM_BIND::ActionListData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEActionManager& src, CDM_BIND::ActionListData& dst)
{
  PBEngine::Serialize(*src.m_PatientActions, dst);
  PBEngine::Serialize(*src.m_EnvironmentActions, dst);
  PBEngine::Serialize(*src.m_EquipmentActions, dst);
}

void PBEngine::Serialize(const SEEquipmentActionCollection& src, CDM_BIND::ActionListData& dst)
{
  if (src.HasAnesthesiaMachineConfiguration())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineConfiguration));
  if (src.HasInhalerConfiguration())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_InhalerConfiguration));
  if (src.HasMechanicalVentilatorConfiguration())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_MechanicalVentilatorConfiguration));

  if (src.HasAnesthesiaMachineOxygenTankPressureLoss())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineOxygenTankPressureLoss));
  if (src.HasAnesthesiaMachineOxygenWallPortPressureLoss())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineOxygenWallPortPressureLoss));

  if (src.HasAnesthesiaMachineExpiratoryValveLeak())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineExpiratoryValveLeak));
  if (src.HasAnesthesiaMachineExpiratoryValveObstruction())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineExpiratoryValveObstruction));
  if (src.HasAnesthesiaMachineInspiratoryValveLeak())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineInspiratoryValveLeak));
  if (src.HasAnesthesiaMachineInspiratoryValveObstruction())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineInspiratoryValveObstruction));
  if (src.HasAnesthesiaMachineMaskLeak())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineMaskLeak));
  if (src.HasAnesthesiaMachineSodaLimeFailure())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineSodaLimeFailure));
  if (src.HasAnesthesiaMachineTubeCuffLeak())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineTubeCuffLeak));
  if (src.HasAnesthesiaMachineVaporizerFailure())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineVaporizerFailure));
  if (src.HasAnesthesiaMachineVentilatorPressureLoss())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineVentilatorPressureLoss));
  if (src.HasAnesthesiaMachineYPieceDisconnect())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AnesthesiaMachineYPieceDisconnect));
}
void PBEngine::Serialize(const SEEnvironmentActionCollection& src, CDM_BIND::ActionListData& dst)
{
  if (src.HasChangeEnvironmentalConditions())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ChangeEnvironmentalConditions));
  if (src.HasThermalApplication())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ThermalApplication));
}

void PBEngine::Serialize(const SEPatientActionCollection& src, CDM_BIND::ActionListData& dst)
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
  if (src.HasChestCompressionForce())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ChestCompressionForce));
  else if (src.HasChestCompressionForceScale())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ChestCompressionForceScale));
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
    for (auto h : src.m_Hemorrhages)
    {
      if(h->IsActive())
        dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*h));
    }
  }
  if (src.HasImpairedAlveolarExchangeExacerbation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ImpairedAlveolarExchangeExacerbation));
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
  if (src.HasPulmonaryShuntExacerbation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_PulmonaryShuntExacerbation));
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
  for (auto b : src.m_SubstanceBoluses)
  {
    if(b->IsActive())
      dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*b));
  }
  for (auto i : src.m_SubstanceInfusions)
  {
    if(i->IsActive())
      dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*i));
  }
  for (auto ci : src.m_SubstanceCompoundInfusions)
  {
    if(ci->IsActive())
      dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*ci));
  }
  if (src.HasUrinate())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Urinate));
}


void PBEngine::Load(const CDM_BIND::PatientConfigurationData& src, SEPatientConfiguration& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const CDM_BIND::PatientConfigurationData& src, SEPatientConfiguration& dst, const SESubstanceManager& subMgr)
{
  if (!src.patientfile().empty())
    dst.SetPatientFile(src.patientfile());
  else if (src.has_patient())
    PBPatient::Load(src.patient(), dst.GetPatient());
  
  if (src.has_conditions())
    PBEngine::Load(src.conditions(), dst.GetConditions(), subMgr);

  for (size_t i = 0; i < src.scalaroverride_size(); i++)
  {
    const CDM_BIND::ScalarPropertyData& sp = src.scalaroverride()[i];
    dst.AddScalarOverride(sp.name(), sp.value(), sp.unit());
  }
  dst.SetDataRoot(src.dataroot());
}
CDM_BIND::PatientConfigurationData* PBEngine::Unload(const SEPatientConfiguration& src)
{
  CDM_BIND::PatientConfigurationData* dst = new CDM_BIND::PatientConfigurationData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEPatientConfiguration& src, CDM_BIND::PatientConfigurationData& dst)
{
  if (src.HasPatientFile())
    dst.set_patientfile(src.m_PatientFile);
  else if (src.HasPatient())
    dst.set_allocated_patient(PBPatient::Unload(*src.m_Patient));

  for (auto& sp : src.GetScalarOverrides())
  {
    CDM_BIND::ScalarPropertyData* so = dst.add_scalaroverride();
    so->set_name(sp.name);
    so->set_value(sp.value);
    so->set_unit(sp.unit);
  }

  if(src.HasConditions())
    dst.set_allocated_conditions(PBEngine::Unload(*src.GetConditions()));

  dst.set_dataroot(src.GetDataRoot());
}

bool PBEngine::SerializeToString(const SEPatientConfiguration& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::PatientConfigurationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEPatientConfiguration& src, const std::string& filename)
{
  CDM_BIND::PatientConfigurationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}
bool PBEngine::SerializeFromString(const std::string& src, SEPatientConfiguration& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::PatientConfigurationData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEPatientConfiguration& dst, const SESubstanceManager& subMgr)
{
  CDM_BIND::PatientConfigurationData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}

void PBEngine::Load(const CDM_BIND::DataRequestData& src, SEDataRequest& dst)
{
  dst.Clear();
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const CDM_BIND::DataRequestData& src, SEDataRequest& dst)
{
  if(src.has_decimalformat())
    PBEngine::Serialize(src.decimalformat(), dst);
  dst.m_ActionName = src.actionname();
  dst.m_CompartmentName = src.compartmentname();
  dst.m_SubstanceName = src.substancename();
  dst.m_PropertyName = src.propertyname();
  dst.m_RequestedUnit = src.unit();
}
CDM_BIND::DataRequestData* PBEngine::Unload(const SEDataRequest& src)
{
  CDM_BIND::DataRequestData* dst = new CDM_BIND::DataRequestData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDataRequest& src, CDM_BIND::DataRequestData& dst)
{
  PBEngine::Serialize(src, *dst.mutable_decimalformat());
  dst.set_category((CDM_BIND::DataRequestData::eCategory)src.m_Category);
  if (src.HasActionName())
    dst.set_actionname(src.m_ActionName);
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
  dst.Clear();
  CDM_BIND::DataRequestData data;
  PBEngine::Serialize(src, data);
  PBEngine::Serialize(data, dst);
}


CDM_BIND::DataRequestedData* PBEngine::Unload(const SEDataRequested& src)
{
  CDM_BIND::DataRequestedData* dst = new CDM_BIND::DataRequestedData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDataRequested& src, CDM_BIND::DataRequestedData& dst)
{
  dst.set_id(src.GetID());
  dst.set_isactive(src.IsActive());
  if (src.KeepLogMessages())
  {
    // TODO Write Logs
  }
  if (src.KeepEventChanges())
  {
    // TODO Write Event Changes
  }
  for(double d : src.GetValues())
    dst.add_value(d);
}
bool PBEngine::SerializeToString(const SEDataRequested& src, std::string& dst, SerializationFormat m)
{
  CDM_BIND::DataRequestedData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, dst, m, nullptr);
}
void PBEngine::Serialize(const std::vector<SEDataRequested*>& src, CDM_BIND::DataRequestedListData& dst)
{
  for (SEDataRequested* s : src)
    Serialize(*s, *dst.add_datarequested());
}
bool PBEngine::SerializeToString(const std::vector<SEDataRequested*>& src, std::string& dst, SerializationFormat m)
{
  CDM_BIND::DataRequestedListData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, dst, m, nullptr);
}

void PBEngine::Load(const CDM_BIND::DataRequestManagerData& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const CDM_BIND::DataRequestManagerData& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  dst.m_ResultsFilename = src.resultsfilename();
  dst.m_SamplesPerSecond = src.samplespersecond();
  if (src.has_defaultdecimalformatting())
    PBEngine::Load(src.defaultdecimalformatting(), dst.GetDefaultDecimalFormatting());
  if (src.has_overridedecimalformatting())
    PBEngine::Load(src.overridedecimalformatting(), dst.GetOverrideDecimalFormatting());

  for (int i = 0; i < src.datarequest_size(); i++)
  {
    const CDM_BIND::DataRequestData& drData = src.datarequest(i);
    SEDataRequest* dr = new SEDataRequest((eDataRequest_Category)drData.category(), dst.HasOverrideDecimalFormatting() ? dst.m_OverrideDecimalFormatting : dst.m_DefaultDecimalFormatting);
    PBEngine::Load(drData, *dr);
    if (!dr->IsValid())
      dst.Error("Ignoring invalid DataRequest for property " + dr->m_PropertyName);
    else
      dst.m_Requests.push_back(dr);
  }
}
CDM_BIND::DataRequestManagerData* PBEngine::Unload(const SEDataRequestManager& src)
{
  CDM_BIND::DataRequestManagerData* dst = new CDM_BIND::DataRequestManagerData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDataRequestManager& src, CDM_BIND::DataRequestManagerData& dst)
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
  dst.Clear();
  CDM_BIND::DataRequestManagerData data;
  PBEngine::Serialize(src, data);
  PBEngine::Serialize(data, dst, subMgr);
}

void PBEngine::Load(const CDM_BIND::DecimalFormatData& src, SEDecimalFormat& dst)
{
  dst.Clear();
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const CDM_BIND::DecimalFormatData& src, SEDecimalFormat& dst)
{
  dst.SetNotation((eDecimalFormat_Type)src.type());
  dst.SetPrecision(src.precision());
}
CDM_BIND::DecimalFormatData* PBEngine::Unload(const SEDecimalFormat& src)
{
  CDM_BIND::DecimalFormatData* dst = new CDM_BIND::DecimalFormatData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDecimalFormat& src, CDM_BIND::DecimalFormatData& dst)
{
  dst.set_type((CDM_BIND::DecimalFormatData::eType)src.m_Notation);
  dst.set_precision((google::protobuf::uint32)src.m_Precision);
}

bool PBEngine::SerializeToString(const SEConditionManager& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::ConditionListData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEConditionManager& src, const std::string& filename)
{
  CDM_BIND::ConditionListData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}
bool PBEngine::SerializeFromString(const std::string& src, SEConditionManager& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::ConditionListData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEConditionManager& dst, const SESubstanceManager& subMgr)
{
  CDM_BIND::ConditionListData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}

bool PBEngine::SerializeToString(const SEActionManager& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::ActionListData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEActionManager& src, const std::string& filename)
{
  CDM_BIND::ActionListData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}
bool PBEngine::SerializeFromString(const std::string& src, SEActionManager& dst, SerializationFormat m)
{
  CDM_BIND::ActionListData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEActionManager& dst)
{
  CDM_BIND::ActionListData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}

bool PBEngine::SerializeFromString(const std::string& src, std::vector<SEAction*>& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::ActionListData data;
  if (!PBUtils::SerializeFromString(src, data, m, subMgr.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
void PBEngine::Load(const CDM_BIND::ActionListData& src, std::vector<SEAction*>& dst, const SESubstanceManager& subMgr)
{
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const CDM_BIND::ActionListData& src, std::vector<SEAction*>& dst, const SESubstanceManager& subMgr)
{
  dst.clear();
  for (int i = 0; i < src.anyaction_size(); i++)
  {
    SEAction* a = PBAction::Load(src.anyaction()[i], subMgr);
    dst.push_back(a);
  }
}

bool PBEngine::SerializeFromString(const std::string& src, std::map<int, std::vector<const SEAction*>>& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::ActionMapData data;
  if (!PBUtils::SerializeFromString(src, data, m, subMgr.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
void PBEngine::Load(const CDM_BIND::ActionMapData& src, std::map<int, std::vector<const SEAction*>>& dst, const SESubstanceManager& subMgr)
{
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const CDM_BIND::ActionMapData& src, std::map<int, std::vector<const SEAction*>>& dst, const SESubstanceManager& subMgr)
{
  dst.clear();
  for (auto& itr : src.actionmap())
  {
    auto& v=dst[itr.first];
    for (int i = 0; i < itr.second.anyaction_size(); i++)
    {
      SEAction* a = PBAction::Load(itr.second.anyaction()[i], subMgr);
      v.push_back(a);
    }
  }
}

bool PBEngine::SerializeToString(const SEDataRequestManager& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::DataRequestManagerData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEDataRequestManager& src, const std::string& filename)
{
  CDM_BIND::DataRequestManagerData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}
bool PBEngine::SerializeFromString(const std::string& src, SEDataRequestManager& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::DataRequestManagerData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  CDM_BIND::DataRequestManagerData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}

void PBEngine::Load(const CDM_BIND::DynamicStabilizationData& src, SEDynamicStabilization& dst)
{
  dst.Clear();
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const CDM_BIND::DynamicStabilizationData& src, SEDynamicStabilization& dst)
{
  if (src.trackingstabilization() != CDM_BIND::eSwitch::NullSwitch)
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
CDM_BIND::DynamicStabilizationData* PBEngine::Unload(const SEDynamicStabilization& src)
{
  CDM_BIND::DynamicStabilizationData* dst(new CDM_BIND::DynamicStabilizationData());
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDynamicStabilization& src, CDM_BIND::DynamicStabilizationData& dst)
{
  dst.set_trackingstabilization((CDM_BIND::eSwitch)src.m_TrackingStabilization);
  dst.set_allocated_restingconvergence(PBEngine::Unload(*src.m_RestingConvergence));
  if (src.HasFeedbackConvergence())
    dst.set_allocated_feedbackconvergence(PBEngine::Unload(*src.m_FeedbackConvergence));
  for (auto &c : src.m_ConditionConvergence)
  {
    CDM_BIND::DynamicStabilizationEngineConvergenceData* cData = PBEngine::Unload(*c.second);
    (*dst.mutable_conditionconvergence())[c.first] = *cData;
    delete cData;
  }
}


void PBEngine::Load(const CDM_BIND::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst)
{
  dst.Clear();
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const CDM_BIND::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst)
{
  // TODO Warn if these are not provided
  if (src.has_convergencetime())
    PBProperty::Load(src.convergencetime(), dst.GetConvergenceTime());
  if (src.has_minimumreactiontime())
    PBProperty::Load(src.minimumreactiontime(), dst.GetMinimumReactionTime());
  if (src.has_maximumallowedstabilizationtime())
    PBProperty::Load(src.maximumallowedstabilizationtime(), dst.GetMaximumAllowedStabilizationTime());
  for (int i = 0; i < src.propertyconvergence_size(); i++)
  {
    const CDM_BIND::DynamicStabilizationPropertyConvergenceData& pcData = src.propertyconvergence(i);
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
CDM_BIND::DynamicStabilizationEngineConvergenceData* PBEngine::Unload(const SEDynamicStabilizationEngineConvergence& src)
{
  CDM_BIND::DynamicStabilizationEngineConvergenceData* dst = new CDM_BIND::DynamicStabilizationEngineConvergenceData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDynamicStabilizationEngineConvergence& src, CDM_BIND::DynamicStabilizationEngineConvergenceData& dst)
{
  dst.set_allocated_convergencetime(PBProperty::Unload(*src.m_ConvergenceTime));
  dst.set_allocated_minimumreactiontime(PBProperty::Unload(*src.m_MinimumReactionTime));
  dst.set_allocated_maximumallowedstabilizationtime(PBProperty::Unload(*src.m_MaximumAllowedStabilizationTime));
  for (auto pc : src.m_PropertyConvergence)
  {
    CDM_BIND::DynamicStabilizationPropertyConvergenceData* pcData = dst.mutable_propertyconvergence()->Add();
    pcData->set_percentdifference(pc->m_Error);
    pcData->set_allocated_datarequest(PBEngine::Unload(pc->m_DataRequest));
  }
}

void PBEngine::Load(const CDM_BIND::TimedStabilizationData& src, SETimedStabilization& dst)
{
  dst.Clear();
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const CDM_BIND::TimedStabilizationData& src, SETimedStabilization& dst)
{
  if (src.trackingstabilization() != CDM_BIND::eSwitch::NullSwitch)
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
CDM_BIND::TimedStabilizationData* PBEngine::Unload(const SETimedStabilization& src)
{
  CDM_BIND::TimedStabilizationData* dst(new CDM_BIND::TimedStabilizationData());
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SETimedStabilization& src, CDM_BIND::TimedStabilizationData& dst)
{
  dst.set_trackingstabilization((CDM_BIND::eSwitch)src.m_TrackingStabilization);
  dst.set_allocated_restingstabilizationtime(PBProperty::Unload(*src.m_RestingStabilizationTime));
  if (src.HasFeedbackStabilizationTime())
    dst.set_allocated_feedbackstabilizationtime(PBProperty::Unload(*src.m_FeedbackStabilizationTime));
  for (auto cc : src.m_ConditionTimes)
  {
    if (cc.second == nullptr)
      continue;
    CDM_BIND::ScalarTimeData* time = PBProperty::Unload(*cc.second);
    (*dst.mutable_conditionstabilization())[cc.first] = *time;
    delete time;
  }
}

bool PBEngine::SerializeToString(const SEDynamicStabilization& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::DynamicStabilizationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SEDynamicStabilization& src, const std::string& filename)
{
  CDM_BIND::DynamicStabilizationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}
bool PBEngine::SerializeFromString(const std::string& src, SEDynamicStabilization& dst, SerializationFormat m)
{
  CDM_BIND::DynamicStabilizationData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEDynamicStabilization& dst)
{
  CDM_BIND::DynamicStabilizationData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}

bool PBEngine::SerializeToString(const SETimedStabilization& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::TimedStabilizationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeToFile(const SETimedStabilization& src, const std::string& filename)
{
  CDM_BIND::TimedStabilizationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}
bool PBEngine::SerializeFromString(const std::string& src, SETimedStabilization& dst, SerializationFormat m)
{
  CDM_BIND::TimedStabilizationData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SETimedStabilization& dst)
{
  CDM_BIND::TimedStabilizationData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst);
  return true;
}


void PBEngine::Load(const CDM_BIND::EngineInitializationData& src, SEEngineInitialization& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBEngine::Serialize(src, dst, subMgr);
}
void PBEngine::Serialize(const CDM_BIND::EngineInitializationData& src, SEEngineInitialization& dst, const SESubstanceManager& subMgr)
{
  dst.SetID(src.id());

  if (src.has_patientconfiguration())
    PBEngine::Load(src.patientconfiguration(), dst.GetPatientConfiguration(), subMgr);
  else if (!src.statefilename().empty())
    dst.SetStateFilename(src.statefilename());
  else if (!src.state().empty())
    dst.SetState(src.state(), SerializationFormat::JSON);// TODO support binary

  if (src.has_datarequestmanager())
    PBEngine::Load(src.datarequestmanager(), dst.GetDataRequestManager(), subMgr);

  if (!src.logfilename().empty())
    dst.SetLogFilename(src.logfilename());

  dst.KeepLogMessages(src.keeplogmessages());
  dst.KeepEventChanges(src.keepeventchanges());
}
CDM_BIND::EngineInitializationData* PBEngine::Unload(const SEEngineInitialization& src)
{
  CDM_BIND::EngineInitializationData* dst = new CDM_BIND::EngineInitializationData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEEngineInitialization& src, CDM_BIND::EngineInitializationData& dst)
{
  dst.set_id(src.m_ID);

  if (src.HasPatientConfiguration())
    dst.set_allocated_patientconfiguration(PBEngine::Unload(*src.m_PatientConfiguration));
  else if (src.HasStateFilename())
    dst.set_statefilename(src.m_StateFilename);
  else if (src.HasState())
    dst.set_state(src.m_State);

  if (src.HasDataRequestManager())
    dst.set_allocated_datarequestmanager(PBEngine::Unload(*src.m_DataRequestManager));

  if (src.HasLogFilename())
    dst.set_logfilename(src.m_LogFilename);

  dst.set_keeplogmessages(src.KeepLogMessages());
  dst.set_keepeventchanges(src.KeepEventChanges());
}

bool PBEngine::SerializeToString(const SEEngineInitialization& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::EngineInitializationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBEngine::SerializeFromString(const std::string& src, SEEngineInitialization& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::EngineInitializationData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBEngine::Load(data, dst, subMgr);
  return true;
}
bool PBEngine::SerializeFromString(const std::string& src, std::vector<SEEngineInitialization*>& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::EngineInitializationListData data;
  if (!PBUtils::SerializeFromString(src, data, m, subMgr.GetLogger()))
    return false;
  for (int i = 0; i < data.engineinitialization_size(); i++)
  {
    SEEngineInitialization* ei = new SEEngineInitialization(subMgr.GetLogger());
    PBEngine::Load(data.engineinitialization()[i], *ei, subMgr);
    dst.push_back(ei);
  }
  return true;
}

void PBEngine::Copy(const SEEngineInitialization& src, SEEngineInitialization& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  CDM_BIND::EngineInitializationData data;
  PBEngine::Serialize(src, data);
  PBEngine::Serialize(data, dst, subMgr);
}
