/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cdm/ElectroCardioGram.pb.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"

const std::string& eElectroCardioGram_WaveformLead_Name(eElectroCardioGram_WaveformLead m)
{
  return cdm::eElectroCardioGram_WaveformLead_Name((cdm::eElectroCardioGram_WaveformLead)m);
}