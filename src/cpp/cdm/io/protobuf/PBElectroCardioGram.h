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

  static void Load(const CDM_BIND::ElectroCardioGramData& src, SEElectroCardioGram& dst);
  static CDM_BIND::ElectroCardioGramData* Unload(const SEElectroCardioGram& src);
  static void Serialize(const CDM_BIND::ElectroCardioGramData& src, SEElectroCardioGram& dst);
  static void Serialize(const SEElectroCardioGram& src, CDM_BIND::ElectroCardioGramData& dst);

  static void Load(const CDM_BIND::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst);
  static CDM_BIND::ElectroCardioGramWaveformData* Unload(const SEElectroCardioGramWaveform& src);
  static void Serialize(const CDM_BIND::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst);
  static void Serialize(const SEElectroCardioGramWaveform& src, CDM_BIND::ElectroCardioGramWaveformData& dst);

  static void Load(const CDM_BIND::ElectroCardioGramWaveformListData& src, SEElectroCardioGramWaveformInterpolator& dst);
  static CDM_BIND::ElectroCardioGramWaveformListData* Unload(const SEElectroCardioGramWaveformInterpolator& src);
  static void Serialize(const CDM_BIND::ElectroCardioGramWaveformListData& src, SEElectroCardioGramWaveformInterpolator& dst);
  static void Serialize(const SEElectroCardioGramWaveformInterpolator& src, CDM_BIND::ElectroCardioGramWaveformListData& dst);
  static void Copy(const SEElectroCardioGramWaveformInterpolator& src, SEElectroCardioGramWaveformInterpolator& dst);

  static bool SerializeToString(const SEElectroCardioGramWaveformInterpolator& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEElectroCardioGramWaveformInterpolator& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEElectroCardioGramWaveformInterpolator& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEElectroCardioGramWaveformInterpolator& dst);

};