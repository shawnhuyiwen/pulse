/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBScenario.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBConditions.h"
#include "io/protobuf/PBPatient.h"
#include "io/protobuf/PBUtils.h"
#include <google/protobuf/text_format.h>
#include "bind/cdm/Scenario.pb.h"
#include "bind/cdm/ScenarioEnums.pb.h"
#include "scenario/SEDataRequest.h"
#include "scenario/SEDataRequestManager.h"
#include "scenario/SEDecimalFormat.h"
#include "scenario/SEScenario.h"
#include "scenario/SEScenarioInitialParameters.h"
#include "scenario/SECondition.h"
#include "scenario/SEConditionManager.h"
#include "system/environment/conditions/SEInitialEnvironmentConditions.h"
#include "patient/conditions/SEChronicAnemia.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SEChronicPericardialEffusion.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "patient/conditions/SEConsumeMeal.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "scenario/SEAction.h"
#include "scenario/SEActionManager.h"
#include "scenario/SEAnesthesiaMachineActionCollection.h"
#include "scenario/SEEnvironmentActionCollection.h"
#include "scenario/SEInhalerActionCollection.h"
#include "scenario/SEPatientActionCollection.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesiamachine/actions/SEOxygenWallPortPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEOxygenTankPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEExpiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEExpiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEInspiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEInspiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEMaskLeak.h"
#include "system/equipment/anesthesiamachine/actions/SESodaLimeFailure.h"
#include "system/equipment/anesthesiamachine/actions/SETubeCuffLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEVaporizerFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEVentilatorPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEYPieceDisconnect.h"
#include "system/environment/actions/SEChangeEnvironmentConditions.h"
#include "system/environment/actions/SEThermalApplication.h"
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "patient/actions/SEPatientAssessmentRequest.h"
#include "patient/actions/SEAcuteStress.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "patient/actions/SEApnea.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "patient/actions/SEBrainInjury.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "patient/actions/SECardiacArrest.h"
#include "patient/actions/SEChestCompressionForce.h"
#include "patient/actions/SEChestCompressionForceScale.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "patient/actions/SEConsciousRespiration.h"
/**/#include "patient/actions/SEBreathHold.h"
/**/#include "patient/actions/SEForcedExhale.h"
/**/#include "patient/actions/SEForcedInhale.h"
/**/#include "patient/actions/SEUseInhaler.h"
#include "patient/actions/SEConsumeNutrients.h"
#include "patient/actions/SEExercise.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SEPericardialEffusion.h"
#include "patient/actions/SESubstanceBolus.h"
#include "patient/actions/SESubstanceInfusion.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "patient/actions/SETensionPneumothorax.h"
#include "patient/actions/SEUrinate.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "utils/unitconversion/UCCommon.h"
#include "utils/FileUtils.h"


void PBScenario::Load(const cdm::ConditionListData& src, SEConditionManager& dst)
{
  PBScenario::Serialize(src, dst);
}
void PBScenario::Serialize(const cdm::ConditionListData& src, SEConditionManager& dst)
{
  for (int i = 0; i < src.anycondition_size(); i++)
  {
    SECondition* c = PBCondition::Load(src.anycondition()[i], dst.m_Substances);
    dst.ProcessCondition(*c);
    delete c;
  }
}
cdm::ConditionListData* PBScenario::Unload(const SEConditionManager& src)
{
  cdm::ConditionListData* dst = new cdm::ConditionListData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEConditionManager& src, cdm::ConditionListData& dst)
{
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
  if (src.HasInitialEnvironmentConditions())
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*src.m_InitialEnvironmentConditions));
}

