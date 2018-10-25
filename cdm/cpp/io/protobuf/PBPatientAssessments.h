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

  static void Serialize(const cdm::PatientAssessmentData& src, SEPatientAssessment& dst);
  static void Serialize(const SEPatientAssessment& src, cdm::PatientAssessmentData& dst);

  static bool SerializeToString(const SECompleteBloodCount& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SECompleteBloodCount& src, const std::string& filename, SerializationMode m);
  static void Load(const cdm::CompleteBloodCountData& src, SECompleteBloodCount& dst);
  static cdm::CompleteBloodCountData* Unload(const SECompleteBloodCount& src);
  static void Serialize(const cdm::CompleteBloodCountData& src, SECompleteBloodCount& dst);
  static void Serialize(const SECompleteBloodCount& src, cdm::CompleteBloodCountData& dst);

  static bool SerializeToString(const SEComprehensiveMetabolicPanel& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEComprehensiveMetabolicPanel& src, const std::string& filename, SerializationMode m);
  static void Load(const cdm::ComprehensiveMetabolicPanelData& src, SEComprehensiveMetabolicPanel& dst);
  static cdm::ComprehensiveMetabolicPanelData* Unload(const SEComprehensiveMetabolicPanel& src);
  static void Serialize(const cdm::ComprehensiveMetabolicPanelData& src, SEComprehensiveMetabolicPanel& dst);
  static void Serialize(const SEComprehensiveMetabolicPanel& src, cdm::ComprehensiveMetabolicPanelData& dst);

  static bool SerializeToString(const SEPulmonaryFunctionTest& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEPulmonaryFunctionTest& src, const std::string& filename, SerializationMode m);
  static void Load(const cdm::PulmonaryFunctionTestData& src, SEPulmonaryFunctionTest& dst);
  static cdm::PulmonaryFunctionTestData* Unload(const SEPulmonaryFunctionTest& src);
  static void Serialize(const cdm::PulmonaryFunctionTestData& src, SEPulmonaryFunctionTest& dst);
  static void Serialize(const SEPulmonaryFunctionTest& src, cdm::PulmonaryFunctionTestData& dst);

  static bool SerializeToString(const SEUrinalysis& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEUrinalysis& src, const std::string& filename, SerializationMode m);
  static void Load(const cdm::UrinalysisData& src, SEUrinalysis& dst);
  static cdm::UrinalysisData* Unload(const SEUrinalysis& src);
  static void Serialize(const cdm::UrinalysisData& src, SEUrinalysis& dst);
  static void Serialize(const SEUrinalysis& src, cdm::UrinalysisData& dst);

  static void Load(const cdm::UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst);
  static cdm::UrinalysisMicroscopicData* Unload(const SEUrinalysisMicroscopic& src);
  static void Serialize(const cdm::UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst);
  static void Serialize(const SEUrinalysisMicroscopic& src, cdm::UrinalysisMicroscopicData& dst);

};