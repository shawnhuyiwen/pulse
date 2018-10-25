/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/physiology/SECardiovascularSystem.h"

// Keep enums in sync with appropriate schema/cdm/CompartmentEnums.proto file !!
enum class eElectroCardioGram_WaveformLead { NullLead = 0, 
                                             Lead1, 
                                             Lead2,
                                             Lead3,
                                             Lead4,
                                             Lead5,
                                             Lead6,
                                             Lead7,
                                             Lead8,
                                             Lead9,
                                             Lead10,
                                             Lead11,
                                             Lead12 };
extern const std::string& eElectroCardioGram_WaveformLead_Name(eElectroCardioGram_WaveformLead m);


class CDM_DECL SEElectroCardioGramWaveform : public Loggable
{
  friend class PBElectroCardioGram;//friend the serialization class
public:

  SEElectroCardioGramWaveform(Logger* logger);
  virtual ~SEElectroCardioGramWaveform();

  virtual void Clear();// Deletes all members

  virtual bool HasLeadNumber() const;
  virtual eElectroCardioGram_WaveformLead  GetLeadNumber() const;
  virtual void SetLeadNumber(eElectroCardioGram_WaveformLead n);
  virtual void InvalidateLeadNumber();

  virtual eHeartRhythm GetRhythm() const;
  virtual void SetRhythm(eHeartRhythm name);

  virtual bool HasData() const;
  virtual SEFunctionElectricPotentialVsTime& GetData();
  virtual const SEFunctionElectricPotentialVsTime* GetData() const;

  virtual bool HasTimeStep() const;
  virtual SEScalarTime& GetTimeStep();
  virtual double GetTimeStep(const TimeUnit& unit) const;

  virtual std::vector<unsigned int>& GetActiveIndicies() { return m_ActiveIndicies; }

protected:

  eElectroCardioGram_WaveformLead          m_LeadNumber;
  eHeartRhythm                             m_Rhythm;
  SEScalarTime*                            m_TimeStep;
  SEFunctionElectricPotentialVsTime*       m_Data;
  std::vector<unsigned int>                m_ActiveIndicies;
};