void PBScenario::Load(const cdm::ActionListData& src, SEActionManager& dst)
{
  PBScenario::Serialize(src, dst);
}
void PBScenario::Serialize(const cdm::ActionListData& src, SEActionManager& dst)
{
  for (int i = 0; i < src.anyaction_size(); i++)
  {
    SEAction* a = PBAction::Load(src.anyaction()[i], dst.m_Substances);
    dst.ProcessAction(*a);
    delete a;
  }
}
cdm::ActionListData* PBScenario::Unload(const SEActionManager& src)
{
  cdm::ActionListData* dst = new cdm::ActionListData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEActionManager& src, cdm::ActionListData& dst)
{
  PBScenario::Serialize(*src.m_PatientActions, dst);
  PBScenario::Serialize(*src.m_EnvironmentActions, dst);
  PBScenario::Serialize(*src.m_AnesthesiaMachineActions, dst);
  PBScenario::Serialize(*src.m_InhalerActions, dst);
}
void PBScenario::Serialize(const SEAnesthesiaMachineActionCollection& src, cdm::ActionListData& dst)
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
void PBScenario::Serialize(const SEEnvironmentActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasChange())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Change));
  if (src.HasThermalApplication())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ThermalApplication));
}
void PBScenario::Serialize(const SEInhalerActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasConfiguration())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Configuration));
}

void PBScenario::Serialize(const SEPatientActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasAcuteStress())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AcuteStress));
  if (src.HasAirwayObstruction())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_AirwayObstruction));
  if (src.HasApnea())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Apnea));
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
  if (src.HasConsciousRespiration())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ConsciousRespiration));
  if (src.HasConsumeNutrients())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_ConsumeNutrients));
  if (src.HasExercise())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Exercise));
  if (src.HasHemorrhage())
  {
    for (auto itr : src.m_Hemorrhages)
      dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*itr.second));
  }
  if (src.HasIntubation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_Intubation));
  if (src.HasMechanicalVentilation())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_MechanicalVentilation));
  if (src.HasLeftNeedleDecompression())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_LeftNeedleDecompression));
  if (src.HasRightNeedleDecompression())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_RightNeedleDecompression));
  if (src.HasPericardialEffusion())
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*src.m_PericardialEffusion));
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

void PBScenario::Load(const cdm::ScenarioData& src, SEScenario& dst)
{
  PBScenario::Serialize(src, dst);
}
void PBScenario::Serialize(const cdm::ScenarioData& src, SEScenario& dst)
{
  dst.Clear();
  dst.SetName(src.name());
  dst.SetDescription(src.description());

  if (src.has_initialparameters())
    PBScenario::Load(src.initialparameters(), dst.GetInitialParameters());
  else
  {
    dst.SetEngineStateFile(src.enginestatefile());
  }

  if (src.has_datarequestmanager())
    PBScenario::Load(src.datarequestmanager(), dst.GetDataRequestManager(), dst.m_SubMgr);

  for (int i = 0; i < src.anyaction_size(); i++)
    dst.m_Actions.push_back(PBAction::Load(src.anyaction()[i], dst.m_SubMgr));
}

cdm::ScenarioData* PBScenario::Unload(const SEScenario& src)
{
  cdm::ScenarioData* dst = new cdm::ScenarioData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEScenario& src, cdm::ScenarioData& dst)
{
  dst.set_name(src.m_Name);
  dst.set_description(src.m_Description);

  if (src.HasEngineStateFile())
    dst.set_enginestatefile(src.m_EngineStateFile);
  else if (src.HasInitialParameters())
    dst.set_allocated_initialparameters(PBScenario::Unload(*src.m_InitialParameters));

  dst.set_allocated_datarequestmanager(PBScenario::Unload(*src.m_DataRequestMgr));

  for (const SEAction* a : src.m_Actions)
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*a));
}

