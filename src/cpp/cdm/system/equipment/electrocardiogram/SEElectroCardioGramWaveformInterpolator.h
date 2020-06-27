/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"

class CDM_DECL SEElectroCardioGramWaveformInterpolator : public Loggable
{
  friend class PBElectroCardioGram;//friend the serialization class
public:

  SEElectroCardioGramWaveformInterpolator(Logger* logger);
  virtual ~SEElectroCardioGramWaveformInterpolator();

  virtual void Clear();// Deletes all members
  virtual void Copy(const SEElectroCardioGramWaveformInterpolator& src);

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, const SEScalarTime* timeStep = nullptr);
  bool SerializeFromFile(const std::string& filename, const SEScalarTime* timeStep = nullptr);

  virtual void Interpolate(const SEScalarTime& timeStep);
  virtual bool StartNewCycle(eHeartRhythm rhythm);
  virtual void CalculateWaveformsElectricPotential();

  // These are where the interpolator will put the interpolated electric potential data for each lead when you call CalculateWaveformsElectricPotential
  // You must have a waveform for the lead of the current rhythm for these scalars to be populated
  virtual bool CanInterpolateLeadPotential(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm) const;
  virtual void SetLeadElectricPotential(eElectroCardioGram_WaveformLead lead, SEScalarElectricPotential& ep);


  virtual bool HasWaveform(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm) const;
  virtual SEElectroCardioGramWaveform& GetWaveform(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm);
  virtual const SEElectroCardioGramWaveform* GetWaveform(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm) const;
  virtual void RemoveWaveform(eElectroCardioGram_WaveformLead lead, eHeartRhythm rhythm);

protected:

  virtual void Interpolate(SEElectroCardioGramWaveform& waveform, const SEScalarTime& timeStep);
  virtual bool InterpolateToTime(SEFunctionElectricPotentialVsTime& waveform, std::vector<double>& newTime, const TimeUnit& unit);
  
  std::map<eElectroCardioGram_WaveformLead, SEScalarElectricPotential*> m_Leads;
  std::map<eElectroCardioGram_WaveformLead, std::map<eHeartRhythm, SEElectroCardioGramWaveform*>> m_Waveforms;
};