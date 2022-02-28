/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/ElectroCardioGram.pb.h"
POP_PROTO_WARNINGS
#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"

const std::string& eElectroCardioGram_WaveformLead_Name(eElectroCardioGram_WaveformLead n)
{
  return CDM_BIND::ElectroCardioGramWaveformData::eWaveformLead_Name((CDM_BIND::ElectroCardioGramWaveformData::eWaveformLead)n);
}

const std::string& eElectroCardioGram_WaveformType_Name(eElectroCardioGram_WaveformType t)
{
  return CDM_BIND::ElectroCardioGramWaveformData::eWaveformType_Name((CDM_BIND::ElectroCardioGramWaveformData::eWaveformType)t);
}