void PBScenario::Load(const cdm::ScenarioInitialParametersData& src, SEScenarioInitialParameters& dst)
{
  PBScenario::Serialize(src, dst);
}
void PBScenario::Serialize(const cdm::ScenarioInitialParametersData& src, SEScenarioInitialParameters& dst)
{
  dst.Clear();

  if (src.has_patient())
    PBPatient::Load(src.patient(), dst.GetPatient());
  else
    dst.SetPatientFile(src.patientfile());

  for (int i = 0; i < src.anycondition_size(); i++)
  {
    SECondition* c = PBCondition::Load(src.anycondition()[i], dst.m_SubMgr);
    if (c != nullptr)
      dst.m_Conditions.push_back(c);
  }
}
cdm::ScenarioInitialParametersData* PBScenario::Unload(const SEScenarioInitialParameters& src)
{
  cdm::ScenarioInitialParametersData* dst = new cdm::ScenarioInitialParametersData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEScenarioInitialParameters& src, cdm::ScenarioInitialParametersData& dst)
{
  if (src.HasPatientFile())
    dst.set_patientfile(src.m_PatientFile);
  else if (src.HasPatient())
    dst.set_allocated_patient(PBPatient::Unload(*src.m_Patient));
  for (SECondition* c : src.m_Conditions)
    dst.mutable_anycondition()->AddAllocated(PBCondition::Unload(*c));
}

