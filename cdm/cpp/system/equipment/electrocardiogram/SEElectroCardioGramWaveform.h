/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(ElectroCardioGramWaveformData)
PROTO_PUSH
#include "bind/cdm/PhysiologyEnums.pb.h"
#include "bind/cdm/ElectroCardioGramEnums.pb.h"
PROTO_POP

class CDM_DECL SEElectroCardioGramWaveform : public Loggable
{
public:

  SEElectroCardioGramWaveform(Logger* logger);
  virtual ~SEElectroCardioGramWaveform();

  virtual void Clear();// Deletes all members

  static void Load(const cdm::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst);
  static cdm::ElectroCardioGramWaveformData* Unload(const SEElectroCardioGramWaveform& src);
protected:
  static void Serialize(const cdm::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst);
  static void Serialize(const SEElectroCardioGramWaveform& src, cdm::ElectroCardioGramWaveformData& dst);

public:  
  virtual bool HasLeadNumber() const;
  virtual cdm::eElectroCardioGram_WaveformLead  GetLeadNumber() const;
  virtual void SetLeadNumber(cdm::eElectroCardioGram_WaveformLead n);
  virtual void InvalidateLeadNumber();

  virtual cdm::eHeartRhythm GetRhythm() const;
  virtual void SetRhythm(cdm::eHeartRhythm name);

  virtual bool HasData() const;
  virtual SEFunctionElectricPotentialVsTime& GetData();
  virtual const SEFunctionElectricPotentialVsTime* GetData() const;

  virtual bool HasTimeStep() const;
  virtual SEScalarTime& GetTimeStep();
  virtual double GetTimeStep(const TimeUnit& unit) const;

  virtual std::vector<unsigned int>& GetActiveIndicies() { return m_ActiveIndicies; }

protected:

  cdm::eElectroCardioGram_WaveformLead     m_LeadNumber;
  cdm::eHeartRhythm                        m_Rhythm;
  SEScalarTime*                            m_TimeStep;
  SEFunctionElectricPotentialVsTime*       m_Data;
  std::vector<unsigned int>                m_ActiveIndicies;
};