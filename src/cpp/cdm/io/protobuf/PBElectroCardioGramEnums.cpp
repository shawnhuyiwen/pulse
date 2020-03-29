/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/ElectroCardioGram.pb.h"
POP_PROTO_WARNINGS()
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"

const std::string& eElectroCardioGram_WaveformLead_Name(eElectroCardioGram_WaveformLead m)
{
  return CDM_BIND::ElectroCardioGramWaveformData::eWaveformLead_Name((CDM_BIND::ElectroCardioGramWaveformData::eWaveformLead)m);
}