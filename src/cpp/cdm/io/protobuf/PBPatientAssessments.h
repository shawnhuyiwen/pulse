/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(PatientAssessment)
CDM_BIND_DECL2(CompleteBloodCount)
CDM_BIND_DECL2(ComprehensiveMetabolicPanel)
CDM_BIND_DECL2(PulmonaryFunctionTest)
CDM_BIND_DECL2(Urinalysis)
CDM_BIND_DECL2(UrinalysisMicroscopic)

class CDM_DECL PBPatientAssessment
{
public:

  static void Serialize(const CDM_BIND::PatientAssessmentData& src, SEPatientAssessment& dst);
  static void Serialize(const SEPatientAssessment& src, CDM_BIND::PatientAssessmentData& dst);

  static bool SerializeToString(const SECompleteBloodCount& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SECompleteBloodCount& src, const std::string& filename);
  static void Load(const CDM_BIND::CompleteBloodCountData& src, SECompleteBloodCount& dst);
  static CDM_BIND::CompleteBloodCountData* Unload(const SECompleteBloodCount& src);
  static void Serialize(const CDM_BIND::CompleteBloodCountData& src, SECompleteBloodCount& dst);
  static void Serialize(const SECompleteBloodCount& src, CDM_BIND::CompleteBloodCountData& dst);

  static bool SerializeToString(const SEComprehensiveMetabolicPanel& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEComprehensiveMetabolicPanel& src, const std::string& filename);
  static void Load(const CDM_BIND::ComprehensiveMetabolicPanelData& src, SEComprehensiveMetabolicPanel& dst);
  static CDM_BIND::ComprehensiveMetabolicPanelData* Unload(const SEComprehensiveMetabolicPanel& src);
  static void Serialize(const CDM_BIND::ComprehensiveMetabolicPanelData& src, SEComprehensiveMetabolicPanel& dst);
  static void Serialize(const SEComprehensiveMetabolicPanel& src, CDM_BIND::ComprehensiveMetabolicPanelData& dst);

  static bool SerializeToString(const SEPulmonaryFunctionTest& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEPulmonaryFunctionTest& src, const std::string& filename);
  static void Load(const CDM_BIND::PulmonaryFunctionTestData& src, SEPulmonaryFunctionTest& dst);
  static CDM_BIND::PulmonaryFunctionTestData* Unload(const SEPulmonaryFunctionTest& src);
  static void Serialize(const CDM_BIND::PulmonaryFunctionTestData& src, SEPulmonaryFunctionTest& dst);
  static void Serialize(const SEPulmonaryFunctionTest& src, CDM_BIND::PulmonaryFunctionTestData& dst);

  static bool SerializeToString(const SEUrinalysis& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEUrinalysis& src, const std::string& filename);
  static void Load(const CDM_BIND::UrinalysisData& src, SEUrinalysis& dst);
  static CDM_BIND::UrinalysisData* Unload(const SEUrinalysis& src);
  static void Serialize(const CDM_BIND::UrinalysisData& src, SEUrinalysis& dst);
  static void Serialize(const SEUrinalysis& src, CDM_BIND::UrinalysisData& dst);

  static void Load(const CDM_BIND::UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst);
  static CDM_BIND::UrinalysisMicroscopicData* Unload(const SEUrinalysisMicroscopic& src);
  static void Serialize(const CDM_BIND::UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst);
  static void Serialize(const SEUrinalysisMicroscopic& src, CDM_BIND::UrinalysisMicroscopicData& dst);

};