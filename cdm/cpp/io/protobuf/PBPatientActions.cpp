/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBPatientActions.h"
#include "io/protobuf/PBPatientNutrition.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBSubstance.h"
#include "io/protobuf/PBProperties.h"
#include "bind/cpp/cdm/PatientActions.pb.h"

#include "substance/SESubstanceManager.h"
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
#include "patient/actions/SESupplementalOxygen.h"
#include "patient/actions/SETensionPneumothorax.h"
#include "patient/actions/SEUrinate.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceCompound.h"
#include "substance/SESubstanceManager.h"

void PBPatientAction::Serialize(const cdm::PatientActionData& src, SEPatientAction& dst)
{
  PBAction::Serialize(src.action(), dst);
}
void PBPatientAction::Serialize(const SEPatientAction& src, cdm::PatientActionData& dst)
{
  PBAction::Serialize(src, *dst.mutable_action());
}

void PBPatientAction::Load(const cdm::AcuteStressData& src, SEAcuteStress& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::AcuteStressData& src, SEAcuteStress& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AcuteStressData* PBPatientAction::Unload(const SEAcuteStress& src)
{
  cdm::AcuteStressData* dst = new cdm::AcuteStressData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEAcuteStress& src, cdm::AcuteStressData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientAction::Copy(const SEAcuteStress& src, SEAcuteStress& dst)
{
  cdm::AcuteStressData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}


void PBPatientAction::Load(const cdm::AirwayObstructionData& src, SEAirwayObstruction& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::AirwayObstructionData& src, SEAirwayObstruction& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AirwayObstructionData* PBPatientAction::Unload(const SEAirwayObstruction& src)
{
  cdm::AirwayObstructionData* dst = new cdm::AirwayObstructionData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEAirwayObstruction& src, cdm::AirwayObstructionData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientAction::Copy(const SEAirwayObstruction& src, SEAirwayObstruction& dst)
{
  cdm::AirwayObstructionData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ApneaData& src, SEApnea& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ApneaData& src, SEApnea& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::ApneaData* PBPatientAction::Unload(const SEApnea& src)
{
  cdm::ApneaData* dst = new cdm::ApneaData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEApnea& src, cdm::ApneaData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientAction::Copy(const SEApnea& src, SEApnea& dst)
{
  cdm::ApneaData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::AsthmaAttackData& src, SEAsthmaAttack& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::AsthmaAttackData& src, SEAsthmaAttack& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AsthmaAttackData* PBPatientAction::Unload(const SEAsthmaAttack& src)
{
  cdm::AsthmaAttackData* dst = new cdm::AsthmaAttackData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEAsthmaAttack& src, cdm::AsthmaAttackData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientAction::Copy(const SEAsthmaAttack& src, SEAsthmaAttack& dst)
{
  cdm::AsthmaAttackData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::BrainInjuryData& src, SEBrainInjury& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::BrainInjuryData& src, SEBrainInjury& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
  dst.SetType((eBrainInjury_Type)src.type());
}
cdm::BrainInjuryData* PBPatientAction::Unload(const SEBrainInjury& src)
{
  cdm::BrainInjuryData* dst = new cdm::BrainInjuryData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEBrainInjury& src, cdm::BrainInjuryData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
  dst.set_type((cdm::BrainInjuryData::eType)src.m_Type);
}
void PBPatientAction::Copy(const SEBrainInjury& src, SEBrainInjury& dst)
{
  cdm::BrainInjuryData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::BreathHoldData& src, SEBreathHold& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::BreathHoldData& src, SEBreathHold& dst)
{
  dst.Clear();
  if (src.has_period())
    PBProperty::Load(src.period(), dst.GetPeriod());
}
cdm::BreathHoldData* PBPatientAction::Unload(const SEBreathHold& src)
{
  cdm:: BreathHoldData* dst = new cdm::BreathHoldData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEBreathHold& src, cdm::BreathHoldData& dst)
{
  if (src.HasPeriod())
    dst.set_allocated_period(PBProperty::Unload(*src.m_Period));
}
void PBPatientAction::Copy(const SEBreathHold& src, SEBreathHold& dst)
{
  cdm::BreathHoldData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::BronchoconstrictionData& src, SEBronchoconstriction& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::BronchoconstrictionData& src, SEBronchoconstriction& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::BronchoconstrictionData* PBPatientAction::Unload(const SEBronchoconstriction& src)
{
  cdm::BronchoconstrictionData* dst = new cdm::BronchoconstrictionData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEBronchoconstriction& src, cdm::BronchoconstrictionData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientAction::Copy(const SEBronchoconstriction& src, SEBronchoconstriction& dst)
{
  cdm::BronchoconstrictionData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::CardiacArrestData& src, SECardiacArrest& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::CardiacArrestData& src, SECardiacArrest& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
}
cdm::CardiacArrestData* PBPatientAction::Unload(const SECardiacArrest& src)
{
  cdm::CardiacArrestData* dst = new cdm::CardiacArrestData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SECardiacArrest& src, cdm::CardiacArrestData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_state((cdm::eSwitch)src.m_State);
}
void PBPatientAction::Copy(const SECardiacArrest& src, SECardiacArrest& dst)
{
  cdm::CardiacArrestData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ChestCompressionForceData& src, SEChestCompressionForce& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ChestCompressionForceData& src, SEChestCompressionForce& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_force())
    PBProperty::Load(src.force(), dst.GetForce());
}
cdm::ChestCompressionForceData* PBPatientAction::Unload(const SEChestCompressionForce& src)
{
  cdm::ChestCompressionForceData* dst = new cdm::ChestCompressionForceData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEChestCompressionForce& src, cdm::ChestCompressionForceData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasForce())
    dst.set_allocated_force(PBProperty::Unload(*src.m_Force));
}
void PBPatientAction::Copy(const SEChestCompressionForce& src, SEChestCompressionForce& dst)
{
  cdm::ChestCompressionForceData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ChestCompressionForceScaleData& src, SEChestCompressionForceScale& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ChestCompressionForceScaleData& src, SEChestCompressionForceScale& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_forcescale())
    PBProperty::Load(src.forcescale(), dst.GetForceScale());
  if (src.has_forceperiod())
    PBProperty::Load(src.forceperiod(), dst.GetForcePeriod());
}
cdm::ChestCompressionForceScaleData* PBPatientAction::Unload(const SEChestCompressionForceScale& src)
{
  cdm::ChestCompressionForceScaleData* dst = new cdm::ChestCompressionForceScaleData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEChestCompressionForceScale& src, cdm::ChestCompressionForceScaleData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasForceScale())
    dst.set_allocated_forcescale(PBProperty::Unload(*src.m_ForceScale));
  if (src.HasForcePeriod())
    dst.set_allocated_forceperiod(PBProperty::Unload(*src.m_ForcePeriod));
}
void PBPatientAction::Copy(const SEChestCompressionForceScale& src, SEChestCompressionForceScale& dst)
{
  cdm::ChestCompressionForceScaleData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ChestOcclusiveDressingData& src, SEChestOcclusiveDressing& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ChestOcclusiveDressingData& src, SEChestOcclusiveDressing& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  dst.SetSide((eSide)src.side());
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
}
cdm::ChestOcclusiveDressingData* PBPatientAction::Unload(const SEChestOcclusiveDressing& src)
{
  cdm::ChestOcclusiveDressingData* dst = new cdm::ChestOcclusiveDressingData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEChestOcclusiveDressing& src, cdm::ChestOcclusiveDressingData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_state((cdm::eSwitch)src.m_State);
  if (src.HasSide())
    dst.set_side((cdm::eSide)src.m_Side);
}
void PBPatientAction::Copy(const SEChestOcclusiveDressing& src, SEChestOcclusiveDressing& dst)
{
  cdm::ChestOcclusiveDressingData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ConsciousRespirationData& src, SEConsciousRespiration& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ConsciousRespirationData& src, SEConsciousRespiration& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  dst.m_ClearCommands = true;
  
  for (int i = 0; i < src.command().size(); i++)
  {
    const cdm::AnyConsciousRespirationCommandData& command = src.command()[i];

    switch (command.Command_case())
    {
    case cdm::AnyConsciousRespirationCommandData::CommandCase::kBreathHold:
      PBPatientAction::Load(command.breathhold(), dst.AddBreathHold());
      break;
    case cdm::AnyConsciousRespirationCommandData::CommandCase::kForcedExhale:
      PBPatientAction::Load(command.forcedexhale(), dst.AddForcedExhale());
      break;
    case cdm::AnyConsciousRespirationCommandData::CommandCase::kForcedInhale:
      PBPatientAction::Load(command.forcedinhale(), dst.AddForcedInhale());
      break;
    case cdm::AnyConsciousRespirationCommandData::CommandCase::kUseInhaler:
      PBPatientAction::Load(command.useinhaler(), dst.AddUseInhaler());
      break;
    default:
      dst.Warning("Ignoring unknown Conscious Respiration Command : " + command.Command_case());
      continue;
    }
    dst.m_Commands.back()->SetComment(command.comment());
  }
}
cdm::ConsciousRespirationData* PBPatientAction::Unload(const SEConsciousRespiration& src)
{
  cdm::ConsciousRespirationData* dst = new cdm::ConsciousRespirationData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEConsciousRespiration& src, cdm::ConsciousRespirationData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.appendtoprevious();
  for (SEConsciousRespirationCommand* cmd : src.m_Commands)
  {
    cdm::AnyConsciousRespirationCommandData* cmdData = dst.add_command();
    cmdData->set_comment(cmd->GetComment());
    SEBreathHold* bh = dynamic_cast<SEBreathHold*>(cmd);
    if (bh != nullptr)
    {
      cmdData->set_allocated_breathhold(PBPatientAction::Unload(*bh));
      continue;
    }
    SEForcedExhale* fe = dynamic_cast<SEForcedExhale*>(cmd);
    if (fe != nullptr)
    {
      cmdData->set_allocated_forcedexhale(PBPatientAction::Unload(*fe));
      continue;
    }
    SEForcedInhale* fi = dynamic_cast<SEForcedInhale*>(cmd);
    if (fi != nullptr)
    {
      cmdData->set_allocated_forcedinhale(PBPatientAction::Unload(*fi));
      continue;
    }
    SEUseInhaler* ui = dynamic_cast<SEUseInhaler*>(cmd);
    if (ui != nullptr)
    {
      cmdData->set_allocated_useinhaler(PBPatientAction::Unload(*ui));
      continue;
    }
    src.Warning("The unmapped respiration command in SEConsciousRespiration::Serialize");
  }
}
void PBPatientAction::Copy(const SEConsciousRespiration& src, SEConsciousRespiration& dst)
{
  cdm::ConsciousRespirationData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ConsumeNutrientsData& src, SEConsumeNutrients& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ConsumeNutrientsData& src, SEConsumeNutrients& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_nutrition())
    PBPatientNutrition::Load(src.nutrition(), dst.GetNutrition());
  else
    dst.SetNutritionFile(src.nutritionfile());
}
cdm::ConsumeNutrientsData* PBPatientAction::Unload(const SEConsumeNutrients& src)
{
  cdm::ConsumeNutrientsData* dst = new cdm::ConsumeNutrientsData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEConsumeNutrients& src, cdm::ConsumeNutrientsData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasNutrition())
    dst.set_allocated_nutrition(PBPatientNutrition::Unload(*src.m_Nutrition));
  else if (src.HasNutritionFile())
    dst.set_nutritionfile(src.m_NutritionFile);
}
void PBPatientAction::Copy(const SEConsumeNutrients& src, SEConsumeNutrients& dst)
{
  cdm::ConsumeNutrientsData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ExerciseData& src, SEExercise& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ExerciseData& src, SEExercise& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_intensity())
    PBProperty::Load(src.intensity(), dst.GetIntensity());
}
cdm::ExerciseData* PBPatientAction::Unload(const SEExercise& src)
{
  cdm::ExerciseData* dst = new cdm::ExerciseData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEExercise& src, cdm::ExerciseData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasIntensity())
    dst.set_allocated_intensity(PBProperty::Unload(*src.m_Intensity));
}
void PBPatientAction::Copy(const SEExercise& src, SEExercise& dst)
{
  cdm::ExerciseData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ForcedExhaleData& src, SEForcedExhale& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ForcedExhaleData& src, SEForcedExhale& dst)
{
  dst.Clear();
  if (src.has_expiratoryreservevolumefraction())
    PBProperty::Load(src.expiratoryreservevolumefraction(), dst.GetExpiratoryReserveVolumeFraction());
  if (src.has_period())
    PBProperty::Load(src.period(), dst.GetPeriod());
}
cdm::ForcedExhaleData* PBPatientAction::Unload(const SEForcedExhale& src)
{
  cdm::ForcedExhaleData* dst = new cdm::ForcedExhaleData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEForcedExhale& src, cdm::ForcedExhaleData& dst)
{
  if (src.HasExpiratoryReserveVolumeFraction())
    dst.set_allocated_expiratoryreservevolumefraction(PBProperty::Unload(*src.m_ExpiratoryReserveVolumeFraction));
  if (src.HasPeriod())
    dst.set_allocated_period(PBProperty::Unload(*src.m_Period));
}
void PBPatientAction::Copy(const SEForcedExhale& src, SEForcedExhale& dst)
{
  cdm::ForcedExhaleData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::ForcedInhaleData& src, SEForcedInhale& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::ForcedInhaleData& src, SEForcedInhale& dst)
{
  dst.Clear();
  if (src.has_inspiratorycapacityfraction())
    PBProperty::Load(src.inspiratorycapacityfraction(), dst.GetInspiratoryCapacityFraction());
  if (src.has_period())
    PBProperty::Load(src.period(), dst.GetPeriod());
}
cdm::ForcedInhaleData* PBPatientAction::Unload(const SEForcedInhale& src)
{
  cdm::ForcedInhaleData* dst = new cdm::ForcedInhaleData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEForcedInhale& src, cdm::ForcedInhaleData& dst)
{
  if (src.HasInspiratoryCapacityFraction())
    dst.set_allocated_inspiratorycapacityfraction(PBProperty::Unload(*src.m_InspiratoryCapacityFraction));
  if (src.HasPeriod())
    dst.set_allocated_period(PBProperty::Unload(*src.m_Period));
}
void PBPatientAction::Copy(const SEForcedInhale& src, SEForcedInhale& dst)
{
  cdm::ForcedInhaleData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::HemorrhageData& src, SEHemorrhage& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::HemorrhageData& src, SEHemorrhage& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_rate())
    PBProperty::Load(src.rate(), dst.GetRate());
  dst.m_Compartment = src.compartment();
  dst.SetType((eHemorrhage_Type)src.type());
}
cdm::HemorrhageData* PBPatientAction::Unload(const SEHemorrhage& src)
{
  cdm::HemorrhageData* dst = new cdm::HemorrhageData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEHemorrhage& src, cdm::HemorrhageData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasRate())
    dst.set_allocated_rate(PBProperty::Unload(*src.m_Rate));
  if (src.HasCompartment())
    dst.set_compartment(src.m_Compartment);
  dst.set_type((cdm::HemorrhageData::eType)src.m_Type);
}
void PBPatientAction::Copy(const SEHemorrhage& src, SEHemorrhage& dst)
{
  cdm::HemorrhageData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::IntubationData& src, SEIntubation& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::IntubationData& src, SEIntubation& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  dst.SetType((eIntubation_Type)src.type());
}
cdm::IntubationData* PBPatientAction::Unload(const SEIntubation& src)
{
  cdm::IntubationData* dst = new cdm::IntubationData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEIntubation& src, cdm::IntubationData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_type((cdm::IntubationData::eType)src.m_Type);
}
void PBPatientAction::Copy(const SEIntubation& src, SEIntubation& dst)
{
  cdm::IntubationData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::MechanicalVentilationData& src, SEMechanicalVentilation& dst, const SESubstanceManager& subMgr)
{
  PBPatientAction::Serialize(src, dst, subMgr);
}
void PBPatientAction::Serialize(const cdm::MechanicalVentilationData& src, SEMechanicalVentilation& dst, const SESubstanceManager& subMgr)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
  if (src.has_flow())
    PBProperty::Load(src.flow(), dst.GetFlow());
  if (src.has_pressure())
    PBProperty::Load(src.pressure(), dst.GetPressure());

  dst.m_GasFractions.clear();
  dst.m_cGasFractions.clear();
  SESubstance* sub;
  for (int i = 0; i < src.gasfraction_size(); i++)
  {
    const cdm::SubstanceFractionData& sfData = src.gasfraction()[i];
    sub = subMgr.GetSubstance(sfData.name());
    if (sub == nullptr)
    {
      dst.Error("MechanicalVentilation substance not found : " + sfData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Gas)
    {
      dst.Error("MechanicalVentilation substance not gas : " + sfData.name());
      continue;
    }
    PBSubstance::Load(sfData, dst.GetGasFraction(*sub));
  }

  for (int i = 0; i < src.aerosol_size(); i++)
  {
    const cdm::SubstanceConcentrationData& scData = src.aerosol()[i];
    sub = subMgr.GetSubstance(scData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring an environmental conditions aerosol that was not found : " + scData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Liquid && sub->GetState() != eSubstance_State::Solid)
    {
      dst.Error("Ignoring an environmental conditions aerosol that is not a gas : " + scData.name());
      continue;
    }
    PBSubstance::Load(scData, dst.GetAerosol(*sub));
  }
}
cdm::MechanicalVentilationData* PBPatientAction::Unload(const SEMechanicalVentilation& src)
{
  cdm::MechanicalVentilationData* dst = new cdm::MechanicalVentilationData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEMechanicalVentilation& src, cdm::MechanicalVentilationData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_state((cdm::eSwitch)src.m_State);
  if (src.HasFlow())
    dst.set_allocated_flow(PBProperty::Unload(*src.m_Flow));
  if (src.HasPressure())
    dst.set_allocated_pressure(PBProperty::Unload(*src.m_Pressure));
  for (SESubstanceFraction *sf : src.m_GasFractions)
    dst.mutable_gasfraction()->AddAllocated(PBSubstance::Unload(*sf));
  for (SESubstanceConcentration *sc : src.m_Aerosols)
    dst.mutable_aerosol()->AddAllocated(PBSubstance::Unload(*sc));
}
void PBPatientAction::Copy(const SEMechanicalVentilation& src, SEMechanicalVentilation& dst, const SESubstanceManager& subMgr)
{
  cdm::MechanicalVentilationData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst, subMgr);
}

