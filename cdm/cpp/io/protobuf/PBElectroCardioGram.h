/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(ElectroCardioGram)
CDM_BIND_DECL2(ElectroCardioGramWaveform)
CDM_BIND_DECL(ElectroCardioGramWaveformListData)
class SEElectroCardioGramWaveformInterpolator;

class CDM_DECL PBElectroCardioGram
{
public:

  static void Load(const cdm::ElectroCardioGramData& src, SEElectroCardioGram& dst);
  static cdm::ElectroCardioGramData* Unload(const SEElectroCardioGram& src);
  static void Serialize(const cdm::ElectroCardioGramData& src, SEElectroCardioGram& dst);
  static void Serialize(const SEElectroCardioGram& src, cdm::ElectroCardioGramData& dst);

  static void Load(const cdm::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst);
  static cdm::ElectroCardioGramWaveformData* Unload(const SEElectroCardioGramWaveform& src);
  static void Serialize(const cdm::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst);
  static void Serialize(const SEElectroCardioGramWaveform& src, cdm::ElectroCardioGramWaveformData& dst);

  static void Load(const cdm::ElectroCardioGramWaveformListData& src, SEElectroCardioGramWaveformInterpolator& dst);
  static cdm::ElectroCardioGramWaveformListData* Unload(const SEElectroCardioGramWaveformInterpolator& src);
  static void Serialize(const cdm::ElectroCardioGramWaveformListData& src, SEElectroCardioGramWaveformInterpolator& dst);
  static void Serialize(const SEElectroCardioGramWaveformInterpolator& src, cdm::ElectroCardioGramWaveformListData& dst);
  static void Copy(const SEElectroCardioGramWaveformInterpolator& src, SEElectroCardioGramWaveformInterpolator& dst);

  static bool SerializeToString(const SEElectroCardioGramWaveformInterpolator& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEElectroCardioGramWaveformInterpolator& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEElectroCardioGramWaveformInterpolator& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEElectroCardioGramWaveformInterpolator& dst, SerializationMode m);

};