void PBScenario::Load(const cdm::DataRequestData& src, SEDataRequest& dst)
{
  PBScenario::Serialize(src, dst);
}
void PBScenario::Serialize(const cdm::DataRequestData& src, SEDataRequest& dst)
{
  PBScenario::Serialize(src.decimalformat(), dst);
  dst.m_CompartmentName = src.compartmentname();
  dst.m_SubstanceName = src.substancename();
  dst.m_PropertyName = src.propertyname();
  dst.m_RequestedUnit = src.unit();
}
cdm::DataRequestData* PBScenario::Unload(const SEDataRequest& src)
{
  cdm::DataRequestData* dst = new cdm::DataRequestData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEDataRequest& src, cdm::DataRequestData& dst)
{
  PBScenario::Serialize(src, *dst.mutable_decimalformat());
  dst.set_category((cdm::eDataRequest_Category)src.m_Category);
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
void PBScenario::Copy(const SEDataRequest& src, SEDataRequest& dst)
{
  cdm::DataRequestData data;
  PBScenario::Serialize(src, data);
  PBScenario::Serialize(data, dst);
}

void PBScenario::Load(const cdm::DataRequestManagerData& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  PBScenario::Serialize(src, dst, subMgr);
}
void PBScenario::Serialize(const cdm::DataRequestManagerData& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  dst.m_ResultsFilename = src.resultsfilename();
  dst.m_SamplesPerSecond = src.samplespersecond();
  if (src.has_defaultdecimalformatting())
    PBScenario::Load(src.defaultdecimalformatting(), dst.GetDefaultDecimalFormatting());
  if (src.has_overridedecimalformatting())
    PBScenario::Load(src.overridedecimalformatting(), dst.GetOverrideDecimalFormatting());

  for (int i = 0; i < src.datarequest_size(); i++)
  {
    const cdm::DataRequestData& drData = src.datarequest(i);
    SEDataRequest* dr = new SEDataRequest((eDataRequest_Category)drData.category(), dst.HasOverrideDecimalFormatting() ? dst.m_OverrideDecimalFormatting : dst.m_DefaultDecimalFormatting);
    PBScenario::Load(drData, *dr);
    if (!dr->IsValid())
      dst.Error("Ignoring invalid DataRequest for property " + dr->m_PropertyName);
    else
      dst.m_Requests.push_back(dr);
  }
}
cdm::DataRequestManagerData* PBScenario::Unload(const SEDataRequestManager& src)
{
  cdm::DataRequestManagerData* dst = new cdm::DataRequestManagerData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEDataRequestManager& src, cdm::DataRequestManagerData& dst)
{
  dst.set_resultsfilename(src.m_ResultsFilename);
  dst.set_samplespersecond(src.m_SamplesPerSecond);
  if (src.HasDefaultDecimalFormatting())
    dst.set_allocated_defaultdecimalformatting(PBScenario::Unload(*src.m_DefaultDecimalFormatting));
  if (src.HasOverrideDecimalFormatting())
    dst.set_allocated_overridedecimalformatting(PBScenario::Unload(*src.m_OverrideDecimalFormatting));
  for (SEDataRequest* dr : src.m_Requests)
    dst.mutable_datarequest()->AddAllocated(PBScenario::Unload(*dr));
}
void PBScenario::Copy(const SEDataRequestManager& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr)
{
  cdm::DataRequestManagerData data;
  PBScenario::Serialize(src, data);
  PBScenario::Serialize(data, dst, subMgr);
}

void PBScenario::Load(const cdm::DecimalFormatData& src, SEDecimalFormat& dst)
{
  PBScenario::Serialize(src, dst);
}
void PBScenario::Serialize(const cdm::DecimalFormatData& src, SEDecimalFormat& dst)
{
  dst.Clear();
  dst.SetNotation((eDecimalFormat_Type)src.type());
  dst.SetPrecision(src.precision());
}
cdm::DecimalFormatData* PBScenario::Unload(const SEDecimalFormat& src)
{
  cdm::DecimalFormatData* dst = new cdm::DecimalFormatData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEDecimalFormat& src, cdm::DecimalFormatData& dst)
{
  dst.set_type((cdm::eDecimalFormat_Type)src.m_Notation);
  dst.set_precision((google::protobuf::uint32)src.m_Precision);
}

bool PBScenario::SerializeToString(const SEScenario& src, std::string& output, SerializationMode m)
{
  cdm::ScenarioData data;
  PBScenario::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBScenario::SerializeToFile(const SEScenario& src, const std::string& filename, SerializationMode m)
{
  cdm::ScenarioData data;
  PBScenario::Serialize(src, data);
  std::string content;
  PBScenario::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBScenario::SerializeFromString(const std::string& src, SEScenario& dst, SerializationMode m)
{
  cdm::ScenarioData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBScenario::Load(data, dst);
  return true;
}
bool PBScenario::SerializeFromFile(const std::string& filename, SEScenario& dst, SerializationMode m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBScenario::SerializeFromString(content, dst, m);
}

bool PBScenario::SerializeToString(const SEConditionManager& src, std::string& output, SerializationMode m)
{
  cdm::ConditionListData data;
  PBScenario::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBScenario::SerializeToFile(const SEConditionManager& src, const std::string& filename, SerializationMode m)
{
  cdm::ConditionListData data;
  PBScenario::Serialize(src, data);
  std::string content;
  PBScenario::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBScenario::SerializeFromString(const std::string& src, SEConditionManager& dst, SerializationMode m)
{
  cdm::ConditionListData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBScenario::Load(data, dst);
  return true;
}
bool PBScenario::SerializeFromFile(const std::string& filename, SEConditionManager& dst, SerializationMode m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBScenario::SerializeFromString(content, dst, m);
}

bool PBScenario::SerializeToString(const SEActionManager& src, std::string& output, SerializationMode m)
{
  cdm::ActionListData data;
  PBScenario::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBScenario::SerializeToFile(const SEActionManager& src, const std::string& filename, SerializationMode m)
{
  cdm::ActionListData data;
  PBScenario::Serialize(src, data);
  std::string content;
  PBScenario::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBScenario::SerializeFromString(const std::string& src, SEActionManager& dst, SerializationMode m)
{
  cdm::ActionListData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBScenario::Load(data, dst);
  return true;
}
bool PBScenario::SerializeFromFile(const std::string& filename, SEActionManager& dst, SerializationMode m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBScenario::SerializeFromString(content, dst, m);
}

bool PBScenario::SerializeToString(const SEDataRequestManager& src, std::string& output, SerializationMode m)
{
  cdm::DataRequestManagerData data;
  PBScenario::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBScenario::SerializeToFile(const SEDataRequestManager& src, const std::string& filename, SerializationMode m)
{
  cdm::DataRequestManagerData data;
  PBScenario::Serialize(src, data);
  std::string content;
  PBScenario::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBScenario::SerializeFromString(const std::string& src, SEDataRequestManager& dst, SerializationMode m, const SESubstanceManager& subMgr)
{
  cdm::DataRequestManagerData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBScenario::Load(data, dst, subMgr);
  return true;
}
bool PBScenario::SerializeFromFile(const std::string& filename, SEDataRequestManager& dst, SerializationMode m, const SESubstanceManager& subMgr)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBScenario::SerializeFromString(content, dst, m, subMgr);
}