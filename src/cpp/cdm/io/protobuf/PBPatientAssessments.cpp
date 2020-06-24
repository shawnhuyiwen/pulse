/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/PatientAssessments.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBUtils.h"
#include "io/protobuf/PBPatientAssessments.h"
#include "io/protobuf/PBProperties.h"
#include "patient/assessments/SECompleteBloodCount.h"
#include "patient/assessments/SEComprehensiveMetabolicPanel.h"
#include "patient/assessments/SEPulmonaryFunctionTest.h"
#include "patient/assessments/SEUrinalysis.h"
#include "patient/assessments/SEUrinalysisMicroscopic.h"
#include "utils/FileUtils.h"


void PBPatientAssessment::Serialize(const CDM_BIND::PatientAssessmentData& src, SEPatientAssessment& dst)
{

}

void PBPatientAssessment::Serialize(const SEPatientAssessment& src, CDM_BIND::PatientAssessmentData& dst)
{

}

bool PBPatientAssessment::SerializeToString(const SECompleteBloodCount& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::CompleteBloodCountData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientAssessment::SerializeToFile(const SECompleteBloodCount& src, const std::string& filename)
{
  CDM_BIND::CompleteBloodCountData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

void PBPatientAssessment::Load(const CDM_BIND::CompleteBloodCountData& src, SECompleteBloodCount& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::CompleteBloodCountData& src, SECompleteBloodCount& dst)
{
  PBPatientAssessment::Serialize(src.patientassessment(), dst);
  if (src.has_hematocrit())
    PBProperty::Load(src.hematocrit(), dst.GetHematocrit());
  if (src.has_hemoglobin())
    PBProperty::Load(src.hemoglobin(), dst.GetHemoglobin());
  if (src.has_plateletcount())
    PBProperty::Load(src.plateletcount(), dst.GetPlateletCount());
  if (src.has_meancorpuscularhemoglobin())
    PBProperty::Load(src.meancorpuscularhemoglobin(), dst.GetMeanCorpuscularHemoglobin());
  if (src.has_meancorpuscularhemoglobinconcentration())
    PBProperty::Load(src.meancorpuscularhemoglobinconcentration(), dst.GetMeanCorpuscularHemoglobinConcentration());
  if (src.has_meancorpuscularvolume())
    PBProperty::Load(src.meancorpuscularvolume(), dst.GetMeanCorpuscularVolume());
  if (src.has_redbloodcellcount())
    PBProperty::Load(src.redbloodcellcount(), dst.GetRedBloodCellCount());
  if (src.has_whitebloodcellcount())
    PBProperty::Load(src.whitebloodcellcount(), dst.GetWhiteBloodCellCount());
}

CDM_BIND::CompleteBloodCountData* PBPatientAssessment::Unload(const SECompleteBloodCount& src)
{
  CDM_BIND::CompleteBloodCountData* dst = new CDM_BIND::CompleteBloodCountData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SECompleteBloodCount& src, CDM_BIND::CompleteBloodCountData& dst)
{
  PBPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasHematocrit())
    dst.set_allocated_hematocrit(PBProperty::Unload(*src.m_Hematocrit));
  if (src.HasHemoglobin())
    dst.set_allocated_hemoglobin(PBProperty::Unload(*src.m_Hemoglobin));
  if (src.HasPlateletCount())
    dst.set_allocated_plateletcount(PBProperty::Unload(*src.m_PlateletCount));
  if (src.HasMeanCorpuscularHemoglobin())
    dst.set_allocated_meancorpuscularhemoglobin(PBProperty::Unload(*src.m_MeanCorpuscularHemoglobin));
  if (src.HasMeanCorpuscularHemoglobinConcentration())
    dst.set_allocated_meancorpuscularhemoglobinconcentration(PBProperty::Unload(*src.m_MeanCorpuscularHemoglobinConcentration));
  if (src.HasMeanCorpuscularVolume())
    dst.set_allocated_meancorpuscularvolume(PBProperty::Unload(*src.m_MeanCorpuscularVolume));
  if (src.HasRedBloodCellCount())
    dst.set_allocated_redbloodcellcount(PBProperty::Unload(*src.m_RedBloodCellCount));
  if (src.HasWhiteBloodCellCount())
    dst.set_allocated_whitebloodcellcount(PBProperty::Unload(*src.m_WhiteBloodCellCount));
}

bool PBPatientAssessment::SerializeToString(const SEComprehensiveMetabolicPanel& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::ComprehensiveMetabolicPanelData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientAssessment::SerializeToFile(const SEComprehensiveMetabolicPanel& src, const std::string& filename)
{
  CDM_BIND::ComprehensiveMetabolicPanelData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

void PBPatientAssessment::Load(const CDM_BIND::ComprehensiveMetabolicPanelData& src, SEComprehensiveMetabolicPanel& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::ComprehensiveMetabolicPanelData& src, SEComprehensiveMetabolicPanel& dst)
{
  PBPatientAssessment::Serialize(src.patientassessment(), dst);
  if (src.has_albumin())
    PBProperty::Load(src.albumin(), dst.GetAlbumin());
  if (src.has_alp())
    PBProperty::Load(src.alp(), dst.GetALP());
  if (src.has_alt())
    PBProperty::Load(src.alt(), dst.GetALT());
  if (src.has_ast())
    PBProperty::Load(src.ast(), dst.GetAST());
  if (src.has_bun())
    PBProperty::Load(src.bun(), dst.GetBUN());
  if (src.has_calcium())
    PBProperty::Load(src.calcium(), dst.GetCalcium());
  if (src.has_chloride())
    PBProperty::Load(src.chloride(), dst.GetChloride());
  if (src.has_co2())
    PBProperty::Load(src.co2(), dst.GetCO2());
  if (src.has_creatinine())
    PBProperty::Load(src.creatinine(), dst.GetCreatinine());
  if (src.has_glucose())
    PBProperty::Load(src.glucose(), dst.GetGlucose());
  if (src.has_potassium())
    PBProperty::Load(src.potassium(), dst.GetPotassium());
  if (src.has_sodium())
    PBProperty::Load(src.sodium(), dst.GetSodium());
  if (src.has_totalbilirubin())
    PBProperty::Load(src.totalbilirubin(), dst.GetTotalBilirubin());
  if (src.has_totalprotein())
    PBProperty::Load(src.totalprotein(), dst.GetTotalProtein());
}

CDM_BIND::ComprehensiveMetabolicPanelData* PBPatientAssessment::Unload(const SEComprehensiveMetabolicPanel& src)
{
  CDM_BIND::ComprehensiveMetabolicPanelData* dst = new CDM_BIND::ComprehensiveMetabolicPanelData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SEComprehensiveMetabolicPanel& src, CDM_BIND::ComprehensiveMetabolicPanelData& dst)
{
  PBPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasAlbumin())
    dst.set_allocated_albumin(PBProperty::Unload(*src.m_Albumin));
  if (src.HasALP())
    dst.set_allocated_alp(PBProperty::Unload(*src.m_ALP));
  if (src.HasALT())
    dst.set_allocated_alt(PBProperty::Unload(*src.m_ALT));
  if (src.HasAST())
    dst.set_allocated_ast(PBProperty::Unload(*src.m_AST));
  if (src.HasBUN())
    dst.set_allocated_bun(PBProperty::Unload(*src.m_BUN));
  if (src.HasCalcium())
    dst.set_allocated_calcium(PBProperty::Unload(*src.m_Calcium));
  if (src.HasChloride())
    dst.set_allocated_chloride(PBProperty::Unload(*src.m_Chloride));
  if (src.HasCO2())
    dst.set_allocated_co2(PBProperty::Unload(*src.m_CO2));
  if (src.HasCreatinine())
    dst.set_allocated_creatinine(PBProperty::Unload(*src.m_Creatinine));
  if (src.HasGlucose())
    dst.set_allocated_glucose(PBProperty::Unload(*src.m_Glucose));
  if (src.HasPotassium())
    dst.set_allocated_potassium(PBProperty::Unload(*src.m_Potassium));
  if (src.HasSodium())
    dst.set_allocated_sodium(PBProperty::Unload(*src.m_Sodium));
  if (src.HasTotalBilirubin())
    dst.set_allocated_totalbilirubin(PBProperty::Unload(*src.m_TotalBilirubin));
  if (src.HasTotalProtein())
    dst.set_allocated_totalprotein(PBProperty::Unload(*src.m_TotalProtein));
}


bool PBPatientAssessment::SerializeToString(const SEPulmonaryFunctionTest& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::PulmonaryFunctionTestData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientAssessment::SerializeToFile(const SEPulmonaryFunctionTest& src, const std::string& filename)
{
  CDM_BIND::PulmonaryFunctionTestData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}


void PBPatientAssessment::Load(const CDM_BIND::PulmonaryFunctionTestData& src, SEPulmonaryFunctionTest& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::PulmonaryFunctionTestData& src, SEPulmonaryFunctionTest& dst)
{
  PBPatientAssessment::Serialize(src.patientassessment(), dst);
  if (src.has_expiratoryreservevolume())
    PBProperty::Load(src.expiratoryreservevolume(), dst.GetExpiratoryReserveVolume());
  if (src.has_forcedvitalcapacity())
    PBProperty::Load(src.forcedvitalcapacity(), dst.GetForcedVitalCapacity());
  if (src.has_forcedexpiratoryvolume())
    PBProperty::Load(src.forcedexpiratoryvolume(), dst.GetForcedExpiratoryVolume());
  if (src.has_forcedexpiratoryflow())
    PBProperty::Load(src.forcedexpiratoryflow(), dst.GetForcedExpiratoryFlow());
  if (src.has_functionalresidualcapacity())
    PBProperty::Load(src.functionalresidualcapacity(), dst.GetFunctionalResidualCapacity());
  if (src.has_inspiratorycapacity())
    PBProperty::Load(src.inspiratorycapacity(), dst.GetInspiratoryCapacity());
  if (src.has_maximumvoluntaryventilation())
    PBProperty::Load(src.maximumvoluntaryventilation(), dst.GetMaximumVoluntaryVentilation());
  if (src.has_peakexpiratoryflow())
    PBProperty::Load(src.peakexpiratoryflow(), dst.GetPeakExpiratoryFlow());
  if (src.has_residualvolume())
    PBProperty::Load(src.residualvolume(), dst.GetResidualVolume());
  if (src.has_slowvitalcapacity())
    PBProperty::Load(src.slowvitalcapacity(), dst.GetSlowVitalCapacity());
  if (src.has_totallungcapacity())
    PBProperty::Load(src.totallungcapacity(), dst.GetTotalLungCapacity());
  if (src.has_vitalcapacity())
    PBProperty::Load(src.vitalcapacity(), dst.GetVitalCapacity());
  if (src.has_lungvolumeplot())
    PBProperty::Load(src.lungvolumeplot(), dst.GetLungVolumePlot());
}

CDM_BIND::PulmonaryFunctionTestData* PBPatientAssessment::Unload(const SEPulmonaryFunctionTest& src)
{
  CDM_BIND::PulmonaryFunctionTestData* dst = new CDM_BIND::PulmonaryFunctionTestData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SEPulmonaryFunctionTest& src, CDM_BIND::PulmonaryFunctionTestData& dst)
{
  PBPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasExpiratoryReserveVolume())
    dst.set_allocated_expiratoryreservevolume(PBProperty::Unload(*src.m_ExpiratoryReserveVolume));
  if (src.HasForcedVitalCapacity())
    dst.set_allocated_forcedvitalcapacity(PBProperty::Unload(*src.m_ForcedVitalCapacity));
  if (src.HasForcedExpiratoryVolume())
    dst.set_allocated_forcedexpiratoryvolume(PBProperty::Unload(*src.m_ForcedExpiratoryVolume));
  if (src.HasForcedExpiratoryFlow())
    dst.set_allocated_forcedexpiratoryflow(PBProperty::Unload(*src.m_ForcedExpiratoryFlow));
  if (src.HasFunctionalResidualCapacity())
    dst.set_allocated_functionalresidualcapacity(PBProperty::Unload(*src.m_FunctionalResidualCapacity));
  if (src.HasInspiratoryCapacity())
    dst.set_allocated_inspiratorycapacity(PBProperty::Unload(*src.m_InspiratoryCapacity));
  if (src.HasInspiratoryReserveVolume())
    dst.set_allocated_inspiratoryreservevolume(PBProperty::Unload(*src.m_InspiratoryReserveVolume));
  if (src.HasMaximumVoluntaryVentilation())
    dst.set_allocated_maximumvoluntaryventilation(PBProperty::Unload(*src.m_MaximumVoluntaryVentilation));
  if (src.HasPeakExpiratoryFlow())
    dst.set_allocated_peakexpiratoryflow(PBProperty::Unload(*src.m_PeakExpiratoryFlow));
  if (src.HasResidualVolume())
    dst.set_allocated_residualvolume(PBProperty::Unload(*src.m_ResidualVolume));
  if (src.HasSlowVitalCapacity())
    dst.set_allocated_slowvitalcapacity(PBProperty::Unload(*src.m_SlowVitalCapacity));
  if (src.HasTotalLungCapacity())
    dst.set_allocated_totallungcapacity(PBProperty::Unload(*src.m_TotalLungCapacity));
  if (src.HasVitalCapacity())
    dst.set_allocated_vitalcapacity(PBProperty::Unload(*src.m_VitalCapacity));
  if (src.HasLungVolumePlot())
    dst.set_allocated_lungvolumeplot(PBProperty::Unload(*src.m_LungVolumePlot));
}

bool PBPatientAssessment::SerializeToString(const SEUrinalysis& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::UrinalysisData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientAssessment::SerializeToFile(const SEUrinalysis& src, const std::string& filename)
{
  CDM_BIND::UrinalysisData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

void PBPatientAssessment::Load(const CDM_BIND::UrinalysisData& src, SEUrinalysis& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::UrinalysisData& src, SEUrinalysis& dst)
{
  PBPatientAssessment::Serialize(src.patientassessment(), dst);
  dst.SetColorResult((eUrinalysis_UrineColor)src.color());
  dst.SetAppearanceResult((eUrinalysis_ClarityIndicator)src.appearance());
  dst.SetGlucoseResult((eUrinalysis_PresenceIndicator)src.glucose());
  dst.SetKetoneResult((eUrinalysis_PresenceIndicator)src.ketone());
  dst.SetBloodResult((eUrinalysis_PresenceIndicator)src.blood());
  dst.SetProteinResult((eUrinalysis_PresenceIndicator)src.protein());
  dst.SetNitriteResult((eUrinalysis_PresenceIndicator)src.nitrite());
  dst.SetLeukocyteEsteraseResult((eUrinalysis_PresenceIndicator)src.leukocyteesterase());
  if (src.has_bilirubin())
    PBProperty::Load(src.bilirubin(), dst.GetBilirubinResult());
  if (src.has_specificgravity())
    PBProperty::Load(src.specificgravity(), dst.GetSpecificGravityResult());
  if (src.has_ph())
    PBProperty::Load(src.ph(), dst.GetPHResult());
  if (src.has_urobilinogen())
    PBProperty::Load(src.urobilinogen(), dst.GetUrobilinogenResult());
  if (src.has_microscopic())
    PBPatientAssessment::Load(src.microscopic(), dst.GetMicroscopicResult());
}

CDM_BIND::UrinalysisData* PBPatientAssessment::Unload(const SEUrinalysis& src)
{
  CDM_BIND::UrinalysisData* dst = new CDM_BIND::UrinalysisData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SEUrinalysis& src, CDM_BIND::UrinalysisData& dst)
{
  PBPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasColorResult())
    dst.set_color((CDM_BIND::UrinalysisData::eUrineColor)src.m_Color);
  if (src.HasAppearanceResult())
    dst.set_appearance((CDM_BIND::UrinalysisData::eClarityIndicator)src.m_Appearance);
  if (src.HasGlucoseResult())
    dst.set_glucose((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Glucose);
  if (src.HasKetoneResult())
    dst.set_ketone((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Ketone);
  if (src.HasBloodResult())
    dst.set_blood((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Blood);
  if (src.HasProteinResult())
    dst.set_protein((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Protein);
  if (src.HasNitriteResult())
    dst.set_nitrite((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Nitrite);
  if (src.HasBilirubinResult())
    dst.set_allocated_bilirubin(PBProperty::Unload(*src.m_Bilirubin));
  if (src.HasSpecificGravityResult())
    dst.set_allocated_specificgravity(PBProperty::Unload(*src.m_SpecificGravity));
  if (src.HasPHResult())
    dst.set_allocated_ph(PBProperty::Unload(*src.m_pH));
  if (src.HasUrobilinogenResult())
    dst.set_allocated_urobilinogen(PBProperty::Unload(*src.m_Urobilinogen));
  if (src.HasMicroscopicResult())
    dst.set_allocated_microscopic(PBPatientAssessment::Unload(*src.m_Microscopic));
}

void PBPatientAssessment::Load(const CDM_BIND::UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst)
{
  dst.SetObservationType((eUrinalysis_MicroscopicObservationType)src.observationtype());
  dst.SetEpithelialCellsResult((eUrinalysis_MicroscopicObservationAmount)src.epithelialcells());
  dst.SetCrystalsResult((eUrinalysis_MicroscopicObservationAmount)src.crystals());
  dst.SetBacteriaResult((eUrinalysis_MicroscopicObservationAmount)src.bacteria());
  dst.SetTrichomonadsResult((eUrinalysis_MicroscopicObservationAmount)src.trichomonads());
  dst.SetYeastResult((eUrinalysis_MicroscopicObservationAmount)src.yeast());
  if (src.has_redbloodcells())
    PBProperty::Load(src.redbloodcells(), dst.GetRedBloodCellsResult());
  if (src.has_whitebloodcells())
    PBProperty::Load(src.whitebloodcells(), dst.GetWhiteBloodCellsResult());
  if (src.has_casts())
    PBProperty::Load(src.casts(), dst.GetCastsResult());
}

CDM_BIND::UrinalysisMicroscopicData* PBPatientAssessment::Unload(const SEUrinalysisMicroscopic& src)
{
  CDM_BIND::UrinalysisMicroscopicData* dst = new CDM_BIND::UrinalysisMicroscopicData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SEUrinalysisMicroscopic& src, CDM_BIND::UrinalysisMicroscopicData& dst)
{
  if (src.HasObservationType())
    dst.set_observationtype((CDM_BIND::UrinalysisMicroscopicData::eObservationType)src.m_ObservationType);
  if (src.HasEpithelialCellsResult())
    dst.set_epithelialcells((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_EpithelialCells);
  if (src.HasCrystalsResult())
    dst.set_crystals((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_Crystals);
  if (src.HasBacteriaResult())
    dst.set_bacteria((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_Bacteria);
  if (src.HasTrichomonadsResult())
    dst.set_trichomonads((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_Trichomonads);
  if (src.HasYeastResult())
    dst.set_yeast((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_Yeast);
  if (src.HasRedBloodCellsResult())
    dst.set_allocated_redbloodcells(PBProperty::Unload(*src.m_RedBloodCells));
  if (src.HasWhiteBloodCellsResult())
    dst.set_allocated_whitebloodcells(PBProperty::Unload(*src.m_WhiteBloodCells));
  if (src.HasCastsResult())
    dst.set_allocated_casts(PBProperty::Unload(*src.m_Casts));
}