void PBPatientAction::Load(const cdm::NeedleDecompressionData& src, SENeedleDecompression& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::NeedleDecompressionData& src, SENeedleDecompression& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  dst.SetSide((eSide)src.side());
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
}
cdm::NeedleDecompressionData* PBPatientAction::Unload(const SENeedleDecompression& src)
{
  cdm::NeedleDecompressionData* dst = new cdm::NeedleDecompressionData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SENeedleDecompression& src, cdm::NeedleDecompressionData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasSide())
    dst.set_side((cdm::eSide)src.m_Side);
  dst.set_state((cdm::eSwitch)src.m_State);
}
void PBPatientAction::Copy(const SENeedleDecompression& src, SENeedleDecompression& dst)
{
  cdm::NeedleDecompressionData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::PatientAssessmentRequestData& src, SEPatientAssessmentRequest& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::PatientAssessmentRequestData& src, SEPatientAssessmentRequest& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  dst.SetType((ePatientAssessment_Type)src.type());
}
cdm::PatientAssessmentRequestData* PBPatientAction::Unload(const SEPatientAssessmentRequest& src)
{
  cdm::PatientAssessmentRequestData* dst = new cdm::PatientAssessmentRequestData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEPatientAssessmentRequest& src, cdm::PatientAssessmentRequestData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_type((cdm::ePatientAssessmentType)src.m_Type);
}
void PBPatientAction::Copy(const SEPatientAssessmentRequest& src, SEPatientAssessmentRequest& dst)
{
  cdm::PatientAssessmentRequestData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::PericardialEffusionData& src, SEPericardialEffusion& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::PericardialEffusionData& src, SEPericardialEffusion& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_effusionrate())
    PBProperty::Load(src.effusionrate(), dst.GetEffusionRate());
}
cdm::PericardialEffusionData* PBPatientAction::Unload(const SEPericardialEffusion& src)
{
  cdm::PericardialEffusionData* dst = new cdm::PericardialEffusionData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEPericardialEffusion& src, cdm::PericardialEffusionData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasEffusionRate())
    dst.set_allocated_effusionrate(PBProperty::Unload(*src.m_EffusionRate));
}
void PBPatientAction::Copy(const SEPericardialEffusion& src, SEPericardialEffusion& dst)
{
  cdm::PericardialEffusionData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::SubstanceBolusData& src, SESubstanceBolus& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::SubstanceBolusData& src, SESubstanceBolus& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  dst.SetAdminRoute((eSubstanceAdministration_Route)src.administrationroute());
  if (src.has_dose())
    PBProperty::Load(src.dose(), dst.GetDose());
  if (src.has_concentration())
    PBProperty::Load(src.concentration(), dst.GetConcentration());
  if (src.has_state())
    PBPatientAction::Load(src.state(), dst.m_State);
}
cdm::SubstanceBolusData* PBPatientAction::Unload(const SESubstanceBolus& src)
{
  cdm::SubstanceBolusData* dst = new cdm::SubstanceBolusData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SESubstanceBolus& src, cdm::SubstanceBolusData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_substance(src.m_Substance.GetName());
  dst.set_administrationroute((cdm::SubstanceBolusData::eRoute)src.m_AdminRoute);
  if (src.HasDose())
    dst.set_allocated_dose(PBProperty::Unload(*src.m_Dose));
  if (src.HasConcentration())
    dst.set_allocated_concentration(PBProperty::Unload(*src.m_Concentration));
  dst.set_allocated_state(PBPatientAction::Unload(src.m_State));
}
void PBPatientAction::Copy(const SESubstanceBolus& src, SESubstanceBolus& dst)
{
  cdm::SubstanceBolusData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::SubstanceBolusStateData& src, SESubstanceBolusState& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::SubstanceBolusStateData& src, SESubstanceBolusState& dst)
{
  if (src.has_elapsedtime())
    PBProperty::Load(src.elapsedtime(), dst.GetElapsedTime());
  if (src.has_administereddose())
    PBProperty::Load(src.administereddose(), dst.GetAdministeredDose());
}
cdm::SubstanceBolusStateData* PBPatientAction::Unload(const SESubstanceBolusState& src)
{
  cdm::SubstanceBolusStateData* dst = new cdm::SubstanceBolusStateData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SESubstanceBolusState& src, cdm::SubstanceBolusStateData& dst)
{
  dst.set_substance(src.m_Substance.GetName());
  dst.set_allocated_elapsedtime(PBProperty::Unload(*src.m_ElapsedTime));
  dst.set_allocated_administereddose(PBProperty::Unload(*src.m_AdministeredDose));
}
void PBPatientAction::Copy(const SESubstanceBolusState& src, SESubstanceBolusState& dst)
{
  cdm::SubstanceBolusStateData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::SubstanceCompoundInfusionData& src, SESubstanceCompoundInfusion& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::SubstanceCompoundInfusionData& src, SESubstanceCompoundInfusion& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_rate())
    PBProperty::Load(src.rate(), dst.GetRate());
  if (src.has_bagvolume())
    PBProperty::Load(src.bagvolume(), dst.GetBagVolume());
}
cdm::SubstanceCompoundInfusionData* PBPatientAction::Unload(const SESubstanceCompoundInfusion& src)
{
  cdm::SubstanceCompoundInfusionData* dst = new cdm::SubstanceCompoundInfusionData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SESubstanceCompoundInfusion& src, cdm::SubstanceCompoundInfusionData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_substancecompound(src.m_Compound.GetName());
  if (src.HasRate())
    dst.set_allocated_rate(PBProperty::Unload(*src.m_Rate));
  if (src.HasBagVolume())
    dst.set_allocated_bagvolume(PBProperty::Unload(*src.m_BagVolume));
}
void PBPatientAction::Copy(const SESubstanceCompoundInfusion& src, SESubstanceCompoundInfusion& dst)
{
  cdm::SubstanceCompoundInfusionData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::SubstanceInfusionData& src, SESubstanceInfusion& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::SubstanceInfusionData& src, SESubstanceInfusion& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_rate())
    PBProperty::Load(src.rate(), dst.GetRate());
  if (src.has_concentration())
    PBProperty::Load(src.concentration(), dst.GetConcentration());
}
cdm::SubstanceInfusionData* PBPatientAction::Unload(const SESubstanceInfusion& src)
{
  cdm::SubstanceInfusionData* dst = new cdm::SubstanceInfusionData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SESubstanceInfusion& src, cdm::SubstanceInfusionData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_substance(src.m_Substance.GetName());
  if (src.HasRate())
    dst.set_allocated_rate(PBProperty::Unload(*src.m_Rate));
  if (src.HasConcentration())
    dst.set_allocated_concentration(PBProperty::Unload(*src.m_Concentration));
}
void PBPatientAction::Copy(const SESubstanceInfusion& src, SESubstanceInfusion& dst)
{
  cdm::SubstanceInfusionData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::SupplementalOxygenData& src, SESupplementalOxygen& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::SupplementalOxygenData& src, SESupplementalOxygen& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  dst.SetDevice((eSupplementalOxygen_Device)src.device());
  if (src.has_flow())
    PBProperty::Load(src.flow(), dst.GetFlow());
  if (src.has_volume())
    PBProperty::Load(src.volume(), dst.GetVolume());
}
cdm::SupplementalOxygenData* PBPatientAction::Unload(const SESupplementalOxygen& src)
{
  cdm::SupplementalOxygenData* dst = new cdm::SupplementalOxygenData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SESupplementalOxygen& src, cdm::SupplementalOxygenData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_device((cdm::SupplementalOxygenData::eDevice)src.m_Device);
  if (src.HasFlow())
    dst.set_allocated_flow(PBProperty::Unload(*src.m_Flow));
  if (src.HasVolume())
    dst.set_allocated_volume(PBProperty::Unload(*src.m_Volume));
}
void PBPatientAction::Copy(const SESupplementalOxygen& src, SESupplementalOxygen& dst)
{
  cdm::SupplementalOxygenData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::TensionPneumothoraxData& src, SETensionPneumothorax& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::TensionPneumothoraxData& src, SETensionPneumothorax& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
  dst.SetType((eGate)src.type());
  dst.SetSide((eSide)src.side());
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::TensionPneumothoraxData* PBPatientAction::Unload(const SETensionPneumothorax& src)
{
  cdm::TensionPneumothoraxData* dst = new cdm::TensionPneumothoraxData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SETensionPneumothorax& src, cdm::TensionPneumothoraxData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasType())
    dst.set_type((cdm::eGate)src.m_Type);
  if (src.HasSide())
    dst.set_side((cdm::eSide)src.m_Side);
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBPatientAction::Copy(const SETensionPneumothorax& src, SETensionPneumothorax& dst)
{
  cdm::TensionPneumothoraxData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::UrinateData& src, SEUrinate& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::UrinateData& src, SEUrinate& dst)
{
  PBPatientAction::Serialize(src.patientaction(), dst);
}
cdm::UrinateData* PBPatientAction::Unload(const SEUrinate& src)
{
  cdm::UrinateData* dst = new cdm::UrinateData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEUrinate& src, cdm::UrinateData& dst)
{
  PBPatientAction::Serialize(src, *dst.mutable_patientaction());
}
void PBPatientAction::Copy(const SEUrinate& src, SEUrinate& dst)
{
  cdm::UrinateData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}

void PBPatientAction::Load(const cdm::UseInhalerData& src, SEUseInhaler& dst)
{
  PBPatientAction::Serialize(src, dst);
}
void PBPatientAction::Serialize(const cdm::UseInhalerData& src, SEUseInhaler& dst)
{
  dst.Clear();
}
cdm::UseInhalerData* PBPatientAction::Unload(const SEUseInhaler& src)
{
  cdm::UseInhalerData* dst = new cdm::UseInhalerData();
  PBPatientAction::Serialize(src, *dst);
  return dst;
}
void PBPatientAction::Serialize(const SEUseInhaler& src, cdm::UseInhalerData& dst)
{

}
void PBPatientAction::Copy(const SEUseInhaler& src, SEUseInhaler& dst)
{
  cdm::UseInhalerData data;
  PBPatientAction::Serialize(src, data);
  PBPatientAction::Serialize(data, dst);
}


SEPatientAction* PBPatientAction::Load(const cdm::AnyPatientActionData& any, SESubstanceManager& subMgr)
{
  switch (any.Action_case())
  {
  case cdm::AnyPatientActionData::ActionCase::kAssessment:
  {
    SEPatientAssessmentRequest* a = new SEPatientAssessmentRequest();
    PBPatientAction::Load(any.assessment(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kAcuteStress:
  {
    SEAcuteStress* a = new SEAcuteStress();
    PBPatientAction::Load(any.acutestress(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kAirwayObstruction:
  {
    SEAirwayObstruction* a = new SEAirwayObstruction();
    PBPatientAction::Load(any.airwayobstruction(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kApnea:
  {
    SEApnea* a = new SEApnea();
    PBPatientAction::Load(any.apnea(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kAsthmaAttack:
  {
    SEAsthmaAttack* a = new SEAsthmaAttack();
    PBPatientAction::Load(any.asthmaattack(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kBrainInjury:
  {
    SEBrainInjury* a = new SEBrainInjury();
    PBPatientAction::Load(any.braininjury(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kBronchoconstriction:
  {
    SEBronchoconstriction* a = new SEBronchoconstriction();
    PBPatientAction::Load(any.bronchoconstriction(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kCardiacArrest:
  {
    SECardiacArrest* a = new SECardiacArrest();
    PBPatientAction::Load(any.cardiacarrest(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kChestCompressionForce:
  {
    SEChestCompressionForce* a = new SEChestCompressionForce();
    PBPatientAction::Load(any.chestcompressionforce(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kChestCompressionForceScale:
  {
    SEChestCompressionForceScale* a = new SEChestCompressionForceScale();
    PBPatientAction::Load(any.chestcompressionforcescale(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kChestOcclusiveDressing:
  {
    SEChestOcclusiveDressing* a = new SEChestOcclusiveDressing();
    PBPatientAction::Load(any.chestocclusivedressing(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kConsciousRespiration:
  {
    SEConsciousRespiration* a = new SEConsciousRespiration();
    PBPatientAction::Load(any.consciousrespiration(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kConsumeNutrients:
  {
    SEConsumeNutrients* a = new SEConsumeNutrients();
    PBPatientAction::Load(any.consumenutrients(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kExercise:
  {
    SEExercise* a = new SEExercise();
    PBPatientAction::Load(any.exercise(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kHemorrhage:
  {
    SEHemorrhage* a = new SEHemorrhage();
    PBPatientAction::Load(any.hemorrhage(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kIntubation:
  {
    SEIntubation* a = new SEIntubation();
    PBPatientAction::Load(any.intubation(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kMechanicalVentilation:
  {
    SEMechanicalVentilation* a = new SEMechanicalVentilation();
    PBPatientAction::Load(any.mechanicalventilation(), *a, subMgr);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kNeedleDecompression:
  {
    SENeedleDecompression* a = new SENeedleDecompression();
    PBPatientAction::Load(any.needledecompression(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kPericardialEffusion:
  {
    SEPericardialEffusion* a = new SEPericardialEffusion();
    PBPatientAction::Load(any.pericardialeffusion(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kSubstanceBolus:
  {
    SESubstance* sub = subMgr.GetSubstance(any.substancebolus().substance());
    if (sub == nullptr)
    {
      subMgr.Error("Unknown substance " + any.substancebolus().substance(), "PBPatientAction::Load");
      return nullptr;
    }
    SESubstanceBolus* a = new SESubstanceBolus(*sub);
    PBPatientAction::Load(any.substancebolus(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kSubstanceInfusion:
  {
    SESubstance* sub = subMgr.GetSubstance(any.substanceinfusion().substance());
    if (sub == nullptr)
    {
      subMgr.Error("Unknown substance " + any.substanceinfusion().substance(), "PBPatientAction::Load");
      return nullptr;
    }
    SESubstanceInfusion* a = new SESubstanceInfusion(*sub);
    PBPatientAction::Load(any.substanceinfusion(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kSubstanceCompoundInfusion:
  {
    SESubstanceCompound* subC = subMgr.GetCompound(any.substancecompoundinfusion().substancecompound());
    if (subC == nullptr)
    {
      subMgr.Error("Unknown substance compound " + any.substancecompoundinfusion().substancecompound(), "PBPatientAction::Load");
      return nullptr;
    }
    SESubstanceCompoundInfusion* a = new SESubstanceCompoundInfusion(*subC);
    PBPatientAction::Load(any.substancecompoundinfusion(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kSupplementalOxygenData:
  {
    SESupplementalOxygen* a = new SESupplementalOxygen();
    PBPatientAction::Load(any.supplementaloxygendata(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kTensionPneumothorax:
  {
    SETensionPneumothorax* a = new SETensionPneumothorax();
    PBPatientAction::Load(any.tensionpneumothorax(), *a);
    return a;
  }
  case cdm::AnyPatientActionData::ActionCase::kUrinate:
  {
    SEUrinate* a = new SEUrinate();
    PBPatientAction::Load(any.urinate(), *a);
    return a;
  }
  }
  subMgr.Error("Unknown action type : " + any.Action_case());
  return nullptr;
}
cdm::AnyPatientActionData* PBPatientAction::Unload(const SEPatientAction& action)
{
  cdm::AnyPatientActionData* any = new cdm::AnyPatientActionData();
  const SEPatientAssessmentRequest* ar = dynamic_cast<const SEPatientAssessmentRequest*>(&action);
  if (ar != nullptr)
  {
    any->set_allocated_assessment(PBPatientAction::Unload(*ar));
    return any;
  }
  const SEAcuteStress* as = dynamic_cast<const SEAcuteStress*>(&action);
  if (as != nullptr)
  {
    any->set_allocated_acutestress(PBPatientAction::Unload(*as));
    return any;
  }
  const SEAirwayObstruction* ao = dynamic_cast<const SEAirwayObstruction*>(&action);
  if (ao != nullptr)
  {
    any->set_allocated_airwayobstruction(PBPatientAction::Unload(*ao));
    return any;
  }
  const SEApnea* a = dynamic_cast<const SEApnea*>(&action);
  if (a != nullptr)
  {
    any->set_allocated_apnea(PBPatientAction::Unload(*a));
    return any;
  }
  const SEAsthmaAttack* aa = dynamic_cast<const SEAsthmaAttack*>(&action);
  if (aa != nullptr)
  {
    any->set_allocated_asthmaattack(PBPatientAction::Unload(*aa));
    return any;
  }
  const SEBrainInjury* bi = dynamic_cast<const SEBrainInjury*>(&action);
  if (bi != nullptr)
  {
    any->set_allocated_braininjury(PBPatientAction::Unload(*bi));
    return any;
  }
  const SEBronchoconstriction* b = dynamic_cast<const SEBronchoconstriction*>(&action);
  if (b != nullptr)
  {
    any->set_allocated_bronchoconstriction(PBPatientAction::Unload(*b));
    return any;
  }
  const SECardiacArrest* ca = dynamic_cast<const SECardiacArrest*>(&action);
  if (ca != nullptr)
  {
    any->set_allocated_cardiacarrest(PBPatientAction::Unload(*ca));
    return any;
  }
  const SEChestCompressionForce* ccf = dynamic_cast<const SEChestCompressionForce*>(&action);
  if (ccf != nullptr)
  {
    any->set_allocated_chestcompressionforce(PBPatientAction::Unload(*ccf));
    return any;
  }
  const SEChestCompressionForceScale* ccfs = dynamic_cast<const SEChestCompressionForceScale*>(&action);
  if (ccfs != nullptr)
  {
    any->set_allocated_chestcompressionforcescale(PBPatientAction::Unload(*ccfs));
    return any;
  }
  const SEChestOcclusiveDressing* chd = dynamic_cast<const SEChestOcclusiveDressing*>(&action);
  if (chd != nullptr)
  {
    any->set_allocated_chestocclusivedressing(PBPatientAction::Unload(*chd));
    return any;
  }
  const SEConsciousRespiration* cr = dynamic_cast<const SEConsciousRespiration*>(&action);
  if (cr != nullptr)
  {
    any->set_allocated_consciousrespiration(PBPatientAction::Unload(*cr));
    return any;
  }
  const SEConsumeNutrients* cn = dynamic_cast<const SEConsumeNutrients*>(&action);
  if (cn != nullptr)
  {
    any->set_allocated_consumenutrients(PBPatientAction::Unload(*cn));
    return any;
  }
  const SEExercise* e = dynamic_cast<const SEExercise*>(&action);
  if (e != nullptr)
  {
    any->set_allocated_exercise(PBPatientAction::Unload(*e));
    return any;
  }
  const SEHemorrhage* h = dynamic_cast<const SEHemorrhage*>(&action);
  if (h != nullptr)
  {
    any->set_allocated_hemorrhage(PBPatientAction::Unload(*h));
    return any;
  }
  const SEIntubation* i = dynamic_cast<const SEIntubation*>(&action);
  if (i != nullptr)
  {
    any->set_allocated_intubation(PBPatientAction::Unload(*i));
    return any;
  }
  const SEMechanicalVentilation* mv = dynamic_cast<const SEMechanicalVentilation*>(&action);
  if (mv != nullptr)
  {
    any->set_allocated_mechanicalventilation(PBPatientAction::Unload(*mv));
    return any;
  }
  const SENeedleDecompression* nd = dynamic_cast<const SENeedleDecompression*>(&action);
  if (nd != nullptr)
  {
    any->set_allocated_needledecompression(PBPatientAction::Unload(*nd));
    return any;
  }
  const SEPericardialEffusion* pe = dynamic_cast<const SEPericardialEffusion*>(&action);
  if (pe != nullptr)
  {
    any->set_allocated_pericardialeffusion(PBPatientAction::Unload(*pe));
    return any;
  }
  const SESubstanceBolus* sb = dynamic_cast<const SESubstanceBolus*>(&action);
  if (sb != nullptr)
  {
    any->set_allocated_substancebolus(PBPatientAction::Unload(*sb));
    return any;
  }
  const SESubstanceInfusion* si = dynamic_cast<const SESubstanceInfusion*>(&action);
  if (si != nullptr)
  {
    any->set_allocated_substanceinfusion(PBPatientAction::Unload(*si));
    return any;
  }
  const SESubstanceCompoundInfusion* sci = dynamic_cast<const SESubstanceCompoundInfusion*>(&action);
  if (sci != nullptr)
  {
    any->set_allocated_substancecompoundinfusion(PBPatientAction::Unload(*sci));
    return any;
  }
  const SETensionPneumothorax* tp = dynamic_cast<const SETensionPneumothorax*>(&action);
  if (tp != nullptr)
  {
    any->set_allocated_tensionpneumothorax(PBPatientAction::Unload(*tp));
    return any;
  }
  const SEUrinate* u = dynamic_cast<const SEUrinate*>(&action);
  if (u != nullptr)
  {
    any->set_allocated_urinate(PBPatientAction::Unload(*u));
    return any;
  }
  delete any;
  return nullptr;
}
