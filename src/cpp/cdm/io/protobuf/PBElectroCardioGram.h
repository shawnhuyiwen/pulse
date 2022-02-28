/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(ElectroCardioGram)
CDM_BIND_DECL2(ElectroCardioGramWaveform)
CDM_BIND_DECL(ElectroCardioGramWaveformListData)

class CDM_DECL PBElectroCardioGram
{
public:

  static void Load(const CDM_BIND::ElectroCardioGramData& src, SEElectroCardioGram& dst);
  static CDM_BIND::ElectroCardioGramData* Unload(const SEElectroCardioGram& src);
  static void Serialize(const CDM_BIND::ElectroCardioGramData& src, SEElectroCardioGram& dst);
  static void Serialize(const SEElectroCardioGram& src, CDM_BIND::ElectroCardioGramData& dst);
  static void Copy(const SEElectroCardioGram& src, SEElectroCardioGram& dst);

  static bool SerializeToString(const SEElectroCardioGram& src, std::string& output, eSerializationFormat m);
  static bool SerializeToFile(const SEElectroCardioGram& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEElectroCardioGram& dst, eSerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEElectroCardioGram& dst);

  static void Load(const CDM_BIND::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst);
  static CDM_BIND::ElectroCardioGramWaveformData* Unload(const SEElectroCardioGramWaveform& src);
  static void Serialize(const CDM_BIND::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst);
  static void Serialize(const SEElectroCardioGramWaveform& src, CDM_BIND::ElectroCardioGramWaveformData& dst);
};
